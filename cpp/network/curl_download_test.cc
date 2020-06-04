#include "network/curl_download.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

TEST(CurlDownload, Basic) {
	auto init_status = curl::Initialize();
	ASSERT_TRUE(init_status.ok());

	auto foreign_url = "https://propaganda.gold/propaganda-gold.png";
	auto local_path = "/images/gold.png";
	auto download_status = CurlDownload(foreign_url, local_path);
	ASSERT_TRUE(download_status.ok()) << download_status.status();

	auto clean_status = curl::CleanUp();
	ASSERT_TRUE(clean_status.ok());
}

} // namespace vectorbook
