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
#include "genome.h"
#include "ltr.h"

using namespace std;

/* 一些全局变量 */
int k = 100;   //k-mer长度，LTR长度为100-6000bp
int MAX_LENGTH_LTR = 6000;
int MAX_LENGTH_LTR_TE = 22000;

typedef std::unordered_map<std::string,std::vector<int>*> Ltr_map;

typedef std::list<Match> match_set;

bool clearMap(Ltr_map& lmap);
bool extract_seed_from_ltrmap(Ltr_map &lmap,match_set &lset,int disable_mismatch);
bool build_kmer_index(Ltr_map& ltrmap,std::string& fragment,int pos, int k);
bool test_print_ltrmap(Ltr_map& ltrmap);
bool remove_duplicate_lseed(match_set& lset);
bool merge_seed(match_set &lset, char* pchr);

bool ltr_finder(char* pchr, match_set &lset, int disable_mismatch)
{
    int len = (int)std::strlen(pchr);//整条序列的长度
    Ltr_map lmap;
    char* pCurr = pchr;
    int pos = 0;
    int fragLen = 100000;
    while(pos < len){
        int remainder = (int)std::strlen(pCurr);
        std::string fragment;
        if(remainder <= fragLen*1.5){
            fragment = std::string(pCurr,remainder);
            build_kmer_index(lmap,fragment,pos,k);
            pos += remainder;
        }
        else{
            fragment=std::string(pCurr,fragLen);
            build_kmer_index(lmap,fragment,pos,k);
            pos += fragLen - MAX_LENGTH_LTR;
        }
        extract_seed_from_ltrmap(lmap,lset,disable_mismatch);
        clearMap(lmap);
        pCurr = pchr + pos;
    }
    cout<<"*****************"<<endl;
    lset.sort();
    cout<<"_________________"<<endl;
    remove_duplicate_lseed(lset);
    cout<<"*________________"<<endl;
    merge_seed(lset, pchr);
    cout<<"**_______________"<<endl;
    return true;
}

bool build_kmer_index(Ltr_map& ltrmap,std::string& fragment,int pos, int k)
{//k_mer长度为k
    int len = (int)fragment.length();//整条序列的长度
    if(len < k){
        return false;
    }
    string seq;
    seq = fragment;
    for(int i = 0; i + k < len; i++){
        std::string key;
        key = seq.substr(i,k);
        if(key.find_first_of('N')!=std::string::npos)
            continue;
        if(ltrmap.find(key)!=ltrmap.end())
        {
            ltrmap.at(key)->push_back(pos+i);
        }
        else
        {
            ltrmap[key]=new std::vector<int>();
            ltrmap.at(key)->push_back(pos+i);
        }
    }
    return true;
}

bool extract_seed_from_ltrmap(Ltr_map &lmap,match_set &lset,int disable_mismatch)
{
    std::string key1,key2;
    std::vector<int> *v1, *v2;
    for(auto it=lmap.begin();it!=lmap.end();++it){
        v1 = v2 = it->second;
        std::vector<int>::iterator i1,i2;
        /*这个地方还需要进一步处理，不能直接丢弃*/
        if(v1->size()<2){//该k-mer未找到重复
            continue;
        }
        for(i1=v1->begin();i1!=v1->end();i1++){
            for(i2=v2->begin();i2!=v2->end();i2++){
                int p1 = *i1;
                int p2 = *i2;
                if(std::abs(p1-p2)<k)continue;//交叉的情况
                if(p2>p1){
                    if((p2-p1)>(MAX_LENGTH_LTR_TE - k))break;//重复序列距离过大，不作为一个seed
                    lset.push_back(Match(p1,p1+k-1,p2,p2+k-1,disable_mismatch));
                }
                else{
                    if((p1-p2)>(MAX_LENGTH_LTR_TE-k))continue;
                    lset.push_back(Match(p2,p2+k-1,p1,p1+k-1,disable_mismatch));
                }
            }
        }
    }
    return true;
}

bool clearMap(Ltr_map& lmap) 
{
    for(auto it=lmap.begin();it!=lmap.end();++it){
        delete it->second;
    }
    lmap.clear();
    return true;
}

bool remove_duplicate_lseed(match_set& lset)
{
    match_set::iterator sit;
    for(match_set::iterator it=lset.begin();it!=lset.end();++it){
        sit=it;
        sit++;
        while(sit!=lset.end()) {
            if((*sit)==(*it)){
                sit=lset.erase(sit);
            }else{
                break;
            }
        }
    }
}

bool merge_seed(match_set &lset, char* pchr)
{
    Match tmp1,tmp2,tmp3,tmp4,one,two(2);
    match_set::iterator sit,it = lset.begin();
    /*对于每一个match，包含前向合并和后向合并*/
    //先只考虑perfect情况
    while(it!=lset.end()){
        tmp1 = tmp2 = tmp3 = tmp4 = *it;
        tmp1 += one;
        tmp2 += two;
        tmp3 -= one;
        tmp4 -= two;

        sit = it;
        sit++;
        while(sit!=lset.end()){
            if(tmp1==(*sit)){
                (*it) += 1;
                tmp1 += one;
                tmp2 += one;
                sit = lset.erase(sit);
            }
            else if(tmp3 == (*sit)){
                (*it) -= 1;
                tmp1 -= one;
                tmp2 -= one;
                sit = lset.erase(sit);
            }
            else if(tmp2 == (*sit)){
                (*it)+=2;
                tmp1 += two;
                sit = lset.erase(sit);
            }
            else if(tmp4 == (*sit)){
                (*it)-=2;
                tmp1 -= two;
                sit = lset.erase(sit);
            }
            else{
                sit++;
            }
        }
        /*
        if(!check_LTR_TE_structure(*it,pchr)){
            it=tset.erase(it);
        }
        else{
            it++;
        }
        */
        it++;
        
    }
    return true;
}


bool test_print_ltrmap(Ltr_map& ltrmap)
{

    unordered_map<std::string,std::vector<int>*>::iterator it;
    it=ltrmap.begin();
    for(;it!=ltrmap.end();it++){
        if(it->second->size() >= 2){
            cout<<it->first<<" "<<endl;
            for(int i = 0;i < it->second->size();i++){
               // cout<<it->second->at(i)<<"  "<<it->second->size();
                cout<<it->second->size();
            }
            cout<<endl;
        }
       // break;
    }

    return false;
}


#endif /* ltr_finder_h */
