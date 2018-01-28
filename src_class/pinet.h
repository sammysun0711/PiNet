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
#include <map>
#include <chrono>
#include <iomanip>
#include <fstream>

class PiNet
{
public:
    PiNet(const std::string& username, const std::string& passwort){
        usr = username;
        pwd = passwort;        
    };
    ~PiNet(){};

    template <typename T1>
    bool isNumber(const T1& x);
    std::string tail(const std::string& source, const size_t& length);
    template <typename T2>
    std::map<std::string,std::string> valItem(const std::string& varname, const std::string& datatype, T2 value, int row, int col);
    std::map<std::string,std::string> getDateTime();
    std::string readfile(const std::string& filename);
    
    void parseReply(const std::string& reply);
    std::string sendRequest(const std::string& prozedur, const std::list<std::string>& parameter);

    std::string initSet(const std::string& setname);
    std::string delSet( const std::string& setname);
    std::string initVar(const std::string& setname, int row, int col, const std::string& datatype, const std::string& varname, const std::string& unit);
    std::string resetVal(const std::string& setname, const std::string& varname);
    std::string delVar(const std::string& setname, const std::string& varname);

    std::string getVal (const std::string& setname, const std::string& varname, const std::string& datatype);
    std::string getVals (const std::string& setname, const std::list<std::string>& varnames);
    // TODO find alternative template way, aim to reduce override function of saveVal
    std::string saveVal (const std::string& setname, const std::string& varname, const bool& value, const std::string& datatype, int row , int col);
    std::string saveVal (const std::string& setname, const std::string& varname, const int& value, const std::string& datatype, int row , int col);
    std::string saveVal (const std::string& setname, const std::string& varname, const double& value, const std::string& datatype, int row , int col );
    std::string saveVal (const std::string& setname, const std::string& varname, const std::string& value, const std::string& datatype, int row , int col);
    std::string saveVals (const std::string& setname, const std::list<std::map<std::string,std::string> >& varList);

private:
    std::string usr;
    std::string pwd;    
};

#include "pinet.hpp"

#endif // PINET_H