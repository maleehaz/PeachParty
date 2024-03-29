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
	~StudentWorld() { delete b; }
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	Board* getBoard() const { return b; }
	bool isBlocked(Actor* p, int dir);
	bool atFork(Actor* actor, int dir);
	bool isForkBlocked(Actor* p, int dir);
	Player* getPlayer(int i) const { return player[i - 1]; }
	void depositBank(int coins) { m_bank += coins; }
	void withdrawBank(int& coins) { 
		coins = m_bank;
		m_bank = 0;
	}
	void givePlayerVortex(int num_player);
	Actor* actor_overlap_vortex(Vortex* v);
	void get_random_square(int& x, int& y);
	void drop_square(Baddie* baddie);
	void killActors();
	void addDropSquare(int x, int y);

	// Given a player object pointer, returns a pointer to the other player object. Used for swapping
	// actions.
	Player* get_other_player(Player* p) const {
		int p_num = p->getPlayerNum();
		if (p_num == 1) {
			return getPlayer(2);
		}
		return getPlayer(1);
	}

private:
	std::vector<Actor*> actor;
	std::vector<Player*> player;
	Board* b;
	int m_bank;
};

#endif // STUDENTWORLD_H_
