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

struct Option{
    std::string inputfilename;
    std::string outputfilename;
    bool enable_mismatch;
    int fragnment_length;
    Option(){
        enable_mismatch=true;
        fragnment_length=10000;
    }
};

int main(int argc, const char * argv[]) {
	double seconds=0;
    ArgParser mf_paser;
    Option mf_option;
    Genome osgenome;
    
    // Read DNA sequences from the input file.
	time_t start_time,end_time;
	time(&start_time);
	osgenome.readSequence(mf_option.inputfilename);
    int numChr=osgenome.getNumChrom();
    Seed_set tset;
    std::fstream output;
    
    // Write MITEs to the output file.
    output.open(mf_option.outputfilename,std::ios_base::out);
    for(int i=0;i<numChr;i++) {
        char* pchr=osgenome.getChrom(i);
		mite_finder(tset,pchr,mf_option.enable_mismatch,mf_option.fragnment_length,MIN_LENGTH_TIR);
        write_seed(tset,pchr,output,i+1);
        tset.clear();
    }
	time(&end_time);
    
    // Show the processing duration.
	seconds= difftime(end_time, start_time);
	std::cout << "The program cost "<<seconds<<" seconds totally to search for MITEs."<<std::endl;
	output.close();
    return 0;
}
