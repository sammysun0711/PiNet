#include <iostream>
#include <sstream>
#include <boost/network.hpp>
#include <boost/network/uri.hpp>
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

//helper function for concrete [] part from httpResponse        
std::string tail(std::string const& source, size_t const length) {
  if (length >= source.size()) { return source; }
  return source.substr(source.size() - length);
}

// parse reply from response from server
// TO DO: Add Situation for two TRIGGER_ERROR
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
std::string sendRequest(std::string prozedur, std::string parameter){
    std::stringstream query;
    query << "prozedur=" << uri::encoded(prozedur) << parameter;
    //cout<< endl << endl << query.str() << endl;

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
    std::stringstream parameter;
    std::string vtyp = "string";

    parameter  << "&username=" << uri::encoded(usr)
               << "&passwort=" << uri::encoded(pwd)
               << "&vtyp1=" << uri::encoded(vtyp)
               << "&variable1=" << uri::encoded(usr)
               << "&vtyp2=" << uri::encoded(vtyp)
               << "&variable2=" << uri::encoded(pwd)
               << "&vtyp3=" << uri::encoded(vtyp)
               << "&variable3=" << uri::encoded(setname)
               << "&vtyp4=" << uri::encoded(vtyp)
               << "&variable4=" << uri::encoded(varname)
               << "&vtyp5=" << uri::encoded(vtyp)
               << "&variable5=" << uri::encoded(datatype);
    return sendRequest("p_getVal", parameter.str());    
} 

// helper function for saving value of variable in databank 
std::string saveVal (std::string setname, std::string varname, int n, std::string datatype){
    std::stringstream parameter;
    std::string vtyp = "string";
    std::string zahl = "zahl";
    parameter  << "&username=" << uri::encoded(usr)
               << "&passwort=" << uri::encoded(pwd)
               << "&vtyp1=" << uri::encoded(vtyp)
               << "&variable1=" << uri::encoded(usr)
               << "&vtyp2=" << uri::encoded(vtyp)
               << "&variable2=" << uri::encoded(pwd)
               << "&vtyp3=" << uri::encoded(vtyp)
               << "&variable3=" << uri::encoded(setname)
               << "&vtyp4=" << uri::encoded(vtyp)
               << "&variable4=" << uri::encoded(varname)
               << "&vtyp5=" << uri::encoded(vtyp)
               << "&variable5=" << uri::encoded(std::to_string(n))
               << "&vtyp6=" << uri::encoded(vtyp)
               << "&variable6=" << uri::encoded(datatype)
               << "&vtyp7=" << uri::encoded(zahl)
               << "&variable7=" << uri::encoded(std::to_string(0))
               << "&vtyp8=" << uri::encoded(zahl)
               << "&variable8=" << uri::encoded(std::to_string(0));

    return sendRequest("p_saveValItem", parameter.str());    
}
/**
void sendRequest(std::string prozedur, std::list<std::string> parameter){
    std::stringstream formParams;
    formParmas << "prozedur=" << uri::encoded(prozedur)
               << "&username=" << uri::encoded(usr)
               << "&passwort=" << uri::encoded(pwd);
    
	for (std::list<std::string>::const_iterator iterator = parameter.begin(); iterator != parameter.end(); ++iterator)
	{
        // set vtyp default to be "string"
        std::string vtyp = "string";

		bool isNumber = false;
        //if parameter->at(i) is a double, then parameter->at(i).toInt(&isNumber) do not work, set isNumber=false
        isNumber = std::stoi(*iterator);
		if (!isNumber)
        //if parameter->at(i) is a double, then parameter->at(i).toDouble(&isNumber) works,and set isNumber=true
            isNumber = std::stod(*iterator);    
		formParams << "&variable" << std::to_string(iterator+1) <<*iterator; 
        //qu.addQueryItem("variable" + QString::number(i + 1), parameter->at(i));
		if (*iterator == "0")
			vtyp = "zero";
		//else if (*iterator.parameter->at(i).isNull()
		//	vtyp = "null";
		else if (*iterator.empty() || *iterator == "")
			vtyp = "emptyString";
		else if (isNumber)
            vtyp = "zahl";
        formParams << "&vtyp" << std::to_string(iterator+1) <<vtyp; 
	}
    
}

std::string getVal(std::string setname, std::string varname, std::string datatype){
    std::stringstream parameter;
    parameter << usr << pwd << setname << varname << datatype; 
    sendRequest("p_getVal", parameter);  
}


**/

int main()
{   
    const std::string setname = "TestsetXiake";
    const std::string varname = "n";
    const std::string datatype = "INT32";
    const int value = 28;
    
    std::string reply_getVal = getVal(setname, varname, datatype);
    parse(reply_getVal, setname, varname, datatype);

    std::string reply_saveVal = saveVal(setname, varname, value, datatype);
    parse(reply_saveVal, setname, varname, datatype);
    
    return 0;
}

