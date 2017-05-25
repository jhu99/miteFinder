#ifndef FASTA_H_
#define FASTA_H_

#include <iostream>
#include <string>
#include <stdlib.h>
//#include <malloc.h>
#include <fstream>

using namespace std;

const int MAX_CHROSOME_NUM=100;

class Genomes
{
public:
    Genomes();
    ~Genomes();
    int readGenome(string);
    string getFragment(int,int,int);
private:
    int chrnum;
    char* chrnote[MAX_CHROSOME_NUM];
    char* chromosomes[MAX_CHROSOME_NUM];
    int chr_length[MAX_CHROSOME_NUM];
};
#endif // FASTA_H_
