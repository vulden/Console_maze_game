#include <iostream>
#include <iomanip>
#include "Header.h"

using namespace std;

int main()
{
	srand(time(NULL));
	int x, y;
	cout << "----------------THE GAME OF MAZE-------------------\n\nPlease enter the number of rows and columns of maze\n";
	cin >> x >> y;
	system("cls");
	Game maze = Game(x, y);
	maze.Play();
}