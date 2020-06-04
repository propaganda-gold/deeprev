cd ~
wget https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-1.3.1%2Bcpu.zip
unzip libtorch-cxx11-abi-shared-with-deps-1.3.1+cpu.zip
sudo cp -r ~/libtorch/include/* /usr/include/
sudo cp ~/libtorch/lib/*.so* /usr/lib
sudo cp -r ~/libtorch/include/* /usr/include
