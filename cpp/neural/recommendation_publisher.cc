#include "neural/recommendation_publisher.h"

#include "util/includes.h"
#include "data/globals.h"

namespace vectorbook {

namespace impl {

class BasicRecommendationPublisher : public RecommendationPublisher {
public:
	BasicRecommendationPublisher() {}
  Void Process(const string& object) const {
		auto user_map = data::user_map(); ASSERT_OK(user_map);
		for (auto it : *user_map) {
			ASSERT_OK(it);
		}
		return Ok();
	}
};
} // namespace impl

Unique<RecommendationPublisher>
RecommendationPublisher::Create(const PublisherOptions &options) {
  auto result = new impl::BasicRecommendationPublisher();
  return result;
}

} // namespace vectorbook
