# "hiredis" is a c-language client library
cd ~
git clone --depth 1 https://github.com/redis/hiredis.git
cd -
cp templates/hiredis.BUILD ~/hiredis/BUILD
cp templates/hiredis.WORKSPACE ~/hiredis/WORKSPACE
cd ~/hiredis
sudo make install
sudo cp ~/hiredis/libhiredis.so /usr/lib/libhiredis.so.0.14

# redis server
sudo apt -y install redis-server
