#include <iostream>
#include <string>
#include <time.h>
#include "genome.h"
#include "mite.h"
#include "argparser.h"
#include "ltr_finder.h"

struct Option{
    std::string inputfilename;
    std::string outputfilename;
    bool help;
    bool version;
    /*结构体名(){函数体}可以实现结构体的初始化*/
    Option(){ 
      //  this->disable_mismatch = 0;  
    }
};

int disable_mismatch = 0;

int main(int argc, const char * argv[]) {
	double seconds=0;

    ArgParser mf_parser;
    Option ltrf_option;
    Genome osgenome;
    
    // Parser the argument.
    mf_parser.setName("LtrFinder", "An application for detecting long terminal repeat-retrotransposons on a genome-wide scale.");
    mf_parser.setVerion("1.0.000");
    mf_parser.refOption("help", "Show help information.", ltrf_option.help);
    mf_parser.refOption("version", "Show the current version.", ltrf_option.version);
    mf_parser.refOption("input", "The path of an input file.", ltrf_option.inputfilename, "",true);
    mf_parser.refOption("output", "The path of an output file. Default is ./default_output.fsa", ltrf_option.outputfilename, "./defaut_output.fsa");
    //mf_parser.refOption("disable_mismatch", "Int. It represents the maximum allowed number of mismatched bases. Default is 0.", ltrf_option.disable_mismatch);
    if(!mf_parser.run(argc, argv))
        return 1;
    
    // Read DNA sequences from the input file.
	time_t start_time,end_time;
	time(&start_time);
	osgenome.readSequence(ltrf_option.inputfilename);
    int numChr=osgenome.getNumChrom();/*拟南芥：5条染色体*/
    cout<<"拟南芥："<<numChr<<"条染色体"<<endl;
    std::fstream output;
    match_set lset;
    // Write LTRs to the output file.
    output.open(ltrf_option.outputfilename,std::ios_base::out);

    for(int i=0;i<numChr;i++) {
        char* pchr=osgenome.getChrom(i);
        // pchr：获取每一条染色体序列
        cout<<"第"<<i<<"条染色体"<<endl;
        ltr_finder(pchr,lset,disable_mismatch);
		// mite_finder(tset,pchr,mf_option.disable_mismatch,mf_option.fragnment_length,MIN_LENGTH_TIR);
        // filter_low_score_candidates(tset,pchr,pattern_map,mf_option.threshold);
        // std::cout << "#Sequence "<<i <<": "<< tset.size() <<std::endl;
        // write_seed(tset,pchr,output,i+1);
        // tset.clear();
    }
	time(&end_time);
    
    // Show the processing duration.
	seconds= difftime(end_time, start_time);
	std::cout << "##############\n#The program cost "<<seconds<<" seconds totally to search for LTRs."<<std::endl;
	output.close();
    return 0;
}
