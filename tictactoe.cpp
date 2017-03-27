#include <iostream>
#include <string>
#include <functional>
#include <map>
#include <stdlib.h>
using namespace std;

// tic tac toe board
template <unsigned int B>
class TTTB {
	public:
		const string PL, CL; // person letter, computer letter (X or O)
		const bool ODD; // is the board size odd?
		const int PN, CN; // board size, person number, computer number
		int board[B][B] = {}, colSum[B] = {}, rowSum[B] = {}, diagSum[2] = {};

		int winner; // winner = who won (1 = person, 10 = computer); 0 = game not over (or tie if @ B*B iterations); -1 = something broke
		bool person_playing; // is the person currently playing? (false if computer playing)

		TTTB<B>(string pl, string cl, bool pp, int pn=1, int cn=10, bool odd=((B%2)!=0)) : 
				PL(pl), CL(cl), person_playing(pp), PN(pn), CN(cn), ODD(odd) {
			winner=0;
		}

		// translate number into X, O or empty
		string translate(int n) {
		    if (n == PN) {
		        return PL;    
		    } else if (n == CN) {
		        return CL;
		    } else {
		        return " ";
		    }
		}

		// prints the board
		void printBoard() {
			string dashes = " -";
			for (int i = 0; i < B; i++) {
			      dashes += "----";
			}

		    cout << dashes << endl;
		    for (int r = 0; r < B; r++) {
		        cout << " | ";
		        for (int c = 0; c < B; c++) {
		            cout << translate(board[r][c]) << " | ";
		        }
		        cout << endl;
		        cout << dashes << endl;
		    }
		}

		// update sum arrays after new value added
		void updateSum(int r, int c, int adding) {
			cout << "row" << r << "col" << c << "adding" << adding << endl;

		    rowSum[r] += adding;
		    colSum[c] += adding;

		    if (r==c) {
				diagSum[0] += adding;
				cout << "diagsum" << diagSum[0] << endl;
				if (ODD and r==(B/2)) { // if board size is odd and middle box chosen
					diagSum[1] += adding;
				}
			} else if ((r+c)==(B-1)) {
				diagSum[1] += adding;
			}

		    for (int i = 0; i < B; i++) {
		        cout << "rowSum " << i + 1 << " is: " << rowSum[i] << endl;
   		        cout << "colSum " << i + 1 << " is: " << colSum[i] << endl;

		    }

		    for (int i = 0; i < 2; i++) {
		        cout << "diagSum " << i << " is: " << diagSum[i] << endl; // diag0 is \ (- slope) diag1 is / (+ slope)
		    }
		}

		// gets input from person and updates board and sum arrays, then sets "person" boolean to false
		void person() {
			cout << "person()" << endl;

			int row, col;

			bool error = false;

			do {
				if (error) {
					cout << "either your input was invalid or something was already there. try again." << endl;
				}

				cout << "row (please enter a number between 1 and " << B << "): ";
				cin >> row;
				cout << "column (please enter a number between 1 and " << B << "): ";
				cin >> col;

				error = true;
			} while (row > B or col > B or row < 1 or col < 1 or board[--row][--col] != 0); 
			// indices have range [0,B-1] & user input has range [1,B]

			cout << "you chose (" << row << ", " << col << ")" << endl;

			board[row][col] = PN;
			updateSum(row, col, PN);

			person_playing=false;
		}

		// if row or col or diag sum == person#*boardsize => person won
		// if row or col or diag sum == comp#*boardsize => computer won
		void checkForWinner() {
			for (int i = 0; i < B; i++) {
				if (rowSum[i]==PN*B or colSum[i]==PN*B) {
					winner = PN;
					return;
				} else if (rowSum[i]==CN*B or colSum[i]==CN*B) {
					winner = CN;
					return;
				}
		    }

		    for (int i = 0; i < 2; i++) {
		    	if (diagSum[i]==PN*B) {
					winner = PN;
					return;
				} else if (diagSum[i]==CN*B) {
					winner = CN;
					return;
				}
		    }
		}

		// find empty spot in given row
		void compRow(int r) {
			for (int c = 0; c < B; c++) {
				if (board[r][c]==0) {
					board[r][c] = CN;
					updateSum(r,c,CN);
					return;
				}
			}
			winner = -1; //something broke (probably there was supposed to be a tie)
		}

		// find empty spot in given column
		void compCol(int c) {
			for (int r = 0; r < B; r++) {
				if (board[r][c]==0) {
					board[r][c] = CN;
					updateSum(r,c,CN);
					return;
				}
			}
			winner = -1; //something broke (probably there was supposed to be a tie)
		}

		// find empty spot in given diagonal
		void compDiag(int d) {
			if (d==0) { // diagonal with - slope (\)
				for (int rc = 0; rc < B; rc++) {
					if (board[rc][rc]==0) {
						board[rc][rc] = CN;
						updateSum(rc,rc,CN);
						return;
					}
				}
			} else { // diagonal with + slope (/)
				int c;
				for (int r = 0; r < B; r++) {
					c = B - 1 - r;
					if(board[r][c]==0) {
						board[r][c] = CN;
						updateSum(r,c,CN);
						return;
					}
				}
			}
			winner = -1; //something broke (probably supposed to be a tie)
		}

		void computer() {
			person_playing = true;

			map<int,int> rows;
			map<int,int> cols;
			map<int,int> diags;

			for (int i = 0; i < B; i++) {
				int rs = rowSum[i], cs = colSum[i];
				int ds;

				cout << "rs " << rs << " cs " << cs << endl;

				// check diagonals if i < 2 (only two diagonal sums)
				if (i < 2) {
					ds = diagSum[i];
					diags.insert({ds,i}); //only need one instance of each sum
				}

				// check if computer can win (i.e. computer only needs one more spot)
				if (rs == (B-1)*CN) {
					cout << "computer can win; rs = " << endl;
					compRow(i);
					return;
				} else if (cs == (B-1)*CN) {
					cout << "computer can win; cs = " << endl;
					compCol(i);
					return;
				} else if (i < 2 and ds == (B-1)*CN) {
					cout << "computer can win; ds = " << endl;
					compDiag(i);
					return;
				} else if (rs == (B-1)*PN) { //check if person can win; if they can, block them
					cout << "checking if person can win; rs = " << rs << endl;
					compRow(i);
					return;
				} else if (cs == (B-1)*PN) {
					cout << "checking if person an win; cs = " << cs << endl;
					compCol(i);
					return;
				} else if (i < 2 and ds == (B-1)*PN) {
					cout << "checking if person can win; ds = " << ds << endl;
					compDiag(i);
					return;
				}

				rows.insert({rs,i}); //only need one instance of each sum
				cols.insert({cs,i}); //only need one instance of each sum
			}

			// get row or col or diag with greatest number of computer placed letters and run that
			for (int i = B-2; i >= 0; i--) {
				if (rows.count(i*CN)) {
					compRow(rows.at(i*CN));
					return;
				} else if (cols.count(i*CN)) {
					compCol(cols.at(i*CN));
					return;
				} else if (diags.count(i*CN)) {
					compDiag(diags.at(i*CN));
					return;
				}
			}

			// computer can't win; try to keep the person from winning
			// get row or col or diag with greatest number of person placed letters and place something there
			for (int i = B-2; i >= 0; i--) {
				if (rows.count(i*PN)) {
					compRow(rows.at(i*PN));
					return;
				} else if (cols.count(i*PN)) {
					compCol(cols.at(i*PN));
					return;
				} else if (diags.count(i*PN)) {
					compDiag(diags.at(i*PN));
					return;
				}
			}

			// there should be no reason you reach this point 
			// if you do, something is probably broken
			// just pick something random
			int randRow = -1, randCol = -1, iterations = 0, MAX_ITERS=10000;
			do {
				randRow = rand() % B;
				randCol = rand() % B;
				iterations++;
			} while (board[randRow][randCol] != 0 and iterations < MAX_ITERS);

			if (iterations==MAX_ITERS) { //something broke even more (supposed to be a tie probably)
				winner = -1;
				return;
			}

			board[randRow][randCol] = CN;
			updateSum(randRow, randCol, CN);

		}

		// returns who wins
		void game() {
			cout << "game()" << endl;

			if (person_playing) { // if person is playing
				person();
			} else { // computer is playing
				computer();
			}

			checkForWinner();
		}
};

int main() {
	const int b = 5;

	cout << "hello world 0" << endl;
	TTTB<b> ttt = TTTB<b>("X", "O", true);
	
	int iters = 0;

	do {
		ttt.game();
		ttt.printBoard();
		iters++;
	} while (not ttt.winner and iters < b*b); // do this while there is no winner (i.e. while winner == 0)
	// or before max iterations reached (iters = board_size squared => board full => tie)
	cout << "winner" << ttt.winner << endl;
	cout << "hello world 1" << endl;
	return 0;
}
