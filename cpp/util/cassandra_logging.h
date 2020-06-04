#ifndef util_cassandra_logging_h
#define util_cassandra_logging_h
#include "util/includes.h"

namespace vectorbook {

// Note: 'table_name' contains the keyspace, e.g., 'mykeyspace.mytable'.
Void WriteToCassandra(const string &table_name, const string &uuid,
                      const string &pbtxt);

Option<string> GetFromCassandra(const string &table_name,
                                const string &uuid_string);

} // namespace vectorbook
#endif /* util_cassandra_logging_h */
