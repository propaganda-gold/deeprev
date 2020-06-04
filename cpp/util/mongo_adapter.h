#ifndef util_mongo_adapter_h
#define util_mongo_adapter_h
#include "util/includes.h"

#include <stdio.h>
#include <iostream>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

namespace vectorbook {
namespace mongodb {

using BsonDocument = bsoncxx::document::value;

Void Initialize(const string &uri_string);
Void CleanUp();

Option<BsonDocument> FindById(const string &database, const string &collection,
                              const string &id);

} // namespace mongodb
} // namespace vectorbook
#endif /* util_mongo_adapter_h */
