#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int startX, int startY, int imageId, int depth, StudentWorld* world);
	virtual ~Actor();
	virtual bool isBaddie() { return false; }
	virtual void doSomething() { return; } // switch to = 0 later on after implementing other classes
	StudentWorld* getWorld() { return m_world; }
	int getTicksToMove() const { return ticks_to_move; }
	void setTicksToMove(int i) { ticks_to_move = i; }
	void decTick() { ticks_to_move--; }
	virtual bool isActive() { return false; }
	virtual int getCount() const { return 0; }
	virtual bool isDirSquare() const { return false; }

private:
	int ticks_to_move;
	StudentWorld* m_world;
};

//PLAYER AVATAR
class Player : public Actor
{
public:
	Player(int startX, int startY, int imageId, StudentWorld* world, int p_num);
	virtual void doSomething();
	int getPath() const { return m_path; }
	void setPath(int i) { m_path = i; }
	int getStars() const { return m_stars; }
	void addStars(int i) { m_stars += i; }
	int getCoins() const { return m_coins; }
	int getRoll() const { return die_roll; }
	void addCoins(int i) { m_coins += i; }
	void setIsAffected(bool i) { isAffected = i; }
	bool getIsAffected() { return isAffected; }
	virtual int getCount() const { return count; }
	bool switchedDir() const { return switch_dir; }
	void switchDir(bool i) { switch_dir = i; }


private:
	const int m_player; // player 1 or 2
	int m_stars, m_coins, die_roll, count;
	int m_path;
	bool WaitingToRoll; // starts out in waiting to roll state
	bool isAffected;
	bool switch_dir;

	

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
	virtual bool isActive();
	virtual void doSomething();
	virtual void interactWithPlayer(Player* player) { return; }
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
	virtual void interactWithPlayer(Player* player);
};

//RED
class RedCoinSquare : public CoinSquare
{
public:
	RedCoinSquare(int startX, int startY, StudentWorld* world);
	virtual void interactWithPlayer(Player* player);
};

//STAR SQUARE
class StarSquare : public CoinSquare
{
public:
	StarSquare(int startX, int startY, StudentWorld* world);
	virtual void interactWithPlayer(Player* player);
};

//DIRECTIONAL SQUARE
class DirSquare : public Square
{
public:
	DirSquare(int startX, int startY, StudentWorld* world, int direction);
	virtual void interactWithPlayer(Player* player);
	virtual bool isActive() { return true; }


private:
	const int m_direction; 
};

//BANK SQUARE
class BankSquare : public CoinSquare
{
public:
	BankSquare(int startX, int startY, StudentWorld* world);
	virtual void interactWithPlayer(Player* player);
	virtual bool isActive() { return true; }
	virtual void doSomething();
private:
	bool withdrawn; 
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
