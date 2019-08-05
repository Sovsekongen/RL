#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

#define seed 5

struct bandit1{
    public:
        bandit1();

        int operator()(){
            return 8;
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
            srand (seed);
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
            srand (seed);
            int value = rand() % diff + lb;
            int array[] = {val1, value, val2};
            int val = rand() %2+1;
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
            srand (seed);
            int value = rand() % 100 + 1;
            if (value<=88){
                return val1;
            }else{
                return val2;
            }
        }
};

int main()
{
    bandit1 b1;
    cout << "bandit1: " << b1() << endl;
    bandit2 b2(20,50);
    cout << "bandit2: " << b2() << endl;
    bandit3 b3(20,50,2,100);
    cout << "bandit3: " << b3() << endl;
    bandit4 b4(3,42);
    cout << "bandit4: " << b4() << endl;
    return 0;
}
