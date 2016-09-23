// Example program
#include <iostream>
#include <string>
#include <functional>
using namespace std;

//board size
const int b = 3;
//make empty board
int board[b][b] = {};
//row, col, and diagonal sum arrays
int rowSum[b] = {}, colSum[b] = {}, diagSum[2] = {};
//who won (if 0 game not over), if -1 tie
int winner = 0;
// true if person plays first
bool personFirst = false;
// true if first turn of computer/ai
bool first = true;
// keep track of number of turns if there’s a tie
int turns = 0;

//translate number into X, O or empty
char translate(int n) {
    if (n == 1) { //person is X
        return 'X';    
    } else if (n == 10) {
        return  'O'; //computer is O
    } else {
        return ' ';    
    }
}

// print the board
void printBoard() {
    cout << "  -----------" << endl;
    for (int r = 0; r < 3; r++) {
        cout << " | ";
        for (int c = 0; c < 3; c++) {
            cout << translate(board[r][c]) << " | ";
        }
        cout << endl;
        cout << "  -----------" << endl;
    }
}

// update sum arrays after new value added
void updateSum(int r, int c, int adding) {
    rowSum[r] += adding;
    colSum[c] += adding;
    if ((r-c)==0) {
        diagSum[0] += adding;
    } 
    if ((r+c)==(b-1)) {
        diagSum[1] += adding;
    }
    cout << "rows" << endl;
    for (int i = 0; i < b; i++) {
        cout << "index " << i << " is: " << rowSum[i] << endl;
    }
    cout << "cols" << endl;
    for (int i = 0; i < b; i++) {
        cout << "index " << i << " is: " << colSum[i] << endl;
    }
    cout << "diags" << endl;
    for (int i = 0; i < 2; i++) {
        cout << "index " << i << " is: " << diagSum[i] << endl;
    }
}

// check if sum means person or AI won; bool is true if person is playing
// called by checkBoard
bool checkSum(int sum, bool person) {
    if (person) {
        if (sum == 3) {
            winner = 1;
            cout << "YOU WON!!!";
            return true;
        }
    } else {
        if (sum == 30) {
            winner = 10;
            cout << "THE AI BEAT YOU HAHA LOL" << endl;
            return true;
        }
    }
    return false;
}

// return true if someone won
bool checkBoard(bool person) {
    // check horizontals
    for (int sum : rowSum) {
        if (checkSum(sum, person)) {
            return true;
        }
    }

    // check verticals
    for (int sum : colSum) {
        if (checkSum(sum, person)) {
            return true;
        }
    }
   
    // check diagonals
    for (int sum : diagSum) {
        if (checkSum(sum, person)) {
            return true;
        }
    }
    return false;
}

// returns true if the AI placed an O (in an empty spot)
// check means are you looking for Xs (1) or Os (10)
// m means are you looking two in a row or one in a row with the rest empty
bool check(int check, int m) {
    int sum = 0;
    // check diagonals
    sum = diagSum[0];
    if (sum==check*m) {
        for (int rc = 0; rc < b; rc++) {
            if (board[rc][rc] == 0) {
                board[rc][rc] = 10;
                updateSum(rc, rc, 10);
                cout << "check diag 1 The AI placed an O at row " << rc + 1 << " and column " << rc + 1 << "." << endl;
                return true;
            }
        }
    }
    sum = diagSum[1];
    if (sum==check*m) {
        for (int r = 0; r < b; r++) {
            int c = b - r - 1;
            if (board[r][c] == 0) {
                board[r][c] = 10;
                updateSum(r, c, 10);
                cout << "check diag 2 The AI placed an O at row " << r + 1 << " and column " << c + 1 << "." << endl;
                return true;
            }
        }
    }

    // check horizontals; i is the index of the row array and the index of its sum in rowSum
    sum = 0;
    for (int r = 0; r < b; r++) {
        sum = rowSum[r];
        cout << "sum is " << sum << " and check*m is " << check*m << endl;
        if (sum==check*m) {
            for (int c = 0; c < b; c++) {
                if (board[r][c] == 0) {
                    // put an O here 
                    board[r][c] = 10;
                    updateSum(r, c, 10);
                    cout << "check row The AI placed an O at row " << r + 1 << " and column " << c + 1 << "." << endl;
                    return true;
                }
            }
        }
    }
    // check verticals
    for (int c = 0; c < b; c++) {
        sum = colSum[c];
        if (sum==check*m) {
            for (int r = 0;  r < b; r++) {
                if (board[r][c] == 0) {
                    board[r][c] = 10;
                    updateSum(r, c, 10);
                    cout << "check vert The AI placed an O at row " << r + 1 << " and column " << c + 1 << "." << endl;
                    return true;
                }
            }
        }
    }
   
    return false;
}

bool secret() {
    if (b==3 && !personFirst) {
        int sum = diagSum[0];
        if (sum==11) {
            for (int rc = 0; rc < b; rc++) {
                if (board[rc][rc] == 0) {
                    board[rc][rc] = 10;
                    updateSum(rc, rc, 10);
                    cout << "The AI placed an O at row " << rc + 1 << " and column " << rc + 1 << "." << endl;
                    return true;
                }
            }
        }
        sum = diagSum[1];
        if (sum==11) {
            for (int r = 0; r < b; r++) {
                int c = b - r - 1;
                if (board[r][c] == 0) {
                    board[r][c] = 10;
                    updateSum(r, c, 10);
                    cout << "The AI placed an O at row " << r + 1 << " and column " << c + 1 << "." << endl;
                    return true;
                }
            }
        }
    }  
    return false;
}

// returns true if AI wins
bool ai() {
    cout << "ai on" << endl;
    if (first) {
        int rc = b/2;
        if (board[rc][rc] == 0) {
            board[rc][rc] = 10;
            updateSum(rc, rc, 10);
            cout << "The AI placed an O at row " << rc + 1 << " and column " << rc + 1 << "." << endl;
        } else {
            int row =  (rand() % 2)*(b-1);
            int col = (rand() % 2)*(b-1);
            board[row][col] = 10;
            updateSum(row, col, 10);
            cout <<  "The AI placed an O at row " << row + 1 << " and column " << col + 1 << "." << endl;
        }
        first = false;
        return false;
    }
    if(check(1, 2)) { //the ai played
        cout << "check 1 2" << endl;
        return false;
    }
    if(check(10, 2)) { //the ai won
        cout << "check 10 2" << endl;
        winner = 10;
        cout << "THE AI BEAT YOU HAHA LOL" << endl;
        return true;
    }
    if (secret()) {
        return false;    
    }
    if (check(10, 1)) {
        return false;
    }
    return false; //the ai didn’t win
}

// returns true if person won
bool person() {
    //get row input
    string r;
    cout << "row: ";
    getline (cin, r);
    int row = atoi(r.c_str());
    row = row - 1;    

    //get col input
    string c;
    cout << "column: ";
    getline (cin, c);
    int col = atoi(c.c_str());
    col = col - 1;

    // check if valid input
    if (row >= 0 && row <= (b-1) && col >= 0 && col <= (b-1) && board[row][col] == 0) {
        //update board
        board[row][col] = 1;
        //update sum array
        updateSum(row, col, 1);
    } else {
        // request new user input
        cout << "invalid input or there’s something there. please try again" << endl;
        person();
    }
    
    //print board
    printBoard();
    
    //checks if player won
    if(checkBoard(true)) {
        return true;
    }
    return false;
}

void play() {
    if (personFirst) {
        //calls person to play
        if(person()) {
            return;
        }
        turns++; //there was one more turn
        cout << "turns " << turns << endl;
        if (turns == b*b) {
            printBoard();
            winner = -1;
            return;
        }
        //calls AI to play; prints board; returns if AI won
        if(ai()) {
            printBoard();
            return;
        }
        turns++;
        cout << "turns " << turns << endl;
        if (turns == b*b) {
            printBoard();
            winner = -1;
            return;
        }
        printBoard();  
    } else {
       //calls AI to play; prints board; returns if AI won
        if(ai()) {
            printBoard();
            return;
        }
        turns++;
        if (turns == b*b) {
            printBoard();
            winner = -1;
            return;
        }
        printBoard();    

        //calls person to play
        if(person()) {
            return;
        }
        turns++;
        if (turns == b*b) {
            printBoard();
            winner = -1;
            return;
        }
    }
}

int main() {  
    printBoard();
    while (winner==0) {
        play();
    }
}


