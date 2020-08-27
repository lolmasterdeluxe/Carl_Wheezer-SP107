#ifndef _GAME_H
#define _GAME_H

#include "Framework\timer.h"
#include "save.h"

extern CStopWatch g_swTimer;
extern bool g_bQuitGame;
// struct to store keyboard events
// a small subset of KEY_EVENT_RECORD
struct SKeyEvent
{
    bool keyDown;
    bool keyReleased;
};

// struct to store mouse events
// a small subset of MOUSE_EVENT_RECORD
struct SMouseEvent
{
    COORD mousePosition;
    DWORD buttonState;
    DWORD eventFlags;
};

// Enumeration to store the control keys that your game will have
enum EKEYS
{
    K_57,    //up              (W)
    K_53,    //down            (A)
    K_41,    //left            (S)
    K_44,    //right           (D)
    K_52,    //reset           (R)
    K_51,    //Dodge backwards (Q)
    K_45,    //Dodge forwards  (E)
    K_55,    //Gin's Ult       (U)
    K_ENTER, //enter         (ENTER)
    K_ESCAPE,
    K_SPACE,
    K_LEFT,
    K_RIGHT,
    K_UP,
    K_DOWN,
    K_COUNT
};

// Enumeration for the different screen states
enum EGAMESTATES
{
    S_SPLASHSCREEN,
    S_GAME,
    S_COUNT,
    S_MENU,
    S_CUTSCENE,
    S_WIN,
    S_LOSE,
    S_START,
    S_EDITOR
};

// struct for the game character
struct SGameChar
{
    COORD m_cLocation;
    double m_dHealth;
    double m_dMana;
    bool m_bActive;
};

struct Platform {
    COORD p_cLocation;
};

void init(void);      // initialize your variables, allocate memory, etc
void getInput(void);      // get input from player
void update(double dt); // update the game and the state of the game
void render(void);      // renders the current state of the game to the console
void shutdown(void);      // do clean up, free memory
void splashScreenWait();    // waits for time to pass in splash screen
void updateGame();          // gameplay logic
void moveCharacter(int n);       // moves the character, collision detection, physics, etc
void sneakCharacter();       //Gin sneak ability
void moveProjectile();      // moves player projectile
void dodge(int i);               // Gin exclusive dodge
void slashAttack(double n, int i); //Slash attack
void downslam();            //Seraph's down slam
void seraphUlt();           //Seraph combo breaker
void focusAttack();         //Gin's focus ability
void focusUlt();            //Gin's focus ultimate
void setdamage();           //damage conditions
void setUltimate(int t);    // sets ultimate metre conditions
void moveEnemy(int n, double t, int d, int e); //set distance and speed for enemy patrolling
void moveBoss(int n, double t, double t2, int d); //set distance and speed for boss movement
void DewmAwaken();          // Dewm Guy's awakening
void DewmIntro();           // Dewm Guy's intro
void processUserInput();    // checks if you should change states or do something else with the game, e.g. pause, exit
void clearScreen();         // clears the current screen and draw from scratch 
void renderNewGameOption();  // renders the splash screen
void renderGame();          // renders the game stuff
void renderMap();           // renders the map to the buffer first
void renderHUD();
void renderCharacter();     // renders the character into the buffer
void renderObj();           // renders object into the buffer
void renderFramerate();     // renders debug information, frame rate, elapsed time, etc
void renderToScreen();      // dump the contents of the buffer to the screen, one frame worth of game
void renderInputEvents();   // renders the status of input events
void renderMenu();
void updateMenu();
void scroll();
void updateTime(double dt);
void saveGame();
void renderPlatform();
void loadLevelData(int number);
void resetToStart();
void renderSavedGame();
void deletePlatforms();
void LEMoveChar();
void nextLevel();
void renderPortal();
void renderMenuBackground();
void renderStartMenu();
void updateStart();
void renderEnemyStats();
void renderDialogue(string d, int x, int y);
void renderWin();
void renderLose();

// keyboard and mouse input event managers
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent);  // define this function for the console to call when there are keyboard events
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent);      // define this function for the console to call when there are mouse events

void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent);   // handles keyboard events for gameplay 
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent); // handles mouse events for gameplay 

#endif // _GAME_H