#ifndef neural_recommendation_publisher_h
#define neural_recommendation_publisher_h 
#include "util/includes.h"
#include "server/options.pb.h"

namespace vectorbook {

class RecommendationPublisher {
public:
	static Unique<RecommendationPublisher> Create(const PublisherOptions& options);
	// NOTE: This will make predictions, but NOT do training.
  virtual Void Process(const string& object) const = 0;
};


} // namespace vectorbook
#endif /* neural_recommendation_publisher_h */
