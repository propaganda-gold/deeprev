bazel build neural:train2 && \
	bazel-bin/neural/train2 \
	--model_spec=neural/config/model_spec1.pbtxt \
	--training_spec=neural/config/training_spec1.pbtxt
