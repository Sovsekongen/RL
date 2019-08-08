#include <iostream>
#include <banditcontainer.h>

using namespace std;

int main()
{
    srand(time(NULL));

    bandit b1(1);
    bandit b2(1);
    bandit b3(1);
    bandit b4(1);
    bandit b5(1);
    bandit b6(1);
    bandit b7(1);
    bandit b8(1);
    bandit b9(1);
    bandit b10(1);
    std::vector<bandit> banVec = {b1, b2, b3, b4, b5, b6, b7, b8, b9, b10};

    banditContainer bc(banVec);
    //iterations, c, epsilon, alpha, range, sets
    bc.ucb(1000, 1, 0.1, 1, 1, 2000, banVec.size());



    return 0;
}
