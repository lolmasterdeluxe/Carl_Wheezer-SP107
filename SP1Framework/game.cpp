// This is the main file for the game logic and function
//
//

#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>

// data naming = g_(type)(name)
// data type = { bool(b) , int(i) , float(f) , double(d) , etc...}
int* g_aPlatformsX; int* g_aPlatformsY; int g_iPlatforms = 0;
double  g_dElapsedTime; //delta time elapsed
double  g_dDeltaTime;   //delta time
double  g_pElapsedTime; //projectile elapsed time
double  g_eElapsedTime[10] = { }; //enemy movement elapsed time
double  g_epElapsedTime[10] = { }; //enemy projectile elapsed time
double  g_enemyDelay[10] = { };   //enemy attack delay specific to Ninjas && Vikings
double  g_sElapsedTime; //slash elapsed time
double  g_doElapsedTime;//Dodge elapsed time
double  g_bElapsedTime; //boss movement elapsed time
double  g_bpElapsedTime[5]; //Boss projectile elapsed time
double  g_bjElapsedTime;//boss jump elapsed time
double  g_jElapsedTime; //player jump elapsed time
double  g_hElapsedTime; //health elapsed time
double  g_delay1;       //boss movement delay 1
double  g_delay2;       //boss movement delay 2
double  g_uSpawn;       //Monster spawner
double  g_slashdelay;   //slash attack delay
double  g_stun = 2;     //enemy stunned when attacked
double  g_staminaregen; //regen stamina after slashing
double  g_suElapsedTime;//Seraph ult elapsed time
double  g_udElapsedTime;//Seraph ulti delay elapsed time
double  g_uElapsedTime; //ultimate elapsed time
double  g_cElapsedTime; //Cutscene Elapsed time
double  g_cDelayTime;   //Some cutscene delays
std::string levelFile;
int characterSelect; int startMenuSelect; int pauseMenuSelect;
int oneTime; int canDo; int saveTimer;  int level;

//UI specific timers
int g_iElapsedTime; int g_iTimeAfter;

//mechanics counters
int i[10] = {0,0,0,0,0,0,0,0,0,0}; //enemy movement counter
int ip[10] = { 0,0,0,0,0,0,0,0,0,0 }; //enemy projectile counter
int j = 0;       //projectile counter
int k = 0;       //boss movement counter
int kp[5] = { 0,0,0,0,0 };//Boss projectile counter
int l = 0;       //player movement counter
int s = 0;       //slash attack counter
int n = 2;       //control movement speed (used for movement && sneak)
int p = 0;       //slash delay counter and down slam condition
int ne = 10;      //number of enemies
int obj = 24;    //number of objects
int hp = 4;    //number of health potion objects
int fe = 0.01;   //Focus delay
int dashl = 0;   //counter for dodge (left)
int dashr = 0;   //counter for dodge (right)
int su = 0;      //ultimate counter
int f = 0;       //focus counter
int stun = 0;    //stun counter
int cut = 0;     //Cutscene counter
double sd = 0.2; //slash delay time elapsed condition
bool dragging = false; bool edit = false; bool godMode = false;

//mechanic for Seraph's ult
int y;           //cmd screen set centre y
int x;           //cmd screen set centre x

int slam = 0; //slam down attack counter

//start/end game boolean
bool g_bPlayGame = false; bool g_sAttackState = false;
bool g_sInvulnerable = false; bool g_sUltimate = false;
bool g_sRage = false; bool g_sFocus = false;
bool g_sCutscene = false; bool collide = false;
bool g_sLeftSlash[10]; bool g_sRightSlash[10];
bool g_ninjaState[10]; bool g_sRanged;
bool g_sSpirit;

char c1 = 203;                     //main player's ascii characters
char c2 = 203;
auto c3 = std::string(1, c1) + c2; //combined into 2

char c4;                     //projectile ascii

char c5[10] = { };  //Enemy ascii
char cP5[10] = { };
char bP5[5] = { };
char c0[25] = { }; //Object Ascii
char cO[5] = { 160, 160 };  // Potion ascii
char NPC[9] = { }; //NPC ascii

char b1 = 254; //bear spirit ascii 
char b2 = 254;
auto bear1 = std::string(3, b1) + b2;
char b3 = 249;
char b4 = 249;
auto bear2 = std::string(3, b1) + b2; //combined into 2

char c6 = 232;                     //mini - Boss ascii (c6 - 9)
char c7 = 232;
auto boss1 = std::string(3, c6) + c7; //combined into 2
char c8 = 186;
char c9 = 186;
auto boss2 = std::string(3, c8) + c9;

char c10 = 237;
char c11 = 237;
auto boss3 = std::string(4, c10) + c11; //2nd boss ascii

char c12 = 207;
char c13 = 207;
auto boss4 = std::string(4, c12) + c13; //3rd boss ascii

//Colours for entities, backgrounds 
WORD enemyColor[10] = { };
WORD bossColor[4] = { 0x4E , 0x0E, 0X01, 0X8E };
WORD BProjColor[5] = { 0X7E,0X7E,0X7E,0X7E,0X7E };
WORD bearColor = 0x9F;
WORD BGcolor;
WORD Healthcolor;
WORD ObjColor[25] = {};
WORD PotionColor[5] = {};
WORD NPCcolor[9] = {};
WORD EProjColor[9] = {};
// bool g_bInMenu = false;
SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;

// Game specific variables here
save state;
std::string status;
SGameChar   g_sChar; SGameChar g_sCharSpawn; SGameChar g_sProj; //Player specific chars
SGameChar   g_sEnemy[10]; SGameChar g_sEProj[10]; //Enemy specific chars
SGameChar   g_sPortal;
SGameChar   g_sBearSpirit[2];   //Thorfinn's bear
SGameChar   g_sBoss1[2];       //Boss top and bottom half
SGameChar   g_sBoss2; SGameChar g_sBProj[5]; // 2nd boss and its projectile
SGameChar   g_sBoss3; //3rd boss
SGameChar   g_sNPC[9];       //NPC
SGameChar   g_sPotion[5];     //Potion Object
SGameChar   g_sObj[25];      //Any obj
SGameChar   g_sbCutscene[10000];  //Cutscene bacground
SGameChar   g_sTrainTracks[200]; //Train tracks for train scene 
SGameChar   g_sPrimeObj[2];  //Collidable Obj used in cutscenes
EGAMESTATES g_eGameState = S_START; // initial state

// Console object
Console g_Console(100, 30, "Portal Legends");

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init(void) {
    // Set precision for floating point output
    g_dElapsedTime = 0.0; characterSelect; startMenuSelect = 0; pauseMenuSelect = 0;
    oneTime = 0; canDo = 0; level = 0; saveTimer = 0;
    
    // sets the initial state for the game
    g_eGameState = S_START;

    //loadLevelData(1);

    std::ifstream save;
    save.open("save.txt");
    if (!save) {
  //      g_sChar.m_cLocation.X = 0;
  //      g_sChar.m_cLocation.Y = 0;
  //      g_sChar.m_dMana = 0;
  //      g_sChar.m_dHealth = 50;
  //      g_sBossP1.m_cLocation.X = state.returnBossX();
  //      g_sBossP1.m_cLocation.Y = state.returnBossY();
  //      g_sBossP2.m_cLocation.X = state.returnBossX();
  //      g_sBossP2.m_cLocation.Y = state.returnBossY() - 1;
  //      g_sProj.m_cLocation.X = g_sChar.m_cLocation.X;
  //      g_sProj.m_cLocation.Y = g_sChar.m_cLocation.Y;
  //      g_sChar.m_bActive = state.returnCharState();
  //      g_sChar.m_dHealth = state.returnCharHealth();
  //      g_sBossP1.m_cLocation.X = bossd;
  //      g_sBossP1.m_cLocation.Y = 28;
  //      g_sBossP2.m_cLocation.X = bossd;
  //      g_sBossP2.m_cLocation.Y = 28 - 1;
        for (int i = 0; i <= ne; i++)
        {
            g_sEnemy[i].m_dHealth = 5;
            g_ninjaState[i] = false;
            g_sRightSlash[i] = false;
            g_sLeftSlash[i] = false;
        }
        for (int i = 0; i <= obj; i++)
        {
            g_sObj[i].m_dHealth = 5;
            if (level == 6)
            {
                g_sObj[i].m_dHealth = 20;
            }
        }
        g_sBoss1[0].m_dHealth = 300;
        g_sBoss2.m_dHealth = 300;
    }
    else
    {
        state.loadSave();
        loadSavedGame();
        loadLevelData(level);
        save.close();
    }
    if (characterSelect == 0)
    {
        c4 = 205;
    }
    else if (characterSelect == 3)
    {
        c4 = 246;
    }
    g_sBProj[0].m_cLocation.X = g_sBoss2.m_cLocation.X;
    g_sBProj[1].m_cLocation.X = g_sBoss2.m_cLocation.X + 1;
    g_sBProj[2].m_cLocation.X = g_sBoss2.m_cLocation.X + 2;
    g_sBProj[3].m_cLocation.X = g_sBoss2.m_cLocation.X + 3;
    g_sBProj[4].m_cLocation.X = g_sBoss2.m_cLocation.X + 4;
    for (int i = 0; i < 5; i++)
    {
        g_sBProj[i].m_cLocation.Y = g_sBoss2.m_cLocation.Y;
    }
    g_sChar.m_dMana = state.returnCharMana();
    g_sChar.m_dMana = 0;
    y = g_Console.getConsoleSize().Y - 2;
    x = g_Console.getConsoleSize().X - 2;
    g_sBoss1[0].m_dHealth = 300;
    g_sBoss2.m_dHealth = 300;
    
    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 20, L"Consolas");

    // remember to set your keyboard handler, so that your functions can be notified of input events
    g_Console.setKeyboardHandler(keyboardHandler);
    g_Console.setMouseHandler(mouseHandler);
    colour(0x00);
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown(void) {
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
    //saveGame();
    deletePlatforms();
    g_Console.clearBuffer();
}

//--------------------------------------------------------------
// Purpose  : Get all the console input events
//            This function sets up the keyboard and mouse input from the console.
//            We will need to reset all the keyboard status, because no events will be sent if no keys are pressed.
//
//            Remember to set the handlers for keyboard and mouse events.
//            The function prototype of the handler is a function that takes in a const reference to the event struct
//            and returns nothing. 
//            void pfKeyboardHandler(const KEY_EVENT_RECORD&);
//            void pfMouseHandlerconst MOUSE_EVENT_RECORD&);
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput(void) {
    // resets all the keyboard events
    memset(g_skKeyEvent, 0, K_COUNT * sizeof(*g_skKeyEvent));
    // then call the console to detect input from user
    g_Console.readConsoleInput();
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the keyboard input. Whenever there is a keyboard event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            The KEY_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any keyboard event in the Splashscreen state
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent - reference to a key event struct
// Output   : void
//--------------------------------------------------------------
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent) {
    switch (g_eGameState) {
    case S_SPLASHSCREEN: gameplayKBHandler(keyboardEvent); // handle menu keyboard event for the splash screen
        break;
    case S_START: gameplayKBHandler(keyboardEvent);
        break;
    case S_GAME: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event 
        break;
    case S_MENU: gameplayKBHandler(keyboardEvent);
        break;
    case S_CUTSCENE: break;
    case S_WIN: break;
    case S_LOSE: break;
    case S_EDITOR: break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the mouse input. Whenever there is a mouse event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            For the mouse event, if the mouse is not moved, no event will be sent, hence you should not reset the mouse status.
//            However, if the mouse goes out of the window, you would not be able to know either. 
//
//            The MOUSE_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any mouse event in the Splashscreen state
//            
// Input    : const MOUSE_EVENT_RECORD& mouseEvent - reference to a mouse event struct
// Output   : void
//--------------------------------------------------------------
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent) {
    switch (g_eGameState) {
    case S_SPLASHSCREEN: // don't handle anything for the splash screen
        break;
    case S_GAME: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
        break;
    case S_EDITOR: break;
    }
}

void saveGame() {
    state.saveState(sendSaveData());
}

//--------------------------------------------------------------
// Purpose  : This is the keyboard handler in the game state. Whenever there is a keyboard event in the game state, this function will be called.
//            
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent) {
    // here, we map the key to our enums
    EKEYS key;
    key = K_COUNT;
    switch (keyboardEvent.wVirtualKeyCode) {
    case 0x57: key = K_57; break; //W
    case 0x53: key = K_53; break; //S
    case 0x41: key = K_41; break; //A
    case 0x44: key = K_44; break; //D
    case 0x52: key = K_52; break; //R
    case 0x51: key = K_51; break; //Q
    case 0x45: key = K_45; break; //E
    case 0x55: key = K_55; break; //U
    case 0x47: key = K_47; break; //G
    case 0x31: key = K_31; break; //1
    case 0x32: key = K_32; break; //2
    case VK_SPACE: key = K_SPACE; break;
    case VK_ESCAPE: key = K_ESCAPE; break;
    case VK_RETURN: key = K_ENTER; break;
    case VK_LEFT: key = K_LEFT; break;
    case VK_RIGHT: key = K_RIGHT; break;
    case VK_UP: key = K_UP; break;
    case VK_DOWN: key = K_DOWN; break;
    }
    // a key pressed event would be one with bKeyDown == true
    // a key released event would be one with bKeyDown == false
    // if no key is pressed, no event would be fired.
    // so we are tracking if a key is either pressed, or released
    if (key != K_COUNT) {
        g_skKeyEvent[key].keyDown = keyboardEvent.bKeyDown;
        g_skKeyEvent[key].keyReleased = !keyboardEvent.bKeyDown;
    }
    //processUserInput();
    canDo = 0;
}

//--------------------------------------------------------------
// Purpose  : This is the mouse handler in the game state. Whenever there is a mouse event in the game state, this function will be called.
//            
//            If mouse clicks are detected, the corresponding bit for that mouse button will be set.
//            mouse wheel, 
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent) {
    if (MOUSE_MOVED) // update the mouse position if there are no events
        g_mouseEvent.mousePosition = mouseEvent.dwMousePosition;
    g_mouseEvent.buttonState = mouseEvent.dwButtonState;
    g_mouseEvent.eventFlags = mouseEvent.dwEventFlags;
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt) {
    // get the delta time
    /*g_dElapsedTime += dt;
    g_dDeltaTime = dt;
    g_iElapsedTime = (int)round(g_dElapsedTime);
    g_iTimeAfter = g_iElapsedTime + 1;*/

    switch (g_eGameState) {
    case S_SPLASHSCREEN: splashScreenWait(); // game logic for the splash screen
        break;
    case S_GAME: updateGame(); updateEnemy(); updateTime(dt); // gameplay logic when we are in the game
        break;
    case S_MENU: updateMenu();
        break;
    case S_START: updateStart();
        break;
    case S_CUTSCENE: break;
    case S_WIN: break;
    case S_LOSE: processUserInput(); break;
    case S_EDITOR: updateEditor(); break;
    }
    //processUserInput();
}

void updateTime(double dt) {
    g_dElapsedTime += dt;   //game time elapsed
    g_dDeltaTime = dt;      //seconds between each frame (if 90fps, deltatime = 1/90)
    g_pElapsedTime += dt;   //Projectile time elapsed
    for (int i = 0; i <= 9; i++)
    {
        g_eElapsedTime[i] += dt; //Enemy movement time elapsed
        g_epElapsedTime[i] += dt; //Enemy projectile elapsed time
        g_enemyDelay[i] += dt;    //Enemy ninja delay
        g_bpElapsedTime[i] += dt; //Boss projectile elapsed time
    }
    g_sElapsedTime += dt;   //Slash movement time elapsed
    g_doElapsedTime += dt;  //Dodge time elapsed
    g_uElapsedTime += dt;   //Ultimate meter time elapsed
    g_hElapsedTime += dt;   //Health meter time elapsed
    g_jElapsedTime += dt;   //Player jump time elapsed
    g_bElapsedTime += dt;   //Boss movement time elapsed
    g_bjElapsedTime += dt;  //boss jump elapsed time
    g_suElapsedTime += dt;  //Seraph ultimate time elapsed
    g_udElapsedTime += dt;  //Seraph ulti delay
    g_cElapsedTime += dt;   //Cutscene elapsed time
    g_cDelayTime += dt;     //Some cutscene delay
    g_delay1 += dt;         //boss movement delay 1
    g_delay2 += dt;         //boss movement delay 2
    g_slashdelay += dt;     //delay after each slash (applies to melee attacks)
    g_stun += dt;           //stun enemy when hit
    g_uSpawn += dt;         //Enemy Spawner
    g_staminaregen += dt;   //regen stamina time
    g_iElapsedTime = (int)round(g_dElapsedTime);
    g_iTimeAfter = g_iElapsedTime + 1;
}

void splashScreenWait() {      // waits for time to pass in splash screen
    if (g_bPlayGame == true)   // wait for keyboard to switch to game mode, else do nothing
        g_eGameState = S_GAME;
    //renderSplashScreen();
    reset();
    processUserInput();
}

void updateGame() {     // gameplay logic
    if (g_bPlayGame) {
        processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit

        if (g_sCutscene == false)
        {
            moveCharacter(2); // moves the player by *2 steps, collision detection, physics, etc
        }
        if (characterSelect == 0)  //Dewm Guy
        {
            if (levelFile == "Dewmtutorial_1.txt")
            {
                DewmAwaken();
            }
            if (levelFile == "DewmIntro.txt")
            {
                DewmIntro();
            }
            if (level >= 2)
            {
                moveProjectile(); //Shooting mechanic
                setUltimate(50);  //Set ultimate capacity to *50
            }
        }
        if (characterSelect == 1) { //Seraph
            if (level >= 1)
            {
                slashAttack(0.2, 10);   //slash forward by *10 steps, speed of slash is *.2 seconds
                downslam();             //Seraph down slam attack
            }
            if (level >= 2)
            {
                seraphUlt();            //seraph star combo breaker
                setUltimate(200);        //Set ultimate capacity to *200
            }
            if (levelFile == "SeraphIntro.txt")
            {
                SeraphIntro();
            }
        }
        if (characterSelect == 2) { //Gin
            if (level == 0)
            {
                GinIntro();
            }
            
            if (g_sCutscene == false)
            {
                moveCharacter(2);     // moves the player by *2 steps, collision detection, physics, etc
                slashAttack(0.1, 10); //slash forward by *10 steps, speed of slash is *.1 seconds
                sneakCharacter();     // Gin sneak ability
                dodge(5);             //dodge for Gin (10 steps)
                
            }
            if (levelFile == "Gintutorial_3.txt")
            {
                focusAttack();        //Gin's focus ability
                focusUlt();           //Gin's focus ultimate
                setUltimate(75);      //Set ultimate capacity to *75
            }
        }
        if (characterSelect == 3)
        {
            ThorfinnArms(0.01, 0.2, 10, 50); //speed of projectile || melee attack, melee attack distance and projectile distance
            spiritUlt();
            moveCharacter(1);
            setUltimate(100);      //Set ultimate capacity to *50
        }
        setdamage();              //find reason for damage
        nextLevel();
        if (godMode)
            LEMoveChar();
        if (g_sChar.m_dHealth == 0) {
            resetToStart();
        }
        //scroll();
    }
    //state.saveState(std::to_string(g_sChar.m_cLocation.X), std::to_string(g_sChar.m_cLocation.Y), status, std::to_string(g_sProj.m_cLocation.X), std::to_string(g_sProj.m_cLocation.Y));
}

void updateEnemy()
{
    //loading of enemies in levels with specific behaviours and counters
    if (levelFile == "lvlDewm1.txt")
    {
        moveEnemy(i[0], 'r', 15, 0.5, 15, 0);  //move enemy [e] by *5 steps back and forth from position x = *50 every *0.5 seconds
        moveEnemy(i[1], 'l', 26, 0.2, 63, 1);  //0 is the constant
        EnemyProjectile(i[2], 2, 0.1, 19);       //Enemy int counter, Enemy array number, delay between shots and distance
        moveEnemy(i[3], 'r', 19, 0.4, 61, 3);
        moveEnemy(i[4], 'l', 20, 0.3, 35, 4);
        moveEnemy(i[5], 'r', 17, 0.1, 38, 5);
        moveEnemy(i[6], 'l', 22, 0.5, 79, 6);
        moveEnemy(i[7], 'r', 46, 0.4, 33, 7);
        moveEnemy(i[8], 'r', 20, 0.3, 29, 8);
        EnemyProjectile(i[9], 9, 0.1, 28);
        ne = 10;
        hp = -1; //health potion count
    }
    if (levelFile == "lvlDewm2.txt")
    {
        moveEnemy(i[0], 'r', 44, 0.2, 36, 0);  //move enemy [e] by *5 steps back and forth from position x = *50 every *0.5 seconds
        moveEnemy(i[1], 'l', 15, 0.2, 97, 1);  //0 is the constant
        EnemyProjectile(ip[2], 2, 0.1, 10);
        moveEnemy(i[3], 'r', 19, 0.4, 16, 3);
        moveEnemy(i[4], 'l', 12, 0.3, 81, 4);
        moveEnemy(i[5], 'l', 22, 0.1, 63, 5);
        EnemyProjectile(ip[6], 6, 0.1, 10);
        moveEnemy(i[7], 'l', 33, 0.4, 85, 7);
        moveEnemy(i[8], 'r', 33, 0.3, 51, 8);
        EnemyProjectile(ip[9], 9, 0.1, 24);
        healthPotion(0);
        ne = 10;
        hp = 0;
    }
    if (levelFile == "lvlDewm3.txt")
    {
        moveEnemy(i[0], 'l', 14, 0.2, 29, 0);  //move enemy [e] by *5 steps back and forth from position x = *50 every *0.5 seconds
        moveEnemy(i[1], 'l', 13, 0.3, 28, 1);  //0 is the constant
        moveEnemy(i[2], 'r', 13, 0.3, 32, 2);
        moveEnemy(i[3], 'r', 12, 0.4, 33, 3);
        EnemyProjectile(ip[4], 4, 0.1, 26);
        moveEnemy(i[5], 'l', 26, 0.1, 61, 5);
        EnemyProjectile(ip[6], 6, 0.1, 19);
        moveEnemy(i[7], 'r', 22, 0.4, 44, 7);
        EnemyProjectile(ip[8], 8, 0.1, 12);
        moveEnemy(i[9], 'l', 17, 0.1, 51, 9);
        healthPotion(0);
        healthPotion(1);
        ne = 10;
        hp = 1;
    }
    if (levelFile == "lvlDewm4.txt")
    {
        EnemyProjectile(ip[0], 0, 0.1, 35);  //move enemy [e] by *5 steps back and forth from position x = *50 every *0.5 seconds
        moveEnemy(i[1], 'l', 15, 0.3, 97, 1);  //0 is the constant
        moveEnemy(i[2], 'r', 17, 0.3, 38, 2);
        EnemyProjectile(ip[3], 3, 0.1, 16);
        EnemyProjectile(ip[4], 4, 0.1, 9);
        moveEnemy(i[5], 'l', 13, 0.1, 15, 5);
        moveBoss(26, 0.05, 5, 43); //move *26 steps, delays his movement by *0.05 seconds, stops for *2 seconds, returns to position x = *43
        healthPotion(0);
        healthPotion(1);
        ne = 6;
        hp = 1;
    }
    if (levelFile == "lvlSeraph1.txt")
    {
        moveEnemy(i[0], 'l', 22, 0.2, 34, 0);  //move enemy [e] by *5 steps back and forth from position x = *50 every *0.5 seconds
        EnemyProjectile(ip[1], 1, 0.1, 20);
        moveEnemy(i[2], 'r', 30, 0.3, 58, 2);
        EnemyProjectile(ip[2], 2, 0.1, 15);
        EnemyProjectile(ip[3], 3, 0.05, 66);
        EnemyProjectile(ip[4], 4, 0.05, 66);
        moveEnemy(i[5], 'r', 10, 0.1, 54, 5);
        EnemyProjectile(ip[6], 6, 0.1, 23);
        moveEnemy(i[7], 'l', 7, 0.4, 50, 7);
        EnemyProjectile(ip[8], 8, 0.1, 32);
        healthPotion(0);
        healthPotion(1);
        ne = 9;
        hp = 1;
    }
    if (levelFile == "lvlSeraph2.txt")
    {
        moveEnemy(i[0], 'l', 7, 0.2, 56, 0);
        EnemyProjectile(ip[1], 1, 0.1, 12);
        moveEnemy(i[2], 'r', 26, 0.2, 36, 2);
        EnemyProjectile(ip[3], 3, 0.1, 51);
        moveEnemy(i[4], 'r', 30, 0.2, 11, 4);
        EnemyProjectile(ip[4], 4, 0.1, 30);
        EnemyProjectile(ip[5], 5, 0.05, 77);
        moveEnemy(i[6], 'l', 20, 0.2, 88, 6);
        EnemyProjectile(ip[7], 7, 0.1, 15);
        moveEnemy(i[8], 'l', 67, 0.1, 86, 8);
        EnemyProjectile(ip[9], 9, 0.05, 68);
        healthPotion(0);
        ne = 10;
        hp = 0;
    }
    if (levelFile == "lvlSeraph3.txt")
    {
        moveEnemy(i[0], 'r', 62, 0.2, 13, 0);
        EnemyProjectile(ip[1], 1, 0.1, 18);
        moveEnemy(i[2], 'r', 44, 0.3, 32, 2);
        EnemyProjectile(ip[2], 2, 0.1, 44);
        EnemyProjectile(ip[3], 3, 0.05, 47);
        moveEnemy(i[4], 'l', 29, 0.4, 87, 4);
        EnemyProjectile(ip[4], 4, 0.1, 29);
        EnemyProjectile(ip[5], 5, 0.1, 25);
        moveEnemy(i[6], 'r', 30, 0.2, 37, 6);
        EnemyProjectile(ip[6], 6, 0.1, 51);
        EnemyProjectile(ip[7], 7, 0.1, 12);
        moveEnemy(i[8], 'l', 15, 0.3, 56, 8);
        EnemyProjectile(ip[9], 9, 0.1, 29);
        healthPotion(0);
        hp = 0;
        ne = 10;
    }
    if (levelFile == "lvlSeraph4.txt")
    {
        moveEnemy(i[0], 'l', 15, 0.2, 58, 0);
        EnemyProjectile(ip[1], 1, 0.05, 67);
        EnemyProjectile(ip[2], 2, 0.05, 80);
        EnemyProjectile(ip[3], 3, 0.1, 34);
        moveEnemy(i[4], 'r', 23, 0.2, 34, 4);
        EnemyProjectile(ip[5], 5, 0.05, 67);
        BossProjectile(0, 0.3);
        BossProjectile(1, 0.1);
        if (g_sBoss2.m_dHealth < 150) //Boss becomes more difficult
        {
            BossProjectile(2, 0.2);
            BossProjectile(3, 0.4);
            BossProjectile(4, 0.1);
        }
        healthPotion(0);
        ne = 6;
        hp = 0;
    }
    if (levelFile == "Gintutorial_1.txt")
    {
        ne = 3;
        hp = -1;
    }
    if (levelFile == "Gintutorial_2.txt")
    {
        EnemyProjectile(ip[0], 0, 0.05, 75);
        ninjaAttack(i[1], ip[1], 36, 0.03, 58, 1);
        healthPotion(0);
        ne = 2;
        hp = 0;
    }
    if (levelFile == "lvlGin1.txt")
    {
        ninjaAttack(i[0], ip[0], 20, 0.03, 55, 0);
        ninjaAttack(i[1], ip[1], 18, 0.03, 89, 1);
        ninjaAttack(i[2], ip[2], 8, 0.03, 89, 2);
        healthPotion(0);
        ne = 3;
        hp = 0;
    }
    if (levelFile == "lvlGin2.txt")
    {
        ninjaAttack(i[0], ip[0], 29, 0.03, 49, 0);
        ninjaAttack(i[1], ip[1], 48, 0.03, 67, 1);
        EnemyProjectile(ip[2], 2, 0.1, 90);
        ninjaAttack(i[3], ip[3], 14, 0.03, 21, 3);
        ninjaAttack(i[4], ip[4], 20, 0.03, 63, 4);
        EnemyProjectile(ip[5], 5, 0.1, 69);
        ninjaAttack(i[6], ip[6], 18, 0.03, 52, 6);
        ninjaAttack(i[7], ip[7], 32, 0.03, 67, 7);
        EnemyProjectile(ip[8], 8, 0.1, 58);
        healthPotion(0);
        ne = 9;
        hp = 0;
    }
    if (levelFile == "lvlGin3.txt")
    {
        ninjaAttack(i[0], ip[0], 12, 0.03, 67, 0);
        ninjaAttack(i[1], ip[1], 12, 0.03, 29, 1);
        ninjaAttack(i[2], ip[2], 10, 0.03, 57, 2);
        ninjaAttack(i[3], ip[3], 30, 0.03, 96, 3);
        ninjaAttack(i[4], ip[4], 28, 0.03, 69, 4);
        ninjaAttack(i[5], ip[5], 58, 0.03, 60, 5);
        EnemyProjectile(ip[6], 6, 0.05, 87);
        healthPotion(0);
        healthPotion(1);
        ne = 7;
        hp = 1;
    }
    if (levelFile == "lvlGin4.txt")
    {
        //ninja boss is here
        ne = 0;
        hp = 0;
    }
    if (levelFile == "lvlVik1.txt")
    {
        vikingAttack(i[0], 25, 0.03, 49, 0);
        vikingAttack(i[1], 29, 0.03, 53, 1);
        vikingAttack(i[2], 32, 0.03, 56, 2);
        vikingBow(i[3], 3, 0.01, 3, 67);
        healthPotion(0);
        ne = 4;
        hp = 0;
    }
    if (levelFile == "lvlVik2.txt")
    {
        if (g_sChar.m_cLocation.X > g_sEnemy[0].m_cLocation.X)
        {
            vikingAttack(i[0], 31, 0.03, 20, 0);
        }
        vikingAttack(i[1], 33, 0.03, 51, 1);
        vikingAttack(i[2], 7, 0.03, 40, 2);
        vikingBow(i[3], 3, 0.01, 3, 51);
        vikingAttack(i[4], 20, 0.03, 86, 4);
        vikingBow(i[5], 5, 0.01, 3, 53);
        vikingAttack(i[6], 6, 0.03, 70, 6);
        moveEnemy(i[7], 'r', 7, 0.2, 68, 7);
        healthPotion(0);
        hp = 0;
        ne = 8;
    }
    if (levelFile == "lvlVik3.txt")
    {
        vikingAttack(i[0], 5, 0.03, 22, 0);
        vikingAttack(i[1], 8, 0.03, 5, 1);
        vikingAttack(i[2], 16, 0.03, 35, 2);
        vikingAttack(i[3], 21, 0.03, 40, 3);
        vikingBow(i[4], 4, 0.01, 3, 39);
        healthPotion(0);
        hp = 0;
        ne = 5;
    }
    if (levelFile == "lvlVik4.txt")
    {
        //viking boss here
        hp = 0;
        ne = 0;
    }
  
}

void updateMenu() {
    processUserInput();
}

void updateStart() {
    processUserInput();
}


void moveCharacter(int n)
{
    // Updating the location of the character based on the key release
    // providing a beep sound whenver we shift the character
    int oneStep;
    if (g_skKeyEvent[K_57].keyDown) {
        for (int j = 0; j < g_iPlatforms; j++) {
            if (g_sChar.m_cLocation.Y + 1 == g_aPlatformsY[j] && g_sChar.m_cLocation.X == g_aPlatformsX[j]) //Jump up
            {
                l = 0;
                l++;
                g_sChar.m_cLocation.Y++;
            }
            else if (g_sChar.m_cLocation.Y + 1 == g_aPlatformsY[j] && g_sChar.m_cLocation.X + 1 == g_aPlatformsX[j]) //Jump up
            {
                l = 0;
                l++;
                g_sChar.m_cLocation.Y++;
            }
            if (g_sChar.m_cLocation.Y == g_aPlatformsY[j] && g_sChar.m_cLocation.X == g_aPlatformsX[j]) {
                g_sChar.m_cLocation.Y--;
            }
            else if (g_sChar.m_cLocation.Y == g_aPlatformsY[j] && g_sChar.m_cLocation.X + 1 == g_aPlatformsX[j]) {
                g_sChar.m_cLocation.Y--;
            }
        }
    }
    if (l >= 1 && l < 4 && g_sUltimate == false) //check if player is mid air to move up to 4
    {
        if (g_jElapsedTime > 0.05)
        {
            /*if (l == 1)
            {
                Beep(1440, 30);
            }*/
            g_sChar.m_cLocation.Y--;
            for (int i = 0; i < g_iPlatforms; i++) {
                if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X == g_aPlatformsX[i]) {
                    g_sChar.m_cLocation.Y++;
                }
                else if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X + 1 == g_aPlatformsX[i]) {
                    g_sChar.m_cLocation.Y++;
                }
            }
            for (int i = 0; i <= obj; i++)
            {
                if (c0[i] != 0)
                {
                    if ((g_sChar.m_cLocation.X == g_sObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sObj[i].m_cLocation.Y) || (g_sChar.m_cLocation.X == g_sPrimeObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPrimeObj[i].m_cLocation.Y))
                    {
                        g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y++;
                    }
                    else if ((g_sChar.m_cLocation.X + 1 == g_sObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sObj[i].m_cLocation.Y) || (g_sChar.m_cLocation.X + 1 == g_sPrimeObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPrimeObj[i].m_cLocation.Y))
                    {
                        g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y++;
                    }
                }
            }
            if (GetKeyState(0x41) & 0x800) //check for A input and jump left
            {
                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 2;
                for (int i = 0; i < g_iPlatforms; i++) {
                    if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X == g_aPlatformsX[i]) {
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 2;
                    }
                    else if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X + 1 == g_aPlatformsX[i]) {
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 2;
                    }
                }
                for (int i = 0; i <= obj; i++)
                {
                    if (c0[i] != 0)
                    {
                        if ((g_sChar.m_cLocation.X == g_sObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sObj[i].m_cLocation.Y) || (g_sChar.m_cLocation.X == g_sPrimeObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPrimeObj[i].m_cLocation.Y))
                        {
                            g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 2;
                        }
                        else if ((g_sChar.m_cLocation.X + 1 == g_sObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sObj[i].m_cLocation.Y) || (g_sChar.m_cLocation.X + 1 == g_sPrimeObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPrimeObj[i].m_cLocation.Y))
                        {
                            g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 2;
                        }
                    }
                }
            }
            if (GetKeyState(0x44) & 0x800) //check for D input and jump right;
            {
                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 2;
                for (int i = 0; i < g_iPlatforms; i++) {
                    if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X == g_aPlatformsX[i]) {
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 2;
                    }
                    else if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X + 1 == g_aPlatformsX[i]) {
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 2;
                    }
                }
                for (int i = 0; i <= obj; i++)
                {
                    if (c0[i] != 0)
                    {
                        if ((g_sChar.m_cLocation.X == g_sObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sObj[i].m_cLocation.Y) || (g_sChar.m_cLocation.X == g_sPrimeObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPrimeObj[i].m_cLocation.Y))
                        {
                            g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 2;
                        }
                        else if ((g_sChar.m_cLocation.X + 1 == g_sObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sObj[i].m_cLocation.Y) || (g_sChar.m_cLocation.X + 1 == g_sPrimeObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPrimeObj[i].m_cLocation.Y))
                        {
                            g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 2;
                        }
                    }
                }


            }
            g_jElapsedTime = 0;
            l++;
        }
    }
    for (int j = 0; j < g_iPlatforms; j++) { //check for collision with platforms
        if (l >= 4 && g_sUltimate == false && g_sChar.m_cLocation.Y - 1 != g_aPlatformsY[j]) //check if l is 3 and push down for gravity
        {
            if (g_jElapsedTime > 0.05)
            {
                if (l == 1)
                {
                    Beep(1440, 30);
                }
                g_sChar.m_cLocation.Y++;
                for (int i = 0; i < g_iPlatforms; i++) {
                    if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                        g_sChar.m_cLocation.Y--;
                    }
                    else if (g_sChar.m_cLocation.X + 1 == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                        g_sChar.m_cLocation.Y--;
                    }
                }
                for (int i = 0; i <= obj; i++)
                {
                    if (c0[i] != 0)
                    {
                        if ((g_sChar.m_cLocation.X == g_sObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sObj[i].m_cLocation.Y) || (g_sChar.m_cLocation.X == g_sPrimeObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPrimeObj[i].m_cLocation.Y))
                        {
                            g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y - 1;
                        }
                        else if ((g_sChar.m_cLocation.X + 1 == g_sObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sObj[i].m_cLocation.Y) || (g_sChar.m_cLocation.X + 1 == g_sPrimeObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPrimeObj[i].m_cLocation.Y))
                        {
                            g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y - 1;
                        }
                    }
                }
                if (GetKeyState(0x41) & 0x800) //check for A input and fall left
                {
                    g_sChar.m_cLocation.X--;
                    for (int i = 0; i < g_iPlatforms; i++) {
                        if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X == g_aPlatformsX[i]) {
                            g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 1;
                        }
                        else if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X + 1 == g_aPlatformsX[i]) {
                            g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 1;
                        }
                    }
                    for (int i = 0; i <= obj; i++)
                    {
                        if (c0[i] != 0)
                        {
                            if ((g_sChar.m_cLocation.X == g_sObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sObj[i].m_cLocation.Y) || (g_sChar.m_cLocation.X == g_sPrimeObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPrimeObj[i].m_cLocation.Y))
                            {
                                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 1;
                            }
                            else if ((g_sChar.m_cLocation.X + 1 == g_sObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sObj[i].m_cLocation.Y) || (g_sChar.m_cLocation.X + 1 == g_sPrimeObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPrimeObj[i].m_cLocation.Y))
                            {
                                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 1;
                            }
                        }
                    }
                }
                if (GetKeyState(0x44) & 0x800) //check for D input and fall right
                {
                    g_sChar.m_cLocation.X++;
                    for (int i = 0; i < g_iPlatforms; i++) {
                        if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X == g_aPlatformsX[i]) {
                            g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 1;
                        }
                        else if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X + 1 == g_aPlatformsX[i]) {
                            g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 1;
                        }
                    }
                    for (int i = 0; i <= obj; i++)
                    {
                        if (c0[i] != 0)
                        {
                            if ((g_sChar.m_cLocation.X == g_sObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sObj[i].m_cLocation.Y) || (g_sChar.m_cLocation.X == g_sPrimeObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPrimeObj[i].m_cLocation.Y))
                            {
                                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 1;
                            }
                            else if ((g_sChar.m_cLocation.X + 1 == g_sObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sObj[i].m_cLocation.Y) || (g_sChar.m_cLocation.X + 1 == g_sPrimeObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPrimeObj[i].m_cLocation.Y))
                            {
                                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 1;
                            }
                        }
                    }
                }
                g_jElapsedTime = 0;
                l++;
            }
        }
    }
    if (g_skKeyEvent[K_41].keyDown && g_sChar.m_cLocation.X > 2 && g_sUltimate == false) //LEFT
    {
        /*Beep(1440, 30);*/
        oneStep = g_sChar.m_cLocation.X - n;
        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - n;
        for (int i = 0; i < g_iPlatforms; i++) {
            if (oneStep == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + n;
            }
            else if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + n;
            }
        }
        for (int i = 0; i <= obj; i++)
        {
            if (c0[i] != 0)
            {
                if ((g_sChar.m_cLocation.X == g_sObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sObj[i].m_cLocation.Y) || (g_sChar.m_cLocation.X == g_sPrimeObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPrimeObj[i].m_cLocation.Y))
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + n;
                }
                else if ((g_sChar.m_cLocation.X + 1 == g_sObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sObj[i].m_cLocation.Y) || (g_sChar.m_cLocation.X + 1 == g_sPrimeObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPrimeObj[i].m_cLocation.Y))
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + n;
                }
            }
        }
    }
    //if (g_skKeyEvent[K_53].keyDown && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1) { //DOWN
    //    /*Beep(1440, 30);*/
    //    g_sChar.m_cLocation.Y++;
    //    for (int x = 0; x < g_iPlatforms; x++) {
    //        if (g_sChar.m_cLocation.X == g_aPlatformsX[x] && g_sChar.m_cLocation.Y == g_aPlatformsY[x]) {
    //            g_sChar.m_cLocation.Y--;
    //        }
    //    }
    //}
    if (g_skKeyEvent[K_44].keyDown && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 3 && g_sUltimate == false) //RIGHT
    {
        /*Beep(1440, 30);*/
        oneStep = g_sChar.m_cLocation.X + n;
        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + n;
        for (int i = 0; i < g_iPlatforms; i++) {
            if (oneStep == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - n;
            }
            else if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - n;
            }
            else if (g_sChar.m_cLocation.X + 1 == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - n;
            }
        }
        for (int i = 0; i <= obj; i++)
        {
            if (c0[i] != 0)
            {
                if ((g_sChar.m_cLocation.X == g_sObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sObj[i].m_cLocation.Y) || (g_sChar.m_cLocation.X == g_sPrimeObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPrimeObj[i].m_cLocation.Y))
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - n;
                }
                else if ((g_sChar.m_cLocation.X + 1 == g_sObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sObj[i].m_cLocation.Y) || (g_sChar.m_cLocation.X + 1 == g_sPrimeObj[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPrimeObj[i].m_cLocation.Y))
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - n;
                }
            }
        }
    }

}

void sneakCharacter() //sneak ability
{
    if (g_skKeyEvent[K_41].keyDown)
    {
        if (GetKeyState(0x53) & 0x800)
        {
            n = 1;
            g_sInvulnerable = true;
        }
        else
        {
            n = 2;
            g_sInvulnerable = false;
        }
    }
    if (g_skKeyEvent[K_44].keyDown)
    {
        if (GetKeyState(0x53) & 0x800)
        {
            n = 1;
            g_sInvulnerable = true;
        }
        else
        {
            n = 2;
            g_sInvulnerable = false;
        }
    }
}

void moveProjectile() //projectile movement animation
{
    double n = 0;
    if (g_sRage == true)
        n = 0.01;
    else
        n = 0.02;
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED && g_mouseEvent.mousePosition.X > g_sChar.m_cLocation.X && g_sProj.m_cLocation.X >= g_sChar.m_cLocation.X) //shoot to right
    { 
        PlaySound(TEXT("DewmShot.wav"), NULL, SND_ALIAS | SND_ASYNC);
        c4 = 205;
        if (g_pElapsedTime > n) 
        {
            if (j != 20)
            {
                /*if (j == 1)
                {
                    Beep(1440, 30);
                }*/
                g_sProj.m_cLocation.X++;
                g_pElapsedTime = 0;
                j++;

            }
            else
            {
                g_sProj.m_cLocation.Y = g_sChar.m_cLocation.Y;
                g_sProj.m_cLocation.X = g_sChar.m_cLocation.X;
                j = 0;
            }
        }
    }
    else if (g_sProj.m_cLocation.X > g_sChar.m_cLocation.X)
    {
        PlaySound(TEXT("DewmShot.wav"), NULL, SND_ALIAS | SND_ASYNC);
        if (j > 0 && j < 20)
        {
            if (g_pElapsedTime > n)
            {
                g_sProj.m_cLocation.X++;
                g_pElapsedTime = 0;
                j++;
            }
        }
        else
        {
            g_sProj.m_cLocation.Y = g_sChar.m_cLocation.Y;
            g_sProj.m_cLocation.X = g_sChar.m_cLocation.X;
            c4 = 0;
            j = 0;
        }
    }
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED && g_mouseEvent.mousePosition.X < g_sChar.m_cLocation.X && g_sProj.m_cLocation.X <= g_sChar.m_cLocation.X) //shoot to left
    { 
        c4 = 205;
        if (g_pElapsedTime > n) 
        {
            if (j != 20)
            {
                /*if (j == 1)
                {
                    Beep(1440, 30);
                }*/
                g_sProj.m_cLocation.X--;
                g_pElapsedTime = 0;
                j++;
            }
            else
            {
                g_sProj.m_cLocation.Y = g_sChar.m_cLocation.Y;
                g_sProj.m_cLocation.X = g_sChar.m_cLocation.X;
                c4 = 0;
                j = 0;
            }
        }
    }
    else if (g_sProj.m_cLocation.X < g_sChar.m_cLocation.X)
    {
        if (j > 0 && j < 20)
        {
            if (g_pElapsedTime > n)
            {
                g_sProj.m_cLocation.X--;
                g_pElapsedTime = 0;
                j++;
            }
        }
        else
        {
            g_sProj.m_cLocation.Y = g_sChar.m_cLocation.Y;
            g_sProj.m_cLocation.X = g_sChar.m_cLocation.X;
            c4 = 0;
            j = 0;
        }
    }
}

void ThorfinnArms(double n1, double n2, int i, int k) //speed of projectile || melee attack, melee attack distance and projectile distance
{
    if (g_skKeyEvent[K_32].keyReleased) //1
    {
        g_sRanged = true;
    }
    else if (g_skKeyEvent[K_31].keyReleased) //2
    {
        g_sRanged = false;
    }
    if (g_sRanged == true)
    {
        if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED && g_mouseEvent.mousePosition.X > g_sChar.m_cLocation.X && g_sProj.m_cLocation.X >= g_sChar.m_cLocation.X && g_slashdelay > 1) //shoot to right
        {
            PlaySound(TEXT("ThorfinnBow.wav"), NULL, SND_ALIAS | SND_ASYNC);
            c4 = 246;
            if (g_pElapsedTime > n1)
            {
                if (j != k)
                {
                    /*if (j == 1)
                    {
                        Beep(1440, 30);
                    }*/
                    g_sProj.m_cLocation.X++;
                    g_pElapsedTime = 0;
                    j++;

                }
                else
                {
                    g_sProj.m_cLocation.Y = g_sChar.m_cLocation.Y;
                    g_sProj.m_cLocation.X = g_sChar.m_cLocation.X;
                    j = 0;
                    c4 = 0;
                    g_slashdelay = 0;
                }
            }
        }
        else if (g_sProj.m_cLocation.X > g_sChar.m_cLocation.X)
        {
            if (j > 0 && j < k)
            {
                if (g_pElapsedTime > n1)
                {
                    g_sProj.m_cLocation.X++;
                    g_pElapsedTime = 0;
                    j++;
                }
            }
            else
            {
                g_sProj.m_cLocation.Y = g_sChar.m_cLocation.Y;
                g_sProj.m_cLocation.X = g_sChar.m_cLocation.X;
                c4 = 0;
                j = 0;
                g_slashdelay = 0;
            }
        }
        if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED && g_mouseEvent.mousePosition.X < g_sChar.m_cLocation.X && g_sProj.m_cLocation.X <= g_sChar.m_cLocation.X && g_slashdelay > 1) //shoot to left
        {
            PlaySound(TEXT("ThorfinnBow.wav"), NULL, SND_ALIAS | SND_ASYNC);
            c4 = 246;
            if (g_pElapsedTime > n1)
            {
                if (j != k)
                {
                    /*if (j == 1)
                    {
                        Beep(1440, 30);
                    }*/
                    g_sProj.m_cLocation.X--;
                    g_pElapsedTime = 0;
                    j++;
                }
                else
                {
                    g_sProj.m_cLocation.Y = g_sChar.m_cLocation.Y;
                    g_sProj.m_cLocation.X = g_sChar.m_cLocation.X;
                    c4 = 0;
                    j = 0;
                    g_slashdelay = 0;
                }
            }
        }
        else if (g_sProj.m_cLocation.X < g_sChar.m_cLocation.X)
        {
            if (j > 0 && j < k)
            {
                if (g_pElapsedTime > n1)
                {
                    g_sProj.m_cLocation.X--;
                    g_pElapsedTime = 0;
                    j++;
                }
            }
            else
            {
                g_sProj.m_cLocation.Y = g_sChar.m_cLocation.Y;
                g_sProj.m_cLocation.X = g_sChar.m_cLocation.X;
                c4 = 0;
                j = 0;
                g_slashdelay = 0;
            }
        }
    }
    else if (g_sRanged == false)
    {
        g_sAttackState = false;
        if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED && g_mouseEvent.mousePosition.X > g_sChar.m_cLocation.X && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 3 && !g_sUltimate && !g_sFocus) //Slash right
        {
            PlaySound(TEXT("ThorfinnAxe.wav"), NULL, SND_ALIAS | SND_ASYNC);
            g_sAttackState = true;
            if (g_sElapsedTime > n2)
            {
                if (s != i)
                {
                    g_sChar.m_cLocation.X++;
                    g_sElapsedTime = 0;
                    s++;
                    for (int i = 0; i < g_iPlatforms; i++) {
                        if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                            g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 1;
                            g_sAttackState = false;
                        }
                    }
                }
                else if (s == i && g_slashdelay > sd)
                {
                    s = 0;
                    g_slashdelay = 0;
                    if (sd == 1)
                    {
                        sd = 0.2;
                        p = 0;
                    }
                    p++;
                    if (p == 3)
                    {
                        sd = 1;
                    }
                }
            }
        }
        if (s > 0 && s < i /*&& g_sChar.m_bActive*/ && g_mouseEvent.mousePosition.X > g_sChar.m_cLocation.X)
        {
            g_sChar.m_cLocation.X++;
            s++;
            for (int i = 0; i < g_iPlatforms; i++) {
                if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 1;
                    g_sAttackState = false;
                }
            }
        }
        if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED && g_mouseEvent.mousePosition.X < g_sChar.m_cLocation.X && g_sChar.m_cLocation.X > 2 && !g_sUltimate && !g_sFocus) //Slash left
        {
            PlaySound(TEXT("ThorfinnAxe.wav"), NULL, SND_ALIAS | SND_ASYNC);
            g_sAttackState = true;
            if (g_sElapsedTime > n2)
            {
                if (s != i)
                {
                    /*if (s == 1)
                    {
                        Beep(1440, 30);
                    }*/
                    g_sChar.m_cLocation.X--;
                    g_sElapsedTime = 0;
                    s++;
                    for (int i = 0; i < g_iPlatforms; i++) {
                        if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                            g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 1;
                            g_sAttackState = false;
                        }
                    }
                }
                else if (s == i && g_slashdelay > sd)
                {
                    s = 0;
                    g_slashdelay = 0;
                    if (sd == 1)
                    {
                        sd = 0.2;
                        p = 0;
                    }
                    p++;
                    if (p == 3)
                    {
                        sd = 1;
                    }
                }
            }
        }
        if (s > 0 && s < i /*&& g_sChar.m_bActive*/ && g_mouseEvent.mousePosition.X < g_sChar.m_cLocation.X)
        {
            g_sChar.m_cLocation.X--;
            s++;
            for (int i = 0; i < g_iPlatforms; i++) {
                if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 1;
                    g_sAttackState = false;
                }
            }
        }

        if (g_staminaregen > 1 && sd != 1)
        {
            p--;
            g_staminaregen = 0;
            if (p < 0)
            {
                p = 0;
            }
        }
    }
}
void dodge(int i) //dodge ability
{
    if (g_skKeyEvent[K_51].keyDown && g_sChar.m_cLocation.X > 2) //dodge left
    {
        g_sInvulnerable = true;
        dashl++;
        for (int i = 0; i < g_iPlatforms; i++) {
            if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 2;
                g_sInvulnerable = false;
            }
        }
    }
    if (g_doElapsedTime > 0.01)
    {
        if (dashl >= 1 && dashl < i && g_sChar.m_cLocation.X > 2)
        {
            g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 2;
            g_doElapsedTime = 0;
            dashl++;
            for (int i = 0; i < g_iPlatforms; i++) {
                if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 2;
                    g_sInvulnerable = false;
                }
            }
        }  
    }
    if (g_slashdelay > 0.2 && dashl >= i)
    { 
        g_sInvulnerable = false;
        dashl = 0;
        g_slashdelay = 0;
    }
    if (g_skKeyEvent[K_45].keyDown && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 3) //dodge right
    {
        g_sInvulnerable = true;
        dashr++;
        for (int i = 0; i < g_iPlatforms; i++) {
            if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]){
                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 2;
                g_sInvulnerable = false;
            }
        }
    }
    if (g_doElapsedTime > 0.01)
    {
        if (dashr >= 1 && dashr < i && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 3)
        {
            g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 2;
            g_doElapsedTime = 0;
            dashr++;
            for (int i = 0; i < g_iPlatforms; i++) {
                if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 2;
                    g_sInvulnerable = false;
                }
            }
        }
    }
    if (g_slashdelay > 0.2 && dashr >= i)
    {
        g_sInvulnerable = false;
        dashr = 0;
        g_slashdelay = 0;
    }
}

void slashAttack(double n, int i) //slash ability
{
    g_sAttackState = false;
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED && g_mouseEvent.mousePosition.X > g_sChar.m_cLocation.X && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 3 && !g_sUltimate && !g_sFocus) //Slash right
    {
        PlaySound(TEXT("SeraphSish.wav"), NULL, SND_ALIAS | SND_ASYNC);
        g_sAttackState = true;
        if (g_sElapsedTime > n)
        {
            if (s != i)
            {
                g_sChar.m_cLocation.X++;
                g_sElapsedTime = 0;
                s++;
                for (int i = 0; i < g_iPlatforms; i++) {
                    if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 1;
                        g_sAttackState = false;
                    }
                }
            }
            else if (s == i && g_slashdelay > sd)
            {
                s = 0;
                g_slashdelay = 0;
                if (sd == 1)
                {
                    sd = 0.2;
                    p = 0;
                }
                p++;
                if (p == 3)
                {
                    sd = 1;
                }
            }
        }
    }
    if (s > 0 && s < i /*&& g_sChar.m_bActive*/ && g_mouseEvent.mousePosition.X > g_sChar.m_cLocation.X)
    {
            g_sChar.m_cLocation.X++;
            s++;
            for (int i = 0; i < g_iPlatforms; i++) {
                if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 1;
                    g_sAttackState = false;
                }
            }
    }
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED && g_mouseEvent.mousePosition.X < g_sChar.m_cLocation.X && g_sChar.m_cLocation.X > 2 && !g_sUltimate && !g_sFocus) //Slash left
    {
        PlaySound(TEXT("SeraphSish.wav"), NULL, SND_ALIAS | SND_ASYNC);
        g_sAttackState = true;
        if (g_sElapsedTime > n)
        {
            if (s != i)
            {
                /*if (s == 1)
                {
                    Beep(1440, 30);
                }*/
                g_sChar.m_cLocation.X--;
                g_sElapsedTime = 0;
                s++;
                for (int i = 0; i < g_iPlatforms; i++) {
                    if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 1;
                        g_sAttackState = false;
                    }
                }
            }
            else if (s == i && g_slashdelay > sd)
            {
                s = 0;
                g_slashdelay = 0;
                if (sd == 1)
                {
                    sd = 0.2;
                    p = 0;
                }
                p++;
                if (p == 3)
                {
                    sd = 1;
                }
            }  
        }  
    }
    if (s > 0 && s < i /*&& g_sChar.m_bActive*/ && g_mouseEvent.mousePosition.X < g_sChar.m_cLocation.X)
    {
        g_sChar.m_cLocation.X--;
        s++;
        for (int i = 0; i < g_iPlatforms; i++) {
            if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 1;
                g_sAttackState = false;
            }
        }
    }
    
    if (g_staminaregen > 1 && sd != 1)
    {
        p--;
        g_staminaregen = 0;
        if (p < 0)
        {
            p = 0;
        }
    }
}

void downslam() //down slam ability
{
    if (g_skKeyEvent[K_53].keyDown && p != 3 && !g_sUltimate) //DOWN slam attack with S
    {
        l = 0;
        slam++;
    }
    if (g_sElapsedTime > 0.02)
    {
        if (slam > 0)
        {
            g_sAttackState = true;
        }
        if (slam >= 1 && slam < 3)
        {
            g_sChar.m_cLocation.Y--;
            for (int i = 0; i < g_iPlatforms; i++) {
                if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X == g_aPlatformsX[i]) {
                    g_sChar.m_cLocation.Y++;
                }
            }
            g_sElapsedTime = 0;
            slam++;
        }
        for (int i = 0; i <= g_iPlatforms; i++) {
            if (slam >= 3 && slam < 6 && g_sChar.m_cLocation.Y + 1 != g_aPlatformsY[i] && g_sChar.m_cLocation.X == g_aPlatformsX[i])
            {
                g_sChar.m_cLocation.Y++;
                slam++;
                g_sElapsedTime = 0;
                if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X == g_aPlatformsX[i])
                {
                    g_sChar.m_cLocation.Y--;
                }
            }
            if (slam == 6)
            {
                if (g_sChar.m_cLocation.Y + 1 != g_aPlatformsY[i] && g_sChar.m_cLocation.X == g_aPlatformsX[i])
                {
                    g_sChar.m_cLocation.Y++;
                }
                else if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X == g_aPlatformsX[i])
                {
                    g_sChar.m_cLocation.Y--;
                }
                else
                {
                    slam = 0;
                    for (int i = 0; i < ne; i++)
                    {
                        if (g_sChar.m_cLocation.X + 3 >= g_sEnemy[i].m_cLocation.X && g_sEnemy[i].m_cLocation.X > g_sChar.m_cLocation.X)
                        {
                            g_sEnemy[i].m_cLocation.X = g_sEnemy[i].m_cLocation.X + 3;
                            g_sEnemy[i].m_dHealth = g_sEnemy[i].m_dHealth - 2;
                            g_stun = -1;
                        }
                        if (g_sChar.m_cLocation.X - 3 <= g_sEnemy[i].m_cLocation.X && g_sEnemy[i].m_cLocation.X < g_sChar.m_cLocation.X)
                        {
                            g_sEnemy[i].m_cLocation.X = g_sEnemy[i].m_cLocation.X - 3;
                            g_sEnemy[i].m_dHealth = g_sEnemy[i].m_dHealth - 2;
                            g_stun = -1;
                        }
                         if (g_sChar.m_cLocation.X + 3 >= g_sObj[i].m_cLocation.X && g_sObj[i].m_cLocation.X > g_sChar.m_cLocation.X)
                        {
                            g_sObj[i].m_cLocation.X = g_sObj[i].m_cLocation.X + 3;
                            g_sObj[i].m_dHealth = g_sObj[i].m_dHealth - 2;
                            g_stun = -1;
                        }
                        if (g_sChar.m_cLocation.X - 3 <= g_sObj[i].m_cLocation.X && g_sObj[i].m_cLocation.X < g_sChar.m_cLocation.X)
                        {
                            g_sObj[i].m_cLocation.X = g_sObj[i].m_cLocation.X - 3;
                            g_sObj[i].m_dHealth = g_sObj[i].m_dHealth - 2;
                            g_stun = -1;
                        }
                    }
                    g_sAttackState = false;
                }
            }
            
        }
    }
}

void seraphUlt() //Seraph's ultimate animation
{
    if (g_sUltimate == true)
    {
        g_sChar.m_dMana = 200;
        if (su != 359)
        {
            if (g_suElapsedTime > 0.02)
            {
                if (su < 13)
                {
                    g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y - 1;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 13 && su < 41)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 2;
                    g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y + 1;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 41 && su < 55)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 7;
                    g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y - 1;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 55 && su < 104)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 2;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 104 && su < 118)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 7;
                    g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y + 1;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 118 && su < 146)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 2;
                    g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y - 1;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 146 && su < 159) //ends at x = 42, y = 14
                {
                    g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y + 1;
                    g_suElapsedTime = 0;
                    su++;
                    g_udElapsedTime = 0;
                }
                if (g_udElapsedTime > 3)
                {
                    if (su >= 159 && su < 173) //x = 42, y = 28
                    {
                        g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y + 1;
                        g_suElapsedTime = 0;
                        su++;
                    }
                    if (su >= 173 && su < 199) //x = 2, y = 2
                    {
                        g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y - 1;
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 1;
                        g_suElapsedTime = 0;
                        su++;
                    }
                    if (su >= 199 && su < 227) //x = 2, y = 28
                    {
                        g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y + 1;
                        g_suElapsedTime = 0;
                        su++;
                    }
                    if (su >= 227 && su < 237)
                    {
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 1;
                        g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y - 3;
                        g_suElapsedTime = 0;
                        su++;
                    }
                    if (su >= 237 && su < 247)
                    {
                        g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y + 3;
                        g_suElapsedTime = 0;
                        su++;
                    }
                    if (su >= 247 && su < 257)
                    {
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 1;
                        g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y - 3;
                        g_suElapsedTime = 0;
                        su++;
                    }
                    if (su >= 257 && su < 267)
                    {
                        g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y + 3;
                        g_suElapsedTime = 0;
                        su++;
                    }
                    if (su >= 267 && su < 277)
                    {
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 1;
                        g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y - 3;
                        g_suElapsedTime = 0;
                        su++;
                    }
                    if (su >= 277 && su < 287)
                    {
                        g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y + 3;
                        g_suElapsedTime = 0;
                        su++;
                    }
                    if (su >= 287 && su < 297)
                    {
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 1;
                        g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y - 3;
                        g_suElapsedTime = 0;
                        su++;
                    }
                    if (su >= 297 && su < 307)
                    {
                        g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y + 3;
                        g_suElapsedTime = 0;
                        su++;
                    }
                    if (su >= 307 && su < 317)
                    {
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 1;
                        g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y - 3;
                        g_suElapsedTime = 0;
                        su++;
                    }
                    if (su >= 317 && su < 327)
                    {
                        g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y + 3;
                        g_suElapsedTime = 0;
                        su++;
                    }
                    if (su >= 327 && su < 337)
                    {
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 1;
                        g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y - 3;
                        g_suElapsedTime = 0;
                        su++;
                    }
                    if (su >= 337 && su < 347)
                    {
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 1;
                        g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y + 3;
                        g_suElapsedTime = 0;
                        su++;
                    }
                    if (su >= 347 && su < 356) // y = 3
                    {
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 1;
                        g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y - 3;
                        g_suElapsedTime = 0;
                        su++;
                    }
                    if (su >= 356 && su < 359)
                    {
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 16;
                        g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y + 3;
                        g_suElapsedTime = 0;
                        su++;
                    }
                }
                if (su == 359) 
                {
                    g_udElapsedTime = 0;
                }
            }
        }
        else if (g_udElapsedTime > 1.5 && (g_udElapsedTime < 1.5 + g_dDeltaTime))
        { 
        //kills all enemies in map and -100 to boss hp
            for (int i = 0; i < ne; i++)
            {
                g_sEnemy[i].m_dHealth = 0;
            }
            g_sBoss1[0].m_dHealth = g_sBoss1[0].m_dHealth - 150;
            g_sBoss2.m_dHealth = g_sBoss2.m_dHealth - 150;
        }
        else if (g_udElapsedTime > 3)
        {
            l = 1;
            g_sChar.m_dHealth = 50;
            g_sChar.m_dMana = 0;
            g_sUltimate = false;
        }
    }
}

void focusAttack() //focus ability enhancement for Gin
{
    int move;
    if (g_sFocus == true)
    {
        if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED && (g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 3 || g_sChar.m_cLocation.X > 2))
        {
            PlaySound(TEXT("GinSlice.wav"), NULL, SND_ALIAS | SND_ASYNC);
            f++;
        }
        if (f < 25)
        {
            if (g_suElapsedTime > 0.001)
            {
                if (f >= 1 && f < 25 && g_mouseEvent.mousePosition.X > g_sChar.m_cLocation.X)
                {
                    move = g_sChar.m_cLocation.X + 2;
                    for (int i = 0; i < g_iPlatforms; i++) {
                        if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && move-1 == g_aPlatformsX[i]) {
                            move = g_sChar.m_cLocation.X - 1;
                        }
                        if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && move == g_aPlatformsX[i]) {
                            move = g_sChar.m_cLocation.X - 2;
                        }
                    }
                    g_sChar.m_cLocation.X = move;
                    su++;
                    f++;
                    g_suElapsedTime = 0;
                    g_slashdelay = 0;
                }

                if (f >= 1 && f < 25 && g_mouseEvent.mousePosition.X < g_sChar.m_cLocation.X)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 2;
                    for (int i = 0; i < g_iPlatforms; i++) {
                        if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X == g_aPlatformsX[i]) {
                            g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 2;
                        }
                    }
                    su++;
                    f++;
                    g_suElapsedTime = 0;
                    g_slashdelay = 0;
                }

            }
        }
        else if (f >= 25 && g_slashdelay > 0.3)
        {
            f = 0;
            g_sChar.m_dMana = g_sChar.m_dMana - 2;
        }

    }
}

void focusUlt() //Gin's Focus ultimate
{
    if (g_sFocus == true && g_skKeyEvent[K_55].keyReleased)
    {
        g_sUltimate = true;
        g_sChar.m_cLocation.X = 3;
        g_sFocus = false;
    }
    if (g_sUltimate == true)
    {
        if (g_sChar.m_dMana > 25)
        {

            for (int i = 0; i < g_iPlatforms; i++)
            {
                if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y + 1 == g_aPlatformsY[i])
                {
                    su++;
                    break;
                }
            }
        }
        if (su < 1095) //slash back and forth 10 times at light speed
        {
            g_sChar.m_dMana = 75;
            if (g_suElapsedTime > fe)
            {
                if (su < 55)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 3;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 55 && su < 110)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 3;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 110 && su < 165)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 3;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 165 && su < 220)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 3;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 220 && su < 275)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 3;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 275 && su < 330)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 3;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 330 && su < 385)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 3;
                    g_suElapsedTime = 0;
                    su++;
                    fe = 0.005;
                }
                if (su >= 385 && su < 440)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 3;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 440 && su < 495)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 3;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 495 && su < 550)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 3;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 550 && su < 605)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 3;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 605 && su < 660)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 3;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 660 && su < 715)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 3;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 715 && su < 770)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 3;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 770 && su < 825)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 3;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 825 && su < 880)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 3;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 880 && su < 935)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 3;
                    g_suElapsedTime = 0;
                    su++;

                }
                if (su >= 935 && su < 990)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 3;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 990 && su < 1045)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 3;
                    g_suElapsedTime = 0;
                    su++;
                }
                if (su >= 1045 && su < 1095)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 3;
                    g_suElapsedTime = 0;
                    su++;
                }

            }
        }
        else
        {
            if (su == 1095 && g_udElapsedTime != 0)
            {
                g_udElapsedTime = 0;
                su++;
            }
            if (g_udElapsedTime > 3 && g_udElapsedTime < 3 + g_dDeltaTime)
            {
                //kill all enemies in Y axis and -100 boss in Y axis
                for (int i = 0; i < ne; i++)
                {
                    if (g_sEnemy[i].m_cLocation.Y == g_sChar.m_cLocation.Y)
                    {
                        g_sEnemy[i].m_dHealth = 0;
                    }
                }
                if (g_sBoss1[0].m_cLocation.Y == g_sChar.m_cLocation.Y)
                {
                    g_sBoss1[0].m_dHealth = g_sBoss1[0].m_dHealth - 50;
                }

            }
            if (g_udElapsedTime > 4)
            {
                l = 1;
                g_sUltimate = false;
                g_sChar.m_dMana = 0;
            }
        }

    }

}

void spiritUlt() //summon bear entity
{
    if (g_sSpirit == false) //check if spirit is false before summoning
    {
        g_sBearSpirit[0].m_cLocation.X = g_sChar.m_cLocation.X + 2;
        g_sBearSpirit[0].m_cLocation.Y = g_sChar.m_cLocation.Y;
        g_sBearSpirit[1].m_cLocation.X = g_sChar.m_cLocation.X + 2;
        g_sBearSpirit[1].m_cLocation.Y = g_sChar.m_cLocation.Y - 1;
    }
    else if (g_sSpirit == true) //bear goes after any enemy in Y axis and attacks
    {
        for (int i = 0; i <= ne; i++)
        {
            if (g_sBearSpirit[0].m_cLocation.Y == g_sEnemy[i].m_cLocation.Y)
            {
                if (g_sBearSpirit[0].m_cLocation.X < g_sEnemy[i].m_cLocation.X && su == 0) //Go right
                {
                    if (g_suElapsedTime > 0.05)
                    {
                        g_sBearSpirit[0].m_cLocation.X = g_sBearSpirit[0].m_cLocation.X + 1;
                        g_sBearSpirit[1].m_cLocation.X = g_sBearSpirit[1].m_cLocation.X + 1;
                        g_suElapsedTime = 0;
                    }
                }
                if (g_sBearSpirit[0].m_cLocation.X > g_sEnemy[i].m_cLocation.X && su == 0) //Go left
                {
                    if (g_suElapsedTime > 0.05)
                    {
                        g_sBearSpirit[1].m_cLocation.X = g_sBearSpirit[1].m_cLocation.X - 1;
                        g_sBearSpirit[0].m_cLocation.X = g_sBearSpirit[0].m_cLocation.X - 1;
                        g_suElapsedTime = 0;
                    }
                }
                if ((g_sBearSpirit[0].m_cLocation.X + 3 == g_sEnemy[i].m_cLocation.X || g_sBearSpirit[0].m_cLocation.X == g_sBoss1[0].m_cLocation.X) || su > 1 && g_sEnemy[i].m_dHealth >= 0)
                {
                    su++;
                    if (g_suElapsedTime > 0.05)
                    {
                        if (su >= 1 && su < 5)
                        {
                            g_sBearSpirit[0].m_cLocation.X = g_sBearSpirit[0].m_cLocation.X - 1;
                            g_sBearSpirit[1].m_cLocation.X = g_sBearSpirit[1].m_cLocation.X - 1;
                            su++;
                            g_suElapsedTime = 0;
                        }
                        if (su >= 5 && su < 20)
                        {
                            g_sBearSpirit[0].m_cLocation.X = g_sBearSpirit[0].m_cLocation.X + 2;
                            g_sBearSpirit[1].m_cLocation.X = g_sBearSpirit[1].m_cLocation.X + 2;
                            su++;
                            g_suElapsedTime = 0;
                        }
                        if (su >= 20 && su < 35)
                        {
                            g_sBearSpirit[0].m_cLocation.X = g_sBearSpirit[0].m_cLocation.X - 2;
                            g_sBearSpirit[1].m_cLocation.X = g_sBearSpirit[1].m_cLocation.X - 2;
                            su++;
                            g_suElapsedTime = 0;
                            g_sEnemy[i].m_dHealth = g_sEnemy[i].m_dHealth - 1;
                        }
                        if (g_sEnemy[i].m_dHealth >= 0 && su >= 35)
                        {
                            g_suElapsedTime = 0;
                            su = 5;
                        }
                        
                    }
                    if (su >= 35 && g_sEnemy[i].m_dHealth >= 0 && (g_sEnemy[i].m_cLocation.X > g_sBearSpirit[0].m_cLocation.X || g_sEnemy[i].m_cLocation.X < g_sBearSpirit[0].m_cLocation.X))
                    {
                        su = 0;
                    }
                    if (g_sBearSpirit[0].m_cLocation.X == g_sEnemy[i].m_cLocation.X && g_sBearSpirit[0].m_cLocation.Y == g_sEnemy[i].m_cLocation.Y)
                    {
                        g_sEnemy[i].m_dHealth = g_sEnemy[i].m_dHealth - 5;
                    }
                }
            }
        }
    }
}


void setdamage()
{
    //Check damage for enemy projectile
    for (int i = 0; i < ne; i++)
    {
        if (g_sEnemy[i].m_cLocation.X == g_sChar.m_cLocation.X && g_sEnemy[i].m_cLocation.Y == g_sChar.m_cLocation.Y || g_sEnemy[i].m_cLocation.X == g_sChar.m_cLocation.X + 1 && g_sEnemy[i].m_cLocation.Y == g_sChar.m_cLocation.Y)
        {
            if (g_hElapsedTime > 0.3)
            {
                if (g_sEnemy[i].m_dHealth > 0 && !g_sUltimate && !g_sRage && !g_sAttackState && !g_sInvulnerable && !g_sFocus && !g_sSpirit)
                {
                    g_sChar.m_dHealth = g_sChar.m_dHealth - 5;
                }
                g_hElapsedTime = 0;
                if (g_sAttackState == true)
                {
                    g_sEnemy[i].m_dHealth = g_sEnemy[i].m_dHealth - 2; //program recognises as 2 damage
                }
                if (g_sFocus == true || g_sSpirit == true)
                {
                    g_sEnemy[i].m_dHealth = g_sEnemy[i].m_dHealth - 5;
                }
            }
        }
        if ((g_sEProj[i].m_cLocation.X == g_sChar.m_cLocation.X && g_sEProj[i].m_cLocation.Y == g_sChar.m_cLocation.Y || g_sEProj[i].m_cLocation.X == g_sChar.m_cLocation.X + 1 && g_sEProj[i].m_cLocation.Y == g_sChar.m_cLocation.Y) && g_sEnemy[i].m_dHealth > 0)
        {
            if (g_hElapsedTime > 0.5)
            {
                if (g_sChar.m_dHealth > 0 && !g_sUltimate && !g_sRage && !g_sAttackState && !g_sInvulnerable && !g_sFocus && !g_sSpirit)
                {
                    g_sChar.m_dHealth = g_sChar.m_dHealth - 5;
                }
                g_hElapsedTime = 0;
                
            }
            if (!g_sInvulnerable)
            {
                g_sEProj[i].m_cLocation = g_sEnemy[i].m_cLocation;
            }
        }
    }
    //Boss 1 damage
    if ((g_sBoss1[0].m_cLocation.X == g_sChar.m_cLocation.X || g_sBoss1[0].m_cLocation.X + 1 == g_sChar.m_cLocation.X || g_sBoss1[0].m_cLocation.X + 2 == g_sChar.m_cLocation.X) && g_sBoss1[0].m_cLocation.Y == g_sChar.m_cLocation.Y)
    {
        if (g_hElapsedTime > 0.2)
        {
            if (g_sBoss1[0].m_dHealth > 0 && !g_sUltimate && !g_sRage && !g_sAttackState && !g_sInvulnerable && !g_sFocus && !g_sSpirit)
            {
                g_sChar.m_dHealth = g_sChar.m_dHealth - 10;
            }
            g_hElapsedTime = 0;
        }
        if (g_sAttackState == true)
        {
            g_sBoss1[0].m_dHealth = g_sBoss1[0].m_dHealth - 1; //program recognises as 2 damage
        }
        if (g_sFocus == true || g_sSpirit == true)
        {
            g_sBoss1[0].m_dHealth = g_sBoss1[0].m_dHealth - 10;
        }
    }
    //Boss 2 damage
    if ((g_sBoss2.m_cLocation.X == g_sChar.m_cLocation.X || g_sBoss2.m_cLocation.X + 1 == g_sChar.m_cLocation.X || g_sBoss2.m_cLocation.X + 2 == g_sChar.m_cLocation.X) && g_sBoss2.m_cLocation.Y == g_sChar.m_cLocation.Y)
    {
        if (g_hElapsedTime > 0.2)
        {
            if (g_sBoss2.m_dHealth > 0 && !g_sUltimate && !g_sRage && !g_sAttackState && !g_sInvulnerable && !g_sFocus && !g_sSpirit)
            {
                g_sChar.m_dHealth = g_sChar.m_dHealth - 10;
            }
            g_hElapsedTime = 0;
        }
        if (g_sAttackState == true)
        {
            g_sBoss2.m_dHealth = g_sBoss2.m_dHealth - 1; //program recognises as 2 damage
        }
        if (g_sFocus == true || g_sSpirit == true)
        {
            g_sBoss2.m_dHealth = g_sBoss2.m_dHealth - 10;
        }
    }
    //Melee damage for obj
    for (int i = 0; i < obj; i++)
    {
        if (g_sChar.m_cLocation.X + 2 >= g_sObj[i].m_cLocation.X && g_sChar.m_cLocation.X < g_sObj[i].m_cLocation.X && g_sAttackState) //right slash attack
        {
            if (g_hElapsedTime > 0.2)
            {
                g_sObj[i].m_dHealth = g_sObj[i].m_dHealth - 2;
                g_hElapsedTime = 0;
                
            }
        }
        if (g_sChar.m_cLocation.X - 2 <= g_sObj[i].m_cLocation.X && g_sChar.m_cLocation.X > g_sObj[i].m_cLocation.X && g_sAttackState) //left slash attack
        {
            if (g_hElapsedTime > 0.2)
            {
                g_sObj[i].m_dHealth = g_sObj[i].m_dHealth - 2;
                g_hElapsedTime = 0;
            }
        }
    }
    for (int i = 0; i < ne; i++)  //check projectile damage to enemy
    {
        if ((g_sProj.m_cLocation.X == g_sEnemy[i].m_cLocation.X && g_sProj.m_cLocation.Y == g_sEnemy[i].m_cLocation.Y) && (g_sProj.m_cLocation.X > g_sChar.m_cLocation.X || g_sProj.m_cLocation.X < g_sChar.m_cLocation.X) && c5[i] != 0)
        {
            if (!g_sRage)
            {
                g_sEnemy[i].m_dHealth--;
            }
            else
            {
                g_sEnemy[i].m_dHealth = g_sEnemy[i].m_dHealth - 5;
            }
            j = 0;
            g_sProj.m_cLocation = g_sChar.m_cLocation;
            if (characterSelect == 3)
            {
                g_slashdelay = 0;
            }

        }
        if (g_sEnemy[i].m_dHealth <= 0)
        {
            c5[i] = 0;
            enemyColor[i] = 0x4A;
        }
    }
    //to Boss 1
    if (g_sProj.m_cLocation.X == g_sBoss1[0].m_cLocation.X && g_sProj.m_cLocation.Y == g_sBoss1[0].m_cLocation.Y && (g_sProj.m_cLocation.X > g_sChar.m_cLocation.X || g_sProj.m_cLocation.X < g_sChar.m_cLocation.X) && c6 != 0)
    {
        if (!g_sRage)
        {
            g_sBoss1[0].m_dHealth--;
        }
        else
        {
            g_sBoss1[0].m_dHealth = g_sBoss1[0].m_dHealth - 2;
        }
        if (characterSelect == 3)
        {
            g_slashdelay = 0;
        }
        j = 0;
        g_sProj.m_cLocation = g_sChar.m_cLocation;
    }
    //to Boss 2
    if (g_sProj.m_cLocation.X == g_sBoss2.m_cLocation.X && g_sProj.m_cLocation.Y == g_sBoss2.m_cLocation.Y && (g_sProj.m_cLocation.X > g_sChar.m_cLocation.X || g_sProj.m_cLocation.X < g_sChar.m_cLocation.X) && c10 != 0)
    {
        if (!g_sRage)
        {
            g_sBoss2.m_dHealth--;
        }
        else
        {
            g_sBoss2.m_dHealth = g_sBoss2.m_dHealth - 2;
        }
        if (characterSelect == 3)
        {
            g_slashdelay = 0;
        }
        j = 0;
        g_sProj.m_cLocation = g_sChar.m_cLocation;
    }
    //to object
    for (int i = 0; i <= obj; i++)
    {
        if ((g_sProj.m_cLocation.X == g_sObj[i].m_cLocation.X && g_sProj.m_cLocation.Y == g_sObj[i].m_cLocation.Y) && (g_sProj.m_cLocation.X > g_sChar.m_cLocation.X || g_sProj.m_cLocation.X < g_sChar.m_cLocation.X) && c0[i] != 0)
        {
            if (!g_sRage)
            {
                g_sObj[i].m_dHealth--;
            }
            else
            {
                g_sObj[i].m_dHealth = g_sObj[i].m_dHealth - 5;
            }
            j = 0;
            g_sProj.m_cLocation = g_sChar.m_cLocation;
            if (characterSelect == 3)
            {
                g_slashdelay = 0;
            }
        }
    }
    //For platforms
    for (int i = 0; i < g_iPlatforms; i++)
    {
        if ((g_sProj.m_cLocation.X == g_aPlatformsX[i] && g_sProj.m_cLocation.Y == g_aPlatformsY[i]) && (g_sProj.m_cLocation.X > g_sChar.m_cLocation.X || g_sProj.m_cLocation.X < g_sChar.m_cLocation.X))
        {
            j = 0;
            g_sProj.m_cLocation = g_sChar.m_cLocation;
            if (characterSelect == 3)
            {
                g_slashdelay = 0;
            }
        }
    }
    if (g_sBoss1[0].m_dHealth <= 0)
    {
        c6 = 0;
        c7 = 0;
        boss1 = std::string(3, c6) + c7;
        c8 = 0;
        c9 = 0;
        boss2 = std::string(3, c8) + c9;
        bossColor[0] = BGcolor;
    }
    if (g_sBoss2.m_dHealth <= 0)
    {
        c10 = 0;
        c11 = 0;
        boss1 = std::string(3, c10) + c11;
        bossColor[1] = BGcolor;
    }
    if (g_sChar.m_cLocation.Y > g_Console.getConsoleSize().Y)
        g_sChar.m_dHealth--;
    if (g_sChar.m_dHealth <= 0)
    {
        g_sChar.m_dHealth = 0;
    }
}

void setUltimate(int M) //setting ultimate meter and mechanics
{
    if (g_sChar.m_dMana < M && !g_sUltimate && !g_sRage && !g_sFocus && !g_sSpirit) //checks if Player mana is less than 50 and is not in rage mode
    {
        if (g_uElapsedTime > 0.5) //increase mana by 1 every * second
        {
            g_uElapsedTime = 0;
            g_sChar.m_dMana++;
        }
        /*if (g_sEnemy.m_dHealth == 0)
        {
            g_sChar.m_dMana++;
        }*/
    }
    else if (g_sUltimate || g_sRage || g_sFocus || g_sSpirit)
    {
        if (g_uElapsedTime > 0.5)
        {
            g_uElapsedTime = 0;
            g_sChar.m_dMana--;
        }
        if (g_sChar.m_dMana <= 0)
        {
            g_sChar.m_dMana = 0;
            if (characterSelect == 0)
            {
                g_sRage = !g_sRage;
            }
            if (characterSelect == 1)
            {
                g_sUltimate = !g_sUltimate;
            }
            if (characterSelect == 2)
            {
                g_sFocus = !g_sFocus;
            }
            if (characterSelect == 3)
            {
                g_sSpirit = !g_sSpirit;
            }
        }
    }
    if (g_sChar.m_dMana == M && g_skKeyEvent[K_SPACE].keyReleased) //activate ultimate
    {
        if (characterSelect == 0)
        {
            g_sRage = !g_sRage;
        }
        if (characterSelect == 1)
        {
            g_sChar.m_cLocation.X = x / 2 + 7;
            g_sChar.m_cLocation.Y = y / 2;
            g_sUltimate = !g_sUltimate;
        }
        if (characterSelect == 2)
        {
            g_sFocus = !g_sFocus;
        }
        if (characterSelect == 3)
        {
            g_sSpirit = !g_sSpirit;
        }
        su = 0;
    }
    if (g_sChar.m_dMana < M && g_skKeyEvent[K_SPACE].keyReleased && g_sSpirit == true)
    {
        g_sSpirit = !g_sSpirit;
        su = 0;
    }

}

void healthPotion(int i) //use health potion
{
    if (g_sChar.m_cLocation.X == g_sPotion[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPotion[i].m_cLocation.Y && cO[i] != 0)
    {
        g_sChar.m_dHealth = g_sChar.m_dHealth + 10;
        cO[i] = 0;
        PotionColor[i] = BGcolor;
    }
    
}

void moveEnemy(int &i, char a, int n, double t, int d, int e) //enemy movement mechanic
{
    if (c5[e] != 0 && !g_sUltimate && !g_sFocus)
    {
        if (g_stun > 2)
        {
            if (g_eElapsedTime[e] > t)
            {
                if (i < n && a == 'r') //direction = right
                {
                    g_sEnemy[e].m_cLocation.X = g_sEnemy[e].m_cLocation.X + 1;
                    g_eElapsedTime[e] = 0;
                    i++;
                }
                else if (a == 'r') //go back
                {
                    g_sEnemy[e].m_cLocation.X = g_sEnemy[e].m_cLocation.X - 1;
                    g_eElapsedTime[e] = 0;
                    if (g_sEnemy[e].m_cLocation.X <= d)
                    {
                        i = 0;
                    }
                }
                if (i < n && a == 'l') //direction = left
                {
                    g_sEnemy[e].m_cLocation.X = g_sEnemy[e].m_cLocation.X - 1;
                    g_eElapsedTime[e] = 0;
                    i++;
                }
                else if (a == 'l') //go back
                {
                    g_sEnemy[e].m_cLocation.X = g_sEnemy[e].m_cLocation.X + 1;
                    g_eElapsedTime[e] = 0;
                    if (g_sEnemy[e].m_cLocation.X >= d)
                    {
                        i = 0;
                    }
                }
            }
        }

        //for (int k = 0; k < ne; k++)
        //{
        //    if (g_mouseEvent.mousePosition.X > g_sChar.m_cLocation.X && g_sChar.m_cLocation.X + 2 >= g_sEnemy[k].m_cLocation.X && g_sChar.m_cLocation.X < g_sEnemy[k].m_cLocation.X && g_sChar.m_cLocation.Y == g_sEnemy[k].m_cLocation.Y && g_sAttackState == true) //push right
        //    {
        //        g_sEnemy[k].m_cLocation.X = g_sEnemy[k].m_cLocation.X + 1;
        //        i = i + 1;
        //        stun++;
        //        for (int i = 0; i <= g_iPlatforms; i++)
        //        {
        //            if (g_sEnemy[k].m_cLocation.Y + 1 != g_aPlatformsY[i])
        //            {
        //                g_sEnemy[k].m_cLocation.X = g_sEnemy[k].m_cLocation.X - 1;
        //                i = i - 1;
        //            }
        //        }
        //    }
        //    else if (g_mouseEvent.mousePosition.X < g_sChar.m_cLocation.X && g_sChar.m_cLocation.X - 1 <= g_sEnemy[k].m_cLocation.X && g_sChar.m_cLocation.X > g_sEnemy[k].m_cLocation.X && g_sChar.m_cLocation.Y == g_sEnemy[k].m_cLocation.Y && g_sAttackState == true) //push left
        //    {
        //        g_sEnemy[k].m_cLocation.X = g_sEnemy[k].m_cLocation.X - 1;
        //        i = i - 1;
        //        stun++;
        //        if (g_sEnemy[k].m_cLocation.X - 1 == d)
        //        {
        //            n++;
        //            for (int i = 0; i <= g_iPlatforms; i++)
        //            {
        //                if (g_sEnemy[k].m_cLocation.Y + 1 != g_aPlatformsY[i])
        //                {
        //                    g_sEnemy[k].m_cLocation.X = g_sEnemy[k].m_cLocation.X + 1;
        //                    n--;
        //                }
        //            }
        //        }
        //        else if (g_sEnemy[k].m_cLocation.X - 2 == d)
        //        {
        //            n = n + 2;
        //            for (int i = 0; i <= g_iPlatforms; i++)
        //            {
        //                if (g_sEnemy[k].m_cLocation.Y + 1 != g_aPlatformsY[i])
        //                {
        //                    g_sEnemy[k].m_cLocation.X = g_sEnemy[k].m_cLocation.X + 1;
        //                    n = n - 2;
        //                }
        //            }
        //        }
        //    }
        //    else if (stun > 1)
        //    {
        //        g_stun = 0;
        //        stun = 0;
        //    }
        //    if (slam == 6 && g_sChar.m_cLocation.X + 2 == g_sEnemy[k].m_cLocation.X)
        //    {
        //        i = i + 3;

        //    }
        //    if (slam == 6 && g_sChar.m_cLocation.X - 2 == g_sEnemy[k].m_cLocation.X)
        //    {
        //        i = i - 3;
        //    }
        //}
    }
}

void moveBoss(int n, double t, double t2, int d) //Boss movement mechanic
{
    if (g_sBoss1[0].m_cLocation.Y - 4 <= g_sChar.m_cLocation.Y)
    { 
        if (g_sBoss1[0].m_dHealth > 0 && !g_sUltimate && !g_sFocus)
        {
            if (g_sBoss1[0].m_dHealth <= 150)
            {
                t2 = t2 / 3;
                t = t / 2;
            }
            if (g_bElapsedTime > t)
            {
                if (k != n)
                {
                    if (g_delay1 > t2) //go to right
                    {
                        g_sBoss1[0].m_cLocation.X = g_sBoss1[0].m_cLocation.X + 2;
                        g_sBoss1[1].m_cLocation.X = g_sBoss1[1].m_cLocation.X + 2;
                        g_bElapsedTime = 0;
                        k++;
                        g_delay2 = 0;
                    }
                }
                else
                {
                    if (g_delay2 > t2) //go to left
                    {
                        g_sBoss1[0].m_cLocation.X = g_sBoss1[0].m_cLocation.X - 2;
                        g_sBoss1[1].m_cLocation.X = g_sBoss1[1].m_cLocation.X - 2;
                        g_bElapsedTime = 0;
                        g_delay1 = 0;
                        if (g_sBoss1[0].m_cLocation.X <= d)
                        {
                            k = 0;
                        }
                    }
                }
            }
        }
        //boss jump mechanic prototype
        //if (m != 5)
        //{
        //    if (g_bjElapsedTime > 0.03)
        //    {
        //        if (m < 2.5 && g_sBossP1.m_cLocation.X < g_sChar.m_cLocation.X && g_sBossP1.m_cLocation.X + 10 >= g_sChar.m_cLocation.X) //jump to right
        //        {
        //            g_sBossP1.m_cLocation.X++;
        //            g_sBossP1.m_cLocation.Y--;
        //            g_sBossP2.m_cLocation.X++;
        //            g_sBossP2.m_cLocation.Y--;
        //            m = m + 0.5;
        //            g_bjElapsedTime = 0;
        //            n = n + 5;
        //            d = d + 5;
        //        }
        //        else if (m < 2.5 && g_sBossP1.m_cLocation.X > g_sChar.m_cLocation.X && g_sBossP1.m_cLocation.X - 10 <= g_sChar.m_cLocation.X ) //jump to left
        //        {
        //            g_sBossP1.m_cLocation.X--;
        //            g_sBossP1.m_cLocation.Y--;
        //            g_sBossP2.m_cLocation.X--;
        //            g_sBossP2.m_cLocation.Y--;
        //            m = m + 0.5;
        //            g_bjElapsedTime = 0;
        //        }
        //        if (m >= 2.5 && m < 5 && g_sBossP1.m_cLocation.X <= g_sChar.m_cLocation.X)
        //        {
        //            g_sBossP1.m_cLocation.X++;
        //            g_sBossP1.m_cLocation.Y++;
        //            g_sBossP2.m_cLocation.X++;
        //            g_sBossP2.m_cLocation.Y++;
        //            m = m + 0.5;
        //            g_bjElapsedTime = 0;
        //        }
        //        if (m >= 2.5 && m < 5 && g_sBossP1.m_cLocation.X >= g_sChar.m_cLocation.X)
        //        {
        //            g_sBossP1.m_cLocation.X--;
        //            g_sBossP1.m_cLocation.Y++;
        //            g_sBossP2.m_cLocation.X--;
        //            g_sBossP2.m_cLocation.Y++;
        //            m = m + 0.5;
        //            g_bjElapsedTime = 0;
        //        }
        //            
        //        if (g_sBossP1.m_cLocation.X < d)
        //        {
        //            n = n + 10;
        //        }
        //        else if (g_sBossP1.m_cLocation.X > d)
        //        {
        //            n = n - 10;
        //        }
        //        else if (g_sBossP1.m_cLocation.X < d + n)
        //        {
        //            n = n - 10;
        //        }
        //        else if (g_sBossP1.m_cLocation.X > d + n)
        //        {
        //            n = n + 10;
        //        }
        //    }
        //      
        //}
        //else if (g_sBossP1.m_cLocation.X == d || g_sBossP1.m_cLocation.X == d + n)
        //{
        //    m = 0;
        //}
    }
}

void BossProjectile(int i,  double n) //boss projectile for Robot boss
{
    if (g_sBoss2.m_dHealth > 0 && !g_sUltimate && !g_sFocus) 
    {
        if (g_sChar.m_cLocation.X > g_sBoss2.m_cLocation.X && g_sChar.m_cLocation.Y == g_sBoss2.m_cLocation.Y)
        {

            if (g_sChar.m_cLocation.X <= g_sBoss2.m_cLocation.X + x) //shoot to right
            {
                bP5[i] = 233;
                if (g_bpElapsedTime[i] > n) //time between shots
                {
                    if (kp[i] >= 0 && kp[i] < 5)
                    {
                        g_sBProj[i].m_cLocation.Y--;
                        g_bpElapsedTime[i] = 0;
                        kp[i]++;
                    }
                    if (kp[i] >= 5 && g_sBProj[i].m_cLocation.X < g_sChar.m_cLocation.X)
                    {
                        g_sBProj[i].m_cLocation.X++;
                        g_bpElapsedTime[i] = 0;
                    }
                    else if (kp[i] >= 5 && g_sBProj[i].m_cLocation.X > g_sChar.m_cLocation.X)
                    {
                        g_sBProj[i].m_cLocation.X--;
                        g_bpElapsedTime[i] = 0;
                    }
                    else if (g_sBProj[i].m_cLocation.X == g_sChar.m_cLocation.X && g_sBProj[i].m_cLocation.Y != g_sChar.m_cLocation.Y)
                    {
                        g_sBProj[i].m_cLocation.Y++;
                        kp[i]++;
                        g_bpElapsedTime[i] = 0;
                    }
                    else if (g_sBProj[i].m_cLocation.X + 3 >= g_sChar.m_cLocation.X && g_sBProj[i].m_cLocation.Y == g_sChar.m_cLocation.Y)
                    {
                        if (!g_sUltimate && !g_sFocus && !g_sRage && !g_sSpirit && !g_sAttackState)
                        {
                            g_sChar.m_dHealth = g_sChar.m_dHealth - 10;
                        }
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 2;
                        g_sBProj[i].m_cLocation.X = g_sBoss2.m_cLocation.X;
                        g_sBProj[i].m_cLocation.Y = g_sBoss2.m_cLocation.Y;
                        kp[i] = 0;
                    }
                    else if (kp[i] >= 10 && kp[i] < 30)
                    {
                        g_sBoss2.m_cLocation.X = g_sBoss2.m_cLocation.X + 1;
                    }
                }
            }

        }
        if (g_sChar.m_cLocation.X < g_sBoss2.m_cLocation.X && g_sChar.m_cLocation.Y == g_sBoss2.m_cLocation.Y)
        {
            if (g_sChar.m_cLocation.X >= g_sBoss2.m_cLocation.X - x) //shoot to left
            {
                bP5[i] = 233;
                if (g_bpElapsedTime[i] > n) //time between shots
                {
                    if (kp[i] >= 0 && kp[i] < 5)
                    {
                        g_sBProj[i].m_cLocation.Y--;
                        g_bpElapsedTime[i] = 0;
                        kp[i]++;
                    }
                    if (kp[i] >= 5 && g_sBProj[i].m_cLocation.X > g_sChar.m_cLocation.X)
                    {
                        g_sBProj[i].m_cLocation.X--;
                        g_bpElapsedTime[i] = 0;
                    }
                    else if (kp[i] >= 5 && g_sBProj[i].m_cLocation.X < g_sChar.m_cLocation.X)
                    {
                        g_sBProj[i].m_cLocation.X++;
                        g_bpElapsedTime[i] = 0;
                    }
                    else if (g_sBProj[i].m_cLocation.X == g_sChar.m_cLocation.X && g_sBProj[i].m_cLocation.Y != g_sChar.m_cLocation.Y)
                    {
                        g_sBProj[i].m_cLocation.Y++;
                        kp[i]++;
                        g_bpElapsedTime[i] = 0;
                    }
                    else if (g_sBProj[i].m_cLocation.X - 3 <= g_sChar.m_cLocation.X && g_sBProj[i].m_cLocation.Y == g_sChar.m_cLocation.Y)
                    {
                        if (!g_sUltimate && !g_sFocus && !g_sRage && !g_sSpirit && !g_sAttackState)
                        {
                            g_sChar.m_dHealth = g_sChar.m_dHealth - 10;
                        }
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 2;
                        g_sBProj[i].m_cLocation.X = g_sBoss2.m_cLocation.X;
                        g_sBProj[i].m_cLocation.Y = g_sBoss2.m_cLocation.Y;
                        kp[i] = 0;
                    }
                    else if (kp[i] >= 10 && kp[i] < 30)
                    {
                        g_sBoss2.m_cLocation.X = g_sBoss2.m_cLocation.X - 1;
                    }
                }
            }

        }
    }
}

//enemy projectile physics
void EnemyProjectile(int& k, int i, double n, int x) //i = enemy number, x is distance of range
{
    if (!g_sFocus && !g_sUltimate)
    {
        if (g_sChar.m_cLocation.X > g_sEnemy[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sEnemy[i].m_cLocation.Y)
        {
            if (g_sChar.m_cLocation.X <= g_sEnemy[i].m_cLocation.X + x) //shoot to right
            {
                cP5[i] = 248;
                if (g_epElapsedTime[i] > n) //time between shots
                {
                    if (k != x)
                    {
                        /*if (j == 1)
                        {
                            Beep(1440, 30);
                        }*/
                        g_sEProj[i].m_cLocation.X++;
                        g_epElapsedTime[i] = 0;
                        k++;

                    }
                    else
                    {
                        g_sEProj[i].m_cLocation.Y = g_sEnemy[i].m_cLocation.Y;
                        g_sEProj[i].m_cLocation.X = g_sEnemy[i].m_cLocation.X;
                        k = 0;
                    }
                }
            }
        }
        if (g_sEProj[i].m_cLocation.X > g_sEnemy[i].m_cLocation.X)
        {
            if (k >= 1 && k < x)
            {
                if (g_epElapsedTime[i] > n)
                {
                    g_sEProj[i].m_cLocation.X++;
                    g_epElapsedTime[i] = 0;
                    k++;
                }
            }
            else
            {
                g_sEProj[i].m_cLocation.Y = g_sEnemy[i].m_cLocation.Y;
                g_sEProj[i].m_cLocation.X = g_sEnemy[i].m_cLocation.X;
                cP5[i] = 0;
                k = 0;
            }
        }
        if (g_sChar.m_cLocation.X < g_sEnemy[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sEnemy[i].m_cLocation.Y)
        {
            if (g_sChar.m_cLocation.X >= g_sEnemy[i].m_cLocation.X - x) //shoot to left
            {
                cP5[i] = 248;
                if (g_epElapsedTime[i] > n)
                {
                    if (k != x)
                    {
                        /*if (j == 1)
                        {
                            Beep(1440, 30);
                        }*/
                        g_sEProj[i].m_cLocation.X--;
                        g_epElapsedTime[i] = 0;
                        k++;
                    }
                    else
                    {
                        g_sEProj[i].m_cLocation.Y = g_sEnemy[i].m_cLocation.Y;
                        g_sEProj[i].m_cLocation.X = g_sEnemy[i].m_cLocation.X;
                        k = 0;
                    }
                }
            }

        }
        if (g_sEProj[i].m_cLocation.X < g_sEnemy[i].m_cLocation.X)
        {
            if (k >= 1 && k < x)
            {
                if (g_epElapsedTime[i] > n)
                {
                    g_sEProj[i].m_cLocation.X--;
                    g_epElapsedTime[i] = 0;
                    k++;
                }
            }
            else
            {
                g_sEProj[i].m_cLocation.Y = g_sEnemy[i].m_cLocation.Y;
                g_sEProj[i].m_cLocation.X = g_sEnemy[i].m_cLocation.X;
                cP5[i] = 0;
                k = 0;
            }
        }
    }
    
}

//same as enemy projectile but with delay
void vikingBow(int& k, int i, double n, double t, int x) //i = enemy number, x is distance of range
{
    if (!g_sFocus && !g_sUltimate && !g_sInvulnerable)
    {
        if (g_sChar.m_cLocation.X > g_sEnemy[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sEnemy[i].m_cLocation.Y)
        {
            if (g_sChar.m_cLocation.X <= g_sEnemy[i].m_cLocation.X + x) //shoot to right
            {
                if (g_epElapsedTime[i] > n && g_enemyDelay[i] > t) //time between shots
                {
                    if (k != x)
                    {
                        /*if (j == 1)
                        {
                            Beep(1440, 30);
                        }*/
                        g_sEProj[i].m_cLocation.X++;
                        g_epElapsedTime[i] = 0;
                        k++;

                    }
                    else
                    {
                        g_sEProj[i].m_cLocation.Y = g_sEnemy[i].m_cLocation.Y;
                        g_sEProj[i].m_cLocation.X = g_sEnemy[i].m_cLocation.X;
                        k = 0;
                    }
                }
            }
        }
        if (g_sEProj[i].m_cLocation.X > g_sEnemy[i].m_cLocation.X)
        {
            if (k >= 1 && k < x)
            {
                if (g_epElapsedTime[i] > n)
                {
                    g_sEProj[i].m_cLocation.X++;
                    g_epElapsedTime[i] = 0;
                    k++;
                }
            }
            else
            {
                g_sEProj[i].m_cLocation.Y = g_sEnemy[i].m_cLocation.Y;
                g_sEProj[i].m_cLocation.X = g_sEnemy[i].m_cLocation.X;
                cP5[i] = 0;
                k = 0;
                g_enemyDelay[i] = 0;
            }
        }
        if (g_sChar.m_cLocation.X < g_sEnemy[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sEnemy[i].m_cLocation.Y)
        {
            if (g_sChar.m_cLocation.X >= g_sEnemy[i].m_cLocation.X - x) //shoot to left
            {
                cP5[i] = 248;
                if (g_epElapsedTime[i] > n && g_enemyDelay[i] > t)
                {
                    if (k != x)
                    {
                        /*if (j == 1)
                        {
                            Beep(1440, 30);
                        }*/
                        g_sEProj[i].m_cLocation.X--;
                        g_epElapsedTime[i] = 0;
                        k++;
                    }
                    else
                    {
                        g_sEProj[i].m_cLocation.Y = g_sEnemy[i].m_cLocation.Y;
                        g_sEProj[i].m_cLocation.X = g_sEnemy[i].m_cLocation.X;
                        k = 0;
                    }
                }
            }

        }
        if (g_sEProj[i].m_cLocation.X < g_sEnemy[i].m_cLocation.X)
        {
            if (k >= 1 && k < x)
            {
                if (g_epElapsedTime[i] > n)
                {
                    g_sEProj[i].m_cLocation.X--;
                    g_epElapsedTime[i] = 0;
                    k++;
                }
            }
            else
            {
                g_sEProj[i].m_cLocation.Y = g_sEnemy[i].m_cLocation.Y;
                g_sEProj[i].m_cLocation.X = g_sEnemy[i].m_cLocation.X;
                cP5[i] = 0;
                k = 0;
                g_enemyDelay[i] = 0;
            }
        }
    }

}

//attacking with disappearance and appearance
void ninjaAttack(int& i, int& a, int n, double t, int d, int e) //stores 2 counters, boundary of attack, attack speed, original position and enemy array
{
    if (g_sEnemy[e].m_dHealth > 0 && !g_sUltimate && !g_sFocus && !g_sInvulnerable)
    {
        if (g_stun > 2)
        {
            if (g_eElapsedTime[e] > t && g_enemyDelay[e] > 3)
            {
                if (g_sChar.m_cLocation.X > g_sEnemy[e].m_cLocation.X && g_sChar.m_cLocation.Y == g_sEnemy[e].m_cLocation.Y && g_sLeftSlash[e] == false) //attack to right
                {
                    if (i == 0 && a == 0 && g_sChar.m_cLocation.X <= (g_sEnemy[e].m_cLocation.X + n) && g_sChar.m_cLocation.Y == g_sEnemy[e].m_cLocation.Y) //direction = right
                    {
                        i++;
                        g_sRightSlash[e] = true;
                    }
                }
                if (g_sRightSlash[e] == true)
                {
                    if (i == 1)
                    {
                        g_sEnemy[e].m_cLocation.Y = g_sEnemy[e].m_cLocation.Y - 1;
                        i++;
                        g_eElapsedTime[e] = 0;
                    }
                    else if (i > 1 && (g_sChar.m_cLocation.X - g_sEnemy[e].m_cLocation.X >= 1))
                    {
                        g_ninjaState[e] = true;
                        g_sEnemy[e].m_cLocation.X = g_sEnemy[e].m_cLocation.X + 1;
                        c5[e] = 0;
                        i++;
                        g_eElapsedTime[e] = 0;
                    }
                    else if (a == 0 && (g_sChar.m_cLocation.X <= g_sEnemy[e].m_cLocation.X + 1)) //slam down on player
                    {
                        g_ninjaState[e] = false;
                        c5[e] = 157;
                        g_sEnemy[e].m_cLocation.Y = g_sEnemy[e].m_cLocation.Y + 1;
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 2; //knockback
                        if (g_sChar.m_cLocation.Y == g_sEnemy[e].m_cLocation.Y && !g_sRage && !g_sInvulnerable && !g_sAttackState)
                        {
                            g_sChar.m_dHealth = g_sChar.m_dHealth - 5;
                        }
                        i = 0;
                        g_eElapsedTime[e] = 0;
                        g_enemyDelay[e] = 0;
                        a++;
                    }
                    if (g_enemyDelay[e] > 3)
                    {
                        if (a == 1)
                        {
                            g_sEnemy[e].m_cLocation.Y = g_sEnemy[e].m_cLocation.Y - 1;
                            a++;
                        }
                        else if (a > 1) //go back
                        {
                            g_ninjaState[e] = true;
                            c5[e] = 0;
                            g_sEnemy[e].m_cLocation.X = g_sEnemy[e].m_cLocation.X - 1;
                            g_eElapsedTime[e] = 0;
                            if (g_sEnemy[e].m_cLocation.X <= d)
                            {
                                g_sEnemy[e].m_cLocation.Y = g_sEnemy[e].m_cLocation.Y + 1;
                                g_enemyDelay[e] = 0;
                                c5[e] = 157;
                                a = 0;
                                i = 0;
                                g_ninjaState[e] = false;
                                g_sRightSlash[e] = false;
                            }
                        }
                    }
                }  
                if (g_sChar.m_cLocation.X < g_sEnemy[e].m_cLocation.X && g_sChar.m_cLocation.Y == g_sEnemy[e].m_cLocation.Y && g_sRightSlash[e] == false) //attack to left
                {
                    if (i == 0 && a == 0 && g_sChar.m_cLocation.X >= (g_sEnemy[e].m_cLocation.X - n) && g_sChar.m_cLocation.Y == g_sEnemy[e].m_cLocation.Y) //direction = right
                    {
                        i++;
                        g_sLeftSlash[e] = true;
                    }     
                }
                if (g_sLeftSlash[e] == true)
                {
                    if (i == 1)
                    {
                        g_sEnemy[e].m_cLocation.Y = g_sEnemy[e].m_cLocation.Y - 1;
                        i++;
                        g_eElapsedTime[e] = 0;
                    }
                    else if (i > 1 && (g_sEnemy[e].m_cLocation.X - g_sChar.m_cLocation.X >= 1))
                    {
                        g_ninjaState[e] = true;
                        g_sEnemy[e].m_cLocation.X = g_sEnemy[e].m_cLocation.X - 1;
                        c5[e] = 0;
                        i++;
                        g_eElapsedTime[e] = 0;
                    }
                    else if (a == 0 && (g_sChar.m_cLocation.X >= g_sEnemy[e].m_cLocation.X - 1)) //slam down on player
                    {
                        g_ninjaState[e] = false;
                        c5[e] = 157;
                        g_sEnemy[e].m_cLocation.Y = g_sEnemy[e].m_cLocation.Y + 1;
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 2; //knockback
                        if (g_sChar.m_cLocation.Y == g_sEnemy[e].m_cLocation.Y && !g_sRage && !g_sInvulnerable && !g_sAttackState)
                        {
                            g_sChar.m_dHealth = g_sChar.m_dHealth - 5;
                        }
                        i = 0;
                        g_eElapsedTime[e] = 0;
                        g_enemyDelay[e] = 0;
                        a++;
                    }
                    if (g_enemyDelay[e] > 3)
                    {
                        if (a == 1)
                        {
                            g_sEnemy[e].m_cLocation.Y = g_sEnemy[e].m_cLocation.Y - 1;
                            a++;
                        }
                        else if (a > 1) //go back
                        {
                            g_ninjaState[e] = true;
                            c5[e] = 0;
                            g_sEnemy[e].m_cLocation.X = g_sEnemy[e].m_cLocation.X + 1;
                            g_eElapsedTime[e] = 0;
                            if (g_sEnemy[e].m_cLocation.X >= d)
                            {
                                g_sEnemy[e].m_cLocation.Y = g_sEnemy[e].m_cLocation.Y + 1;
                                g_enemyDelay[e] = 0;
                                c5[e] = 157;
                                g_ninjaState[e] = false;
                                a = 0;
                                i = 0;
                                g_sLeftSlash[e] = false;
                            }
                        }
                    }
                }
                
            }
        }
    }
    for (int k = 0; k <= g_iPlatforms; k++)
    {
        if (g_sEnemy[e].m_cLocation.Y == g_aPlatformsY[k] && g_sEnemy[e].m_cLocation.X == g_aPlatformsX[k] && g_sLeftSlash[e] == false && g_sRightSlash[e] == false)
        {
            g_sEnemy[e].m_cLocation.Y = g_sEnemy[e].m_cLocation.Y - 1;
            a = 0;
            i = 0;
        }
    }
}

//approach player and knockback
void vikingAttack(int& i, int n, double t, int d, int e) //stores 2 counters, boundary of attack, attack speed, original position and enemy array
{
    if (g_sEnemy[e].m_dHealth > 0 && !g_sUltimate && !g_sFocus && !g_sInvulnerable)
    {
        if (g_stun > 2)
        {
            if (g_eElapsedTime[e] > t && g_enemyDelay[e] > 3)
            {
                if (g_sChar.m_cLocation.X > g_sEnemy[e].m_cLocation.X && g_sChar.m_cLocation.Y == g_sEnemy[e].m_cLocation.Y && g_sLeftSlash[e] == false) //attack to right
                {
                    if (i == 0 && g_sChar.m_cLocation.X <= (g_sEnemy[e].m_cLocation.X + n)) //direction = right
                    {
                        i++;
                        g_sRightSlash[e] = true;
                    }
                }
                if (g_sRightSlash[e] == true)
                {
                    if (i >= 1 && (g_sChar.m_cLocation.X - g_sEnemy[e].m_cLocation.X >= 2))
                    {
                        g_sEnemy[e].m_cLocation.X = g_sEnemy[e].m_cLocation.X + 1;
                        c5[e] = 0;
                        i++;
                        g_eElapsedTime[e] = 0;
                        for (int i = 0; i <= g_iPlatforms; i++)
                        {
                            if (g_sEnemy[e].m_cLocation.X == g_aPlatformsX[i] && g_sEnemy[e].m_cLocation.Y == g_aPlatformsY[i])
                            {
                                g_sEnemy[e].m_cLocation.X = g_sEnemy[e].m_cLocation.X - 1;
                            }
                        }
                    }
                    else if (g_sChar.m_cLocation.X <= g_sEnemy[e].m_cLocation.X + 2 && g_sChar.m_cLocation.X > g_sEnemy[e].m_cLocation.X) //hit player
                    {
                        c5[e] = 157;
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 2; //knockback
                        if (g_sChar.m_cLocation.Y == g_sEnemy[e].m_cLocation.Y && !g_sRage && !g_sInvulnerable && !g_sAttackState)
                        {
                            g_sChar.m_dHealth = g_sChar.m_dHealth - 5;
                        }
                        i = 0;
                        g_eElapsedTime[e] = 0;
                        g_enemyDelay[e] = 0;
                    }
                    if (g_enemyDelay[e] > 3)
                    {
                        if (i == 0 && (g_sEnemy[e].m_cLocation.X != d)) //go back to initial position
                        {
                            g_sEnemy[e].m_cLocation.X = g_sEnemy[e].m_cLocation.X - 1;
                            g_eElapsedTime[e] = 0;
                            for (int i = 0; i <= g_iPlatforms; i++)
                            {
                                if (g_sEnemy[e].m_cLocation.X == g_aPlatformsX[i] && g_sEnemy[e].m_cLocation.Y == g_aPlatformsY[i])
                                {
                                    g_sEnemy[e].m_cLocation.X = g_sEnemy[e].m_cLocation.X + 1;
                                }
                            }
                            
                        } 
                        else if (i > 0) //hit again
                        {
                            g_sRightSlash[e] = true;
                        } 
                        else if (g_sEnemy[e].m_cLocation.X != d)
                        {
                            g_sEnemy[e].m_cLocation.X = g_sEnemy[e].m_cLocation.X - 1;
                            g_eElapsedTime[e] = 0;
                        }
                        if (g_sEnemy[e].m_cLocation.X == d)
                        {
                            g_sRightSlash[e] = false;
                        }
                    }
                }


                if (g_sChar.m_cLocation.X < g_sEnemy[e].m_cLocation.X && g_sChar.m_cLocation.Y == g_sEnemy[e].m_cLocation.Y && g_sRightSlash[e] == false) //attack to left
                {
                    if (i == 0 && g_sChar.m_cLocation.X >= (g_sEnemy[e].m_cLocation.X - n) && g_sChar.m_cLocation.Y == g_sEnemy[e].m_cLocation.Y) //direction = left
                    {
                        i++;
                        g_sLeftSlash[e] = true;
                    }
                }
                if (g_sLeftSlash[e] == true)
                {
                    if (i >= 1 && (g_sEnemy[e].m_cLocation.X - g_sChar.m_cLocation.X >= 3))
                    {
                        g_sEnemy[e].m_cLocation.X = g_sEnemy[e].m_cLocation.X - 1;
                        c5[e] = 0;
                        i++;
                        g_eElapsedTime[e] = 0;
                        for (int i = 0; i <= g_iPlatforms; i++)
                        {
                            if (g_sEnemy[e].m_cLocation.X == g_aPlatformsX[i] && g_sEnemy[e].m_cLocation.Y == g_aPlatformsY[i])
                            {
                                g_sEnemy[e].m_cLocation.X = g_sEnemy[e].m_cLocation.X + 1;
                            }
                        }
                    }
                    else if (g_sChar.m_cLocation.X >= g_sEnemy[e].m_cLocation.X - 3 && g_sChar.m_cLocation.X < g_sEnemy[e].m_cLocation.X) //slam down on player
                    {
                        c5[e] = 157;
                        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 2; //knockback
                        if (g_sChar.m_cLocation.Y == g_sEnemy[e].m_cLocation.Y && !g_sRage && !g_sInvulnerable && !g_sAttackState)
                        {
                            g_sChar.m_dHealth = g_sChar.m_dHealth - 5;
                        }
                        i = 0;
                        g_eElapsedTime[e] = 0;
                        g_enemyDelay[e] = 0;
                    }
                    if (g_enemyDelay[e] > 3)
                    {
                        if (i == 0 && (g_sEnemy[e].m_cLocation.X != d))//go back to initial position
                        {
                            g_sEnemy[e].m_cLocation.X = g_sEnemy[e].m_cLocation.X + 1;
                            g_eElapsedTime[e] = 0;
                            for (int i = 0; i <= g_iPlatforms; i++)
                            {
                                if (g_sEnemy[e].m_cLocation.X == g_aPlatformsX[i] && g_sEnemy[e].m_cLocation.Y == g_aPlatformsY[i])
                                {
                                    g_sEnemy[e].m_cLocation.X = g_sEnemy[e].m_cLocation.X - 1;
                                }
                            }
                        }
                        else if (i > 0) //hit again
                        {
                            g_sLeftSlash[e] = true;

                        }
                        else if (g_sEnemy[e].m_cLocation.X != d)
                        {
                            g_sEnemy[e].m_cLocation.X = g_sEnemy[e].m_cLocation.X + 1;
                            
                            g_eElapsedTime[e] = 0;
                        }
                        if (g_sEnemy[e].m_cLocation.X == d)
                        {
                            g_sLeftSlash[e] = false;
                        }
                    }
                }

            }
        }
    }
    /*for (int k = 0; k <= g_iPlatforms; k++)
    {
        if (g_sEnemy[e].m_cLocation.Y == g_aPlatformsY[k] && g_sEnemy[e].m_cLocation.X == g_aPlatformsX[k] && g_sLeftSlash[e] == false && g_sRightSlash[e] == false)
        {
            g_sEnemy[e].m_cLocation.Y = g_sEnemy[e].m_cLocation.Y - 1;
            i = 0;
        }
    }*/
}

//Cutscenes in intros

void DewmAwaken()
{
    if (levelFile == "Dewmtutorial_1.txt")
    {
        if (g_skKeyEvent[K_SPACE].keyReleased && g_sCutscene == false)
        {
            g_sCutscene = true;
            cut++;
        }
        /*g_sChar.m_cLocation.X = 26;
        g_sChar.m_cLocation.Y = 48;*/
        if (cut < 31 && g_sCutscene == true)
        {
            if (g_cElapsedTime > 0.05)
            {
                if (cut >= 1 && cut < 10)
                {
                    for (int i = 0; i <= 1; i++)
                    {
                        g_sPrimeObj[i].m_cLocation.Y = g_sPrimeObj[i].m_cLocation.Y - 3;
                        g_sPrimeObj[i].m_cLocation.X = g_sPrimeObj[i].m_cLocation.X - 2;
                    }
                    cut++;
                    g_cElapsedTime = 0;
                }
                if (cut >= 10 && cut < 19)
                {
                    for (int i = 0; i <= 1; i++)
                    {
                        g_sPrimeObj[i].m_cLocation.Y = g_sPrimeObj[i].m_cLocation.Y + 3;
                        g_sPrimeObj[i].m_cLocation.X = g_sPrimeObj[i].m_cLocation.X - 1;
                        if (cut == 18)
                        {
                            g_sPrimeObj[i].m_cLocation.Y = g_sPrimeObj[i].m_cLocation.Y + 1;
                        }
                    }
                    cut++;
                    g_cElapsedTime = 0;
                }
                if (cut >= 19 && cut < 23)
                {
                    for (int i = 0; i <= 1; i++)
                    {
                        g_sPrimeObj[i].m_cLocation.X = g_sPrimeObj[i].m_cLocation.X - 2;
                    }
                    cut++;
                    g_cElapsedTime = 0;
                }

            }
            if (g_cElapsedTime > 0.05)
            {
                if (cut >= 23 && cut < 27)
                {
                    g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y - 2;
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 2;
                    cut++;
                    g_cElapsedTime = 0;
                }
                if (cut >= 27 && cut < 31)
                {
                    g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y + 2;
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 2;
                    cut++;
                    g_cElapsedTime = 0;
                }
            }
        }
        else
        {
            g_sCutscene = false;
            cut = 0;
        }

    }
}

void DewmIntro()
{
    if (level == 3)
    {
        if (g_sChar.m_cLocation.X == 4 && g_sChar.m_cLocation.Y == 28 && cut == 0)
        {
            g_sCutscene = true;
            cut++;
        }
        if (cut < 93 && g_sCutscene == true)
        {
            if (g_cElapsedTime > 0.3)
            {
                if (cut >= 1 && cut < 93)
                {
                    g_sChar.m_cLocation.X++;
                    if (cut >= 59)
                    {
                        g_sNPC[5].m_cLocation.X++;
                    }
                    if (cut >= 85)
                    {
                        g_sNPC[8].m_cLocation.X--;
                    }
                    cut++;
                    g_cElapsedTime = 0; 
                }
            }
        }
        else
        {
            g_sCutscene = false;
        }
    }
}

void SeraphIntro()
{
    if (levelFile == "SeraphIntro.txt")
    {
        if (g_sChar.m_cLocation.X == 15 && g_sChar.m_cLocation.Y == 23 && cut == 0)
        {
            g_sCutscene = true;
            cut++;
        }
        if (cut < 89 && g_sCutscene == true)
        {
            if (g_cElapsedTime > 0.3)
            {
                if (cut >= 1 && cut < 7)
                {
                    g_sNPC[0].m_cLocation.X--;
                    g_sNPC[1].m_cLocation.X--;
                    g_sNPC[2].m_cLocation.X--;
                    g_cElapsedTime = 0;
                    cut++;
                    if (cut == 7)
                    {
                        g_cDelayTime = 0;
                    }
                }
                if (cut >= 6 && cut <= 61 && g_cDelayTime > 17)
                {
                    g_sNPC[0].m_cLocation.X++;
                    g_sNPC[1].m_cLocation.X++;
                    g_sNPC[2].m_cLocation.X++;
                    g_sChar.m_cLocation.X++;
                    g_cElapsedTime = 0;
                    cut++;
                    if (cut == 61)
                    {
                        g_cDelayTime = 0;
                        cut++;
                    }
                }
                if (cut > 61 && cut <= 89 && g_cDelayTime > 23)
                {
                    g_sChar.m_cLocation.X++;
                    g_cElapsedTime = 0;
                    cut++;
                }
            }
        }
        else
        {
            g_sCutscene = false;
        }
    }
}

void GinIntro()
{
    if (levelFile == "GinIntro.txt")
    {
        if (g_sChar.m_cLocation.X == 85 && g_sChar.m_cLocation.Y == 22 && g_sCutscene == false && cut == 0)
        {
            g_sCutscene = true;
            cut++;
        }
        if (cut < 163 && g_sCutscene == true)
        {
            if (g_cElapsedTime > 0.3)
            {
                if (cut >= 1 && cut < 38)
                {
                    g_sChar.m_cLocation.X--;
                    cut++;
                    g_cElapsedTime = 0;
                    if (cut == 38)
                    {
                        g_cDelayTime = 0;
                    }
                }
                if (cut >= 54 && cut < 64)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 1;
                    g_cElapsedTime = 0;
                    cut++;
                }
                if (cut >= 130 && cut < 156 && g_cDelayTime > 5)
                {
                    g_sNPC[0].m_cLocation.X = g_sNPC[0].m_cLocation.X - 2;
                    g_sNPC[1].m_cLocation.X = g_sNPC[1].m_cLocation.X - 2;
                    g_sNPC[2].m_cLocation.X = g_sNPC[2].m_cLocation.X - 2;
                    g_cElapsedTime = 0;
                    cut++;
                }
            }
            if (g_cElapsedTime > 0.05)
            {
                if (cut >= 38 && cut < 47 && g_cDelayTime > 3)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 2;
                    g_cElapsedTime = 0;
                    cut++;
                }
                if (cut >= 47 && cut < 52)
                {
                    g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 1;
                    g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y - 1;
                    g_cElapsedTime = 0;
                    cut++;
                }
                if (cut >= 52 && cut < 54)
                {
                    g_sChar.m_cLocation.Y = g_sChar.m_cLocation.Y - 1;
                    g_cElapsedTime = 0;
                    cut++;
                }
                if (cut >= 64 && cut < 128)
                {
                    g_sNPC[0].m_cLocation.X = g_sNPC[0].m_cLocation.X - 1;
                    g_cElapsedTime = 0;
                    cut++;
                }
                if (cut >= 87 && cut < 130)
                {
                    g_sNPC[1].m_cLocation.X = g_sNPC[1].m_cLocation.X - 2;
                    if (cut >= 89)
                    {
                        g_sNPC[2].m_cLocation.X = g_sNPC[2].m_cLocation.X - 2;
                    }
                    g_cElapsedTime = 0;
                    cut++;
                    if (cut == 130)
                    {
                        g_cDelayTime = 0;
                    }
                }
                if (cut >= 156 && cut < 163)
                {
                    g_sChar.m_cLocation.Y++;
                    cut++;
                    g_cElapsedTime = 0;
                }
            }
        }
        else
        {
            g_sCutscene = false;
        }
    }
}

void processUserInput() {
    // process inputs depending on game states
    if (g_skKeyEvent[K_ESCAPE].keyReleased) {
        switch (g_eGameState) {
        case S_GAME:
            g_eGameState = S_MENU;
            g_bPlayGame = false;
            break;
        case S_SPLASHSCREEN:
            g_eGameState = S_START;
            break;
        case S_MENU:
            if (!edit) {
                g_eGameState = S_GAME;
                g_bPlayGame = true;
            }
            if (edit) {
                g_eGameState = S_EDITOR;
            }
            break;
        case S_CUTSCENE:
            break;
        case S_WIN:
            break;
        case S_LOSE:
            g_eGameState = S_START;
            break;
        case S_START:
            break;
        case S_EDITOR:
            g_eGameState = S_MENU;
            break;
        }
    }
    if (g_skKeyEvent[K_SPACE].keyReleased) {
        switch (g_eGameState) {
        case S_GAME:
            break;
        case S_SPLASHSCREEN:
            resetToStart();
            g_bPlayGame = true;
            break;
        case S_MENU:
            g_bQuitGame = true;
            break;
        case S_CUTSCENE:
            break;
        case S_WIN:
            break;
        case S_LOSE:
            break;
        case S_START:
            break;
        case S_EDITOR: break;
        }
    }
    if (g_skKeyEvent[K_52].keyReleased) {
        switch (g_eGameState) {
        case S_GAME:
            resetToStart();
            break;
        case S_SPLASHSCREEN:
            break;
        case S_MENU:
            break;
        case S_CUTSCENE:
            break;
        case S_WIN:
            break;
        case S_LOSE:
            break;
        case S_START:
            break;
        case S_EDITOR: break;
        }
    }
    if (g_skKeyEvent[K_ENTER].keyReleased) {
        switch (g_eGameState) {
        case S_GAME:
            break;
        case S_SPLASHSCREEN:
            break;
        case S_MENU:
            if (pauseMenuSelect == 1) {
                saveGame();
                g_bPlayGame = true;
                g_eGameState = S_GAME;
            }
            else if (pauseMenuSelect == 0) {
                g_bPlayGame = true;
                if (!edit) {
                    loadSavedGame();
                    loadLevelData(level);
                    g_eGameState = S_GAME;
                }
                if (edit)
                    g_eGameState = S_EDITOR;
            }
            else if (pauseMenuSelect == -1) {
                g_bPlayGame = false;
                g_eGameState = S_START;
                edit = false;
            }
            break;
        case S_CUTSCENE:
            break;
        case S_WIN:
            break;
        case S_LOSE:
            break;
        case S_START:
            if (startMenuSelect == 1) {
                reset();
                g_eGameState = S_SPLASHSCREEN;
            }
            else if (startMenuSelect == 0) {
                g_bPlayGame = true;
                g_sChar.m_cLocation.X = g_sCharSpawn.m_cLocation.X;
                g_sChar.m_cLocation.Y = g_sCharSpawn.m_cLocation.Y;
                g_eGameState = S_GAME;
            }
            else if (startMenuSelect == -1) {
                g_bQuitGame = true;
            }
            else if (startMenuSelect == -2) {
                g_eGameState = S_EDITOR;
                edit = true;
            }
            break;
        case S_EDITOR: break;
        }
    }
    if (g_skKeyEvent[K_LEFT].keyReleased) {
        switch (g_eGameState) {
        case S_GAME:
            break;
        case S_SPLASHSCREEN:
            characterSelect--;
            if (characterSelect < 0)
                characterSelect = 3;
            break;
        case S_MENU:
            break;
        case S_CUTSCENE:
            break;
        case S_WIN:
            break;
        case S_LOSE:
            break;
        case S_START:
            break;
        case S_EDITOR: break;
        }
    }
    if (g_skKeyEvent[K_RIGHT].keyReleased) {
        switch (g_eGameState) {
        case S_GAME:
            break;
        case S_SPLASHSCREEN:
            characterSelect++;
            if (characterSelect > 3)
                characterSelect = 0;
            break;
        case S_MENU:
            break;
        case S_CUTSCENE:
            break;
        case S_WIN:
            break;
        case S_LOSE:
            break;
        case S_START:
            break;
        case S_EDITOR: break;
        }
    }
    if (g_skKeyEvent[K_UP].keyReleased) {
        switch (g_eGameState) {
        case S_START:
            startMenuSelect++;
            if (startMenuSelect > 1)
                startMenuSelect = 1;
            break;
        case S_SPLASHSCREEN: break;
        case S_GAME: break;
        case S_MENU:
            pauseMenuSelect++;
            if (pauseMenuSelect > 1)
                pauseMenuSelect = 1;
            break;
        case S_CUTSCENE: break;
        case S_WIN: break;
        case S_LOSE: break;
        case S_EDITOR: break;
        }
    }
    if (g_skKeyEvent[K_DOWN].keyReleased) {
        switch (g_eGameState) {
        case S_START:
            startMenuSelect--;
            if (startMenuSelect < -2)
                startMenuSelect = -2;
            break;
        case S_SPLASHSCREEN: break;
        case S_GAME: break;
        case S_MENU:
            pauseMenuSelect--;
            if (pauseMenuSelect < -1)
                pauseMenuSelect = -1;
            break;
        case S_CUTSCENE: break;
        case S_WIN: break;
        case S_LOSE: break;
        case S_EDITOR: break;
        }
    }
    if (g_skKeyEvent[K_47].keyReleased) {
        switch (g_eGameState) {
        case S_START: break;
        case S_SPLASHSCREEN: break;
        case S_GAME:
            if (godMode)
                godMode = false;
            else if (!godMode)
                godMode = true;
            break;
        case S_MENU: break;
        case S_CUTSCENE: break;
        case S_WIN: break;
        case S_LOSE: break;
        case S_EDITOR: break;
        }
    }
}

//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render() {
    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState) {
    case S_SPLASHSCREEN: 
        renderNewGameOption();
        break;
    case S_START:
        renderStartMenu();
        break;
    case S_GAME: 
        renderGame(); 
        renderFramerate();
        break;
    case S_MENU: 
        renderMenu();
        break;
    case S_CUTSCENE: break;
    case S_WIN: renderWin();  break;
    case S_LOSE: renderLose();  break;
    case S_EDITOR: renderEditor(); break;
    }
    // renderFramerate();      // renders debug information, frame rate, elapsed time, etc
    //renderInputEvents();    // renders status of input events
    renderToScreen();       // dump the contents of the buffer to the screen, one frame worth of game
}

void resetToStart() {
    g_sChar.m_cLocation.X = g_sCharSpawn.m_cLocation.X;
    g_sChar.m_cLocation.Y = g_sCharSpawn.m_cLocation.Y;
    l = 1;
    if (characterSelect == 0) { //Dewm Guy
        g_sChar.m_dHealth = 25;
        g_sChar.m_dMana = 0;

    }
    if (characterSelect == 1) { //Seraph
        g_sChar.m_dHealth = 55;
        g_sChar.m_dMana = 0;
    }
    if (characterSelect == 2) { //Gin
        g_sChar.m_dHealth = 25;
        g_sChar.m_dMana = 0;
    }
    if (characterSelect == 3) { //Thorfinn
        g_sChar.m_dHealth = 55;
        g_sChar.m_dMana = 0;
    }
}

void deletePlatforms() {
    delete[] g_aPlatformsX;
    delete[] g_aPlatformsY;
}

void clearScreen() {
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(BACKGROUND_RED);
}

void renderToScreen() {
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

void renderNewGameOption() {
    // renders the splash screen
    COORD c = g_Console.getConsoleSize();
    string ch; string desc; WORD color;
    renderMenuBackground();
    c.Y /= 3;
    c.Y += 6;
    c.X = g_Console.getConsoleSize().X / 2 - 9;
    if (characterSelect == 0) {
        ch = "Dewm Guy";
        color = 0x0C;
    }
    if (characterSelect == 1) {
        ch = "Seraph";
        color = 0x09;
    }
    if (characterSelect == 2) {
        ch = "Gin";
        color = 0x0E;
    }
    if (characterSelect == 3) {
        ch = "Thorfinn";
        color = 0x0B;
    }
    g_Console.writeToBuffer(c, ch, color);
    c.Y += 2;
    c.X = 0;
    if (characterSelect == 0) {
        desc = "In the last age, in the last battle, when the light last shortened, one sat. Frozen by the frost of  ice, his soul blistered by the light of Heaven and tainted below descension, he did not choose the path of perpetual relaxation. In his ravenous happiness he lost peace; and with freezing blood he   lazed around the Umbral Plains seeking vengeance against the light slaves who had right him. He tookoff the crown of the Day Sentinels, and those that did not taste the bite of his sword named him... the Dewm Slayer.";
    }
    if (characterSelect == 1) {
        desc = "The Cetra people were the first civilization to occupy the Planet. Originating from the outer confines of the universe, these nomadic people saw the Planet as their Promised Land, a perfect place in  harmony with their vision of supreme happiness: a paradise. Nevertheless,the concept of a Promised   Land is subject to individual appraisal; it is not the same for everyone. And so, some of the Cetra left the Planet when they did not find the fulfillment they were seeking. In contrast, those who remained became its permanent residents.";
    }
    if (characterSelect == 2) {
        desc = "In the late 13th century, the Sushi empire has laid waste to entire nations along their campaign to conquer the East. Sashimi Island is all that stands between mainland Japan and a massive Ninja invasion fleet led by the ruthless and cunning general, Wasabi. As the island burns in the wake of the   first wave of the Sushi assault, samurai warrior Gin Isei stands as one of the last surviving members of his clan. He is resolved to do whatever it takes, at any cost, to protect his people and reclaim his home. He must set aside the traditions that have shaped him as a warrior to forge a new path, the path of the Raven, and wage an unconventional war for the freedom of Sashimi";
    }
    if (characterSelect == 3) {
        desc = "Many years have passed since Thorfinn took his vengeance against the Norse gods. Having survived hisfinal encounter with his father Thor, Thorfinn has since travelled to Olympus in Ancient Greece and now lives with his young son Askeladd in the world of the Olympian gods, a savage land inhabited by many ferocious monsters and warriors. In order to teach his son, whose mother has recently died,    how to survive in such a world, Thorfinn must master the spirit that has driven him for many years  and embrace his newfound role as a father and a mentor.";
    }
    g_Console.writeToBuffer(c, desc, color);
    if (characterSelect == 2)
    {
        c.Y += 7;
    }
    else
    {
        c.Y += 6;
    }
    c.X = g_Console.getConsoleSize().X / 2 - 20;
    g_Console.writeToBuffer(c, "Arrow Keys to switch characters", 0x0F);
    c.Y += 2;
    c.X = g_Console.getConsoleSize().X / 2 - 16;
    g_Console.writeToBuffer(c, "Press <Space> to Play", 0x0F);
    c.Y += 2;
    c.X = g_Console.getConsoleSize().X / 2 - 23;
    g_Console.writeToBuffer(c, "Press <Esc> to return to Start Menu", 0x0F);
}

void renderStartMenu() {
    COORD c = g_Console.getConsoleSize();
    renderMenuBackground();
    c.Y /= 1.5;
    c.X = c.X / 2 - 9;
    if (startMenuSelect == 0) {
        c.X += 1;
        g_Console.writeToBuffer(c, "New Game", 0x0F);
        c.Y += 1;
        g_Console.writeToBuffer(c, "Load Game", 0x9F);
        c.Y += 1;
        g_Console.writeToBuffer(c, "Exit Game", 0x0F);
        c.Y += 1;
        c.X -= 1;
        //g_Console.writeToBuffer(c, "Level Editor", 0x0F);
    }
    if (startMenuSelect == 1) {
        c.X += 1;
        g_Console.writeToBuffer(c, "New Game", 0x9F);
        c.Y += 1;
        g_Console.writeToBuffer(c, "Load Game", 0x0F);
        c.Y += 1;
        g_Console.writeToBuffer(c, "Exit Game", 0x0F);
        c.Y += 1;
        c.X -= 1;
        //g_Console.writeToBuffer(c, "Level Editor", 0x0F);
    }
    if (startMenuSelect == -1) {
        c.X += 1;
        g_Console.writeToBuffer(c, "New Game", 0x0F);
        c.Y += 1;
        g_Console.writeToBuffer(c, "Load Game", 0x0F);
        c.Y += 1;
        g_Console.writeToBuffer(c, "Exit Game", 0x9F);
        c.Y += 1;
        c.X -= 1;
        //g_Console.writeToBuffer(c, "Level Editor", 0x0F);
    }
    if (startMenuSelect == -2) {
        c.X += 1;
        g_Console.writeToBuffer(c, "New Game", 0x0F);
        c.Y += 1;
        g_Console.writeToBuffer(c, "Load Game", 0x0F);
        c.Y += 1;
        g_Console.writeToBuffer(c, "Exit Game", 0x0F);
        c.Y += 1;
        c.X -= 1;
        //g_Console.writeToBuffer(c, "Level Editor", 0x9F);
    }
    c.Y += 2;
    c.X -= 1;
    g_Console.writeToBuffer(c, "By Team-07 ;D", 0x0F);
}

void renderMenuBackground() {
    int y = 0;
    std::ifstream map("menu.txt"); std::string line;
    if (map.is_open()) {
        std::string perLine;
        while (std::getline(map, line)) {
            perLine = line;
            for (int x = 0; x < perLine.length(); x++) {
                if (perLine[x] == '.') {
                    g_Console.writeToBuffer(x, y, " ", 0X0000);
                }
                if (perLine[x] == '8') {
                    g_Console.writeToBuffer(x, y, " ", 0xA0);
                }
                if (perLine[x] == '7') {
                    g_Console.writeToBuffer(x, y, " ", 0xB0);
                }
            }
            y++;
        }
    }
}

void renderEnemyStats() {
    if (levelFile == "lvlDewm1.txt" || levelFile == "lvlDewm2.txt" || levelFile == "lvlDewm3.txt" || levelFile == "lvlDewm4.txt" || levelFile == "lvlSeraph1.txt" || levelFile == "lvlSeraph2.txt" || levelFile == "lvlSeraph3.txt" || levelFile == "lvlSeraph4.txt" || levelFile == "Gintutorial_1.txt" || levelFile == "Gintutorial_2.txt" || levelFile == "lvlNinja1.txt" || levelFile == "lvlNinja2.txt" || levelFile == "lvlNinja3.txt" || levelFile == "lvlVik1.txt" || levelFile == "lvlVik2.txt" || levelFile == "lvlVik3.txt")
    {
        if (levelFile == "lvlDewm1.txt" || levelFile == "lvlDewm2.txt" || levelFile == "lvlDewm3.txt" || levelFile == "lvlDewm4.txt") //Demw guy lvls
        {
            Healthcolor = 0x4E;
        }
        else if (levelFile == "lvlSeraph1.txt" || levelFile == "lvlSeraph2.txt" || levelFile == "lvlSeraph3.txt" || levelFile == "lvlSeraph4.txt" || levelFile == "Gintutorial_1.txt" || levelFile == "Gintutorial_2.txt" || levelFile == "lvlNinja1.txt" || levelFile == "lvlNinja2.txt" || levelFile == "lvlNinja3.txt") // Seraph's and Gin's levels
        {
            Healthcolor = 0x1E;
        }
        else if (levelFile == "Gintutorial_1.txt" || levelFile == "Gintutorial_2.txt")
        {
            Healthcolor = 0x9E;
        }
        else if (levelFile == "lvlVik1.txt" || levelFile == "lvlVik2.txt" || levelFile == "lvlVik3.txt") //Thorfinn's levels
        {
            Healthcolor = 0x37;
        }
        for (int e = 0; e < ne; e++) //Render health stats
        {
            int enemyhealth = g_sEnemy[e].m_dHealth;
            if (g_sEnemy[e].m_dHealth > 0 && g_ninjaState[e] == false) {
                if (g_sEnemy[e].m_cLocation.X > 0 && g_sEnemy[e].m_cLocation.X < g_Console.getConsoleSize().X) {
                    g_Console.writeToBuffer(g_sEnemy[e].m_cLocation.X, g_sEnemy[e].m_cLocation.Y - 1, std::to_string(enemyhealth), Healthcolor);
                }
            }

        }
        
        if (levelFile == "lvlDewm4.txt") //first boss in Dewm
        {
            if (g_sBoss1[0].m_dHealth > 0)
            {
                int bosshealth = g_sBoss1[0].m_dHealth;
                g_Console.writeToBuffer(g_sBoss1[0].m_cLocation.X + 1, g_sBoss1[0].m_cLocation.Y - 2, std::to_string(bosshealth), 0x4A);
            }
        }
        if (levelFile == "lvlSeraph4.txt") //Second boss in Seraph
        {
            if (g_sBoss2.m_dHealth > 0)
            {
                int bosshealth = g_sBoss2.m_dHealth;
                g_Console.writeToBuffer(g_sBoss2.m_cLocation.X + 1, g_sBoss2.m_cLocation.Y - 2, std::to_string(bosshealth), 0x1A);
            }
        }
    }
}

void renderGame() {
    renderMap();        // renders the map to the buffer first
    renderObj();        // renders obj under char
    renderCharacter();  // renders the character into the buffer
    renderEnemy();      // renders enemies into the buffer
    renderHUD();
    //Set portal spawn if level is boss
    if (levelFile != "lvlDewm4.txt" && levelFile != "lvlSeraph4.txt" && levelFile != "lvlNinja4.txt" && levelFile != "lvlVik4.txt")
    {
        renderPortal();
    }
    else if (levelFile == "lvlDewm4.txt" && g_sBoss1[0].m_dHealth <= 0)
    {
        renderPortal();
    }
    else if (levelFile == "lvlSeraph4.txt" && g_sBoss2.m_dHealth <= 0)
    {
        renderPortal();
    }
    renderNPCDialogue();
    renderEnemyStats();  //Renders enemy health bar
    LEMoveChar();
    //renderInputEvents();
}

void renderHUD() {
    const WORD colors[] = {
        0x0A, 0x0B, 0x0C, 0x4D, 0x5E, 0x6F,
        0x03, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6, 0x00
    };
    COORD c;
    c.X = 1;
    c.Y = 0;
    colour(colors[1]);
    std::string health = std::to_string(g_sChar.m_dHealth);
    health = health.substr(0, 4);
    std::string healthDisplay = "Health = " + health;
    g_Console.writeToBuffer(c, healthDisplay, colors[0]);
    
    c.X = 17;
    c.Y = 0;
    colour(colors[1]);
    std::string mana = std::to_string(g_sChar.m_dMana);
    mana = mana.substr(0, 4);

    if (characterSelect == 0)
    {
        std::string manaDisplay = "Rage = " + mana;
        g_Console.writeToBuffer(c, manaDisplay, colors[1]);
    }
    if (characterSelect == 1)
    {
        std::string manaDisplay = "Ultimate = " + mana;
        g_Console.writeToBuffer(c, manaDisplay, colors[1]);
    }
    if (characterSelect == 2)
    {
        std::string manaDisplay = "Focus = " + mana;
        g_Console.writeToBuffer(c, manaDisplay, colors[1]);
    }
    if (characterSelect == 3)
    {
        std::string manaDisplay = "Spirit = " + mana;
        g_Console.writeToBuffer(c, manaDisplay, colors[1]);
        c.X = 55;
        c.Y = 0;
        colour(colors[1]);
        if (g_sRanged == true)
        {
            std::string rangedDisplay = "Current weapon: Bow";
            g_Console.writeToBuffer(c, rangedDisplay, colors[6]);
        }
        else if (g_sRanged == false)
        {
            std::string rangedDisplay = "Current weapon: Axe";
            g_Console.writeToBuffer(c, rangedDisplay, colors[6]);
        }
    }

    c.X = 35;
    c.Y = 0;
    colour(colors[1]);
    int elapsed = g_dElapsedTime;
    std::string elapsedTime = "Elapsed time = " + std::to_string(elapsed);
    g_Console.writeToBuffer(c, elapsedTime, colors[2]);
}

void nextLevel() {
    if (((g_sChar.m_cLocation.X == g_sPortal.m_cLocation.X && g_sChar.m_cLocation.Y == g_sPortal.m_cLocation.Y) || (g_sChar.m_cLocation.X + 1 == g_sPortal.m_cLocation.X && g_sChar.m_cLocation.Y == g_sPortal.m_cLocation.Y)) && oneTime == 1 && g_sUltimate == false && g_sCutscene == false)
    {
        oneTime = 0;
        level++;
        g_sChar.m_cLocation.X = g_sCharSpawn.m_cLocation.X;
        g_sChar.m_cLocation.Y = g_sCharSpawn.m_cLocation.Y;
        for (int i = 0; i < obj; i++)
        {
            g_sObj[i].m_cLocation.X = 0;
            g_sObj[i].m_cLocation.Y = 0;
            g_sObj[i].m_dHealth = 5;
        }
        for (int i = 0; i <= 1; i++)
        {
            g_sPrimeObj[i].m_cLocation.X = 0;
            g_sPrimeObj[i].m_cLocation.Y = 0;
        }
        for (int i = 0; i <= hp; i++)
        {
            g_sPotion[i].m_cLocation.X = 0;
            g_sPotion[i].m_cLocation.Y = 0;
            cO[i] = 160;
        }
        for (int e = 0; e < ne; e++)
        {
            g_sEnemy[e].m_dHealth = 5;
            g_sEnemy[e].m_cLocation.X = 0;
            g_sEnemy[e].m_cLocation.Y = 0;
            g_sEProj[e].m_cLocation.X = 0;
            g_sEProj[e].m_cLocation.Y = 0;
            i[e] = 0;
            ip[e] = 0;
        }
        for (int i = 0; i < 10000; i++)
        {
            g_sbCutscene[i].m_cLocation.X = 0;
            g_sbCutscene[i].m_cLocation.Y = 0;
        }
        k = 0;
        g_sBoss1[0].m_cLocation.X = 0;
        g_sBoss1[0].m_cLocation.Y = 0;
        g_sBoss1[1].m_cLocation.X = 0;
        g_sBoss1[1].m_cLocation.Y = 0;
        g_sBearSpirit[0].m_cLocation.X = 0;
        g_sBearSpirit[0].m_cLocation.Y = 0;
        g_sBearSpirit[1].m_cLocation.X = 0;
        g_sBearSpirit[1].m_cLocation.Y = 0;
        deletePlatforms();
        
    }
    if (oneTime == 0) {
        loadLevelData(level);
        oneTime = 1;
        g_sChar.m_cLocation.X = g_sCharSpawn.m_cLocation.X;
        g_sChar.m_cLocation.Y = g_sCharSpawn.m_cLocation.Y;
    }
}

void renderMenu() {
    COORD c = g_Console.getConsoleSize();
    renderMenuBackground();
    c.Y /= 1.5;
    c.X = c.X / 2 - 10;
    g_Console.writeToBuffer(c, "Pause Menu", 0x09);
    c.Y += 1;
    c.X += 1;
    if (pauseMenuSelect == 0) {
        g_Console.writeToBuffer(c, "Save Game", 0x09);
        c.Y += 1;
        g_Console.writeToBuffer(c, "Continue", 0x90);
        c.Y += 1;
        c.X -= 2;
        g_Console.writeToBuffer(c, "Exit to Start", 0x09);
    }
    if (pauseMenuSelect == 1) {
        g_Console.writeToBuffer(c, "Save Game", 0x90);
        c.Y += 1;
        g_Console.writeToBuffer(c, "Continue", 0x09);
        c.Y += 1;
        c.X -= 2;
        g_Console.writeToBuffer(c, "Exit to Start", 0x09);
    }
    if (pauseMenuSelect == -1) {
        g_Console.writeToBuffer(c, "Save Game", 0x09);
        c.Y += 1;
        g_Console.writeToBuffer(c, "Continue", 0x09);
        c.Y += 1;
        c.X -= 2;
        g_Console.writeToBuffer(c, "Exit to Start", 0x90);
    }
}

void renderPortal() {
    if (g_sPortal.m_cLocation.X > 0 && g_sPortal.m_cLocation.X < x)
        g_Console.writeToBuffer(g_sPortal.m_cLocation.X, g_sPortal.m_cLocation.Y, " ", 0xF0);
}

void renderSavedGame() {
    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - 10;
    c.Y--;
    g_Console.writeToBuffer(c, "Saved game", 0x09);
}

void renderMap() {
    // Set up sample colours, and output shadings
    int y = 0;
    std::ifstream map("map.txt"); std::string line;
    if (map.is_open()) {
        std::string perLine;
        while (std::getline(map, line)) {
            perLine = line;
            for (int x = 0; x < perLine.length(); x++) {
                if (perLine[x] == '.') {
                    g_Console.writeToBuffer(x, y, " ", BGcolor);
                }  
            }
            y++;
        }
    }
    map.close();
    renderPlatform();

    //Dewm Guy Intro
    if (levelFile == "Dewmtutorial_1.txt") {
        if (g_sChar.m_cLocation.X == 45)
        {
            renderDialogue("Press <Space> to awaken", 11, 16);
        }
        else
        {
            renderDialogue("Press <AD> to move left and right", 11, 16);
        }
    }
    if (levelFile == "Dewmtutorial_2.txt")
        renderDialogue("Press <W> to jump", 11, 16);

    if (levelFile == "Dewmtutorial_3.txt") {
        renderDialogue("RED symbolises an enemy, WHITE symbolises a portal", 11, 14);
        renderDialogue("Left Click to shoot", 11, 15);
    }

    if (levelFile == "DewmIntro.txt")
    {
        renderDialogue("When Rage meter is full,", 10, 11);
        renderDialogue("press space to unleash havoc", 10, 12);
    }
    
    //Seraph intro
    if (levelFile == "SeraphIntro.txt") 
    {
        renderDialogue("Press <AD> to move left and right", 40, 13);
        renderDialogue("Press <W> to jump", 40, 14);
    }

    if (levelFile == "Seraphtutorial_1.txt")
    {
        renderDialogue("PINK symbolises an enemy, WHITE symbolises a portal", 33, 10);
        renderDialogue("Left click to attack", 33, 11);
        renderDialogue("When in the air, press <S> to downslam to do AOE", 33, 12);
    }

    if (levelFile == "Seraphtutorial_2.txt")
    {
        renderDialogue("When ultimate meter is full,", 31, 21);
        renderDialogue("press <SPACE> to limit break", 31, 22);
    }

    //Gin intro
    if (levelFile == "GinIntro.txt")
    {
        if (cut >= 163)
        {
            renderDialogue("Press <AD> to move left and right", 29, 25);
        }
    }
    if (levelFile == "Gintutorial_1.txt")
    {
        renderDialogue("Press <W> to jump", 73, 25);
        renderDialogue("BLUE symbolises an enemy, WHITE symbolises a portal", 11, 25);
        renderDialogue("Left click to attack", 11, 26);
    }
    if (levelFile == "Gintutorial_2.txt")
    {
        renderDialogue("Press <Q> or <E> to dodge", 45, 10);
        renderDialogue("Press <S> to sneak", 35, 19);
    }
    if (levelFile == "Gintutorial_3.txt")
    {
        renderDialogue("When focus meter is full, press <SPACE> to enter Focus mode", 3, 17);
        renderDialogue("While focus meter is full, press <U> to unleash Focus ultimate", 3, 18);
    }
    
}

void renderDialogue(string d, int x, int y) {
    if (characterSelect == 0)
    {
        if (x > 0 && x < g_Console.getConsoleSize().X)
            g_Console.writeToBuffer(x, y, d, 0x80);
    }
    if (characterSelect == 1)
    {
        if (x > 0 && x < g_Console.getConsoleSize().X)
            g_Console.writeToBuffer(x, y, d, 0x0E);
    }
    if (characterSelect == 2)
    {
        if (x > 0 && x < g_Console.getConsoleSize().X)
            g_Console.writeToBuffer(x, y, d, 0x0F);
    }
}

void renderEditor() {
    renderMap();
}

void updateEditor() {
    processUserInput();
}

void renderCharacter()
{
    // Draw the location of the character and weapon

    // Draw the location of the character and weapon
    WORD charColor; //non - ultimate mode color

    
    if (g_sChar.m_dHealth > 0) //ASCII Designs
    {
        if (characterSelect == 0) //Dewm Guy
        {
            if (g_sRage == true)
            {
                charColor = 0x40; //Ultimate mode
            }
            else
            {
                charColor = 0x20;
            }
            c1 = 203;
            c2 = 203;
            if (g_sProj.m_cLocation.X > g_sChar.m_cLocation.X || g_sProj.m_cLocation.X < g_sChar.m_cLocation.X)
            {
                if (levelFile == "Dewmtutorial_3.txt" || levelFile == "DewmIntro.txt")
                {
                    g_Console.writeToBuffer(g_sProj.m_cLocation, c4, 0x8E);
                }
                if (levelFile == "lvlDewm1.txt" || levelFile == "lvlDewm2.txt" || levelFile == "lvlDewm3.txt" || levelFile == "lvlDewm4.txt")
                {
                    g_Console.writeToBuffer(g_sProj.m_cLocation, c4, 0x4E);
                }
                if (levelFile == "lvlSeraph1.txt" || levelFile == "lvlSeraph2.txt" || levelFile == "lvlSeraph3.txt" || levelFile == "lvlSeraph4.txt" || levelFile == "lvlNinja1.txt" || levelFile == "lvlNinja2.txt" || levelFile == "lvlNinja3.txt")
                {
                    g_Console.writeToBuffer(g_sProj.m_cLocation, c4, 0x1E);
                }
                if (levelFile == "lvlNinja4.txt")
                {
                    g_Console.writeToBuffer(g_sProj.m_cLocation, c4, 0x7E);
                }
                if (levelFile == "lvlVik1.txt" || levelFile == "lvlVik2.txt" || levelFile == "lvlVik3.txt" || levelFile == "lvlVik4.txt")
                {
                    g_Console.writeToBuffer(g_sProj.m_cLocation, c4, 0x3E);
                }
            }
            g_Console.writeToBuffer(g_sChar.m_cLocation, c3, charColor);

        }
        if (characterSelect == 1) //Seraph
        {
            if (g_sUltimate == true)
            {
                charColor = 0x0F;
            }
            else
            {
                charColor = 0x0E; //F for white
            }

            c1 = 210;
            c2 = 197;
            c3 = std::string(1, c1) + c2;
            g_Console.writeToBuffer(g_sChar.m_cLocation, c3, charColor);
        }
        if (characterSelect == 2) //Gin
        {
            if (g_sFocus == true)
            {
                charColor = 0x5A;
            }
            else if (g_sUltimate == true)
            {
                charColor = 0x0A;
            }
            else
            {
                charColor = 0x5F;
            }
            c1 = 234;
            c2 = 234;
            c3 = std::string(1, c1) + c2;
            g_Console.writeToBuffer(g_sChar.m_cLocation, c3, charColor);
        }
        if (characterSelect == 3) //Thorfinn
        {
            if (g_sSpirit == true)
            {
                charColor = 0xF7; //Ultimate mode
                b1 = 254; //bear spirit ascii 
                b2 = 254;
                bear1 = std::string(3, b1) + b2;
                b3 = 221;
                b4 = 221;
                bear2 = std::string(3, b1) + b2;
                bearColor = 0x9F;
                g_Console.writeToBuffer(g_sBearSpirit[0].m_cLocation, bear1, bearColor);
                g_Console.writeToBuffer(g_sBearSpirit[1].m_cLocation, bear2, bearColor);
            }
            else
            {
                b1 = 0;
                b2 = 0;
                b3 = 0;
                b4 = 0;
                bear1 = std::string(3, b1) + b2;
                bear2 = std::string(3, b1) + b2;
                bearColor = BGcolor;
                charColor = 0x9F;
            }
            c1 = 254;
            c2 = 254;
            c3 = std::string(1, c1) + c2;
            if ((g_sProj.m_cLocation.X > g_sChar.m_cLocation.X || g_sProj.m_cLocation.X < g_sChar.m_cLocation.X) && g_sRanged == true)
            {
                if (levelFile == "Dewmtutorial_1.txt" || levelFile == "Dewmtutorial_2.txt" || levelFile == "Dewmtutorial_3.txt" || levelFile == "DewmIntro.txt")
                {
                    g_Console.writeToBuffer(g_sProj.m_cLocation, c4, 0x8B);
                }
                if (levelFile == "lvlDewm1.txt" || levelFile == "lvlDewm2.txt" || levelFile == "lvlDewm3.txt" || levelFile == "lvlDewm4.txt" || levelFile == "lvlSeraph1.txt" || levelFile == "lvlSeraph2.txt" || levelFile == "lvlSeraph3.txt" || levelFile == "lvlSeraph4.txt" || levelFile == "lvlNinja1.txt" || levelFile == "lvlNinja2.txt" || levelFile == "lvlNinja3.txt" || levelFile == "lvlVik1.txt" || levelFile == "lvlVik2.txt" || levelFile == "lvlVik3.txt")
                {
                    g_Console.writeToBuffer(g_sProj.m_cLocation, c4, 0x4B);
                }
                if (levelFile == "lvlSeraph1.txt" || levelFile == "lvlSeraph2.txt" || levelFile == "lvlSeraph3.txt" || levelFile == "lvlSeraph4.txt" || levelFile == "lvlNinja1.txt" || levelFile == "lvlNinja2.txt" || levelFile == "lvlNinja3.txt")
                {
                    g_Console.writeToBuffer(g_sProj.m_cLocation, c4, 0x1B);
                }
                if (levelFile == "lvlNinja4.txt")
                {
                    g_Console.writeToBuffer(g_sProj.m_cLocation, c4, 0x7B);
                }
                if (levelFile == "lvlVik1.txt" || levelFile == "lvlVik2.txt" || levelFile == "lvlVik3.txt" || levelFile == "lvlVik4.txt")
                {
                    g_Console.writeToBuffer(g_sProj.m_cLocation, c4, 0x3B);
                }
            }
            g_Console.writeToBuffer(g_sChar.m_cLocation, c3, charColor);
            
        }
    }
}

void renderEnemy()
{
    for (int i = 0; i < ne; i++) //Enemy
    {
        if (levelFile == "lvlDewm1.txt" || levelFile == "lvlDewm2.txt" || levelFile == "lvlDewm3.txt" || levelFile == "lvlDewm4.txt" || levelFile == "lvlSeraph1.txt" || levelFile == "lvlSeraph2.txt" || levelFile == "lvlSeraph3.txt" || levelFile == "lvlSeraph4.txt" || levelFile == "Gintutorial_1.txt" || levelFile == "Gintutorial_2.txt" || levelFile == "lvlNinja1.txt" || levelFile == "lvlNinja2.txt" || levelFile == "lvlNinja3.txt" || levelFile == "lvlVik1.txt" || levelFile == "lvlVik2.txt" || levelFile == "lvlVik3.txt")
        {
            if (g_sEnemy[i].m_dHealth > 0 && g_ninjaState[i] == false)
            {
                if (levelFile == "lvlDewm1.txt" || levelFile == "lvlDewm2.txt" || levelFile == "lvlDewm3.txt" || levelFile == "lvlDewm4.txt")
                {
                    enemyColor[i] = 0x4F;
                    EProjColor[i] = 0x4F;
                    c5[i] = 157;
                }
                else if (levelFile == "lvlSeraph1.txt" || levelFile == "lvlSeraph2.txt" || levelFile == "lvlSeraph3.txt" || levelFile == "lvlSeraph4.txt")
                {
                    enemyColor[i] = 0x8C;
                    EProjColor[i] = 0x1C;
                    c5[i] = 232;
                    cP5[i] = 247;
                }
                else if (levelFile == "Gintutorial_1.txt" || levelFile == "Gintutorial_2.txt" || levelFile == "lvlNinja1.txt" || levelFile == "lvlNinja2.txt" || levelFile == "lvlNinja3.txt")
                {
                    enemyColor[i] = 0x0B;
                    EProjColor[i] = 0x18;
                    c5[i] = 223;
                    cP5[i] = 43;
                    if (levelFile == "Gintutorial_2.txt")
                    {
                        EProjColor[i] = 0x98;
                    }
                }
                else if (levelFile == "lvlVik1.txt" || levelFile == "lvlVik2.txt" || levelFile == "lvlVik3.txt")
                {
                    enemyColor[i] = 0x0F;
                    EProjColor[i] = 0x37;
                    c5[i] = 86;
                    cP5[i] = 196;
                }
                if ((g_sEProj[i].m_cLocation.X > g_sEnemy[i].m_cLocation.X || g_sEProj[i].m_cLocation.X < g_sEnemy[i].m_cLocation.X) && g_sEProj[i].m_cLocation.Y == g_sEnemy[i].m_cLocation.Y)
                {
                    g_Console.writeToBuffer(g_sEProj[i].m_cLocation, cP5[i], EProjColor[i]); //enemy projectile
                }
                g_Console.writeToBuffer(g_sEnemy[i].m_cLocation, c5[i], enemyColor[i]); //enemy write
            }
            else
            {
                c5[i] = 0;
                cP5[i] = 0;
                enemyColor[i] = BGcolor;
            }
        }
        
        for (int i = 0; i <= 3; i++)
        {
            if (levelFile == "lvlDewm3.txt") //spawner level
            {
                if (g_sObj[0].m_dHealth > 0 || g_sObj[1].m_dHealth > 0) //writes spawner obj
                {
                    if (g_uSpawn > 3 && c5[i] == 0)
                    {
                        c5[i] = 152;
                        enemyColor[i] = 0x4F;
                        g_sEnemy[i].m_dHealth = 5;
                        g_uSpawn = 0;
                        g_Console.writeToBuffer(g_sEnemy[i].m_cLocation, c5[i], enemyColor[i]); //spawns enemy
                    }
                }
            }
        }

        if (levelFile == "lvlDewm4.txt") //mini - boss level
        {
            g_Console.writeToBuffer(g_sBoss1[0].m_cLocation, boss1, bossColor[0]);
            g_Console.writeToBuffer(g_sBoss1[1].m_cLocation, boss2, bossColor[0]);
        }
        if (levelFile == "lvlSeraph4.txt")
        {
            for (int i = 0; i <= 5; i++) {
                if (g_sBProj[i].m_cLocation.Y != g_sChar.m_cLocation.Y)
                {
                    g_Console.writeToBuffer(g_sBProj[i].m_cLocation, bP5[i], BProjColor[i]);
                }
            }
            g_Console.writeToBuffer(g_sBoss2.m_cLocation, boss3, bossColor[1]);
        }
    }
}

void renderObj()
{
    if (levelFile == "Dewmtutorial_1.txt")
    {
        for (int i = 0; i <= 3; i++)
        {
            if (g_sObj[i].m_dHealth > 0)
            {
                c0[i] = 221;
                ObjColor[i] = 0x44;
            }
            else
            {
                ObjColor[i] = BGcolor;
                c0[i] = 0;
            }
            g_Console.writeToBuffer(g_sObj[i].m_cLocation, c0[i], ObjColor[i]);
            g_Console.writeToBuffer(g_sPrimeObj[i].m_cLocation, 220, 0x44);
        }
    }
    if (levelFile == "Dewmtutorial_3.txt")
    {
        for (int i = 0; i <= 4; i++)
        {
            if (g_sObj[i].m_dHealth > 0)
            {
                c0[i] = 237;
                ObjColor[i] = 0x40;
            }
            else
            {
                ObjColor[i] = BGcolor;
                c0[i] = 0;
            }
            g_Console.writeToBuffer(g_sObj[i].m_cLocation, c0[i], ObjColor[i]);
        }
    }
    if (levelFile == "DewmIntro.txt")
    {
        for (int i = 0; i <= 24; i++)
        {
            if (g_sObj[i].m_dHealth > 0)
            {
                c0[i] = 219;
                ObjColor[i] = 0x00;
            }
            else
            {
                ObjColor[i] = BGcolor;
                c0[i] = 0;
            }
            g_Console.writeToBuffer(g_sObj[i].m_cLocation, c0[i], ObjColor[i]);
        }
        for (int i = 0; i <= 8; i++)
        {
            NPC[i] = 246;
            NPCcolor[i] = 0x9F;
            NPCcolor[5] = 0x9C;
            NPC[8] = 241;
            NPCcolor[8] = 0x0F;
            g_Console.writeToBuffer(g_sNPC[i].m_cLocation, NPC[i], NPCcolor[i]);
        }
        for (int i = 0; i < 500; i++)
        {
            g_Console.writeToBuffer(g_sbCutscene[i].m_cLocation, " ", BACKGROUND_GREEN);
        }
    }
        
    //potion rendering
    if (levelFile == "lvlDewm1.txt" || levelFile == "lvlDewm2.txt" || levelFile == "lvlDewm3.txt" || levelFile == "lvlDewm4.txt" || levelFile == "lvlSeraph1.txt" || levelFile == "lvlSeraph2.txt" || levelFile == "lvlSeraph3.txt" || levelFile == "lvlSeraph4.txt" || levelFile == "Gintutorial_2.txt" || levelFile == "lvlNinja1.txt" || levelFile == "lvlNinja2.txt" || levelFile == "lvlNinja3.txt" || levelFile == "lvlVik1.txt" || levelFile == "lvlVik2.txt" || levelFile == "lvlVik3.txt")
    {
        for (int i = 0; i <= hp; i++)
        {
            if (g_sChar.m_cLocation.X == g_sPotion[i].m_cLocation.X && g_sChar.m_cLocation.Y == g_sPotion[i].m_cLocation.Y && cO[i] != 0)
            {
                cO[i] = 0;
                PotionColor[i] = BGcolor;
            }
            else
            {
                if (levelFile == "lvlDewm1.txt" || levelFile == "lvlDewm2.txt" || levelFile == "lvlDewm3.txt" || levelFile == "lvlDewm4.txt")
                {
                    PotionColor[i] = 0x4A;
                }
                else if (levelFile == "Gintutorial_2.txt")
                {
                    PotionColor[i] = 0x9A;
                }
                else if (levelFile == "lvlSeraph1.txt" || levelFile == "lvlSeraph2.txt" || levelFile == "lvlSeraph3.txt" || levelFile == "lvlSeraph4.txt" || levelFile == "lvlNinja1.txt" || levelFile == "lvlNinja2.txt" || levelFile == "lvlNinja3.txt")
                {
                    PotionColor[i] = 0x1A;
                }
                else if (levelFile == "lvlVik1.txt" || levelFile == "lvlVik2.txt" || levelFile == "lvlVik3.txt")
                {
                    PotionColor[i] = 0x3A;
                }
                g_Console.writeToBuffer(g_sPotion[i].m_cLocation, cO[i], PotionColor[i]);
            }
        }
    }
    if (levelFile == "lvlDewm3.txt")
    {
        for (int i = 0; i < 2; i++)
        {
            if (g_sObj[i].m_dHealth > 0)
            {
                c0[i] = 144;
                ObjColor[i] = 0x4E;
                g_Console.writeToBuffer(g_sObj[i].m_cLocation, c0[i], ObjColor[i]);
            }
            else
            {
                ObjColor[i] = BGcolor;
                c0[i] = 0;
            }
        }
    }
    
    if (levelFile == "SeraphIntro.txt")
    {
        for (int i = 0; i <= 2; i++)
        {
            NPC[0] = 194;
            NPCcolor[0] = 0x05;
            NPC[1] = 223;
            NPCcolor[1] = 0x0B;
            NPC[2] = 236;
            NPCcolor[2] = 0x0F;
            g_Console.writeToBuffer(g_sNPC[i].m_cLocation, NPC[i], NPCcolor[i]);
        }
        for (int i = 0; i < 384; i++)
        {
            g_Console.writeToBuffer(g_sbCutscene[i].m_cLocation, " ", 0x80);
        }
        for (int i = 0; i < 96; i++)
        {
            g_Console.writeToBuffer(g_sTrainTracks[i].m_cLocation, " ", 0x70);
        }
    }
    if (levelFile == "Seraphtutorial_1.txt") {
        for (int i = 0; i < 1500; i++)
        {
            g_Console.writeToBuffer(g_sbCutscene[i].m_cLocation, " ", 0x00);
        }
        for (int i = 0; i < 4; i++)
        {
            if (g_sObj[i].m_dHealth > 0)
            {
                c0[i] = 173;
                ObjColor[i] = 0x50;
                g_Console.writeToBuffer(g_sObj[i].m_cLocation, c0[i], ObjColor[i]);
            }
            else
            {
                ObjColor[i] = BGcolor;
                c0[i] = 0;
            }
        }
    }
    if (levelFile == "Seraphtutorial_2.txt") {
        for (int i = 0; i < 117; i++)
        {
            g_Console.writeToBuffer(g_sbCutscene[i].m_cLocation, " ", 0x00);
        }
    }

    if (levelFile == "GinIntro.txt")
    {
        for (int i = 0; i < 650; i++)
        {
            g_Console.writeToBuffer(g_sbCutscene[i].m_cLocation, " ", 0x00);
        }
        for (int i = 0; i < 3; i++)
        {
            NPC[0] = 229;
            NPCcolor[0] = 0xF5;
            NPC[1] = 223;
            NPCcolor[1] = 0x0B;
            NPC[2] = 223;
            NPCcolor[2] = 0x0B;
            if (g_sNPC[i].m_cLocation.X > 2)
            {
                if (cut >= 64)
                {
                    g_Console.writeToBuffer(g_sNPC[0].m_cLocation, NPC[0], NPCcolor[0]);
                }
                if (cut >= 87)
                {
                    g_Console.writeToBuffer(g_sNPC[i].m_cLocation, NPC[i], NPCcolor[i]);
                }
            }
        }
    }
    if (levelFile == "Gintutorial_2.txt")
    {
        for (int i = 0; i < 55; i++)
        {
            g_Console.writeToBuffer(g_sbCutscene[i].m_cLocation, " ", 0x00);
        }
    }
    

}

void renderPlatform() {
    for (int i = 0; i < g_iPlatforms; i++) {
        if (g_aPlatformsX[i] >= 0 && g_aPlatformsX[i] < g_Console.getConsoleSize().X)
            g_Console.writeToBuffer(g_aPlatformsX[i], g_aPlatformsY[i], " ", 0x0F);
    }
}

bool checkCollision() {
    bool hit = false;
    for (int i = 0; i < g_iPlatforms; i++) {
        if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
            hit = true;
        }
        if (g_sChar.m_cLocation.X + 1 == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
            hit = true;
        }
        if (g_sChar.m_cLocation.X - 1 == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
            hit = true;
        }
    }
    return hit;
}

void renderFramerate() {
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(0);
    ss << /*1.0 / g_dDeltaTime*/cut << "FPS";
    c.X = g_Console.getConsoleSize().X - 5;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    // displays the elapsed time
    /*ss.str("");
    ss << g_dElapsedTime << "secs";
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 0x59);*/
}

// this is an example of how you would use the input events
void renderInputEvents() {
    // keyboard events
    COORD startPos = { 50, 2 };
    std::ostringstream ss;
    std::string key;
    for (int i = 0; i < K_COUNT; ++i) {
        ss.str("");
        switch (i) {
        case K_57: key = "UP";
            break;
        case K_53: key = "DOWN";
            break;
        case K_41: key = "LEFT";
            break;
        case K_44: key = "RIGHT";
            break;
        case K_SPACE: key = "SPACE";
            break;
        default: continue;
        }
        if (g_skKeyEvent[i].keyDown)
            ss << key << " pressed";
        else if (g_skKeyEvent[i].keyReleased)
            ss << key << " released";
        else
            ss << key << " not pressed";

        COORD c = { startPos.X, startPos.Y + i };
        g_Console.writeToBuffer(c, ss.str(), 0x17);
    }

    // mouse events    
    ss.str("");
    ss << "Mouse position (" << g_mouseEvent.mousePosition.X << ", " << g_mouseEvent.mousePosition.Y << ")";
    g_Console.writeToBuffer(g_mouseEvent.mousePosition, ss.str(), 0x59);
    ss.str("");
    switch (g_mouseEvent.eventFlags) {
    case 0:
        if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
            ss.str("Left Button Pressed");
            g_Console.writeToBuffer(20, 20, ss.str(), 0x59);
        }
        else if (g_mouseEvent.buttonState == RIGHTMOST_BUTTON_PRESSED) {
            ss.str("Right Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 2, ss.str(), 0x59);
        }
        else {
            ss.str("Some Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 3, ss.str(), 0x59);
        }
        break;
    case DOUBLE_CLICK:
        ss.str("Double Clicked");
        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 4, ss.str(), 0x59);
        break;
    case MOUSE_WHEELED:
        if (g_mouseEvent.buttonState & 0xFF000000)
            ss.str("Mouse wheeled down");
        else
            ss.str("Mouse wheeled up");
        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 5, ss.str(), 0x59);
        break;
    default:
        break;
    }
}

void loadLevelData(int number) {
    int n1 = 0; int n = 0; int b = 0; int h = 0;
    int y = 0; int e = 0; int o = 0; int po = 0;
    int t = 0; int tt = 0; int bp = 0;
    std::string line2;
    if (characterSelect == 0)
    {
        if (number == 0) {
            BGcolor = 0x80;
            levelFile = "Dewmtutorial_1.txt";
        }
        if (number == 1) {
            levelFile = "Dewmtutorial_2.txt";
        }
        if (number == 2) {
            levelFile = "Dewmtutorial_3.txt";
        }
        if (number == 3) {
            levelFile = "DewmIntro.txt";
        }
        if (number == 4) {
            BGcolor = 0x40;
            levelFile = "lvlDewm1.txt";
        }
        if (number == 5) {
            levelFile = "lvlDewm2.txt";
        }
        if (number == 6) {
            levelFile = "lvlDewm3.txt";
        }
        if (number == 7) {
            levelFile = "lvlDewm4.txt";
        }
        if (number == 8) {
            BGcolor = 0x10;
            levelFile = "lvlSeraph1.txt";
        }
        if (number == 9) {
            levelFile = "lvlSeraph2.txt";
        }
        if (number == 10) {
            levelFile = "lvlSeraph3.txt";
        }
        if (number == 11) {
            levelFile = "lvlSeraph4.txt";
        }
        if (number == 12) {
            BGcolor = 0x10;
            levelFile = "lvlNinja1.txt";
        }
        if (number == 13) {
            levelFile = "lvlNinja2.txt";
        }
        if (number == 14) {
            levelFile = "lvlNinja3.txt";
        }
        if (number == 15) {
            BGcolor = 0x70;
            levelFile = "lvlNinja4.txt";
        }
        if (number == 16) {
            BGcolor = 0x30;
            levelFile = "lvlVik1.txt";
        }
        if (number == 17) {
            levelFile = "lvlVik2.txt";
        }
        if (number == 18) {
            levelFile = "lvlVik3.txt";
        }
        if (number == 19) {
            levelFile = "lvlVik4.txt";
        }
    }
    else if (characterSelect == 1)
    {
        if (number == 0) {
            BGcolor = 0x10;
            levelFile = "SeraphIntro.txt";
        }
        if (number == 1) {
            levelFile = "Seraphtutorial_1.txt";
        }
        if (number == 2) {
            levelFile = "Seraphtutorial_2.txt";
        }
        if (number == 3) {
            levelFile = "lvlSeraph1.txt";
        }
        if (number == 4) {
            levelFile = "lvlSeraph2.txt";
        }
        if (number == 5) {
            levelFile = "lvlSeraph3.txt";
        }
        if (number == 6) {
            levelFile = "lvlSeraph4.txt";
        }
        if (number == 7) {
            BGcolor = 0x40;
            levelFile = "lvlDewm1.txt";
        }
        if (number == 8) {
            levelFile = "lvlDewm2.txt";
        }
        if (number == 9) {
            levelFile = "lvlDewm3.txt";
        }
        if (number == 10) {
            levelFile = "lvlDewm4.txt";
        }
        if (number == 11) {
            BGcolor = 0x10;
            levelFile = "lvlNinja1.txt";
        }
        if (number == 12) {
            levelFile = "lvlNinja2.txt";
        }
        if (number == 13) {
            levelFile = "lvlNinja3.txt";
        }
        if (number == 14) {
            BGcolor = 0x70;
            levelFile = "lvlNinja4.txt";
        }
        if (number == 15) {
            BGcolor = 0x30;
            levelFile = "lvlVik1.txt";
        }
        if (number == 16) {
            levelFile = "lvlVik2.txt";
        }
        if (number == 17) {
            levelFile = "lvlVik3.txt";
        }
        if (number == 18) {
            levelFile = "lvlVik4.txt";
        }
    }
    else if (characterSelect == 2)
    {
        if (number == 0) {
            BGcolor = 0x90;
            levelFile = "GinIntro.txt";
        }
        if (number == 1) {
            levelFile = "Gintutorial_1.txt";
        }
        if (number == 2) {
            levelFile = "Gintutorial_2.txt";
        }
        if (number == 3) {
            levelFile = "Gintutorial_3.txt";
        }
        if (number == 4) {
            BGcolor = 0x10;
            levelFile = "lvlNinja1.txt";
        }
        if (number == 5) {
            levelFile = "lvlNinja2.txt";
        }
        if (number == 6) {
            levelFile = "lvlNinja3.txt";
        }
        if (number == 7) {
            BGcolor = 0x70;
            levelFile = "lvlNinja4.txt";
        }
        if (number == 8) {
            BGcolor = 0x40;
            levelFile = "lvlDewm1.txt";
        }
        if (number == 9) {
            levelFile = "lvlDewm2.txt";
        }
        if (number == 10) {
            levelFile = "lvlDewm3.txt";
        }
        if (number == 11) {
            levelFile = "lvlDewm4.txt";
        }
        if (number == 12) {
            BGcolor = 0x10;
            levelFile = "lvlSeraph1.txt";
        }
        if (number == 13) {
            levelFile = "lvlSeraph2.txt";
        }
        if (number == 14) {
            levelFile = "lvlSeraph3.txt";
        }
        if (number == 15) {
            BGcolor = 0x10;
            levelFile = "lvlSeraph4.txt";
        }
        if (number == 16) {
            BGcolor = 0x30;
            levelFile = "lvlVik1.txt";
        }
        if (number == 17) {
            levelFile = "lvlVik2.txt";
        }
        if (number == 18) {
            levelFile = "lvlVik3.txt";
        }
        if (number == 19) {
            levelFile = "lvlVik4.txt";
        }
    }
    else if (characterSelect == 3)
    {
        if (number == 0) {
            BGcolor = 0x30;
            levelFile = "lvlVik1.txt";
        }
        if (number == 1) {
            levelFile = "lvlVik2.txt";
        }
        if (number == 2) {
            levelFile = "lvlVik3.txt";
        }
        if (number == 3) {
            levelFile = "lvlVik4.txt";
        }
        if (number == 4) {
            BGcolor = 0x40;
            levelFile = "lvlDewm1.txt";
        }
        if (number == 5) {
            levelFile = "lvlDewm2.txt";
        }
        if (number == 6) {
            levelFile = "lvlDewm3.txt";
        }
        if (number == 7) {
            levelFile = "lvlDewm4.txt";
        }
        if (number == 8) {
            BGcolor = 0x10;
            levelFile = "lvlSeraph1.txt";
        }
        if (number == 9) {
            levelFile = "lvlSeraph2.txt";
        }
        if (number == 10) {
            levelFile = "lvlSeraph3.txt";
        }
        if (number == 11) {
            BGcolor = 0x10;
            levelFile = "lvlSeraph4.txt";
        }
        if (number == 12) {
            BGcolor = 0x10;
            levelFile = "lvlNinja1.txt";
        }
        if (number == 13) {
            levelFile = "lvlNinja2.txt";
        }
        if (number == 14) {
            levelFile = "lvlNinja3.txt";
        }
        if (number == 15) {
            BGcolor = 0x70;
            levelFile = "lvlNinja4.txt";
        }
    }
    std::ifstream level;
    std::ifstream levelPtr(levelFile);
    level.open(levelFile);
    char c = level.get();
    while (level.good()) {
        if (c == 'P') {
            g_iPlatforms++;
        }
        c = level.get();
    }
    level.close();
    int j = 1;
    g_aPlatformsX = new int[g_iPlatforms];
    g_aPlatformsY = new int[g_iPlatforms];
    if (levelPtr.is_open()) {
        std::string perLine;
        while (std::getline(levelPtr, line2)) {
            perLine = line2;
            for (int x = 0; x < perLine.length(); x++) {
                if (perLine[x] == 'P') {
                    g_aPlatformsX[j - 1] = x;
                    g_aPlatformsY[j - 1] = y;
                    j++;
                }
                if (perLine[x] == 'C') {
                    g_sCharSpawn.m_cLocation.X = x;
                    g_sCharSpawn.m_cLocation.Y = y;
                }
                if (perLine[x] == '0') {
                    g_sEnemy[0].m_cLocation.X = x;
                    g_sEnemy[0].m_cLocation.Y = y;
                }
                if (perLine[x] == '1') {
                    g_sEnemy[1].m_cLocation.X = x;
                    g_sEnemy[1].m_cLocation.Y = y;
                }
                if (perLine[x] == '2') {
                    g_sEnemy[2].m_cLocation.X = x;
                    g_sEnemy[2].m_cLocation.Y = y;
                }
                if (perLine[x] == '3') {
                    g_sEnemy[3].m_cLocation.X = x;
                    g_sEnemy[3].m_cLocation.Y = y;
                }
                if (perLine[x] == '4') {
                    g_sEnemy[4].m_cLocation.X = x;
                    g_sEnemy[4].m_cLocation.Y = y;
                }
                if (perLine[x] == '5') {
                    g_sEnemy[5].m_cLocation.X = x;
                    g_sEnemy[5].m_cLocation.Y = y;
                }
                if (perLine[x] == '6') {
                    g_sEnemy[6].m_cLocation.X = x;
                    g_sEnemy[6].m_cLocation.Y = y;
                }
                if (perLine[x] == '7') {
                    g_sEnemy[7].m_cLocation.X = x;
                    g_sEnemy[7].m_cLocation.Y = y;
                }
                if (perLine[x] == '8') {
                    g_sEnemy[8].m_cLocation.X = x;
                    g_sEnemy[8].m_cLocation.Y = y;
                }
                if (perLine[x] == '9') {
                    g_sEnemy[9].m_cLocation.X = x;
                    g_sEnemy[9].m_cLocation.Y = y;
                }
                if (perLine[x] == 'B') {
                    g_sBoss1[0].m_cLocation.X = x;
                    g_sBoss1[0].m_cLocation.Y = y;
                    g_sBoss1[1].m_cLocation.X = x;
                    g_sBoss1[1].m_cLocation.Y = y - 1;
                }
                if (perLine[x] == 'S')
                {
                    g_sBoss2.m_cLocation.X = x;
                    g_sBoss2.m_cLocation.Y = y;
                }
                if (perLine[x] == 'M')
                {
                    g_sBProj[bp].m_cLocation.X = x;
                    g_sBProj[bp].m_cLocation.Y = y;
                }
                if (perLine[x] == 'O')
                {
                    g_sPortal.m_cLocation.X = x;
                    g_sPortal.m_cLocation.Y = y;
                }
                if (perLine[x] == 'N')
                {
                    g_sNPC[n].m_cLocation.X = x;
                    g_sNPC[n].m_cLocation.Y = y;
                    n++;
                }
                if (perLine[x] == 'H')
                {
                    g_sPotion[h].m_cLocation.X = x;
                    g_sPotion[h].m_cLocation.Y = y;
                    h++;
                }
                if (perLine[x] == 'Q')
                {
                    g_sObj[o].m_cLocation.X = x;
                    g_sObj[o].m_cLocation.Y = y;
                    o++;
                }
                if (perLine[x] == 'q')
                {
                    g_sPrimeObj[po].m_cLocation.X = x;
                    g_sPrimeObj[po].m_cLocation.Y = y;
                    po++;
                }
                if (perLine[x] == 'T')
                {
                    g_sbCutscene[t].m_cLocation.X = x;
                    g_sbCutscene[t].m_cLocation.Y = y;
                    t++;
                }
                if (perLine[x] == 'U')
                {
                    g_sTrainTracks[tt].m_cLocation.X = x;
                    g_sTrainTracks[tt].m_cLocation.Y = y;
                    tt++;
                }
            }
            y++;
        }
    }
    levelPtr.close();
}

void LEMoveChar() {
    if (g_mouseEvent.mousePosition.X == g_sChar.m_cLocation.X && g_mouseEvent.mousePosition.Y == g_sChar.m_cLocation.Y) {
        if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
            dragging = true;
        }
        else {
            dragging = false;
        }
    }
    if (g_mouseEvent.mousePosition.X == (g_sChar.m_cLocation.X + 1) && g_mouseEvent.mousePosition.Y == g_sChar.m_cLocation.Y) {
        if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
            dragging = true;
        }
        else {
            dragging = false;
        }
    }
    if (dragging) {
        g_sChar.m_cLocation.X = g_mouseEvent.mousePosition.X;
        g_sChar.m_cLocation.Y = g_mouseEvent.mousePosition.Y;
    }
}

void LEMarkEnemy() {

}

void LEMarkPlatform() {

}

void writeLevel() {

}

void renderWin() {
    COORD c;
    c.X = g_Console.getConsoleSize().X / 2;
    c.Y = g_Console.getConsoleSize().Y / 2;
    g_Console.writeToBuffer(c, "Win", 0x0F);
}

void renderLose() {
    COORD c;
    c.X = g_Console.getConsoleSize().X / 2;
    c.Y = g_Console.getConsoleSize().Y / 2;
    c.X -= 2;
    g_Console.writeToBuffer(c, "Game Over", 0x0F);
    c.Y += 1;
    c.X -= 2;
    g_Console.writeToBuffer(c, "Press <Esc> to restart", 0x0F);
}

void scroll() {
    int move;
    int c = g_Console.getConsoleSize().X / 2 - 1;
    if (g_sChar.m_cLocation.X > c) {
        move = g_sChar.m_cLocation.X - c;
        for (int i = 0; i < g_iPlatforms; i++) {
            g_aPlatformsX[i] = g_aPlatformsX[i] - move;
        }
        for (int i = 0; i < ne; i++) {
            g_sEnemy[i].m_cLocation.X = g_sEnemy[i].m_cLocation.X - move;
        }
        for (int i = 0; i <= obj; i++) {
            g_sObj[i].m_cLocation.X = g_sObj[i].m_cLocation.X - move;
        }
        for (int i = 0; i < 2; i++) {
            g_sPrimeObj[i].m_cLocation.X = g_sPrimeObj[i].m_cLocation.X - move;
        }
        g_sPortal.m_cLocation.X = g_sPortal.m_cLocation.X - move;
        g_sChar.m_cLocation.X = c;
    }
    if (g_sChar.m_cLocation.X < c) {
        move = c - g_sChar.m_cLocation.X;
        for (int i = 0; i < g_iPlatforms; i++) {
            g_aPlatformsX[i] = g_aPlatformsX[i] + move;
        }
        for (int i = 0; i < ne; i++) {
            g_sEnemy[i].m_cLocation.X = g_sEnemy[i].m_cLocation.X + move;
        }
        for (int i = 0; i <= obj; i++) {
            g_sObj[i].m_cLocation.X = g_sObj[i].m_cLocation.X + move;
        }
        for (int i = 0; i < 2; i++) {
            g_sPrimeObj[i].m_cLocation.X = g_sPrimeObj[i].m_cLocation.X + move;
        }
        g_sPortal.m_cLocation.X = g_sPortal.m_cLocation.X + move;
        g_sChar.m_cLocation.X = c;
    }
}

void reset() {
    g_dElapsedTime = 0.0; characterSelect;
    oneTime = 0; canDo = 0; level = 0; saveTimer = 0;
    for (int i = 0; i <= ne; i++)
    {
        g_sEnemy[i].m_dHealth = 5;
    }
    for (int i = 0; i <= obj; i++)
    {
        g_sObj[i].m_dHealth = 5;
    }
    g_sChar.m_dMana = 0;
    g_sChar.m_dHealth = 50;
}


void renderIntro() {
    string introFile; string line; int y = 0;
    if (characterSelect == 0) {
        introFile = "dewmIntroscreen.txt";
    }
    if (characterSelect == 1) {
        introFile = "seraphIntroscreen.txt";
    }
    if (characterSelect == 2) {
        introFile = "ginIntroscreen.txt";
    }
    if (characterSelect == 3) {
        introFile = "thorfinnIntroscreen.txt";
    }
    ifstream intro(introFile);
    if (intro.is_open()) {
        std::string perLine;
        while (std::getline(intro, line)) {
            perLine = line;
            for (int x = 0; x < perLine.length(); x++) {
                if (perLine[x] == '.') {
                    g_Console.writeToBuffer(x, y, " ", 0x00);
                }
                if (perLine[x] == '8') {
                    g_Console.writeToBuffer(x, y, " ", BACKGROUND_RED);
                }
            }
            y++;
        }
    }
}

void renderNPCDialogue() {
    if (levelFile == "DewmIntro.txt")
    {
        if (g_sChar.m_cLocation.Y == g_sNPC[0].m_cLocation.Y) {
            if ((g_sChar.m_cLocation.X > g_sNPC[0].m_cLocation.X - 3) && (g_sChar.m_cLocation.X < g_sNPC[0].m_cLocation.X + 3)) {
                g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 3, g_sNPC[0].m_cLocation.Y - 1, "  ...!?", 0x80);
            }
            if ((g_sChar.m_cLocation.X > g_sNPC[1].m_cLocation.X - 3) && (g_sChar.m_cLocation.X < g_sNPC[1].m_cLocation.X + 3)) {
                g_Console.writeToBuffer(g_sNPC[1].m_cLocation.X - 3, g_sNPC[1].m_cLocation.Y - 1, "Excuse me..?", 0x80);
            }
            if ((g_sChar.m_cLocation.X > g_sNPC[2].m_cLocation.X - 3) && (g_sChar.m_cLocation.X < g_sNPC[2].m_cLocation.X + 3)) {
                g_Console.writeToBuffer(g_sNPC[2].m_cLocation.X - 1, g_sNPC[2].m_cLocation.Y - 1, "It's him...", 0x80);
            }
            if ((g_sChar.m_cLocation.X > g_sNPC[3].m_cLocation.X - 3) && (g_sChar.m_cLocation.X < g_sNPC[3].m_cLocation.X + 3)) {
                g_Console.writeToBuffer(g_sNPC[3].m_cLocation.X - 3, g_sNPC[3].m_cLocation.Y - 1, " Huh!?", 0x80);
            }
            if ((g_sChar.m_cLocation.X > g_sNPC[4].m_cLocation.X - 3) && (g_sChar.m_cLocation.X < g_sNPC[4].m_cLocation.X + 3)) {
                g_Console.writeToBuffer(g_sNPC[4].m_cLocation.X - 3, g_sNPC[4].m_cLocation.Y - 1, "What the..", 0x80);
            }
            if ((g_sChar.m_cLocation.X > g_sNPC[5].m_cLocation.X - 3) && (g_sChar.m_cLocation.X < g_sNPC[5].m_cLocation.X + 3) && g_sNPC[5].m_cLocation.X < g_sNPC[6].m_cLocation.X) {
                g_Console.writeToBuffer(g_sNPC[5].m_cLocation.X - 6, g_sNPC[5].m_cLocation.Y - 1, "    Waaa!", 0x80);
            }
            if ((g_sChar.m_cLocation.X > g_sNPC[6].m_cLocation.X - 3) && (g_sChar.m_cLocation.X < g_sNPC[6].m_cLocation.X + 3)) {
                g_Console.writeToBuffer(g_sNPC[6].m_cLocation.X - 9, g_sNPC[6].m_cLocation.Y - 1, "     Oh sh*t..", 0x80);
            }
            if ((g_sChar.m_cLocation.X > g_sNPC[7].m_cLocation.X - 3) && (g_sChar.m_cLocation.X < g_sNPC[7].m_cLocation.X + 3)) {
                g_Console.writeToBuffer(g_sNPC[7].m_cLocation.X - 3, g_sNPC[7].m_cLocation.Y - 1, "  :O", 0x80);
            }
            if ((g_sChar.m_cLocation.X > g_sNPC[8].m_cLocation.X - 3) && (g_sChar.m_cLocation.X < g_sNPC[8].m_cLocation.X + 3)) {
                g_Console.writeToBuffer(g_sNPC[8].m_cLocation.X - 1, g_sNPC[8].m_cLocation.Y - 1, " !!!", 0x80);
            }
        }
    }
    if (levelFile == "SeraphIntro.txt")
    {
        if (g_sChar.m_cLocation.Y == g_sNPC[0].m_cLocation.Y) {
            if (cut == 7 && g_cDelayTime < 2) {
                g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 2, g_sNPC[0].m_cLocation.Y - 1, "Mifa: Seraph!? ", 0x1E);
            }
            if (cut == 7 && g_cDelayTime > 2 && g_cDelayTime < 4) {
                g_Console.writeToBuffer(g_sNPC[1].m_cLocation.X - 2, g_sNPC[1].m_cLocation.Y - 1, "Aireth:Hey..", 0x1E);
            }
            if (cut == 7 && g_cDelayTime > 4 && g_cDelayTime < 6) {
                g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 2, g_sNPC[0].m_cLocation.Y - 1, "Mifa: It's been such a long time...", 0x1E);
            }
            if (cut == 7 && g_cDelayTime > 6 && g_cDelayTime < 8) {
                g_Console.writeToBuffer(g_sNPC[2].m_cLocation.X - 9, g_sNPC[2].m_cLocation.Y - 1, "Warren: Well yeah we have to get moving soon", 0x1E);
            }
            if (cut == 7 && g_cDelayTime > 8 && g_cDelayTime < 11) {
                g_Console.writeToBuffer(g_sNPC[2].m_cLocation.X - 9, g_sNPC[2].m_cLocation.Y - 1, "My senses telling me that something's bad about to happen", 0x1E);
            }
            if (cut == 7 && g_cDelayTime > 11 && g_cDelayTime < 13) {
                g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 6, g_sNPC[0].m_cLocation.Y - 1, "Mifa: Warren... you worry too much", 0x1E);
            }
            if (cut == 7 && g_cDelayTime > 15 && g_cDelayTime < 17) {
                g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 3, g_sNPC[0].m_cLocation.Y - 1, "Let's walk shall we?", 0x1E);
            }
            if (cut > 7 && cut < 62 && g_cDelayTime > 19 && g_cDelayTime < 22)
            {
                g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 3, g_sNPC[0].m_cLocation.Y - 1, "Seraph, now that you're here.. ", 0x1E);
            }
            if (cut > 7 && cut < 62 && g_cDelayTime > 22 && g_cDelayTime < 25)
            {
                g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 3, g_sNPC[0].m_cLocation.Y - 1, "The people, they need you", 0x1E);
            }
            if (cut > 7 && cut < 62 && g_cDelayTime > 25 && g_cDelayTime < 28)
            {
                g_Console.writeToBuffer(g_sNPC[2].m_cLocation.X - 9, g_sNPC[2].m_cLocation.Y - 2, "Warren(sarcastically): ", 0x1E);
                g_Console.writeToBuffer(g_sNPC[2].m_cLocation.X - 9, g_sNPC[2].m_cLocation.Y - 1, "You picked a great time to come back buddy", 0x1E);
            }
            if (cut > 7 && cut < 62 && g_cDelayTime > 28 && g_cDelayTime < 31)
            {
                g_Console.writeToBuffer(g_sNPC[2].m_cLocation.X - 13, g_sNPC[2].m_cLocation.Y - 1, "Ever since you left, Gaia has gone to hell", 0x1E);
            }
            if (cut > 7 && cut < 62 && g_cDelayTime > 31 && g_cDelayTime < 34)
            {
                g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 9, g_sNPC[2].m_cLocation.Y - 1, "Mifa: Warren, don't be like that..", 0x1E);
            }
            if (cut == 62 && g_cDelayTime < 3)
            {
                g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 7, g_sNPC[2].m_cLocation.Y - 1, "It's partly our fault too", 0x1E);
            }
            if (cut == 62 && g_cDelayTime > 3 && g_cDelayTime < 6)
            {
                g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 25, g_sNPC[2].m_cLocation.Y - 1, "Sorry Seraph but, we were actually waiting here...", 0x1E);
            }
            if (cut == 62 && g_cDelayTime > 6 && g_cDelayTime < 8)
            {
                g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 25, g_sNPC[2].m_cLocation.Y - 1, "Warren: Yeah, we're actually waiting for our train", 0x1E);
            }
            if (cut == 62 && g_cDelayTime > 8 && g_cDelayTime < 10)
            {
                g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 20, g_sNPC[2].m_cLocation.Y - 1, "We don't have time for a deserter like you", 0x1E);
            }
            if (cut == 62 && g_cDelayTime > 10 && g_cDelayTime < 11)
            {
                g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 2, g_sNPC[2].m_cLocation.Y - 1, "Warren!", 0x1E);
            }
            if (cut == 62 && g_cDelayTime > 11 && g_cDelayTime < 13)
            {
                g_Console.writeToBuffer(g_sNPC[1].m_cLocation.X - 25, g_sNPC[1].m_cLocation.Y - 1, "Aireth: How about, we just say our last goodbyes", 0x1E);
            }
            if (cut == 62 && g_cDelayTime > 13 && g_cDelayTime < 15)
            {
                g_Console.writeToBuffer(g_sNPC[1].m_cLocation.X - 9, g_sNPC[1].m_cLocation.Y - 1, "and bid him farewell", 0x1E);
            }
            if (cut == 62 && g_cDelayTime > 15 && g_cDelayTime < 17)
            {
                g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 9, g_sNPC[0].m_cLocation.Y - 1, "Mifa: Yeah, you're right", 0x1E);
            }
            if (cut == 62 && g_cDelayTime > 17 && g_cDelayTime < 19)
            {
                g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 15, g_sNPC[0].m_cLocation.Y - 1, "Bye Seraph, it was nice seeing you..", 0x1E);
            }
            if (cut == 62 && g_cDelayTime > 19 && g_cDelayTime < 21)
            {
                g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 15, g_sNPC[0].m_cLocation.Y - 1, "Sorry for dragging you into this,", 0x1E);
            }
            if (cut == 62 && g_cDelayTime > 21 && g_cDelayTime < 23)
            {
                g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 10, g_sNPC[0].m_cLocation.Y - 1, "but you're our only hope", 0x1E);
            }
            if (cut > 62)
            {
                g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 6, g_sNPC[0].m_cLocation.Y - 1, "See you!", 0x1E);
                g_Console.writeToBuffer(g_sNPC[2].m_cLocation.X - 6, g_sNPC[2].m_cLocation.Y - 2, "Farewell", 0x1E);
            }
      
        }
    }
    if (levelFile == "GinIntro.txt")
    {
        if (cut > 22 && cut < 38) {
            g_Console.writeToBuffer(g_sChar.m_cLocation.X - 1, g_sChar.m_cLocation.Y-1, "....", 0x90);
        }
        if (cut == 38 && g_cDelayTime < 3)
        {
            g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 9, g_sNPC[0].m_cLocation.Y-1, "AAAAAHHHH!!!", 0x90);
        }
        if (cut == 38 && g_cDelayTime > 1)
        {
            g_Console.writeToBuffer(g_sChar.m_cLocation.X - 1, g_sChar.m_cLocation.Y - 1, "...!?", 0x90);
        }
        if (cut >= 64 && cut < 112)
        {
            g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 5, g_sNPC[0].m_cLocation.Y - 1, "HELP! SOMEBODY!!", 0x90);
        }
        if (cut >= 88 && cut < 112)
        {
            g_Console.writeToBuffer(g_sNPC[1].m_cLocation.X - 3, g_sNPC[1].m_cLocation.Y - 1, "Get back here!", 0x90);
        }
        if (cut == 130 && g_cDelayTime < 2)
        {
            g_Console.writeToBuffer(g_sNPC[0].m_cLocation.X - 3, g_sNPC[0].m_cLocation.Y - 1, "NOOOO!!!", 0x90);
        }
        if (cut == 130 && g_cDelayTime > 2 && g_cDelayTime < 3)
        {
            g_Console.writeToBuffer(g_sNPC[1].m_cLocation.X, g_sNPC[1].m_cLocation.Y - 1, "Shut up!", 0x90);
        }
        if (cut == 130 && g_cDelayTime > 3 && g_cDelayTime < 5)
        {
            g_Console.writeToBuffer(g_sNPC[2].m_cLocation.X - 9, g_sNPC[2].m_cLocation.Y - 1, "You're coming with us, young lady!", 0x90);
        }
        if (cut > 130 && cut < 145)
        {
            g_Console.writeToBuffer(g_sNPC[1].m_cLocation.X - 3, g_sNPC[1].m_cLocation.Y - 1, "Get moving you mutt!", 0x90);
        }
    }
}

string sendSaveData() {
    int x, y, hl, mn, ex[10], ey[10], eh[10],
        bx, by, bh;
    string c, cx, cy, hlth, mana, l, ste, enx[10], eny[10], enh[10],
        bosx, bosy, bosh, save;
    x = g_sChar.m_cLocation.X; y = g_sChar.m_cLocation.Y; mn = g_sChar.m_dMana; hl = g_sChar.m_dHealth;
    cx = to_string(x); cy = to_string(y); mana = to_string(mn); hlth = to_string(hl);
    l = to_string(level); c = to_string(characterSelect);

    if (cx.length() == 1) cx = "0" + cx;
    if (cy.length() == 1) cy = "0" + cy;
    if (hlth.length() == 1) hlth = "00" + hlth;
    if (hlth.length() == 2) hlth = "0" + hlth;
    if (mana.length() == 1) mana = "00" + mana;
    if (mana.length() == 2) mana = "0" + mana;
    if (g_sChar.m_bActive == false) ste = "1";
    if (g_sChar.m_bActive == true) ste = "0";

    save = c + cx + cy + hlth + mana + l + ste;

    for (int i = 0; i < ne; i++) {
        ex[i] = g_sEnemy[i].m_cLocation.X; ey[i] = g_sEnemy[i].m_cLocation.Y; eh[i] = g_sEnemy[i].m_dHealth;
        enx[i] = to_string(ex[i]); eny[i] = to_string(ey[i]); enh[i] = to_string(eh[i]);
        if (enx[i].length() == 1) enx[i] = "0" + enx[i];
        if (eny[i].length() == 1) eny[i] = "0" + eny[i];
        if (enh[i].length() == 1) enh[i] = "0" + enh[i];

        save = save + enx[i] + eny[i] + enh[i];
    }

    bx = g_sBoss1[0].m_cLocation.X; by = g_sBoss1[0].m_cLocation.Y; bh = g_sBoss1[0].m_dHealth;
    bosx = to_string(bx); bosy = to_string(by); bosh = to_string(bh);
    if (bosx.length() == 1) bosx = "0" + bosx;
    if (bosy.length() == 1) bosy = "0" + bosy;
    if (bosh.length() == 1) bosh = "00" + bosh;
    if (bosh.length() == 2) bosh = "0" + bosh;

    save = save + bosx + bosy + bosh;

    return save;
}

void loadSavedGame() {
    characterSelect = state.returnChar();
    g_sChar.m_cLocation.X = state.returnX();
    g_sChar.m_cLocation.Y = state.returnY();
    g_sChar.m_dMana = state.returnCharMana();
    g_sProj.m_cLocation.X = g_sChar.m_cLocation.X;
    g_sProj.m_cLocation.Y = g_sChar.m_cLocation.Y;
    g_sChar.m_bActive = state.returnCharState();
    g_sChar.m_dHealth = state.returnCharHealth();
    for (int i = 0; i < ne; i++) {
        g_sEnemy[i].m_cLocation.X = state.returnEnemyX(i);
        g_sEnemy[i].m_cLocation.Y = state.returnEnemyY(i);
        g_sEnemy[i].m_dHealth = state.returnEnemyH(i);
    }
    level = state.returnLevel();
    if (level > 4) {
        g_sBoss1[0].m_dHealth = state.returnBossH();
        g_sBoss1[0].m_cLocation.X = state.returnBossX();
        g_sBoss1[0].m_cLocation.Y = state.returnBossY();
        g_sBoss1[1].m_cLocation.X = state.returnBossX();
        g_sBoss1[1].m_cLocation.Y = state.returnBossY() - 1;
    }
}