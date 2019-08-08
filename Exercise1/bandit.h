#ifndef BANDIT_H
#define BANDIT_H

#include <random>
#include <iostream>
#include <time.h>

class bandit
{
public:
    bandit();
    bandit(double range);

    double getReward(double range);

    double getMean();
    double getRandomFloat();
    void setMean(double mean);
private:
    double avg, mean;
};

#endif // BANDIT_H
