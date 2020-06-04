#ifndef neural_example_generator_h
#define neural_example_generator_h 
#include "coln/api.h"
#include "neural/api.h"
#include "neural/training_config.pb.h"
#include "util/includes.h"

namespace vectorbook {

Unique<ExampleGenerator> ActionGenerator(const ModelSpec &model_spec);

} // namespace vectorbook
#endif /* neural_example_generator_h */
