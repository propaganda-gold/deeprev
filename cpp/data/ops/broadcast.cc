#include "data/ops/broadcast.h"

#include "data/globals.h"
#include "data/ops/resource.h"
#include "data/ops/time.h"
#include "util/cassandra_logging.h"
#include "util/includes.h"
#include "util/mongo_adapter.h"
#include "util/protos.h"
#include "util/re.h"
#include "util/uuid.h"

namespace vectorbook {

using BroadcastTokens = RepeatedPtrField<Broadcast::Token>;

namespace {

bool IsUrl(const string &token) {
  string http_regex =
      R"(^(http:\/\/www\.|https:\/\/www\.|http:\/\/|https:\/\/)?[a-z0-9]+([\-\.]{1}[a-z0-9]+)*\.[a-z]{2,5}(:[0-9]{1,5})?(\/.*)?$)";
  vector<string> matches = re::FindAllMatches(token, http_regex);
  return matches.size() > 0;
}

// If 'url' is a url, this will return a string-valued id to a Resource.
Option<string> OptionallyResolveLink(const string &url) {
  auto is_url = IsUrl(url);
  if (is_url) {
    auto id = ResourceIdFromUrl(url);
    ASSERT_OK(id);
    return *id;
  } else {
    return Null();
  }
}

Option<Broadcast::Token> CreateToken(const string &raw_token) {
  Broadcast::Token result;
  result.set_word(raw_token);

  auto link_opt = OptionallyResolveLink(raw_token);
  ASSERT_OK(link_opt);
  if (link_opt.has_value()) {
    result.set_resource(*link_opt);
  }
  return result;
}

Option<BroadcastTokens> TokenizeTextarea(const string &textarea) {
  BroadcastTokens result;
  auto parts = Split(textarea);
  for (const string &raw_token : parts) {
    auto token = CreateToken(raw_token);
    ASSERT_OK(token);
    *result.Add() = *token;
  }
  return result;
}

Option<Broadcast::ReferenceInfo> BroadcastToInfo(const Broadcast &broadcast) {
  Broadcast::ReferenceInfo result;
  result.set_id(broadcast.id());
  if (broadcast.rendering_data().has_resource()) {
    *result.mutable_resource() = broadcast.rendering_data().resource();
  }
  *result.mutable_basic_data() = broadcast.basic_data();
  auto producer_user =
      data::user_map()->const_get(broadcast.basic_data().producer());
  ASSERT_EXISTS(producer_user);
  result.set_producer_name(producer_user->user_name());
  return result;
}

Option<Broadcast::RenderingData>
CreateRenderingData(const Broadcast::BasicData &basic_data) {
  Broadcast::RenderingData result;
  auto token_seq = TokenizeTextarea(basic_data.textarea());
  ASSERT_EXISTS(token_seq);
  *result.mutable_token() = *token_seq;

  for (const auto &token : *token_seq) {
    if (token.has_resource()) {
      auto resource = data::resource_map()->const_get(token.resource());
      ASSERT_EXISTS(resource);
      *result.mutable_resource() = *resource;
      break;
    }
  }

  result.set_time_created(CurrentTimeInfo());

  if (basic_data.has_reference()) {
    auto reference = data::broadcast_map()->const_get(basic_data.reference());
    ASSERT_EXISTS(reference);
    auto reference_info = BroadcastToInfo(*reference);
    ASSERT_EXISTS(reference_info);
    *result.mutable_reference_info() = *reference_info;
  }

  Print_value(basic_data.producer());
  //  auto producer_user = data::user_map()->const_get(basic_data.producer());
  //  ASSERT_EXISTS(producer_user);
  //  result.set_producer_user_name(producer_user->user_name());

  const string database_name = "test";
  const string collection_name = "users";
  const string id = basic_data.producer();
  Print_value(database_name);
  Print_value(collection_name);
  Print_value(id);

  auto document = mongodb::FindById("test", "users", basic_data.producer());
  ASSERT_EXISTS(document);

	std::cout << bsoncxx::to_json(*document) << "\n";

  result.set_producer_user_name("MrHack");

  return result;
}

} // namespace

Option<Broadcast> BuildBroadcast(const Broadcast::BasicData &basic_data) {
  Broadcast result;

  auto uuid = FreshTimeUUID();
  ASSERT_OK(uuid);
  result.set_id(*uuid);
  *result.mutable_basic_data() = basic_data;

  auto rendering_data = CreateRenderingData(basic_data);
  ASSERT_OK(rendering_data);

  if (basic_data.has_producer()) { // producer user name
                                   //		auto producer_id =
    // data::user_name_map()->const_get(basic_data.producer());
    //		ASSERT_EXISTS(producer_id);
    //    auto producer_user =
    //    data::user_map()->const_get(basic_data.producer());
    //    ASSERT_EXISTS(producer_user);
  }

  *result.mutable_rendering_data() = *rendering_data;

  ASSERT_SUCCEEDS(data::broadcast_map()->put(*uuid, result));
  ASSERT_SUCCEEDS(data::broadcast_set()->insert(*uuid));
  ASSERT_SUCCEEDS(data::unsorted_broadcast_queue()->append(*uuid));
  ASSERT_SUCCEEDS(
      data::broadcast_history(basic_data.producer())->append(*uuid));
  auto uq = data::unsorted_broadcast_queue();
  auto size = uq->size();
  ASSERT_OK(size);

  //  auto json = ProtoToJsonString(result);
  //  ASSERT_OK(json);
  //  Print_value(*json);
  ASSERT_SUCCEEDS(
      WriteToCassandra("keyspace1.broadcast", *uuid, result.DebugString()));
  return result;
}

} // namespace vectorbook
