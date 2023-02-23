#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	
	Board* getBoard() const { return b; }
	bool isBlocked(Player* player);
	

private:
	std::vector<Actor*> actor;
	std::vector<Player*> player;
	Board* b;
};

#endif // STUDENTWORLD_H_
