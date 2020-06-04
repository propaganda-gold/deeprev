sudo apt-get -y install pkg-config zip g++ zlib1g-dev unzip python3
mkdir -p ~/Downloads
cd ~/Downloads
wget https://github.com/bazelbuild/bazel/releases/download/1.2.0/bazel-1.2.0-installer-linux-x86_64.sh
chmod u+x bazel-1.2.0-installer-linux-x86_64.sh 
sudo apt -y install unzip
sudo ./bazel-1.2.0-installer-linux-x86_64.sh
echo "source /usr/local/lib/bazel/bin/bazel-complete.bash" >> ~/.bash_aliases
