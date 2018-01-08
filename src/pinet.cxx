#include <iostream>
#include <sstream>
#include <boost/network.hpp>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include <string>

using namespace std;
using namespace boost::network;
using namespace boost::network::http;
using namespace rapidjson; 

const std::string TRIGGER_RESPONSE_MYSQL = "??**wejnonwr4234gTRIGGER_RESPONSE_MYSQL**??";
const std::string usr = "Xiake";
const std::string pwd = "123";
const std::string setname = "TestsetXiake";
const std::string INT32 = "INT32";

std::string tail(std::string const& source, size_t const length) {
  if (length >= source.size()) { return source; }
  return source.substr(source.size() - length);
} 

/**
void sendRequest(std::string prozedur, std::stringstream *parameter){
    std::stringstream formParams;
    formParmas << "prozedur=" << uri::encoded(prozedur)
               << "&username=" << uri::encoded(usr)
               << "&passwort=" << uri::encoded(pwd);
    
	for (int i = 0; i < parameter.size(); i++)
	{
        // set vtyp default to be "string"
        std::string vtyp = "string";

		bool isNumber = false;
        //if parameter->at(i) is a double, then parameter->at(i).toInt(&isNumber) do not work, set isNumber=false
		parameter->at(i).toInt(&isNumber);
		if (!isNumber)
        //if parameter->at(i) is a double, then parameter->at(i).toDouble(&isNumber) works,and set isNumber=true
            parameter->at(i).toDouble(&isNumber);
		qu.addQueryItem("variable" + QString::number(i + 1), parameter->at(i));
		if (parameter->at(i) == "0")
			vtyp = "zero";
		else if (parameter->at(i).isNull())
			vtyp = "null";
		else if (parameter->at(i).isEmpty() || parameter->at(i) == "")
			vtyp = "emptyString";
		else if (isNumber)
            vtyp = "zahl";
		qu.addQueryItem("vtyp" + QString::number(i + 1), vtyp);
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
 
    // HTTP request object
    //client::request httpRequest("http://posttestserver.com/post.php?dump");
    client::request httpRequest("http://codset.lorch-group.de/PROCinterface.php");
    httpRequest << header("Connection", "close")
                << header("Content-Type","application/x-www-form-urlencoded");
 
    // Form parameters
    // Prozedur p_getVal
    
    std::stringstream formParams;
    //std::list<std::string> formParams;
    formParams << "prozedur=" << uri::encoded("p_getVal")
               << "&username=" << uri::encoded("Xiake")
               << "&passwort=" << uri::encoded("123")
               << "&vtyp1=" << uri::encoded("string")
               << "&variable1=" << uri::encoded("Xiake")
               << "&vtyp2=" << uri::encoded("string")
               << "&variable2=" << uri::encoded("123")
               << "&vtyp3=" << uri::encoded("string")
               << "&variable3=" << uri::encoded("TestsetXiake")
               << "&vtyp4=" << uri::encoded("string")
               << "&variable4=" << uri::encoded("n")
               << "&vtyp5=" << uri::encoded("string")
               << "&variable5=" << uri::encoded("INT32");
    
    /**
    // Prozedur p_saveValItem
    std::stringstream formParams;
    formParams << "prozedur=" << uri::encoded("p_saveValItem")
               << "&username=" << uri::encoded("Xiake")
               << "&passwort=" << uri::encoded("123")
               << "&vtyp1=" << uri::encoded("string")
               << "&variable1=" << uri::encoded("Xiake")
               << "&vtyp2=" << uri::encoded("string")
               << "&variable2=" << uri::encoded("123")
               << "&vtyp3=" << uri::encoded("string")
               << "&variable3=" << uri::encoded("TestsetXiake")
               << "&vtyp4=" << uri::encoded("string")
               << "&variable4=" << uri::encoded("n")
               << "&vtyp5=" << uri::encoded("string")
               << "&variable5=" << uri::encoded("92")
               << "&vtyp6=" << uri::encoded("string")
               << "&variable6=" << uri::encoded("INT32")
               << "&vtyp7=" << uri::encoded("zahl")
               << "&variable7=" << uri::encoded("0")
               << "&vtyp8=" << uri::encoded("zahl")
               << "&variable8=" << uri::encoded("0");
    **/          
    // Print the form params
    cout << endl << "FORM PARAMS: " << formParams.str();
 
    // Http client and sync post request
    client httpClient;
    client::response httpResponse = httpClient.post(httpRequest, formParams.str());
 
    // Print the HTTP response
    std::string httpResponseBody = body(httpResponse);
    cout << endl << endl << "RESPONSE: " << httpResponseBody;

    // Parse Json into document using rapidjson
    std::string strJson;
    if (httpResponseBody.find(TRIGGER_RESPONSE_MYSQL)!=std::string::npos)
    {
        int count;
        count =httpResponseBody.length()-httpResponseBody.find(TRIGGER_RESPONSE_MYSQL)-TRIGGER_RESPONSE_MYSQL.length();
        strJson = tail(httpResponseBody, count);
        cout<< endl<<endl<<strJson<<endl;
    }
    
    // if there is json in [], parse json into doc, otherweise return 0
    if (strJson.length()>2){
    strJson.erase(0,1);
    strJson.erase(strJson.size()-1);
    cout<< endl<<endl<<strJson<<endl;

    Document doc;
    try{
        if(doc.Parse<0>(strJson.c_str()).HasParseError())
        cout << "ERROR" << endl;
        else
        cout << "OK" << endl;
    
        doc.Parse(strJson.c_str());
        Value& setname = doc["setname"];
        Value& varname = doc["varname"];
        Value& datatype = doc["datatype"];
        Value& value = doc["value"];
        cout << "setname = " << setname.GetString()<<endl;
        cout << "varname = " << varname.GetString()<<endl;
        cout << "datatype = " << datatype.GetString()<<endl;
        cout << "value = " << value.GetString()<<endl;
    } 

    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    }
    return 0;
}

