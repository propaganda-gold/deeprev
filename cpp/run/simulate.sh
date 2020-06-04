bazel build simulate:main

if [ $? -ne 0 ]; then
    exit 1;
fi

bazel-bin/simulate/main --model_spec=neural/config/model_spec1.pbtxt --simulation_config=simulate/config/add_users.pbtxt
bazel-bin/simulate/main --model_spec=neural/config/model_spec1.pbtxt --simulation_config=simulate/config/add_links.pbtxt
bazel-bin/simulate/main --model_spec=neural/config/model_spec1.pbtxt --simulation_config=simulate/config/add_posts.pbtxt
bazel-bin/simulate/main --model_spec=neural/config/model_spec1.pbtxt --simulation_config=simulate/config/add_actions.pbtxt
