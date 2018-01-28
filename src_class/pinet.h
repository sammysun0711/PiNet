#ifndef PINET_H
#define PINET_H

#include <iostream>
#include <sstream>
#include <boost/network.hpp>
#include <boost/network/uri.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <string>
#include <map>
#include <chrono>
#include <iomanip>
#include <fstream>

class PiNet
{
public:
    PiNet(std::string username, std::string passwort){
        usr = username;
        pwd = passwort;        
    };
    ~PiNet(){};

    template <typename T1>
    bool isNumber(T1 x);
    std::string tail(std::string const& source, size_t const length);
    template <typename T2>
    std::map<std::string,std::string> valItem(std::string varname, std::string datatype, T2 value, int row, int col);
    std::map<std::string,std::string> getDateTime();
    std::string readfile( std::string filename);
    void parseReply(std::string reply);
    std::string sendRequest(std::string prozedur, std::list<std::string> parameter);

    std::string initSet( std::string setname);
    std::string delSet( std::string setname);
    std::string initVar( std::string setname, int row, int col, std::string datatype, std::string varname, std::string unit);
    std::string resetVal(std::string setname, std::string varname);
    std::string delVar(std::string setname, std::string varname);

    std::string getVal (std::string setname, std::string varname, std::string datatype);
    std::string getVals (std::string setname, std::list<std::string> varnames);
    // TODO find alternative template way, aim to reduce override function of saveVal
    std::string saveVal (std::string setname, std::string varname, bool value, std::string datatype, int row , int col);
    std::string saveVal (std::string setname, std::string varname, int value, std::string datatype, int row , int col);
    std::string saveVal (std::string setname, std::string varname, double value, std::string datatype, int row , int col );
    std::string saveVal (std::string setname, std::string varname, std::string value, std::string datatype, int row , int col);
    std::string saveVals (std::string setname, std::list<std::map<std::string,std::string> > varList);


private:
    std::string usr;
    std::string pwd;    
};

#include "pinet.hpp"
#endif // PINET_H
