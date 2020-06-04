#include "data/globals.h"

#include "util/includes.h"

namespace vectorbook {
namespace data {

unique_ptr<UserMap> user_map() { return RedisProtoMap<User>("user_map"); }
unique_ptr<BroadcastMap> broadcast_map() {
  return RedisProtoMap<Broadcast>("broadcast_map");
}
unique_ptr<StringSet> broadcast_set() { return RedisSet("broadcast_set"); }
unique_ptr<ActionMap> action_map() {
  return RedisProtoMap<Action>("action_map");
}
unique_ptr<LinkMap> resource_map() {
  return RedisProtoMap<Resource>("resource_map");
}

unique_ptr<StringMap> user_name_map() {
  return RedisStringMap("user_name_map");
}
unique_ptr<StringMap> email_map() { return RedisStringMap("email_map"); }

unique_ptr<StringQueue> unsorted_broadcast_queue() {
  return RedisStringQueue("unsorted_broadcast_queue");
}
unique_ptr<StringQueue> unsorted_relay_queue() {
  return RedisStringQueue("unsorted_relay_queue");
}
unique_ptr<StringQueue> unsorted_action_queue() {
  return RedisStringQueue("unsorted_action_queue");
}
unique_ptr<StringQueue> unsorted_obscenity_queue() {
  return RedisStringQueue("unsorted_obscenity_queue");
}

unique_ptr<StringQueue> recommendation_queue(const string &user) {
  return RedisStringQueue(StrCat("recommendation_queue:", user));
}

unique_ptr<StringQueue> broadcast_history(const string &user) {
  return RedisStringQueue(StrCat("broadcast_history:", user));
}

unique_ptr<StringMap> emailtoken_map() {
  return RedisStringMap("emailtoken_map");
}
unique_ptr<StringMap> cookietoken_map() {
  return RedisStringMap("cookietoken_map");
}

unique_ptr<StringMap> url_resource_map() {
  return RedisStringMap("url_resource_map");
}

unique_ptr<StringMap> applytoken_map() {
  return RedisStringMap("applytoken_map");
}

unique_ptr<ApplicationMap> application_map() {
  return RedisProtoMap<Application>("application_map");
}

unique_ptr<InvitationMap> invitation_map() {
  return RedisProtoMap<Invitation>("invitation_map");
}
unique_ptr<StringSet> user_pending_invitations(const string &user_id) {
  return RedisSet(StrCat("user_pending_invitations:", user_id));
}
unique_ptr<StringQueue> user_chronological_invitations(const string &user_id) {
  return RedisStringQueue(StrCat("user_chronological_invitations:", user_id));
}
unique_ptr<StringSet> email_optout_set() {
  return RedisSet("email_optout_set");
}

unique_ptr<StringMap> emailtoken_invitation_map() {
  return RedisStringMap("emailtoken_invitation_map");
}

} // namespace data
} // namespace vectorbook
