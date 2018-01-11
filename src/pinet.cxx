#include <iostream>
#include <sstream>
#include <boost/network.hpp>
#include <boost/network/uri.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include <string>
#include <cassert>
#include <map>

//using namespace std;
using namespace boost::network;
using namespace boost::network::http;
using namespace rapidjson; 

const std::string url = "http://codset.lorch-group.de/PROCinterface.php";
const std::string TRIGGER_RESPONSE_MYSQL = "??**wejnonwr4234gTRIGGER_RESPONSE_MYSQL**??";
const std::string TRIGGER_ERRORS_PHP = "??**wejnonwr4234gTRIGGER_ERRORS_PHP**??";
const std::string TRIGGER_ERRORS_MYSQL = "??**wejnonwr4234gTRIGGER_ERRORS_MYSQL**??";
//const std::string ERROR_CODE_KEY = "errorCode";
//const std::string ERROR_MESS_KEY = "errorMessage";
const std::string usr = "Xiake";
const std::string pwd = "123";    
const std::string TRENNER = "?/?TRENN?/?";

//helper function for check if value is number
template<typename T>
bool isNumber(T x){
   std::string s;
   std::stringstream ss; 
   ss << x;
   ss >> s;
   if(s.empty() || std::isspace(s[0]) || std::isalpha(s[0])) return false ;
   char * p ;
   strtod(s.c_str(), &p) ;
   return (*p == 0) ;
}

//helper function for truncrate [] part from httpResponse        
std::string tail(std::string const& source, size_t const length) {
  if (length >= source.size()) { return source; }
  return source.substr(source.size() - length);
}

//helper function for spilt multiply json in string into std::list<std::string>
std::list<std::string> get_str_between_two_str(const std::string &s,
        const std::string &start_delim,
        const std::string &stop_delim)
{
    std::string tmpString = s;
    std::list<std::string> list;
    unsigned first_delim_pos;
    unsigned end_pos_of_first_delim;
    unsigned last_delim_pos;
    while(tmpString.find(start_delim)!= std::string::npos){
        first_delim_pos = tmpString.find(start_delim);
        end_pos_of_first_delim = first_delim_pos + start_delim.length();
        last_delim_pos = tmpString.find_first_of(stop_delim, end_pos_of_first_delim);
        
        list.push_back("{" + tmpString.substr(end_pos_of_first_delim,
                                last_delim_pos - end_pos_of_first_delim) + "}");
        tmpString = tail(s, tmpString.length() - last_delim_pos -1);
    }
    return list; 
}

// helper function to make std::map object to make key-value mapping
template<typename T>
std::map<std::string,std::string> valItem(std::string varname, std::string datatype, T value, int row, int col){
    std::map<std::string,std::string> Item;
    Item.insert(std::make_pair("varname", varname));
    std::string value_;
    if (isNumber(value)){
        value_ = boost::lexical_cast<std::string>(value);
    }
    else{
        value_ = value;
    }
    Item.insert(std::make_pair("datatype", datatype));
    Item.insert(std::make_pair("value", value_));
    Item.insert(std::make_pair("row", std::to_string(row)));
    Item.insert(std::make_pair("col", std::to_string(col)));
    
    return Item;
}

// helper function aim to parse reply from response from server
void parseItem(std::string reply){
    // Parse Json into document using rapidjson
    std::string strJson;
    if (reply.find(TRIGGER_RESPONSE_MYSQL)!=std::string::npos){
        int count;
        count = reply.length()-reply.find(TRIGGER_RESPONSE_MYSQL)-TRIGGER_RESPONSE_MYSQL.length();
        strJson = tail(reply, count);
        // if there are multiple json in [], save every content between {} in std::list<std::string>,
        // and parse single parse json into doc, when there is no json in list, return no json in reply from server
        if (strJson.length()>2){
            std::list<std::string> jsonList = get_str_between_two_str(strJson, "{","}"); 
            for (std::list<std::string>::const_iterator itr = jsonList.begin(), end = jsonList.end(); itr != end; ++itr) {
                std::string json = *itr;
                Document doc;
                if(!doc.Parse<rapidjson::kParseStopWhenDoneFlag>(json.c_str()).HasParseError()){
                    std::cout << std::endl << "parse result: " << std::endl;
                    for (Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr){
                        std::cout << itr->name.GetString() << ": "; //key name
                        if (itr->value.IsInt())          // if integer
                            std::cout << itr->value.GetInt() << std::endl;
                        else if (itr->value.IsDouble())  // if 
                            std::cout << itr->value.GetDouble() << std::endl;
                        else if (itr->value.IsString())  // if string
                            std::cout << itr->value.GetString() << std::endl;
                    }
                }
                else{
                    std::cerr << std::endl <<  "parse result : there is no valid content in reply" << std::endl;
                }      
            }
        }
            
        else if(strJson == "[]"){
            std::cerr << std::endl <<"parse result : prozedur success, there is no Json in reply from server" << std::endl;   
        }
    }
    else if(reply.find(TRIGGER_ERRORS_MYSQL)!=std::string::npos){
        int count;
        count = reply.length()-reply.find(TRIGGER_ERRORS_MYSQL)-TRIGGER_ERRORS_MYSQL.length();
        strJson = tail(reply, count);
        strJson.erase(0,1);
        strJson.erase(strJson.size()-1);
        std::cout << strJson << std::endl;

        Document doc;
        if(!doc.Parse<0>(strJson.c_str()).HasParseError()){
            std::stringstream failure;
            failure << doc["errorCode"].GetString() << " - " << doc["errorMessage"].GetString();
            std::cout << failure.str() << std::endl;
        }
    }
    else if (reply.find(TRIGGER_ERRORS_PHP)!=std::string::npos){
        int count;
        count = reply.length()-reply.find(TRIGGER_ERRORS_PHP)-TRIGGER_ERRORS_PHP.length();
        std::string failure = tail(reply, count);
        std::cout << std::endl << failure << std::endl;
    }
}

// helper function for sendRequest to server
std::string sendRequest(std::string prozedur, std::list<std::string> parameter){
    
    std::stringstream query;
    int count = 1;
    query << "username=" << uri::encoded(usr)
          << "&passwort=" << uri::encoded(pwd)
          << "&prozedur=" << uri::encoded(prozedur);

    for (std::list<std::string>::const_iterator iterator = parameter.begin(), end = parameter.end(); iterator != end; ++iterator) {
        std::string value = *iterator;
        std::string vtyp = "string";
        bool isNumber_ = false;

        isNumber_ = isNumber(value);
        if (value == "0")
            vtyp = "zero";
        else if (value.empty() || value == "")
            vtyp = "emptyString";
        else if (isNumber_)
            vtyp = "zahl";
    
        std::string tmpVtyp = "&vtyp" + std::to_string(count) + "=";
        query << tmpVtyp << uri::encoded(vtyp);

        std::string tmpVar = "&variable" + std::to_string(count) + "=";
        query << tmpVar << uri::encoded(value);
         
        count++;
    }
    
    //std::cout << query.str() << std::endl;
    // HTTP request object
    //client::request httpRequest("http://posttestserver.com/post.php?dump");
    client::request httpRequest(url);
    httpRequest << header("Connection", "close")
                << header("Content-Type","application/x-www-form-urlencoded");

    client httpClient;
    client::response httpResponse = httpClient.post(httpRequest, query.str());
    std::string httpResponseBody = body(httpResponse);
    std::cout << std::endl << std::endl << "RESPONSE: " << httpResponseBody << std::endl;
    
    return httpResponseBody;
}

// helper function for getting value of variable from databank
std::string getVal (std::string setname, std::string varname, std::string datatype){
    
    std::list<std::string> parameter;
    parameter.push_back(usr);
    parameter.push_back(pwd);
    parameter.push_back(setname);
    parameter.push_back(varname);
    parameter.push_back(datatype);

    return sendRequest("p_getVal", parameter);    
} 

std::string getVals (std::string setname, std::list<std::string> varnames){
    
    std::string vars;
    for (std::list<std::string>::const_iterator iterator = varnames.begin(), end = varnames.end(); iterator != end; ++iterator) {
        vars = vars + *iterator + TRENNER;
    }
    std::list<std::string> parameter;
    parameter.push_back(usr);
    parameter.push_back(pwd);
    parameter.push_back(setname);
    parameter.push_back(vars);

    return sendRequest("p_getVals", parameter);    
}

// helper function for saving value of variable in databank 
template <typename T>
std::string saveVal (std::string setname, std::string varname, T value, std::string datatype, int row = 0 , int col = 0){
    
    std::list<std::string> parameter;
    
    std::string value_;
    if (isNumber(value)){
        value_ = boost::lexical_cast<std::string>(value);
    }
    else{
        value_ = value;
    }
    parameter.push_back(usr);
    parameter.push_back(pwd);
    parameter.push_back(setname);
    parameter.push_back(varname);
    parameter.push_back(value_);
    parameter.push_back(datatype);
    parameter.push_back(std::to_string(row));
    parameter.push_back(std::to_string(col));

    return sendRequest("p_saveValItem", parameter);    
}

// helper function aim to save multiply variable in databank
std::string saveVals (std::string setname, std::list<std::map<std::string,std::string> > varList){
    
    std::string vars = "";
    std::string values = "";
    std::string datatypes = "";
    std::string rows = "";
    std::string cols = "";
    
    for (auto itr = varList.begin(), end = varList.end(); itr != end; ++itr) {
        auto valItem = *itr;        
        vars = vars + valItem["varname"] + TRENNER;
        values = values + valItem["value"] + TRENNER;
        datatypes = datatypes + valItem["datatype"] + TRENNER;
        rows = rows + valItem["row"] + TRENNER;
        cols = cols + valItem["col"] + TRENNER; 
    }

    std::list<std::string> parameter;
    parameter.push_back(usr);
    parameter.push_back(pwd);
    parameter.push_back(setname);
    parameter.push_back(vars);
    parameter.push_back(values);
    parameter.push_back(datatypes);
    parameter.push_back(rows);
    parameter.push_back(cols);

    return sendRequest("p_saveValItems", parameter);    
}



std::string initSet( std::string setname){
    std::list<std::string> parameter;
    parameter.push_back(usr);
    parameter.push_back(pwd);
    parameter.push_back(setname);
    return sendRequest("p_initSet", parameter);
}

std::string delSet( std::string setname){
    std::list<std::string> parameter;
    parameter.push_back(usr);
    parameter.push_back(pwd);
    parameter.push_back(setname);
    
    return sendRequest("p_delSet", parameter);
}

std::string initVar( std::string setname, int row, int col, std::string datatype, std::string varname, std::string unit){
    std::list<std::string> parameter;
    parameter.push_back(usr);
    parameter.push_back(pwd);
    parameter.push_back(setname);
    parameter.push_back(std::to_string(row));
    parameter.push_back(std::to_string(col));
    parameter.push_back(datatype);
    parameter.push_back(varname);
    parameter.push_back(unit);
    
    return sendRequest("p_initVar", parameter);
}

std::string resetVal(std::string setname, std::string varname){
    std::list<std::string> parameter;
    parameter.push_back(usr);
    parameter.push_back(pwd);
    parameter.push_back(setname);
    parameter.push_back(varname);

    return sendRequest("p_resetVal", parameter);
}

std::string delVar(std::string setname, std::string varname){
    std::list<std::string> parameter;
    parameter.push_back(usr);
    parameter.push_back(pwd);
    parameter.push_back(setname);
    parameter.push_back(varname);

    return sendRequest("p_delVar", parameter);
}


int main()
{   
    const std::string setname = "TestsetXiake";
    
    const std::string varname = "n";
    const std::string datatype = "INT32";
    const int value = 28;

    //const std::string varname = "x";
    //const std::string datatype = "DOUBLE";
    //const double value = 3.14;
    
    //const std::string varname = "str100";
    //const std::string datatype = "STRING100";
    //const std::string value = "hello world";
    
    std::string reply_initSet = initSet("initSet");
    parseItem(reply_initSet);
    
    std::string reply_initVar = initVar("initSet", 1, 1, datatype, varname, "");
    parseItem(reply_initVar);
    
    std::string reply_resetVal = resetVal("initSet", varname);
    parseItem(reply_resetVal);
    
    std::string reply_delVar = delVar("initSet", varname);
    parseItem(reply_delVar);
    
    std::string reply_delSet = delSet("initSet");
    parseItem(reply_delSet);

    std::string reply_getVal = getVal(setname, varname, datatype);
    parseItem(reply_getVal);

    std::list<std::string> varnames;
    varnames.push_back("n");
    varnames.push_back("x");
    varnames.push_back("str100");    
    std::string reply_getVals = getVals(setname, varnames);
    parseItem(reply_getVals);
    

    std::string reply_saveVal = saveVal(setname, varname, value, datatype, 0, 0);
    parseItem(reply_saveVal);

    
    std::list<std::map<std::string,std::string> > varList;
    varList.push_back(valItem("n", "INT32",  32, 0, 0));
    varList.push_back(valItem("x", "DOUBLE", 3.14, 0, 0));
    varList.push_back(valItem("str100", "STRING100", "hello world", 0, 0));
    std::string reply_saveVals = saveVals(setname, varList);
    parseItem(reply_saveVals);

    return 0;
}

