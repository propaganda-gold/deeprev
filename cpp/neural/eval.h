#ifndef neural_eval_h
#define neural_eval_h
#include <iostream> // std::cout, std::right, std::endl

#include "neural/features.pb.h"
#include "util/includes.h"

namespace vectorbook {

class Eval {
public:
  virtual ~Eval() {}
  virtual void Tally(const BooleanRequest &request,
                     const BooleanResponse &gold_response,
                     const BooleanResponse &pred_response) = 0;

	// TODO(greg) Make it work for ostream.
  virtual string OutputStats() const = 0;
};

unique_ptr<Eval> BasicEval();

class EvalSampler {
public:
	// Returns true if we should print an eval.
	virtual bool Tick() = 0;
};

unique_ptr<EvalSampler> BasicSampler();

} // namespace vectorbook
#endif /* neural_eval_h */
