#ifndef MITE_H_
#define MITE_H_

#include <unordered_map>
#include <vector>

class Seed
{
public:
    int pos1;
    int pos2;
    int pos3;
    int pos4;
    int tsd;
    int mismatch_tir;
    int mis_tirpos;
	double ave_score;
    Seed()
    {
        this->pos1=1;
        this->pos2=1;
        this->pos3=-1;
        this->pos4=-1;
        this->tsd=0;
    }
    Seed(int step)
    {
        this->pos1=step;
        this->pos2=step;
        this->pos3=step*(-1);
        this->pos4=step*(-1);
    }
    Seed(int p1,
         int p2,
         int p3,
         int p4,
         int mis,
         int pos)
    {
        this->pos1=p1;
        this->pos2=p2;
        this->pos3=p3;
        this->pos4=p4;
        this->tsd=0;
        this->mismatch_tir=mis;
        this->mis_tirpos=pos;
    }
    ~Seed(){}
    bool operator<(const Seed& other) const
    {
        if(this->pos1 < other.pos1) return true;
        if(this->pos1 > other.pos1) return false;
        if(this->pos2 < other.pos2) return true;
        if(this->pos2 > other.pos2) return false;
        if(this->pos3 < other.pos3) return true;
        if(this->pos3 > other.pos3) return false;
        if(this->pos4 < other.pos4) return true;
        if(this->pos4 > other.pos4) return false;
        return false;
    }
    bool operator<=(const Seed& other) const
    {
        if(this->pos1 < other.pos1) return true;
        if(this->pos1 > other.pos1) return false;
        if(this->pos2 < other.pos2) return true;
        if(this->pos2 > other.pos2) return false;
        if(this->pos3 < other.pos3) return true;
        if(this->pos3 > other.pos3) return false;
        if(this->pos4 < other.pos4) return true;
        if(this->pos4 > other.pos4) return false;
        return true;
    }
    bool operator==(const Seed& other) const
    {
        return (this->pos1==other.pos1 && this->pos2==other.pos2 && this->pos3==other.pos3 && this->pos4==other.pos4);
    }
    Seed& operator+=(const Seed& other)
    {
        this->pos1+=other.pos1;
        this->pos2+=other.pos2;
        this->pos3+=other.pos3;
        this->pos4+=other.pos4;
        return (*this);
    }
    Seed& operator+=(const int i)
    {
        this->pos2+=i;
        this->pos3-=i;
        return (*this);
    }
};

struct Pattern_value
{
    double score1;
    double score2;
    int TP;
    int FP;
    Pattern_value(double s1, double s2, int T, int F)
    {
        this->score1=s1;
        this->score2=s2;
        this->TP=T;
        this->FP=F;
    }
};
#endif
