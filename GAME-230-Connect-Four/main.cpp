#include "iostream"

using namespace std;

int BOARD[20][20] = { 0 };
int ROW = 0;
int COLUMN = 0;
int REQTOWIN = 0;
enum GAMEMODE
{
	SIMPLE,
	CUSTOMIZEDBOARD,
	WRAPAROUND,
	REMOVE,
	AI	
}; 
GAMEMODE mode;

int InitialBoard() {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			BOARD[i][j] = 0;
		}
	}
	return 0;
}

int ShowMainMenu() {
	int modeNum;
	
	int modeFlag = 1;
	system("cls");
	cout << "This is a Connect Four game.\nPlease choose the game mode you want to play.\n"
		<< "\t1.Simple mode\n"
		<< "\t2.Customized Board\n"
		<< "\t3.Wrap Around\n"
		<< "\t4.Remove Enable\n"
		<< "\t5.Play With A.I.\n"
		<< "\t6.Exit\n"
		<< ">";
	
	while (modeFlag) {
		cin >> modeNum;
		if (cin.fail() || modeNum > 6 || modeNum < 1) {
			cout << "Invalid input, please try again.\n";
			//fflush(stdin);
			cin.clear();
			//std::cin.sync();
			cin.ignore(1000, '\n');
			cout << ">";
		}
		else {
			if (modeNum == 6) {
				return 1;
			}
			modeFlag = 0;
			mode = (GAMEMODE)(modeNum - 1);
			
		}
	}
	return 0;
}
//Return 1 means playerX wins, -1 means playerO wins, 0 means nobody wins, 777 means both win draw(in remove mode)
int CheckWin() {
	int temp = 0;
	int tempI = 0;
	int tempJ = 0;
	int rest = 0;
	int playerXWin = 0;
	int playerOWin = 0;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			if (BOARD[i][j] != 0) {
				temp = BOARD[i][j];				
				//Check ¡ú win
				if ((j+REQTOWIN) <= COLUMN || mode == WRAPAROUND) {
					tempI = i;
					tempJ = j;
					rest = REQTOWIN;
					while (rest) {
						tempJ++;
						if (mode == WRAPAROUND) {
							tempJ = tempJ % COLUMN;
						}
						if (BOARD[tempI][tempJ] != temp) {
							break;
						}
						else
						{
							rest--;
							if (rest == 1) {
								//return temp;
								if (temp == -1) {
									playerXWin = 1;
								}
								else {
									playerOWin = 1;
								}
							}
						}						
					}
				}
				//Check ¡ý win
				if ((i+REQTOWIN) <= ROW) {
					tempI = i;
					tempJ = j;
					rest = REQTOWIN;
					while (rest) {
						tempI++;
						if (BOARD[tempI][tempJ] != temp) {
							break;
						}
						else
						{
							rest--;
							if (rest == 1) {
								//return temp;
								if (temp == -1) {
									playerXWin = 1;
								}
								else {
									playerOWin = 1;
								}
							}
						}
					}
				}
				//Check ¨K win
				if (((j + REQTOWIN) <= COLUMN || mode == WRAPAROUND )&& (i + REQTOWIN) <= ROW) {
					tempI = i;
					tempJ = j;
					rest = REQTOWIN;
					while (rest) {
						tempI++;
						tempJ++;
						if (mode == WRAPAROUND) {
							tempJ = tempJ % COLUMN;
						}
						if (BOARD[tempI][tempJ] != temp) {
							break;
						}
						else
						{
							rest--;
							if (rest == 1) {
								//return temp;
								if (temp == -1) {
									playerXWin = 1;
								}
								else {
									playerOWin = 1;
								}
							}
						}
					}
				}
				//Check ¨L win
				if (((j - REQTOWIN) >= -1 || mode == WRAPAROUND) && (i + REQTOWIN) <= ROW) {
					tempI = i;
					tempJ = j;
					rest = REQTOWIN;
					while (rest) {
						tempI++;
						tempJ--;
						if (mode == WRAPAROUND) {
							tempJ = (tempJ + COLUMN) % COLUMN;
						}
						if (BOARD[tempI][tempJ] != temp) {
							break;
						}
						else
						{
							rest--;
							if (rest == 1) {
								//return temp;
								if (temp == -1) {
									playerXWin = 1;
								}
								else {
									playerOWin = 1;
								}
							}
						}
					}
				}
			}
		}
	}
	if (playerOWin && playerXWin) {
		return 777;
	}
	else if (playerXWin) {
		return -1;
	}
	else if (playerOWin) {
		return 1;
	}
	return 0;
}

int CheckDraw() {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			if (BOARD[i][j] == 0) {
				return 0;
			}
		}
	}
	return 1;
}

int ShowBoard() {
	for (int i = -1; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			if (i == -1) {
				cout << j + 1;
			}
			else {
				if (BOARD[i][j] == 0) {
					cout << ".";
				}
				if (BOARD[i][j] == -1) {
					cout << "X";
				}
				if (BOARD[i][j] == 1) {
					cout << "O";
				}
			}	
		}
		cout << endl;
	}
	return 0;
}

//Return 0 means insert succeed, return 1 means the column is full
int InsertPiece(int player, int column) {
	int depth = ROW - 1;
	while (depth >= 0) {
		if (BOARD[depth][column - 1] == 0) {
			BOARD[depth][column - 1] = player;
			return 0;
		}
		else {
			depth--;
		}
	}
	return 1;
}
//Return 0 means remove succeed, return 1 means remove failed
int RemovePiece(int player, int column) {
	int depth = ROW - 1;
	if (BOARD[depth][column - 1] == player) {
		while (depth > 0 || BOARD[depth][column - 1] != 0) {
			BOARD[depth][column - 1] = BOARD[depth - 1][column - 1];
			depth--;
			if (depth == 0) {
				BOARD[depth][column - 1] = 0;
			}
		}
		return 0;
	}
	
	return 1;
}

int GameFunc() {
	InitialBoard();
	int rowFlag = 1;
	int columnFlag = 1;
	int reqToWinFlag = 1;
	int turn = 0;
	int gameEnd = 1;
	int columnNum = 0;
	int player = 0;
	int winner = 0;
	switch (mode)
	{
	case CUSTOMIZEDBOARD:
		cout << "Please enter the number of rows.\n";
		cout << ">";
		while (rowFlag) {
			cin >> ROW;
			if (cin.fail() || ROW < 4 || ROW > 20) {
				cout << "Invalid input, please try again.\n";
				cin.clear();
				cin.ignore(1000, '\n');
				cout << ">";
			}
			else {
				rowFlag = 0;
			}
		}
		cout << "Please enter the number of columns.\n";
		cout << ">";
		while (columnFlag) {
			cin >> COLUMN;
			if (cin.fail() || COLUMN < 4 || COLUMN > 20) {
				cout << "Invalid input, please try again.\n";
				cin.clear();
				cin.ignore(1000, '\n');
				cout << ">";
			}
			else {
				columnFlag = 0;
			}
		}
		cout << "Please enter the number of connection to win.\n";
		cout << ">";
		while (reqToWinFlag) {
			cin >> REQTOWIN;
			if (cin.fail() || REQTOWIN < 3 || REQTOWIN > 20 || (REQTOWIN > ROW && REQTOWIN > COLUMN)) {
				cout << "Invalid input, please try again.\n";
				cin.clear();
				cin.ignore(1000, '\n');
				cout << ">";
			}
			else {
				reqToWinFlag = 0;
			}
		}

		break;
	default:
		ROW = 6;
		COLUMN = 7;
		REQTOWIN = 4;
		break;
	}
	while (gameEnd) {
		system("cls");
		ShowBoard();
		winner = CheckWin();
		if (winner == -1) {
			cout << "Game Over!Player X has won!\n";
			break;
		}
		else if (winner == 1) {
			cout << "Game Over!Player O has won!\n";
			break;
		}
		else if (winner == 777) {
			cout << "Game Over!It is a draw since both players have won at the same time!\n";
			break;
		}
		if (CheckDraw()) {
			cout << "Game Over!This is a draw!\n";
			break;
		}

		player = (turn % 2) * 2 - 1;
		if (player == -1) {
			cout << "This is the Player X`s turn\nPlease enter the column number to insert a piece.\n";
		}
		if (player == 1) {
			cout << "This is the Player O`s turn\nPlease enter the column number to insert a piece.\n";
		}

		if (mode == REMOVE) {
			cout << "Or, Enter \"-\" plus the the column number to remove a piece.\n";
		}

		cout << ">";
		
		while (true) {
			cin >> columnNum;
			if (cin.fail() || !((columnNum >= 1 && columnNum <= COLUMN) || (mode == REMOVE && (columnNum >= -COLUMN && columnNum <= -1)))) {
				cout << "Invalid input, please try again.\n";
				cin.clear();
				cin.ignore(1000, '\n');
				cout << ">";
			}
			else {
				if (columnNum > 0) {
					if (InsertPiece(player, columnNum)) {
						cout << "This column is full, please try again.\n";
						cin.clear();
						cin.ignore(1000, '\n');
						cout << ">";
					}
					else {
						turn++;
						break;
					}
				}
				else if (columnNum < 0) {
					if (RemovePiece(player, -columnNum)) {
						cout << "Cannot remove this piece, please try again.\n";
						cin.clear();
						cin.ignore(1000, '\n');
						cout << ">";
					}
					else {
						turn++;
						break;
					}
				}
				
				
			}
		}
	}

	return 0;
}

int GameControl() {
	char input;
	while (true) {
		if (ShowMainMenu()) {
			return 0;
		}
		InitialBoard();
		GameFunc();
		cout << "Want another Game?(y/n)\n>";		
		while (true) {
			cin >> input;
			if (cin.fail() || (input != 'y' && input != 'Y' && input != 'n' && input != 'N')) {
				cout << "Invalid input, please try again.\n";
				cin.clear();
				cin.ignore(1000, '\n');
				cout << ">";
			}
			else if (input == 'y' || input == 'Y') {
				break;
			}
			else if (input == 'n' || input == 'N') {
				return 0;
			}
		}
	}	
	return 0;
}

int main() {
	GameControl();
	system("cls");
	cout << "This is a simple game produced by Weiran Ye in UCSC GPM project GAME-230.\n"
		<<"Thank you so much for playing my game!\n";
	system("pause");
}
