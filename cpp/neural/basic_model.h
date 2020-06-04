#ifndef neural_basic_model_h
#define neural_basic_model_h
#include "coln/api.h"
#include "coln/map.h"
#include "neural/api.h"
#include "util/includes.h"
#include "neural/training_config.pb.h"

namespace vectorbook {

Unique<ForwardBackwardModel> CreateBasicModel(const ModelSpec &model_spec);

} // namespace vectorbook
#endif /* neural_basic_model_h */
