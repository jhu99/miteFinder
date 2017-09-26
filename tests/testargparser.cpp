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
    Option(){
        integer=1;
        filename="test";
        ratio=0.01;
    }
};

int main(int argc, const char * argv[]) {
    // insert code here...
    Option opt;
    ArgParser parser(argc,argv);
    parser.refOption("input", "The path of input file.", opt.filename,1);
    parser.refOption("num", "The number of species.", opt.integer,0);
    parser.refOption("ratio", "The ratio of something.", opt.ratio,0);
    if (!parser.run())
		return 0;
    else
		std::cout << opt.integer << opt.filename << opt.ratio << std::endl;
    return 0;
}
