#include "util/cassandra_logging.h"

#include "cassandra.h"
#include "util/includes.h"
#include "util/protos.h"

namespace vectorbook {

Void WriteToCassandra(const string &table_name, const string &uuid_string,
                      const string &pbtxt) {
  CassCluster *cluster = cass_cluster_new();
  CassSession *session = cass_session_new();

  // TODO(greg) set this to a server.
  cass_cluster_set_contact_points(cluster, "127.0.0.1");
  // TODO(greg) Should the version be 4?
  cass_cluster_set_protocol_version(cluster, 3);

  CassFuture *connect_future = cass_session_connect(session, cluster);

  CassUuid cass_uuid;
  cass_uuid_from_string(uuid_string.c_str(), &cass_uuid);

  if (cass_future_error_code(connect_future) == CASS_OK) {
    CassFuture *close_future = NULL;

    /* Build statement and execute query */
    const string query =
        StrCat("INSERT INTO ", table_name, "(id, pbtxt, creationTime) values(?, ?, toTimeStamp(now()))");
    CassStatement *statement = cass_statement_new(query.c_str(), 2);
    cass_statement_bind_uuid(statement, 0, cass_uuid);
    cass_statement_bind_string(statement, 1, pbtxt.c_str());

    CassFuture *result_future = cass_session_execute(session, statement);

    if (cass_future_error_code(result_future) == CASS_OK) {
      return Ok();
    } else {
      // Failed to write.
      const char *message;
      size_t message_length;
      cass_future_error_message(result_future, &message, &message_length);
      fprintf(stderr, "Unable to run query: '%.*s'\n", (int)message_length,
              message);
    }
  } else {
		Print_msg("Failed to connect.");
    // Failed to connect.
  }

  // Clean up.

  cass_future_free(connect_future);
  cass_session_free(session);
  cass_cluster_free(cluster);

  return Ok();
}

Option<string> GetFromCassandra(const string &table_name,
                                const string &uuid_string) {
  Print_line();
	Print_value(uuid_string);
  CassCluster *cluster = cass_cluster_new();
  CassSession *session = cass_session_new();

  // TODO(greg) set this to a server.
  cass_cluster_set_contact_points(cluster, "127.0.0.1");
  // TODO(greg) Should the version be 4?
  cass_cluster_set_protocol_version(cluster, 3);

  CassFuture *connect_future = cass_session_connect(session, cluster);

  CassUuid cass_uuid;
  cass_uuid_from_string(uuid_string.c_str(), &cass_uuid);

	string final_result;
	bool success = false;

  Print_line();
  if (cass_future_error_code(connect_future) == CASS_OK) {
    Print_line();
    CassFuture *close_future = NULL;

    /* Build statement and execute query */
    const string query =
        StrCat("SELECT pbtxt FROM ", table_name, " WHERE id = ?");
    CassStatement *statement = cass_statement_new(query.c_str(), 1);
    cass_statement_bind_uuid(statement, 0, cass_uuid);
    //    cass_statement_bind_string(statement, 1, pbtxt.c_str());

    CassFuture *result_future = cass_session_execute(session, statement);

    if (cass_future_error_code(result_future) == CASS_OK) {
      Print_line();
      /* Retrieve result set and get the first row */
      const CassResult *result = cass_future_get_result(result_future);
      const CassRow *row = cass_result_first_row(result);


      if (row) {
        Print_line();
        const CassValue *value = cass_row_get_column_by_name(row, "pbtxt");

        const char *pbtxt_string;
        size_t pbtxt_string_length;
        cass_value_get_string(value, &pbtxt_string, &pbtxt_string_length);
        printf("pbtxt_string: '%.*s'\n", (int)pbtxt_string_length,
               pbtxt_string);

				final_result = string(pbtxt_string, pbtxt_string_length);
				success = true;
				Print_value(final_result);
      }

      cass_result_free(result);
    } else {
      Print_line();
      // Failed to read.
      const char *message;
      size_t message_length;
      cass_future_error_message(result_future, &message, &message_length);
      fprintf(stderr, "Unable to run query: '%.*s'\n", (int)message_length,
              message);
    }
  } else {
    Print_line();
    // Failed to connect.
    Print_msg("Failed to connect!");
  }

  // Clean up.

  cass_future_free(connect_future);
  cass_session_free(session);
  cass_cluster_free(cluster);

	if (success) {
		return final_result;
	} else {
		return Null();
	}
}

} // namespace vectorbook
