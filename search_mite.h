#ifndef SEARCH_MITE_H_
#define SEARCH_MITE_H_

#include "mite.h"
#include "genome.h"
#include "variable.h"
#include "omp.h"
#include <algorithm>
#include <array>
#include <iostream>

extern int posMite;

template <typename T>
bool check_tir(const T& chromStr,int& i,int& j)
{
  if(chromStr[i]=='A' && chromStr[j]=='T')return true;
  if(chromStr[i]=='T' && chromStr[j]=='A')return true;
  if(chromStr[i]=='C' && chromStr[j]=='G')return true;
  if(chromStr[i]=='G' && chromStr[j]=='C')return true;
  return false;
}
template <typename T>
int check_tsd(const T& chromStr,int& psi,int& psj, int& i)
{
  for(i=0;i<MAX_LENGTH_TSD;i++,psi--,psj++)
  {
    if(psi<0) break;
    if(chromStr[psi]!=chromStr[psj]) break;
  }
  return i;
}
template <typename T>
int parallel_search_mite(const T& chromStr,const int pChrom, Mite* pMite)
{
  const int LEN=std::char_traits<char>::length(chromStr);
  int minI,maxI,lower_bound,pi,pj,ind,psi,psj,tirlen,i,tsdlen;
  omp_lock_t lock;
  omp_init_lock(&lock);
#pragma omp parallel for num_threads(NUM_USED_THREADS),schedule(dynamic),shared(chromStr,pMite,posMite),private(minI,maxI,lower_bound,pi,pj,ind,psi,psj,tirlen,i,tsdlen)
  for(int deltaL=1-LEN;deltaL<=LEN-1;deltaL++)
  {
    minI=std::max(0,deltaL);
    lower_bound=static_cast<int>(ceil((LEN+deltaL-MAX_LENGTH_MITE)/2.0));
    minI=std::max(minI,lower_bound);
    maxI=static_cast<int>(floor(LEN+deltaL-1)/2.0);
    if(maxI-minI+1<MIN_LENGTH_TIR){}
    else
    {
      pi=minI;
      pj=LEN-pi-1+deltaL;
      ind=0;tirlen=0;
      for(;pi<=maxI;pi++,pj--)
      {
        if(check_tir(chromStr,pi,pj))
        {
          if(ind==0)
          {
            psi=pi;psj=pj;ind=1;
          }
          tirlen++;
        }else
        {
          if(ind==1)
          {
            tsdlen=check_tsd(chromStr,psi,psj,i);
            if(tirlen>=MIN_LENGTH_TIR && tsdlen>1 && (psj-psi+1)>=50)
            {
              // Save it as a mite candiate.
              omp_set_lock(&lock);
              #pragma omp flush(posMite)
              posMite++;
              if(posMite>=MAX_MEMBER_MITE)
              {
                std::cerr<<"Please enlarge the capacity of Mite stack!"<<std::endl;
              }else{
                std::cout << "Mite: "<<posMite << std::endl;
                pMite[posMite].start=psi-tsdlen+1;
                pMite[posMite].tirLen=tirlen;
                pMite[posMite].tsdLen=tsdlen;
                pMite[posMite].wholeLen=psj-psi+2*tsdlen+1;
                pMite[posMite].pChrom=pChrom;
                std::cout <<"Mite:"<<posMite<<std::endl;
                omp_unset_lock(&lock);
              }
            }
            ind=0;tirlen=0;
          }
        }
      }
    }
  }
  omp_destroy_lock(&lock);
  return 0;
}

bool search_mite(Genome& g)
{
  int gsize=g.getNumChrom();
  Mite* pMite=new Mite[MAX_MEMBER_MITE];
  posMite=0;
  for(int i=0;i<gsize;i++)
  {
    std::cout <<"Chromosom,posMite:" << i <<"," <<posMite <<std::endl;
    parallel_search_mite(g.getChrom(i),i,pMite);
  }
  delete [] pMite;
  return true;
}
#endif
