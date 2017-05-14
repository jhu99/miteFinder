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
#include <string.h>
#include <unordered_map>
const int LENGTH_KMER=6;
struct kmer_num
{
    int match;
    int mis_match;
};
typedef std::unordered_map<std::string,kmer_num> Count_Map;
void kmer_counter(char*,Count_Map&,int,int);

void countKmerFrequence(Count_Map& kmer_map,std::string filename,int is_match,int k=LENGTH_KMER){
	Genome seqs;
	seqs.readSequence(filename);
	int numSeq=seqs.getNumChrom();
	for(int i=0;i<numSeq;i++){
		char* pStr=seqs.getChrom(i);
		kmer_counter(pStr,kmer_map,is_match,k);
	}
}
void kmer_counter(char* pStr,Count_Map& kmer_map,int is_match,int k){
	int len=(int)strlen(pStr);
	for(int i=0;i<len-k;i++){
		std::string key(pStr+i,k);
		if(kmer_map.find(key)!=kmer_map.end()){
            if (is_match)
			kmer_map[key].match++;
            else
            kmer_map[key].mis_match++;
		}else{
			kmer_map[key].match=0;
            kmer_map[key].mis_match=0;
            if (is_match)
            kmer_map[key].match++;
            else
            kmer_map[key].mis_match++;
		}
	}
}
void patternLearning(){
}

#endif /* pattern_learning_h */
