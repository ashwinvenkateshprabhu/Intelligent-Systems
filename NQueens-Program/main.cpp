//
//  main.cpp
//  NQueens-Program
//
//  Created by Ashwin V Prabhu on 10/8/16.
//  Copyright © 2016 Ashwin Venkatesh Prabhu. All rights reserved.
//  Built using: Mac OSX Version 10.11.6, XCode Version 7.3.1 (C++11)
//  C++ language dialecct: GNU++11
//  C++ standard library: libc++ (LLVM standard library with C++11 support)

/**
 Objective: Solve the N-Queens problem using Hill Climbing random restart algorithm
 Heuristic used: number of conflicts between the queens
 Input: User need to enter the value of N which is the number of queens to be present on the solution board without any conflicts
 Output: The initial state, the solution, number of steps to reach the solution, number of restarts made
 **/

#include <iostream>
#include <vector>
#include <time.h>

using namespace std;

vector<int> state(10);
int N;
int noOfSteps = 0;
int restart = 0;

vector<int> CreateState();
void PrintState(vector<int> );
int GetNumberOfConflicts(vector<int> );
void HillClimbingUsingRandomRestart();

// Generates a random states with N queens and returns the same
vector<int> CreateState()
{
    vector<int> state(N);
    for (int i = 0; i < N; i++)
    {
        state[i] = rand() % N;
    }
    
    return state;
}

// Prints the state
void PrintState(vector<int> s)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (s[i] == j)
            {
                cout << "Q|";
            }
            else
            {
                cout << "_|";
            }
        }
        cout << endl;
    }
    cout << endl;
}

// Calculates the number of conflicts for each state.
// The number of conflicts will be 0 for the solution
int GetNumberOfConflicts(vector<int> t)
{
    int conflicts = 0;
    
    for (int q = 0; q < N; q++)
    {
        for (int nq = q + 1; nq < N; nq++)
        {
            if ((t[q] == t[nq]) || (abs(q-nq) == abs(t[q]-t[nq])))
            {
                conflicts++;
            }
        }
    }
    
    return conflicts;
}

// Implements the hill climbing restart algorithm
void HillClimbingUsingRandomRestart()
{
    int conflicts = GetNumberOfConflicts(state);
    vector<int> newState(N);
    if (conflicts == 0)
    {
        cout << "Solution: " << endl;
        PrintState(state);
        cout << "Number of steps: " << noOfSteps << endl;
        cout << "Number of restarts: " << restart << endl;
        exit(0);
    }
    else
    {
        int c1 = conflicts;
        for (int c = 0; c < N; c++)
        {
            int y = state[c];
            for (int r = 0; r < N; r++)
            {
                if (r != state[c])
                {
                    state[c] = r;
                    int c2 = GetNumberOfConflicts(state);
                    
                    if (c2 < c1)
                    {
                        for (size_t i = 0; i < N; i++)
                        {
                            newState[i] = state[i];
                        }
                        c1 = c2;
                    }
                }
            }
            state[c] = y;
        }
        
        if (c1 < conflicts)
        {
            for (int i = 0; i < N; i++)
            {
                state = newState;
                noOfSteps++;
                HillClimbingUsingRandomRestart();
            }
        }
        else
        {
            state = CreateState();
            restart++;
            noOfSteps++;
            HillClimbingUsingRandomRestart();
        }
    }
}

int main(int argc, const char * argv[])
{
    srand((unsigned int)time(NULL));
    cout << "Enter the number of queens: ";
    cin >> N;
    state = CreateState();
    
    cout << "Start state: " << endl;
    PrintState(state);
    HillClimbingUsingRandomRestart();
    return 0;
}
