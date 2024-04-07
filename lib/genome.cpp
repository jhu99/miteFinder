#include "genome.h"
#include <fstream>
#include <sstream>
#include <limits>
#include <iostream>
#include <thread>

Genome::Genome():maxLength(0),numChrom(0)
{
}
Genome::~Genome()
{
  for(std::vector<char*>::iterator it=this->vecChrom.begin();it!=this->vecChrom.end();it++)
  {
    delete [] *it;
  }
}
  //  std::vector<char*> vecChrom; // 染色体序列存储
  
    // std::vector<int> sizeChrom; // 染色体大小（行数）
    // std::vector<char*> vecChrom; // 染色体序列存储
    // int maxLength; // 最长行的长度
    // int numChrom; // 染色体数量

void Genome::readChromosomeSequence2(const std::string& filepath, int startPos, int endPos, int index) {
        std::ifstream ifs(filepath);
        ifs.seekg(startPos); // 定位到染色体的起始位置

        std::string line;
        std::string sequence;
        while (ifs.tellg() < endPos && std::getline(ifs, line)) {
            if (line[0] != '>') {
                sequence += line;
            }
        }

        vecChrom[index] = new char[sequence.length() + 1];
        std::copy(sequence.begin(), sequence.end(), vecChrom[index]);
        vecChrom[index][sequence.length()] = '\0'; // 空字符终止
    }

    
void Genome::readSequence(std::string path_to_file){
	std::ifstream input(path_to_file.c_str());
	std::string line,chrom;
	int count=0;
	char* pChrom = nullptr;
	while(std::getline(input, line)){
		if(line[0]=='>'){
			count++;//当遇到一个新的染色体条目时，count 会递增。
			if(count==1)continue;//对于文件中的第一个染色体条目，没有前一个条目需要处理，所以会跳过剩余的代码进入下一次循
			int chromsize=(int)chrom.length();
			pChrom=new char[chromsize+1];
			this->vecChrom.push_back(pChrom);//并将内存地址添加到 vecChrom 向量中。
			chrom.copy(pChrom, chrom.length());//对于随后的条目，函数会动态分配足够的内存来存储之前累积的 chrom 序列，将序列复制到这块内存中
			pChrom[chromsize]='\0';
			chrom.clear();
		}else{
			chrom.append(line);
		}
	}
	pChrom=new char[chrom.length()+1];
	this->vecChrom.push_back(pChrom);
	chrom.copy(pChrom, chrom.length());
	this->numChrom=count;///循环结束后，需要处理文件中的最后一个染色体条目。同样地，动态分配内存，将累积的序列复制进去，并将内存地址添加到 vecChrom 向量中。
}

void Genome::readGenome(std::string path_to_file)
{
  std::ifstream input(path_to_file.c_str());
  std::string line;
  int mark=0,count,linenum;
  linenum=0;
  while(std::getline(input,line))//第一次遍历文件：循环读取文件中的每一行，并通过行号 linenum 记录当前行的位置。

//a. 当遇到一个以 > 开头的行时，这标志着一个新的染色体序列的开始：对于文件的第一行，简单地更新 mark 并继续。
//对于其他情况，计算上一个染色体的行数（当前行号 linenum 减去上一个染色体开始的行号 mark），并将结果添加到 sizeChrom 向量中。更新 mark 为当前行号，以便下次使用。
//b. 对于不以 > 开头的行，检查其长度是否超过了目前记录的 maxLength，如果是，更新 maxLength。

  {
    linenum++;
    if(line[0]=='>')
    {
      if(linenum==1)
      {
        mark=1;
        continue;
      }
      else{
        this->sizeChrom.push_back(linenum-mark-1);//sizechrom是一个向量，其每个元素代表一个染色体序列中的行数。向量中的每个元素对应于文件中的一个染色体条目。
      }
      mark=linenum;
    }
    else
    {
      if(line.length()>(unsigned)this->maxLength)
      {
        this->maxLength=(int)line.length();
      }
    }
  }
  this->sizeChrom.push_back(linenum-mark);//sizechrom是一个向量，其每个元素代表一个染色体序列中的行数。向量中的每个元素对应于文件中的一个染色体条目。
  input.clear();
  input.seekg(0,input.beg);//重置文件流状态，并将文件指针移回文件开始，准备第二次遍历。
  count=-1;
  char* pChrom = nullptr;

   vecChrom.resize(numChrom);

        // 创建并启动线程
        std::vector<std::thread> threads;
        for (int i = 0; i < numChrom; ++i) {
            // 假设我们已经计算了每个染色体的起始和结束位置：startPos[i] 和 endPos[i]
            threads.emplace_back(&Genome::readChromosomeSequence2, this, path_to_file, startPos[i], endPos[i], i);
        }

        // 等待所有线程完成
        for (auto& th : threads) {
            if (th.joinable()) {
                th.join();
            }
        }

  
}
//readGenome这个函数在第一次遍历文件时计算每个染色体的行数和最长行的长度，
//然后在第二次遍历时为每个染色体序列分配内存并存储序列。
//这种两次遍历的方法允许函数在不知道每个染色体序列具体长度的情况下，动态地分配足够的内存来存储序列。
int Genome::getNumChrom()
{
  return this->numChrom;
}

char* Genome::getChrom(int pos)
{
  return this->vecChrom[pos];
}


//我改成多线程的逻辑是并行化第二次文件遍历的过程，即并行地为每个染色体序列分配内存并存储序列：