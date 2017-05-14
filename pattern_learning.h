//
//  pattern_learning.h
//  miteFinder
//
//  Created by Jialu Hu on 2017/5/11.
//  Copyright © 2017年 Jialu Hu. All rights reserved.
//

#ifndef pattern_learning_h
#define pattern_learning_h

#include "genome.h"
#include <string>
#include <unordered_map>
const int LENGTH_KMER=6;

typedef std::unordered_map<std::string, int> Count_Map;
void kmer_counter(char*,Count_Map&,int);


void countKmerFrequence(Count_Map& kmer_map,std::string filename,int k=LENGTH_KMER){
	Genome seqs;
	seqs.readSequence(filename);
	int numSeq=seqs.getNumChrom();
	for(int i=0;i<numSeq;i++){
		char* pStr=seqs.getChrom(i);
		kmer_counter(pStr,kmer_map,k);
	}
}
void kmer_counter(char* pStr,Count_Map& kmer_map,int k){
	int len=(int)std::strlen(pStr);
	for(int i=0;i<len-k;i++){
		std::string key(pStr+i,k);
		if(kmer_map.find(key)!=kmer_map.end()){
			kmer_map[key]++;
		}else{
			kmer_map[key]=1;
		}
	}
}
void outputMap(Count_Map& tpmap,Count_Map& fnmap, std::fstream& output){
	for(auto it=tpmap.begin();it!=tpmap.end();it++){
		std::string key=it->first;
		if(fnmap.find(key)!=fnmap.end()){
			output << it->first <<"\t"<< tpmap[key] <<"\t"<< fnmap[key] <<std::endl;
		}else{
			output << it->first <<"\t"<<tpmap[key]<<"\t"<<0<<std::endl;
		}
	}
}
void patternLearning(std::string tpfilename,std::string fnfilename,std::fstream& output){
	Count_Map tpMap, fnMap;
	countKmerFrequence(tpMap, tpfilename);
	countKmerFrequence(fnMap, fnfilename);
	outputMap(tpMap,fnMap,output);
}

#endif /* pattern_learning_h */
