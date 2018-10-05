#include "stdlib.h"  
#include "windows.h"
#include "iostream"
#include "time.h"   

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

//AI Func
int Eval(int **board, int player);
int AIFlag = 0;
int Min(int depth,int player);
int Max(int depth,int player);
int AIMove(int player);
//

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
		<< "\t5.Play With A.I.(Minimax algorithm)\n"
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
//Return -1 means playerX wins, 1 means playerO wins, 0 means nobody wins, 777 means both win draw(in remove mode)
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
				if (j < 9) {
					cout << " ";
				}
			}
			else {
				if (BOARD[i][j] == 0) {
					cout << ". ";
				}
				if (BOARD[i][j] == -1) {
					cout << "X ";
				}
				if (BOARD[i][j] == 1) {
					cout << "O ";
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
//Return 0 means reset succeed, return 1 means reset failed
int ResetPiece(int column) {
	int depth = 0;
	while (depth <= ROW - 1) {
		if (BOARD[depth][column - 1] != 0) {
			BOARD[depth][column - 1] = 0;
			return 0;
		}
		else {
			depth++;
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
	char input = NULL;
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
	case AI:
		ROW = 6;
		COLUMN = 7;
		REQTOWIN = 4;
		cout << "Do you want to play FIRST or not?(y/n)\n>";		
		while (true) {
			cin >> input;
			if (cin.fail() || (input != 'y' && input != 'Y' && input != 'n' && input != 'N')) {
				cout << "Invalid input, please try again.\n";
				cin.clear();
				cin.ignore(1000, '\n');
				cout << ">";
			}
			else if (input == 'y' || input == 'Y') {
				AIFlag = 1;
				break;
			}
			else if (input == 'n' || input == 'N') {
				AIFlag = -1;
				break;
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
		if (player == -1 && mode == AI) {
			if (AIFlag == 1) {
				cout << "This is the Player`s turn\nPlease enter the column number to insert a piece.\n";
			}
			else if (AIFlag == -1) {
				cout << "Now it is the A.I.`s turn.\n";
				Sleep(1000);
			}
		}
		else if (player == -1) {
			cout << "This is the Player X`s turn\nPlease enter the column number to insert a piece.\n";
		}
		if (player == 1 && mode == AI) {
			if (AIFlag == 1) {
				cout << "Now it is the A.I.`s turn.\n";
				Sleep(1000);
			}
			else if (AIFlag == -1) {
				cout << "This is the Player`s turn\nPlease enter the column number to insert a piece.\n";
			}
		}
		else if (player == 1) {
			cout << "This is the Player O`s turn\nPlease enter the column number to insert a piece.\n";
		}

		if (mode == REMOVE) {
			cout << "Or, Enter \"-\" plus the the column number to remove a piece.\n";
		}

		cout << ">";
		
		while (true) {
			if (mode == AI && player == AIFlag) {
				break;
			}
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
		if (mode == AI && player == AIFlag) {
			int choice = AIMove(AIFlag);
			if (InsertPiece(AIFlag, choice)) {
				cout << "AI choose: " << choice << "!\n";
				cout << "AI is break.\n";
				system("pause");
				return 0;
			}
			else {
				cout << "AI choose: " << choice << "!\n";
				getchar();
				turn++;
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
	srand((unsigned)time(NULL));
	GameControl();
	system("cls");
	cout << "This is a simple game produced by Weiran Ye in UCSC GPM project GAME-230.\n"
		<<"Thank you so much for playing my game!\n";
	system("pause");
}

//AI Part
//For c4, score + 100; For c3, score + 10; For c2, score + 1
int Eval(int **tempBoard, int player) {
	int temp = 0;
	int tempI = 0;
	int tempJ = 0;
	int rest = 0;
	int score = 0;
	int playerXWin = 0;
	int playerOWin = 0;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			if (*((int*)tempBoard + i * 20 + j) != 0) {
				temp = *((int*)tempBoard + i * 20 + j);
				//Check ¡ú win
				if ((j + REQTOWIN) <= COLUMN) {
					tempI = i;
					tempJ = j;
					rest = REQTOWIN;
					while (rest) {
						tempJ++;
						if (*((int*)tempBoard + tempI * 20 + tempJ) != temp) {
							break;
						}
						else
						{
							rest--;
							//c4
							if (rest == 1) {
								if (temp == player) {
									return 10000;
								}
								else {
									return -10000;
								}
							}
							//c3
							else if (rest == 2) {
								if (temp == player) {
									score += 50;
								}
								else {
									score -= 50;
								}
							}
							//c2
							else if (rest == 3) {
								if (temp == player) {
									score += 20;
								}
								else {
									score -= 20;
								}
							}
						}
					}
				}
				//Check ¡û win
				if ((j - REQTOWIN) >= -1) {
					tempI = i;
					tempJ = j;
					rest = REQTOWIN;
					while (rest) {
						tempJ--;
						if (*((int*)tempBoard + tempI * 20 + tempJ) != temp) {
							break;
						}
						else
						{
							rest--;
							//c4
							if (rest == 1) {
								if (temp == player) {
									return 10000;
								}
								else {
									return -10000;
								}
							}
							//c3
							else if (rest == 2) {
								if (temp == player) {
									score += 50;
								}
								else {
									score -= 50;
								}
							}
							//c2
							else if (rest == 3) {
								if (temp == player) {
									score += 20;
								}
								else {
									score -= 20;
								}
							}
						}
					}
				}
				//Check ¡ü win
				if ((i - REQTOWIN) >= -1) {
					tempI = i;
					tempJ = j;
					rest = REQTOWIN;
					while (rest) {
						tempI--;
						if (*((int*)tempBoard + tempI * 20 + tempJ) != temp) {
							break;
						}
						else
						{
							rest--;
							//c4
							if (rest == 1) {
								if (temp == player) {
									return 10000;
								}
								else {
									return -10000;
								}
							}
							//c3
							else if (rest == 2) {
								if (temp == player) {
									score += 90;
								}
								else {
									score -= 90;
								}
							}
							//c2
							else if (rest == 3) {
								if (temp == player) {
									score += 30;
								}
								else {
									score -= 30;
								}
							}
						}
					}
				}

				//Check ¨J win
				if ((j + REQTOWIN) <= COLUMN && (i - REQTOWIN) >= -1) {
					tempI = i;
					tempJ = j;
					rest = REQTOWIN;
					while (rest) {
						tempI--;
						tempJ++;
						if (*((int*)tempBoard + tempI * 20 + tempJ) != temp) {
							break;
						}
						else
						{
							rest--;
							//c4
							if (rest == 1) {
								if (temp == player) {
									return 10000;
								}
								else {
									return -10000;
								}
							}
							//c3
							else if (rest == 2) {
								if (temp == player) {
									score += 50;
								}
								else {
									score -= 50;
								}
							}
							//c2
							else if (rest == 3) {
								if (temp == player) {
									score += 10;
								}
								else {
									score -= 10;
								}
							}
						}
					}
				}
				//Check ¨I win
				if ((j - REQTOWIN) >= -1 && (i + REQTOWIN) >= -1) {
					tempI = i;
					tempJ = j;
					rest = REQTOWIN;
					while (rest) {
						tempI--;
						tempJ--;
						if (*((int*)tempBoard + tempI * 20 + tempJ) != temp) {
							break;
						}
						else
						{
							rest--;
							//c4
							if (rest == 1) {
								if (temp == player) {
									return 10000;
								}
								else {
									return -10000;
								}
							}
							//c3
							else if (rest == 2) {
								if (temp == player) {
									score += 50;
								}
								else {
									score -= 50;
								}
							}
							//c2
							else if (rest == 3) {
								if (temp == player) {
									score += 10;
								}
								else {
									score -= 10;
								}
							}
						}
					}
				}
			}
		}
	}
	return score;
}

int AIMove(int player) {
	int column = -1;
	int bestValue = -10000;
	int value = -10000;
	int depth = 6;
	int choice = -1;
	int choiceList[100] = { -1 };
	int index = -1;
	for (column = 0; column < COLUMN; column++) {
		if (InsertPiece(player, column + 1)) {
			continue;
		}
		else {
			value = Min(depth,player);
			if (value > bestValue) {
				bestValue = value;
				index = 0;
				choiceList[index] = column;
			}
			else if (value == bestValue) {
				index++;
				choiceList[index] = column;
			}
			ResetPiece(column + 1);
		}

	}
	if (index == -1) {
		while (true) {
			column = rand() % COLUMN;
			if (InsertPiece(player, column + 1)) {
				continue;
			}
			else {
				ResetPiece(column + 1);
				choice = column;
				return choice + 1;
			}
		}
	}
	choice = choiceList[rand() % (index + 1) ];
	return choice + 1;
}

int Max(int depth,int player) {
	int eva = Eval((int**)BOARD,player);
	if (depth == 0 || CheckWin()) {
		return eva;
	}
	int value = -10000;
	int bestValue = -10000;
	for (int column = 0; column < COLUMN; column++) {
		if (InsertPiece(player, column + 1)) {
			continue;
		}
		else {
			value = Min(depth - 1, player);
			if (value > bestValue) {
				bestValue = value;
			}
			ResetPiece(column + 1);
		}

	}
	return bestValue;
}

int Min(int depth,int player) {
	int eva = Eval((int**)BOARD, player);
	if (depth == 0 || CheckWin()) {
		return eva;
	}
	int value = 10000;
	int bestValue = 10000;
	for (int column = 0; column < COLUMN; column++) {
		if (InsertPiece(-player, column + 1)) {
			continue;
		}
		else {
			value = Max(depth - 1, player);
			if (value < bestValue) {
				bestValue = value;
			}
			ResetPiece(column + 1);
		}

	}
	return   bestValue;
}
