#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#define N 3 //The length of the square-shaped puzzle board

enum direction{left, right, up, down}; //Direction type used in moving square parts of puzzle
int RandomRange(int lower, int upper); 
void FillPuzzleBoard(int puzzleBoard[N][N]); 
void SwapPuzzleSquares(int puzzleBoard[N][N], int x1, int y1, int x2, int y2); 
void ShufflePuzzleBoard(int puzzleBoard[N][N]); 
void ReadyPuzzleBoard(int puzzleBoard[N][N]); 
void PrintPuzzleBoard(int puzzleBoard[N][N]); 
int MoveSquare(int puzzleBoard[N][N], int x, int y, enum direction dir); 
int CheckSolved(int puzzleBoard[N][N]); 

int main()
{
    int puzzleBoard[N][N]; //Puzzle board
    ReadyPuzzleBoard(puzzleBoard); //Ready it
    PrintPuzzleBoard(puzzleBoard); //Print it to user to see
    int userRow; //User's row input to make a move
    int userColumn; //User's column input to make a move
    int userDir; //User's direction input to make a move
    while(1)
    {
        //I received input from the user
        printf("\nEnter -1 anytime to give up.\n"); //When user -1 is entered, the program is exited
        printf("Enter row: ");
        scanf("%d", &userRow);
        if(userRow == -1) break;
        printf("Enter column: ");
        scanf("%d", &userColumn);
        if(userColumn == -1) break;

        printf("Enter direction (left-->0, right-->1, up-->2, down-->3): ");
        scanf("%d", &userDir);
        if(userDir == -1) break;
        printf("\n");

        if(!MoveSquare(puzzleBoard, userColumn, userRow, userDir)) /*Movement is done with the given starting square 
		position and direction*/
            printf("Invalid move!\n\n"); //I notified the user when the movement could not be performed
        
        PrintPuzzleBoard(puzzleBoard); //I printed the new version of the puzzle board
        if(CheckSolved(puzzleBoard)) /*I checked whether the puzzle board has been solved, and if it is, I informed the 
		user and finished the program*/
        {
            printf("\n\nCongratulations! You have solved the puzzle!\n\n");
            break;
        }
    }

    return 0;
}

//Returns a random number between given numbers
int RandomRange(int lower, int upper)
{
    return rand() % (upper - lower) + lower;
}

//to fill puzzle board
void FillPuzzleBoard(int puzzleBoard[N][N])
{
    int i, j, n = 1; //Iteration variables i, j, and counter variable n
    for(i = 0; i < N; ++i)
        for(j = 0; j < N; ++j)
            puzzleBoard[i][j] = n++ % (N * N); /*to get 0 in the lower right corner, I assign the current element moduloed 
			with N * N to n, then increment n one by one.*/
}

/*to swap two squares of the puzzle board x1: First square's column number, 0 is left-most, y1: First square's row number, 
0 is upper-most, x2: Second square's column number, 0 is left-most, y2: Second square's row number, 0 is upper-most*/ 
void SwapPuzzleSquares(int puzzleBoard[N][N], int x1, int y1, int x2, int y2)
{
    int temp = puzzleBoard[y1][x1];                 
    puzzleBoard[y1][x1] = puzzleBoard[y2][x2];
    puzzleBoard[y2][x2] = temp;
}

//to shuffle squares of puzzle board
void ShufflePuzzleBoard(int puzzleBoard[N][N])
{
    int i;
    int bx = N - 1; //Blank square column position
    int by = N - 1; //Blank square row position 
    /*I randomly moved an empty square to shuffle the puzzle board. This is done 10 * N * N times to make sure that if the 
	starting position of the mixing process is proportional to the number of frames that continue to be shuffled.*/
    for(i = 0; i < 10 * N * N || CheckSolved(puzzleBoard); ++i)
    {
        //I randomly chose which position, row or column to change
        if(RandomRange(0, 2))
        {  
        	//Column
            if(bx != 0 && (RandomRange(0, 2) || bx == N - 1))
            {  
            	//Left, respect to blank square
                bx = RandomRange(0, bx);
                MoveSquare(puzzleBoard, bx, by, right);
            }
            else
            {  
            	//Right, respect to blank square                                           
                bx = RandomRange(bx + 1, N);
                MoveSquare(puzzleBoard, bx, by, left);
            }
        }
        else
        { 
        	//Row   
            if(by != 0 && (RandomRange(0, 2) || by == N - 1))
            {  
            	//Up, respect to blank square
                by = RandomRange(0, by);
                MoveSquare(puzzleBoard, bx, by, down);
            }
            else
            {    
            	//Down, respect to blank square                                   
                by = RandomRange(by + 1, N);
                MoveSquare(puzzleBoard, bx, by, up);
            }
        }
    }
}

//Clears given 2D square-shaped array and basically turns it into a puzzle board by assigning its elements accordingly
void ReadyPuzzleBoard(int puzzleBoard[N][N])
{
    FillPuzzleBoard(puzzleBoard); //to assign correct values to puzzle board's elements
    ShufflePuzzleBoard(puzzleBoard); //to shuffle puzzle board
}

//to print puzzle board
void PrintPuzzleBoard(int puzzleBoard[N][N])
{
    int i, j, k;
    for(i = 0; i < N; ++i)
    {

        //Print row seperators
        for(k = 0; k < N; ++k) printf("****************");
        printf("*\n");
        //Print square values with '*' in between
        for(j = 0; j < N; ++j)
        {
            printf("*\t");
            if(puzzleBoard[i][j]) printf("%d", puzzleBoard[i][j]);
            printf("\t");
        }
        printf("*\n");
    }
    //Print under end
    for(k = 0; k < N; ++k) printf("****************");
    printf("*\n");
}

/*Makes a move on given puzzle board with given starting square and direction. x: Starting square column number, 
y: Starting square row number, dir: Direction of the move*/ 
int MoveSquare(int puzzleBoard[N][N], int x, int y, enum direction dir)
{
    int isMoved = 0; //Is move performed successfully?
    int sx = x, sy = y, dx = 0, dy = 0; /*Variables to assign right values according to starting position and direction of the move in switch 
    condition to use in one for loop, instead of writing separate for loops in switch condition.*/

    /*Idea is that a move is basically the transportation of the blank square to the "starting square" position
    *If move direction is left then blank square is searched starting from left-most of given row, and carried to right each step 
    until the starting square position is reached.*/
    switch(dir)
    {
        case left:
            sx = 0;
            dx = 1;
            break;
        case right:
            sx = N - 1;
            dx = -1;
            break;
        case up:
            sy = 0;
            dy = 1;
            break;
        case down:
            sy = N - 1;
            dy = -1;
            break;
    }
    while(sx != x || sy != y)
    {
        if(!puzzleBoard[sy][sx])
        {    
        	//If the current frame is an empty square, I replaced it with the next frame in the given direction                                   
            SwapPuzzleSquares(puzzleBoard, sx, sy, sx + dx, sy + dy);
            isMoved = 1; //Move performed successfully                                              
        }
        //Goes to the next square in the given direction
        sx += dx;
        sy += dy;
    }
    return isMoved;
}

//to check if puzzle board is solved
int CheckSolved(int puzzleBoard[N][N])
{
    int isSolved = 1; //Is puzzle board solved?
    int i, j, n = 1; //Iteration variables i, j, and counter variable n
    /*If current square's value is not one more than the number of squares  has to be counted starting from left upper most 
	corner to reach it,  in a way as if reading a book, which is actually modulo'd. We want right lower most square to be 0.
	Then I set return value to 0 (false), and quit the loop.*/    
    for(i = 0; i < N; ++i)
    {
        for(j = 0; j < N; ++j)
        {
            if(puzzleBoard[i][j] != (n++) % (N * N))
            {
                isSolved = 0; 
                break;
            } 
        }
        if(!isSolved) break;
    }
    return isSolved;
}
