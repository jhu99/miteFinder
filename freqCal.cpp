#include <iostream>
#include <unordered_map>
#include <fstream>
#include <malloc.h>
#include <string.h>
using namespace std;
struct matchnum
{
    int match;
    int mismach;
};
char* matchseq;
char* mismachseq;
int matchlen=0;
int mismachlen=0;
typedef unordered_map<string,matchnum> structure_map;
int main()
{
    matchnum ini;
    ini.match=0;
    ini.mismach=0;
    structure_map str_map;
    structure_map::iterator it;
    string temp1;
    string filepath1="matchresult.fsa";
    const char *fpath1;
    fpath1=filepath1.c_str();
    ifstream ifs1(fpath1);
    getline(ifs1,temp1);
    while(!ifs1.eof())
    {
       if (temp1[0]!='>')
       {
           matchlen+=temp1.length();
       }
       getline(ifs1,temp1);
    }
    ifs1.clear();
    ifs1.seekg(0,ios::beg);
    matchseq=(char *)malloc(matchlen+100);
    getline(ifs1,temp1);
    int num1=0;
    while(!ifs1.eof())
    {
        if (temp1[0]!='>')
        {
            temp1.copy(matchseq+num1,temp1.length());
            num1+=temp1.length();
        }
        getline(ifs1,temp1);
    }
    ifs1.close();
    string temp2;
    string filepath2="mismatchresult.fsa";
    const char *fpath2;
    fpath2=filepath2.c_str();
    ifstream ifs2(fpath2);
    getline(ifs2,temp2);
    while(!ifs2.eof())
    {
       if (temp2[0]!='>')
       {
           mismachlen+=temp2.length();
       }
       getline(ifs2,temp2);
    }
    ifs2.clear();
    ifs2.seekg(0,ios::beg);
    mismachseq=(char *)malloc(mismachlen+100);
    getline(ifs2,temp2);
    int num2=0;
    while(!ifs2.eof())
    {
        if (temp2[0]!='>')
        {
            temp2.copy(mismachseq+num2,temp2.length());
            num2+=temp2.length();
        }
        getline(ifs2,temp2);
    }
    ifs2.close();
    string fragment1;
    fragment1=string(matchseq,matchlen);
    for (int i=0;i<(matchlen-10);i++)
    {
        string key=fragment1.substr(i,10);
        if (str_map.find(key)!=str_map.end())
        {
            it=str_map.find(key);
            it->second.match++;
        }
        else
        {
            str_map.insert(pair<string,matchnum>(key,ini));
            it=str_map.find(key);
            it->second.match++;
        }
    }
    string fragment2;
    fragment2=string(mismachseq,mismachlen);
    for (int i=0;i<(mismachlen-10);i++)
    {
        string key=fragment2.substr(i,10);
        if (str_map.find(key)!=str_map.end())
        {
            it=str_map.find(key);
            it->second.mismach++;
        }
        else
        {
            str_map.insert(pair<string,matchnum>(key,ini));
            it=str_map.find(key);
            it->second.mismach++;
        }
    }
    for (it=str_map.begin();it!=str_map.end();it++)
        cout << it->first <<" "<< it->second.match <<" "<< it->second.mismach <<endl;
    free(matchseq);
    free(mismachseq);
    return 0;
}
