#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//ACTOR -> pure virtual
Actor::Actor(int startX, int startY, int imageId, int depth, StudentWorld* world) :
	GraphObject(imageId, startX, startY, right, depth), ticks_to_move(0)
{
	m_world = world;
}





//PLAYER AVATAR -> pure virtual
Player::Player(int startX, int startY, int imageId, StudentWorld* world, int p_num) :
	Actor(startX, startY, imageId, 0, world),
	m_player(p_num), m_stars(0), m_coins(0), die_roll(0), count(0), WaitingToRoll(true), m_path(right),
	isAffected(false), switch_dir(false), m_vortex(NULL), attacked(false)
{
	vortex_notif = "";
	m_msg = "P" + std::to_string(m_player) + " Roll: " +
		std::to_string(getRoll()) +
		" Stars: " +
		std::to_string(getStars()) +
		" $$: " + 
		std::to_string(getCoins()) + vortex_notif;
}

void Player::updateVortexMsg() {
	if (hasVortex())
		vortex_notif = " VOR";
	else {
		vortex_notif = "";
	}
}

void Player::updateText() {
	m_msg = "P" + std::to_string(m_player) + " Roll: " +
		std::to_string(getRoll()) +
		" Stars: " +
		std::to_string(getStars()) +
		" $$: " +
		std::to_string(getCoins()) +
		vortex_notif;
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
		case ACTION_FIRE:
			shootVortex(m_vortex);
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
				setDirection(right);
			}
			break;
		case ACTION_DOWN:
			if (!getWorld()->isForkBlocked(this, down)) {
				if (getPath() == up)
					return;
				setPath(down);
				setDirection(right);
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
					setDirection(right);
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
					setDirection(right);
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
						setDirection(right);
						moveAtAngle(up, 2);
						decTick();
						break;
					}
					else {
						setPath(down);
						setDirection(right);
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
						setDirection(right);
						moveAtAngle(up, 2);
						decTick();
						break;
					}
					else {
						setPath(down);
						setDirection(right);
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

void Player::shootVortex(Vortex* vortex) {
	if (hasVortex()) {
		vortex->shoot(this);
		setVortex(NULL);
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
	Actor(startX, startY, imageId, 1, world), must_be_killed(false) {}


void Square::doSomething() {


	int p1_x = getWorld()->getPlayer(1)->getX();
	int p1_y = getWorld()->getPlayer(1)->getY();
	int p2_x = getWorld()->getPlayer(2)->getX();
	int p2_y = getWorld()->getPlayer(2)->getY();

	if (this->getX() == p1_x && this->getY() == p1_y
		&& getWorld()->getPlayer(1)->getRoll() == 0) {
		interactWithPlayer(getWorld()->getPlayer(1));
	}
	if (this->getX() == p2_x && this->getY() == p2_y
		&& getWorld()->getPlayer(2)->getRoll() == 0) {
		interactWithPlayer(getWorld()->getPlayer(2));
	}
		
	
}

bool Square::isActive() {
	if ( getWorld()->getPlayer(1)->getRoll() != 0 ) {
		getWorld()->getPlayer(1)->setIsAffected(false);
	}
	if ( getWorld()->getPlayer(2)->getRoll() != 0 ) {
		getWorld()->getPlayer(2)->setIsAffected(false);
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
	else if (player->getRoll() != 0) {
		getWorld()->playSound(SOUND_DEPOSIT_BANK);
		if (player->getCoins() >= 5) {
			player->addCoins(-5);
			getWorld()->depositBank(5);
		}
		else {
			getWorld()->depositBank(player->getCoins());
			player->addCoins(-player->getCoins());
		}
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
	else if (this->getX() == p2_x && this->getY() == p2_y) {
		interactWithPlayer(getWorld()->getPlayer(2));
	}
	else {
		withdrawn = false;
	}


}



//EVENT SQUARE
EventSquare::EventSquare(int startX, int startY, StudentWorld* world) : Square(startX, startY, IID_EVENT_SQUARE, world) {}

void EventSquare::interactWithPlayer(Player* player) {
	int option = randInt(1, 3);
	if (!player->getIsAffected()) {
		switch (option) {
		case 1:
			option1(player); // teleport player to random square
			break;
		case 2:
			option2(player); // swap positions with other player
			break;
		case 3:
			option3(player); // give vortex projectile
			break;
		default:
			break;
		}
		player->setIsAffected(true);
	}
}

void EventSquare::option1(Player* p) { 
	int newX, newY;
	getWorld()->get_random_square(newX, newY);
	p->moveTo(newX, newY);
	getWorld()->playSound(SOUND_PLAYER_TELEPORT);
	switch (getWorld()->getAction(p->getPlayerNum())) {
	case ACTION_UP:
		if (!getWorld()->isForkBlocked(this, up)) {
			if (p->getPath() == down)
				return;
			setPath(up);
		}
		break;
	case ACTION_DOWN:
		if (!getWorld()->isForkBlocked(this, down)) {
			if (p->getPath() == up)
				return;
			setPath(down);
		}
		break;
	case ACTION_LEFT:
		if (!getWorld()->isForkBlocked(this, left)) {
			if (p->getPath() == right)
				return;
			setPath(left);
		}
		break;
	case ACTION_RIGHT:
		if (!getWorld()->isForkBlocked(this, right)) {
			if (p->getPath() == left)
				return;
			setPath(right);
		}
		break;
	default:
		return;
	}
}
void EventSquare::option2(Player* p) {
	Player* other = getWorld()->get_other_player(p);
	p->swapPositions(other);
	getWorld()->playSound(SOUND_PLAYER_TELEPORT);
}
void EventSquare::option3(Player* p) { // complete after doing vortex class
	getWorld()->givePlayerVortex(p->getPlayerNum());
	getWorld()->playSound(SOUND_GIVE_VORTEX);
}


//DROPPING SQUARE
DropSquare::DropSquare(int startX, int startY, StudentWorld* world) : CoinSquare(startX, startY, IID_DROPPING_SQUARE, world) {}

void DropSquare::interactWithPlayer(Player* player) {
	int option = randInt(1, 2);
	if (!player->getIsAffected()) {
		switch (option) {
		case 1:
			if (player->getCoins() < 10)
				player->addCoins(-(player->getCoins()));
			else {
				player->addCoins(-10);
			}
			getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			break;
		case 2:
			if (player->getStars() > 0) {
				player->addStars(-1);
			}
			getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
			break;
		default:
			break;
		}
		player->setIsAffected(true);
	}
}

//BADDIE
Baddie::Baddie(int startX, int startY, int imageId, StudentWorld* world) : 
	Actor(startX, startY, imageId, 0, world), m_path(right), m_count(0), switch_dir(false),
	paused(true), pause_count(180) {}

bool Baddie::isActive() {

	if (getWorld()->getPlayer(1)->getRoll() != 0) {
		getWorld()->getPlayer(1)->setAttackedByBaddie(false);
	}
	else if (getWorld()->getPlayer(2)->getRoll() != 0) {
		getWorld()->getPlayer(2)->setAttackedByBaddie(false);
	}

	return true;
}

void Baddie::doSomething() {

	if (isPaused()) {

		if (pause_count == 0) {
			setPause(false);
			setTicksToMove(randInt(1, 10) * 8);
			return;
		}

		if (getWorld()->getPlayer(1)->getX() == this->getX() &&
			getWorld()->getPlayer(1)->getY() == this->getY() &&
			getWorld()->getPlayer(1)->isWaitingToRoll()) {
			interactWithPlayer(getWorld()->getPlayer(1));
		}
		if (getWorld()->getPlayer(2)->getX() == this->getX() &&
			getWorld()->getPlayer(2)->getY() == this->getY() &&
			getWorld()->getPlayer(2)->isWaitingToRoll()) {
			interactWithPlayer(getWorld()->getPlayer(2));
		}
		dec_pause();
		return;
	}
	else {
		int aX = getX();
		int aY = getY();
		int x = 2;

		if (getWorld()->atFork(this, getPath()) && !switchedDir()) {
			switch (randInt(1, 4)) {
			case 1: // UP
				if (!getWorld()->isForkBlocked(this, up)) {
					
					setPath(up);
					setDirection(right);
				}
				break;
			case 2: // DOWN
				if (!getWorld()->isForkBlocked(this, down)) {
					
					setPath(down);
					setDirection(right);
				}
				break;
			case 3: // LEFT
				if (!getWorld()->isForkBlocked(this, left)) {
					
					setPath(left);
					setDirection(left);
				}
				break;
			case 4: // RIGHT
				if (!getWorld()->isForkBlocked(this, right)) {
					
					setPath(right);
					setDirection(right);
				}
				break;
			default:
				return;
			}
		}




		switchDir(false);
		if (getTicksToMove() != 0) {

			m_count++;

			switch (getPath()) {
			case up: // UP
				if (!getWorld()->isBlocked(this, up)) {
					setPath(up);
					setDirection(right);
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
			case down: // DOWN
				if (!getWorld()->isBlocked(this, down)) {
					setPath(down);
					setDirection(right);
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
			case right: // RIGHT
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
						setDirection(right);
						moveAtAngle(up, 2);
						decTick();
						break;
					}
					else {
						setPath(down);
						setDirection(right);
						moveAtAngle(down, 2);
						decTick();
						break;
					}
				}
				break;
			case left: // LEFT
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
						setDirection(right);
						moveAtAngle(up, 2);
						decTick();
						break;
					}
					else {
						setPath(down);
						setDirection(right);
						moveAtAngle(down, 2);
						decTick();
						break;
					}
				}
				break;
			default:
				break;
			}
			if (m_count == 8) {
				m_count = 0;
			}
			if (getTicksToMove() == 0) {
				setPause(true);
				reset_pause();
				whenZeroTicks();
				return;
			}

		}
	}
}





//BOWSER
Bowser::Bowser(int startX, int startY, StudentWorld* world) : Baddie(startX, startY, IID_BOWSER, world) {}

void Bowser::whenZeroTicks() {
	switch (randInt(1, 4)) {
	case 1:
		getWorld()->drop_square(this);
		getWorld()->addDropSquare(getX(), getY());
		getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
		break;
	case 2:
	case 3:
	case 4:
		break;
	default:
		break;
	}
	return;
}

void Bowser::interactWithPlayer(Player* p) {
	if (!p->getIsAffected()) {
		switch (randInt(0, 1)) {
		case 0:
			p->addCoins(-(p->getCoins()));
			getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
			break;
		case 1:
			break;
		default:
			break;
		}
		p->setIsAffected(true);
	}
}

//BOO
Boo::Boo(int startX, int startY, StudentWorld* world) : Baddie(startX, startY, IID_BOO, world) {}

void Boo::interactWithPlayer(Player* p) {
	if (!p->getIsAffected()) {
		switch (randInt(0, 1)) {
		case 0:
			p->swapCoins(getWorld()->get_other_player(p));
			break;
		case 1:
			p->swapStars(getWorld()->get_other_player(p));
			break;
		default:
			break;
		}
		getWorld()->playSound(SOUND_BOO_ACTIVATE);
		p->setIsAffected(true);
	}
}

//VORTEX
Vortex::Vortex(int startX, int startY, StudentWorld* world) : 
	Actor(startX, startY, IID_VORTEX, 0, world)
	,must_be_killed(false), is_active(false), is_moving(false) {
	setVisible(false);
}



void Vortex::shoot(Player* p) {
	setVisible(true);
	moveTo(p->getX(),p->getY());
	setDirection(p->getPath());
	is_active = true;
	getWorld()->playSound(SOUND_PLAYER_FIRE);
	return;
}

void Vortex::interactWithBaddie() {
	Actor* target = getWorld()->actor_overlap_vortex(this);
	int newX = 0; int newY = 0;
	if (target == NULL)
		return;
	else {
		getWorld()->get_random_square(newX, newY);
		target->moveTo(newX, newY);
		target->setDirection(right);
		target->setPath(right);
		setVisible(false);
		setKill(true);
		getWorld()->playSound(SOUND_HIT_BY_VORTEX);
	}

}

void Vortex::doSomething() {

	if (is_moving) {
		if (getTicksToMove() > 0) {
			interactWithBaddie();
			moveAtAngle(getDirection(), 2);
			decTick();
			return;
		}
		is_moving = false;
	}
	else if (getTicksToMove() == 0) {
		setTicksToMove(18 * 8);
		is_moving = true;
		return;
	}
	else {
		is_active = false;
		setKill(true);
	}

	return;
}