#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int startX, int startY, int imageId, StudentWorld* world);
	virtual ~Actor();
	virtual bool isBaddie();
	StudentWorld* getWorld();
	int getTicksToMove() { return ticks_to_move; }
	void setTicksToMove(int i) { ticks_to_move = i; }
	void decTick() { ticks_to_move--; }

protected:
	bool validMove(int x, int y);

private:
	int ticks_to_move;
	StudentWorld* m_world;

};

//PLAYER AVATAR
class Player : public Actor
{
public:
	Player(int startX, int startY, int imageId, StudentWorld* world, int p_num);
	void doSomething();

private:
	const int m_player; // player 1 or 2
	int m_stars, m_coins;
	bool WaitingToRoll; // starts out in waiting to roll state
};


//PEACH
class Peach : public Player
{
public:
	Peach(int startX, int startY, StudentWorld* world);

};
//YOSHI
class Yoshi : public Player
{
public:
	Yoshi(int startX, int startY, StudentWorld* world);

};

//SQUARE
class Square : public Actor
{
public:
	Square(int startX, int startY, int imageId, StudentWorld* world);
};

//COIN SQUARE
class CoinSquare : public Square
{
public:
	CoinSquare(int startX, int startY, int imageId, StudentWorld* world);
};

//BLUE COIN SQUARE
class BlueCoinSquare : public CoinSquare
{
public:
	BlueCoinSquare(int startX, int startY, StudentWorld* world);

};

//RED
class RedCoinSquare : public CoinSquare
{
public:
	RedCoinSquare(int startX, int startY, StudentWorld* world);
};

//STAR SQUARE
class StarSquare : public CoinSquare
{
public:
	StarSquare(int startX, int startY, StudentWorld* world);
};

//DIRECTIONAL SQUARE
class DirSquare : public Square
{
public:
	DirSquare(int startX, int startY, StudentWorld* world);
};

//BANK SQUARE
class BankSquare : public CoinSquare
{
public:
	BankSquare(int startX, int startY, StudentWorld* world);
};

//EVENT SQUARE
class EventSquare : public Square
{
public:
	EventSquare(int startX, int startY, StudentWorld* world);
};

//DROPPING SQUARE
class DropSquare : public CoinSquare
{
public:
	DropSquare(int startX, int startY, StudentWorld* world);
};

//BADDIE
class Baddie : public Actor
{
public:
	Baddie(int startX, int startY, int imageId, StudentWorld* world);

};

//BOWSER
class Bowser : public Baddie
{
public:
	Bowser(int startX, int startY, StudentWorld* world);
};

//BOO
class Boo : public Baddie
{
public:
	Boo(int startX, int startY, StudentWorld* world);
};

//VORTEX
class Vortex : public Actor
{
public:
	Vortex(int startX, int startY, StudentWorld* world);
};


#endif // ACTOR_H_
