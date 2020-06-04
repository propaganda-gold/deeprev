#ifndef data_ops_invitation_h
#define data_ops_invitation_h

#include "handlers/api.h"
#include "util/includes.h"

namespace vectorbook {
Boolean EmailAlreadyRegistered(const string &email);
Option<Invitation> CreateInvitation(const string &sponsor_id,
                                    const string &candidate_email);
Option<RepeatedPtrField<Invitation>>
InvitationListForUser(const string &user, const string &filter);
// This does not create a user, but only does the invitation book-keeping.
Void AcceptInvitation(const string &invitation);
Void DeletePendingInvitation(const string &user, const string &invitation);
Void AddEmailOptout(const string &email);

} // namespace vectorbook
#endif /* data_ops_invitation_h */
