bazel build server:email_server && \
	bazel-bin/server/email_server \
	--http_options=/secrets/deeprevelations.http_options.prod.pbtxt \
	--sendgrid_secret_file=/secrets/deeprevelations-at-sendgrid.auth-key
