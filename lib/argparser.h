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
    std::string commandDesc;
    std::string commandVersion;
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
	ArgParser();
	~ArgParser();
    void setName(const char*,const char*);
    void setVerion(const char*);
	void refOption(const char*,const char*,bool&,bool dft=false,bool mandatory=false);
	void refOption(const std::string &name,const std::string &help,std::string&,std::string,bool mandatory=false);
	void refOption(const std::string &name,const std::string &help,int&,int,bool mandatory=false);
	void refOption(const std::string &name,const std::string &help,double&,double,bool mandatory=false);
	bool run(int,const char**);
	bool checkMandatories();
	bool checkParaValue(int,const char**);
	void showUsages();
	void showOptions();
	void showVersion();
	void showLicense();
};


#endif /* parser_h */
