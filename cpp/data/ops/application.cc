#include "data/ops/application.h"

#include "data/globals.h"
#include "data/ops/time.h"
#include "util/includes.h"
#include "util/uuid.h"

namespace vectorbook {

Option<Application> CreateApplication(const string &email,
                                      const string &textarea) {
  Application result;
	auto id = FreshTimeUUID(); ASSERT_EXISTS(id);
  result.set_id(*id);
  result.set_email(email);
  result.set_textarea(textarea);
	result.set_time_created(CurrentTimeInfo());

  ASSERT_SUCCEEDS(data::application_map()->put(result.id(), result));
  return result;
}

Boolean MarkApplicationVerified(const string &id) {
	auto application = data::application_map()->const_get(id);
	if (!application.has_value()) {
		return false;
	}

	auto copy = *application;
	copy.set_verified(true);
  ASSERT_SUCCEEDS(data::application_map()->put(copy.id(), copy));
	return true;
}

} // namespace vectorbook
