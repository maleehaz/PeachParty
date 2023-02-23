#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//ACTOR -> pure virtual
Actor::Actor(int startX, int startY, int imageId, StudentWorld* world) :
	GraphObject(imageId, startX, startY), ticks_to_move(0)
{
	m_world = world;
}

Actor::~Actor() {
	delete m_world;
}

bool Actor::isBaddie() { return false; }

StudentWorld* Actor::getWorld() { return m_world; }

bool Actor::validMove(int x, int y) {
	if (x < 0 || y < 0) { return false; }
	Board::GridEntry ge = getWorld()->getBoard()->getContentsOf(x, y);
	switch (ge) {
	case Board::empty:
		return false;
	default:
		break;
	}
	return true;
}


//PLAYER AVATAR -> pure virtual
Player::Player(int startX, int startY, int imageId, StudentWorld* world, int p_num) :
	Actor(startX, startY, imageId, world),
	m_player(p_num), m_stars(0), m_coins(0), WaitingToRoll(true)
{}

void Player::doSomething() {

	int x = 16;
	int die_roll;

	if (WaitingToRoll) {
		switch (getWorld()->getAction(m_player)) {
		case ACTION_NONE:
			return;
		case ACTION_ROLL:
			die_roll = randInt(1,10);
			setTicksToMove(die_roll * 8);
			WaitingToRoll = false;
			break;
		case ACTION_FIRE:
			return;
		default:
			return;
		}
	}

	int aX = getX();
	int aY = getY();

	if (!WaitingToRoll) {
		if (getTicksToMove() != 0) {
			switch (getWorld()->getAction(m_player)) {
			case ACTION_RIGHT:
				setDirection(right);
				aX++;
				if (!getWorld()->isBlocked(this)) {
					moveTo(aX, aY);
					decTick();
				}
				break;
			case ACTION_LEFT:
				setDirection(left);
				aX--;
				if (!getWorld()->isBlocked(this)) {
					moveTo(aX, aY);
					decTick();
				}
				break;
			case ACTION_UP:
				setDirection(up);
				aY++;
				if (!getWorld()->isBlocked(this)) {
					moveTo(aX, aY);
					decTick();
				}
				break;
			case ACTION_DOWN:
				setDirection(down);
				aY--;
				if (!getWorld()->isBlocked(this)) {
					moveTo(aX, aY);
					decTick();
				}
				break;
			default:
				break;
			}
		}
	}


}


//PEACH
Peach::Peach(int startX, int startY, StudentWorld* world) :
	Player(startX, startY, IID_PEACH, world, 1) {}

//YOSHI
Yoshi::Yoshi(int startX, int startY, StudentWorld* world) :
	Player(startX, startY, IID_YOSHI, world, 2) {}


//SQUARE -> pure virtual
Square::Square(int startX, int startY, int imageId, StudentWorld* world) :
	Actor(startX, startY, imageId, world) {}

//COIN SQUARE -> pure virtual
CoinSquare::CoinSquare(int startX, int startY, int imageId, StudentWorld* world) : Square(startX, startY, imageId, world) {}

//BLUE COIN SQUARE
BlueCoinSquare::BlueCoinSquare(int startX, int startY, StudentWorld* world) : CoinSquare(startX, startY, IID_BLUE_COIN_SQUARE, world) {}

//RED COIN SQUARE
RedCoinSquare::RedCoinSquare(int startX, int startY, StudentWorld* world) : CoinSquare(startX, startY, IID_RED_COIN_SQUARE, world) {}

//STAR SQUARE
StarSquare::StarSquare(int startX, int startY, StudentWorld* world) : CoinSquare(startX, startY, IID_STAR_SQUARE, world) {}

//DIRECTIONAL SQUARE
DirSquare::DirSquare(int startX, int startY, StudentWorld* world) : Square(startX, startY, IID_DIR_SQUARE, world) {}

//BANK SQUARE
BankSquare::BankSquare(int startX, int startY, StudentWorld* world) : CoinSquare(startX, startY, IID_BANK_SQUARE, world) {}

//EVENT SQUARE
EventSquare::EventSquare(int startX, int startY, StudentWorld* world) : Square(startX, startY, IID_EVENT_SQUARE, world) {}

//DROPPING SQUARE
DropSquare::DropSquare(int startX, int startY, StudentWorld* world) : CoinSquare(startX, startY, IID_DROPPING_SQUARE, world) {}

//BADDIE
Baddie::Baddie(int startX, int startY, int imageId, StudentWorld* world) : Actor(startX, startY, imageId, world) {}

//BOWSER
Bowser::Bowser(int startX, int startY, StudentWorld* world) : Baddie(startX, startY, IID_BOWSER, world) {}

//BOO
Boo::Boo(int startX, int startY, StudentWorld* world) : Baddie(startX, startY, IID_BOO, world) {}

//VORTEX
Vortex::Vortex(int startX, int startY, StudentWorld* world) : Actor(startX, startY, IID_VORTEX, world) {}