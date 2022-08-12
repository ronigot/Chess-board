/*************
* Roni Gotlib
* 322805029
* 01
* ass4
*************/
#include<ctype.h>
#include<assert.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#define SIZE 8

// Move logical representation
typedef struct 
{
	char srcPiece, srcRow, srcCol, destPiece, destRow, destCol;
	int iSrc, jSrc, iDest, jDest;
	int isWhite, isCapture, isPromotion, isCheck, isMate;
} Move;

/**************************
 *Function Name: toDigit
 *Input: char
 *Output: int number
 *Function Operation: Auxiliary function that convert char to digit
 **************************/
int toDigit(char c) 
{
	assert('0' <= c && c <= '9');
	return (c - '0');
}

/******************
*Function Name: max
*Input: int a, int b - integer numbers
*Output: integer nember
*Function Operation: The function checks what is the larger
number of the two and returns its value
*******************/
int max(int a, int b)
{
	if (a > b)
		return a;
	return b;
}

/*******************
*Function Name: min
*Input: int a, int b - integer numbers
*Output: integer nember
*Function Operation: The function checks what is the smaller
number of the two and returns its value
********************/
int min(int a, int b)
{
	if (a < b)
		return a;
	return b;
}

/*******************
*Function Name: createBoard
*Input: char board[][SIZE], char fen[]
*Output: -
*Function Operation: Saves the fen string data in a 2D array
********************/
void createBoard(char board[][SIZE], char fen[]) 
{
	int i = 0, j = 0, spaces = 0;
	// Running on the fen string in the loop
	for (int count = 0; count < strlen(fen); count++)
	{
		// Test for letter
		if (isalpha(fen[count]))
		{
			board[i][j++] = fen[count];
		}
		// Test for digit
		else if (isdigit(fen[count]))
		{
			// Placing spaces by the given digit
			for (spaces = toDigit(fen[count]); spaces > 0; spaces--, j++)
			{
				board[i][j] = ' ';
			}
		}
		// Go to the next row
		if (j >= SIZE)
		{
			j = 0;
			i++;
		}
		
	}
}

/****************
*Function Name: printRow1
*Input: -
*Output:-
*Function Operation: Auxiliary function that
 prints the first and last rowes in the board
*****************/
void printRow1()
{
	printf("* |");
	// Prints the letters representing the columns
	for (char i = 'A'; i < SIZE + 'A'; i++)
	{
		if (i == (SIZE + 'A'-1))
		{
			printf("%c",i);
		}
		else
			printf("%c ", i);
	}
	printf("| *\n");

}

/****************
*Function Name: printRow2
*Input: -
*Output: -
*Function Operation: Auxiliary function that prints the two frame rows
*****************/
void printRow2()
{
	printf("* -");
	for (int i = 0; i < SIZE; i++)
	{
		printf("--");
	}
	printf(" *\n");
}

/****************
*Function Name: printBoard
*Input: char board[][SIZE]
*Output: -
*Function Operation: Prints a chessboard for the fen string data
*****************/
void printBoard(char board[][SIZE])
{
	//Using the auxiliary functions for the first two rows
	printRow1();
	printRow2();
	int i, j;
	//Printing the tools and dividing lines in a loop
	for ( i = 0; i < SIZE; i++)
	{
		printf("%d |", (SIZE-i));
		for ( j = 0; j < SIZE; j++)
		{
			printf("%c|", board[i][j]);
		}
		printf(" %d\n", (SIZE-i));
	}
	//Using the auxiliary functions for the last two rows
	printRow2();
	printRow1();
}

/*****************
*Function Name: checkPgn
*Input: char pgn[], int isWhiteTurn, Move* moveI -Pointer to type struct Move
*Output: -
*Function Operation: A function that saves the pgn data in a Move struct
******************/
void checkPgn(char pgn[], int isWhiteTurn, Move* moveI)
{
	moveI->iSrc = 10;
	moveI->jSrc = 10;
	moveI->isWhite = isWhiteTurn;
	int i = 0;
	// Running on the pgn string in the loop
	while (pgn[i] != '\0')
	{
		// Check for the type of piece
		if (pgn[i] <= 'R' && pgn[i] >= 'B')
		{
			if (moveI->isPromotion != 1)
			{
				if (moveI->isWhite == 1)
					moveI->srcPiece = pgn[i];
				// Convert to a small letter for the black player
				else
					moveI->srcPiece = (pgn[i] - 'A' + 'a');
			}
		}
		// Check destination and source columns (if available) and convert to an integer
		if (pgn[i] >= 'a' && pgn[i] <= 'i')
		{
			if (moveI->destCol == 0)
			{
				moveI->destCol = pgn[i];
				moveI->jDest = (toDigit(pgn[i] - '0')-1);
			}
			else
			{
				moveI->srcCol = moveI->destCol;
				moveI->jSrc = moveI->jDest;
				moveI->destCol = pgn[i];
				moveI->jDest = (toDigit(pgn[i] - '0')-1);
			}
		}
		// Check destination and source rows (if available) and convert to an integer
		if (pgn[i] > '0' && pgn[i] <= '9')
		{
			if (moveI->destRow == 0)
			{
				moveI->destRow = pgn[i];
				moveI->iDest = (SIZE - toDigit(moveI->destRow));
			}
			else
			{
				moveI->srcRow = moveI->destRow;
				moveI->iSrc = moveI->iDest;
				moveI->destRow = pgn[i];
				moveI->iDest = (SIZE - toDigit(moveI->destRow));
			}
		}
		// Check for capture
		if (pgn[i] == 'x')
		{
			moveI->isCapture = 1;
		}
		// Check for promotion and saving the piece type after the promotion (if there is)
		if (pgn[i] == '=')
		{
			moveI->isPromotion = 1;
			if (moveI->isWhite==1)
				moveI->destPiece = pgn[++i];
			// Convert to a small letter for the black player
			else
				moveI->destPiece = (pgn[++i]-'A'+'a');
		}
		// Check for 'check'
		if (pgn[i] == '+')
		{
			moveI->isCheck = 1;
		}
		// Check formate
		if (pgn[i] == '#')
		{
			moveI->isMate = 1;
		}
		i++;
	}
	// Placing p if no piece is given
	if (moveI->srcPiece == 0)
	{
		if (moveI->isWhite == 1)
			moveI->srcPiece = 'P';
		else
			moveI->srcPiece = 'p';
	}
	// Placing the source piece as a destination if there was no promotion
	if (moveI->destPiece == 0)
	{
		moveI->destPiece = moveI->srcPiece;
	}
}

/*****************
*Function Name: isDestEmpty
*Input: char pgn[], int isWhiteTurn - boolean value
*Output: int - boolean value
*Function Operation: A function that checks whether the destition is empty
or occupied by an opponent's piece during a capture
******************/
int isDestEmpty(int iDest, int jDest, int isCapture, int isWhite, char board[][SIZE])
{
	// Check if the destination is empty
	if (board[iDest][jDest] == ' ')
	{
		if (isCapture == 0)
			return 1;
		return 0;
	}
	if (isWhite == 1)
	{
		// A situation where a white piece "capture" a white piece
		if (isupper(board[iDest][jDest]))
		{
			return 0;
		}
		// Check for capture
		if (isCapture == 1)
			return 1;
		return 0;
	}
	// Black turn
	//A situation where a black piece "capture" a black piece
	if (islower(board[iDest][jDest]))
	{
		return 0;
	}
	// Check for capture
	if (isCapture == 1)
		return 1;
	return 0;
}

/*****************
*Function Name: moveRook
*Input: char board[][SIZE], Move* moveI -Pointer to type struct Move,
 int dX-Distance in columns, int dY- Distance in rows
*Output: int - boolean value
*Function Operation: A function that checks whether the path is valid
according to the legality of Rook movement
******************/
int moveRook(char board[][SIZE], Move* moveI, int dX, int dY)
{
	// A situation where there is only movement in rows
	if (dX == 0)
	{
		int maxI = max(moveI->iSrc, moveI->iDest);
		int minI = min(moveI->iSrc, moveI->iDest);
		for (int i = (minI + 1); i < maxI && i < SIZE; i++)
		{
			// Check if the way is free
			if (board[i][moveI->jDest] != ' ')
			{
				return 0;
			}
		}
		return 1;
	}
	// A situation where there is only movement in columns
	if (dY == 0)
	{
		int maxJ = max(moveI->jSrc, moveI->jDest);
		int minJ = min(moveI->jSrc, moveI->jDest);
		for (int j = (minJ + 1); j < maxJ && j < SIZE; j++)
		{
			// Check if the way is free
			if (board[moveI->iDest][j] != ' ')
			{
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

/*****************
*Function Name: moveBishop
*Input: char board[][SIZE], Move* moveI -Pointer to type struct Move,
 int dX-Distance in columns, int dY- Distance in rows
*Output: int - boolean value
*Function Operation: A function that checks whether the path is valid
according to the legality of Bishop movement
******************/
int moveBishop(char board[][SIZE], Move* moveI, int dX, int dY)
{
	// Check that columns movement is the same as the rows
	if (dY == dX)
	{
		int i, j;
		// Check if the way is free in four possible directions of the move
		if (moveI->iDest > moveI->iSrc)
		{
			if (moveI->jDest > moveI->jSrc)
			{
				// 1
				for (i=moveI->iSrc + 1, j = moveI->jSrc + 1;
					i < moveI->iDest && j < moveI->jDest && i < SIZE && j < SIZE; i++,j++)
				{
					if (board[i][j] != ' ')
					{
						return 0;
					}
				}
				return 1;
			}
			// 2
			for (i = moveI->iSrc + 1, j = moveI->jSrc - 1;
				i < moveI->iDest && j > moveI->jDest && i < SIZE && j > 0; i++, j--)
			{
				if (board[i][j] != ' ')
				{
					return 0;
				}
			}
			return 1;
		}
		if (moveI->jDest > moveI->jSrc)
		{
			// 3
			for (i = moveI->iSrc - 1, j = moveI->jSrc + 1;
				i > moveI->iDest && j < moveI->jDest && i > 0 && j < SIZE; i--, j++)
			{
				if (board[i][j] != ' ')
				{
					return 0;
				}
			}
			return 1;
		}
		// 4
		for (i = moveI->iSrc - 1, j = moveI->jSrc - 1;
			i > moveI->iDest && j > moveI->jDest && i > 0 && j > 0; i--, j--)
		{
			if (board[i][j] != ' ')
			{
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

/*****************
*Fuction Name: isValidWay
*Input: char board[][SIZE], Move* moveI -Pointer to type struct Move 
*Output: int - boolean value
*Function Operation: A function that checks whether the path is valid
according to the legality of each piece's movement
******************/
int isValidWay(char board[][SIZE], Move* moveI)
{
	// Saving the distance of columns and rows
	int dY = abs(moveI->iSrc - moveI->iDest);
	int dX = abs(moveI->jSrc - moveI->jDest);
	// Pawn
	if (moveI->srcPiece == 'p' || moveI->srcPiece == 'P')
	{
		// White turn
		if (moveI->isWhite == 1)
		{
			if (moveI->isCapture == 1)
			{
				if ((moveI->iSrc - moveI->iDest) == 1 && dX == 1)
				{
					return 1;
				}
				return 0;
			}
			if (dX == 0)
			{
				if ((moveI->iSrc - moveI->iDest) == 1)
				{
					return 1;
				}
				// Location where the white pawn can go two steps
				if (moveI->iSrc == (SIZE - 2))
				{
					if ((moveI->iSrc - moveI->iDest) == 2 && board[moveI->iSrc - 1][moveI->jSrc] == ' ')
						return 1;
				}
			}
			return 0;
		}
		// Black turn
		if (moveI->isCapture == 1)
		{
			if ((moveI->iDest - moveI->iSrc) == 1 && dX == 1)
			{
				return 1;
			}
			return 0;
		}
		if (dX == 0)
		{
			if ((moveI->iDest - moveI->iSrc) == 1)
			{
				return 1;
			}
			// Location where the black pawn can go two steps
			if (moveI->iSrc == 1)
			{
				if ((moveI->iDest - moveI->iSrc) == 2 && board[moveI->iSrc + 1][moveI->jSrc] == ' ')
					return 1;

			}
		}
		return 0;
	}
	// Rook
	if (moveI->srcPiece == 'r' || moveI->srcPiece == 'R')
	{
		return moveRook(board, moveI, dX, dY);
	}
	// Bishop
	if (moveI->srcPiece == 'b' || moveI->srcPiece == 'B')
	{
		return moveBishop(board, moveI, dX, dY);
	}
	// Knight
	if (moveI->srcPiece == 'n' || moveI->srcPiece == 'N')
	{
		if ((dX == 2 && dY == 1) || (dX == 1 && dY == 2))
		{
			return 1;
		}
		return 0;
	}
	// King
	if (moveI->srcPiece == 'k' || moveI->srcPiece == 'K')
	{
		if ((dX == 1 && dY == 1) || (dX == 1 && dY == 0) || (dX == 0 && dY == 1))
		{
			return 1;
		}
		return 0;
	}
	//Queen
	return (moveBishop(board, moveI, dX, dY) || moveRook(board, moveI, dX,dY));
}

/****************
*Function Name: isKingCheck
*Input: char board[][SIZE], Move* moveI -Pointer to type struct Move
*Output: int - boolean value
*Function Operation: A function that checks whether the action
causes the opponent's piece to threaten the current player's king
*****************/
int isKingCheck(char board[][SIZE], Move* moveI)
{
	//Copy the board to a temporary array with the change of move to see if the move creates check
	char tempBoard[SIZE][SIZE];
	int i, j;
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			// Emptying the source location
			if ((i == moveI->iSrc) && (j == moveI->jSrc))
			{
				tempBoard[i][j] = ' ';
			}
			else
			{
				// Putting the piece in the destination
				if ((i == moveI->iDest) && (j == moveI->jDest))
				{
					tempBoard[i][j] = moveI->destPiece;
				}
				else
				{
					// Copy the rest of the board
					tempBoard[i][j] = board[i][j];
				}
			}
		}
	}
	int isCheck = 0;
	// Reboot a new Move to save data to check an opponent piece's access to the current player's king
	Move moveTemp = { 0 };
	moveTemp.isCapture = 1;
	moveTemp.isWhite = !(moveI->isWhite);
	// Check for the current king by the loops
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			if (moveI->isWhite == 1 && tempBoard[i][j] == 'K')
			{
				moveTemp.jDest = j;
				moveTemp.iDest = i;
			}
			else
			{
				if (moveI->isWhite == 0 && tempBoard[i][j] == 'k')
				{
					moveTemp.jDest = j;
					moveTemp.iDest = i;
				}
			}
		}
	}
	// Check for a rival piece that has access to the king
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			if ((moveI->isWhite == 1 && islower(tempBoard[i][j])) ||
				(moveI->isWhite == 0 && isupper(tempBoard[i][j])))
			{
				moveTemp.srcPiece = tempBoard[i][j];
				moveTemp.iSrc = i;
				moveTemp.jSrc = j;
				isCheck = isValidWay(tempBoard, &moveTemp);
				if (isCheck)
				{
					return 1;
				}
			}
		}
	}
	return isCheck;
}

/****************
*Function Name: isCheck
*Input: char board[][SIZE], Move* moveI -Pointer to type struct Move
*Output: int - boolean value
*Function Operation: A function that checks whether the action causes the current player
 to threaten the opposing king
*****************/
int isCheck(char board[][SIZE], Move* moveI)
{
	//Copy the board to a temporary array with the change of move to see if the move creates check
	char tempBoard[SIZE][SIZE];
	int i, j;
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			// Emptying the source location
			if ((i == moveI->iSrc) && (j == moveI->jSrc))
			{
				tempBoard[i][j] = ' ';
			}
			else
			{
				// Putting the piece in the destination
				if ((i == moveI->iDest) && (j == moveI->jDest))
				{
					tempBoard[i][j] = moveI->destPiece;
				}
				else
				{
					// Copy the rest of the board
					tempBoard[i][j] = board[i][j];
				}
			}
		}
	}
	int isCheck = 0;
	// Reboot a new Move to save data to check an access of the current player's pieces to the rival king
	Move moveTemp = {0};
	moveTemp.isCapture = 1;
	moveTemp.isWhite = moveI->isWhite;
	// Check for the rival king by the loops
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			if (moveI->isWhite == 1 && tempBoard[i][j] == 'k')
			{
				moveTemp.jDest = j;
				moveTemp.iDest = i;
			}
			else
			{
				if (moveI->isWhite == 0 && tempBoard[i][j] == 'K')
				{
					moveTemp.jDest = j;
					moveTemp.iDest = i;
				}
			}
		}
	}
	// Check for a current player's piece that has access to the rival king
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			if ((moveI->isWhite == 1 && isupper(tempBoard[i][j])) ||
				(moveI->isWhite == 0 && islower(tempBoard[i][j])))
			{
				moveTemp.srcPiece = tempBoard[i][j];
				moveTemp.iSrc = i;
				moveTemp.jSrc = j;
				isCheck = isValidWay(tempBoard, &moveTemp);
				if (isCheck)
					return 1;
			}
		}
	}
	return isCheck;
}

/*****************
*Function Name: findPlayer
*Input: char board[][SIZE], Move* moveI -Pointer to type struct Move
*Output: int - boolean value
*Function Operation: The function searches the source row
and the source column according to the piece type and it displacement
******************/
int findPlayer(char board[][SIZE], Move* moveI)
{
	int i, j;
	// A case where only a sourse row was received
	if (moveI->iSrc != 10)
	{
		i = moveI->iSrc;
		for (j = 0; j < SIZE; j++)
		{
			if (board[i][j] == moveI->srcPiece)
			{
				moveI->jSrc = j;
				if (isValidWay(board, moveI) && !isKingCheck(board, moveI))
				{
					return 1;
				}
			}
		}
		return 0;
	}
	//  A case where only a sourse column was received
	if (moveI->jSrc != 10)
	{
		j = moveI->jSrc;
		for (i = 0; i < SIZE; i++)
		{
			if (board[i][j] == moveI->srcPiece)
			{
				moveI->iSrc = i;
				if (isValidWay(board, moveI) && !isKingCheck(board, moveI))
				{
					return 1;
				}
			}
		}
		return 0;
	}
	// A case where the sourse was not received at all
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			if (board[i][j] == moveI->srcPiece)
			{
				moveI->jSrc=j;
				moveI->iSrc=i;
				if (isValidWay(board, moveI) &&!isKingCheck(board, moveI))
					return 1;
			}
		}
	}
	return 0;

}

/*****************
*Function Name: changeBoard
*Input: char board[][SIZE], Move* moveI -Pointer to type struct Move
*Output: -
Function Operation: The function changes the board according to the move
******************/
void changeBoard(char board[][SIZE], Move* moveI)
{
	// Running on the board in the loops
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			// Emptying the source location
			if (i == moveI->iSrc && j == moveI->jSrc)
			{
				board[i][j] = ' ';
			}
			// Putting the piece in the destination
			if (i == moveI->iDest && j == moveI->jDest)
			{
				board[i][j] = moveI->destPiece;
			}
		}
	}
}

/*****************
*Function Name: makeMove
*Input: char board[][SIZE], char pgn[], int isWhiteTurn
*Output: int- boolean value
*Function Operation: The function checks whether a move is valid.
  If so, carry it out
******************/
int makeMove(char board[][SIZE], char pgn[], int isWhiteTurn)
{
	// Defination of a Move type variable that saves the data given in the string
	Move moveI = { 0 };
	// Calling to function that enters to the struct Move the data's string
	checkPgn(pgn, isWhiteTurn, &moveI);
	// Check if the destination is empty (unless if it's a capture move)
	int isDempty = isDestEmpty(moveI.iDest, moveI.jDest, moveI.isCapture, moveI.isWhite, board);
	if (!isDempty)
		return 0;
	// Checking the validly of the move when the source was given
	if (moveI.iSrc != 10 && moveI.jSrc != 10)
	{
		if (!isValidWay(board, &moveI)||isKingCheck(board, &moveI))
		{
			return 0;
		}
	}
	// Find the soerce and checking the validly of the move when the source wasn't given (or partially given)
	else
	{
		if (!findPlayer(board, &moveI))
		{
			return 0;
		}
	}
	// Check if a 'check' mark was given and whether there is a 'check' situation
	if (moveI.isCheck == 1 || moveI.isMate == 1)
	{
		if (isCheck(board, &moveI))
		{
			// Making the move
			changeBoard(board, &moveI);
			return 1;
		}
		return 0;
	}
	if (isCheck(board, &moveI))
	{
		return 0;
	} 
	// Making the move
	changeBoard(board, &moveI);
	return 1;
}