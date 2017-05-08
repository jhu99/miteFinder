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
	std::unordered_map<std::string, std::string> para_map;
	std::queue<std::string> args;
	std::queue<std::string> parValues;
public:
	ArgParser(int,char**);
	~ArgParser();
	void boolOption(char*,char*);
	void refOption(char*,char*,std::string&);
	void refOption(char*,char*,int&);
	void refOption(char*,char*,float&);
	void run();
};


#endif /* parser_h */
