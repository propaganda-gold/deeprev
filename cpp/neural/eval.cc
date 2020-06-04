#include "neural/eval.h"

#include "util/includes.h"

namespace vectorbook {
namespace impl {

class BasicEval : public Eval {
public:
  ~BasicEval() {}
  void Tally(const BooleanRequest &request,
             const BooleanResponse &gold_response,
             const BooleanResponse &pred_response) override {
    assert(gold_response.label_size() == 1);
    assert(pred_response.label_size() == 1);

		auto gold_idx = gold_response.label(0).value();
		auto pred_idx = pred_response.label(0).value();
		if (gold_idx == pred_idx) {
			scores_.push_back(1);
		} else {
			scores_.push_back(0);
		}
  }
  string OutputStats() const override {
		size_t total = 0;
		size_t correct = 0;
		for (int i = 0; i < 100; ++i) {
			if (i < 0) break;
			total += 1;
			correct += scores_[scores_.size() - i];
		}
		float avg = 1.0f * correct / total;
		return StrCat("round: ", scores_.size(), "; recent accuracy: ", correct, "/", total,  " = ", avg, "%");
	}
private:
	vector<size_t> scores_;
};

class BasicSampler : public EvalSampler {
public:
	bool Tick() override {
		count_ += 1;
		return count_ % modulus_ == 0; 
	}
private:
	size_t count_ = 0;
	size_t modulus_ = 1;
};

} // namespace impl

unique_ptr<Eval> BasicEval() { return unique_ptr<Eval>(new impl::BasicEval()); }

unique_ptr<EvalSampler> BasicSampler() {
return unique_ptr<EvalSampler>(new impl::BasicSampler());
}

} // namespace vectorbook
