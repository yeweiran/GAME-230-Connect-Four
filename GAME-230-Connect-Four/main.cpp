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
	//TODO
	return 0;
}

int ShowMainMenu() {
	int modeNum;
	
	int modeFlag = 1;
	cout << "This is a Connect Four game.\nPlease choose the game mode you want to play.\n"
		<< "\t1.Simple mode\n"
		<< "\t2.Customized Board\n"
		<< "\t3.Wrap Around\n"
		<< "\t4.Remove Enable\n"
		<< "\t5.Play With A.I.\n"
		<< ">";
	
	while (modeFlag) {
		cin >> modeNum;
		if (cin.fail() || modeNum > 5 || modeNum < 1) {
			cout << "Invalid input, please try again.\n";
			//fflush(stdin);
			cin.clear();
			//std::cin.sync();
			cin.ignore(1000, '\n');
			cout << ">";
		}
		else {
			//std::cout << modeNum;
			modeFlag = 0;
			mode = (GAMEMODE)(modeNum - 1);
			
		}
	}
	
	
	system("cls");
	return 0;
}

int CheckWin() {
	//TODO
	return 0;
}

int CheckDraw() {
	//TODO
	return 0;
}

int GameFunc() {
	InitialBoard();
	int rowFlag = 1;
	int columnFlag = 1;
	int reqToWinFlag = 1;
	int turn = 0;
	int gameEnd = 1;
	switch (mode)
	{
	case SIMPLE:
		ROW = 6;
		COLUMN = 7;
		REQTOWIN = 4;
		break;
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
			if (cin.fail() || REQTOWIN < 3 || REQTOWIN > 20) {
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
	case WRAPAROUND:
		ROW = 6;
		COLUMN = 7;
		REQTOWIN = 4;
		break;
	case REMOVE:
		ROW = 6;
		COLUMN = 7;
		REQTOWIN = 4;
		break;
	case AI:
		ROW = 6;
		COLUMN = 7;
		REQTOWIN = 4;
		break;
	default:
		break;
	}

	//cout << "ROW:" << ROW << "\nCOLUMN:" << COLUMN << "\nRTW:" << REQTOWIN << endl;
	while (gameEnd) {
		if (CheckWin()) {
			//TODO
		}
		if (CheckDraw()) {

		}



	}
	return 0;
}

int GameControl() {

	ShowMainMenu();
	GameFunc();

	return 0;
}

int main() {
	
	GameControl();
	//ShowMainMenu();

	//InitialBoard();
	//cout << mode;
	system("pause");
}
