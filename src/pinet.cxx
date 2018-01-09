#include <iostream>
#include <sstream>
#include <boost/network.hpp>
#include <boost/network/uri.hpp>
#include <boost/lexical_cast.hpp>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include <string>


using namespace std;
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

//helper function for check if value is number
template<typename T>
bool isNumber(T x){
   std::string s;
   std::stringstream ss; 
   ss << x;
   ss >>s;
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
void parse(std::string reply, std::string setname, std::string varname, std::string datatype){
    // Parse Json into document using rapidjson
    std::string strJson;
    if (reply.find(TRIGGER_RESPONSE_MYSQL)!=std::string::npos){
        int count;
        count = reply.length()-reply.find(TRIGGER_RESPONSE_MYSQL)-TRIGGER_RESPONSE_MYSQL.length();
        strJson = tail(reply, count);
        cout<< endl<< endl << strJson <<endl;
        // if there is json in [], parse json into doc, otherweise return no json in reply from server
        if (strJson.length()>2){
        strJson.erase(0,1);
        strJson.erase(strJson.size()-1);
        cout<< endl << endl << strJson <<endl;
    
        Document doc;
        try{
            if(doc.Parse<0>(strJson.c_str()).HasParseError())
            cout << "ERROR" << endl;
            else
            cout << "OK" << endl;

            doc.Parse(strJson.c_str());
            std::string setname_ = doc["setname"].GetString();
            std::string varname_ = doc["varname"].GetString();
            std::string datatype_ = doc["datatype"].GetString();
        
            if (setname_ == setname && varname_==varname && datatype_ == datatype){
            std::string value_ = doc["value"].GetString();
            cout << "setname = " << setname_ << endl;
            cout << "varname = " << varname_ << endl;
            cout << "datatype = " << datatype_ << endl;
            cout << "value = " << value_ << endl;
            }
            else{
                cout << "there is no valid content in reply" << endl;
            }
        } 

        catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
        }

        else if(strJson == "[]"){
            std::cerr << "there is no json in reply from server" << endl;   
        }
    }
    else if(reply.find(TRIGGER_ERRORS_MYSQL)!=std::string::npos){
        int count;
        count = reply.length()-reply.find(TRIGGER_ERRORS_MYSQL)-TRIGGER_ERRORS_MYSQL.length();
        strJson = tail(reply, count);
        strJson.erase(0,1);
        strJson.erase(strJson.size()-1);
        cout << strJson << endl;
        Document doc;
        if(!doc.Parse<0>(strJson.c_str()).HasParseError()){
        doc.Parse(strJson.c_str());
        std::stringstream failure;
        failure << doc["errorCode"].GetString() << " - " << doc["errorMessage"].GetString();
        cout << failure.str() << endl;
        }
    }
    else if (reply.find(TRIGGER_ERRORS_PHP)!=std::string::npos){
        int count;
        count = reply.length()-reply.find(TRIGGER_ERRORS_PHP)-TRIGGER_ERRORS_PHP.length();
        std::string failure = tail(reply, count);
        cout << endl << failure << endl;
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
    cout << query.str() << endl;
    
    // HTTP request object
    //client::request httpRequest("http://posttestserver.com/post.php?dump");
    client::request httpRequest(url);
    httpRequest << header("Connection", "close")
                << header("Content-Type","application/x-www-form-urlencoded");

    client httpClient;
    client::response httpResponse = httpClient.post(httpRequest, query.str());
    std::string httpResponseBody = body(httpResponse);
    cout << endl << endl << "RESPONSE: " << httpResponseBody;
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


// helper function for saving value of variable in databank 
template <typename T>
std::string saveVal (std::string setname, std::string varname, T value, std::string datatype, int row, int col){
    
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
    
    std::string reply_getVal = getVal(setname, varname, datatype);
    parse(reply_getVal, setname, varname, datatype);

    std::string reply_saveVal = saveVal(setname, varname, value, datatype , 0, 0);
    parse(reply_saveVal, setname, varname, datatype);
    return 0;
}

