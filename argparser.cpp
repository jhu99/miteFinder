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
}

