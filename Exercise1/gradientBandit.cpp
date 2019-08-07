#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cmath>

using namespace std;

//#define seed time(NULL)
struct bandit1{
    public:
        int val;
        bandit1(int value) : val(value){}
        int operator()(){
            return val;
        }
};

struct bandit2{
    public:
        int lb;
        int ub;
        bandit2(int lb, int ub) : lb(lb), ub(ub){
        }
        int operator()(){
            int diff = ub-lb;
            //srand (seed);
            return rand() % diff + lb;
        }
};

struct bandit3{
    public:
        int  lb;
        int ub;
        int val1;
        int val2;

        bandit3(int lb, int ub, int val1, int val2) : lb(lb), ub(ub), val1(val1), val2(val2){
        }
        int operator()(){
            int diff = ub-lb;
            //srand (seed);
            int value = rand() % diff + lb;
            int array[] = {val1, value, val2};
            int val = rand() %3;
            return array[val];
        }
};

struct bandit4{
    public:
        int val1;
        int val2;
        bandit4(int val1, int val2) : val1(val1), val2(val2){
        }
        int operator()(){
            //srand (seed);
            int value = rand() % 100 + 1;
            if (value<=88){
                return val1;
            }else{
                return val2;
            }
        }
};

void gradientBanditAlgo(double alpha){
    double H_t[4] = {0,0,0,0};
    double Probs[4] = {0.25,0.25,0.25,0.25};
    double sumRewards = 0;
    int currReward = 0;
    bool is_baseline = false;
    double avg_reward = 0.0;

    bandit1 b1(-6);
    bandit1 b2(4);
    bandit1 b3(-5);
    bandit1 b4(0);

    for (int i=1;i<=1000;++i){
        //Calculate Probabilities
        double overallSum = 0;
        for (int var = 0; var < 4; ++var) {
           overallSum += exp(H_t[var]);
        }
        cout << "Wahrscheinlichkeiten ";
        for (int var = 0; var < 4; ++var) {
           Probs[var] = exp(H_t[var])/overallSum;
           cout << Probs[var] << " ";
        }
        cout << endl;

        avg_reward = avg_reward + (1.0 / i) * (currReward - avg_reward);
        double baseline;
        if (is_baseline){
            baseline = 0.0;
        }else {
            baseline = avg_reward;
        }
        int index = 0;

        //Evaluate current reward
        //int val = rand() % 100 + 1; //get random number in range of 1 and 100
        //double value = static_cast<double>(val);
        double value = ((double) rand() / (RAND_MAX));
        cout << "randnumber " << value << endl;
        if ((0.0<=value) && (value <=Probs[0])){
            currReward=b1();
            index = 0;
        }else if ((Probs[0] < value) && (value <= Probs[0]+Probs[1])){
            currReward=b2();
            index = 1;
        }else if ((Probs[0]+Probs[1] < value) && (value <= Probs[0]+Probs[1]+Probs[2])){
            currReward=b3();
            index = 2;
        }else {
            currReward=b4();
            index = 3;
        }
        std::cout << index << " " << currReward << endl;

        //Update all H
        for (int j=0;j<4;++j){
            if (j==index){
                    H_t[j]=H_t[j] - alpha*(currReward-baseline)*Probs[j];

            }
            else{
                    H_t[j]=H_t[j] - alpha*(currReward-baseline)*(1-Probs[j]);

            }
        }

        //Update sumRewards
        sumRewards += currReward;


    }

}

int main()
{
    gradientBanditAlgo(0.1);
}
