#include "data/ops/invitation.h"

#include "data/globals.h"
#include "data/ops/api.pb.h"
#include "data/ops/emailtoken.h"
#include "data/ops/time.h"
#include "handlers/http.pb.h"
#include "util/uuid.h"

namespace vectorbook {

Boolean EmailAlreadyRegistered(const string &email) {
  auto user = data::email_map()->const_get(email);
  ASSERT_OK(user);
  return user.has_value();
}

Option<Invitation> CreateInvitation(const string &sponsor_id,
                                    const string &candidate_email) {

  auto email_optout = data::email_optout_set()->contains(candidate_email);
  ASSERT_EXISTS(email_optout);

  if (*email_optout) {
    // TODO(greg) Put a status here?
    return Null();
  }

  Invitation result;
  auto invitation_id = FreshTimeUUID();
  ASSERT_EXISTS(invitation_id);
  result.set_id(*invitation_id);
  result.set_sponsor_id(sponsor_id);
  result.set_candidate_email(candidate_email);
	result.set_time_created(CurrentTimeInfo());
  ASSERT_SUCCEEDS(data::invitation_map()->put(result.id(), result));
  ASSERT_SUCCEEDS(
      data::user_pending_invitations(sponsor_id)->insert(result.id()));
  ASSERT_SUCCEEDS(
      data::user_chronological_invitations(sponsor_id)->append(result.id()));
  return result;
}

Option<RepeatedPtrField<Invitation>>
InvitationListForUser(const string &user, const string &filter) {
  RepeatedPtrField<Invitation> buffer;
  for (auto id : *data::user_chronological_invitations(user)) {
    ASSERT_OK(id);

    auto invitation = data::invitation_map()->const_get(*id);
    ASSERT_OK(invitation);
    if (!invitation.has_value()) {
      // ERROR;
      continue;
    }
    *buffer.Add() = *invitation;
  }

  return buffer;
}

Void AcceptInvitation(const string &invitation_id) {
  auto invitation = data::invitation_map()->const_get(invitation_id);
  ASSERT_EXISTS(invitation);
  auto invitation_copy = *invitation;
  // invitation_copy.set_accepted("true"); // TODO(greg) add time
  ASSERT_SUCCEEDS(
      data::invitation_map()->put(invitation_copy.id(), invitation_copy));
  ASSERT_SUCCEEDS(data::user_pending_invitations(invitation->sponsor_id())
                      ->erase(invitation->candidate_email()));
  return Ok();
}

// 1. Mark the Invitation as accepted.
// 2. Remove this user from the sponsor's "pending set".
Void DeletePendingInvitation(const string &user, const string &invitation_id) {
  return Null();
}

Void AddEmailOptout(const string &email) {
  auto status = data::email_optout_set()->insert(email);
  ASSERT_OK(status);
  return Ok();
}

} // namespace vectorbook
