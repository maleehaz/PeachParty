#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class Vortex;

class Actor : public GraphObject
{
public:
	Actor(int startX, int startY, int imageId, int depth, StudentWorld* world);
	virtual bool isBaddie() { return false; }
	virtual void doSomething() { return; } // switch to = 0 later on after implementing other classes
	StudentWorld* getWorld() { return m_world; }
	int getTicksToMove() const { return ticks_to_move; }
	void setTicksToMove(int i) { ticks_to_move = i; }
	void decTick() { ticks_to_move--; }
	virtual bool isActive() { return false; }
	virtual int getCount() const { return 0; }
	virtual bool isDirSquare() const { return false; }
	virtual bool isImpactedByVortex() const { return false; }
	virtual bool Kill() { return false; }
	virtual void setKill(bool k) { return; }
	virtual void setPath(int i) { return; }
	virtual bool isSquare() { return false; }


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
	void setStars(int i) { m_stars = i; }
	int getCoins() const { return m_coins; }
	int getRoll() const { return die_roll; }
	void setRoll(int i) { die_roll = i; }
	void addCoins(int i) { m_coins += i; }
	void setCoins(int i) { m_coins = i; }
	void setIsAffected(bool i) { isAffected = i; }
	bool getIsAffected() { return isAffected; }
	virtual int getCount() const { return count; }
	bool switchedDir() const { return switch_dir; }
	void switchDir(bool i) { switch_dir = i; }
	int getPlayerNum() { return m_player; }
	void swapPositions(Player* other) {
		int i = other->getTicksToMove();
		other->setTicksToMove(this->getTicksToMove());
		this->setTicksToMove(i);
		int newDir = other->getDirection();
		other->setDirection(this->getDirection());
		this->setDirection(newDir);
		int newPath = other->getPath();
		other->setPath(this->getPath());
		this->setPath(newPath);
		int roll = other->getRoll();
		other->setRoll(this->getRoll());
		this->setRoll(roll);
		int newX = other->getX();
		int newY = other->getY();
		other->moveTo(this->getX(), this->getY());
		this->moveTo(newX, newY);
	}
	bool hasVortex() { return m_vortex != NULL; }
	void setVortex(Vortex* vortex) {
		vortex_notif = " VOR";
		m_vortex = vortex; 
	}
	void shootVortex(Vortex* vortex);
	bool isWaitingToRoll() const { return WaitingToRoll; }
	virtual void setAttackedByBaddie(bool i) { attacked = i; }
	virtual bool isAttacked() { return attacked; }

	void swapCoins(Player * other) {
		int newCoins = other->getCoins();
		other->setCoins(this->getCoins());
		this->setCoins(newCoins);
	}
	void swapStars(Player * other) {
		int newStars = other->getStars();
		other->setStars(this->getStars());
		this->setStars(newStars);
	}

	std::string getText() { return m_msg; }
	void updateText();
	void updateVortexMsg();

private:
	const int m_player; // player 1 or 2
	int m_stars, m_coins, die_roll, count;
	int m_path;
	bool WaitingToRoll; // starts out in waiting to roll state
	bool isAffected;
	bool switch_dir;
	bool attacked;
	std::string m_msg;
	std::string vortex_notif;
	Vortex* m_vortex;

	

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
	virtual bool Kill() { return must_be_killed; }
	virtual void setKill(bool k) { must_be_killed = k; }
	virtual bool isSquare() { return true; }

private:
	bool must_be_killed;
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
	virtual bool isSquare() { return false; }


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
	virtual void interactWithPlayer(Player* player);
	void option1(Player* p);
	void option2(Player * p);
	void option3(Player* p);
};

//DROPPING SQUARE
class DropSquare : public CoinSquare
{
public:
	DropSquare(int startX, int startY, StudentWorld* world);
	virtual void interactWithPlayer(Player* player);
};

//BADDIE
class Baddie : public Actor
{
public:
	Baddie(int startX, int startY, int imageId, StudentWorld* world);
	virtual bool isImpactedByVortex() const { return true; }
	virtual void doSomething();
	int getPath() const { return m_path; }
	int getCount() const { return m_count; }
	void setPath(int i) { m_path = i; }
	bool isPaused() const { return paused; }
	void setPause(bool i) { paused = i; }
	void dec_pause() { pause_count--; }
	void reset_pause() { pause_count = 180; }
	virtual void interactWithPlayer(Player* p) { return; }
	virtual bool isActive();
	virtual bool switchedDir() { return switch_dir; }
	virtual void switchDir(bool i) { switch_dir = i; }
	virtual void whenZeroTicks() { return; }

private:
	int m_path, m_count, pause_count;
	bool switch_dir;
	bool paused;

};

//BOWSER
class Bowser : public Baddie
{
public:
	Bowser(int startX, int startY, StudentWorld* world);
	virtual void interactWithPlayer(Player* p);
	virtual void whenZeroTicks();
};

//BOO
class Boo : public Baddie
{
public:
	Boo(int startX, int startY, StudentWorld* world);
	virtual void interactWithPlayer(Player* p);
};

//VORTEX
class Vortex : public Square
{
public:
	Vortex(int startX, int startY, StudentWorld* world);
	virtual bool isActive() { return is_active; }
	void shoot(Player* p);
	virtual void doSomething();
	virtual void interactWithBaddie();
	virtual bool isSquare() { return false; }

private:
	bool is_active;
	bool is_moving;
};


#endif // ACTOR_H_
