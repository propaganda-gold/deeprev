set -e

PROD_DIR=/prod
DATA_DIR=deploy/rest_server

sudo mkdir -p ${PROD_DIR}

bazel build -c opt server:debug_server
sudo cp bazel-bin/server/debug_server ${PROD_DIR}

sudo cp ${DATA_DIR}/run.sh ${PROD_DIR}
sudo cp public-config/local_redis.pbtxt ${PROD_DIR}
sudo cp ${DATA_DIR}/rest_server.conf /etc/supervisor/conf.d
