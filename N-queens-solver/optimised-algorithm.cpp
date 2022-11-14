#include <iostream>
#include <vector>
#include <time.h>
using namespace std;

bool HAS_CONFLICTS;

int n;

int* queens;
int* r;
int* d1;
int* d2;

void putQueens()
{
	int col = 1;
	for (int row = 0; row < n; row++)
	{
        queens[col] = row;
        r[row]++;
        d1[col - row + n - 1]++;
        d2[col + row]++;

        col += 2;
		if (col >= n)
		{
			col = 0;
		}
    }
}

int colWithQueenWithMaxConf()
{
	int maxConf = -1;
	vector<int> colsWithMaxConf;

	int curRow, curConf;

	for (int curCol = 0; curCol < n; curCol++)
	{
		curRow = queens[curCol];
		//we are counting the current queen 3 times - these are not conflicts
		curConf = r[curRow] + d1[curCol - curRow + n - 1] + d2[curCol + curRow] - 3;
		if (curConf == maxConf)
		{
			colsWithMaxConf.push_back(curCol);
		}
		else if (curConf > maxConf)
		{
			maxConf = curConf;
			colsWithMaxConf.clear();
			colsWithMaxConf.push_back(curCol);
		}
	}

	if (maxConf == 0)
	{
		HAS_CONFLICTS = false;
	}

	int randIndex = rand() % colsWithMaxConf.size();

	return colsWithMaxConf[randIndex];
}

int rowWithMinConf(int col)
{
	int minConf = n + 1;
	vector<int> rowsWithMinConf;

	int curConf;

	for (int curRow = 0; curRow < n; curRow++)
	{
		// if there is a queen
		if (queens[col] == curRow)
		{
			curConf = r[curRow] + d1[col - curRow + n - 1] + d2[col + curRow] - 3;
		}
		//else - no queen
		else
		{
			curConf = r[curRow] + d1[col - curRow + n - 1] + d2[col + curRow];
		}

		if (curConf == minConf)
		{
			rowsWithMinConf.push_back(curRow);
		}
		else if (curConf < minConf)
		{
			minConf = curConf;
			rowsWithMinConf.clear();
			rowsWithMinConf.push_back(curRow);
		}
	}

	int randIndex = rand() % rowsWithMinConf.size();

	return rowsWithMinConf[randIndex];
}

void updateState(int row, int col)
{
	//decrease number of queens for previous position
	int prevRow = queens[col];
	r[prevRow]--;
	d1[col - prevRow + n - 1]--;
	d2[col + prevRow]--;

	//increase number of queens for new position
	queens[col] = row;
	r[row]++;
	d1[col - row + n - 1]++;
	d2[col + row]++;
}

void printQueens()
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (queens[j] == i) {
				cout << "* ";
			}
			else {
				cout << "_ ";
			}
		}
		cout << endl;
	}
}

void solve()
{
	int iter = 0;
	int k = 1;
	int col, row;

	while (iter++ <= k * n)
	{
		col = colWithQueenWithMaxConf();
		if (!HAS_CONFLICTS)
		{
			break;
		}
		row = rowWithMinConf(col);
		updateState(row, col);
	}

	if (HAS_CONFLICTS)
	{
		solve();
	}
}

int main()
{
	cin >> n;

	queens = new int[n];
	r = new int[n] { 0 };
	d1 = new int[2 * n - 1]{ 0 };
	d2 = new int[2 * n - 1]{ 0 };

    HAS_CONFLICTS = true;

	clock_t start = clock();

	putQueens();

	solve();

	clock_t end = clock();

	if (n < 50) {
		printQueens();
	}

	cout << "Time:" << (double) (end - start) << endl;

	delete[] queens;
	delete[] r;
	delete[] d1;
	delete[] d2;

	return 0;
}