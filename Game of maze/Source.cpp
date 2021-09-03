#include <iostream>
#include <iomanip>
#include "conio.h"
#include "Console.h"
#include "stdafx.h"
#include <algorithm>
#include <cstdlib>
#include <list>
#include <iterator>
#include "Header.h"

using namespace std;

template < typename T >

void createMatrix(T**& arr, int nrow, int ncol)
{
	arr = new T * [nrow];						 //Выделяем новую память
	for (int i = 0; i < nrow; ++i)
		arr[i] = new T[ncol];
}

template < typename T >
void freeMatrix(T**& arr, int nrow) {
	for (int i = 0; i < nrow; ++i) delete[]arr[i]; //удаляем 
	delete[]arr;
	arr = nullptr;
}

void position::set(int a, int b) {
	x = a; y = b;
}

position::position(int a = 0, int b = 0) {
	x = a;
	y = b;
}



Game::Game(int x1, int y1) {
	ncol = x1;
	nrow = y1;
	createMatrix(maze, ncol, nrow);
	geM();
}


void Game::generateMaze(int cx, int cy) {	//сам лабиринт генерируется случайными путями
	int arr[] = { 1, 2, 3, 4 }, SIZE = sizeof(arr) / sizeof(*arr);
	random_shuffle(arr, arr + SIZE);		// перемешиваем случайно 4 направления
	for (int i = 0; i < 4; i++) {
		int nx, ny;
		switch (arr[i]) {
		case 1: nx = cx;  ny = cy - 1;		//если хотим двигаться вверх
			if (isBetween(ny, nrow) && isCellEmpty(maze[nx][ny])) {		// проверяем, можно ли туда и не были ли мы уже там
				maze[cx][cy].u = 0;
				maze[nx][ny].d = 0;
				generateMaze(nx, ny);			//смещаемся и снова начинаем из той клетки
			} break;
		case 2: nx = cx;  ny = cy + 1;		//если хотим двигаться вниз
			if (isBetween(ny, nrow) && isCellEmpty(maze[nx][ny])) {
				maze[cx][cy].d = 0;
				maze[nx][ny].u = 0;
				generateMaze(nx, ny);
			}  break;
		case 3: nx = cx + 1;  ny = cy;		//если хотим двигаться вправо
			if (isBetween(nx, ncol) && isCellEmpty(maze[nx][ny])) {
				maze[cx][cy].r = 0;
				maze[nx][ny].l = 0;
				generateMaze(nx, ny);
			} break;
		case 4: nx = cx - 1; ny = cy;		//если хотим двигаться влево
			if (isBetween(nx, ncol) && isCellEmpty(maze[nx][ny])) {
				maze[cx][cy].l = 0;
				maze[nx][ny].r = 0;
				generateMaze(nx, ny);
			} break;
		}
	}
}

void Game::geM()
{
	generateMaze(0, 0);
	for (size_t i = 0; i < nrow + ncol; ++i)
	{
		int x = rand() % (ncol - 1) + 1;
		int y = rand() % (nrow - 1) + 1;
		int direction = rand() % 4;
		switch (direction) {
		case 1:maze[x][y].d = 0;  maze[x][y + 1].u = 0; break;
		case 2:maze[x][y].u = 0;  maze[x][y - 1].d = 0; break;
		case 3:maze[x][y].l = 0;  maze[x - 1][y].r = 0; break;
		case 4:maze[x][y].r = 0;  maze[x + 1][y].l = 0; break;
		}
	}
}

void Game::mazePrint() {
	for (int i = 0; i < nrow; ++i) {
		// рисуем верхнюю линию
		for (int j = 0; j < ncol; ++j) {
			cout << (maze[j][i].u ? "+-" : "+ ");
		}
		cout << "+\n";
		// рисуем нижнюю линию
		for (int j = 0; j < ncol; ++j) {
			cout << (maze[j][i].l ? "| " : "  ");
		}
		cout << ("|\n");
	}
	// рисуем последнюю линию
	for (int j = 0; j < ncol; ++j) {
		cout << ("+-");
	}
	cout << ("+");
}

bool Game::recBack(int** arr, int x, int y, list <position>& path) {	//собственно поиск пути
	if (x == (nrow - 1) && y == (ncol - 1))
		return true;

	if ((x >= 1) && !maze[y][x - 1].d && !arr[x - 1][y]) {	//если можем сместиться влево, там нет стенки и мы там еще не были
		arr[x][y] = 1;
		bool id = arr[x - 1][y] = recBack(arr, x - 1, y, path);
		if (id) {								//если все хорошо и путь найден, то добавляем позицию в список
			path.push_back(position(y, x - 1));
			return 1;
		}
	}			//аналогично с остальными направлениями
	if (((x + 1) < nrow) && !maze[y][x + 1].u && !arr[x + 1][y]) {
		arr[x][y] = 1;
		bool id = arr[x + 1][y] = recBack(arr, x + 1, y, path);
		if (id) {
			path.push_back(position(y, x + 1));
			return 1;
		}
	}
	if (((y + 1) < ncol) && !maze[y + 1][x].l && !arr[x][y + 1]) {
		arr[x][y] = 1;
		bool id = arr[x][y + 1] = recBack(arr, x, y + 1, path);
		if (id) {
			path.push_back(position(y + 1, x));
			return 1;
		}
	}
	if ((y >= 1) && !maze[y - 1][x].r && !arr[x][y - 1]) {
		arr[x][y] = 1;
		bool id = arr[x][y - 1] = recBack(arr, x, y - 1, path);
		if (id) {
			path.push_back(position(y - 1, x));
			return 1;
		}
	}
	return 0;
}

bool isCellEmpty(cell& c) {
	return c.u & c.d & c.l & c.r;
}


void Game::GivingUp() {
	int** arr;
	createMatrix(arr, nrow, ncol);
	for (int i = 0; i < nrow; ++i)
		for (int j = 0; j < ncol; ++j)
			arr[i][j] = 0;
	list <position> listOfPos;				//список для сохранения пути
	listOfPos.push_back(position(nrow - 1, ncol - 1));
	recBack(arr, 0, 0, listOfPos);
	listOfPos.push_back(position(0, 0));
	COORD cr = { 1, 1 };					//начинаем вывод пути
	HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	FlushConsoleInputBuffer(hConsole);
	SetConsoleTextAttribute(hConsole, 192);
	SetConsoleCursorPosition(hConsole, cr);
	list <position> ::iterator it1 = --listOfPos.end(), it2 = --listOfPos.end();
	for (--it1; it1 != listOfPos.begin(); --it1, --it2) {	//бегаем по итераторам, смотрим, в какую сторону нужно сместиться
		if (((*it1).x - (*it2).x) == 1) {
			cr.X = cr.X + 2;
			cout << "  ";
		}
		if (((*it1).y - (*it2).y) == 1) {
			for (int i = 0; i < 2; ++i) {
				cr.Y++;
				cout << " ";
				SetConsoleCursorPosition(hConsole, cr);
			}
		}
		if (((*it1).x - (*it2).x) == -1) {
			for (int i = 0; i < 2; ++i) {
				cr.X--;
				cout << " ";
				SetConsoleCursorPosition(hConsole, cr);
			}
		}
		if (((*it1).y - (*it2).y) == -1) {
			for (int i = 0; i < 2; ++i) {
				cr.Y--;
				cout << " ";
				SetConsoleCursorPosition(hConsole, cr);
			}
		}
	}
	SetConsoleTextAttribute(hConsole, 15);
}

void Game::Play()
{
	this->mazePrint();
	int k1, k2;
	COORD cr = { 1, 1 };
	HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole, cr);
	for (;;)			//играем, пока не выиграем или не сдадимся
	{
		k2 = _getch();
		switch (k2)
		{
		case 75:					//если нажата кнопка влево
			if (curr.x > 0 && !maze[curr.x - 1][curr.y].r)
			{
				cr.X = cr.X - 2;
				curr.set(curr.x - 1, curr.y);
				SetConsoleCursorPosition(hConsole, cr);
			}
			else cout << "\a";
			break;
		case 77:					//если нажата кнопка вправо
			if (curr.x + 1 < ncol && !maze[curr.x + 1][curr.y].l)
			{
				curr.set(curr.x + 1, curr.y);
				cr.X = cr.X + 2;
				SetConsoleCursorPosition(hConsole, cr);

			}
			else cout << "\a";
			break;
		case 72:					//если нажата кнопка вверх	
			if (curr.y > 0 && !maze[curr.x][curr.y - 1].d)
			{
				curr.set(curr.x, curr.y - 1);
				cr.Y = cr.Y - 2;
				SetConsoleCursorPosition(hConsole, cr);

			}
			else cout << "\a";
			break;
		case 80:						//если нажата кнопка вниз	
			if (curr.y + 1 < nrow && !maze[curr.x][curr.y + 1].u)
			{
				curr.set(curr.x, curr.y + 1);
				cr.Y = cr.Y + 2;
				SetConsoleCursorPosition(hConsole, cr);
			}
			else cout << "\a";
			break;
		case 68:					//если нажата F10
			GivingUp();
			cout << "\n\nYOU LOSER!";
			return;
		}
		if (curr.x == ncol - 1 && curr.y == nrow - 1)
			break;
	}
	cout << "\n\nYou win! ";
	freeMatrix(maze, ncol);
}

bool isBetween(int lower, int upper) {		// больше нуля и меньше второго
	return (lower >= 0) && (lower < upper);
}