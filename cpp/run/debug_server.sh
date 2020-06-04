bazel build server:debug_server && \
	bazel-bin/server/debug_server \
	--rest_options=config/rest_server.rest_options.pbtxt \
	--secrets=${HOME}/secrets/rest_server.secrets.pbtxt
