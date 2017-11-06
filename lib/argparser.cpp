//
//  argparser.cpp
//  miteFinder
//
//  Created by Jialu Hu on 2017/5/7.
//  Copyright © 2017年 Jialu Hu. All rights reserved.
//

#include "argparser.h"
#include <stdlib.h>

ArgParser::ArgParser(){
}
ArgParser::~ArgParser(){
}
void ArgParser::setName(const char* comName,const char* comDesc)
{
    commandName=comName;
    commandDesc=comDesc;
}
void ArgParser::setVerion(const char* version)
{
    commandVersion=version;
}
void ArgParser::refOption(const char* optName, const char* help, bool &refVariable, bool dft,bool mandatory){
    std::string opt=optName;
    ParData obj;
    obj.type=BOOL;
    obj.help=help;
    obj.mandatory=mandatory;
    obj.pBool=&refVariable;
    *obj.pBool=dft;
    if(para_map.find(opt)!=para_map.end()){
        std::cerr <<"Warning: Duplicate options were used!"<< std::endl;
    }else{
        para_map[opt]=obj;
    }
}
void ArgParser::refOption(const std::string &optName,const std::string &help,
                                int &refVariable,int dft,bool mandatory){
    std::string opt=optName;
    ParData obj;
    obj.type=INTEGER;
    obj.mandatory=mandatory;
    obj.help=help;
    obj.pInt=&refVariable;
	*obj.pInt=dft;
    if(para_map.find(opt)!=para_map.end()){
        std::cerr <<"Warning: Duplicate options were used!"<< std::endl;
    }else{
        para_map[opt]=obj;
    }
}
void ArgParser::refOption(const std::string &optName,const std::string &help,
						  std::string &refVariable,std::string dft, bool mandatory){
    std::string opt=optName;
    ParData obj;
    obj.type=STRING;
    obj.mandatory=mandatory;
    obj.help=help;
    obj.pString=&refVariable;
	*obj.pString=dft;
    if(para_map.find(opt)!=para_map.end()){
        std::cerr <<"Warning: Duplicate options were used!"<< std::endl;
    }else{
        para_map[opt]=obj;
    }
}
void ArgParser::refOption(const std::string &optName,const std::string &help,
                                double &refVariable,double dft, bool mandatory){
    std::string opt=optName;
    ParData obj;
    obj.type=DOUBLE;
    obj.mandatory=mandatory;
    obj.help=help;
    obj.pDouble=&refVariable;
	*obj.pDouble=dft;
    if(para_map.find(opt)!=para_map.end()){
        std::cerr <<"Warning: Duplicate options were used!"<< std::endl;
    }else{
        para_map[opt]=obj;
    }
}
void ArgParser::showUsages()
{
	std::cerr <<"USAGE: "<< commandName<<" ";
	for(std::unordered_map<std::string, ParData>::iterator it=para_map.begin();it!=para_map.end();it++)
	{
		if(!it->second.mandatory)
			std::cerr <<" [";
        else{
            std::cerr <<" ";
        }
		if(it->second.type==STRING)
			std::cerr <<"-"<<it->first<<" STRING";
		if(it->second.type==INTEGER)
			std::cerr <<"-"<<it->first<<" INTEGER";
		if(it->second.type==DOUBLE)
		std::cerr <<"-"<<it->first<<" DOUBLE";
		if(it->second.type==BOOL)
			std::cerr <<"-"<<it->first;
		if(!it->second.mandatory)
			std::cerr <<"]";
	}
	std::cerr <<std::endl;
    std::cerr <<"DESCRIPTION: "<<commandDesc<<std::endl;
}
void ArgParser::showVersion()
{
	std::cerr <<"VERSION: "<<commandName<<" "<<commandVersion<<std::endl;
}
void ArgParser::showOptions()
{
    std::cerr <<"ALL MANDATORY AND OPTIONAL ARGUMENTS: "<<std::endl;
    for(std::unordered_map<std::string, ParData>::iterator it=para_map.begin();it!=para_map.end();it++)
    {
        std::cerr << "-" << it->first<<"\t";
		if(it->second.mandatory)
		{
			std::cerr <<"A mandatory option. ";
		}
		std::cerr<< it->second.help<< std::endl;
    }
}
void ArgParser::showLicense(){
	std::cerr <<"Show License Here!"<<std::endl;
}
bool ArgParser::checkParaValue(int pos,const char** argv)
{
	if(pos>=optnum || argv[pos][0]=='-'){
		std::cerr <<"Error: Please give the value of parameter \""<<argv[pos-1]<<"\"."<<std::endl;
		showUsages();
		return false;
	}
	return true;
}
bool ArgParser::checkMandatories()
{
    for (std::unordered_map<std::string, ParData>::iterator it=para_map.begin();it!=para_map.end();it++)
    {
        if (it->second.mandatory==1 && it->second.sign==false)
        {
            std::cerr <<"Error: You forgot to give the mandatory parameter: "<<it->first<<"."<< std::endl;
			showUsages();
            return false;
        }
    }
    return true;
}
bool ArgParser::run(int argc, const char** argv)
{
    // check mandatory
    // check help version
    //
	optnum=argc;
	const char* pArg;
	std::string optname;
	for(int i=1;i<argc;i++){
		if(argv[i][0]=='-'){
			pArg=&argv[i][1];
			optname=pArg;
			args.push(optname);
			std::unordered_map<std::string, ParData>::iterator it=para_map.find(optname);
			if(it!=para_map.end()){
				it->second.sign=true;
				switch (it->second.type) {
					case UNKNOWN:
						break;
					case BOOL:
						if (!optname.compare("help")) {
							showUsages();
							showVersion();
							showOptions();
							return false;
						}else if (!optname.compare("version"))
						{
							showVersion();
							return false;
						}else{
                            *it->second.pBool=true;
						}
						break;
					case STRING:
						if(!checkParaValue(i+1,argv))return false;
						*it->second.pString=argv[i+1];
						i++;
						break;
					case DOUBLE:
						if(!checkParaValue(i+1, argv))return false;
						*it->second.pDouble=atof(argv[i+1]);
						i++;
						break;
					case INTEGER:
						if(!checkParaValue(i+1, argv))return false;
						*it->second.pInt=atoi(argv[i+1]);
						i++;
						break;
					default:
						break;
				}
			}else{
				std::cerr <<"Error: The parameter \""<< optname<<"\" doesn't exit!"<<std::endl;
				showUsages();
				showVersion();
				showOptions();
				return false;
			}
		}else{
			std::cerr <<"WARNING: There is an invalid parameter value."<<std::endl;
		}
	}
    return ArgParser::checkMandatories();
}
