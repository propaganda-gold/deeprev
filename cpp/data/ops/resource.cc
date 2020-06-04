#include "data/ops/resource.h"

#include "data/globals.h"
#include "util/download.h"
#include "util/includes.h"
#include "util/random.h"
#include "util/uuid.h"
#include "data/ops/image.h"

namespace vectorbook {

namespace {

Option<string> GetTitle(const string &page_contents) {
  vector<string> tags = xml::FindAllSimpleTags("meta", page_contents);
  for (const string &tag : tags) {
    optional<string> tag1 = xml::GetSimpleProperty(tag, "property");
    if (tag1.has_value() && *tag1 == "og:title") {
      optional<string> tag2 = xml::GetSimpleProperty(tag, "content");
      if (tag2.has_value()) {
        return *tag2;
      }
    }
  }
  return Null();
}

Option<string> GetImageRemotePath(const string &page_contents) {
  vector<string> tags = xml::FindAllSimpleTags("meta", page_contents);
  for (const string &simple_tag : tags) {
    // string st2 = Replace(simple_tag, {{"\">", "\" />"}});
    optional<string> tag1 = xml::GetSimpleProperty(simple_tag, "property");
    if (tag1.has_value() && *tag1 == "og:image") {
      optional<string> tag2 = xml::GetSimpleProperty(simple_tag, "content");
      if (tag2.has_value()) {
        return *tag2;
      }
    }
  }
  return Null();
}

Void DownloadImageToLocal(const string &path) {
//  auto root_path = GetRootImagePath();
//  ASSERT_OK(root_path);
  return Ok();
}

} // namespace

Option<Resource> CreateResource(const string &id, const string &url) {
  Resource resource;

  // Not found, download the data.
  auto page_contents = DownloadHttpPage(url);
  ASSERT_OK(page_contents);

  resource.set_id(id);
  auto title = GetTitle(*page_contents);
  ASSERT_OK(title);
  if (title.has_value()) {
    resource.set_title(*title);
  }
  auto image_remote_path = GetImageRemotePath(*page_contents);
  ASSERT_OK(image_remote_path);

  if (image_remote_path.has_value()) {
		auto image = BuildImage(*image_remote_path);
		ASSERT_EXISTS(image);
		*resource.mutable_image() = *image;
  }
  resource.set_url(url);

  // Store to the db again.
  ASSERT_SUCCEEDS(data::resource_map()->put(resource.id(), resource));
  return resource;
}

Option<string> ResourceIdFromUrl(const string &url) {
  auto id = data::url_resource_map()->const_get(url);
  ASSERT_OK(id);
  if (id.has_value()) {
    // return *id;
  }

  // Assume we need to add this URL.
  auto uuid = FreshTimeUUID();
  ASSERT_OK(uuid);
  auto put_status = data::url_resource_map()->put(url, *uuid);
  ASSERT_OK(put_status);

  auto resource = CreateResource(*uuid, url);
  ASSERT_OK(resource);
  return *uuid;
}

Option<Resource> ResourceFromUrl(const string &url) {
  auto id = ResourceIdFromUrl(url);
  ASSERT_OK(id);
  auto resource = data::resource_map()->const_get(*id);
  ASSERT_OK(resource);
  ASSERT(resource.has_value());
  return *resource;
}

} // namespace vectorbook
