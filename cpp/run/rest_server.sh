bazel build server:rest_server && \
	bazel-bin/server/rest_server \
	--http_options=public-config/local_redis.pbtxt
