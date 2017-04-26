//
//  main.cpp
//  miteFinderTest
//
//  Created by Jialu HU on 16/10/14.
//  Copyright © 2016年 Jialu HU. All rights reserved.
//
#include <iostream>
#include <string>
#include "genome.h"
#include "mite.h"
#include "mite_finder.h"

int main(int argc, const char * argv[]) {
    std::string filename="/Users/jialu/Research/datasets/OSgenomeV6.man";
    bool enable_mismatch=false;
    Genome osgenome;
    osgenome.readGenome(filename);
    int numChr=osgenome.getNumChrom();
    Seed_set tset;
    std::fstream output;
    output.open("./mite_candidates.fsa",std::ios_base::out);
    for(int i=0;i<numChr;i++) {
        char* pchr=osgenome.getChrom(i);
        mite_finder(tset,pchr,enable_mismatch,10000,MIN_LENGTH_TIR);
        write_seed(tset,pchr,output,i+1);
        tset.clear();
    }
    output.close();
    return 0;
}
