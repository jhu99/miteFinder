#ifndef STRUCTURE_H_INCLUDED
#define STRUCTURE_H_INCLUDED
#include <unordered_map>

using namespace std;


void readscore(std::unordered_map<string,Pattern_value> &candidate, std::string filepath)
{
    string seq,temp;
    double score1=0,score2=0;
    int TP=0,FP=0;
    const char *fpath;
    fpath=filepath.c_str();
    ifstream ifs(fpath);
    getline(ifs,temp);
    getline(ifs,temp);
    getline(ifs,temp);
    while(!ifs.eof())
    {
        int temp_len=(int)temp.length();
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
        candidate.insert(pair<string,Pattern_value>(seq,Pattern_value(score1,score2,TP,FP)));
        getline(ifs,temp);
    }
    ifs.close();
}

/*void caculate_value(unordered_map<string,string> &mite,
                   unordered_map<string,double> &mitescore,
                   unordered_map<string,Pattern_value> &candidate)
{
    string shortseq;
    double sum;
    unordered_map<string,string>::iterator iter1;
    for (iter1=mite.begin();iter1!=mite.end();iter1++)
    {
        sum=0;
        int len=(int)iter1->second.length();
        cout<<iter1->first<<endl;
        for (int i=0;i<len-5;i++)
        {
            shortseq=iter1->second.substr(i,6);
            unordered_map<string,Pattern_value>::iterator iter=candidate.find(shortseq);
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
}*/
#endif // STRUCTURE_H_INCLUDED
