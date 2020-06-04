#include "util/mongo_adapter.h"

#include "server/options.h"
#include "util/includes.h"

namespace vectorbook {
namespace mongodb {

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;

namespace {

struct MongoState {
	unique_ptr<mongocxx::instance> instance_;
	unique_ptr<mongocxx::uri> uri_;
	unique_ptr<mongocxx::client> client_;
	string uri_string_;
};

MongoState* mutable_mongo_state() {
	static MongoState* static_state = new MongoState();
	return static_state;
}

}  // namespace

Void Initialize(const string &uri_string) {
  mongocxx::options::client client_options;
  mongocxx::options::tls tls_options;

	auto state = mutable_mongo_state();
	state->instance_.reset(new mongocxx::instance());
	state->uri_.reset(new mongocxx::uri(uri_string));
	state->client_.reset(new mongocxx::client());
	state->uri_string_ = uri_string;

  return Ok();
}

Void CleanUp() {
  // Nothing to clean up for mongo?
  return Ok();
}

Option<BsonDocument> FindById(const string &database, const string &collection,
                              const string &id) {
	MongoState* state = mutable_mongo_state();

  mongocxx::uri uri(state->uri_string_);
  mongocxx::client client(uri);
  mongocxx::database db = client[database];

//  mongocxx::database db = (*state->client_)[database];
  mongocxx::collection coll = db[collection];
  core::v1::optional<bsoncxx::v_noabi::document::value> cursor = coll.find_one(
      document{} << "_id" << bsoncxx::oid{bsoncxx::stdx::string_view{id}}
                 << finalize);
	if (cursor) {
		return *cursor;
	} else {
		return Null();
	}
}

} // namespace mongodb
} // namespace vectorbook
