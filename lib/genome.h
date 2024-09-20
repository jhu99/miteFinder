#ifndef GENOME_H_
#define GENOME_H_

#include <string>
#include <vector>

class Genome
{
private:
  // Vector of chromosomes
  std::vector<char*> vecChrom;
  // Vector of the line number of each chromosome
  std::vector<int> sizeChrom;
  // maximum bases for a line
  int maxLength;
  
  // Number of chromosomes
  int numChrom;
public:
  Genome();
  ~Genome();
  // It reads a genome formatted in a fasta file.
  void readGenome(std::string);
  void readSequence(std::string);
  int getNumChrom();
  char* getChrom(int);
  void readChromosomeSequence2(const std::string& filepath, int startPos, int endPos, int index);
  // std::vector<char*> vecChrom; 
private:

    // 假定 startPos 和 endPos 已经在类中声明
    std::vector<int> startPos; // 每个染色体序列的起始位置
    std::vector<int> endPos; // 每个染色体序列的结束位置
};

#endif
