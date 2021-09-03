#pragma once
#include <list>

using namespace std;

template < typename T >

void createMatrix(T**& , int , int );

template < typename T >
void freeMatrix(T**& , int);

struct cell {
	bool l = true;
	bool r = true;
	bool u = true;
	bool d = true;
};

struct position {
	int x;
	int y;
	void set(int , int );
	position(int , int );
};

class Game {

private: int ncol;
	   int nrow;
	   position curr;
	   cell** maze;

public:Game(int , int );

	  void geM();

	  void mazePrint();

	  void generateMaze(int , int );

	  bool recBack(int** , int , int , list <position>& );

	  void GivingUp();

	  void Play(); 
}; 
bool isBetween(int , int );	  
bool isCellEmpty(cell&);

