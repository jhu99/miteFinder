#ifndef ltr_finder_h
#define ltr_finder_h

#include <typeinfo>
#include <iostream>
#include <map>
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
#include "mite_finder.h"

using namespace std;

/* 一些全局变量 */
int k = 100;   //k-mer长度，LTR长度为100-6000bp
typedef std::unordered_map<std::string,std::vector<int>*> Ltr_map;
struct match{
    int start1;
    int end1;
    int start2;
    int end2;
};
std::vector<match> ltrmatch;

bool build_kmer_index(Ltr_map& ltrmap,char* pcurr);
bool test_print_ltrmap(Ltr_map& ltrmap);
bool stretch(Ltr_map& ltrmap,char* pcurr);
int forward_stretch(Ltr_map& ltrmap,char* pcurr);//返回后扩展后序列的最右边的position
int backward_stretch(Ltr_map& ltrmap,char* pcurr);//返回扩展后序列的最左边的position


bool ltr_finder(char* pchr,int disable_mismatch)
{
    int len = (int)std::strlen(pchr);//整条序列的长度
    Ltr_map lmap;
    cout<<"*************************"<<endl;
    //遍历整条序列
    build_kmer_index(lmap,pchr);
    //对每个桶进行遍历，遍历的同时进行扩展，并删除已经确认为当前扩展中一部分的k-mer
    //包含perfect 和 unperfect
    //perfect情况
    stretch(lmap,pchr);
    cout<<"*************************"<<endl;
    test_print_ltrmap(lmap);
    return false;
}

bool build_kmer_index(Ltr_map& ltrmap,char* pcurr)
{//k_mer长度为k
    int len = (int)std::strlen(pcurr);//整条序列的长度
    string seq;
    seq = pcurr;
    for(int pos = 0; pos + k < len; pos++){
        std::string key;
        key = seq.substr(pos,k);
        if(key.find_first_of('N')!=std::string::npos)
            continue;
        if(ltrmap.find(key)!=ltrmap.end())
        {
            ltrmap.at(key)->push_back(pos);
        }
        else
        {
            ltrmap[key]=new std::vector<int>();
            ltrmap.at(key)->push_back(pos);
        }
    }
    return false;
}
bool stretch(Ltr_map& ltrmap,char* pcurr)
{   
    string stretched;
    string seq = pcurr;    
    for(auto it=ltrmap.begin();it!=ltrmap.end();it++){
        string k_mer = it->first;
        std::vector<int>* location_vector = it->second;
        int k_mer_num = location_vector->size();
        if(k_mer_num == 1){
            continue;//保留进行进一步的筛选
        }
        else if(k_mer_num == 2){
            //该两个精确的配对，扩展这两个k-mer
            int right_index = forward_stretch(ltrmap,pcurr);
            int left_index = backward_stretch(ltrmap,pcurr);

            stretched = stretched +seq.substr(left_index,(right_index-left_index))+'#';

        }
        else{//可能为嵌套结构
                
        }
    }
    return true;
}

bool test_print_ltrmap(Ltr_map& ltrmap)
{

    unordered_map<std::string,std::vector<int>*>::iterator it;
    it=ltrmap.begin();
    for(;it!=ltrmap.end();it++){
       // cout<<it->first<<" "<<it->second<<endl;
        break;
    }

    return false;
}



/*
bool mite_finder(Seed_set& seedset,
                 char* pChr,
                 bool disable_mismatch,
                int fragLen,
                int k) {//k:MIN_LENGTH_TIR
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
      fragment=std::string(pCurr,fragLen);//获取的一个大的片段序列
      build_kmer_index(tmap,fragment,pos,k);
      pos += fragLen-MAX_LENGTH_MITE;   //实现最大MITE长度的重叠
    }
    extract_seed_from_map(tmap,seedset,k,disable_mismatch);//从map中提取种子
    clearMap(tmap);
    pCurr=pChr+pos;//剩余的字符串
  }
  seedset.sort();
  remove_duplicate_seed(seedset);
  collapse_seed(seedset, pChr);
  return true;
}
*/

#endif /* ltr_finder_h */
