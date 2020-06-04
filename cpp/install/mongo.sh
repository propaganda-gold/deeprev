# 1. mongo-c-driver
cd ~
git clone https://github.com/mongodb/mongo-c-driver.git
cd mongo-c-driver
mkdir cmake-build
cd cmake-build
cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..
sudo make install

# 2. mongo-cxx-driver
cd ~
git clone https://github.com/mongodb/mongo-cxx-driver.git --branch releases/stable --depth 1
cd mongo-cxx-driver/build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
sudo make EP_mnmlstc_core
make && sudo make install
cd /usr/local/include
sudo cp -r mongocxx/v_noabi/mongocxx/* mongocxx/
sudo cp -r bsoncxx/v_noabi/bsoncxx/* bsoncxx/
