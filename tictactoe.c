#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define boardsize 3
void foreword();   // show the foreword and default the player's pieces
void showstate();  // display the current state
void showboard();  // display the current board
void coordinate(); // create a correct coordinate(row*column)
int judge();       // check if someone won, someone won return 1, or 0
int endproject();  // depends on returned win's value and return 0 to replay
int main()
{
    HWND hwnd = GetConsoleWindow(); // get the handle
    ShowWindow(hwnd, SW_MAXIMIZE);  // maximum the window
    int board[boardsize][boardsize] = {0};
    char playerpiece[2];      //'O', 'X'
    char remainedpiece[4][3]; //'O'=> -4,'o'=> -1, 'X'=> 4, 'x'=> 1
    int roundcounter = 0;
    int win = 0; // will be assigned by judge(), 0 to nobody won, 1 means someone won
    int key = 0; // will be assigned by endproject(), 0 to replay
    while (key == 0)
    {
        foreword(playerpiece, remainedpiece, roundcounter);
        showstate(playerpiece, remainedpiece, roundcounter);
        showboard(board);
        coordinate(board, playerpiece, remainedpiece, roundcounter);
        win = judge(board);
        key = endproject(board, remainedpiece, &roundcounter, win);
    }
}
void foreword(char(*mplayerpiece), char (*mremainedpiece)[3], int mroundcounter) // show the foreword and default the player's pieces
{
    if (mroundcounter < 1)
    {
        printf("--------------------------\n> FOREWORD <\n--------------------------\n");
        printf("Hello players, this is tic tac toe made by \"B11207026\"\n");
        printf("BUT THIS IS NOT THE SAME AS THE OTHERS!!\n");
        printf("The new rules are as follows:\n");
        printf("Each player has 3 big pieces and 3 small pieces\n");
        printf("Players's first choose must be smaller one\n");
        printf("The bigger one can cover the smaller one but the same size\n");
        printf("--------------------------------------------------------------------\n");
        while (1) // default the players's piece into array
        {
            printf("Player1, please choose your chess piece(O/o or X/x) :");
            scanf(" %c", &mplayerpiece[0]);
            if (getchar() != '\n')
            {
                while (getchar() != '\n') // clear input buffer
                {
                }
                continue;
            }
            if (mplayerpiece[0] == 'O' || mplayerpiece[0] == 'o')
            {
                mplayerpiece[0] = 'O';
                mplayerpiece[1] = 'X';
                break;
            }
            else if (mplayerpiece[0] == 'X' || mplayerpiece[0] == 'x')
            {
                mplayerpiece[0] = 'X';
                mplayerpiece[1] = 'O';
                break;
            }
            else
            {
                while (getchar() != '\n') // clear input buffer
                {
                }
                continue;
            }
        }
        char pieceO[12] = {'O', 'O', 'O', 'o', 'o', 'o', 'X', 'X', 'X', 'x', 'x', 'x'};
        char pieceX[12] = {'X', 'X', 'X', 'x', 'x', 'x', 'O', 'O', 'O', 'o', 'o', 'o'};
        int index = 0;
        int i, j;
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 3; j++)
            {
                if (mplayerpiece[0] == 'O') // default pieces after Player1 chose O
                {
                    mremainedpiece[i][j] = pieceO[index];
                    index++;
                }
                else // default pieces after Player1 chose x
                {
                    mremainedpiece[i][j] = pieceX[index];
                    index++;
                }
            }
        }
        printf("---------------\n> GAME START <\n---------------\n\n");
    }
}
void showstate(char(*mplayerpiece), char (*mremainedpiece)[3], int mroundcounter) // display the current state
{
    int i, j, index = 0, pieceindex = 0;
    while (pieceindex < 2)
    {
        printf("Player%d is %2c, remained pieces :\n", pieceindex + 1, mplayerpiece[pieceindex]);
        pieceindex++;
        for (i = index; i < 4; i++)
        {
            for (j = 0; j < 3; j++)
            {
                if ((i == 0 && j == 0) || (i == 2 && j == 0))
                {
                    printf("[ ");
                }
                printf("%3c ", mremainedpiece[i][j]);
                if (j != 2 || i == 0 || i == 2)
                {
                    printf(", ");
                }
                if ((i == 1 && j == 2) || (i == 3 && j == 2))
                {
                    printf(" ]");
                }
            }
            if (i == 1)
            {
                index = 2;
                printf("\n");
                break;
            }
        }
    }
    printf("\n");
    printf("Current round is %2d\n", mroundcounter + 1);
    printf("It's %s turn\n", (mroundcounter % 2 == 0) ? "Player1" : "Player2");
    printf("-----------------------\n\n");
}
void showboard(int (*mboard)[boardsize]) // display the current board
{
    int i, j;
    int index;
    printf("%5c", ' ');
    for (index = 1; index <= boardsize; index++) // +index print
    {
        printf("%5d", index);
    }
    printf("    COLUMN v\n\n");
    for (i = 0, index = -1; i < boardsize; i++, index--) // i(column) print & -index print & show the board
    {
        printf("%5d", index);
        for (j = 0; j < boardsize; j++) // j(row) print
        {
            if (mboard[i][j] == -4)
            {
                printf("%5c", 'O');
            }
            else if (mboard[i][j] == -1)
            {
                printf("%5c", 'o');
            }
            else if (mboard[i][j] == 4)
            {
                printf("%5c", 'X');
            }
            else if (mboard[i][j] == 1)
            {
                printf("%5c", 'x');
            }
            else
            {
                printf("%5c", '.');
            }
        }
        printf("\n\n");
    }
    printf("  ROW >\n\n");
}
void coordinate(int (*mboard)[boardsize], char(*mplayerpiece), char (*mremainedpiece)[3], int mroundcounter)
// create a correct coordinate(row*column)
{
    int gamerow = 0, gamecolumn = 0, pcheck = 0, value = 0;
    char checker, psize;
regret:
    while (1) // choose the size of piece correctly
    {
        if (mroundcounter < 2) // player's first choose
        {
            while (1)
            {
                printf("it's Player%d's first choose, please choose smaller piece :", mroundcounter + 1);
                scanf(" %c", &psize);
                if (psize == 'o' || psize == 'x')
                {
                    if (getchar() != '\n')
                    {
                        while (getchar() != '\n') // clear input buffer
                        {
                        }
                        continue;
                    }
                    break;
                }
                else
                {
                    while (getchar() != '\n') // clear input buffer
                    {
                    }
                    continue;
                }
            }
        }
        else // choose size after first choose
        {
            printf("Player%d please choose a size of your piece to set :", mroundcounter % 2 + 1);
            scanf(" %c", &psize);
            if (getchar() != '\n')
            {
                while (getchar() != '\n') // clear input buffer
                {
                }
                continue;
            }
        }
        pcheck = (mplayerpiece[mroundcounter % 2] == 'O') ? -1 : 1;
        if (((psize == 'O' || psize == 'o') && pcheck == -1) || ((psize == 'X' || psize == 'x') && pcheck == 1))
        // make sure player chose the correct kind of size
        {
            int i, j, index = 0;
            for (i = 0; i < 4; i++) // check the remained pieces
            {
                for (j = 0; j < 3; j++)
                {
                    if (mremainedpiece[i][j] != psize)
                    {
                        index++;
                    }
                }
            }
            if (index == 12)
            {
                printf("there are no more size of piece you chose !!\n");
            }
            else
            {
                break;
            }
        }
        else
        {
            continue;
        }
    }
    if (psize == 'O')
    {
        value = -4;
    }
    else if (psize == 'o')
    {
        value = -1;
    }
    else if (psize == 'X')
    {
        value = 4;
    }
    else
    {
        value = 1;
    }
    while (1) // choose a available coordinate
    {
        printf("please choose a available coordinate(row*column) to set your piece :");
        scanf(" %d*%d", &gamerow, &gamecolumn);
        if (gamerow < 1 || gamerow > boardsize || gamecolumn > -1 || gamecolumn < -boardsize)
        {
            printf("invalid row or column !!\n");
            while (getchar() != '\n') // clear input buffer
            {
            }
            continue;
        }
        else if (mboard[-(gamecolumn + 1)][gamerow - 1] == 0 || abs(value) > abs(mboard[-(gamecolumn + 1)][gamerow - 1]))
        // the piece can put on the 0 or smaller one
        {
            if (getchar() != '\n')
            {
                while (getchar() != '\n') // clear input buffer
                {
                }
                continue;
            }
            break;
        }
    }
    printf("set size \"%c\" on (row: %2d, column: %2d)\n", psize, gamerow, gamecolumn);
    while (1) // check the coordinate and size was chosen correctly
    {
        printf("input \"C/c\" to confirm the coordinate(row*column) you chose or \"R/r\" to choose again :");
        scanf(" %c", &checker);
        if (checker == 'C' || checker == 'c')
        {
            if (getchar() != '\n')
            {
                while (getchar() != '\n') // clear input buffer
                {
                }
                continue;
            }
            break;
        }
        else if (checker == 'R' || checker == 'r') // regret system
        {
            if (getchar() != '\n')
            {
                while (getchar() != '\n') // clear input buffer
                {
                }
                continue;
            }
            goto regret;
        }
        else
        {
            while (getchar() != '\n') // clear input buffer
            {
            }
            continue;
        }
    }
    int i = 0, j = 0;
    for (i = 0; i < 4; i++) // decrease the remainedpiece
    {
        for (j = 0; j < 3; j++)
        {
            if (mremainedpiece[i][j] == psize)
            {
                mremainedpiece[i][j] = ' ';
                break;
            }
        }
    }
    for (i = 0; i < boardsize; i++) // change the board
    {
        for (j = 0; j < boardsize; j++)
        {
            if (j + 1 == gamerow && i + 1 == -gamecolumn)
            {
                mboard[i][j] = value;
            }
        }
    }
    system("cls");
}
int judge(int (*mboard)[boardsize]) // check if someone won, someone won return 1, or 0
{
    int i, j, mwin = 0;
    for (i = 0; i < boardsize; i++) // check "--"
    {
        if (mboard[i][0] != 0 && mboard[i][1] != 0 && mboard[i][2] != 0)
        {
            mwin = abs(mboard[i][0] + mboard[i][1] + mboard[i][2]);
        }
        if (mwin == 3 || mwin == 9 || mwin == 12)
        {
            return 1;
        }
    }
    for (j = 0; j < boardsize; j++) // check "|"
    {
        if (mboard[0][j] != 0 && mboard[1][j] != 0 && mboard[2][j] != 0)
        {
            mwin = abs(mboard[0][j] + mboard[1][j] + mboard[2][j]);
        }
        if (mwin == 3 || mwin == 9 || mwin == 12)
        {
            return 1;
        }
    }
    if (mboard[0][0] != 0 && mboard[1][1] != 0 && mboard[2][2] != 0) // check "\"
    {
        mwin = abs(mboard[0][0] + mboard[1][1] + mboard[2][2]);
    }
    if (mwin == 3 || mwin == 9 || mwin == 12)
    {
        return 1;
    }
    if (mboard[0][2] != 0 && mboard[1][1] != 0 && mboard[2][0] != 0) // check "/"
    {
        mwin = abs(mboard[0][2] + mboard[1][1] + mboard[2][0]);
    }
    if (mwin == 3 || mwin == 9 || mwin == 12)
    {
        return 1;
    }
    return 0;
}
int endproject(int (*mboard)[boardsize], char (*mremainedpiece)[3], int *mroundcounter, int mwin)
// depends on returned win's value and return 0 to replay
{
    char checker;
    int tie = 0;
    if (mwin == 1)
    {
        printf("---------------\n> GAME OVER <\n---------------\n\n");
        printf("Total round: %d\n", (*mroundcounter) + 1);
        printf(">**  The winner is %s  **<\n", (*mroundcounter % 2 == 0) ? "Player 1" : "Player 2");
    }
    else if (mwin == 0)
    {
        int i, j;
        int count = 0;
        int bigct = 6, smallct = 6;
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 3; j++)
            {
                if (mremainedpiece[i][j] == ' ')
                {
                    count++;
                }
                else if (mremainedpiece[i][j] == 'O' || mremainedpiece[i][j] == 'X')
                {
                    bigct--;
                }
                else
                {
                    smallct--;
                }
            }
        }
        if (count == 12 || (bigct == 6 && smallct >= 3))
        {
            tie = 1;
            printf("!! Nobody wins !!\n");
        }
    }
    while (mwin == 1 || tie == 1)
    {
        printf("\n");
        showboard(mboard);
        printf("input \"Q/q\" to close the project or \"R/r\" to play again :");
        scanf(" %c", &checker);
        if (checker == 'Q' || checker == 'q') // quit
        {
            if (getchar() != '\n')
            {
                while (getchar() != '\n') // clear input buffer
                {
                }
                continue;
            }
            printf("---THANKS FOR PLAYING---\n");
            return 1;
        }
        else if (checker == 'R' || checker == 'r') // replay
        {
            if (getchar() != '\n')
            {
                while (getchar() != '\n') // clear input buffer
                {
                }
                continue;
            }
            int i, j;
            for (i = 0; i < boardsize; i++)
            {
                for (j = 0; j < boardsize; j++)
                {
                    mboard[i][j] = 0;
                }
            }
            *mroundcounter = 0;
            system("cls");
            return 0;
        }
        else
        {
            while (getchar() != '\n') // clear input buffer
            {
            }
            continue;
        }
    }
    *mroundcounter = *mroundcounter + 1;
    return 0;
}