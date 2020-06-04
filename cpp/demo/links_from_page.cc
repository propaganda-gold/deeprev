#include "util/download.h"
#include "util/xml.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "util/includes.h"
#include "data/init_ops.h"

using absl::GetFlag;

ABSL_FLAG(std::string, url, "", "");
ABSL_FLAG(std::string, model_spec, "", "");

using namespace vectorbook;

Void Main() {
	auto url = GetFlag(FLAGS_url); ASSERT(!url.empty());
	auto content = DownloadHttpPage(url); ASSERT_OK(content);
	auto links = xml::AllLinks(*content); ASSERT_OK(links);
	for(auto inner_url : *links) {
		auto link_content = DownloadHttpPage(*inner_url);
		if (!link_content.ok()) continue;
		auto social_image = GetSocialImage(*link_content);
		if (!social_image.ok()) continue;
		if (!social_image.has_value()) continue;

		auto broadcast = CreateCaptionPost(*inner_url, "drudge"); ASSERT_OK(broadcast);
		auto init_status = InitObject(broadcast.get());
		ASSERT_OK(init_status);
	}

	return Ok();
}

#include "util/main_incl.h"
