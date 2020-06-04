set -e

PROD_DIR=/prod
DATA_DIR=deploy/data/email_server

sudo mkdir -p ${PROD_DIR}

bazel build server:email_server
sudo cp bazel-bin/server/email_server ${PROD_DIR}

sudo cp ${DATA_DIR}/run.sh ${PROD_DIR}
sudo cp ${DATA_DIR}/email_server.conf /etc/supervisor/conf.d
