#include "util/check_recaptcha.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

TEST(A,B) {
	RecaptchaRequest request;
	request.set_secret("6LfLhMcUAAAAAMPZldb-mhBeGFGBS_a1r86z21OJ");
	request.set_token("03AOLTBLRWkN628w7Cm8wNJvuUHfJzQqJeq39MSsFJl5kQF8gSLwMDQwc5Q_rSIwKwkSU5TC6SDD6K3bPeSr2HHh0b3lcOcjxjxde8XfdkPrfDS5TC0dM4GWQH4eKf0hE7feYwPv2LkY7LrCEUFOdaAS3garl3kkJrv1-s0-6QLRw8CSmGOlwfgbltRi2Vrj4NqIxx73YmlN7SZFIgv95TlALR7N99pW8EoEnfd2F1lkL4XUj1XggMpdRYemcyB3JEvCkjQFLogrI_JsQd3Ha4O1bRb52So1aSlg3JdX6uS1ZtQMz8if3ZzUAhPvFpXHbcx3WHoJQ8g8JG4aq6g77Lzo8VrYTKWNrOHV_pedfGfENnaD1GxHD0vp0x0PT1llfZlxNbubNmSX3i5SRQGbuM2GXuQMUJmQEsOryfpEBEGnZbRLakSZUFa0eB1r5p4yHAz5rL6aF4o457IxRq0kwJbF3nNvQbGmHCD4_O6Ztp00oQzaA1KhK3tlWJw166QMmlUUc_XzAa5kk7");


	Option<RecaptchaResponse> result = CheckRecaptcha(request);

	std::cout << "result: " << result.status() << "\n";
}

} // namespace vectorbook
