#include "neural/inference.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace vectorbook {

//TEST(EmbeddingNetwork, Basic) {
//	{
//		torch::nn::EmbeddingOptions options(2, 5);
//		torch::nn::Embedding network(options);
//		{
//			torch::Tensor tensor = torch::tensor({0L});
//			torch::Tensor tensor1 = network->forward(tensor);
//			std::cout << tensor << std::endl;
//			std::cout << tensor1 << std::endl;
//		}
//		{
//			torch::Tensor tensor = torch::tensor({1L});
//			torch::Tensor tensor1 = network->forward(tensor);
//			std::cout << tensor << std::endl;
//			std::cout << tensor1 << std::endl;
//		}
//	}
//	{
//		torch::nn::EmbeddingOptions options(2, 5);
//		torch::nn::Embedding network(options);
//		{
//			torch::Tensor tensor = torch::tensor({0L});
//			torch::Tensor tensor1 = network->forward(tensor);
//			std::cout << tensor << std::endl;
//			std::cout << tensor1 << std::endl;
//		}
//		{
//			torch::Tensor tensor = torch::tensor({1L});
//			torch::Tensor tensor1 = network->forward(tensor);
//			std::cout << tensor << std::endl;
//			std::cout << tensor1 << std::endl;
//		}
//	}
//}

//TEST(EmbeddingNetwork, Concat) {
//	torch::nn::EmbeddingOptions options(2, 5);
//	torch::nn::Embedding network(options);
//	torch::Tensor tensor0 = torch::tensor({0L});
//	torch::Tensor tensor0o = network->forward(tensor0);
//	std::cout << tensor0o << std::endl;
//	torch::Tensor tensor1 = torch::tensor({1L});
//	torch::Tensor tensor1o = network->forward(tensor1);
//	std::cout << tensor1o << std::endl;
//
//	auto c = torch::cat({tensor0o, tensor1o});
//	std::cout << c << std::endl;
//	auto d = torch::flatten(c);
//	std::cout << d << std::endl;
//
//  torch::nn::Linear fc1(10, 6);
//	auto e = fc1->forward(d);
//	std::cout << e << std::endl;
//
//	auto f = torch::relu(e);
//	std::cout << f << std::endl;
//
//	{
//		torch::nn::Linear fc2 (6, 3);
//		auto g = fc2->forward(f);
//		std::cout << g << std::endl;
//		auto h = torch::softmax(g, 0);
//		std::cout << h << std::endl;
//	}
//
//	{
//		torch::nn::Linear fc2 (6, 1);
//		auto g = fc2->forward(f);
//		std::cout << g << std::endl;
//		auto h = torch::logistic(g);
//		std::cout << h << std::endl;
//	}
//
//	std::cout << "fuck" << std::endl;
//}

} // namespace vectorbook
