#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
using namespace std;


GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), b(new Board()), m_bank(0)
{
}

int StudentWorld::init()
{

	startCountdownTimer(99);  // START GAME TIMER, ENDS IN 99 SECONDS
    
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
   // SET GAME TEXT TO DISPLAY STATS AT THE TOP
    string display = player[0]->getText() + 
        " | Time: " +
        to_string(timeRemaining()) + 
        " | Bank: " + to_string(m_bank) + 
        " | " + 
        player[1]->getText();
                        
    // SET DISPLAY
    setGameStatText(display); 
    
    

    for (vector<Player*>::iterator it = player.begin(); it != player.end(); it++) {
        (*it)->doSomething();
    }

    for (vector<Actor*>::iterator it = actor.begin(); it != actor.end(); it++) {
        if ((*it)->isActive())
            (*it)->doSomething();
    }

    killActors();

    //UPDATE DISPLAY TEXT FOR EACH PLAYER
    player[0]->updateVortexMsg();
    player[1]->updateVortexMsg();
    player[0]->updateText();
    player[1]->updateText();
    display = player[0]->getText() +
        " | Time: " +
        to_string(timeRemaining()) +
        " | Bank: " + to_string(m_bank) +
        " | " +
        player[1]->getText();

    //UPDATE DISPLAY TEXT TO SHOW RECENT CHANGES
    setGameStatText(display);


    // DECIDE WINNER AND DISPLAY WHO WON
    if (timeRemaining() <= 0) {
        playSound(SOUND_GAME_FINISHED);
        int p1_stars = player[0]->getStars();
        int p2_stars = player[1]->getStars();
        int p1_coins = player[0]->getCoins();
        int p2_coins = player[1]->getCoins();
        if (p1_stars > p2_stars) {
            setFinalScore(p1_stars, p1_coins);
            return GWSTATUS_PEACH_WON;
        }
        else if (p2_stars > p1_stars) {
            setFinalScore(p2_stars, p2_coins);
            return GWSTATUS_YOSHI_WON;
        }
        else if (p1_coins > p2_coins) {
            setFinalScore(p1_stars, p1_coins);
            return GWSTATUS_PEACH_WON;
        }
        else if (p2_coins > p1_coins) {
            setFinalScore(p2_stars, p2_coins);
            return GWSTATUS_YOSHI_WON;
        }
        else {
            switch (randInt(0, 1)) {
            case 0:
                setFinalScore(p1_stars, p1_coins);
                return GWSTATUS_PEACH_WON;

            case 1:
                setFinalScore(p2_stars, p2_coins);
                return GWSTATUS_YOSHI_WON;
            }
        }
    }
    
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (vector<Player*>::iterator it = player.begin(); it != player.end(); it++) {
        delete (*it);
    }

    for (vector<Actor*>::iterator it = actor.begin(); it != actor.end(); it++) {
        delete (*it);
    }
    
}

// Checks paths that are blocked at a fork
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

// Checks whether an actor is at a fork
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

//Checks if actor's path is blocked in a certain direction
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

// Get coordinates of a random square on the board
void StudentWorld::get_random_square(int& x, int& y) {
    int randSquare = randInt(0, actor.size());
    while ((actor[randSquare]->getX() == this->getPlayer(1)->getX() &&
        actor[randSquare]->getX() == this->getPlayer(1)->getX()) ||
        (actor[randSquare]->getX() == this->getPlayer(2)->getX() &&
            actor[randSquare]->getX() == this->getPlayer(2)->getX())) {
        randSquare = randInt(0, actor.size());
    }
    x = actor[randSquare]->getX();
    y = actor[randSquare]->getY();
}

// Give a player a vortex and initialize new vortex object and
// and add to vector of actors
void StudentWorld::givePlayerVortex(int num_player) {
    Vortex* vortex = new Vortex(getPlayer(num_player)->getX(),
        getPlayer(num_player)->getY(), this);
    actor.push_back(vortex);
    getPlayer(num_player)->setVortex(vortex);
}

// Check if vortec has hit an actor that is impacted by a vortex
Actor* StudentWorld::actor_overlap_vortex(Vortex* v) {
    int x = v->getX();
    int y = v->getY();
    int cx = 0; int cy = 0;
    Actor* target = NULL;
    for (int i = 0; i < actor.size(); i++) {
        cx = actor[i]->getX();
        cy = actor[i]->getY();
        if (x == cx && y == cy && actor[i]->isImpactedByVortex())
            target = actor[i];
    }
    return target;

}

// Replace square on board with Dropping Square
void StudentWorld::drop_square(Baddie* baddie) { 
    int x = baddie->getX();
    int y = baddie->getY();
    int cx = 0; int cy = 0;
    for (int i = 0; i < actor.size(); i++) {
        cx = actor[i]->getX();
        cy = actor[i]->getY();
        if (x == cx && y == cy && actor[i]->isSquare()) {
            actor[i]->setKill(true);
        }
    }
}

// Add a new Dropping Square to the board
void StudentWorld::addDropSquare(int x, int y) {
    actor.push_back(new DropSquare(x, y, this));
}

// Kill actors that are "killable"
// This includes squares that are replaced with Dropping Squares and vortexes
void StudentWorld::killActors() {
    vector<Actor*>::iterator it;
    it = actor.begin();
    while (it != actor.end()) {
        if ((*it)->Kill()) {
            if (it == actor.begin()) {
                vector<Actor*>::iterator temp = it;
                it++;
                delete(*temp);
                actor.erase(temp);
            }
            delete(*it);
            actor.erase(it--);
        }
        it++;
    }
    return;
}