#ifndef STRUCTURE_H_INCLUDED
#define STRUCTURE_H_INCLUDED

class data
{
public:
    double score1;double score2;int TP;int FP;
    data(double s1,
         double s2,
         int T,
         int F)
    {
        this->score1=s1;
        this->score2=s2;
        this->TP=T;
        this->FP=F;
    }
    ~data(){}
};

#include <unordered_map>

void readscore(unordered_map<string,data> &candidate)
{
    string filepath="kmer_count.txt";
    string seq,temp;
    double score1,score2;
    int TP,FP;
    const char *fpath;
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
}
void caculate_value(unordered_map<string,string> &mite,
                   unordered_map<string,double> &mitescore,
                   unordered_map<string,data> &candidate)
{
    string shortseq;
    double sum;
    unordered_map<string,string>::iterator iter1;
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
        mitescore.insert(pair<string,double>(iter1->first,sum));
    }
    int m1=0,m2=0;
    ofstream outdata;
    outdata.open("result.txt",ios::app);
    unordered_map<string,double>::iterator iter2;
    for (iter2=mitescore.begin();iter2!=mitescore.end();iter2++)
    {
        if (iter2->second>=0.25)
            m1++;
        else
            m2++;//outdata<<iter2->first<<endl;
    }
    outdata.close();
    cout << m1 <<endl;
    cout << m2 <<endl;
}
#endif // STRUCTURE_H_INCLUDED
