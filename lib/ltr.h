#ifndef LTR_H_
#define LTR_H_
#include <unordered_map>
#include <vector>

class Match
{
    public:
        int pos1;
        int pos2;
        int pos3;
        int pos4;
        int mismatch_ltr;
        int mismatch_num;
    Match()
    {
        this->pos1=1;
        this->pos2=1;
        this->pos3=1;
        this->pos4=1;
    }
    Match(int step)
    {
        this->pos1=step;
        this->pos2=step;
        this->pos3=step;
        this->pos4=step;
    }
    Match(int p1,int p2,int p3,int p4,int mis)
    {
        this->pos1 = p1;
        this->pos2 = p2;
        this->pos3 = p3;
        this->pos4 = p4;
        this->mismatch_ltr = mis;
        this->mismatch_num = 0;
    }
    bool operator==(const Match& other) const
    {
        return (this->pos1==other.pos1 && this->pos2==other.pos2 && this->pos3==other.pos3 && this->pos4==other.pos4);
    }
    bool operator<(const Match& other) const
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
    Match& operator+=(const Match& other)
    {
        this->pos1+=other.pos1;
        this->pos2+=other.pos2;
        this->pos3+=other.pos3;
        this->pos4+=other.pos4;
        return (*this);
    }

    Match& operator-=(const Match& other)
    {
        this->pos1-=other.pos1;
        this->pos2-=other.pos2;
        this->pos3-=other.pos3;
        this->pos4-=other.pos4;
        return (*this);
    }

    Match& operator+=(const int i)
    {
        this->pos2+=i;
        this->pos4+=i;
        return (*this);
    }
    Match& operator-=(const int i)
    {
        this->pos1-=i;
        this->pos3-=i;
        return (*this);
    }
};





#endif
