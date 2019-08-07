#include <algorithm>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <sstream>

/**
 * Basic implementation of iterative policy evaluation (see p. 75 in Sutton
 * and Barto (2018) "Reinformcement Learning, an Introduction") for a deterministic
 * 2D lattice environment.
 *
 * Note: works only for determinsitic policies as the "GetAction()" function
 * returns only a single action. It is, however, straightforward to
 * extend.
 *
 * Written 2018 by Anders Lyhne Christensen.
 **/

// Dimensions of the environment
#define COLUMNS 4
#define ROWS 3

// Environment -- spaces: agent can move, "+": reward, "-": punishment.
char environment[ROWS][COLUMNS] = { { ' ', ' ', ' ', '+' },
                                    { ' ', '#', ' ', '-' },
                                    { ' ', ' ', ' ', ' ' }};

//char environment[ROWS][COLUMNS] = { { ' ', ' ', ' ', ' ', '-', ' ' },
//                                    { ' ', '#', '#', ' ', ' ', '-' },
//                                    { ' ', '-', '#', ' ', ' ', ' ' },
//                                    { ' ', '#', '#', '#', '#', ' ' },
//                                    { ' ', ' ', ' ', ' ', '#', '+' },};

// Current estimate of state values under the current policy:
float V[ROWS][COLUMNS];

float estimate[ROWS*COLUMNS][4];

// State is given by (x, y) in the environment. Must be inside the environment to be valid
struct state
{
    int x;
    int y;
    bool is_outside_environment;
};

// A convenient definition of the terminal state
const state TERMINAL_STATE = {-1, -1, true};
// Discount rate:
float discount_rate = 0.9;

// Theta: the thredhold for determining the accuracy of the estimation
float theta = 0.01;
int plus = 0, minus = 0, actionCounter = 0;


// Actions:
enum action { UP, DOWN, LEFT, RIGHT };

// Get the next state given a current state s and an action a:
state GetNextState(state s, action a)
{
    if (environment[s.y][s.x] != ' ')
        return TERMINAL_STATE;

    switch (a)
    {
    case UP:     s.y -= 1; break;
    case DOWN:   s.y += 1; break;
    case LEFT:   s.x -= 1; break;
    case RIGHT:  s.x += 1; break;
    }

    if (s.x < 0 || s.y < 0 || s.x >= COLUMNS || s.y >= ROWS)
        return TERMINAL_STATE;

    if(environment[s.y][s.x] == '#')
    {
        return TERMINAL_STATE;
    }

    s.is_outside_environment = false;
    return s;
}

// Ger the reward given a state and an action:
float GetReward(state s, action a)
{
    state next = GetNextState(s, a);
    if (next.is_outside_environment)
    {
        return 0;
    }
    else
    {
        if (environment[next.y][next.x] == '+')
        {
            plus++;
            return 1.0;
        }


        if (environment[next.y][next.x] == '-')
        {
            minus++;
            return -1.0;
        }

        return 0;
    }
}

float getHighestValue(state s)
{
    float value = std::numeric_limits<int>::min();
    int counter = 0;
    for(int i = 0; i < 4; ++i)
    {
        if(value < estimate[s.x+s.y*COLUMNS][i])
        {
            value = estimate[s.x+s.y*COLUMNS][i];
            counter = i;
        }
    }

    return estimate[s.x+s.y*COLUMNS][counter];
}

action getBestAction(state s)
{
    float value = std::numeric_limits<int>::min();
    int counter = 0, cNew = 0;
    action a = UP;
    for(int i = 0; i < 4; ++i)
    {
        if(estimate[s.x + s.y * COLUMNS][i] == 0)
        {
            cNew++;

        }
        if(value <= estimate[s.x+s.y*COLUMNS][i])
        {
            value = estimate[s.x+s.y*COLUMNS][i];
            counter = i;
        }
    }

    if(cNew != 4)
    {
        do
        {

            switch (counter)
            {
                case UP:    a = UP; break;
                case DOWN:  a = DOWN; break;
                case LEFT:  a = LEFT; break;
                case RIGHT: a = RIGHT; break;
            }
            counter++;
            if(counter > 3)
            {
                counter = 0;
            }
        }while(GetNextState(s, a).is_outside_environment);
    }
    else
    {
        counter = 0;
        do
        {

            switch (counter)
            {
                case UP:    a = UP; break;
                case DOWN:  a = DOWN; break;
                case LEFT:  a = LEFT; break;
                case RIGHT: a = RIGHT; break;
            }
            counter++;
            if(counter > 3)
            {
                counter = 0;
            }
        }while(GetNextState(s, a).is_outside_environment);
    }
    return a;
}

// Get the next action according to the current policy:
action GetNextAction(state s)
{
    return RIGHT;
}

state qLearning(state s, double epsilon, double alpha, double gamma)
{
    double value = ((double)rand() / (RAND_MAX));
    action a;
    float reward, bestReward;
    state next;
    if( value <= epsilon){ //make a guess

        do
        {
            int random = rand() % 4;
            switch (random)
            {
                case UP:    a = UP; break;
                case DOWN:  a = DOWN; break;
                case LEFT:  a = LEFT; break;
                case RIGHT: a = RIGHT; break;
            }
            next = GetNextState(s, a);
        }
        while(next.is_outside_environment);
    }
    else
    { // pick the best action
        a = getBestAction(s);
        next = GetNextState(s, a);
    }


    reward = GetReward(s, a);
    bestReward = getHighestValue(next);

    estimate[s.x + s.y * COLUMNS][a] = estimate[s.x + s.y * COLUMNS][a]
            + alpha * (reward + gamma * bestReward - estimate[s.x + s.y * COLUMNS][a]);

    //std::cout << "Action: " << a << std::endl;
    return next;
}


bool terminate(state s)
{
    if(environment[s.y][s.x] == '+' || environment[s.y][s.x] == '-')
    {
        return false;
    }

    return true;
}

// Print the environment with border around:
void PrintEnvironment()
{
    for (int y = -1; y <= ROWS; y++)
    {
        for (int x = -1; x <= COLUMNS; x++)
            if (y < 0 || y >= ROWS || x < 0 || x >= COLUMNS)
                std::cout << "#";
            else
                std::cout << environment[y][x];

        std::cout << std::endl;
    }
}

// Print the current estimate of state values:
void PrintStateValues()
{
    for (int y = 0; y < ROWS; y++)
    {
        for (int x = 0; x < COLUMNS; x++)
            printf(" %5.2f ", V[y][x]);

        printf("\n");
    }
}

void PrintFinalValues()
{
    float value;
    int chosenAction = 0;

    for (int y = 0; y < ROWS; y++)
    {
        for (int x = 0; x < COLUMNS; x++)
        {
            value = std::numeric_limits<int>::min();
            if(environment[y][x] != ' ')
            {
                printf(" %c (%4s) ", environment[y][x], "xxxxxxxx");
            }
            else
            {
                for(int k = 0; k < 4; k++)
                {
                    if(value < estimate[x + y * COLUMNS][k])
                    {
                        value = estimate[x + y * COLUMNS][k];
                        chosenAction = k;
                    }
                }
                printf(" %d (%4f) ", chosenAction, value);
            }
        }
        printf("\n");
    }
}

void PrintStateEstimates()
{
    for (int y = 0; y < ROWS * COLUMNS; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            printf(" %5.2f ", estimate[y][x]);
        }

        printf("\n");
    }
}


int main(int argc, char** argv)
{
    srand(time(0));
    std::cout << "Environment:" << std::endl;
    PrintEnvironment();
    state startState = {0, 0};

    // Reset all state value estimates to 0:
    for (int y = 0; y < ROWS; y++)
        for (int x = 0; x < COLUMNS; x++)
            V[y][x] = 0;

    for(int i = 0; i < ROWS*COLUMNS; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            estimate[i][j] = 0;
        }
    }


    int sweep   = 0;
    float delta;
    for(int i = 0; i < 10000; i++)
    {
        int actionCounter1 = 0;

        startState = {0, 2};
        do
        {
            startState = qLearning(startState, 0.1, 1, 0.9);
            actionCounter1++;
        } while(terminate(startState)); // Check if our currect estimate is accurate enough.
        if(i < 50)
        {
            std::cout << "Actions used per episode " << i << ": " << actionCounter1 << std::endl;
        }

    }

    PrintFinalValues();
    std::cout << "+: " << plus << " : " << " -: " << minus << std::endl;

};
