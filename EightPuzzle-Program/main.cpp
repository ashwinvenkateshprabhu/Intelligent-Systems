//
//  main.cpp
//  Astar8puzzle
//
//  Created by Ashwin Venkatesh Prabhu on 9/15/16.
//  Copyright © 2016 Ashwin Venkatesh Prabhu. All rights reserved.
//  Built using: Mac OSX Version 10.11.6, XCode Version 7.3.1 (C++11)
//  C++ language dialecct: GNU++11
//  C++ standard library: libc++ (LLVM standard library with C++11 support)

/**
    Objective: Solve the 8 puzzle problem using A* algorithm
    Heuristic used: Manhattan distance
    Input: User need to enter initial state and goal state when prompted for
    Output: Number of moves made to reach the goal, Number of nodes generated in the process, display the moves along with the operators (swap left, swap right, swap up, swap down)
**/

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <queue>

using namespace std;

// Declarations
typedef vector< vector<int> > NODE;

// reachedStates will store all the states which are reached
map<string, int> reachedStates;

// noOfStatesGenerated is a counter to count the overall states generated
long noOfStatesGenerated = 0;

class STATE
{
public:
    NODE node; // Holds value for a 2D vector integer
    int gn; // g(n) value
    int hn; // h(n) value
    int fn; // f(n) value
    string direction; // the direction in which the tile position was swapped to reach the current state
    
    STATE* parent; // link to the parent state
    
    STATE();
    ~STATE();
    
    void ManhattanDistance(); // Calculates the manhattan distance for the current state
};

// initState will store the start state
STATE *initState;

// goalState will store the expected goal state
STATE *goalState;

// result will store all the intermediate nodes b/w start node and goal node
deque<STATE *> result;

void PrintNode(STATE *);
bool IsGoalNode(STATE *);
deque<STATE *> GenerateSuccessorStates(STATE *);
STATE * CreateState(STATE *);
int Solve8Puzzle();
void GetInput();
STATE ConvertInput();
string ConvertToString(STATE *);
void PrintResult();
string GetSwapDirection(int, int);

// Declarations


// Definitions

// Compare function used in priority queue
auto compare = [] (STATE *node1, STATE *node2) ->
bool
{
    return (node1->fn > node2->fn);
};

// STATE class constructor
STATE::STATE()
{
    node = NODE(3, vector<int>(3,0));
    parent = NULL;
    fn = gn = hn = 0;
    direction = "";
}

// STATE class destructor
STATE::~STATE()
{
    delete parent;
}

// ManhattanDistance() calculates the sum of distance current tile positions from the destination position
void STATE::ManhattanDistance()
{
    int manhattanDistance = 0;
    
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (node[i][j] != 0)
            {
                for (int x = 0; x < 3; x++)
                {
                    for (int y = 0; y < 3; y++)
                    {
                        if (node[i][j] == goalState->node[x][y])
                        {
                            manhattanDistance += abs(i - x) + abs(j - y);
                        }
                    }
                }
            }
        }
    }
    
    hn = manhattanDistance;
}

// PrintNode() prints the node of the state which is passed as a parameter
void PrintNode(STATE *S)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << S->node[i][j] << " ";
        }
        cout << endl;
    }
}

// IsGoalNode() returns a boolean value if the goal node is reached
bool IsGoalNode(STATE *S)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (S->node[i][j] != goalState->node[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

// GetSwapDirection() determines which direction the tiles got swapped for the node generated
string GetSwapDirection(int x, int y)
{
    if (x == 1 && y == 0)
    {
        return "swap down";
    }
    else if (x == -1 && y == 0)
    {
        return "swap up";
    }
    else if (x == 0 && y == -1)
    {
        return "swap left";
    }
    else if (x == 0 && y == 1)
    {
        return "swap right";
    }
    return "";
}

// GenerateSuccessorStates() generates all the successor nodes to the node which is passed as a parameter.
// It returns a double-ended queue which holds all the successor nodes
deque<STATE *> GenerateSuccessorStates(STATE *S)
{
    int coordinate_x = 0, coordinate_y = 0, maximumSuccessorsPossible = 4;
    int shift[] = {1, -1, 0, 0};
    
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (S->node[i][j] == 0)
            {
                coordinate_x = i;
                coordinate_y = j;
            }
        }
    }
    
    deque<STATE *> successorStates;
    STATE *temp = new STATE;
    
    for (int x = 0; x < maximumSuccessorsPossible; x++)
    {
        temp = CreateState(S);
        if ( (coordinate_x + shift[x]) >= 0 &&
            (coordinate_x + shift[x]) <= 2 &&
            (coordinate_y + shift[maximumSuccessorsPossible - x - 1]) >= 0 &&
            (coordinate_y + shift[maximumSuccessorsPossible - x - 1]) <= 2 )
        {
            swap(temp->node[coordinate_x][coordinate_y],
                 temp->node[coordinate_x + shift[x]][coordinate_y + shift[maximumSuccessorsPossible - x - 1]]);
            temp->direction = GetSwapDirection(shift[x], shift[maximumSuccessorsPossible-x-1]);
            successorStates.push_back(temp);
        }
    }
    return successorStates;
}

// PrintResult() prints the end result which is No of moves made, No of nodes generated,
// and moves made to reach the solution
void PrintResult()
{
    STATE* temp;
    cout << "Number of nodes generated: " << noOfStatesGenerated << endl;
    cout << "Moves made to reach the solution are given below:" << endl;
    size_t size = result.size();
    for (int i=0; i<size; i++)
    {
        temp = result.back();
        cout << temp->direction << endl;
        PrintNode(temp);
        cout << endl;
        result.pop_back();
    }
}

// Solve8Puzzle() implements the A-star algorithm to solve the 8 puzzle problem
int Solve8Puzzle()
{
    STATE *S = initState;
    int numberOfMoves = 0;
    string str;
    priority_queue<STATE *, vector<STATE *>, decltype(compare)> p_queue(compare);
    p_queue.push(S);
    noOfStatesGenerated++;
    deque<STATE *> successors;
    
    while (!p_queue.empty())
    {
        STATE *bestNode = p_queue.top();
        p_queue.pop();
        str = ConvertToString(bestNode);
        reachedStates[str] = 1;
        
        if (IsGoalNode(bestNode))
        {
            while (bestNode != NULL)
            {
                result.push_back(bestNode);
                bestNode = bestNode->parent;
                numberOfMoves++;
            }
            
            return numberOfMoves - 1;
        }
        
        else
        {
            successors = GenerateSuccessorStates(bestNode);
            
            while (!successors.empty())
            {
                STATE *successor = CreateState(successors.front());
                successor->parent = bestNode;
                
                successor->ManhattanDistance();
                successor->gn = bestNode->gn + 1;
                successor->fn = successor->gn + successor->hn;
                string st = ConvertToString(successor);
                
                if (!reachedStates[st])
                {
                    p_queue.push(successor);
                    noOfStatesGenerated++;
                }
                
                successors.pop_front();
            }
        }
    }
    
    return numberOfMoves;
}

// ConvertToString() is used to convert the nodes in a state to a string value which will be stored in
// the reachedState variable. Each state is unique and hence each string in the reachedState variable
// will be unique.
string ConvertToString(STATE *S)
{
    string str = "";
    int tmp;
    
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            tmp = S->node[i][j];
            str += to_string(tmp);
        }
    }
    
    return str;
}

// CreateState() is used to create a new state
STATE* CreateState(STATE *tmp)
{
    STATE *newState = new STATE;
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            newState->node[i][j] = tmp->node[i][j];
        }
    }
    newState->fn = tmp->fn;
    newState->gn = tmp->gn;
    newState->hn = tmp->hn;
    newState->direction = tmp->direction;
    return newState;
}

// ConvertInput() is used to convert the user input into a NODE of a STATE. This method will return a STATE
STATE* ConvertInput(vector<int> elements)
{
    STATE* S = new STATE;
    int pos = 0;
    
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            S->node[i][j] = elements[pos++];
        }
    }
    elements.clear();
    return S;
}

// GetInput() is used to take users input for initial node and goal node
void GetInput()
{
    int number, count = 0;
    vector<int> elements;
    
    cout << "Enter initial state: " << endl;
    while (count++ < 9 && cin >> number)
    {
        elements.push_back(number);
    }
    
    initState = ConvertInput(elements);
    elements.clear();
    count = 0;
    
    cout << "Enter goal state: " << endl;
    while (count++ < 9 && cin >> number)
    {
        elements.push_back(number);
    }
    
    goalState = ConvertInput(elements);
    elements.clear();
    
    initState->ManhattanDistance();
    initState->fn = initState->gn + initState->hn;
}

// Definitions

int main(int argc, const char * argv[])
{
    
    GetInput();
    cout << "Number of moves made: " << Solve8Puzzle() << endl;
    PrintResult();
    return 0;
}
