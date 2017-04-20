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
    std::string filename="OSgenomeV6.1";
    Genome osgenome;
    osgenome.readGenome(filename);
    int numChr=osgenome.getNumChrom();
    Seed_set tset;
    std::fstream output;
    output.open("mite_candidates.fsa",std::ios_base::out);
    for(int i=0;i<numChr;i++) {
        char* pchr=osgenome.getChrom(i);
        int chrlen=osgenome.getSizeChrom(i);
        mite_finder(tset,pchr,10000);
        write_seed(tset,pchr,output,i+1,chrlen);
        tset.clear();
    }
    output.close();
    return 0;
}
