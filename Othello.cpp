#include <iostream>
#include <vector>

using namespace std;

static const int SIZE = 8;
void createBoard(char board[][SIZE]);
void printBoard(char board[][SIZE]);
void beginGame(char board[][SIZE]);
bool humanOrComp();
void resetBoard(int moveSet[][SIZE]);
void resetBoard(double moveSet[][SIZE]);
void swapPlayer(bool* turn);
void checkBoard(char board[][SIZE], int moveSet[][SIZE], char playerPiece, char opponentsPiece);
int recursiveCheck(char board[][SIZE], int rowDirection, int columnDirection, char basePiece, char targetPiece, int initialRow, int initialColumn);
int checkAmountOfMoves(int moveSet[][SIZE]);
int countSpaces(const char board[][SIZE]);
bool recursiveCheck(char board[][SIZE], int rowDirection, int columnDirection, char basePiece, char targetPiece, int initialRow, int initialColumn, bool flag);
void getPlayerChoice(int moveSet[][SIZE], int* userInput);
void convertChar(string userInput, bool *isValid, int moveSet[][SIZE], int* input);
void checkIfExists(char *userInput, int moveSet[][SIZE], bool *isValid, int *input);
int returnInteger(char userInput);
void blackAndWhiteCount(int *blackAndWhite, char board[][SIZE]);
void tallyScores(int moveSet[][SIZE], int *userMove, int moveAmount);

char convertChar(int i);

/**
 * I use the main to begin the game in the sequence create the board  with 8 X 8 size (or SIZE) -> initialize all values in the board as ' '
 *  -> begin the game handing the board to the function
 *
 * @return standard return 0
 */
int main() {
    char board[SIZE][SIZE];
    createBoard(board);
    beginGame(board);
    return 0;
}
/**
 * not actually recursion, just looping through the given board beginning at the position given by the user which is a ' ' which then
 * -> loops until the iteration of the baord = the player's piece, then breaks from the function
 *
 * @param board current game board
 * @param rowDirection direction that the row will be shifted
 * @param columnDirection direction that the column will be shifted
 * @param basePiece the character that is the player's piece
 * @param initialRow the user's row input
 * @param initialColumn the user's column input
 * @param flag a checkpoint for if more than 1 line of pieces need to be moved to the designated space
 */

void recursiveMove(char board[][SIZE], int rowDirection, int columnDirection, char basePiece, int initialRow, int initialColumn, bool flag) {
    for(int i = 0; i < SIZE; i ++) {
        if (board[initialRow][initialColumn] == ' ') {
            board[initialRow][initialColumn] = basePiece;
        }else if(board[initialRow][initialColumn] == basePiece && flag){
            break;
        }else {
            board[initialRow][initialColumn] = basePiece;
            flag = true;
        }
        initialRow += rowDirection;
        initialColumn += columnDirection;
    }
}

/**
 * moves the piece on the board, checking all 8 directions and storing the false/trues in a array with the size equal to the amount of possible moves
 * then uses the recursivecheck to recursively check if the piece's move is valid or invalid, if it is found to be valid
 * the piece will then move all true indicated booleans using the recursiveMove function until all moves that are possible for that position are done
 *
 * @param move the user's input move
 * @param board the game board
 * @param piece the user's piece
 * @param opponents the opponent's piece
 */

inline void movePiece(int *move, char board[][SIZE], char piece, char opponents){
    int row = move[0];
    int column = move[1];
    if(piece == 'w')
        cout << endl <<"Player 2 played: " << move[0] + 1 << convertChar(move[1]) << endl << endl;
    else if(piece == 'b')
        cout << endl <<"Player 1 played: " << move[0] + 1 << convertChar(move[1]) << endl << endl;
    bool directionCheck[8]{false, false, false, false, false, false, false, false};
    directionCheck[0] = recursiveCheck(board, +1, 0, piece, opponents, row, column, false); //vertically down check
    directionCheck[1] = recursiveCheck(board, +1, 1, piece, opponents, row, column, false); //down right check
    directionCheck[2] = recursiveCheck(board, +1, -1, piece, opponents, row, column, false); //down left check
    directionCheck[3] = recursiveCheck(board, 0, +1, piece, opponents, row, column, false); //right check
    directionCheck[4] = recursiveCheck(board, 0, -1, piece, opponents, row, column, false); //left check
    directionCheck[5] = recursiveCheck(board, -1, -1, piece, opponents, row, column, false); //up left check
    directionCheck[6] = recursiveCheck(board, -1, 0, piece, opponents, row, column, false); //vertically up check
    directionCheck[7] = recursiveCheck(board, -1, +1, piece, opponents, row, column, false); // up right check
    if(directionCheck[0])
        recursiveMove(board, 1, 0, piece, row, column, false);
    if(directionCheck[1])
        recursiveMove(board, 1, 1, piece, row, column, false);
    if(directionCheck[2])
        recursiveMove(board, +1, -1, piece, row, column, false);
    if(directionCheck[3])
        recursiveMove(board, 0, 1, piece, row, column, false);
    if(directionCheck[4])
        recursiveMove(board, 0, -1, piece, row, column, false);
    if(directionCheck[5])
        recursiveMove(board, -1, -1, piece, row, column, false);
    if(directionCheck[6])
        recursiveMove(board, -1, 0, piece, row, column, false);
    if(directionCheck[7])
        recursiveMove(board, -1, +1, piece, row, column, false);
}
/**
 * For printing the move input for each player
 *
 * @param i the int value to be converted to a char to represent the letters of the board
 * @return a char value from the int value
 */

char convertChar(int i) {
    i = i + 'a';
    return i;
}

/**
 * Begins the game, creates a boolean for if the game is over or not, playerturn to track which player's turn it is, and a pointer to the playerTurn bool
 * so that it can be changed after each turn without clutter the function with needless lines of code. playerOrComputer is the question if
 * the user would like to play a computer or another player. A separate array called moveSet stores if there is a valid more in that position
 * and if there is valid move, the int at this position in the index similar to the board will be incremented by one to a maximum of 8
 * the score is also being tracked as well as the turn to indicate to the user what is the score from black to white pieces and the current turn
 * after all information has been set, the function enters a do{}while() loop that plays until gameOver is true. There are 3 options, p1 is human, p2 is
 * a computer or a human. The game is played until there is no more moves able to be played by one of the players or there are no moves left.
 *
 *
 * @param board the game board
 */

void beginGame(char board[][SIZE]) {
    bool gameOver = false; bool playerTurn = false; bool* pPlayerTurn = &playerTurn;//false = black true = white (or player 1 = false, player 2 = true)
    bool playerOrComputer = humanOrComp(); //true = human false == computer
    int* userMove = new int[2]{0,0}; int* countBlackAndWhite = new int[2]{0,0}; int turnNumber = 0;
    cout << endl;
    int moveSet[SIZE][SIZE]; int blankSpaces = countSpaces(board);
    resetBoard(moveSet);
    blackAndWhiteCount(countBlackAndWhite, board);
    do{
        printBoard(board);
        if(!playerTurn){
            resetBoard(moveSet);
            checkBoard(board, moveSet, 'b', 'w');
            if(checkAmountOfMoves(moveSet) == 0) {
            }else{
                cout << "Player 1, ";
                getPlayerChoice(moveSet, userMove);
                movePiece(userMove, board, 'b', 'w');
            }
        }else if(playerTurn && playerOrComputer){
            resetBoard(moveSet);
            checkBoard(board, moveSet, 'w', 'b');
            if(checkAmountOfMoves(moveSet) == 0) {
            }else{
                cout << "Player 2, ";
                getPlayerChoice(moveSet, userMove);
                movePiece(userMove, board, 'w', 'b');
            }
        }else{
            resetBoard(moveSet);
            checkBoard(board, moveSet, 'w', 'b');
            if(checkAmountOfMoves(moveSet) == 0) {
            }else {
                cout << "Computer's turn." << endl;
                tallyScores(moveSet, userMove, blankSpaces);
                movePiece(userMove, board, 'w', 'b');
            }
        }
        swapPlayer(pPlayerTurn);
        blankSpaces = countSpaces(board);
        blackAndWhiteCount(countBlackAndWhite, board);
        turnNumber += 1;
        cout << "\t\tCurrent turn number : " << turnNumber<< endl<< "Current score: Number of Black Pieces: " << countBlackAndWhite[0] << " White Pieces: " << countBlackAndWhite[1] << endl;
        if(blankSpaces == 0 || checkAmountOfMoves(moveSet) == 0 || gameOver){
            printBoard(board);
            gameOver = true;
            if(countBlackAndWhite[1] > countBlackAndWhite[0]){
                cout << endl << "White is the winner!" << endl;
            }else if(countBlackAndWhite[0] > countBlackAndWhite[1]) {
                cout << endl << "Black is the winner!" << endl;
            }
        }
    }while(!gameOver);
}

/**
 * This function is how the computer thinks / decides to make a move. There are certain positions in Othello that are very dangerous to play in,
 * while there are also rows / columns that are extremely good to have. I built the function so that the "danger zones" would be avoided and picked least of all
 * where the "good areas" had higher score values if the computer is able to move the piece into that position.
 *
 * the complexity is O(n^2) as there is a nested loop used to search the board
 *
 * @param moveSet the possible move choices
 * @param userMove the computers move that will be input into a int* which will allow the beginGame function to enter the move into the board
 */

void tallyScores(int moveSet[][SIZE], int *userMove, int amountOfMoves) {
    double scoreDecision[SIZE][SIZE];
    resetBoard(scoreDecision);
    if(amountOfMoves > 10) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if ((i == 0 || i == 7)) {
                    if (j > 1 && j < 6)
                        scoreDecision[i][j] = moveSet[i][j] * 4;
                    else if (j <= 0 || j >= 7)
                        scoreDecision[i][j] = moveSet[i][j] * 10;
                    else
                        scoreDecision[i][j] = moveSet[i][j] * .5;
                } else if ((i == 1 || i == 6) && (j == 0 || j == 7 || j == 1 || j == 6)) {
                    scoreDecision[i][j] = moveSet[i][j] * .5;
                } else if ((i >= 3 && i <= 4) && (j >= 2 && j <= 5)) {
                    scoreDecision[i][j] = moveSet[i][j] * 6;
                } else
                    scoreDecision[i][j] = moveSet[i][j];
            }
        }
    } else{
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if ((i == 0 || i == 7)) {
                    if (j > 1 && j < 6)
                        scoreDecision[i][j] = moveSet[i][j] * 6;
                    else if (j <= 0 || j >= 7)
                        scoreDecision[i][j] = moveSet[i][j] * 10;
                    else
                        scoreDecision[i][j] = moveSet[i][j] * .5;
                } else if ((i == 1 || i == 6) && (j == 0 || j == 7 || j == 1 || j == 6)) {
                    scoreDecision[i][j] = moveSet[i][j] * .5;
                } else if ((i >= 3 && i <= 4) && (j >= 2 && j <= 5)) {
                    scoreDecision[i][j] = moveSet[i][j];
                } else
                    scoreDecision[i][j] = moveSet[i][j];
            }
        }
    }
    double max = 0; int firstIndex = 0, secondIndex = 0; //finding the highest score index for the computer to play
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            if(max < scoreDecision[i][j]){
                max = scoreDecision[i][j];
                firstIndex = i;
                secondIndex = j;
            }
        }
    }
    userMove[0] = firstIndex;
    userMove[1] = secondIndex;
}

/**
 * Counts the amount of black and white pieces on the board
 *
 * @param blackAndWhite a int* with the values of the amount of black and white pieces [0] and [1] respectively
 * @param board the game board
 */

void blackAndWhiteCount(int *blackAndWhite, char board[][SIZE]) {
    blackAndWhite[0] = 0;
    blackAndWhite[1] = 0;
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j ++) {
            if (board[i][j] == 'b')
                blackAndWhite[0] += 1;
            else if(board[i][j] == 'w')
                blackAndWhite[1] += 1;
        }
    }
}

/**
 * This function prints a line asking for a valid input from the user and will loop until a valid choice has been given
 *
 * @param moveSet the valid places on the board able to move to
 * @param input the position the user will move to
 */

void getPlayerChoice(int moveSet[][SIZE], int* input) {
    string userInput = ""; bool isValid = false; bool *pIsValid = &isValid;
    do{
        cout << "please input a valid move (ex: 4a, 3b, 2c, etc) :";
        getline(cin, userInput);
        convertChar(userInput, pIsValid, moveSet, input);
        if(!isValid)
            cout << endl << "Sorry, that was not a correct input." << endl;
    }while(!isValid);
}

/**
 * When a user inputs a position, it is given in char 'LETTER' and 'NUMBER' so I convert them into integer values and then check if
 * index values of those given integers are valid on the game board.  If it is not, it is rejected and another input is requested. The
 * game board knows if there is 0 or 1 moves available, if there is a move available, the function will keep looping until a valid
 * input is given
 *
 * @param userInput the user's input
 * @param isValid if the input is valid
 * @param moveSet the 2d array with valid move placements
 * @param input pointer to an array stored in the begin game function so I am not returning the option, it will exist or the game will end
 */

void convertChar(string userInput, bool *isValid, int moveSet[][SIZE], int* input) {
    char* tempArray = new char[userInput.size()];
    if(userInput.empty() || userInput.size() > 2) {
        *isValid = false;
        return;
    }else {
        for (int i = 0; i < userInput.size(); i++)
            tempArray[i] = userInput[i];
        if(userInput.size() == 2)
            checkIfExists(tempArray, moveSet, isValid, input);
    }
}

/**
 * checks if the place the user has indicated is valid ont he moveSet 2d array
 *
 * @param userInput user's input
 * @param moveSet the available move places on the board
 * @param isValid if the move is valid or not (condition for the getPlayerChoice do{}while() loop)
 * @param input stores the move in an int pointer to be used to move the piece
 */

void checkIfExists(char *userInput, int moveSet[][SIZE], bool *isValid, int* input) {
    int userPlace[2]{0,0};
    for(int i = 0; i < 2; i ++){
        userPlace[i] = returnInteger(userInput[i]);
    }
    if(moveSet[userPlace[0]][userPlace[1]] >= 1) {
        input[0] = userPlace[0];
        input[1] = userPlace[1];
        *isValid = true;
    }
}

/**
 * returns the integer value of the string the user inputs into the move choice option
 *
 * @param userInput the inputs the user has given to move to
 * @return the integer value of the input given
 */

int returnInteger(char userInput) {
    if(isalpha(userInput))
        return userInput - 'a';
    else if(isdigit(userInput))
        return userInput - '0' - 1;
    else
        return 0;
}

/**
 * Counts the spaces on the board currently (used as a gameOver flip switch)
 *
 * @param board the game board
 * @return the available spaces on the board
 */

int countSpaces(const char board[][SIZE]) {
    int spaces = 0;
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            if(board[i][j] == ' ')
                spaces += 1;
    return spaces;
}
/**
 * checks the amount of moves available, (used as a gameOver switch)
 *
 * @param moveSet the 2d array that stores the amount of moves for that given player (if 0, game is over)
 * @return the integer value of the amount of moves availabe on the board
 */

int checkAmountOfMoves(int moveSet[][SIZE]) {
    int moves = 0;
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            if(moveSet[i][j] >= 1)
                moves += 1;
    return moves;
}

/**
 *  recursively checks all 8 directions of the board (O(n^2) complexity) using a directional iteration, the current position, the index of the nested loop
 *
 * @param board the game board
 * @param moveSet valid moves
 * @param playerPiece the current player's turn
 * @param opponentsPiece the current opponent's turn
 */

void checkBoard(char board[][SIZE], int moveSet[][SIZE], char playerPiece, char opponentsPiece) {
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            moveSet[i][j] += recursiveCheck(board, +1, 0, playerPiece, opponentsPiece, i, j); //vertically down check
            moveSet[i][j] += recursiveCheck(board, +1, 1, playerPiece, opponentsPiece, i, j); //down right check
            moveSet[i][j] += recursiveCheck(board, +1, -1, playerPiece, opponentsPiece, i, j); //down left check
            moveSet[i][j] += recursiveCheck(board, 0, 1, playerPiece, opponentsPiece, i, j); //right check
            moveSet[i][j] += recursiveCheck(board, 0, -1, playerPiece, opponentsPiece, i, j); //left check
            moveSet[i][j] += recursiveCheck(board, -1, -1, playerPiece, opponentsPiece, i, j); //up left check
            moveSet[i][j] += recursiveCheck(board, -1, 0, playerPiece, opponentsPiece, i, j); //vertically up check
            moveSet[i][j] += recursiveCheck(board, -1, +1, playerPiece, opponentsPiece, i, j); //up right check
        }
    }
}
/**
 * A helper function that sets up the actual recursive function
 *
 * @param board game board
 * @param rowDirection directional input
 * @param columnDirection directional input
 * @param basePiece player's piece
 * @param targetPiece opponent's piece
 * @param initialRow the initial row
 * @param initialColumn the initial column
 * @return a 1 or 0 based on if there is a move in that position or not
 */

int recursiveCheck(char board[][SIZE], int rowDirection, int columnDirection, char basePiece, char targetPiece, int initialRow, int initialColumn){
    bool checkFlag = false;
    checkFlag = recursiveCheck(board, rowDirection, columnDirection, basePiece, targetPiece, initialRow, initialColumn, checkFlag);
    if(checkFlag)
        return 1;
    else
        return 0;
}

/**
 * recursively checks if the piece of the current player can be moved in that position, if it is found to be true that the piece
 * can be placed in that position, the function will return a true, if it's found out of bounds, or incorrect, the function will reject and return
 * a false
 *
 * @param board the game board
 * @param rowDirection directional input
 * @param columnDirection directional input
 * @param basePiece player's piece
 * @param targetPiece opponent's piece
 * @param initialRow the initial row
 * @param initialColumn the initial column
 * @param flag this flag is used to indicate if the first position is the same piece, if the flag is false and the iterator is equal, it returns false. if the flag is true and it the index is equal, the function will return true
 * @return
 */

bool recursiveCheck(char board[][SIZE], int rowDirection, int columnDirection, char basePiece, char targetPiece, int initialRow, int initialColumn, bool flag) {
    if(board[initialRow][initialColumn] != ' ' && !flag)
        return false;
    initialRow += rowDirection;
    initialColumn += columnDirection;
    if(initialRow < 0 || initialRow > 7 || initialColumn < 0 || initialColumn > 7)
        return false;
    if(board[initialRow][initialColumn] == targetPiece && !flag)
        return recursiveCheck(board, rowDirection, columnDirection, basePiece, targetPiece, initialRow, initialColumn, true);
    else if(board[initialRow][initialColumn] == targetPiece && flag)
        return recursiveCheck(board, rowDirection, columnDirection, basePiece, targetPiece, initialRow, initialColumn, true);
    else if(board[initialRow][initialColumn] == basePiece && !flag)
        return false;
    else if(board[initialRow][initialColumn] == basePiece && flag)
        return true;
    else if(board[initialRow][initialColumn] == ' ' && !flag)
        return false;
}

/**
 * changes the turn from 0 -> 1 -> 0 etc until the game is over
 *
 * @param turn current player's turn
 */

void swapPlayer(bool* turn) {
    if(*turn)
        *turn = false;
    else
        *turn = true;
}

/**
 * resets the board so that no values are carried over to the next user
 *
 * @param moveSet
 */

void resetBoard(int moveSet[][SIZE]) {
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            moveSet[i][j] = 0;
}
/**
 * resets the moveSets values to 0
 *
 * @param moveSet
 */
void resetBoard(double moveSet[][SIZE]) {
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            moveSet[i][j] = 0;
}

/**
 * asks the user if they would like to face a human or computer opponent
 * @return if the user would like to play a human or computer opponent
 */

bool humanOrComp() {
    string answer = "";
    do{
        cout << "Would you like to play Othello against a computer or another person? (please input computer or person): ";
        getline(cin, answer);
        if(answer == "computer") {
            return false;
        } else if (answer == "person") {
            return true;
        } else
            cout << endl;
    }while(true);
}

/**
 * prints the board using cout for the borders and printf for the piece placement
 * @param board the current game board
 */

void printBoard(char board[][SIZE]) {
    char row = 'a';
    cout << "     ";
    for(int l = 0; l < SIZE; l++)
        cout << row++ << "   ";
    cout << endl;
    for(int i = 0; i < SIZE; i++) {
        cout << "   -";
        for (int j = 0; j < SIZE; j++)
            cout << "----";
        cout << endl;
        cout << 1+i << "  ";
        for (int k = 0; k < SIZE; k++) {
            printf("| %c ", (k == SIZE ? ' ' : board[i][k]));
        }
        cout << '|' << endl;
    }
    cout << "   -";
    for (int j = 0; j < SIZE; j++)
        cout << "----";
    cout << endl << endl;
}

/**
 * initializes the board so to have 4 pieces on the board and a clear board of empty slots (' ')
 * @param board current game board
 */

void createBoard(char board[][SIZE]) {
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            board[i][j] = ' ';
    board[SIZE / 2 - 1][SIZE / 2 - 1] = 'w';
    board[SIZE / 2 - 1][SIZE / 2] = 'b';
    board[SIZE / 2][SIZE / 2 - 1] = 'b';
    board[SIZE / 2][SIZE / 2] = 'w';
}