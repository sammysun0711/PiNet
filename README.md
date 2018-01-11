# PiNet
network interface for rasberry pi based on cpp-netlib

# make
cd ~/PiNet/build <br />
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DOPENSSL_ROOT_DIR=/opt/openssl  -DBOOST_ROOT=/usr/local/boost_1_66_0 ../src/  <br />
make <br />
make install <br /> 

# run 
./PiNet

# support datatype
BOOL <br /> 
INT32 <br /> 
DOUBLE <br /> 
STR100 <br /> 
STRK <br /> 
STRM <br /> 
STR100M <br /> 
TIME <br /> 
DATE <br /> 
DATETIME <br /> 

# support function
initSet(std::string setname) <br /> 
delSet(std::string setname) <br />
initVar(std::string setname, int row, int col, std::string datatype, std::string varname, std::string unit) <br />
resetVal(std::string setname, std::string varname) <br />
delVar(std::string setname, std::string varname) <br />
getVal(std::string setname, std::string varname, std::string datatype) <br />
getVals(std::string setname, std::list<std::string> varnames) <br/>
saveVal(std::string setname, std::string varname, T value, std::string datatype, int row, int col) <br />
saveVals(std::string setname, std::list<std::map<std::string,std::string> > varList) <br />