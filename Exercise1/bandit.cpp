#include "bandit.h"


bandit::bandit()
{

}

bandit::bandit(double range)
{
    mean = (-range) + getRandomFloat() * (range - (-range));
}

double bandit::getMean()
{
    return mean;
}

void bandit::setMean(double range)
{
    mean = (-range) + getRandomFloat() * (range - (-range)); //rand() % (range * 2) + (-range);
//    std::cout << "mean: " << mean << std::endl;
}

double bandit::getReward(double range)
{
    double reward;
    reward = mean + ((-range) + getRandomFloat() * (range - (-range)));

//    std::cout << "reward: " << reward << std::endl;

    return  reward;
}

double bandit::getRandomFloat()
{
    return (static_cast<double>(rand()) / (RAND_MAX));
}
