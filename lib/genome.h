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
};

#endif
