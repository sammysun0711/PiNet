#include <iostream>
#include <sstream>
#include <boost/network.hpp>
#include <boost/network/uri.hpp>
#include <boost/lexical_cast.hpp>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include <string>


//using namespace std;
using namespace boost::network;
using namespace boost::network::http;
using namespace rapidjson; 

const std::string url = "http://codset.lorch-group.de/PROCinterface.php";
const std::string TRIGGER_RESPONSE_MYSQL = "??**wejnonwr4234gTRIGGER_RESPONSE_MYSQL**??";
const std::string TRIGGER_ERRORS_PHP = "??**wejnonwr4234gTRIGGER_ERRORS_PHP**??";
const std::string TRIGGER_ERRORS_MYSQL = "??**wejnonwr4234gTRIGGER_ERRORS_MYSQL**??";
const std::string ERROR_CODE_KEY = "errorCode";
const std::string ERROR_MESS_KEY = "errorMessage";
const std::string usr = "Xiake";
const std::string pwd = "123";    
const std::string TRENNER = "?/?TRENN?/?";
//helper function for check if value is number
/**
 * White space will make it return false.
 * NAN and INF will make it return false (to be exact, any character except valid exponent will make it return false). If you want to allow nan and inf, delete the || std::isalpha(s[0]) part.
 * scientific form is allowed i.e 1e+12 will return true.
 * Double/float or integer will return true. 
 * /
**/
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


//helper function for concrete [] part from httpResponse        
std::string tail(std::string const& source, size_t const length) {
  if (length >= source.size()) { return source; }
  return source.substr(source.size() - length);
}

// parse reply from response from server
//void parse(std::string reply, std::string setname, std::string varname, std::string datatype){
void parse(std::string reply){
    // Parse Json into document using rapidjson
    std::string strJson;
    if (reply.find(TRIGGER_RESPONSE_MYSQL)!=std::string::npos){
        int count;
        count = reply.length()-reply.find(TRIGGER_RESPONSE_MYSQL)-TRIGGER_RESPONSE_MYSQL.length();
        strJson = tail(reply, count);
        //cout<< endl<< endl << strJson <<endl;
        // if there is json in [], parse json into doc, otherweise return no json in reply from server
        if (strJson.length()>2){
            strJson.erase(0,1);
            strJson.erase(strJson.size()-1);
            //std::cout<< std::endl << std::endl << strJson << std::endl;
            //strJson.insert(1,"{");
            //strJson.append("}");
            std::cout<< std::endl << std::endl << strJson << std::endl;
            //std::stringstream strJsonStream;
            //strJsonStream << strJson;
            
            Document doc;                     
            //std::cout << doc.Parse<rapidjson::kParseStopWhenDoneFlag>(strJson.c_str()).HasParseError() << std::endl;
            try{
                if(!doc.Parse<rapidjson::kParseStopWhenDoneFlag>(strJson.c_str()).HasParseError()){
                    //doc.Parse<rapidjson::kParseStopWhenDoneFlag>(strJson.c_str());
                    //std::string setname_ = doc["setname"].GetString();
                    //std::string varname_ = doc["varname"].GetString();
                    for (Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr){
                        //const Value& objName = doc[itr->name.GetString()];
                        
                        std::cout << itr->name.GetString() << ": "; //key name
                        if (itr->value.IsInt()) //if integer
                            std::cout << itr->value.GetInt() << std::endl;
                        else if (itr->value.IsDouble()) 
                            std::cout << itr->value.GetDouble() << std::endl;
                        else if (itr->value.IsString()) //if string
                            std::cout << itr->value.GetString() << std::endl;
                    }
                    /**
                    if (setname_ == setname && varname_==varname && datatype_ == datatype){
                        std::string value_ = doc["value"].GetString();
                        std::cout << std::endl <<  "parse result:" << std::endl;
                        std::cout << "setname = " << setname_ << std::endl;
                        std::cout << "varname = " << varname_ << std::endl;
                        std::cout << "datatype = " << datatype_ << std::endl;
                        std::cout << "value = " << value_ << std::endl;
                    
                    }
                    else{
                        std::cerr << std::endl <<  "parse result : there is no valid content in reply" << std::endl;
                    }
                    **/
                }
            }    

            catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            }
        }

        else if(strJson == "[]"){
            std::cerr << std::endl <<"parse result : there is no Json in reply from server" << std::endl;   
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
            doc.Parse(strJson.c_str());
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
    //cout << query.str() << endl;
    
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
    
    //std::string reply_getVal = getVal(setname, varname, datatype);
    //parse(reply_getVal);

    //std::string reply_saveVal = saveVal(setname, varname, value, datatype);
    //parse(reply_saveVal, setname, varname, datatype);

    std::list<std::string> varnames;
    varnames.push_back("n");
    varnames.push_back("x");
    varnames.push_back("str100");
    
    std::string reply_getVals = getVals(setname, varnames);
    parse(reply_getVals);
    //std::string reply_initSet = initSet("initSet");
    //std::string reply_delSet = delSet("initSet");
    //std::string reply_initVar = initVar("initSet", 1, 1, datatype, varname, "");
    //std::string reply_resetVal = resetVal("initSet", varname);
    //std::string reply_delVar = delVar("initSet", varname);
    return 0;
}

