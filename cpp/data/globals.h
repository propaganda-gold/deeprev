#ifndef data_globals_h
#define data_globals_h
#include "coln/map.h"
#include "data/api.h"
#include "data/vectorbook.pb.h"
#include "data/map.h"
#include "data/seq.h"
#include "data/set.h"
#include "neural/training_config.pb.h"
#include "util/includes.h"
#include "util/smtp.pb.h"

// TODO(greg) None of these calls should fail.

namespace vectorbook {
// Maps.
using UserMap = Map<string, User>;
using BroadcastMap = Map<string, Broadcast>;
using LinkMap = Map<string, Resource>;
using ActionMap = Map<string, Action>;
using InvitationMap = Map<string, Invitation>;
using ApplicationMap = Map<string, Application>;

// Queues.
using StringQueue = Queue<string>;
using StringSet = Set<string>;

namespace data {

//
// Users

// key: user_id, value: user_pbtxt
unique_ptr<UserMap> user_map();

// key: user_name, value_user_id
unique_ptr<StringMap> user_name_map();

// key: email, value: user_id
unique_ptr<StringMap> email_map();

// key: emailtoken, value: email
// Note: To get from emailtoken to user_id, one must first get the email.
// Note: An email can be in the email token map whether the email corresponds
// to a registered user or not.
unique_ptr<StringMap> emailtoken_map();

// key: cookietoken, value: user_id
unique_ptr<StringMap> cookietoken_map();

//
// Broadcasts
unique_ptr<BroadcastMap> broadcast_map();

// Contains the id's of broadcast_map.
// Note: This set is redundant, and is used because I think Redis only lets you
// pick a random key from a *set*, not a *map*.
unique_ptr<StringSet> broadcast_set();

//
// Resources
unique_ptr<LinkMap> resource_map();

// 
unique_ptr<StringMap> url_resource_map();

//
// Actions
unique_ptr<ActionMap> action_map();

//
// Unsorted
unique_ptr<StringQueue> unsorted_broadcast_queue();
unique_ptr<StringQueue> unsorted_relay_queue();
unique_ptr<StringQueue> unsorted_action_queue();
unique_ptr<StringQueue> unsorted_obscenity_queue();

//
// Feeds
unique_ptr<StringQueue> recommendation_queue(const string& user);

//
// History
unique_ptr<StringQueue> broadcast_history(const string& user);

//
// Access control
unique_ptr<StringSet> group_members(const string& group_id);
unique_ptr<StringSet> group_invitations(const string& group_id);

//
//
//
//
// UNUSED
//
// Invitations
unique_ptr<InvitationMap> invitation_map();
unique_ptr<StringSet> user_pending_invitations(const string &user_id);
unique_ptr<StringQueue> user_chronological_invitations(const string& user_id);
unique_ptr<StringSet> email_optout_set();
unique_ptr<StringMap> emailtoken_invitation_map();

//
// Applications
unique_ptr<ApplicationMap> application_map();
unique_ptr<StringMap> applytoken_map();


} // namespace data
} // namespace vectorbook
#endif /* data_globals_h */
