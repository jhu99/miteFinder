//
//  argparser.cpp
//  miteFinder
//
//  Created by Jialu Hu on 2017/5/7.
//  Copyright © 2017年 Jialu Hu. All rights reserved.
//

#include "argparser.h"

ArgParser::ArgParser(int argc, const char** argv){
	const char* pArg;
	for(int i=1;i<argc;i++){
		if(argv[i][0]=='-'){
			pArg=&argv[i][1];
			args.push(std::string(pArg));
		}else{
			parValues.push(argv[i]);
		}
	}
}
ArgParser::~ArgParser(){
    boolOption("help", "");
    boolOption("version", "Show the current version.");
}
void ArgParser::boolOption(const char* optName, const char* help){
    std::string opt=optName;
    ParData obj;
    obj.type=BOOL;
    obj.help=help;
    if(para_map.find(opt)!=para_map.end()){
        std::cerr <<"Warning: Duplicate options were used!"<< std::endl;
    }else{
        para_map[opt]=obj;
    }
}
void ArgParser::refOption(const char *optName, const char *help, int &refVariable,bool mandatory=false){
    std::string opt=optName;
    ParData obj;
    obj.type=INTEGER;
    obj.mandatory=mandatory;
    obj.help=help;
    obj.pInt=&refVariable;
    if(para_map.find(opt)!=para_map.end()){
        std::cerr <<"Warning: Duplicate options were used!"<< std::endl;
    }else{
        para_map[opt]=obj;
    }
}
void ArgParser::refOption(const char *optName, const char *help, std::string &refVariable,bool mandatory=false){
    std::string opt=optName;
    ParData obj;
    obj.type=STRING;
    obj.mandatory=mandatory;
    obj.help=help;
    obj.pString=&refVariable;
    if(para_map.find(opt)!=para_map.end()){
        std::cerr <<"Warning: Duplicate options were used!"<< std::endl;
    }else{
        para_map[opt]=obj;
    }
}
void ArgParser::refOption(const char *optName, const char *help, double &refVariable,bool mandatory=false){
    std::string opt=optName;
    ParData obj;
    obj.type=DOUBLE;
    obj.mandatory=mandatory;
    obj.help=help;
    obj.pDouble=&refVariable;
    if(para_map.find(opt)!=para_map.end()){
        std::cerr <<"Warning: Duplicate options were used!"<< std::endl;
    }else{
        para_map[opt]=obj;
    }
}


