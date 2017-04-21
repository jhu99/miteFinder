#include "genome.h"
#include <fstream>
#include <sstream>
#include <limits>

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
void Genome::readGenome(std::string path_to_file)
{
  std::ifstream input(path_to_file.c_str());
  std::string line;
  int mark,count,linenum;
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
        this->maxLength=line.length();
      }
    }
  }
  this->sizeChrom.push_back(linenum-mark);
  this->numChrom=this->sizeChrom.size();
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

int Genome::getSizeChrom(int pos)
{
  return std::strlen(this->vecChrom[pos]);
}


