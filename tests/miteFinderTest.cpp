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
#include "pattern_scoring.h"
#include "argparser.h"

struct Option{
    std::string inputfilename;
    std::string outputfilename;
    std::string patternfilename;
    double threshold;
    bool help;
    bool version;
    bool disable_mismatch;
    int fragnment_length;
    Option(){
    }
};

int main(int argc, const char * argv[]) {
	double seconds=0;
    ArgParser mf_parser;
    Option mf_option;
    Genome osgenome;
    
    // Parser the argument.
    mf_parser.setName("MiteFinder", "An application for detecting miniature inverted-repeat transposable elements on a genome-wide scale.");
    mf_parser.setVerion("1.0.006");
    mf_parser.refOption("help", "Show help information.", mf_option.help);
    mf_parser.refOption("version", "Show the current version.", mf_option.version);
    mf_parser.refOption("input", "The path of an input file.", mf_option.inputfilename, "",true);
    mf_parser.refOption("pattern_scoring", "The path of a scoring file.", mf_option.patternfilename, "./profile/pattern_scoring.txt");
    mf_parser.refOption("output", "The path of an output file. Default is ./default_output.fsa", mf_option.outputfilename, "./defaut_output.fsa");
    mf_parser.refOption("fragnment_length", "Length of fragnment. Default is 10000.", mf_option.fragnment_length, 10000);
    mf_parser.refOption("disable_mismatch", "Logical. It can disable the detection of mismatch base pairs if 1, otherwise 0. Default is 0.", mf_option.disable_mismatch);
    mf_parser.refOption("threshold", "Threshold of removing mite candidates with low-confidence score. Default is 0.2.", mf_option.threshold, 0.2);
    if(!mf_parser.run(argc, argv))
        return 1;
    
    // Read DNA sequences from the input file.
    std::unordered_map<std::string, Pattern_value> pattern_map;
	time_t start_time,end_time;
	time(&start_time);
	osgenome.readSequence(mf_option.inputfilename);
    readscore(pattern_map,mf_option.patternfilename);
    int numChr=osgenome.getNumChrom();
    Seed_set tset;
    std::fstream output;
    
    // Write MITEs to the output file.
    output.open(mf_option.outputfilename,std::ios_base::out);
    for(int i=0;i<numChr;i++) {
        char* pchr=osgenome.getChrom(i);
		mite_finder(tset,pchr,mf_option.disable_mismatch,mf_option.fragnment_length,MIN_LENGTH_TIR);
        filter_low_score_candidates(tset,pchr,pattern_map,mf_option.threshold);
        std::cout << "#Sequence "<<i <<": "<< tset.size() <<std::endl;
        write_seed(tset,pchr,output,i+1);
        tset.clear();
    }
	time(&end_time);
    
    // Show the processing duration.
	seconds= difftime(end_time, start_time);
	std::cout << "##############\n#The program cost "<<seconds<<" seconds totally to search for MITEs."<<std::endl;
	output.close();
    return 0;
}
