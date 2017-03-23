#include "fasta.h"

Genomes::Genomes()
{
}
Genomes::~Genomes()
{
  for(int i=1;i<chrnum;i++)
  {
    free(chromosomes[i]);
  }
}
// Read files formatted in FASTA, requiring the number of sequences not 
// greater than MAX_CHROSOME_NUM.
int Genomes::readGenome(string filepath)
{
     int chr[MAX_CHROSOME_NUM];
     chrnum=0;
     const char *fpath;
     fpath=filepath.c_str();
     ifstream ifs(fpath);
     string temp;
     getline(ifs,temp);
     //Firstly, get the length of genomes from the input file.
     while(!ifs.eof()){
        if (temp[0]=='>'){
            chrnum++;
            chr[chrnum]=0;
            getline(ifs,temp);
            while (temp[0]!='>'&&!ifs.eof()){
                chr[chrnum]+=temp.length();
                getline(ifs,temp);
            }
        }
     }
     ifs.clear();
     ifs.seekg(0,ios::beg);
     for (int i=1;i<=chrnum;i++)
     chromosomes[i]=(char *)malloc(chr[i]+100);
     int sum=0,num;
     getline(ifs,temp);
     //Secondly, read the sequence again.
     while(!ifs.eof()){
        if (temp[0]=='>'){
            sum++;
            chrnote[sum]=(char *)malloc(100);
            temp.copy(chrnote[sum],temp.length());
            num=0;
            getline(ifs,temp);
            while (temp[0]!='>'&&!ifs.eof()){
                temp.copy(chromosomes[sum]+num,temp.length());
                num+=temp.length();
                getline(ifs,temp);
            }
            chr_length[sum]=num;
        }
     }
     ifs.close();
     return 0;
}
//Get a specific sequence fragment.
string Genomes::getFragment(int chr,int start,int count)
{
    if(start+count-1>chr_length[chr]){
      string tmp="error";
      return tmp;
    }
    string seq(chromosomes[chr]+start-1,chromosomes[chr]+start+count-1);
    return seq;
}

