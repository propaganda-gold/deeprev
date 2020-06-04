#include "neural/boolean_network.h"

#include "neural/basic_model.h"
#include "util/includes.h"

namespace vectorbook {

namespace {

class StandardNetworkInterface : public NetworkInterface {
public:
  Const<PredictionModel> prediction_network() const override {
    ASSERT(underlying_ != nullptr);
    return underlying_.get();
  }

  Mutable<ForwardBackwardModel> training_network() override {
    ASSERT(underlying_ != nullptr);
    return underlying_.get();
  }

public:
  unique_ptr<ForwardBackwardModel> underlying_;
};

static NetworkInterface *static_store = nullptr;

} // namespace

namespace globals {

Void LoadBooleanNetwork(const string &pbtxt_file) {
  auto store = new StandardNetworkInterface();
  auto proto = ReadProto<ModelSpec>(pbtxt_file);
  ASSERT_OK(proto);
  auto model = CreateBasicModel(*proto);
  ASSERT_OK(model);
  ForwardBackwardModel *u1 = model.release();
  store->underlying_.reset(u1);
  static_store = store;

	auto sanity_network = static_store->prediction_network();
  return Ok();
}

// Returns the global NetworkInterface
Mutable<NetworkInterface> boolean_network_interface() { return static_store; }

} // namespace globals
} // namespace vectorbook
