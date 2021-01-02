#include <iostream>
#include <time.h>
#include "classes.hh"

using namespace std;

PrefixMatcher prefixMatcher;
char word[100], board[10][10];
int n, m, cc, visited[10][10];

int dirx[] = {1, 1, 0, -1, -1, -1, 0, 1};
int diry[] = {0, -1, -1, -1, 0, 1, 1, 1};

Index *GNN(Index *prv_index)                        //Get Next Neighbour function
{
    int new_row = prv_index->row + diry[prv_index->dir];
    int new_col = prv_index->col + dirx[prv_index->dir];
    if (new_row > n - 1)
        new_row -= n;
    if (new_row < 0)
        new_row += n;
    if (new_col > m - 1)
        new_col -= m;
    if (new_col < 0)
        new_col += m;
    Index *new_index = new Index(new_row, new_col, 0);

    return new_index;
}

void printWord()                                    //Print the word
{
    for (int i = 0; i <= cc; i++)
    {
        cout << word[i];
    }
    cout << endl;
}

void DFS(Index *index, DictRange *range)            //Main Depth First Search
{
    char letter = board[index->row][index->col];
    word[++cc] = letter;
    visited[index->row][index->col] = 1;
    DictRange *new_range = prefixMatcher.match(range, cc, &letter);
    if (new_range == NULL)
        goto end;
    if (prefixMatcher.dict_words[new_range->min].size() == cc + 1)
        printWord();
    if (new_range->min == new_range->max)
        goto end;

    while (index->dir < 8)
    {
        Index *t_index = GNN(index);
        if (!visited[t_index->row][t_index->col])
            DFS(t_index, new_range);
        index->dir_update();
    }

end:
    visited[index->row][index->col] = 0;
    cc--;
    return;
}

int main()
{
    srand(time(0));
    n = m = 0;
    cc = -1;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            visited[i][j] = 0;
        }
    }

    if (prefixMatcher.init() == 1)
        return 1;

    cout << "Enter no. of rows (max 10)" << endl;
    cin >> n;
    cout << "Enter no. of columns (max 10)" << endl;
    cin >> m;

    if (n > 10 || n < 1 || m < 1 || m > 10)
    {
        cout << "Invalid value of n or m" << endl;
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            board[i][j] = (char)(rand() % 26 + 97);
        }
    }

    cout << "Your random letter board" << endl;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }

    while (1)
    {
        int x, y;
        cout << "Enter -1 to quit" << endl;
        cout << "Enter row no. to start finding word (1-" << n << ")" << endl;
        cin >> y;
        cout << "Enter column no. to start finding word (1-" << m << ")" << endl;
        cin >> x;

        if (y == -1 || x == -1)
        {
            return 0;
        }

        x--;
        y--;

        if (x >= 0 && x < m && y >= 0 && x < n)
        {
            DFS(new Index(y, x, 0), new DictRange(0, prefixMatcher.dict_words.size() - 1));
        } else
            cout << "Invalid value of y or x" << endl;
    }

    return 0;
}