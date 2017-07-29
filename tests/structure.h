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

#endif // STRUCTURE_H_INCLUDED
