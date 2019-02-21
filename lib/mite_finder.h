//
//  mite_finder.h
//
//
//  Created by Jialu Hu on 16/10/9.
//
//

#ifndef mite_finder_h
#define mite_finder_h

#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <list>
#include <cmath>
#include <fstream>
#include "variable.h"
#include "mite.h"
#include "genome.h"

// Hash map for kmer index.
typedef std::unordered_map<std::string,std::vector<int>*> Tir_map;
// Data type for a set of seeds detected from chromosomes.
typedef std::list<Seed> Seed_set;
typedef std::unordered_map<std::string, Pattern_value> Pattern_map;

bool clearMap(Tir_map& tmap) {
  for(auto it=tmap.begin();it!=tmap.end();++it){
    delete it->second;
  }
  tmap.clear();
  return true;
}

bool check_repeat_stretch(std::string& tir){
    int len=(int)tir.length();
    int homopolymer=1,max_homopolymer=1,dinucleotide=1,max_dinucleotide=1;
    for(int i=1;i<len;i++){
        if(tir[i]==tir[i-1]){
            homopolymer++;
            if(homopolymer>max_homopolymer) max_homopolymer=homopolymer;
        }
        else{
            homopolymer=1;
        }
    }
    for(int i=3;i<len;i=i+2){
        if((tir[i-1]==tir[i-3])&&tir[i]==tir[i-2]){
            dinucleotide++;
            if(max_dinucleotide<dinucleotide)
                max_dinucleotide=dinucleotide;
        }else{
            dinucleotide=1;
        }
    }
    for(int i=4;i<len;i=i+2){
            if((tir[i-1]==tir[i-3])&&tir[i]==tir[i-2]){
                dinucleotide++;
                if(max_dinucleotide<dinucleotide)
                    max_dinucleotide=dinucleotide;
            }else{
                dinucleotide=1;
            }
        }
    if(max_homopolymer>=8 || max_dinucleotide>=4)
        return true;
    else
        return false;
}

bool inverse_repeat(std::string& invkey,std::string key) {
    int len=(int)key.length();
    for(int i=len-1;i>=0;i--) {
        switch (key[i]) {
        case 'A':
            invkey += 'T';
            break;
        case 'T':
            invkey += 'A';
            break;
        case 'C':
            invkey += 'G';
            break;
        case 'G':
            invkey += 'C';
            break;
        case 'N':
            return false;
        }
    }
    return true;
}

bool build_kmer_index(Tir_map& tirmap,
                      std::string& fragment,
                      int pos,
                      int k=10)
{
    int len=(int)fragment.length();
    if(len<k) return false;
    for(int i=0;i<=len-k;i++)
    {
        std::string key=fragment.substr(i,k);
        if(key.find_first_of('N')!=std::string::npos)
            continue;
        if(tirmap.find(key)!=tirmap.end())
        {
            tirmap.at(key)->push_back(pos+i);
        }
        else
        {
            tirmap[key]=new std::vector<int>();
            tirmap.at(key)->push_back(pos+i);
        }
    }
    return true;
}

bool search_seed(std::vector<int>* v1,
                 std::vector<int>* v2,
                 Seed_set& seedset,
                 int mis_tir=0,
                 int mis_tirpos=0,
                 int k=10) {
    std::vector<int>::iterator i1,i2;
    for(i1=v1->begin();i1!=v1->end();i1++) {
        for(i2=v2->begin();i2!=v2->end();i2++) {
            int p1=*i1;
            int p2=*i2;
            if(std::abs(p1-p2)<k) continue;
            if(p2>p1) {
                if((p2-p1)>(MAX_LENGTH_MITE-k)) break;
                seedset.push_back(Seed(p1,p1+k-1,p2,p2+k-1,mis_tir,p1+mis_tirpos));
            }else
            {
                if((p1-p2)>(MAX_LENGTH_MITE-k)) continue;
                seedset.push_back(Seed(p2,p2+k-1,p1,p1+k-1,mis_tir,p2+k-1-mis_tirpos));
            }
        }
    }
    return true;
}

bool extract_seed_from_map(Tir_map& tmap,
                           Seed_set& tset,
                           int k,
                           bool disable_mismatch
                           ) {
    std::string key,invkey,standinvkey;
    std::vector<int> *v1,*v2;
    std::unordered_map<std::string, bool> record_map;
    for(auto it=tmap.begin();it!=tmap.end();++it)
    {
        key=it->first;
        standinvkey.clear();
        if(!inverse_repeat(standinvkey,key))continue;
        if(tmap.find(standinvkey)!=tmap.end()){
            if(check_repeat_stretch(key))continue;
            v1=it->second;
            v2=tmap.at(standinvkey);
            search_seed(v1,v2,tset,0,0,k);
        }
        if(!disable_mismatch)
        {
            if(check_repeat_stretch(key))continue;
            invkey=standinvkey;
            for (int i=0;i<4;i++)
            for (int j=1;j<(k-1);j++)
            {
                if(standinvkey[j]==DNA_NUCLEOTIDE[i])continue;
                invkey[j]=DNA_NUCLEOTIDE[i];
                if(tmap.find(invkey)==tmap.end()){
                    invkey[j]=standinvkey[j];
                    continue;
                }
                v1=it->second;
                v2=tmap.at(invkey);
                search_seed(v1,v2,tset,1,j,k);
                invkey[j]=standinvkey[j];
            }
        }
    }
  return true;
}

bool check_mite_structure(Seed& sd, const char* pchr) {
    // Check the mite structure.
    if(sd.pos3<=sd.pos2)
        return false;
    if((sd.pos4-sd.pos1+1)<MIN_LENGTH_MITE)
        return false;
    int st1,st2,tir,tsd,GCnum=0,tsdlen=0;;
    double rate;
	for(int i=sd.pos2;i<=sd.pos3;i++){
		if(pchr[i]=='N') return false;
	}
    for (tir=sd.pos1;tir<=sd.pos2;tir++){
        if (pchr[tir]=='G'||pchr[tir]=='C')
            GCnum++;
    }
    rate=((double)GCnum/(double)(sd.pos2-sd.pos1+1));
    if (rate<MIN_GC_CONTANT || rate>MAX_GC_CONTANT)
        return false;
    for(tsd=MAX_LENGTH_TSD;tsd>=MIN_LENGTH_TSD;tsd--) {
        st1=sd.pos1-tsd;
        st2=sd.pos4+1;
        std::string tsdsequence1(pchr+st1,pchr+(st1+tsd));
        std::string tsdsequence2(pchr+st2,pchr+(st2+tsd));
        if (tsdsequence1==tsdsequence2)
        {
            tsdlen=tsd;
            break;
        }
    }
    if (tsdlen==2){
        if (pchr[sd.pos1-1]!='A'||pchr[sd.pos1-2]!='T')
            return false;
    }
    sd.tsd=tsdlen;
    if (tsdlen) return true;
    else return false;
}

bool write_seed(Seed_set& tset,
                char* pchr,
                std::fstream& output,
				int chr,
				bool write_flank=true,
                int maxcol=60){
    char* ps=pchr;
    int start = 0,end = 0,distance = 0;
    int chrlen=(int)strlen(pchr);
    for(auto it=tset.begin();it!=tset.end();++it) {
        output << ">mite|"<<chr<<"|"<<it->pos1 << "|" << it->pos2 << "|"
        << it->pos3 <<"|"<<it->pos4<<"|t"<<it->tsd<<"|"<<it->mis_tirpos<<"|m"<<it->mismatch_tir<<"|ave_score:"<<it->ave_score<<std::endl;
		if(write_flank){
			start=it->pos1-LENGTH_FLANK;
			end=it->pos4+LENGTH_FLANK+1;
			if(start<0)start=0;
			if(end>chrlen)end=chrlen;
		}else{
			start=it->pos1;
			end=it->pos4+1;
		}
		ps=pchr+start;
        while(start<end){
            distance=end-start;
            if(distance>maxcol){
                output << std::string(ps,maxcol) <<std::endl;
            }else{
                output << std::string(ps,distance)<<std::endl;
            }
            start+=maxcol;
            ps+=maxcol;
        }
    }
    return true;
}

// Remove duplicate seeds from tset.
bool remove_duplicate_seed(Seed_set& tset) {
    Seed_set::iterator sit;
    for(Seed_set::iterator it=tset.begin();it!=tset.end();++it){
        sit=it;
        sit++;
        while(sit!=tset.end()) {
            if((*sit)==(*it)){
                sit=tset.erase(sit);
            }else{
                break;
            }
        }
    }
    return true;
}
// Collapse adjacent seeds and check their tsd.
bool collapse_seed(Seed_set& tset, char* pchr) {
    // Collapse adjacent seeds.
    Seed tmp1,tmp2,one,two(2);
    Seed_set::iterator sit,it=tset.begin();
    while(it!=tset.end()){
        tmp2=tmp1=*it;
        tmp1+=one;
        tmp2+=two;
        sit=it;
        sit++;
        while(sit!=tset.end()){
            if(tmp1==(*sit)){
                (*it)+=1;
                tmp1+=one;
                tmp2+=one;
                sit=tset.erase(sit);
            }else if(it->mismatch_tir==0&&tmp2==(*sit)){
                it->mismatch_tir=1;
                (*it)+=2;
                tmp1+=two;
                sit=tset.erase(sit);
            }else if((*sit)<tmp1){
                sit++;
            }else{
                break;
            }
        }
        // Check tsd and mite structure.
        if(!check_mite_structure(*it,pchr)){
            it=tset.erase(it);
        }
        else{
            it++;
        }
    }
    return true;
}

bool mite_finder(Seed_set& seedset,
                 char* pChr,
                 bool disable_mismatch,
                int fragLen,
                int k) {
  int len=(int)std::strlen(pChr);
  Tir_map tmap;
  char* pCurr = pChr;
  int pos = 0;
  while(pos<len) {
    int remainder=(int)std::strlen(pCurr);
    std::string fragment;
    if(remainder <= fragLen*1.5) {
      fragment=std::string(pCurr,remainder);
      build_kmer_index(tmap,fragment,pos,k);
      pos += remainder;
    }else {
      fragment=std::string(pCurr,fragLen);
      build_kmer_index(tmap,fragment,pos,k);
      pos += fragLen-MAX_LENGTH_MITE;
    }
    extract_seed_from_map(tmap,seedset,k,disable_mismatch);
    clearMap(tmap);
    pCurr=pChr+pos;
  }
  seedset.sort();
  remove_duplicate_seed(seedset);
  collapse_seed(seedset, pChr);
  return true;
}

bool filter_low_score_candidates(Seed_set& seedset,
                                 char* pChr,
                                 Pattern_map& pattern_map,
                                 double threshold
                                 ){
    std::string pattern;
    Seed_set::iterator it=seedset.begin();
    while(it!=seedset.end()){
        double score=0.0;
        int end=it->pos4-LENGTH_PATTERN+1;
        for(int i=it->pos1;i<=end;i++){
            pattern=std::string(pChr+i,LENGTH_PATTERN);
            if(pattern_map.find(pattern)!=pattern_map.end()){
                score=score+pattern_map.at(pattern).score1;
            }
        }
        score=score/(it->pos4 - it->pos1 -LENGTH_PATTERN +2);
        if(score<threshold){
            it=seedset.erase(it);
        }
        else{
            it->ave_score=score;
            it++;
        }
    }
    return true;
}
#endif /* mite_finder_h */
