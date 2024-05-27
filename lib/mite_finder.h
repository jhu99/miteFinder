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
#include <mutex>
#include <thread>
#include <chrono>
#include <map>
// Hash map for kmer index.
typedef std::unordered_map<std::string,std::vector<int>*> Tir_map;
// Data type for a set of seeds detected from chromosomes.
typedef std::list<Seed> Seed_set;
typedef std::unordered_map<std::string, Pattern_value> Pattern_map;

std::mutex global_map_mutex; 
int num_threads0=8;
int num_threads=16;

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


void build_kmer_index_thread(Tir_map& local_tmap, const std::string& fragment, int pos, int k) {
    int len = fragment.length();
    for (int i = 0; i <= len - k; i++) {
        std::string key = fragment.substr(i, k);
        if (key.find_first_of('N') != std::string::npos)
            continue;
        if (local_tmap.find(key) != local_tmap.end()) {
            local_tmap[key]->push_back(pos + i);
        } else {
            local_tmap[key] = new std::vector<int>{pos + i};
        }
    }
}

void build_kmer_index_parallel(Tir_map& global_tmap, const std::string& sequence, int k) {
 
    std::vector<std::thread> threads;
    std::vector<Tir_map> local_maps(num_threads0);
    int chunk_size = (sequence.length() / num_threads0) / k * k; // Ensure that we are cutting off at k-mer boundaries

    for (int i = 0; i < num_threads0; ++i) {
        int start = i * chunk_size;
        int end = (i == num_threads0 - 1) ? sequence.length() : (start + chunk_size);
        std::string fragment = sequence.substr(start, end - start);
        threads.emplace_back(build_kmer_index_thread, std::ref(local_maps[i]), fragment, start, k);
    }

    // 等待所有线程完成
    for (auto& thread : threads) {
        thread.join();
    }

    // 合并所有线程的结果到全局 tmap
    for (auto& map : local_maps) {
        for (auto& pair : map) {
            if (global_tmap.find(pair.first) != global_tmap.end()) {
                global_tmap[pair.first]->insert(global_tmap[pair.first]->end(), pair.second->begin(), pair.second->end());
            } else {
                global_tmap[pair.first] = pair.second;
            }
        }
    }
   

}



bool search_seed(std::vector<int>* v1, std::vector<int>* v2, Seed_set& seedset, int mis_tir, int mis_tirpos, int k, std::mutex& seed_set_mutex) {
    std::vector<int>::iterator i1, i2;
    for (i1 = v1->begin(); i1 != v1->end(); i1++) {
        for (i2 = v2->begin(); i2 != v2->end(); i2++) {
            int p1 = *i1;
            int p2 = *i2;
            if (std::abs(p1 - p2) < k) continue;
            if (p2 > p1) {
                if ((p2 - p1) > (MAX_LENGTH_MITE - k)) break;
                {
                    std::lock_guard<std::mutex> guard(seed_set_mutex);
                    seedset.push_back(Seed(p1, p1 + k - 1, p2, p2 + k - 1, mis_tir, p1 + mis_tirpos));
                }
            } else {
                if ((p1 - p2) > (MAX_LENGTH_MITE - k)) continue;
                {
                  std::lock_guard<std::mutex> guard(seed_set_mutex);
                    seedset.push_back(Seed(p2, p2 + k - 1, p1, p1 + k - 1, mis_tir, p2 + k - 1 - mis_tirpos));
                }
            }
        }
    }
    return true;
}

 
void process_kmer_submap(Tir_map& tmap, Tir_map::iterator start, Tir_map::iterator end, Seed_set& tset, int k, bool disable_mismatch, std::mutex& seed_set_mutex) {
 

    std::string key,invkey,standinvkey;
    std::vector<int> *v1,*v2;
    std::unordered_map<std::string, bool> record_map;
   for(auto it=start;it!=end;++it)
    {
        key=it->first;
        standinvkey.clear();
        if(!inverse_repeat(standinvkey,key))continue;
        if(tmap.find(standinvkey)!=tmap.end()){
            if(check_repeat_stretch(key))continue;
           // std::lock_guard<std::mutex> guard(seed_set_mutex);// 使用锁保护共享数据
            v1=it->second;
            v2=tmap.at(standinvkey);

            search_seed(v1,v2,tset,0,0,10,seed_set_mutex);
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
               // std::lock_guard<std::mutex> guard(seed_set_mutex);// 使用锁保护共享数据
                search_seed(v1,v2,tset,1,j,10,seed_set_mutex);
                invkey[j]=standinvkey[j];
            }
        }
    }
    
}



bool extract_seed_from_map_parallel(Tir_map& tmap, Seed_set& tset, int k, bool disable_mismatch) {
 
    std::vector<std::thread> threads;
    std::mutex seed_set_mutex;
 
    auto it = tmap.begin();
    size_t map_size = tmap.size();
    size_t chunk_size = map_size / num_threads;  // 计算每个线程的基本块大小
     Seed_set local_tset[num_threads];
    for (int i = 0; i < num_threads; ++i) {
        auto start = std::next(it, i * chunk_size);
        auto end = (i < num_threads - 1) ? std::next(start, chunk_size) : tmap.end();
        threads.emplace_back(process_kmer_submap, std::ref(tmap), start, end, std::ref(local_tset[i]), k, disable_mismatch, std::ref(seed_set_mutex));
    }

    for (auto& thread : threads) {
        thread.join();
    }
    for (auto& local_set : local_tset) {
        tset.splice(tset.end(), local_set);
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
bool collapse_seed(Seed_set& tset, char* pchr) {//就是在合并种子 有错配+无错位的 两个无错配的
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


bool mite_finder(Seed_set& seedset, char* pChr, bool disable_mismatch, int fragLen, int k) {
    int len = std::strlen(pChr);
    Tir_map global_tmap; // 全局 TIR map
    char* pCurr = pChr;
  
    std::string sequence(pChr, pChr + len); // 将整个字符数组转换为字符串


    // 使用并行方式构建 k-mer 索引
   
    build_kmer_index_parallel(global_tmap, sequence, k);

    extract_seed_from_map_parallel(global_tmap, seedset, k, disable_mismatch);
 
    
    // 计算时间差（以毫秒为单位）

    // 输出函数运行时间
    clearMap(global_tmap);
    seedset.sort();
    remove_duplicate_seed(seedset);
    collapse_seed(seedset, pChr);
    return true;
}


void processSubSet(const Seed_set::iterator& start, const Seed_set::iterator& end,
                   char* pChr, const Pattern_map& pattern_map, double threshold,
                   std::vector<Seed_set::iterator>& local_delete_list) {
    std::string pattern;
    // std::cout << "Thread started processing." << std::endl;
    for (auto it = start; it != end; ++it) {
        double score = 0.0;
        int count = 0;
        int endPos = it->pos4 - LENGTH_PATTERN + 1;
        for (int i = it->pos1; i <= endPos; ++i) {
            pattern = std::string(pChr + i, LENGTH_PATTERN);
            auto find_it = pattern_map.find(pattern);
            if (find_it != pattern_map.end()) {
                score += find_it->second.score1;
                ++count;
            }
        }
        if (count > 0) {
            double avgScore = score / count;
            if (avgScore < threshold) {
                local_delete_list.push_back(it);
               //  std::cout << "Marked for deletion." << std::endl;
            } else {
                it->ave_score = avgScore;
            }
        }//std::cout << "Finished processing seed." << std::endl;
    }  //std::cout << "Thread finished processing." << std::endl;
}
bool filter_low_score_candidates_mt(Seed_set& seedset,
                                    char* pChr,
                                    const Pattern_map& pattern_map,
                                    double threshold) {

    std::vector<std::thread> threads;
    std::vector<std::vector<Seed_set::iterator>> to_delete(num_threads);

    auto it = seedset.begin();
    size_t chunk_size = seedset.size() / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        auto last = (i == num_threads - 1) ? seedset.end() : std::next(it, chunk_size);
        threads.emplace_back(processSubSet, it, last, pChr, std::cref(pattern_map), threshold, std::ref(to_delete[i]));
        it = last;
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Consolidate deletion lists and remove entries
try {
    for (const auto& del_list : to_delete) {
        for (const auto& del_it : del_list) {
            if (del_it != seedset.end()) {  // 确保迭代器有效
                seedset.erase(del_it);
                //std::cout << "Deleted an item." << std::endl;
            } else {
                //std::cerr << "Attempted to delete using an invalid iterator." << std::endl;
            }
        }
    }
   // std::cout << "Deletion process completed." << std::endl;
} catch (const std::exception& e) {
    std::cerr << "Exception occurred during deletion: " << e.what() << std::endl;
} catch (...) {
    std::cerr << "An unknown exception occurred during deletion." << std::endl;
}

    return true;
}

#endif
/* mite_finder_h */