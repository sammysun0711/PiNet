# PiNet
network interface for rasberry pi based on cpp-netlib

# make
cd ~/PiNet/build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DOPENSSL_ROOT_DIR=/opt/openssl -DBOOST_ROOT=/usr/local/boost_1_66_0 ../src/
make
make install 

# run 
./PiNet
