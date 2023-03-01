#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
using namespace std;




GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), b(new Board()), m_bank(0)
{
}

int StudentWorld::init()
{

	startCountdownTimer(99);  // this placeholder causes timeout after 5 seconds
    
    // LOAD CORRECT BOARD
    int BOARDNUM = this->getBoardNumber();
    string fileName = this->assetPath();
    switch (BOARDNUM) {
    case 1:
        fileName += "board01.txt";
        break;
    case 2:
        fileName += "board02.txt";
        break;
    case 3:
        fileName += "board03.txt";
        break;
    case 4:
        fileName += "board04.txt";
        break;
    case 5:
        fileName += "board05.txt";
        break;
    case 6:
        fileName += "board06.txt";
        break;
    case 7:
        fileName += "board07.txt";
        break;
    case 8:
        fileName += "board08.txt";
        break;
    case 9:
        fileName += "board09.txt";
        break;
    default:
        break;
    }
    b->loadBoard(fileName);



  
    // DISPLAY ACTORS ONTO BOARD
    for (int c = 0; c < BOARD_WIDTH; c++) {
        for (int r = 0; r < BOARD_HEIGHT; r++) {
            Board::GridEntry ge = b->getContentsOf(c, r);
            int gc = 16 * c;
            int gr = 16 * r;
            switch (ge) {
            case Board::empty:
                break;
            case Board::player:
                player.push_back(new Peach(gc, gr, this));
                player.push_back(new Yoshi(gc, gr, this));
                actor.push_back(new BlueCoinSquare(gc, gr, this));
                break;
            case Board::bowser:
                actor.push_back(new Bowser(gc, gr, this));
                actor.push_back(new BlueCoinSquare(gc, gr, this));
                break;
            case Board::boo:
                actor.push_back(new Boo(gc, gr, this));
                actor.push_back(new BlueCoinSquare(gc, gr, this));
                break;
            case Board::blue_coin_square:
                actor.push_back(new BlueCoinSquare(gc, gr, this));
                break;
            case Board::red_coin_square:
                actor.push_back(new RedCoinSquare(gc, gr, this));
                break;
            case Board::left_dir_square:
                actor.push_back(new DirSquare(gc, gr, this, GraphObject::left));
                break;
            case Board::right_dir_square:
                actor.push_back(new DirSquare(gc, gr, this, GraphObject::right));
                break;
            case Board::up_dir_square:
                actor.push_back(new DirSquare(gc, gr, this, GraphObject::up));
                break;
            case Board::down_dir_square:
                actor.push_back(new DirSquare(gc, gr, this, GraphObject::down)); 
                break;
            case Board::event_square:
                actor.push_back(new EventSquare(gc, gr, this));
                break;
            case Board::bank_square:
                actor.push_back(new BankSquare(gc, gr, this));
                break;
            case Board::star_square:
                actor.push_back(new StarSquare(gc, gr, this));
                break;
            default:
                break;
            }
        }
    }

    
    

    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.
    string display = "P1 Roll: " + to_string(player[0]->getRoll()) +" Stars: " + to_string(player[0]->getStars()) +
        " $$: " + to_string(player[0]->getCoins()) + " | Time: " +
        to_string(timeRemaining()) + " | Bank: " + to_string(m_bank) + " | P2 Roll: " + to_string(player[1]->getRoll()) +
        " Stars: " + to_string(player[1]->getStars()) + " $$: " + to_string(player[1]->getCoins()) +
        " 22 VOR";
                        

    setGameStatText(display); //edit later to keep track of scores
    
    

    for (vector<Player*>::iterator it = player.begin(); it != player.end(); it++) {
        (*it)->doSomething();
    }

    for (vector<Actor*>::iterator it = actor.begin(); it != actor.end(); it++) {
        if ((*it)->isActive())
            (*it)->doSomething();
    }

    setGameStatText(display);


    if (timeRemaining() <= 0) {
        playSound(SOUND_GAME_FINISHED);
        int p1_stars = player[0]->getStars();
        int p2_stars = player[1]->getStars();
        int p1_coins = player[0]->getCoins();
        int p2_coins = player[1]->getCoins();
        if ((p1_stars + p1_coins) > (p2_stars + p2_coins)) {
            setFinalScore(p1_stars, p1_coins);
            return GWSTATUS_PEACH_WON;
        }
        else if ((p2_stars + p2_coins) > (p1_stars + p1_coins)) {
            setFinalScore(p2_stars, p2_coins);
            return GWSTATUS_YOSHI_WON;
        }
        else {
            return GWSTATUS_NOT_IMPLEMENTED;
        }
    }
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Player*>::iterator p;
    p = player.begin();
    while (p != player.end()) {
        delete(*p);
        player.erase(p--);
        p++;
    }

    vector<Actor*>::iterator a;
    a = actor.begin();
    while (a != actor.end()) {
        delete(*a);
        actor.erase(a--);
        a++;
    }
    delete b;

}


bool StudentWorld::isForkBlocked(Actor* p, int dir) {
    int ax = p->getX();
    int ay = p->getY();
    int dist = 16 - 2 * (p->getCount());

    for (vector<Actor*>::iterator it = actor.begin(); it != actor.end(); it++) {
        if (ay == (*it)->getY() && ax == (*it)->getX() && (*it)->isDirSquare()) {
            playSound(SOUND_GIVE_STAR);
        }
    }
    

    switch (dir) {
    case GraphObject::up:
        for (vector<Actor*>::iterator it = actor.begin(); it != actor.end(); it++) {
            if (ay + dist == (*it)->getY() && ax == (*it)->getX())
                return false;
        }
        break;
    case GraphObject::down:
        for (vector<Actor*>::iterator it = actor.begin(); it != actor.end(); it++) {
            if (ay - dist == (*it)->getY() && ax == (*it)->getX())
                return false;
        }
        break;
    case GraphObject::right:
        for (vector<Actor*>::iterator it = actor.begin(); it != actor.end(); it++) {
            if (ax + dist == (*it)->getX() && ay == (*it)->getY())
                return false;
        }
        break;
    case GraphObject::left:
        for (vector<Actor*>::iterator it = actor.begin(); it != actor.end(); it++) {
            if (ax - dist == (*it)->getX() && ay == (*it)->getY())
                return false;
        }
        break;
    default:
        break;
    }



    return true;

}

bool StudentWorld::atFork(Actor* p, int dir) { 

    switch (dir) {
    case GraphObject::up:
    case GraphObject::down:
        if ( (!isForkBlocked(p, GraphObject::right) &&
            !isForkBlocked(p, GraphObject::left)) ||
            (!isForkBlocked(p, dir) &&
            !isForkBlocked(p, GraphObject::right) && !isForkBlocked(p, 360-dir)) ||
            (!isForkBlocked(p, dir) &&
            !isForkBlocked(p, GraphObject::left) && !isForkBlocked(p, 360-dir))
            )
            return true;
        break;
    case GraphObject::right:
    case GraphObject::left:
        if (!isForkBlocked(p, GraphObject::down) && 
            !isForkBlocked(p, GraphObject::up) ||
            (!isForkBlocked(p, dir) &&
                !isForkBlocked(p, GraphObject::up) && !isForkBlocked(p, abs(dir - 180))) ||
            (!isForkBlocked(p, dir) &&
                !isForkBlocked(p, GraphObject::down) && !isForkBlocked(p, abs(dir - 180)))
            )
            return true;
        break;
    default:
        break;
    }
    return false;
}

bool StudentWorld::isBlocked(Actor* p, int dir) {
    int ax = p->getX();
    int ay = p->getY();
    int dist = 16 - 2 * (p->getCount());

    switch (dir) {
    case GraphObject::up:
        p->
            getPositionInThisDirection(GraphObject::up, 2, ax, ay);
        for (vector<Actor*>::iterator it = actor.begin(); it != actor.end(); it++) {
            if (ay + dist == (*it)->getY() && ax == (*it)->getX())
                return false;
        }
        break;
    case GraphObject::down:
        p->
            getPositionInThisDirection(GraphObject::down, 2, ax, ay);
        for (vector<Actor*>::iterator it = actor.begin(); it != actor.end(); it++) {
            if (ay - dist == (*it)->getY() && ax == (*it)->getX())
                return false;
        }
        break;
    case GraphObject::right:
        p->
            getPositionInThisDirection(GraphObject::right, 2, ax, ay);
        for (vector<Actor*>::iterator it = actor.begin(); it != actor.end(); it++) {
            if (ax + dist == (*it)->getX() && ay == (*it)->getY())
                return false;
        }
        break;
    case GraphObject::left:
        p->
            getPositionInThisDirection(GraphObject::left, 2, ax, ay);
        for (vector<Actor*>::iterator it = actor.begin(); it != actor.end(); it++) {
            if (ax - dist == (*it)->getX() && ay == (*it)->getY())
                return false;
        }
        break;
    default:
        break;
    }

  

    return true;

}

