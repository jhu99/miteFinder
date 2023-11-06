#include <iostream>
#include <string>
#include <time.h>
#include "genome.h"
#include "mite.h"
#include "mite_finder.h"
#include "pattern_scoring.h"
#include "argparser.h"
#include "repeat.h"
struct Option{
    std::string inputfilename;
    std::string outputfilename;
    std::string patternfilename;
    double threshold;
    int fragnment_length;
    Option(){
    }
};

int main(int argc, const char * argv[]) {
	double seconds = 0;
    ArgParser rp_parser;
    Option rp_option;
    Genome osgenome;
    
    // Parser the argument.
    rp_parser.refOption("input", "The path of an input file.", rp_option.inputfilename, "",true);
    //rp_parser.refOption("pattern_scoring", "The path of a scoring file.", rp_option.patternfilename, "./profile/pattern_scoring.txt");
    rp_parser.refOption("output", "The path of an output file. Default is ./default_output.fsa", rp_option.outputfilename, "./defaut_output.fsa");
    //rp_parser.refOption("fragnment_length", "Length of fragnment. Default is 10000.", rp_option.fragnment_length, 10000);
    //rp_parser.refOption("threshold", "Threshold of removing mite candidates with low-confidence score. Default is 0.2.", rp_option.threshold, 0.2);
    if(!rp_parser.run(argc, argv))
        return 1;
    
    // Read DNA sequences from the input file.
	time_t start_time,end_time;
	time(&start_time);
	osgenome.readSequence(rp_option.inputfilename);
    int numChr = osgenome.getNumChrom();
    std::cout << "num:"<< numChr<<std::endl;//5
    std::cout << "length:" <<std::endl;//输出每条染色体长度
    // //vector<int>::iterator itChr = osgenome.sizeChrom.begin();
    // for(i = 0,i < numChr;i++){
    //         cout << *itChr << " ";
    // }

    Seed_set tset;
    std::fstream output;
    output.open(rp_option.outputfilename,std::ios_base::out);
    
    
    for(int i = 0;i < numChr;i++) {//遍历每一条染色体
        char* pchr = osgenome.getChrom(i);
        cout<<"now it is the "<<i<<" chromosome"<<endl;
		//mite_finder(tset,pchr,mf_option.disable_mismatch,mf_option.fragnment_length,MIN_LENGTH_TIR);
        //filter_low_score_candidates(tset,pchr,pattern_map,mf_option.threshold);
        repeat_finder(pchr,0,10);
        //std::cout << "#Sequence "<<i <<": "<< tset.size() <<std::endl;
        //write_seed(tset,pchr,output,i+1);
        //tset.clear();
    }
    
	time(&end_time);
    
    // Show the processing duration.
	seconds= difftime(end_time, start_time);
	std::cout << "##############\n#The program cost "<<seconds<<" seconds totally to search for repeats."<<std::endl;
	output.close();
    return 0;
}

