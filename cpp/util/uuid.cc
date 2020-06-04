#include "util/uuid.h"

#include "cassandra.h"

#include "util/includes.h"
#include "util/strings.h"
#include "util/random.h"

namespace vectorbook {

namespace {

Option<string> CassandraTimeUUID() {
  CassUuidGen* uuid_gen = cass_uuid_gen_new();

	CassUuid uuid;
	// cass_uuid_from_string("550e8400-e29b-41d4-a716-446655440000", &uuid);
	cass_uuid_gen_time(uuid_gen, &uuid);
  cass_uuid_gen_free(uuid_gen);


	/* Extract timestamp and version */
	cass_uint64_t timestamp = cass_uuid_timestamp(uuid);
	cass_uint8_t version = cass_uuid_version(uuid);

	/* Get string representation of the UUID */
	char uuid_str[CASS_UUID_STRING_LENGTH];
	cass_uuid_string(uuid, uuid_str);
	string result(uuid_str);
	return result;
}

}

Option<string> FreshTimeUUID() {
	return CassandraTimeUUID();
}

} // namespace vectorbook
