/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // TODO
    if(d%2 == 0)
    {
        int start = d*d-1;
        int m,n;
        for(m = 0;m<d;m++)
        {
            for(n = 0;n<d;n++)
            {
                if(m == d-1 && n == d-1)
                   {
                       board[m][n] = 0;
                   }

                else
                {
                 board[m][n] = start;
                 start--;   
                }
            }
        }

        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }

    else if(d%2 != 0)
    {
        int start = d*d-1;
        int m,n;
        for(m = 0;m<d;m++)
        {
            for(n = 0;n<d;n++)
            {
                //special value for the blank space
                if(m == d-1 && n == d-1)
                   {
                       board[m][n] = 0;
                   }
                else
                {
                 board[m][n] = start;
                 start--;   
                }
            }
        }
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // TODO
    int m,n;
    for(m=0;m<d;m++)
    {
        for(n=0;n<d;n++)
        {
            //printing the blank space for the empty tile
            if(board[m][n] == 0)
                printf(" __ ");
            else
                printf(" %2i ",board[m][n]);
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // TODO
    int tile_row = -250,tile_col = -250,ctr = 0;
    int empty_row,empty_col;
    // TODO
    // checking for if the tile is valid
    int m,n;
    for(m=0;m<d;m++)
    {
        if(ctr == 1)
            break;
        for(n=0;n<d;n++)
        {
            if(board[m][n] == tile)
            {
                tile_row = m;
                tile_col = n;
                ctr = 1;
                break;
            }
        }
    }

    // if it's not then discarding it
    if(tile_col == -250 && tile_row == -250)
        return false;

    // checking of the position of the blank space
    ctr = 0;
    for(m=0;m<d;m++)
    {
        if(ctr == 1)
            break;
        for(n=0;n<d;n++)
        {
            if(board[m][n] == 0)
            {
                empty_row = m;
                empty_col = n;
                ctr = 1;
                break;
            }
        }
    }
    int temp;
    //chekcing if the blank space and the tile space are in the same row
    if(empty_row == tile_row)
    {
        // checking if the the two tiles are adjacent or not
        if(tile_col == empty_col + 1 || tile_col == empty_col -1)
        {
            temp = board[tile_row][tile_col];
            board[tile_row][tile_col] = 0;
            board[empty_row][empty_col] = temp;
            return true;
        }

        return false;
    }

    //chekcing if the blank space and the tile space are in the same column
    else if(empty_col == tile_col)
    {
        // checking if the the two tiles are adjacent or not
        if(tile_row == empty_row + 1 || tile_row == empty_row -1)
        {
            temp = board[tile_row][tile_col];
            board[tile_row][tile_col] = 0;
            board[empty_row][empty_col] = temp;
            return true;
        }

        return false;
    }

    // if not returning false
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // TODO
    int check = 0; // all the tiles in the board must be correct, hence a ctr variable
    int h,k=0;
    int ele = 1;
    for(h=0;h<d;h++)
    {
        for(k=0;k<d;k++)
        {
            if(k == d-1 && h == d-1)
            {
                if(board[h][k] == 0)
                {
                    check++;
                }
            }

            else if(board[h][k] == ele)
            {
                check++;
                ele++;
            }
        }
    }

    if(check == d*d) // all must be correct
    {
        return true;
    }

    return false;
}