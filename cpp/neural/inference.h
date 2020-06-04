#ifndef neural_inference_h
#define neural_inference_h
#include "coln/api.h"
#include "coln/map.h"
#include "data/globals.h"
#include "neural/api.h"
#include "coln/map.h"
#include "neural/features.pb.h"
#include "torch/torch.h"
#include "util/includes.h"

namespace vectorbook {

unique_ptr<PredictionModel> TorchModel();

} // namespace vectorbook
#endif /* neural_inference_h */
