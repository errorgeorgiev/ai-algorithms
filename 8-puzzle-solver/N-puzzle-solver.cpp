#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <time.h>
#include <chrono>

#define FOUND -1

using namespace std;
using namespace std::chrono;

// store the sequence of the board; pieces[0] is the top left element and pieces[N] is the bottom right element

int size_of_board; // in case 3x3, size_of_board = 3
int numberOfTiles; // in case of 3x3, numberOfTiles = 3 * 3 - 1 = 8

vector<int> currentState;
vector<int> finalState;
vector<string> moves;

// count iversions
int getInvCount()
{
    int inv_count = 0;
    for (int i = 0; i < numberOfTiles; i++)
        for (int j = i+1; j < numberOfTiles + 1; j++)
             // Value 0 is used for empty space
             if (currentState[j] && currentState[i] &&  currentState[i] > currentState[j])
                  inv_count++;
    return inv_count;
}

// check if integer is odd
bool is_odd(int number)
{
    int result = number % 2;
    return result;
}

// find index if blank tile rowe
int find_blank_row_index()
{
    int answer;
    for(int i = 0; i < currentState.size(); i++)
    {
        if(currentState[i] == 0)
        {
            answer = i / size_of_board;
        }
    }
    return answer;
}

// check if board is solvable
bool is_solvable()
{
    // is odd
    if(is_odd(size_of_board))
    {
        if(is_odd(getInvCount()))
        {
            return false;
        }
        return true;
    }
    // is even
    else
    {
        int index_of_blank_row = find_blank_row_index();
        int num_of_inversions = getInvCount();
        if (is_odd(num_of_inversions + index_of_blank_row))
        {
            return true;
        }
        return false;
    }
}
// construct numberOfTiles
int constructNumberOfTiles()
{
    int n;
    cin >> n;
    return n;
}
// construct size_of_board
int constructSizeOfBoard()
{
    int size = sqrt(numberOfTiles + 1);
    return size;
}
// construct initialState vector
vector<int> constructInitialState() // OK
{
    vector<int> initialState;
    int n;
    for(int i = 0; i < numberOfTiles + 1; i++)
    {
        cin >> n;
        initialState.push_back(n);
    }
    return initialState;
}
// construct final state
vector<int> constructFinalState() // OK
{
    int indexOfZero;
    vector<int> finalState;
    int array[size_of_board * size_of_board];
    cin >> indexOfZero;
    if(indexOfZero == -1)
    {
        indexOfZero = numberOfTiles;
    }
    bool flag = false;
    for(int i = 0; i < numberOfTiles + 1; i++)
    {
        if(i == indexOfZero)
        {
            flag = true;
            finalState.push_back(0);
        }
        else
        {
            if(flag)
            {
                finalState.push_back(i);
            }
            else
            {
                finalState.push_back(i+1);
            }
        }
    }
    return finalState;
}

// find current index of zero element
int find_current_index_of_zero() // OK
{
    for(int i = 0; i < numberOfTiles + 1; i++)
    {
        if (currentState[i] == 0)
        {
            return i;
        }
    }
}
// MANHATTAN DISTANCE 
// calculate coordinates of vector element in matrix representation
int calculate_x_coordinates(int index) //OK
{
    return index % size_of_board;
}

int calculate_y_coordinates(int index) //OK
{
    return index / size_of_board;
}

// calculate manhattan distance for certain tile
int calculate_manhattan_distance(int begin_index, int end_index) // OK
{
    int x1 = calculate_x_coordinates(begin_index);
    int y1 = calculate_y_coordinates(begin_index);

    int x2 = calculate_x_coordinates(end_index);
    int y2 = calculate_y_coordinates(end_index);
    
    int manhattan_distance = abs(x1 - x2) + abs(y1 - y2);
    return manhattan_distance;
}
// calculate manhattan priority for certain state
int calculate_manhattan_priority() // OK
{
    int priority = 0;
    for(int i = 0; i < currentState.size(); i++)
    {
        if(finalState[i] != currentState[i])
        {
            // we do not calculate manhattan distance for 0 element
            if(currentState[i] != 0)
            {
                for(int j = 0; j < finalState.size(); j++)
                {
                    if (currentState[i] == finalState[j])
                    {
                        priority += calculate_manhattan_distance(i,j);
                    }
                }
            }
        }
    }
    return priority;
}
// check if solution is found
bool solution() // OK
{
    for(int i = 0; i < numberOfTiles + 1; i++)
    {
        if(currentState[i] != finalState[i])
        {
            return false;
        }
    }
    return true;
}
// swap function
void swap_tiles(int first, int second)
{
    swap(currentState[first],currentState[second]);
}


// create moves
bool right()
{
    int indexOfZero = find_current_index_of_zero();
    if(indexOfZero % size_of_board == 0)
    {
        return false;
    }
    swap_tiles(indexOfZero,indexOfZero - 1);
    return true;
}

bool left()
{
    int indexOfZero = find_current_index_of_zero();
    if(indexOfZero % size_of_board == size_of_board - 1)
    {
        return false;
    }
    swap_tiles(indexOfZero, indexOfZero + 1);
    return true;
}

bool down()
{
    int indexOfZero = find_current_index_of_zero();
    if(indexOfZero / size_of_board == 0)
    {
        return false;
    }
    swap_tiles(indexOfZero, indexOfZero - size_of_board);
    return true;
}

bool up()
{
    int indexOfZero = find_current_index_of_zero();
    if(indexOfZero / size_of_board == size_of_board - 1)
    {
        return false;
    }
    swap_tiles(indexOfZero, indexOfZero + size_of_board);
    return true;
}


//SOLVER f = g + h
int algo(int g, int threshold) // OK
{
    int f = g + calculate_manhattan_priority();

    // found greater f = g + h
    if (f > threshold)
    {
        return f;
    }

    // found final state
    if (solution())
    {
        cout << moves.size() - 1 << endl;
        for (int i = 1; i < moves.size(); i++) {
            cout << moves[i] << endl;
        }
        return FOUND;
    }

    int min = INT_MAX;
    int temp;

    if (moves.back() != "up" && down())
    {
        moves.push_back("down");
        temp = algo(g + 1, threshold);
        if (temp == FOUND)
        {
            return FOUND;
        }
        if (temp < min)
        {
            min = temp;
        }
        moves.pop_back();
        up();
    }
    if (moves.back() != "down" && up())
    {
        moves.push_back("up");
        temp = algo(g + 1, threshold);
        if (temp == FOUND)
        {
            return FOUND;
        }
        if (temp < min)
        {
            min = temp;
        }
        moves.pop_back();
        down();
    }
    if (moves.back() != "left" && right())
    {
        moves.push_back("right");
        temp = algo(g + 1, threshold);
        if (temp == FOUND)
        {
            return FOUND;
        }
        if (temp < min)
        {
            min = temp;
        }
        moves.pop_back();
        left();
    }
    if (moves.back() != "right" && left())
    {
        moves.push_back("left");
        temp = algo(g + 1, threshold);
        if (temp == FOUND)
        {
            return FOUND;
        }
        if (temp < min)
        {
            min = temp;
        }
        moves.pop_back();
        right();
    }
    return min;
}
// IDA*
void idastar() {
    int threshold = calculate_manhattan_priority();
    moves.push_back("foo");
    int temp;

    while (true)
    {
        auto start = high_resolution_clock::now();
        temp = algo(0, threshold);
        if (temp == FOUND)
        {
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            cout << "Time taken by function: "<< duration.count() << " microseconds" << endl;
            break;
        }
        threshold = temp;
    }
}

// MAIN FUNC
int main()
{
    numberOfTiles = constructNumberOfTiles();
    size_of_board = constructSizeOfBoard();
    finalState = constructFinalState();
    currentState = constructInitialState();

    if (is_solvable())
    {
        idastar();
    }
    else
    {
        cout << "Not solvable" << endl;
    }

    return 0;
}
