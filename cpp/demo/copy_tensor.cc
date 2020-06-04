#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "torch/torch.h"
#include "util/includes.h"

using namespace vectorbook;
using namespace torch::nn;

int kUserDim = 3;
int kHiddenDim = 4;
int kNumOutcomes = 2;

struct Net : torch::nn::Module {
  Net(
	const torch::Tensor &etensor, const Linear &linear1,
      const Linear &linear2, const Embedding &embedding)
      : etensor_(etensor), linear1_(linear1), linear2_(linear2),
        embedding_(embedding) {
    register_parameter("etensor", etensor_);
    register_module("linear1", linear1_);
    register_module("linear2", linear2_);
    // register_module("embedding", embedding_);
  }

  torch::Tensor forward(torch::Tensor x) {
    x = torch::embedding(etensor_, x, 0, false, false);
    x = torch::relu(linear1_->forward(x));
    x = linear2_->forward(x);
    return torch::log_softmax(x, /*dim=*/1);
  }

  torch::Tensor etensor_;
  torch::nn::Linear linear1_;
  torch::nn::Linear linear2_;
  torch::nn::Embedding embedding_;
};

int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);

  torch::nn::Linear linear1(kUserDim, kHiddenDim);
  torch::nn::Linear linear2(kHiddenDim, kNumOutcomes);
  torch::nn::Embedding embedding(2, kUserDim);

  auto etensor = torch::empty({2, kUserDim});
  torch::nn::init::normal_(etensor);

  for (int i = 0; i < 4; ++i) {
    size_t label = i % 2;
    vector<long> lv;
    lv.push_back(label);
    auto x = torch::tensor(lv);
    Net net(etensor, linear1, linear2, embedding);
    torch::optim::SGD optim(net.parameters(), torch::optim::SGDOptions(0.01));

    auto output = net.forward(x);
    vector<long> buffer;
    buffer.push_back(label);
    auto target = torch::tensor(buffer);
    auto loss = torch::nll_loss(output, target);
    loss.backward();
    for (auto kv : net.named_parameters()) {
		}

    optim.step();
    for (auto kv : net.named_parameters()) {
    }
  }

  return 0;
}
