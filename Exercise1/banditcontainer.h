#ifndef BANDITCONTAINER_H
#define BANDITCONTAINER_H

#include <vector>
#include <bandit.h>
#include <iostream>
#include <fstream>
#include <ios>
#include <limits>

class banditContainer
{
public:
    banditContainer();
    banditContainer(std::vector<bandit> bandCon);

    void add(bandit band);
    std::vector<bandit> getBandCon();
    void ucb(unsigned int iterations, int c, double epsilon, double alpha, unsigned int range, int sets, int numBandits);
    void writeUcbToFile(unsigned int iterations, int sets);
    void clearAll();
    void greedy_method(double epsilon, int steps);
    void gradientBandit(double alpha);

private:
    std::vector<bandit> mBandCon;
    std::vector<double> resEst, resReal, bufResEst, runningAvg, timesPicked, runningAvgAvg, timesPickedAvg, runAvgEp, timesAvgEp, gradientMean, gradientTimes, avgSumRewards, avgTimesPicked;
    std::vector<std::vector<double>> resEstAll;
    std::vector<unsigned int> count;
};

#endif // BANDITCONTAINER_H
