#include "genome.h"
#include <fstream>
#include <sstream>
#include <limits>
#include <iostream>

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

void Genome::readSequence(std::string path_to_file){
	std::ifstream input(path_to_file.c_str());
	std::string line,chrom;
	int count=0;
	char* pChrom = nullptr;
	while(std::getline(input, line)){
		if(line[0]=='>'){
			count++;
			if(count==1)continue;
			int chromsize=(int)chrom.length();
			pChrom=new char[chromsize+1];
			this->vecChrom.push_back(pChrom);
			chrom.copy(pChrom, chrom.length());
			pChrom[chromsize]='\0';
			chrom.clear();
		}else{
			chrom.append(line);
		}
	}
	pChrom=new char[chrom.length()+1];
	this->vecChrom.push_back(pChrom);
	chrom.copy(pChrom, chrom.length());
	this->numChrom=count;
}

void Genome::readGenome(std::string path_to_file)
{
  std::ifstream input(path_to_file.c_str());
  std::string line;
  int mark=0,count,linenum;
  linenum=0;
  while(std::getline(input,line))
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
        this->sizeChrom.push_back(linenum-mark-1);
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
  this->sizeChrom.push_back(linenum-mark);
  this->numChrom=(int)this->sizeChrom.size();
  input.clear();
  input.seekg(0,input.beg);
  count=-1;
  char* pChrom = nullptr;
  while(std::getline(input,line))
  {
    if(line[0]=='>')
    {
      count++;
      this->vecChrom.push_back(new char[this->maxLength*this->sizeChrom[count]+1]);
      pChrom=this->vecChrom[count];
    }
    else
    {
      line.copy(pChrom,line.length());
      pChrom=pChrom+line.length();
    }
  }
  input.close();
}

int Genome::getNumChrom()
{
  return this->numChrom;
}

char* Genome::getChrom(int pos)
{
  return this->vecChrom[pos];
}


