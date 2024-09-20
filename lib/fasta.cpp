#include "fasta.h"
#include <vector>
#include <thread>


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

void Genomes::readChromosomeSequence1(const string& filepath, int chrNum, int chrLength) {
    // ... 用于从文件读取染色体序列的代码 ...
    // 此函数将被不同的线程调用来并行读取不同的染色体序列
    ifstream localIfs(filepath);
    if (!localIfs) {
        cerr << "Failed to open file: " << filepath << endl;
        return;
    }

    string line;
    int currentChr = 0;
    bool isReadingChr = false;
    int readLength = 0;

    while (getline(localIfs, line)) {
        if (line[0] == '>') {
            currentChr++;
            if (currentChr == chrNum) {
                isReadingChr = true; // 开始读取目标染色体的序列
                continue;
            } else if (currentChr > chrNum) {
                break; // 超出目标染色体，结束读取
            }
        }

        if (isReadingChr) {
            if (readLength + line.length() > chrLength) {
                // 防止读取超过预分配的内存大小
                line = line.substr(0, chrLength - readLength);
            }

            // 复制当前行到染色体序列中
            memcpy(chromosomes[chrNum] + readLength, line.c_str(), line.length());
            readLength += line.length();

            if (readLength >= chrLength) {
                // 已经读取到足够长度的序列，结束读取
                break;
            }
        }
    }

    chr_length[chrNum] = readLength; // 更新实际读取的染色体长度

    if (readLength < chrLength) {
        cerr << "Warning: Did not read full length for chromosome " << chrNum << ". Read " << readLength << " characters." << endl;
    }

    localIfs.close();



}




int Genomes::readGenome(string filepath)
{    vector<std::thread> threads; // to store threads for multi-threading
     int chr[MAX_CHROSOME_NUM];//chr储存每个染色体长度
     chrnum=0;//染色体数量
     const char *fpath;
     fpath=filepath.c_str();
     ifstream ifs(fpath);
     string temp;
     getline(ifs,temp);//将文件内的一行数据读取到temp里面
     //Firstly, get the length of genomes from the input file.
     while(!ifs.eof()){//这个循环确定每个染色体的长度
        if (temp[0]=='>'){//当没遇到以“>”开头的行时，开始处理新的序列，计算每个序列总长度，将每行的长度累加到‘chr【】’
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
    
//这段代码主要实现了从给定文件路径读取基因组数据，统计每个染色体的长度，
//并将文件指针重置到文件开始位置以便后续操作。

      for (int i = 1; i <= chrnum; ++i) {
        threads.push_back(std::thread(&Genomes::readChromosomeSequence1, this, filepath, i, chr[i]));
    }//第二次遍历使用多线程读取

    // Join threads
    for (auto &th : threads) {
        if (th.joinable()) {
            th.join();
        }
    }

    /*  int sum=0,num;
     getline(ifs,temp);
     //Secondly, read the sequence again.


    std::vector<std::thread> threads;//new
    int sum = 0, num;//new

     while(!ifs.eof()){//用于实际读取染色体序列并将它们存储在分配的内存中
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
     ifs.close(); */
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

