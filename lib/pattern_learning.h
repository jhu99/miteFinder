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
#include <cstring>
#include <string>
#include <math.h>
#include <unordered_map>
const int LENGTH_KMER=6;

typedef std::unordered_map<std::string, int> Count_Map;
void kmer_counter(char*,Count_Map&,int);


int countKmerFrequence(Count_Map& kmer_map,std::string filename,int k=LENGTH_KMER){
	Genome seqs;
	seqs.readSequence(filename);
	int numSeq=seqs.getNumChrom();
	int seqLen=0;
	for(int i=0;i<numSeq;i++){
		char* pStr=seqs.getChrom(i);
		kmer_counter(pStr,kmer_map,k);
		seqLen+=strlen(pStr);
	}
	return seqLen;
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
void outputMap(Count_Map& tpmap,Count_Map& fpmap, std::fstream& output, int seqlenTp, int seqlenFP){
	std::string key;
	output <<"seqlenTp:"<<seqlenTp<<std::endl;
	output <<"seqlenFp:"<<seqlenFP<<std::endl;
	double score1=0,score2=0,ratio=0;
	ratio=1.0*seqlenFP/seqlenTp;
	for(auto it=tpmap.begin();it!=tpmap.end();it++){
		key=it->first;
		if(fpmap.find(key)!=fpmap.end()){
			score1=log2((tpmap[key]*ratio)/fpmap[key]);
			score2=log2((tpmap[key]*1.0)/fpmap[key]);
			output << it->first <<"\t"<< score1 <<"\t"<<score2<<"\t"<< tpmap[key] <<"\t"<<fpmap[key] <<std::endl;
		}else{
			score1=log2((tpmap[key]*ratio)/0.001);
			score2=log2((tpmap[key]*1.0)/0.001);
			output << it->first <<"\t"<< score1 <<"\t"<<score2<<"\t"<< tpmap[key] <<"\t"<<fpmap[key] <<std::endl;
		}
	}
	for(auto it=fpmap.begin();it!=fpmap.end();++it){
		key=it->first;
		if(tpmap.find(key)!=tpmap.end())continue;
		score1=log2((0.001*ratio)/fpmap[key]);
		score2=log2((0.001)/fpmap[key]);
		output<<it->first <<"\t"<<score1<<"\t"<<score2<<"\t"<<0<<"\t"<<it->second<<std::endl;
	}
}
void patternLearning(std::string tpfilename,std::string fpfilename,std::fstream& output){
	int seqLenTP,seqLenFP;
	Count_Map tpMap, fpMap;
	seqLenTP=countKmerFrequence(tpMap, tpfilename);
	seqLenFP=countKmerFrequence(fpMap, fpfilename);
	outputMap(tpMap,fpMap,output, seqLenTP, seqLenFP);
}

#endif /* pattern_learning_h */
