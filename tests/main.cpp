#include <iostream>
#include <unordered_map>
#include <fstream>
#include <stdlib.h>
#include "structure.h"
using namespace std;
int main()
{
    unordered_map<string,data> candidate;
    unordered_map<string,string> mite;
    string filepath="kmer_count.txt";
    string mitepath="mite_candidates.fsa";
    string seq,temp;
    double score1,score2;
    int TP,FP;
    const char *fpath,*fpath1;
    fpath=filepath.c_str();
    ifstream ifs(fpath);
    getline(ifs,temp);
    getline(ifs,temp);
    getline(ifs,temp);
    while(!ifs.eof())
    {
        int temp_len=temp.length();
        int site=0;
        char strnum[20];
        for (int i=0;i<=temp_len;)
        {
            site++;
            int num=0;
            while (temp[i+num]!='\n'&&temp[i+num]!='\t'&&temp[i+num]!='\0'&&temp[i+num]!=' ')
            {
                strnum[num]=temp[i+num];
                num++;
            }
            strnum[num]='\0';
            switch(site)
            {
                case 1:seq=strnum;break;
                case 2:score1=atof(strnum);break;
                case 3:score2=atof(strnum);break;
                case 4:TP=atoi(strnum);break;
                case 5:FP=atoi(strnum);break;
                default :break;
            }
            i=i+num+1;
        }
        candidate.insert(pair<string,data>(seq,data(score1,score2,TP,FP)));
        getline(ifs,temp);
    }
    ifs.close();
    string mitename,miteseq;
    fpath1=mitepath.c_str();
    ifstream ifs1(fpath1);
    getline(ifs1,temp);
    while(!ifs1.eof())
    {
        if (temp[0]=='>')
        {
            mitename=temp;
            getline(ifs1,temp);
            miteseq="";
            while (!ifs1.eof()&&temp[0]!='>')
            {
                miteseq=miteseq+temp;
                getline(ifs1,temp);
            }
            mite.insert(pair<string,string>(mitename,miteseq));
        }
    }
    string shortseq;
    double sum;
    unordered_map<string,string>::iterator iter1;
    unordered_map<string,int> mitescore;
    for (iter1=mite.begin();iter1!=mite.end();iter1++)
    {
        sum=0;
        int len=iter1->second.length();
        cout<<iter1->first<<endl;
        for (int i=0;i<len-5;i++)
        {
            shortseq=iter1->second.substr(i,6);
            unordered_map<string,data>::iterator iter=candidate.find(shortseq);
            if (iter!=candidate.end())
            sum=sum+iter->second.score1;
        }
        sum=sum/(len-5);
        cout<<sum<<endl;
        mitescore.insert(pair<string,int>(iter1->first,sum));
    }
    ifs1.close();
    return 0;
}
