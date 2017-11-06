//
//  main.cpp
//  TestArgParser
//
//  Created by Jialu HU on 17/9/18.
//  Copyright © 2017年 Jialu HU. All rights reserved.
//

#include "argparser.h"
#include <iostream>
#include <string>

struct Option{
    int integer;
    std::string filename;
    double ratio;
    bool help;
    bool version;
    Option(){
        integer=1;
        filename="test";
        ratio=0.01;
    }
};

int main(int argc, const char * argv[]) {
    // insert code here...
    Option opt;
    ArgParser parser;
    parser.setName("ArgParserTest","This is a test for parsing arguments.");
    parser.setVerion("0.0.066");
    parser.refOption("help","Show help information.",opt.help,true);
    parser.refOption("version","Show the current version.",opt.version,true);
    parser.refOption("input", "The path of input file.", opt.filename,"",1);
    parser.refOption("num", "The number of species.", opt.integer,6,0);
    parser.refOption("ratio", "The ratio of something.", opt.ratio,0.6,0);
    if (!parser.run(argc,argv))
		return 0;
    else
		std::cout << opt.integer << opt.filename << opt.ratio << std::endl;
    return 0;
}
