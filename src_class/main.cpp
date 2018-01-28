#include <iostream>
#include "pinet.h"

int main()
{   const std::string username = "Xiake";
    const std::string passwort = "123";
    // init object pinet_ from class PiNet
    PiNet pinet_(username,passwort);

    const std::string setname = "TestsetXiake";
    
    const std::string varname = "bool";
    const std::string datatype = "BOOL";
    const bool value = true;

    //const std::string varname = "n";
    //const std::string datatype = "INT32";
    //const int value = 28;

    //const std::string varname = "x";
    //const std::string datatype = "DOUBLE";
    //const double value = 3.14;
    
    //const std::string varname = "str100";
    //const std::string datatype = "STRING100";
    //const std::string value_str100 = "hello world";
    
    //const std::string varname = "strK";
    //const std::string datatype = "STRINGK";
    //const std::string value_strk = "To dream the impossible dream, to fight the unbeatable foe, to bear with unbearable sorrow, and to run where the brave dare not go, to right the unrightable wrong, and to love pure and chaste from afar, to try when your arms are too weary, to reach the unreachable star: this is my quest.To follow that star, no matter how hopeless, no matter how far, to fight for the right, without question or pause, to be willing to march, march into hell, for that heavenly cause, And I know, if I’ll only be true to this glorious quest, that my heart will lie peaceful and calm when I’m laid to my rest and the world will be better for this: That one man, scorned and covered with scars, still strove with his last ounce of courage to reach the unreachable, the unreachable, the unreachable star, and I’ll always dreamthe impossible dream yes, and I’ll reach the unreachable star.  - Joe Darion, Man of La Mancha (1965)";
    
    //const std::string varname = "strM";
    //const std::string datatype = "STRINGM";
    //const std::string value_strM = "One Hundred Years of Solitude begins as a flashback, with Colonel Aureliano Buendía recollecting the years immediately following the founding of Macondo, when a band of gypsies frequently bring technological marvels to the dreamy, isolated village. José Arcadio Buendía, the insatiably curious founder of the town, is obsessed with these magical implements. Using supplies given to him by Melquíades, the leader of the gypsies, he immerses himself in scientific study, to the frustration of his more practical wife, Úrsula Iguarán. Eventually, with Melquíades’s prodding, José Arcadio Buendía begins to explore alchemy, the pseudo-science of making gold out of other metals. He is driven by a desire for progress and by an intense search for knowledge that forces him into solitude. Increasingly, he withdraws from human contact, becoming unkempt, antisocial, and interested only in his pursuit of knowledge. But José Arcadio Buendía is not always a solitary scientist. On the contrary, he is the leader who oversaw the building of the village of Macondo, an idyllic place dedicated to hard work and order, filled with young people, and as yet, unvisited by death.";    

    //const std::string varname = "str100M";
    //const std::string datatype = "STRING100M";
    //const std::string value_str100M = pinet_.readfile("/home/sammysun/PiNet/src/SIDDHARTHA.txt");
    
    auto datetimeMap = pinet_.getDateTime();
     
    //const std::string varname = "time";
    //const std::string datatype = "TIME";
    //const std::string value = datetimeMap["time"];

    
    //const std::string varname = "date";
    //const std::string datatype = "date";
    //const std::string value = datetimeMap["date"];
    
    //const std::string varname = "datetime";
    //const std::string datatype = "DATETIME";
    //const std::string value = datetimeMap["datetime"];    
    

    /*
    std::string reply_initSet = pinet_.initSet("initSet");
    pinet_.parseReply(reply_initSet);
    
    std::string reply_initVar = pinet_.initVar("initSet", 1, 1, datatype, varname, "");
    pinet_.parseReply(reply_initVar);
    
    std::string reply_resetVal = pinet_.resetVal("initSet", varname);
    pinet_.parseReply(reply_resetVal);
    
    std::string reply_delVar = pinet_.delVar("initSet", varname);
    pinet_.parseReply(reply_delVar);
    
    std::string reply_delSet = pinet_.delSet("initSet");
    pinet_.parseReply(reply_delSet);
    
    */
    
    std::string reply_getVal = pinet_.getVal(setname, varname, datatype);
    pinet_.parseReply(reply_getVal);
    
    /*   
    std::list<std::string> varnames;
    varnames.push_back("bool");
    varnames.push_back("n");
    varnames.push_back("x");
    varnames.push_back("str100");
    varnames.push_back("strk");
    varnames.push_back("strM");
    varnames.push_back("str100M");
    varnames.push_back("time");    
    varnames.push_back("date");
    varnames.push_back("datetime");
    std::string reply_getVals = pinet_.getVals(setname, varnames);
    pinet_.parseReply(reply_getVals);
    
       
    std::string reply_saveVal = pinet_.saveVal(setname, varname, value, datatype, 0, 0);
    pinet_.parseReply(reply_saveVal);
    
    
    std::list<std::map<std::string,std::string> > varList;
    varList.push_back(pinet_.valItem("bool", "BOOL", 1, 0, 0));
    varList.push_back(pinet_.valItem("n", "INT32", 32, 0, 0));
    varList.push_back(pinet_.valItem("x", "DOUBLE", 3.14, 0, 0));
    varList.push_back(pinet_.valItem("str100", "STRING100", value_str100, 0, 0));
    varList.push_back(pinet_.valItem("strk", "STRINGK", value_strk, 0, 0));
    varList.push_back(pinet_.valItem("strM", "STRINGM", value_strM, 0, 0));
    varList.push_back(pinet_.valItem("str100M", "STRING100M", value_str100M, 0, 0));
    varList.push_back(pinet_.valItem("time", "TIME", datetimeMap["time"], 0, 0));
    varList.push_back(pinet_.valItem("date", "DATE", datetimeMap["date"], 0, 0));
    varList.push_back(pinet_.valItem("datetime", "DATETIME", datetimeMap["datetime"], 0, 0));
    
    std::string reply_saveVals = pinet_.saveVals(setname, varList);
    pinet_.parseReply(reply_saveVals);
    */
    return 0;
}