#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "util/includes.h"
#include <stdio.h>

#include <iostream>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::stdx::string_view;

using namespace vectorbook;

ABSL_FLAG(std::string, uri, "", "");

void HandleView(const bsoncxx::v_noabi::document::view &view) {
  std::cout << bsoncxx::to_json(view) << "\n";
}

Void Main() {
  std::string uri_string = absl::GetFlag(FLAGS_uri);
  ASSERT(!uri_string.empty());

  mongocxx::options::client client_options;
  mongocxx::options::tls tls_options;

  mongocxx::instance instance{}; // This should be done only once.
  mongocxx::uri uri(uri_string);
  mongocxx::client client(uri);

  mongocxx::database db = client["test"];
  mongocxx::collection coll = db["users"];

  if (false) {
    auto cursor = coll.find({});
    for (auto doc : cursor) {
      std::cout << bsoncxx::to_json(doc) << "\n";
    }
  }

  {
    auto cursor =
        coll.find_one(document{} << "_id"
                                 << bsoncxx::oid{bsoncxx::stdx::string_view{
                                        "5e9b376c4fc9e1541d0e58fb"}}
                                 << finalize);
    if (cursor) {
      HandleView(*cursor);
    }
  }

  return Ok();
}

#include "util/main_incl.h"
