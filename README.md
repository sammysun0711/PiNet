# PiNet
network interface for rasberry pi based on cpp-netlib

## build on ubuntu 16.04 using gcc
	cd ~/PiNet/build
	cmake ../src/ 
	make

## build on ubuntu 16.04 using cross compile toolchain for raspberrypi 1
	cd ~/PiNet
	mkdir raspberrypi_build
	cd raspberrypi_build
	cmake -DCMAKE_TOOLCHAIN_FILE=$HOME/raspberrypi/pi.cmake  ../src 
	make

## run 
	./PiNet

## support datatype
	BOOL 
	INT32 
	DOUBLE 
	STR100
	STRK 
	STRM 
	STR100M 
	TIME 
	DATE 
	DATETIME 

## support function
	initSet(std::string setname)
	delSet(std::string setname)
	initVar(std::string setname, int row, int col, std::string datatype, std::string varname, std::string unit)
	resetVal(std::string setname, std::string varname)
	delVar(std::string setname, std::string varname)
	getVal(std::string setname, std::string varname, std::string datatype)
	getVals(std::string setname, std::list<std::string> varnames)
	saveVal(std::string setname, std::string varname, T value, std::string datatype, int row, int col)
	saveVals(std::string setname, std::list<std::map<std::string,std::string> > varList) 
