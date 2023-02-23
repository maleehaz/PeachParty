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
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{

	startCountdownTimer(15);  // this placeholder causes timeout after 5 seconds
    
    // LOAD CORRECT BOARD
    b = new Board();
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
                break;
            case Board::bowser:
                actor.push_back(new Bowser(gc, gr, this));
                break;
            case Board::boo:
                actor.push_back(new Boo(gc, gr, this));
                break;
            case Board::blue_coin_square:
                actor.push_back(new BlueCoinSquare(gc, gr, this));
                break;
            case Board::red_coin_square:
                actor.push_back(new RedCoinSquare(gc, gr, this));
                break;
            case Board::left_dir_square:
            case Board::right_dir_square:
            case Board::up_dir_square:
            case Board::down_dir_square:
                actor.push_back(new DirSquare(gc, gr, this)); // edit dir square angles to match directions
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

    setGameStatText("Game will end in a few seconds");
    
    if (timeRemaining() <= 0)
		return GWSTATUS_NOT_IMPLEMENTED;
    
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

}

bool StudentWorld::isBlocked(Player* player) {
    int ax = player->getX();
    int ay = player->getY();
    int adir = player->getDirection();
    switch (adir) {
    case GraphObject::up:
        ay++;
        break;
    case GraphObject::down:
        ay--;
        break;
    case GraphObject::right:
        ax++;
        break;
    case GraphObject::left:
        ax--;
        break;
    default:
        break;
    }

    for (vector<Actor*>::iterator it = actor.begin(); it != actor.end(); it++) {
        if ((*it)->getX() == ax && (*it)->getY() == ay) {
            return false;
        }
    }

    return true;

}