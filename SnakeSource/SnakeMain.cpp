
/*define function include
function ShowMenu : options for user choose(map,speed,more changelle) & play & show hight score
function RunSnake : play game & check flag of status game
function Intial   : setup attribute width  height , random food,with snake start at middle of map
function Draw     : create map vs snake + food for user interface with it
function InputKey : recognize command line from user & snake follow order in next function
function Logic	   : will follow order from InputKey & snake go to user's direction
& examine have snake found food yet? Y: score increase N:nothing still play
didn't snake hit map? Y:game over N:still play
score will follow rules of snake game (100 points / 1 food)
function Defeat   : will show your score & Have you had score hight at rank?
Y:save & your'name N:get better again Or maybe dont need
after that show menu again
*/


#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <cstdlib>
#include "ClassSnake.h"
using namespace std;
enum Direction { STOP, LEFT, RIGHT, UP, DOWN };
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // For use of SetConsoleTextAttribute()
#define		wdefault	10
#define		hdefault	6
#define		NEWGAME		8
#define		HIGHSCORE	9
#define		SPEED		10
#define		CONTROL		11
#define		EXIT		12
#define		ARROW		42
#define		PONITER		43
#define		TEXTS		44
//define function
void RunSnack();
void Inital();
void DrawMap();
void Update();
void FindWhere(int i);
void InputKey();
void Logic();
void IsDefeat();
void ShowMenu();
void CheckRules();
void gotoxy(SHORT x,SHORT y);
void Test();
void ShowLocationS();
void ControlMenu();
// define variable type
struct Location
{
	int w;
	int h;
};

//define attribute
int numberSnake = 2;
bool statusGame = true;
const int width = 45;
const int height = 23;
int wLocatedSnack, hLocatedSnack, wLocatedFood, hLocatedFood;
Direction dirSnake = STOP;
ClassSnake Snake= ClassSnake(width / 2 -1, height / 2-1);
int ponitOfScore = 0;
Direction oldDirSnake = STOP;
Location newplace;
bool IsChangeTail ;

/*  x = width : y = height */ 
void gotoxy(SHORT x,SHORT y )
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}  

void Inital() {
	wLocatedSnack = width / 2 -1;
	hLocatedSnack = height / 2 -1;
	Snake.AddTail(wLocatedSnack+ 1, hLocatedSnack);
	Snake.AddTail(wLocatedSnack + 2, hLocatedSnack);
	Snake.UpdateOldTailS();
	Snake.isEat = false;
	do
	{
		srand(static_cast<unsigned int>(time(NULL)));
		wLocatedFood = rand() % (width - 1);
		srand(static_cast<unsigned int>(time(NULL)));
		hLocatedFood = rand() % (height - 1);
	} while (Snake.IsElementS(wLocatedFood, hLocatedFood));

}

void DrawMap() {
	system("cls");
	gotoxy(wdefault+width/2 -5,2);
	cout << "Score:" << ponitOfScore;
	cout << endl<<endl;
	{
		for (int i = 0; i < width + 2; i++)
		{
			if (0 == i) { cout << "\t"; }cout << "*";
		}
		cout << endl;
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width + 2; ++j)
			{
				if (0 == j || (width + 2) - 1 == j)
				{
					if (0 == j) cout << "\t";
					cout << "*";
				}
				else  cout << " ";

			}
			cout << endl;
		}
		for (int i = 0; i < width + 2; i++)
		{
			if (0 == i) { cout << "\t"; }
			cout << "*";
		}
	}
	int resultTail = Snake.TailIs();  // return id allow know TAIL's location
	for (int i = 0; i <= resultTail; ++i)
	{
		FindWhere(i);   // goto location element i of Snake
			switch (i)
			{
			case 0:
				cout << "<";
				break;
			default:
				cout << "=";
				break;
			}
	}
	SHORT w = wdefault + wLocatedFood - 1;
	SHORT h = hdefault + hLocatedFood - 1;
	gotoxy(w, h);
	cout << "@";
	gotoxy(0, 0);
	
}

void Update()
{
	if (dirSnake!=STOP)
	{
		if (!(IsChangeTail))
		{
			SHORT w = wdefault + Snake.oldTailLocatedS.wL -1;
			SHORT h = hdefault + Snake.oldTailLocatedS.hL -1;
			gotoxy(w, h);
			cout << " ";
			IsChangeTail = false;
		}
		FindWhere(0); // update HEAD   0 access HEAD
		cout << "<";
		FindWhere(1);// update behind HEAD  1 access element behind HEAD
		cout << "=";
		gotoxy(0, 0);
	}
	
}

/*goto location element i of Snake */
void FindWhere(int i)
{
	// coz run located at 0,0
	SHORT w = wdefault + Snake.elementS[i].wL -1;
	SHORT h = hdefault + Snake.elementS[i].hL -1;
	gotoxy(w, h);
}

void InputKey() {
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'a':
			
			if (!(dirSnake == RIGHT) ) {
				
				dirSnake = LEFT;
				oldDirSnake = dirSnake;
			}
			break;

		case 'd':
			if (!(dirSnake == LEFT)) {
				if (dirSnake == STOP && oldDirSnake == STOP)   break;
				dirSnake = RIGHT;
				oldDirSnake = dirSnake;
			}
			break;

		case 'w':
			if (!(dirSnake == DOWN)) {
				dirSnake = UP;
				oldDirSnake = dirSnake;
			}
			break;

		case 's':
			if (!(dirSnake == UP)) {
				dirSnake = DOWN;
				oldDirSnake = dirSnake;
			}
			break;

		case 'p':
			dirSnake = STOP;
			oldDirSnake = dirSnake;
			break;
		case 27://ESC
			statusGame = false;
			break;
		default:

			break;
		}
	}

}

void Logic() {
	switch (dirSnake)
	{
	case STOP:

		break;

	case LEFT:
		--wLocatedSnack;
		newplace = { wLocatedSnack,hLocatedSnack };
		CheckRules();
		break;

	case RIGHT:
		++wLocatedSnack;
		newplace = { wLocatedSnack,hLocatedSnack };
		CheckRules();
		break;

	case UP:
		--hLocatedSnack;
		newplace = { wLocatedSnack,hLocatedSnack };
		CheckRules();
		break;

	case DOWN:
		++hLocatedSnack;
		newplace = { wLocatedSnack,hLocatedSnack };
		CheckRules();
		break;

	default:
		break;
	}
	if (Snake.IsElementS(wLocatedFood,hLocatedFood))
	{
		
		Snake.isEat = true;
		++Snake.amoutOfFood;
		Snake.isWhere[Snake.amoutOfFood].ID = Snake.TailIs();
		ponitOfScore += 1;
		gotoxy(wdefault + width / 2 -5, 2);
		cout << "Score:" << ponitOfScore;
		do
		{
			srand(static_cast<unsigned int>(time(NULL)));
			wLocatedFood = rand() % (width-1);
			srand(static_cast<unsigned int>(time(NULL)));
			hLocatedFood = rand() % (height-1);
		} while (Snake.IsElementS(wLocatedFood,hLocatedFood));
		SHORT w = wdefault + wLocatedFood - 1;
		SHORT h = hdefault + hLocatedFood - 1;
		gotoxy(w, h);
		cout << "@";
		
	}
	if (Snake.CheckFood())
	{
		Snake.AddTail(Snake.isWhere[1].wL, Snake.isWhere[1].hL);
		IsChangeTail = false;
	}

}

void IsDefeat() {
	system("cls");
	gotoxy(25, 15);
	cout << "GAME OVER";
	Snake.~ClassSnake();
	Sleep(500);
	
}   

void CheckRules() {
	if (wLocatedSnack > width-1    || 
		wLocatedSnack < 0          ||
		hLocatedSnack > height-1   ||
		hLocatedSnack < 0		   || !(Snake.RunSnake(newplace.w, newplace.h)))
	{
		statusGame = false;
	}
	else if (Snake.RunSnake(newplace.w, newplace.h))
	{
		IsChangeTail=true;
	}
}

/*Show menu options for user*/
void ShowMenu() {
	/*HANDLE cons = GetStdHandle(STD_OUTPUT_HANDLE);
	PCONSOLE_FONT_INFOEX font = new CONSOLE_FONT_INFOEX();
	font->cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(cons, 0, font);
	font->dwFontSize.X = 18;
	font->dwFontSize.Y = 18;
	font->FontWeight = 16;
	font->FontFamily = FF_DECORATIVE;
	SetCurrentConsoleFontEx(cons, 0, font);*/

	SetConsoleTextAttribute(console, 241);
	system("cls"); //clrscr(); //clear the console
				   //Ascii art reference: http://www.chris.com/ascii/index.php?art=animals/reptiles/snakes
	printf("\n\n\n");
	printf("\t    _________         _________ 			\n");
	printf("\t   /         \\       /         \\ 			\n");
	printf("\t  /  /~~~~~\\  \\     /  /~~~~~\\  \\ 			\n");
	printf("\t  |  |     |  |     |  |     |  | 			\n");
	printf("\t  |  |     |  |     |  |     |  | 			\n");
	printf("\t  |  |     |  |     |  |     |  |         /	\n");
	printf("\t  |  |     |  |     |  |     |  |       //	\n");
	printf("\t (o  o)    \\  \\_____/  /     \\  \\_____/ / 	\n");
	printf("\t  \\__/      \\         /       \\        / 	\n");
	printf("\t    |        ~~~~~~~~~         ~~~~~~~~ 		\n");
	printf("\t    ^											\n");
	printf("\t    Welcome To The Snake Game!			\n");
	printf("\t    Press Any Key To Continue...	\n");
	printf("\n");
	_getch();
	system("cls");
	SetConsoleTextAttribute(console, 250);
	cout << endl;
	printf("           ---_ ...... _/_ -     \n");
	printf("          /  . .    ./ .'*\\ \\     \n");
	printf("          : '_-        /__-'  \\.  \n");
	printf("         /                      ) \n");
	printf("       _/                  >   .'  \n");
	printf("     /   .   .       _.-\" / .'    \n");
	printf("     \\           __/\" / .'/|    \n");
	printf("       \\ '--  .-\" /     //' |\\|   \n");
	printf("        \\|  \\ | /     //_ _ |/| \n");
	printf("         `.  \\:     //|_ _ _|\\| CODER \n");
	printf("         | \\/.    //  | _ _ |/| SHEKCON \n");
	printf("          \\_ | \\/ /    \\ _ _ \\\\\\  \n");
	printf("              \\__/      \\ _ _ \\|\\ \n");
	ControlMenu();
}

void ControlMenu() {
	SetConsoleTextAttribute(console, 251);
	gotoxy(38, 4);				printf("Snake Game");
	SetConsoleTextAttribute(console, 241);
	gotoxy(ARROW,   NEWGAME);	printf("> NEW GAME");
	SetConsoleTextAttribute(console, 240);
	gotoxy(ARROW,   HIGHSCORE); printf("  HIGH SCORE");
	gotoxy(ARROW,   SPEED);     printf("  SPEED");
	gotoxy(ARROW,   CONTROL);	printf("  CONTROL");
	gotoxy(ARROW,   EXIT);		printf("  EXIT");
	gotoxy(PONITER, NEWGAME);
	SHORT where = 8;
	do
	{
		if(_kbhit())
		{
			switch (_getch())
			{
			case 'w':
			{
				if (where == 8)
				{

					gotoxy(ARROW, NEWGAME); cout << "  NEW GAME";
					SetConsoleTextAttribute(console, 241);
					gotoxy(ARROW, EXIT); cout << "> EXIT";
					gotoxy(PONITER, EXIT);
					where = 12;
					SetConsoleTextAttribute(console, 240);
					break;
				}
				--where;

				if (where == 11)
				{

					gotoxy(ARROW, EXIT); cout << "  EXIT";
					SetConsoleTextAttribute(console, 241);
					gotoxy(ARROW, CONTROL); cout << "> CONTROL";
					gotoxy(PONITER, CONTROL);
					SetConsoleTextAttribute(console, 240);
					break;
				}
				if (where == 10)
				{

					gotoxy(ARROW, CONTROL); cout << "  CONTROL";
					SetConsoleTextAttribute(console, 241);
					gotoxy(ARROW, SPEED); cout << "> SPEED";
					gotoxy(PONITER, SPEED);
					SetConsoleTextAttribute(console, 240);
					break;
				}
				if (where == 9)
				{

					gotoxy(ARROW, SPEED); cout << "  SPEED";
					SetConsoleTextAttribute(console, 241);
					gotoxy(ARROW, HIGHSCORE); cout << "> HIGH SCORE";
					gotoxy(PONITER, HIGHSCORE);
					SetConsoleTextAttribute(console, 240);
					break;
				}
				if (where == 8)
				{

					gotoxy(ARROW, HIGHSCORE); cout << "  HIGH SCORE";
					SetConsoleTextAttribute(console, 241);
					gotoxy(ARROW, NEWGAME); cout << "> NEW GAME";
					gotoxy(PONITER, NEWGAME);
					SetConsoleTextAttribute(console, 240);
					break;
				}

			}
			case 's':
			{
				if (where == 12)
				{

					gotoxy(ARROW, EXIT); cout << "  EXIT";
					SetConsoleTextAttribute(console, 241);
					gotoxy(ARROW, NEWGAME); cout << "> NEW GAME";
					gotoxy(PONITER, NEWGAME);
					where = 8;
					SetConsoleTextAttribute(console, 240);
					break;
				}
				++where;

				if (where == 9)
				{

					gotoxy(ARROW, NEWGAME); cout << "  NEW GAME";
					SetConsoleTextAttribute(console, 241);
					gotoxy(ARROW, HIGHSCORE); cout << "> HIGH SCORE";
					gotoxy(PONITER, HIGHSCORE);
					SetConsoleTextAttribute(console, 240);
					break;
				}
				if (where == 10)
				{

					gotoxy(ARROW, HIGHSCORE); cout << "  HIGH SCORE";
					SetConsoleTextAttribute(console, 241);
					gotoxy(ARROW, SPEED); cout << "> SPEED";
					gotoxy(PONITER, SPEED);
					SetConsoleTextAttribute(console, 240);
					break;
				}
				if (where == 11)
				{

					gotoxy(ARROW, SPEED); cout << "  SPEED";
					SetConsoleTextAttribute(console, 241);
					gotoxy(ARROW, CONTROL); cout << "> CONTROL";
					gotoxy(PONITER, CONTROL);
					SetConsoleTextAttribute(console, 240);
					break;
				}
				if (where == 12)
				{

					gotoxy(ARROW, CONTROL); cout << "  CONTROL";
					SetConsoleTextAttribute(console, 241);
					gotoxy(ARROW, EXIT); cout << "> EXIT";
					gotoxy(PONITER, EXIT);
					SetConsoleTextAttribute(console, 240);
					break;
				}
				break;
			}

			case 'p':
			
				break;
			case 27://ESC
				ControlMenu();
				break;
			case 13:
				Inital();
				RunSnack();
				break;
			default:

				break;
			}
		}
	} while (statusGame);
	_getch();
}

void RunSnack() {
	DrawMap();
	do
	{
		Update();
		InputKey();
		Logic();
		//Test();
		ShowLocationS();
		Sleep(220);
	} while (statusGame);
}

int main() {
	ShowMenu();
	IsDefeat();
	_getch();
	return 0;
}

void Test() {
	gotoxy(2, hdefault + height/2 );
	cout << "wLocatedSnack < 0 :" << wLocatedSnack;
	if (wLocatedSnack < 0)
	{
		cout << " right";
	}
	else cout << " wrong";
	cout << endl;
	cout << "wLocatedSnack > width-1 :" << wLocatedSnack <<" "<< width -1;
	if (wLocatedSnack > width-2)
	{
		cout << " right";
	}
	else cout << " wrong";
	cout << endl;
	cout << "hLocatedSnack < 0 :" << hLocatedSnack;
	if (hLocatedSnack < 0)
	{
		cout << " right";
	}
	else cout << " wrong";
	cout << endl;
	cout << "hLocatedSnack > height-1 :" << hLocatedSnack << " " << height - 1;
	if (hLocatedSnack > height - 1)
	{
		cout << " right";
	}
	else cout << " wrong";
}

void ShowLocationS() {
	gotoxy(10, hdefault + height); 
	cout << hLocatedSnack +1 <<":" << wLocatedSnack +1 <<"    ";
}
