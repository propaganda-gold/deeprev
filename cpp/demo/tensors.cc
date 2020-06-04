#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "util/includes.h"
#include "torch/torch.h"

using namespace vectorbook;

struct Net : torch::nn::Module {
  Net(const Tensor& linear1, const Tensor& linear2, const Tensor& embedding )
      : linear1_(linear1), linear2_(linear2), embedding_(embedding) {
    register_module("linear1", linear1_);
    register_module("linear2", linear2_);
    register_module("embedding", embedding_);
  }

  torch::Tensor forward(torch::Tensor x) {
		x = embedding_->forward(x);
		x = torch::relu(linear1_->forward(x));
    x = linear2_->forward(x);
    return torch::log_softmax(x, /*dim=*/1);
  }

  torch::nn::Linear linear1_;
  torch::nn::Linear linear2_;
  torch::nn::Embedding embedding_;
};

int main(int argc,  char** argv) {
	absl::ParseCommandLine(argc, argv);

	vector<float> fv {1.0, 0.0, -1.0};
	auto options = at::TensorOptions(at::CPU(c10::ScalarType::BFloat16));

	auto tensor = torch::zeros({1, 5}, options);
	tensor[0][0] = 2.0;

	return 0;
}
