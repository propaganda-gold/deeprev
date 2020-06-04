set -v

# 1. Install the cassandra database.
cd ~
echo "deb http://www.apache.org/dist/cassandra/debian 39x main" | sudo tee -a /etc/apt/sources.list.d/cassandra.sources.list
# curl https://www.apache.org/dist/cassandra/KEYS | sudo apt-key add -
curl https://downloads.apache.org/cassandra/KEYS | sudo apt-key add -

sudo apt update
sudo apt install cassandra
sudo systemctl enable cassandra
sudo systemctl start cassandra


# 2. Install the c++ dependencies.
cd ~
wget https://downloads.datastax.com/cpp-driver/ubuntu/18.04/dependencies/libuv/v1.34.0/libuv1_1.34.0-1_amd64.deb
sudo apt install ./libuv1_1.34.0-1_amd64.deb

wget https://downloads.datastax.com/cpp-driver/ubuntu/18.04/cassandra/v2.15.0/cassandra-cpp-driver_2.15.0-1_amd64.deb
sudo apt install ./cassandra-cpp-driver_2.15.0-1_amd64.deb
sudo cp /usr/lib/x86_64-linux-gnu/libcassandra.so.2 /usr/lib/x86_64-linux-gnu/libcassandra.so

cd ~
git clone https://github.com/datastax/cpp-driver.git
sudo cp ~/cpp-driver/include/* /usr/include
