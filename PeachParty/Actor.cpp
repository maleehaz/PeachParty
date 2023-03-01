#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//ACTOR -> pure virtual
Actor::Actor(int startX, int startY, int imageId, int depth, StudentWorld* world) :
	GraphObject(imageId, startX, startY, right, depth), ticks_to_move(0)
{
	m_world = world;
}

Actor::~Actor() {
	delete m_world;
}



//PLAYER AVATAR -> pure virtual
Player::Player(int startX, int startY, int imageId, StudentWorld* world, int p_num) :
	Actor(startX, startY, imageId, 0, world),
	m_player(p_num), m_stars(0), m_coins(0), die_roll(0), count(0), WaitingToRoll(true), m_path(right),
	isAffected(false), switch_dir(false)
{

}



void Player::doSomething() {

	if (WaitingToRoll) {
		switch (getWorld()->getAction(m_player)) {
		case ACTION_NONE:
			return;
		case ACTION_ROLL:
			die_roll = randInt(1,10);
			setTicksToMove(die_roll * 8);
			WaitingToRoll = false;
			break;
		default:
			return;
		}
	}


	int aX = getX();
	int aY = getY();
	int x = 2;

	if (getWorld()->atFork(this, getPath()) && !switchedDir()) {
		switch (getWorld()->getAction(m_player)) {
		case ACTION_UP:
			if (!getWorld()->isForkBlocked(this, up)) {
				if (getPath() == down)
					return;
				setPath(up);
			}
			break;
		case ACTION_DOWN:
			if (!getWorld()->isForkBlocked(this, down)) {
				if (getPath() == up)
					return;
				setPath(down);
			}
			break;
		case ACTION_LEFT:
			if (!getWorld()->isForkBlocked(this, left)) {
				if (getPath() == right)
					return;
				setPath(left);
				setDirection(left);
			}
			break;
		case ACTION_RIGHT:
			if (!getWorld()->isForkBlocked(this, right)) {
				if (getPath() == left)
					return;
				setPath(right);
				setDirection(right);
			}
			break;
		default:
			return;
		}
	}

	

	if(!WaitingToRoll) {
		switchDir(false);
		if (getTicksToMove() != 0) {
			count++;
			switch (getPath()) {
			case up:
				if (!getWorld()->isBlocked(this, up)) {
					setPath(up);
					moveAtAngle(up, 2);
					decTick();
					break;
				}
				else {
					if (!getWorld()->isBlocked(this, right)) {
						setPath(right);
						setDirection(right);
						moveTo(aX + x, aY);
						decTick();
						break;
					}
					else {
						setPath(left);
						setDirection(left);
						moveTo(aX - x, aY);
						decTick();
						break;
					}
				}
				break;
			case down:
				if (!getWorld()->isBlocked(this, down)) {
					setPath(down);
					moveAtAngle(down, 2);
					decTick();
					break;
				}
				else {
					if (!getWorld()->isBlocked(this, right)) {
						setPath(right);
						setDirection(right);
						moveTo(aX + x, aY);
						decTick();
						break;
					}
					else {
						setPath(left);
						setDirection(left);
						moveTo(aX - x, aY);
						decTick();
						break;
					}
				}
				break;
			case right:
				if (!getWorld()->isBlocked(this, right)) {
					setPath(right);
					setDirection(right);
					moveTo(aX + x, aY);
					decTick();
					break;
				}
				else {
					if (!getWorld()->isBlocked(this, up)) {
						setPath(up);
						moveAtAngle(up, 2);
						decTick();
						break;
					}
					else {
						setPath(down);
						moveAtAngle(down, 2);
						decTick();
						break;
					}
				}
				break;
			case left:
				if (!getWorld()->isBlocked(this, left)) {
					setPath(left);
					setDirection(left);
					moveTo(aX - x, aY);
					decTick();
					break;
				}
				else {
					if (!getWorld()->isBlocked(this, up)) {
						setPath(up);
						moveAtAngle(up, 2);
						decTick();
						break;
					}
					else {
						setPath(down);
						moveAtAngle(down, 2);
						decTick();
						break;
					}
				}
				break;
			default:
				break;
			}
			if (count == 8) {
				count = 0;
				die_roll--;
			}
		}
	}

	if (getTicksToMove() == 0) {
		WaitingToRoll = true;
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
	Actor(startX, startY, imageId, 1, world) {}


void Square::doSomething() {


	int p1_x = getWorld()->getPlayer(1)->getX();
	int p1_y = getWorld()->getPlayer(1)->getY();
	int p2_x = getWorld()->getPlayer(2)->getX();
	int p2_y = getWorld()->getPlayer(2)->getY();

	if (this->getX() == p1_x && this->getY() == p1_y) {
		interactWithPlayer(getWorld()->getPlayer(1));
	}
	if (this->getX() == p2_x && this->getY() == p2_y) {
		interactWithPlayer(getWorld()->getPlayer(2));
	}
		
	
}

bool Square::isActive() {
	if (getWorld()->getPlayer(1)->getRoll() != 0) {
		getWorld()->getPlayer(1)->setIsAffected(false);
		return false;
	}
	else if (getWorld()->getPlayer(2)->getRoll() != 0) {
		getWorld()->getPlayer(2)->setIsAffected(false);
		return false;
	}
	
	return true;
}

//COIN SQUARE -> pure virtual
CoinSquare::CoinSquare(int startX, int startY, int imageId, StudentWorld* world) : Square(startX, startY, imageId, world) {}

//BLUE COIN SQUARE
BlueCoinSquare::BlueCoinSquare(int startX, int startY, StudentWorld* world) : CoinSquare(startX, startY, IID_BLUE_COIN_SQUARE, world) {}

void BlueCoinSquare::interactWithPlayer(Player* player) {
	if (!player->getIsAffected()) {
		player->addCoins(3);
		getWorld()->playSound(SOUND_GIVE_COIN);
		player->setIsAffected(true);
	}
	
}


//RED COIN SQUARE
RedCoinSquare::RedCoinSquare(int startX, int startY, StudentWorld* world) : CoinSquare(startX, startY, IID_RED_COIN_SQUARE, world) {}

void RedCoinSquare::interactWithPlayer(Player* player) {
	if (!player->getIsAffected() && player->getCoins() >= 3) {
		player->addCoins(-3);
		getWorld()->playSound(SOUND_TAKE_COIN);
		player->setIsAffected(true);
	}
}

//STAR SQUARE
StarSquare::StarSquare(int startX, int startY, StudentWorld* world) : CoinSquare(startX, startY, IID_STAR_SQUARE, world) {}

void StarSquare::interactWithPlayer(Player* player) {
	if (!player->getIsAffected() && player->getCoins() >= 20) {
		player->addStars(1);
		getWorld()->playSound(SOUND_GIVE_STAR);
		player->setIsAffected(true);
	}
}
//DIRECTIONAL SQUARE
DirSquare::DirSquare(int startX, int startY, StudentWorld* world, int direction) : 
	Square(startX, startY, IID_DIR_SQUARE, world), m_direction(direction) {
	setDirection(m_direction);
}

void DirSquare::interactWithPlayer(Player* player) {
	if (!player->switchedDir()) {
		player->setPath(m_direction);
		player->switchDir(true);
		return;
	}
}

//BANK SQUARE
BankSquare::BankSquare(int startX, int startY, StudentWorld* world) : 
	CoinSquare(startX, startY, IID_BANK_SQUARE, world) {
	withdrawn = false;
}

void BankSquare::interactWithPlayer(Player* player) {
	if (player->getRoll() == 0 && !withdrawn) {
		int coins;
		getWorld()->withdrawBank(coins);
		player->addCoins(coins);
		player->setIsAffected(true);
		getWorld()->playSound(SOUND_WITHDRAW_BANK);
		withdrawn = true;
		return;
	}
	else if (player->getRoll() != 0 && player->getCoins() >= 5) {
		getWorld()->playSound(SOUND_DEPOSIT_BANK);
		player->addCoins(-5);
		getWorld()->depositBank(5);
	}
}

void BankSquare::doSomething() {

	int p1_x = getWorld()->getPlayer(1)->getX();
	int p1_y = getWorld()->getPlayer(1)->getY();
	int p2_x = getWorld()->getPlayer(2)->getX();
	int p2_y = getWorld()->getPlayer(2)->getY();

	if (this->getX() == p1_x && this->getY() == p1_y) {
		interactWithPlayer(getWorld()->getPlayer(1));
	}
	if (this->getX() == p2_x && this->getY() == p2_y) {
		interactWithPlayer(getWorld()->getPlayer(2));
	}
	else {
		withdrawn = false;
	}


}



//EVENT SQUARE
EventSquare::EventSquare(int startX, int startY, StudentWorld* world) : Square(startX, startY, IID_EVENT_SQUARE, world) {}

//DROPPING SQUARE
DropSquare::DropSquare(int startX, int startY, StudentWorld* world) : CoinSquare(startX, startY, IID_DROPPING_SQUARE, world) {}

//BADDIE
Baddie::Baddie(int startX, int startY, int imageId, StudentWorld* world) : Actor(startX, startY, imageId, 0, world) {}

//BOWSER
Bowser::Bowser(int startX, int startY, StudentWorld* world) : Baddie(startX, startY, IID_BOWSER, world) {}

//BOO
Boo::Boo(int startX, int startY, StudentWorld* world) : Baddie(startX, startY, IID_BOO, world) {}

//VORTEX
Vortex::Vortex(int startX, int startY, StudentWorld* world) : Actor(startX, startY, IID_VORTEX, 0, world) {}