//
//  argparser.h
//  miteFinder
//
//  Created by Jialu Hu on 2017/5/7.
//  Copyright © 2017年 Jialu Hu. All rights reserved.
//

#ifndef argparser_h
#define argparser_h

#include <unordered_map>
#include <string>
#include <queue>
#include <iostream>

class ArgParser
{
private:
	int optnum;
	std::queue<std::string> args;
	std::queue<std::string> parValues;
    std::string commandName;
    enum OptType { UNKNOWN=0, BOOL=1, STRING=2, DOUBLE=3, INTEGER=4, FUNC=5 };
    class ParData{
    public:
        union{
            bool *pBool;
            int* pInt;
            double* pDouble;
            std::string* pString;
        };
        std::string help;
        bool mandatory;
		bool sign;
        OptType type;
        bool has_syn;
        ParData():mandatory(false), sign(false), type(UNKNOWN), has_syn(false){
        }
        ~ParData(){}
    };
    std::unordered_map<std::string, ParData> para_map;
    static void _showHelp(void *p);
public:
	ArgParser(int,const char**);
	~ArgParser();
	void boolOption(const char*,const char*,bool mandatory=false);
	void refOption(const std::string &name,const std::string &help,std::string&,bool mandatory=false);
	void refOption(const std::string &name,const std::string &help,int&,bool mandatory=false);
	void refOption(const std::string &name,const std::string &help,double&,bool mandatory=false);
    //void &refOption(const std::string &name,const std::string &help,bool&,bool mandatory=false);
	bool run();
	bool checkMandatories();
	void showUsages();
	void showOptions();
};


#endif /* parser_h */
