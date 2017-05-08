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

class ArgParser
{
private:
	std::queue<std::string> args;
	std::queue<std::string> parValues;
    std::string commandName;
    enum OptType { UNKNOWN=0, BOOL=1, STRING=2, DOUBLE=3, INTEGER=4, FUNC=5 };
    class ParData{
        union{
            bool *pBool;
            int* pInt;
            double* pDouble;
            std::string* pString;
        };
        std::string help;
        bool mandatory;
        OptType type;
        bool has_syn;
        ParData():mandatory(false), type(UNKNOWN), has_syn(false){
            
        }
    };
    std::unordered_map<std::string, ParData> para_map;
    static void _showHelp(void *p);
public:
	ArgParser(int,const char**);
	~ArgParser();
	void boolOption(const char*,const char*);
	void refOption(const char*,const char*,std::string&);
	void refOption(const char*,const char*,int&);
	void refOption(const char*,const char*,float&);
	void run();
};


#endif /* parser_h */
