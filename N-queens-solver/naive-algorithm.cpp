#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <time.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

// global vars
int N;
vector<int> queens;


int generateRandomIndex() // OK
{
    int random = rand() % N;
    return random; 
}

void createInitialBoard() // OK
{
    for(int i = 0; i < N; i++)
    {
        queens.push_back(generateRandomIndex());
    }
}

int calculate_conflicts_of_queen(int index) //OK
{
    int r = 0;
    int d1 = 0;
    int d2 = 0;

    int current_row = queens[index];
    int current_column = index;

    for(int i = 0; i < N; i++)
    {
        if(index != i)
        {
            if(queens[index] == queens[i])
            {
                r++;
            }
        }
    }
    for(int i = 0; i < N; i++)
    {
        if(index != i)
        {
            int check_row = queens[i];
            int check_column = i;
            
            if((check_row - current_row) == (check_column - current_column))
            {
                d1++;
            }
            else if((check_row + check_column) == (current_row + current_column))
            {
                d2++;
            }
        }

    }
    
    return r + d1 + d2;
}

int findQueenWithMostConflicts() // OK
{
    int max = INT_MIN;
    int currentRowOfQueen;
    int currentConflictsOfQueen;   
    vector<int> queensWithMostConflicts;

    for(int i = 0; i < N; i++)
    {
        currentRowOfQueen = queens[i];
        currentConflictsOfQueen = calculate_conflicts_of_queen(i); 
        if(currentConflictsOfQueen == max)
        {
            queensWithMostConflicts.push_back(i);
        }
        else if(currentConflictsOfQueen > max)
        {
            max = currentConflictsOfQueen;
            // remove current max conflict queens
            queensWithMostConflicts.clear();
            // add current queen
            queensWithMostConflicts.push_back(i);
        }
    }

    // if there are many queens with max conflicts, choose random one
    int random_index = rand() % queensWithMostConflicts.size();
    return queensWithMostConflicts[random_index];
}

vector<int> moveQueen(int colOfQueen, int desiredRowOfQueen) // OK
{
    vector<int> current = queens;
    current[colOfQueen] = desiredRowOfQueen;
    return current;
}

int calculate_conflict_of_empty_square(int rowOfSquare, int columnOfQueen) // OK
{
    
    vector<int> current = moveQueen(columnOfQueen, rowOfSquare);
    int index = columnOfQueen;


    int r = 0;
    int d1 = 0;
    int d2 = 0;

    int current_row = current[index];
    int current_column = index;

    for(int i = 0; i < N; i++)
    {
        if(index != i)
        {
            if(current[index] == queens[i])
            {
                r++;
            }
        }
    }
    for(int i = 0; i < N; i++)
    {
        if(index != i)
        {
            int check_row = queens[i];
            int check_column = i;
            
            if((check_row - current_row) == (check_column - current_column))
            {
                d1++;
            }
            else if((check_row + check_column) == (current_row + current_column))
            {
                d2++;
            }
        }

    }
    return r + d1 + d2;
}


// passing a queen index, find the best row to put the queen
int findRowWithMinConflicts(int columnOfQueen) // OK
{
    int min = INT_MAX; 
    int currentConflictsOfRow;  
    vector<int> rowsWithMinConflicts;

    for(int i = 0; i < N; i++)
    {
        if(queens[columnOfQueen] != i)
        {
            currentConflictsOfRow = calculate_conflict_of_empty_square(i, columnOfQueen);
            //cout << "Current conflict if change to row " << i << " is: "<< currentConflictsOfRow << endl; // OK
        }
        else
        {
            currentConflictsOfRow = INT_MAX;
        }
        // check if current < min
        if(currentConflictsOfRow == min)
        {
            rowsWithMinConflicts.push_back(i);
        }
        else if(currentConflictsOfRow < min)
        {
            min = currentConflictsOfRow;
            rowsWithMinConflicts.clear();
            rowsWithMinConflicts.push_back(i);
        }
        
    }

    int random_index = rand() % rowsWithMinConflicts.size();
    return rowsWithMinConflicts[random_index];
}

void updateBoard(int columnOfQueen, int rowWithLeastConflicts) // OK
{
    queens[columnOfQueen] = rowWithLeastConflicts;
}

bool checkIfBoardHasConflicts() // OK
{
    if (calculate_conflicts_of_queen(findQueenWithMostConflicts()) == 0)
    {
        return false;
    }
    return true;
}

void solve() // OK
{
    int indexOfQueenWithMostConflicts = findQueenWithMostConflicts();
    int indexOfRowWithLeastConflicts = findRowWithMinConflicts(indexOfQueenWithMostConflicts);
    updateBoard(indexOfQueenWithMostConflicts, indexOfRowWithLeastConflicts);

    if (checkIfBoardHasConflicts())
    {
        solve();
    }
}

void printSolution() // OK
{
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            if(queens[j] == i)
            {
                cout << "*" << " ";
            }
            else 
            {
                cout << "_" << " ";
            }
        }
        cout << endl;
    }
}



int main()
{
    srand(time(NULL));
    cin >> N;
    createInitialBoard();
    auto start = high_resolution_clock::now();    
    solve();
    auto stop = high_resolution_clock::now();
    printSolution();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function: "<< duration.count() << " microseconds" << endl;
    

    return 0;
}
