#include "banditcontainer.h"

banditContainer::banditContainer()
{

}

banditContainer::banditContainer(std::vector<bandit> bandCon)
{
    for(size_t i = 0; i < bandCon.size(); i++)
    {
        mBandCon.push_back(bandCon.at(i));
        resEst.push_back(0);
        resReal.push_back(0);
        bufResEst.push_back(0);
        count.push_back(0);
    }

    resEstAll.resize(bandCon.size());
}

void banditContainer::add(bandit band)
{
    mBandCon.push_back(band);
}

std::vector<bandit> banditContainer::getBandCon()
{
    return mBandCon;
}

void banditContainer::ucb(unsigned int iterations, int c, double epsilon, double alpha, unsigned int range, int sets, int numBandits)
{
    double bufVal = 0, val = 0, highAvg, mean = 0, currReward = 0, avg_reward = 0.0, gradientCounter = 0;
    bool is_baseline = false;
    unsigned int action = 0, lastAction = 0, bestAction = 0, timesPickedC = 0;

    std::vector<double> H_t(numBandits,0);

    std::vector<double> Probs(numBandits,0);

    int best_bandit = 2;
    std::vector<double> best_bandit_count/*, H_t = {0 ,0, 0, 0}, Probs = {0.25, 0.25, 0.25, 0.25}*/;

    //printf("%c %15s %15s %15s %15s %15s %15s %15s %15s %15s %15s %15s %15s %15s %15s \n", 'i', "val", "act", "b count", "b est", "b real", "b1 count", "b1 est", "b1 real", "b2 count", "b2 est", "b2 real", "b3 count", "b3 est", "b3 real");

    for(int b = 0; b < sets; b++)
    {
        clearAll();
        best_bandit_count.clear();
        timesPickedC = 0;
        int counter = 1;
        double bandit [numBandits];
        int bandit_count [numBandits];
        double running_average [iterations];
        highAvg = std::numeric_limits<int>::min();
        gradientCounter = 0;
        //H_t.clear();


        for(size_t i = 0; i < mBandCon.size(); i++)
        {
            mBandCon.at(i).setMean(range);
            mean = mBandCon.at(i).getMean();
            H_t[i] = 0;
            bandit[i] = 0;
            bandit_count[i] = 0;

            if(highAvg < mean)
            {
                highAvg = mean;
                bestAction = i;
            }
        }

        best_bandit = bestAction;

        for(unsigned int i = 1; i <= iterations; i++)
        {
            bufVal = std::numeric_limits<int>::min();

            for(unsigned int j = 0; j < 4; j++)
            {
                resReal[j] = mBandCon.at(j).getReward(range);

                if(count[j] == 0)
                {
                    action = j;
                    break;
                }

                val = resEst[j] + (c * (sqrt(log(i) / static_cast<double>(count[j]))));

                if(bufVal < val)
                {
                    action = j;
                    bufVal = val;
                }
            }

            for(unsigned int t = 0; t < 4; t++)
            {
                if(action == t)
                {
                    if(count[t] == 0)
                    {
                        resEst[t] = resReal[t];
                    }
                    else
                    {
                        resEst[t] = bufResEst[t] + ((1 / static_cast<double>(count[t])) * (resReal[t] - bufResEst[t]));
                    }

                    bufResEst[t] = resEst[t];
                    count[t]++;

                    if(bestAction == action)
                    {
                        timesPickedC++;
                    }

                    timesPicked.push_back((((static_cast<double>(timesPickedC)) / static_cast<double>(i))) * 100);

                    if(b == 0)
                    {
                        runningAvgAvg.push_back(resEst[action]);
                        timesPickedAvg.push_back(timesPicked.at(i - 1));
                    }
                    else
                    {
                        runningAvgAvg .at(i - 1) += resEst[action];
                        timesPickedAvg.at(i - 1) += timesPicked.at(i - 1);
                    }
                }

                resEstAll.at(t).push_back(resEst[t]);
            }

{
                //            if(action != lastAction || i == 1 || i == iterations)
                //            {
                //                if(i > 999)
                //                {
                //                    printf("%d %12f %15d %15d %15f %15f %15d %15f %15f %15d %15f %15f %15d %15f %15f \n", i, val, action + 1, count[0], resEst[0], resReal[0], count[1], resEst[1], resReal[1], count[2], resEst[2], resReal[2], count[3], resEst[3], resReal[3]);
                //                }
                //                else if(i > 99)
                //                {
                //                    printf("%d %13f %15d %15d %15f %15f %15d %15f %15f %15d %15f %15f %15d %15f %15f \n", i, val, action + 1, count[0], resEst[0], resReal[0], count[1], resEst[1], resReal[1], count[2], resEst[2], resReal[2], count[3], resEst[3], resReal[3]);
                //                }
                //                else if (i > 9)
                //                {
                //                    printf("%d %14f %15d %15d %15f %15f %15d %15f %15f %15d %15f %15f %15d %15f %15f \n", i, val, action + 1, count[0], resEst[0], resReal[0], count[1], resEst[1], resReal[1], count[2], resEst[2], resReal[2], count[3], resEst[3], resReal[3]);
                //                }
                //                else
                //                {
                //                    printf("%d %15f %15d %15d %15f %15f %15d %15f %15f %15d %15f %15f %15d %15f %15f \n", i, val, action + 1, count[0], resEst[0], resReal[0], count[1], resEst[1], resReal[1], count[2], resEst[2], resReal[2], count[3], resEst[3], resReal[3]);
                //                }

                //                lastAction = action;
                //            }
            }

        }

        for(int i = 0; i < iterations; ++i){
            int picked [numBandits];
            for(int j = 0; j < numBandits; ++j){
                picked[j] = 0;
            }
            int pickedBandit = 0;
            double highNumb = std::numeric_limits<int>::min();
            int random = 0;
            double eps = ((double) rand() / (RAND_MAX));

            if(eps <= epsilon){
                counter = 0;
                //make a random guess
                int pick;
                pick = rand() % numBandits;
                //std::cout << bandit[pick] + (1.0/(i+1))*(helper_function(pick)-bandit[pick])  << "the value calculated " << std::endl;
                bandit[pick] = bandit[pick] + (1.0/(bandit_count[pick]+1))*(mBandCon.at(pick).getReward(range)-bandit[pick]);
                bandit_count[pick]++;
                pickedBandit = pick;

            }
            else {
                for(int j = 0; j < numBandits; ++j){
                    if(highNumb < bandit[j]){
                        highNumb = bandit[j];
                    }
                }
                for(int j = 0; j < numBandits; ++j){
                    if(bandit[j] == highNumb){
                        random++;
                        picked[j] = 1;

                    }
                }
                if(random == 1){
                    for(int j = 0; j < numBandits; ++j){
                        if(picked[j] == 1){
                            bandit[j] = bandit[j]+(1.0/(bandit_count[j]+1))*(mBandCon.at(j).getReward(range)-bandit[j]);
                            bandit_count[j]++;
                            pickedBandit = j;
                        }
                    }
                }
                else if(random == numBandits) {
                    int pick;

                    pick = rand() % numBandits;
                    //std::cout << "pick: " << pick << std::endl;
                    //std::cout << bandit[pick] + (1.0/(i+1))*(helper_function(pick)-bandit[pick])  << "the value calculated " << std::endl;
                    bandit[pick] = bandit[pick] + (1.0/(bandit_count[pick]+1))*(mBandCon.at(pick).getReward(range)-bandit[pick]);
                    bandit_count[pick]++;
                    pickedBandit = pick;
                }
                else {
                    int pick = rand() % random+1;
                    int counter = 0;

                    for(int j = 0; j < numBandits; ++j){
                        if(picked[j] == 1){
                            counter++;
                            if(counter == pick){
                                bandit[j] = bandit[j] + (1.0/(bandit_count[j]+1))*(mBandCon.at(j).getReward(range)-bandit[j]);
                                bandit_count[j]++;
                                pickedBandit = j;
                                break;
                            }
                        }
                    }

                   /* else if (pick == 1) {
                        int counter = 0;
                        for(int j = 0; j < 4; ++j){
                            if(picked[j] == 1){
                                counter++;
                                if(counter == 2){
                                    bandit[j] = bandit[j] + (1/(bandit_count[j]+1))*(mBandCon.at(j).getReward(range)-bandit[j]);
                                    bandit_count[j]++;
                                    pickedBandit = j;
                                    break;
                                }
                            }
                        }
                    }
                    else if (pick==2) {
                        int counter = 0;
                        for(int j = 0; j < 4; ++j){
                            if(picked[j] == 1){
                                counter++;
                                if(counter =73){
                                    bandit[j] = bandit[j] + (1/(bandit_count[j]+1))*(mBandCon.at(j).getReward(range)-bandit[j]);
                                    bandit_count[j]++;
                                    pickedBandit = j;
                                    break;
                                }
                            }
                        }
                    }*/
                }
            }

            if(bandit_count[best_bandit] == 0){
                best_bandit_count.push_back(0.0);
            }
            else {
                best_bandit_count.push_back(((bandit_count[best_bandit])/(i+1.0))*100);
            }

            counter++;
            if(b == 0)
            {
                runAvgEp.push_back(best_bandit_count.at(i));
                timesAvgEp.push_back(bandit[pickedBandit]);

            }
            else
            {
                runAvgEp.at(i) += best_bandit_count.at(i);
                timesAvgEp.at(i) += bandit[pickedBandit];
            }

        }

        // Gradient bandit algorithm
        for (int i = 1; i <= iterations; ++i)
        {
            //Calculate Probabilities
            double overallSum = 0.0;
            for (int var = 0; var < numBandits; ++var)
            {
               overallSum += exp(H_t[var]);
            }

            for (int var = 0; var < numBandits; ++var)
            {
               Probs[var] = exp(H_t[var]) / overallSum;
            }

            // Calculate average reward per every time step
            avg_reward = avg_reward + (1.0 / i) * (currReward - avg_reward);
            double baseline;
            if (is_baseline)
            {
                baseline = 0.0;
            }
            else
            {
                baseline = avg_reward;
            }
            int index = 0;

            //Evaluate current reward
            // Get probability value
            double value = ((double) rand() / (RAND_MAX));

            /*for(int band = 0; band < H_t.size(); band++)
            {

            }*/
            // Choose what bandit to take
            if ((0.0 <= value) && (value <= Probs[0]))
            {
                currReward = mBandCon.at(0).getReward(range);
                index = 0;
            }
            else if ((Probs[0] < value) && (value <= Probs[0]+Probs[1]))
            {
                currReward=mBandCon.at(1).getReward(range);
                index = 1;
            }
            else if ((Probs[0]+Probs[1] < value) && (value <= Probs[0]+Probs[1]+Probs[2]))
            {
                currReward=mBandCon.at(2).getReward(range);
                index = 2;
            }
            else
            {
                currReward=mBandCon.at(3).getReward(range);
                index = 3;
            }

            // Increment counter of optimal bandits picked, if picked
            if(index == bestAction)
            {
                gradientCounter++;
            }

            //Update all H
            for (int j = 0; j < 4; ++j)
            {
                if (j == index)
                {
                        H_t[j] = H_t[j] - alpha * (currReward-baseline) * Probs[j];
                }
                else
                {
                        H_t[j] = H_t[j] - alpha * (currReward-baseline) * (1 - Probs[j]);
                }
            }


            // Save values
            if(b == 0)
            {
                gradientMean.push_back(avg_reward);
                gradientTimes.push_back((gradientCounter / static_cast<double>(i)) * 100);
            }
            else
            {
                gradientMean.at(i - 1) += avg_reward;
                gradientTimes.at(i - 1) += (gradientCounter / static_cast<double>(i)) * 100;
            }

        }
    }

    banditContainer::writeUcbToFile(iterations, sets);
}

void banditContainer::writeUcbToFile(unsigned int iterations, int sets)
{
    std::string fileString = "C:/Users/vikpo/desktop", ucb = "/ucb.txt", ep = "/epsilon.txt", grad = "/gradient.txt";
    std::ofstream fileB;

    fileB.open(fileString + ucb);
    fileB.clear();
    for(unsigned int j = 0; j < static_cast<unsigned int>(iterations); j++)
    {
        fileB << runningAvgAvg.at(j) / static_cast<double>(sets) << " " << timesPickedAvg.at(j) /  static_cast<double>(sets) << "\n";
    }
    fileB.close();

    fileB.open(fileString + ep);
    for(unsigned int j = 0; j < static_cast<unsigned int>(iterations); j++)
    {
        fileB << timesAvgEp.at(j) /  static_cast<double>(sets) << " " << runAvgEp.at(j)/  static_cast<double>(sets) << "\n";
    }
    fileB.close();

    fileB.open(fileString + grad);
    for(unsigned int j = 0; j < static_cast<unsigned int>(iterations); j++)
    {
        fileB << gradientMean.at(j) /  static_cast<double>(sets) << " " << gradientTimes.at(j) /  static_cast<double>(sets)  << "\n";
    }
    fileB.close();
    std::cout << "Wrote to file " << fileString << std::endl;
}

void banditContainer::clearAll()
{
    timesPicked.clear();
    runningAvg.clear();

    for(size_t i = 0; i < mBandCon.size(); i++)
    {
        count.at(i) = 0;
        resEst.at(i) = 0;
    }

    resEstAll.resize(mBandCon.size());
}

void banditContainer::greedy_method(double epsilon, int steps)
{

    double eps = epsilon*100;
    double percent = steps/100;
    int counts = percent*eps;
    double bandit [] = {0, 0, 0, 0};
    int bandit_count [] = {0, 0, 0, 0};
    double running_average [steps];
    int counter = 1;
    int best_bandit = 2;
    std::vector<double> best_bandit_count;
    for(int i = 0; i < steps; ++i){
        int picked [] = {0, 0, 0, 0};
        double highNumb = -100;
        int random = 0;
        if(counter == steps/counts){
            counter = 0;
            //make a random guess
            int pick;
            srand(time(NULL));
            pick = rand() % 4;
            std::cout << pick << " pick " << std::endl;
            std::cout << "random 4" << std::endl;
            //std::cout << bandit[pick] + (1.0/(i+1))*(helper_function(pick)-bandit[pick])  << "the value calculated " << std::endl;
            bandit[pick] = bandit[pick] + (1.0/(bandit_count[pick]+1))*(mBandCon.at(pick).getReward(10)-bandit[pick]);
            bandit_count[pick]++;

        }
        else {
            for(int j = 0; j < 4; ++j){
                std::cout << bandit[j] << " " << j << std::endl;
                if(highNumb < bandit[j]){
                    highNumb = bandit[j];
                }
            }
            for(int j = 0; j < 4; ++j){
                if(bandit[j] == highNumb){
                    random++;
                    picked[j] = 1;

                }
            }
            if(random == 1){
                for(int j = 0; j < 4; ++j){
                    if(picked[j] == 1){
                        std::cout << "random 1" << std::endl;
                        bandit[j] = bandit[j]+(1.0/(bandit_count[j]+1))*(mBandCon.at(j).getReward(10)-bandit[j]);
                        bandit_count[j]++;
                    }
                }
            }
            else if(random == 4) {
                int pick;
                srand(time(NULL));
                pick = rand() % random;
                std::cout << pick << " pick " << std::endl;
                std::cout << "random 4" << std::endl;
                //std::cout << bandit[pick] + (1.0/(i+1))*(helper_function(pick)-bandit[pick])  << "the value calculated " << std::endl;
                bandit[pick] = bandit[pick] + (1.0/(bandit_count[pick]+1))*(mBandCon.at(pick).getReward(10)-bandit[pick]);
                bandit_count[pick]++;
            }
            else {
                std::cout << "random else " << std::endl;
                int pick = rand() % random;
                if(pick == 0){
                    for(int j = 0; j < 4; ++j){
                        if(picked[j] == 1){
                            bandit[j] = bandit[j] + (1.0/(bandit_count[j]+1))*(mBandCon.at(j).getReward(10)-bandit[j]);
                            bandit_count[j]++;
                            break;
                        }
                    }
                }
                else if (pick == 1) {
                    int counter = 0;
                    for(int j = 0; j < 4; ++j){
                        if(picked[j] == 1){
                            counter++;
                            if(counter == 2){
                                bandit[j] = bandit[j] + (1/(bandit_count[j]+1))*(mBandCon.at(j).getReward(10)-bandit[j]);
                                bandit_count[j]++;
                                break;
                            }
                        }
                    }
                }
                else if (pick==2) {
                    int counter = 0;
                    for(int j = 0; j < 4; ++j){
                        if(picked[j] == 1){
                            counter++;
                            if(counter == 3){
                                bandit[j] = bandit[j] + (1/(bandit_count[j]+1))*(mBandCon.at(j).getReward(10)-bandit[j]);
                                bandit_count[j]++;
                                break;
                            }
                        }
                    }
                }
            }

        }
        running_average[i] = (bandit[0]+bandit[1]+bandit[2]+bandit[3])/4;
        if(bandit_count[best_bandit] == 0){
            best_bandit_count.push_back(0.0);
        }
        else {
            best_bandit_count.push_back(((bandit_count[best_bandit])/(i+1.0))*100);
        }

        counter++;
        //std::cout << "running average: " << running_average[i] <<std::endl;
    }

    std::string fileString = "C:/Users/vikpo/desktop";
    std::string file = "/running_average.txt";
    std::ofstream fileB;

    fileB.open(fileString + file);
    fileB.clear();
    for(int j = 0; j < steps; j++)
    {
        fileB << running_average[j]  << " " << best_bandit_count[j] <<  "\n";
    }

    fileB.close();

    std::cout << "the size of b1 " << bandit_count[0] << std::endl;
    std::cout << "the size of b2 " << bandit_count[1] << std::endl;
    std::cout << "the size of b3 " << bandit_count[2] << std::endl;
    std::cout << "the size of b4 " << bandit_count[3] << std::endl;


}


