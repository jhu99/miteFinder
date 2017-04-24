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

bool clearMap(Tir_map& tmap) {
  for(auto it=tmap.begin();it!=tmap.end();++it){
    delete it->second;
  }
  tmap.clear();
  return true;
}

std::string inverse_repeat(std::string key,int base,int pos) {
    std::string invkey;
    int len=key.length();
    for(int i=len-1;i>=0;i--) {
        if (i==pos){
            switch (base) {
            case 0:
                invkey += 'T';
                break;
            case 1:
                invkey += 'A';
                break;
            case 2:
                invkey += 'G';
                break;
            case 3:
                invkey += 'C';
                break;
            }
        }
        else
        {
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
                invkey += 'N';
                break;
            }
        }
    }
    return invkey;
}

bool build_kmer_index(Tir_map& tirmap,
                      std::string& fragment,
                      int pos,
                      int k=10)
{
    int len=fragment.length();
    if(len<k) return false;
    for(int i=0;i<=len-k;i++)
    {
        std::string key=fragment.substr(i,k);
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
                 int mis_tir,
                 int mis_tirpos,
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
                seedset.push_back(Seed(p2,p2+k-1,p1,p1+k-1,mis_tir,p2+9-mis_tirpos));
            }
        }
    }
    return true;
}

bool extract_seed_from_map(Tir_map& tmap,
                           Seed_set& tset,
                           int k=10) {
    std::string key,invkey,standinvkey;
    std::vector<int> *v1,*v2;
    std::unordered_map<std::string, int> record_map;
    for(auto it=tmap.begin();it!=tmap.end();++it)
    {
        key=it->first;
        standinvkey=inverse_repeat(key,0,10);
        for (int i=0;i<4;i++)
        for (int j=1;j<9;j++)
        {
            invkey=inverse_repeat(key,i,j);
            std::string signkey,invsignkey;
            signkey=signkey+key+invkey;
            invsignkey=invsignkey+invkey+key;
            if (record_map.find(signkey)!=record_map.end()) continue;
            else{
                record_map[signkey]=1;
                record_map[invsignkey]=1;
            }
            if(tmap.find(invkey)==tmap.end())continue;
            v1=it->second;
            v2=tmap.at(invkey);
            if (invkey==standinvkey)
            search_seed(v1,v2,tset,0,j,k);
            else
            search_seed(v1,v2,tset,1,j,k);
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
    for (tir=sd.pos1;tir<=sd.pos2;tir++){
        if (pchr[tir]=='G'||pchr[tir]=='C')
            GCnum++;
    }
    rate=((double)GCnum/(double)(sd.pos2-sd.pos1+1));
    if (rate<0.2)
        return false;
    for(tsd=MIN_LENGTH_TSD;tsd<=MAX_LENGTH_TSD;tsd++) {
        st1=sd.pos1-tsd;
        st2=sd.pos4+1;
        while (pchr[st1]==pchr[st2]) {
            st1++;
            st2++;
        }
        if (st1>=sd.pos1)
            tsdlen=tsd;
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
                int maxcol=60){
    char* ps=pchr;
    int start,end,distance;
    int chrlen=strlen(pchr);
    for(auto it=tset.begin();it!=tset.end();++it) {
        output << ">mite|"<<chr<<"|"<<it->pos1 << "|" << it->pos2 << "|"
        << it->pos3 <<"|"<<it->pos4<<"|"<<it->tsd<<"|"<<60<<std::endl;
        if (it->pos1<60||it->pos4+60>=chrlen)
        {
            start=it->pos1-it->tsd;
            end=it->pos4+it->tsd+1;
        }
        else
        {
            start=it->pos1-it->tsd-60;
            end=it->pos4+it->tsd+61;
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

// Collapse adjacent seeds and check their tsd.
bool collapse_seed(Seed_set& tset, char* pchr) {
    // Remove replicate seeds from tset.
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
    // Collapse adjacent seeds.
    Seed tmp,one;
    Seed_set::iterator it=tset.begin();
    while(it!=tset.end()){
        tmp=*it;
        tmp+=one;
        sit=it;
        sit++;
        while(sit!=tset.end()){
            if(tmp==(*sit)){
                 if (it->mismatch_tir==0&&sit->mismatch_tir==1)
                 {
                     it->mismatch_tir=sit->mismatch_tir;
                     it->mis_tirpos=sit->mis_tirpos;
                 }
                 else if (it->mismatch_tir==1&&sit->mismatch_tir==1)
                 {
                     if (it->mis_tirpos!=sit->mis_tirpos)
                        break;
                 }
                (*it)+=1;
                tmp+=one;
                sit=tset.erase(sit);
            }else if((*sit)<tmp){
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
                int fragLen=10000,
                int k=10) {
  int len=std::strlen(pChr);
  Tir_map tmap;
  char* pCurr = pChr;
  int pos = 0;
  while(pos<len) {
    int remainder=std::strlen(pCurr);
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
    extract_seed_from_map(tmap,seedset,k);
    clearMap(tmap);
    pCurr=pChr+pos;
  }
  seedset.sort();
  collapse_seed(seedset, pChr);
  return true;
}
#endif /* mite_finder_h */
