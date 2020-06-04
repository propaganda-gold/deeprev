sudo apt update
sudo apt -y install cmake

cd ~
CURL_VERSION=curl-7.66.0
CURL_FILE=${CURL_VERSION}.tar.gz
wget https://curl.haxx.se/download/${CURL_FILE}
tar -xvzf ${CURL_FILE}
cd ${CURL_VERSION}
mkdir build
cd build
cmake ..
sudo make install
