#include <iostream>
#include <sstream>
#include <map>   

//Due to requirement for separate compilation and templates are instantiation-style polymorphism. we can not actually sepreated template methode in .h and .cpp, so we write it in pinet.hpp and include it in pinet.h

//helper function for check if value is number
template<typename T1>
bool PiNet::isNumber(const T1& x) {
   std::string s;
   std::stringstream ss; 
   ss << x;
   ss >> s;
   if(s.empty() || std::isspace(s[0]) || std::isalpha(s[0])) return false ;
   char * p ;
   strtod(s.c_str(), &p) ;
   return (*p == 0) ;
}

// helper function to make std::map object to make key-value mapping
template<typename T2>
std::map<std::string,std::string> PiNet::valItem(const std::string& varname, const std::string& datatype, T2 value, int row, int col){
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
