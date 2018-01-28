#include <iostream>
#include "pinet.h"

using namespace boost::network;
using namespace boost::network::http;
using namespace rapidjson; 

const std::string url = "http://codset.lorch-group.de/PROCinterface.php";
const std::string TRIGGER_RESPONSE_MYSQL = "??**wejnonwr4234gTRIGGER_RESPONSE_MYSQL**??";
const std::string TRIGGER_ERRORS_PHP = "??**wejnonwr4234gTRIGGER_ERRORS_PHP**??";
const std::string TRIGGER_ERRORS_MYSQL = "??**wejnonwr4234gTRIGGER_ERRORS_MYSQL**??";
const std::string TRENNER = "?/?TRENN?/?";

//helper function for truncrate [] part from httpResponse        
std::string PiNet::tail(std::string const& source, size_t const length) {
  if (length >= source.size()) { return source; }
  return source.substr(source.size() - length);
}

// helper function aim to get current datetime
std::map<std::string,std::string> PiNet::getDateTime(){
    std::array<char, 64> buffer;
    buffer.fill(0);
    time_t rawtime;
    time(&rawtime);
    const auto timeinfo = std::localtime(&rawtime);
    std::strftime(buffer.data(), sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    std::string timeStr(buffer.data());
    
    std::vector<std::string> vec;
    boost::algorithm::split(vec,timeStr, boost::is_any_of(" "),boost::token_compress_on); 
    std::string date = vec[0];
    std::string time = vec[1];
    std::string datetime = date + "T" + time; 
    std::map<std::string,std::string> datetimeMap;
    datetimeMap.insert(std::make_pair("date", date));
    datetimeMap.insert(std::make_pair("time", time));
    datetimeMap.insert(std::make_pair("datetime", datetime));
    
    return datetimeMap; 
}

// helper function aim to read file into std::string 
std::string PiNet::readfile( std::string filename){
    std::ifstream file( filename, std::ifstream::binary);
    std::streambuf* raw_buffer = file.rdbuf();
    size_t size = raw_buffer->pubseekoff(0, file.end, file.in);
    raw_buffer->pubseekpos(0, file.in);
    char* buffer = new char[size];
    raw_buffer->sgetn(buffer, size);
    file.close();
    std::stringstream ss;
    ss << buffer;
    delete[] buffer;
    return ss.str();    
}

// core function aim to parse reply from response from server
void PiNet::parseReply(std::string reply){
    // Parse Json into document using rapidjson
    std::string strJson;
    if (reply.find(TRIGGER_RESPONSE_MYSQL)!=std::string::npos){
        int count;
        count = reply.length()-reply.find(TRIGGER_RESPONSE_MYSQL)-TRIGGER_RESPONSE_MYSQL.length();
        strJson = tail(reply, count);

        Document doc;
        if (strJson.length()>2){
            if(!doc.Parse<rapidjson::kParseStopWhenDoneFlag>(strJson.c_str()).HasParseError()){
                // if there are multiple json in [], after parse, doc is a JSON array, loop every element in array
                for (auto& json : doc.GetArray()) {
                    std::cout << std::endl << "parse result: " << std::endl;
                    // loop json to show all key-value 
                    for (Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr){
                        std::cout << itr->name.GetString() << ": "; //key name
                        if (itr->value.IsInt())          // if integer
                            std::cout << itr->value.GetInt() << std::endl;
                        else if (itr->value.IsDouble())  // if 
                            std::cout << itr->value.GetDouble() << std::endl;
                        else if (itr->value.IsString())  // if string
                            std::cout << itr->value.GetString() << std::endl;
                    }
                }
            }
            else{
                    std::cerr << std::endl <<  "parse result : there is no valid content in reply" << std::endl;
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

// core function for sendRequest to server
std::string PiNet::sendRequest(std::string prozedur, std::list<std::string> parameter){
    
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

    client::request httpRequest(url);
    httpRequest << header("Connection", "close")
                << header("Content-Type","application/x-www-form-urlencoded");

    client httpClient;
    client::response httpResponse = httpClient.post(httpRequest, query.str());
    std::string httpResponseBody = body(httpResponse);
    std::cout << std::endl << std::endl << "RESPONSE: " << httpResponseBody << std::endl;
    
    return httpResponseBody;
}

// core function aim to init setname
std::string PiNet::initSet( std::string setname){
    std::list<std::string> parameter;
    parameter.push_back(usr);
    parameter.push_back(pwd);
    parameter.push_back(setname);
    return sendRequest("p_initSet", parameter);
}
// core function aim to delete setname
std::string PiNet::delSet( std::string setname){
    std::list<std::string> parameter;
    parameter.push_back(usr);
    parameter.push_back(pwd);
    parameter.push_back(setname);
    
    return sendRequest("p_delSet", parameter);
}
// core function aim to init variable
std::string PiNet::initVar( std::string setname, int row, int col, std::string datatype, std::string varname, std::string unit){
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
// core function aim to reset value of variable
std::string PiNet::resetVal(std::string setname, std::string varname){
    std::list<std::string> parameter;
    parameter.push_back(usr);
    parameter.push_back(pwd);
    parameter.push_back(setname);
    parameter.push_back(varname);

    return sendRequest("p_resetVal", parameter);
}
// core function aim to delete variable
std::string PiNet::delVar(std::string setname, std::string varname){
    std::list<std::string> parameter;
    parameter.push_back(usr);
    parameter.push_back(pwd);
    parameter.push_back(setname);
    parameter.push_back(varname);

    return sendRequest("p_delVar", parameter);
}

// core function for getting value of variable from databank
std::string PiNet::getVal (std::string setname, std::string varname, std::string datatype){
    
    std::list<std::string> parameter;
    parameter.push_back(usr);
    parameter.push_back(pwd);
    parameter.push_back(setname);
    parameter.push_back(varname);
    parameter.push_back(datatype);

    return sendRequest("p_getVal", parameter);    
} 
// core function for getting mutiple value of variable from databank
std::string PiNet::getVals (std::string setname, std::list<std::string> varnames){
    
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
// core function aim to save value of variable in databank, total 4 override function
std::string PiNet::saveVal (std::string setname, std::string varname, bool value, std::string datatype, int row = 0 , int col = 0){
    
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

std::string PiNet::saveVal (std::string setname, std::string varname, int value, std::string datatype, int row = 0 , int col = 0){
    
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

std::string PiNet::saveVal (std::string setname, std::string varname, double value, std::string datatype, int row = 0 , int col = 0){
    
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

std::string PiNet::saveVal (std::string setname, std::string varname, std::string value, std::string datatype, int row = 0 , int col = 0){
    
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
// core function aim to save multiply variable in databank
std::string PiNet::saveVals (std::string setname, std::list<std::map<std::string,std::string> > varList){
    
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
