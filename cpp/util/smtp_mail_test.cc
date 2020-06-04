#include "util/smtp_mail.h"

#include "util/protos.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

const string kPath = "/home/greg/vectorbook/server/config/gregcoppola92_smtp.pbtxt";

TEST(SendMail, LoadConnection) {
//	auto status = LoadSmtpConnection(kPath);
//	PrettyPrintError(status);
//	ASSERT_TRUE(status.ok());
}

TEST(SendMail, Basic) {
	auto status1 = LoadSmtpConnection(kPath);
	ASSERT_TRUE(status1.ok());
	auto request = ParseProto<SmtpRequest>(R"(
		date: "Tue Nov 19 18:55:43 UTC 2019"
		to_address: "gfcoppola@gmail.com"
		message_id: "id"
		subject: "Subject"
		body: "Body"
	)");


	auto status = SendEmail(request);
	ASSERT_TRUE(status.ok());
}

} // namespace vectorbook
