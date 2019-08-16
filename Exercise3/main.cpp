#include <iostream>
#include <time.h>
#include <cmath>
#include <vector>
#include <fstream>

using namespace std;


void randomize (int arr[4][2], int n, int t[4])
{

    //srand (time(NULL));
    int temp[2];
    int tempt;

    for (int i = n - 1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i + 1);

        // Swap arr[i] with the element
        // at random index

        temp[0] = arr[i][0];
        temp[1] = arr[i][1];
        arr[i][0] = arr[j][0];
        arr[i][1] = arr[j][1];
        arr[j][0] = temp[0];
        arr[j][1] = temp[1];
        tempt = t[i];
        t[i] = t[j];
        t[j] = tempt;

    }
}

void randomize1 (int arr[1000][2], int n, int t[1000])
{

    //srand (time(NULL));
    int temp[2];
    int tempt;

    for (int i = n - 1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i + 1);

        // Swap arr[i] with the element
        // at random index

        temp[0] = arr[i][0];
        temp[1] = arr[i][1];
        arr[i][0] = arr[j][0];
        arr[i][1] = arr[j][1];
        arr[j][0] = temp[0];
        arr[j][1] = temp[1];
        tempt = t[i];
        t[i] = t[j];
        t[j] = tempt;

    }
}


void perceptron(int input[4][2], int t[4], double learning_rate, double w[3]){

    double error[4];
    double error_check = 0;
    int max_epoch = 200;
    int counter = 0;
    double v[4];
    double z = 0;
    int index = 0;
    for(int i = 0; i < 3; ++i)
    {
         w[i] = ((double)rand() / (RAND_MAX));
         //std::cout << w[i] << std::endl;
    }
    do
    {
        error_check = 0;
        randomize(input, 4, t);

        for(int j = 0; j < 4; ++j)
        {
            //index = rand() % j-1; //der skal generes et random input hvergang
            z = w[0]*input[j][0]+w[1]*input[j][1]+w[2]*(-1);
            if(z <= 0){
                v[j] = 0;
            }
            else {
                v[j] = 1;
            }
            error[j] = t[j]-v[j];
            for(int k = 0; k < 2; ++k){
                w[k] = w[k]+learning_rate*error[j]*input[j][k];
            }
            w[3] = w[3]+learning_rate*error[j]*(-1);
        }
        for(int i = 0; i < 4; ++i){
            error_check += pow(error[i],2);
        }
        //std::cout << error_check << std::endl;
        counter++;

    }while(error_check > 0.0001 and counter < max_epoch);

    std::cout << "it took " << counter << " to learn " << std::endl;
}

void multiple_perceptron(int input[1000][2], int t[4], double learning_rate, double w[2][3], double y[3], int dataSize){

    double errorCheck = 0;
    std::vector<double> errorVec;
    int max_epoch = 10000;
    int counter = 0;
    double x[2], v[dataSize], errorx[2], errory[dataSize];
    double z3=0;
    for(int i = 0; i < 2; ++i)
    {
        for(int j = 0; j < 3; ++j){
            w[i][j] = ((double)rand() / (RAND_MAX));

            if(i == 0)
            {
                y[j] = ((double)rand() / (RAND_MAX));

            }
        }
    }

    do
    {
        randomize1(input, dataSize, t);
        errorCheck = 0;
        for(int i = 0; i < dataSize; ++i){

            //regner hidden layer
            x[0] = input[i][0]*w[0][0]+input[i][1]*w[0][1]+(-1)*w[0][2];
            x[1] = input[i][0]*w[1][0]+input[i][1]*w[1][0]+(-1)*w[1][2];

            x[0] = 1/(1+exp(-x[0]));
            x[1] = 1/(1+exp(-x[1])); //kan ikke få det til at virke for sigmund
            /*for(int j = 0; j < 2; ++j){
                if(x[j] <= 0){
                    x[j] =0;
                }
                else {
                    x[j] = 1;
                }
            }*/

            //regner outputtet
            z3 = x[0]*y[0]+x[1]*y[1]+(-1)*y[2];

            v[i] = 1/(1+exp(-z3));

            //regner fejlen for outputtet
            errory[i] = v[i]*(1-v[i])*(t[i]-v[i]); //sigmund

            //regner fejlen for hidden layer
            for(int j = 0; j < 2; ++j)
            {
                errorx[j] = x[j]*(1-x[j])*errory[i]*y[j];
            }

            //opdater vægten for ydre lag
            for(int j = 0; j < 2; ++j)
            {
                y[j] = y[j]+learning_rate*errory[i]*x[j];
            }

                y[3] = y[3]+learning_rate*errory[i]*(-1);

            //opdatere vægt for hidden layer
            for(int j = 0; j < 2; ++j)
            {
                for(int k = 0; k < 2; ++k)
                {
                    w[j][k] = w[j][k]+learning_rate*errorx[j]*input[i][k];
                }

            }

            w[0][2] = w[0][2]+learning_rate*errorx[0]*(-1);
            w[1][2] = w[1][2]+learning_rate*errorx[1]*(-1);

        }
        for(int i = 0; i < dataSize; ++i){
            errorCheck += pow(errory[i],2);
        }
        if(counter < 800){
            errorVec.push_back(errorCheck);
        }

        for (int i = 0; i < 2; ++i) {
            //std::cout << x[i] << std::endl;
        }


        counter++;
    }while(errorCheck > 0.000001 and counter < max_epoch);
    std::cout << "number of times run " << counter << std::endl;
    std::string fileString = "C:/Users/peter/Desktop/xorerror.txt";
    std::ofstream fileB;
    fileB.open(fileString);
    fileB.clear();
    for(unsigned int i = 0; i < errorVec.size(); ++i){
        fileB << errorVec.at(i) << "\n";
    }

}

void check_multiple_perceptron_result(int input[4][2], int expected[4], double w[2][3], double y[3])
{
    double z3 = 0;
    double result[4];
    int correctResultCount = 0;
    double x[2];


    for(int i = 0; i < 1000; ++i)
    {
        randomize(input, 4, expected);
        for(int j = 0; j < 4; ++j)
        {
            //regner hidden layer
            x[0] = input[j][0]*w[0][0]+input[j][1]*w[0][1]+(-1)*w[0][2];
            x[1] = input[j][0]*w[1][0]+input[j][1]*w[1][0]+(-1)*w[1][2];

            x[0] = 1/(1+exp(-x[0])); //virker ikke for sigmund
            x[1] = 1/(1+exp(-x[1]));

            //regner outputtet
            z3 = x[0]*y[0]+x[1]*y[1]+(-1)*y[2];

            result[j] = 1/(1+exp(-z3));

            /*if(result[j] > 0.98){
                result[j] = 1;
            }
            else if(result[j] < 0.03){
                result[j] = 0;
            }*/


        }
        int counter = 0;
        for(int j = 0; j < 4; ++j)
        {
            if(result[j] == expected[j])
            {
                counter++;
            }
        }
        if(counter == 4)
        {
            correctResultCount++;
        }
    }


    for(int i = 0; i < 4; ++i)
    {
        std::cout << "this is the input: ";
        for(int j = 0; j < 2; ++j)
        {
            std::cout << input[i][j] << " ";
        }
        std::cout << "expected result: " << expected[i] << " real result: " << result[i] << std::endl;
    }
    std::cout << "number of correct results in 1000 runs " << correctResultCount << std::endl;
}

void checkResult(int input[4][2], int expected[4], double w[3])
{
    double z = 0;
    int result[4];
    int correctResultCount = 0;

    for(int j = 0; j < 1000; ++j){
        randomize(input, 4, expected);

        for(int i = 0; i < 4; ++i){

            z = input[i][0]*w[0]+input[i][1]*w[1]+(-1)*w[2];
            if(z <= 0){
                result[i] = 0;
            }
            else {
                result[i] = 1;
            }
            result[i] = z;
        }
        int counter = 0;
        for(int i = 0; i < 4; ++i){
            if(result[i] == expected[i]){
                counter++;
            }
        }
        if(counter == 4){
            correctResultCount++;
        }
    }

    for(int i = 0; i < 4; ++i)
    {
        std::cout << "this is the input: ";
        for(int j = 0; j < 2; ++j)
        {
            std::cout << input[i][j] << " ";
        }
        std::cout << "expected result: " << expected[i] << " real result: " << result[i] << std::endl;
    }
    std::cout << "number of correct results in 1000 runs " << correctResultCount << std::endl;

}


int main()
{
    srand(time(NULL));


    int input_and[4][2] = {{0,0},{0,1}, {1,0}, {1,1}};
    int t_and[4] = {0, 0, 0, 1};
    double w_and[3] = {0, 0, 0};

    int input_or[4][2] = {{0,0}, {0,1},{1,0},{1,1}};
    int t_or[4] = {0, 1, 1, 1};
    double w_or[3] = {0,0,0};

    int input_xor[1000][2];
    int input_for_result[4][2] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
    int input_t_for_result[4] = {0, 1, 1, 0};
    int t_xor[1000] = {0, 1, 1, 0};
    double w_xor[2][3];
    double y_xor[3];
    for(int i = 1; i <= 1000; ++i){
        if(i % 4 == 0){
            input_xor[i-1][0] = 1;
            input_xor[i-1][1] = 1;
            t_xor[i-1] = 0;
        }
        else if(i % 4 == 1){
            input_xor[i-1][0] = 0;
            input_xor[i-1][1] = 1;
            t_xor[i-1] = 1;
        }
        else if(i % 4 == 2){
            input_xor[i-1][0] = 1;
            input_xor[i-1][1] = 0;
            t_xor[i-1] = 1;
        }
        else {
            input_xor[i-1][0] = 0;
            input_xor[i-1][1] = 0;
            t_xor[i-1] = 0;
        }

    }

    std::cout << "------------------------------" << std::endl;
    std::cout << "this is the and function " << std::endl;
    perceptron(input_and, t_and, 0.1, w_and);
    checkResult(input_and, t_and, w_and);

    std::cout << "------------------------------" << std::endl;
    std::cout << "this is the or function " << std::endl;
    perceptron(input_or, t_or, 0.1, w_or);
    checkResult(input_or, t_or, w_or);

    //tror maaske at det er meningen at denne ikke kan virker, da datene ikke er adspredt (korrekt)

    std::cout << "------------------------------" << std::endl;
    std::cout << "this is the xor function " << std::endl;
    multiple_perceptron(input_xor, t_xor, 0.1, w_xor, y_xor, 1000);
    check_multiple_perceptron_result(input_for_result, input_t_for_result, w_xor, y_xor);

    return 0;
}
