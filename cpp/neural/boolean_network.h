#ifndef neural_boolean_network_h
#define neural_boolean_network_h 
#include "util/includes.h"
#include "neural/api.h"

namespace vectorbook {

class NetworkInterface {
public:
	// Returns a constant  prediction model.
	virtual Const<PredictionModel> prediction_network() const = 0;

	// Returns mutable access to a training model.
	virtual Mutable<ForwardBackwardModel> training_network() = 0;
};


namespace globals {

// Must be called before any call to 'boolean_network'.
Void LoadBooleanNetwork(const string& pbtxt_file);

// Returns the global NetworkInterface.
Mutable<NetworkInterface> boolean_network_interface();


} // namespace globals
} // namespace vectorbook
#endif /* neural_boolean_network_h */
