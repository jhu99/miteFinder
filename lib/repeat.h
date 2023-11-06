#include <iostream>
#include <string>
#include <cstring>
#include <unordered_map>//包含C++标准库头文件<unordered_map>，用于使用无序映射数据结构。
#include <vector>//包含C++标准库头文件<vector>，用于使用向量容器数据结构。
#include <list>//包含C++标准库头文件<list>，用于使用链表容器数据结构。
#include <cmath>
#include <fstream>

#include "variable.h"
#include "mite.h"
#include "genome.h"
using namespace std;
typedef std::unordered_map<std::string,std::vector<int>*> Rpmap;
//定义了一个名为"map"的类型别名，表示一种无序映射，其键是字符串，值是指向整数向量的指针（位置信息）
typedef std::list<Seed> Seed_set;
//定义了一个名为"Seed_set"的类型，表示一种链表，每个元素都是"Seed"类对象

using namespace std;
void print_rp(Rpmap& rpmap);
bool inverse_repeat2(std::string& invkey,std::string key);
bool build_kmer_index2(Rpmap& rpmap,char* pcurr,int k);
bool extend(Rpmap& rpmap,char* pcurr);
bool repeat_finder(char* pChr,int mismatch,int k);

void print_rp(Rpmap& rpmap){
    cout<<"Have now entered print_rp"<<endl;
    unordered_map<std::string,std::vector<int>*>::iterator it;
    for(it = rpmap.begin();it != rpmap.end();it++){
        if(it->second->size() >= 2){
            cout << it->first<<" "<<endl;  //输出序列
            for(int i = 0;i < it->second->size();i++){
                cout<<it->second->at(i)<<endl;  //输出位置
                
            }
            cout<<it->second->size()<<endl;  //输出个数
        }
    }
}

bool inverse_repeat2(std::string& invkey,std::string key) {
    int len=(int)key.length();
    for(int i=len-1;i>=0;i--) {
        switch (key[i]) {
        case 'A':// 如果当前字符是 'A'，将 'T' 添加到输出字符串 invkey
            invkey += 'T';
            break;
        case 'T':// 如果当前字符是 'T'，将 'A' 添加到输出字符串 invkey
            invkey += 'A';
            break;
        case 'C':// 如果当前字符是 'C'，将 'G' 添加到输出字符串 invkey
            invkey += 'G';
            break;
        case 'G':// 如果当前字符是 'G'，将 'C' 添加到输出字符串 invkey
            invkey += 'C';
            break;
        case 'N':
            return false;
        }
    }
    return true;
}
//这段代码定义了一个C++函数build_kmer_index，用于构建一个k-mer索引，
//并将结果存储在名为rpmap的无序映射中

bool build_kmer_index2(Rpmap& rpmap,char* pcurr,int k)
{   
    cout<<"Have now entered build_kmer_index"<<endl;
    int len = (int)strlen(pcurr);// 获取输入字符串 fragment 的长度
    if(len < k) return false;// 如果输入字符串长度小于 k，则无法构建索引，返回 false
    string fragment = pcurr;
    // 遍历输入字符串fragment，从头到尾截取长度为 k 的子串，并构建索引
    for(int i = 0;i <= len - k;i++)
    {
        //cout<<"Have now entered build_kmer_index for"<<endl;
        string key = fragment.substr(i,k);// 截取长度为 k 的子串作为 k-mer
        if(key.find_first_of('N')!=std::string::npos)// 如果 k-mer 中包含字符 'N'，则跳过该 k-mer
            continue;
        if(rpmap.find(key)!=rpmap.end())// 检查当前k-mer是否已存在于索引中
        {  // 如果已存在，将当前位置 i 添加到 k-mer 的索引列表中
            rpmap.at(key)->push_back(i);
        }
        else  // 如果不存在，创建一个新的索引列表，将当前位置 i 添加到列表中
        {
            rpmap[key]=new std::vector<int>();
            rpmap.at(key)->push_back(i);
        }
    }
    cout<<"build_kmer_index end."<<endl;
    return false;
}

bool extend(Rpmap& rpmap,char* pcurr){
    string extension;
    string seq = pcurr;
    int k = 100;
    unordered_map<std::string,std::vector<int>*>::iterator it;
    for(it = rpmap.begin();it != rpmap.end();it++){
        int pos1,pos2;
        string fragment = it->first;
        vector<int>* location = it->second;  //位置列表
        int fnum = location->size();
        if(fnum = 1)  continue;
        for(int i = 0;i < fnum;i++){
            pos1 = location->at(i);
            pos2 = location->at(i+1);
            extension = it->first;
            if(seq.substr(pos1+k,i).compare(seq.substr(pos2+k,i)) == 0)  //如果字符串相等
                


        }

        


    }

    //for(){}
    return true;
}

//这个函数的主要功能是构建一个k-mer索引，其中k是用户指定的参数，默认值为10。它遍历输入字符串 fragment，从头到尾截取长度为k的子串作为k-mer，
//并将这些k-mer映射到它们在输入字符串中的位置（pos + i），存储在tirmap中。如果在k-mer中包含字符 'N'，则跳过该k-mer。
//如果k-mer已经存在于索引中，将其位置添加到已有的位置列表中，否则创建一个新的位置列表。
// 读取fasta文件，返回一个unordered_map，键为序列名，值为序列内容

bool collapse_seed2(Seed_set& tset, char* pchr) { // 合并相邻的种子。
    // Collapse adjacent seeds.
    Seed tmp1,tmp2,one,two(2);
    Seed_set::iterator sit,it=tset.begin();// 迭代器用于遍历种子集合
    while(it!=tset.end()){
        tmp2=tmp1=*it;// 复制当前种子
        tmp1+=one;// 将tmp1向右偏移1
        tmp2+=two;// 将tmp2向右偏移2
        sit=it;
        sit++;// 移到下一个种子
        while(sit!=tset.end()){
            if(tmp1==(*sit)){
                (*it)+=1;// 如果tmp1等于下一个种子，将当前种子右移1
                tmp1+=one;// 更新tmp1
                tmp2+=one;// 更新tmp2
                sit=tset.erase(sit);// 移除下一个种子
            }else if(it->mismatch_tir==0&&tmp2==(*sit)){
                it->mismatch_tir=1;// 如果tmp2等于下一个种子，将当前种子的mismatch_tir设置为1
                (*it)+=2;// 将当前种子右移2
                tmp1+=two;// 更新tmp1
                sit=tset.erase(sit);// 移除下一个种子
            }else if((*sit)<tmp1){
                sit++;
            }else{
                break;
            }
        }
        // Check tsd and mite structure. // 检查tsd和MITE结构是否合法。
        if(!check_mite_structure(*it,pchr)){
            it=tset.erase(it);// 如果MITE结构不合法，移除当前种子
        }
        else{
            it++;// 否则，继续下一个种子
        }
    }
    return true;// 返回处理完成的标志
}
//这个函数的主要功能是对输入的MITE种子集合进行合并，合并规则是检查种子是否与其相邻的种子（左侧或右侧）具有特定关系。
//使用两个临时种子 tmp1 和 tmp2 来检查相邻性。tmp1 向右偏移1，tmp2 向右偏移2，以用于检查相邻的种子。
//使用迭代器 it 遍历MITE种子集合。
//在内部循环中，迭代器 sit 被用来检查当前种子与其后续种子之间的关系。
//如果相邻的两个种子具有特定关系（tmp1 与下一个种子相同，或者tmp2 与下一个种子相同），则将当前种子合并到下一个种子，并更新 tmp1 和 tmp2。
//如果种子之间的关系不符合条件，继续检查下一个种子。
//检查 MITE 结构的合法性，如果不合法，则移除当前种子。
//最后，返回处理完成的标志。
//这个函数主要用于对MITE种子集合进行合并，以减少重复和提高分析效率

bool repeat_finder(char* pChr,int mismatch,int k) {
  int len = (int)std::strlen(pChr);// 计算DNA序列的长度
  Rpmap rpmap;  //用于存储kmer索引的哈希映射
  cout<<"Have now entered repeat_finder"<<endl;
  build_kmer_index2(rpmap,pChr,k);
  cout<<"entered print_rp"<<endl;
  //extend(rpmap,pChr);
  //cin.get();
  print_rp(rpmap);
//   int pos = 0;  //当前位置的偏移量
//   while(pos<len) {  //循环处理DNA序列
//     int remainder=(int)std::strlen(pCurr);// 计算剩余未处理的DNA序列长度
//     std::string fragment;// 存储当前处理的DNA片段
//     if(remainder <= fragLen*1.5) {// 如果剩余长度小于等于fragLen的1.5倍，将整个剩余部分作为片段
//       fragment=std::string(pCurr,remainder);
//       build_kmer_index(rpmap,fragment,pos,k);// 构建当前片段的kmer索引
//       pos += remainder;// 更新当前位置
//     }else {
//       fragment=std::string(pCurr,fragLen);// 否则，截取fragLen长度的片段
//       build_kmer_index(rpmap,fragment,pos,k);// 构建当前片段的kmer索引
//       pos += fragLen-MAX_LENGTH_MITE;// 更新当前位置，减去MITE的最大长度以避免重叠
//     }
//     pCurr=pChr+pos;// 移动当前指针到下一个处理位置
  
//   //remove_duplicate_seed(seedset);// 移除重复的种子
//   //collapse_seed(seedset, pChr);// 合并相邻的种子
  return true;// 返回处理完成的标志
}
//这个函数的主要功能是对输入的DNA序列进行分段处理，构建kmer索引，然后从索引中提取MITE种子，
//最后对种子集合进行排序、去除重复和合并相邻的种子。