//
//  main.cpp
//  miteFinder
//
//  Created by Jialu HU on 16/10/14.
//  Copyright © 2016年 Jialu HU. All rights reserved.
//
#include <iostream>
#include <string>
#include <time.h>
#include "genome.h"
#include "mite.h"
#include "mite_finder.h"
#include "argparser.h"

int main(int argc, const char * argv[]) {
    std::string filename="/Users/jialu/Research/datasets/OSgenomeV6.man";
    bool enable_mismatch=true;
	double seconds=0;
    Genome osgenome;
    ArgParser myparser(argc,argv);
	time_t start_time,end_time;
	time(&start_time);
	osgenome.readSequence(filename);
    int numChr=osgenome.getNumChrom();
    Seed_set tset;
    std::fstream output;
    output.open("/Users/jialu/Research/datasets/mite_candidates.fsa",std::ios_base::out);
    for(int i=0;i<numChr;i++) {
        char* pchr=osgenome.getChrom(i);
        mite_finder(tset,pchr,enable_mismatch,10000,MIN_LENGTH_TIR);
        write_seed(tset,pchr,output,i+1);
        tset.clear();
    }
	time(&end_time);
	seconds= difftime(end_time, start_time);
	std::cout << "The program cost "<<seconds<<" seconds totally to search for MITEs."<<std::endl;
    output.close();
    return 0;
}
