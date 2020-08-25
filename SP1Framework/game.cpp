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
int* g_aPlatformsX;
int* g_aPlatformsY;
int g_iPlatforms = 0;
double  g_dElapsedTime; //delta time elapsed
double  g_dDeltaTime;   //delta time
double  g_pElapsedTime; //projectile elapsed time
double  g_eElapsedTime; //enemy movement elapsed time
double  g_sElapsedTime; //slash elapsed time
double  g_doElapsedTime;//Dodge elapsed time
double  g_bElapsedTime; //boss movement elapsed time
double  g_bjElapsedTime;//boss jump elapsed time
double  g_jElapsedTime; //player jump elapsed time
double  g_hElapsedTime; //health elapsed time
double  g_delay1;       //boss movement delay 1
double  g_delay2;       //boss movement delay 2
double  g_slashdelay;   //slash attack delay
double  g_stun = 2;     //enemy stunned when attacked
double  g_staminaregen; //regen stamina after slashing
double  g_suElapsedTime;//Seraph ult elapsed time
double  g_udElapsedTime;//Seraph ulti delay elapsed time
double  g_uElapsedTime; //ultimate elapsed time
int characterSelect;
int oneTime;
int canDo;

//UI specific timers
int g_iElapsedTime;
int g_iTimeAfter;

//mechanics counters
int i = 0;       //enemy movement counter
int j = 0;       //projectile counter
int k = 0;       //boss movement counter
int l = 0;       //player movement counter
int s = 0;       //slash attack counter
int n = 2;       //control movement speed (used for movement && sneak)
int p = 0;       //slash delay counter and down slam condition
int bossd = 3;   //boss initial location
int dashl = 0;   //counter for dodge (left)
int dashr = 0;   //counter for dodge (right)
int su = 0;      //ultimate counter
int f = 0;       //focus counter
int stun = 0;    //stun counter
double sd = 0.2; //slash delay time elapsed condition

//mechanic for Seraph's ult
int y;           //cmd screen set centre y
int x;           //cmd screen set centre x

int slam = 0; //slam down attack counter

//start/end game boolean
bool g_bPlayGame = false;
bool g_sAttackState = false;
bool g_sInvulnerable = false;
bool g_sUltimate = false;
bool g_sRage = false;
bool g_sFocus = false;

char c1 = 203;                     //main player's ascii characters
char c2 = 203;
auto c3 = std::string(1, c1) + c2; //combined into 2

char c4 = 205;                     //projectile ascii

char c5[5] = { 152, 152, 152, 152, 152 };                     //Enemy ascii

char c6 = 232;                     //mini - Boss ascii (c6 - 9)
char c7 = 232;
auto boss1 = std::string(3, c6) + c7; //combined into 2
char c8 = 186;
char c9 = 186;
auto boss2 = std::string(3, c8) + c9;

WORD enemyColor[5] = { 0x4F, 0x4F, 0x4F, 0x4F, 0x4F };
WORD bossColor = 0x4E;
// bool g_bInMenu = false;
SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;

save state;
// Game specific variables here
std::string status;
SGameChar   g_sChar;
SGameChar   g_sCharSpawn;
SGameChar   g_sProj;
SGameChar   g_sEnemy[5];
SGameChar   g_sBossP1;  //Boss bottom half
SGameChar   g_sBossP2;  //Boss top half
EGAMESTATES g_eGameState = S_SPLASHSCREEN; // initial state

// Console object
Console g_Console(100, 30, "Ninjas, monsters n' robots");

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init(void) {

    // Set precision for floating point output
    g_dElapsedTime = 0.0;
    characterSelect = 0;
    oneTime = 0;
    canDo = 0;
    
    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

    //loadLevelData(1);

    std::ifstream save;
    save.open("save.txt");
    if (!save)
        state.defaultSave();
    state.loadSave();
    g_sChar.m_cLocation.X = state.returnX();
    g_sChar.m_cLocation.Y = state.returnY();
    g_sChar.m_cLocation.X = 2;
    g_sChar.m_cLocation.Y = 28;
    g_sEnemy[0].m_cLocation.X = 10;
    g_sEnemy[0].m_cLocation.Y = 28;
    g_sEnemy[1].m_cLocation.X = 20;
    g_sEnemy[1].m_cLocation.Y = 28;
    g_sEnemy[2].m_cLocation.X = 30;
    g_sEnemy[2].m_cLocation.Y = 28;
    g_sEnemy[3].m_cLocation.X = 40;
    g_sEnemy[3].m_cLocation.Y = 28;
    g_sEnemy[4].m_cLocation.X = 55;
    g_sEnemy[4].m_cLocation.Y = 28;
    g_sBossP1.m_cLocation.X = state.returnBossX();
    g_sBossP1.m_cLocation.Y = state.returnBossY();
    g_sBossP2.m_cLocation.X = state.returnBossX();
    g_sBossP2.m_cLocation.Y = state.returnBossY() - 1;
    g_sProj.m_cLocation.X = g_sChar.m_cLocation.X;
    g_sProj.m_cLocation.Y = g_sChar.m_cLocation.Y;
    g_sChar.m_bActive = state.returnCharState();
    g_sChar.m_dHealth = state.returnCharHealth();
    g_sEnemy[0].m_dHealth = 5;
    g_sEnemy[1].m_dHealth = 5;
    g_sEnemy[2].m_dHealth = 5;
    g_sEnemy[3].m_dHealth = 5;
    g_sEnemy[4].m_dHealth = 5;
    g_sBossP1.m_dHealth = 50;
    g_sChar.m_dMana = state.returnCharMana();
    g_sChar.m_dMana = 50;
    y = g_Console.getConsoleSize().Y - 2;
    x = g_Console.getConsoleSize().X - 2;
    
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
    saveGame();
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
    case S_GAME: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event 
        break;
    case S_MENU: gameplayKBHandler(keyboardEvent);
        break;
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
    }
}

void saveGame() {
    /*state.saveState(std::to_string(g_sChar.m_cLocation.X), std::to_string(g_sChar.m_cLocation.Y), status, std::to_string(g_sEnemy.m_dHealth));*/
    /*state.saveState(std::to_string(g_sEnemy.m_cLocation.X), std::to_string(g_sEnemy.m_cLocation.Y), std::to_string(g_sBossP1.m_cLocation.X), std::to_string(g_sBossP1.m_cLocation.Y));*/
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
    EKEYS key = K_COUNT;
    switch (keyboardEvent.wVirtualKeyCode) {
    case 0x57: key = K_57; break; //W
    case 0x53: key = K_53; break; //S
    case 0x41: key = K_41; break; //A
    case 0x44: key = K_44; break; //D
    case 0x52: key = K_52; break; //R
    case 0x51: key = K_51; break; //Q
    case 0x45: key = K_45; break; //E
    case VK_SPACE: key = K_SPACE; break;
    case VK_ESCAPE: key = K_ESCAPE; break;
    case VK_RETURN: key = K_ENTER; break;
    case VK_LEFT: key = K_LEFT; break;
    case VK_RIGHT: key = K_RIGHT; break;
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
    case S_GAME: updateGame(); updateTime(dt); // gameplay logic when we are in the game
        break;
    case S_MENU: updateMenu();
        break;
    }
    //processUserInput();
}

void updateTime(double dt) {
    g_dElapsedTime += dt; //game time elapsed
    g_dDeltaTime = dt;    //seconds between each frame (if 90fps, deltatime = 1/90)
    g_pElapsedTime += dt; //Projectile time elapsed
    g_eElapsedTime += dt; //Enemy movement time elapsed
    g_sElapsedTime += dt; //Slash movement time elapsed
    g_doElapsedTime += dt; //Dodge time elapsed
    g_uElapsedTime += dt; //Ultimate meter time elapsed
    g_hElapsedTime += dt; //Health meter time elapsed
    g_jElapsedTime += dt; //Player jump time elapsed
    g_bElapsedTime += dt; //Boss movement time elapsed
    g_bjElapsedTime += dt; //boss jump elapsed time
    g_suElapsedTime += dt; //Seraph ultimate time elapsed
    g_udElapsedTime += dt; //Seraph ulti delay
    g_delay1 += dt; //boss movement delay 1
    g_delay2 += dt; //boss movement delay 2
    g_slashdelay += dt; //delay after each slash (applies to melee attacks)
    g_stun += dt; //stun enemy when hit
    g_staminaregen += dt; //regen stamina time
    g_iElapsedTime = (int)round(g_dElapsedTime);
    g_iTimeAfter = g_iElapsedTime + 1;
}

void splashScreenWait() {      // waits for time to pass in splash screen
    if (g_bPlayGame == true)   // wait for keyboard to switch to game mode, else do nothing
        g_eGameState = S_GAME;
    //renderSplashScreen();
    processUserInput();
}

void updateGame() {     // gameplay logic
    if (g_bPlayGame) {
        processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
        moveCharacter();    // moves the player by *2 steps, collision detection, physics, etc
        sneakCharacter();   // Gin sneak ability
        moveProjectile();   // sound can be played here too.
        //slashAttack(0.2, 10, 3); //slash forward by *10 steps, speed of slash is *.2 seconds
        downslam();            //Seraph down slam attack
        dodge(5);              //dodge for Gin
        moveEnemy(5, 0.5, 5); //move enemy by *5 steps back and forth from position x = *50 every *0.5 seconds
        moveBoss(30, 0.05, bossd, 10); //movement system same as enemy but with delay at end of path
        setdamage();       //find reason for damage
        setUltimate(50); //Set ultimate capacity to *50
        //seraphUlt(); //seraph star combo breaker
        focusAttack(); //Gin's focus ability
        //LEMoveChar();
        //scroll();
    }
    //state.saveState(std::to_string(g_sChar.m_cLocation.X), std::to_string(g_sChar.m_cLocation.Y), status, std::to_string(g_sProj.m_cLocation.X), std::to_string(g_sProj.m_cLocation.Y));
}

void updateMenu() {
    processUserInput();
}

//void scroll()
//{
//    if (g_skKeyEvent[K_44].keyDown && steps > 50)
//    {
//        g_sEnemy.m_cLocation.X = g_sEnemy.m_cLocation.X - 2;
//        g_sBossP1.m_cLocation.X = g_sBossP1.m_cLocation.X - 2;
//        g_sBossP2.m_cLocation.X = g_sBossP2.m_cLocation.X - 2;
//        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 2;
//        for (int i = 0; i <= g_iPlatforms; i++)
//        {
//            g_aPlatformsX[i] = g_aPlatformsX[i] - 2;
//        }
//    }
//    if (g_skKeyEvent[K_41].keyDown && steps > 50)
//    {
//        g_sEnemy.m_cLocation.X = g_sEnemy.m_cLocation.X + 2;
//        g_sBossP1.m_cLocation.X = g_sBossP1.m_cLocation.X + 2;
//        g_sBossP2.m_cLocation.X = g_sBossP2.m_cLocation.X + 2;
//        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 2;
//        for (int i = 0; i <= g_iPlatforms; i++)
//        {
//            g_aPlatformsX[i] = g_aPlatformsX[i] + 2;
//        }
//    }
//
//}

void moveCharacter() {
    // Updating the location of the character based on the key release
    // providing a beep sound whenver we shift the character
    int oneStep;
        if (g_skKeyEvent[K_57].keyDown) {
            for (int j = 0; j < g_iPlatforms; j++) {
                if (g_sChar.m_cLocation.Y+1 == g_aPlatformsY[j] && g_sChar.m_cLocation.X == g_aPlatformsX[j]) //Jump up
                {
                    l = 0;
                    l++;
                    g_sChar.m_cLocation.Y++;
                    for (int i = 0; i < g_iPlatforms; i++) {
                        if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X == g_aPlatformsX[i]) {
                            g_sChar.m_cLocation.Y--;
                        }
                    }
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
                }
                if (GetKeyState(0x41) & 0x800) //check for A input and jump left
                {
                    g_sChar.m_cLocation.X--;
                    for (int i = 0; i < g_iPlatforms; i++) {
                        if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X == g_aPlatformsX[i]) {
                            g_sChar.m_cLocation.X++;
                        }
                    }
                }
                if (GetKeyState(0x44) & 0x800) //check for D input and jump right;
                {
                    g_sChar.m_cLocation.X++;
                    for (int i = 0; i < g_iPlatforms; i++) {
                        if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X == g_aPlatformsX[i]) {
                            g_sChar.m_cLocation.X--;
                        }
                    }
                }
                if (g_sProj.m_cLocation.X == g_sChar.m_cLocation.X)
                {
                    g_sProj.m_cLocation.Y--;
                }
                g_jElapsedTime = 0;
                l++;
            }
        }
        for (int j = 0; j < g_iPlatforms; j++) {
            if (l == 4 && g_sUltimate == false && g_sChar.m_cLocation.Y+1 != g_aPlatformsY[j]) //check if l is 3 and push down for gravity
            {
                if (g_jElapsedTime > 0.05)
                {
                    if (l == 1)
                    {
                        Beep(1440, 30);
                    }
                    g_sChar.m_cLocation.Y++;
                    for (int i = 0; i < g_iPlatforms; i++) {
                        if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i])
                        {
                            g_sChar.m_cLocation.Y--;
                        }
                    }
                    if (GetKeyState(0x41) & 0x800) //check for A input and jump left
                    {
                        g_sChar.m_cLocation.X--;
                        for (int i = 0; i < g_iPlatforms; i++) {
                            if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X == g_aPlatformsX[i]) {
                                g_sChar.m_cLocation.X++;
                            }
                        }
                    }
                    if (GetKeyState(0x44) & 0x800) //check for D input and jump right
                    {
                        g_sChar.m_cLocation.X++;
                        for (int i = 0; i < g_iPlatforms; i++) {
                            if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X == g_aPlatformsX[i]) {
                                g_sChar.m_cLocation.X--;
                            }
                        }
                    }
                    if (g_sProj.m_cLocation.X == g_sChar.m_cLocation.X)
                    {
                        /*if (l == 1)
                        {
                            Beep(1440, 30);
                        }*/
                        g_sChar.m_cLocation.Y++;
                        for (int i = 0; i < g_iPlatforms; i++) {
                            if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X == g_aPlatformsX[i]) {
                                g_sChar.m_cLocation.Y--;
                            }
                        }
                        if (GetKeyState(0x41) & 0x800) //check for A input and jump left
                        {
                            g_sChar.m_cLocation.X--;
                            for (int i = 0; i < g_iPlatforms; i++) {
                                if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X == g_aPlatformsX[i])
                                {
                                    g_sChar.m_cLocation.X++;
                                }
                            }
                        }
                        if (GetKeyState(0x44) & 0x800) //check for D input and jump right
                        {
                            g_sChar.m_cLocation.X++;
                            for (int i = 0; i < g_iPlatforms; i++) {
                                if (g_sChar.m_cLocation.Y == g_aPlatformsY[i] && g_sChar.m_cLocation.X == g_aPlatformsX[i])
                                {
                                    g_sChar.m_cLocation.X--;
                                }
                            }
                        }
                        if (g_sProj.m_cLocation.X == g_sChar.m_cLocation.X)
                        {
                            g_sProj.m_cLocation.Y++;
                        }
                        g_jElapsedTime = 0;
                    }
                    g_jElapsedTime = 0;
                }
            }
        }
    if (g_skKeyEvent[K_41].keyDown && g_sChar.m_cLocation.X > 2 && g_sUltimate == false/*&& g_sProj.m_cLocation.X == g_sChar.m_cLocation.X)*/) //LEFT
    {
        /*Beep(1440, 30);*/
        oneStep = g_sChar.m_cLocation.X - 1;
        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 1;
        for (int i = 0; i< g_iPlatforms; i++) {
            if (oneStep == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 1;
            }
            else if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 1;
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
    if (g_skKeyEvent[K_44].keyDown && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 3 && g_sUltimate == false/*&& g_sProj.m_cLocation.X == g_sChar.m_cLocation.X*/) //RIGHT
    {
        /*Beep(1440, 30);*/
        oneStep = g_sChar.m_cLocation.X + 1;
        g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + n;
        for (int i = 0; i < g_iPlatforms; i++) {
            if (oneStep == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - n;
            }
            else if (g_sChar.m_cLocation.X == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
                g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - n;
            }
        }
    }
}

void sneakCharacter()
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

void moveProjectile ()
{
    double n = 0;
    if (g_sRage == true)
        n = 0.01;
    else
        n = 0.02;
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED && g_mouseEvent.mousePosition.X > g_sChar.m_cLocation.X && g_sProj.m_cLocation.X >= g_sChar.m_cLocation.X) //shoot to right
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

void dodge(int i)
{
    if (g_skKeyEvent[K_51].keyDown && g_sChar.m_cLocation.X > 2) //dodge left
    {
        g_sInvulnerable = true;
        dashl++;
    }
    if (g_doElapsedTime > 0.01)
    {
        if (dashl >= 1 && dashl < i && g_sChar.m_cLocation.X > 2)
        {
            g_sChar.m_cLocation.X = g_sChar.m_cLocation.X - 2;
            g_doElapsedTime = 0;
            dashl++;
        }  
    }
    if (g_slashdelay > 0.2 && dashl >= i)
    { 
        g_sInvulnerable = false;
        dashl = 0;
    }
    if (g_skKeyEvent[K_45].keyDown && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 3) //dodge right
    {
        g_sInvulnerable = true;
        dashr++;
    }
    if (g_doElapsedTime > 0.01)
    {
        if (dashr >= 1 && dashr <= i && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 3)
        {
            g_sChar.m_cLocation.X = g_sChar.m_cLocation.X + 2;
            g_doElapsedTime = 0;
            dashr++;
        }
    }
    if (g_slashdelay > 0.5 && dashr >= i)
    {
        g_sInvulnerable = false;
        dashr = 0;
        g_slashdelay = 0;
    }
}

void slashAttack(double n, int i, int j)
{
    g_sAttackState = false;
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED && g_mouseEvent.mousePosition.X > g_sChar.m_cLocation.X && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 3 && !g_sUltimate && !g_sFocus) //Slash right
    {
        g_sAttackState = true;
        if (g_sElapsedTime > n)
        {
            if (s != i)
            {
               
                /*if (s == 1)
                {
                    Beep(1440, 30);
                }*/
                g_sChar.m_cLocation.X++;
                g_sElapsedTime = 0;
                s++;
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
    }
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED && g_mouseEvent.mousePosition.X < g_sChar.m_cLocation.X && g_sChar.m_cLocation.X > 2 && !g_sUltimate && !g_sFocus) //Slash left
    {
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

void downslam()
{
    if (g_skKeyEvent[K_53].keyDown && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1 && p != 3 && !g_sUltimate) //DOWN slam attack with S
    {
        slam++;
        l = 0;
        for (int x = 0; x < g_iPlatforms; x++)
        {
            if (g_sChar.m_cLocation.X == g_aPlatformsX[x] && g_sChar.m_cLocation.Y == g_aPlatformsY[x])
            {
                g_sChar.m_cLocation.Y--;
            }
        }
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
            g_sElapsedTime = 0;
            slam++;
        }
        if (slam >= 3 && slam < 6 && g_sChar.m_cLocation.Y != g_Console.getConsoleSize().Y - 1)
        {
            g_sChar.m_cLocation.Y++;
            slam++;
            g_sElapsedTime = 0;
        }
        else if (slam == 6)
        {
            if (g_sChar.m_cLocation.Y != g_Console.getConsoleSize().Y - 1)
            {
                g_sChar.m_cLocation.Y++;
            }
            else
            {
                slam = 0;
                for (int i = 0; i < 5; i++)
                {
                    if (g_sChar.m_cLocation.X + 3 >= g_sEnemy[i].m_cLocation.X && g_sEnemy[i].m_cLocation.X > g_sChar.m_cLocation.X)
                    {
                        g_sEnemy[i].m_cLocation.X = g_sEnemy[i].m_cLocation.X + 3;
                        g_sEnemy[i].m_dHealth = g_sEnemy[i].m_dHealth - 2;
                        g_sBossP1.m_dHealth = g_sBossP1.m_dHealth - 2;
                        g_stun = -1;
                    }
                    if (g_sChar.m_cLocation.X - 3 <= g_sEnemy[i].m_cLocation.X && g_sEnemy[i].m_cLocation.X < g_sChar.m_cLocation.X)
                    {
                        g_sEnemy[i].m_cLocation.X = g_sEnemy[i].m_cLocation.X - 3;
                        g_sEnemy[i].m_dHealth = g_sEnemy[i].m_dHealth - 2;
                        g_sBossP1.m_dHealth = g_sBossP1.m_dHealth - 2;
                        g_stun = -1;
                    }
                }
                g_sAttackState = false;
            }
        }
    }
}

void seraphUlt()
{
    if (g_sUltimate == true)
    {
        g_sChar.m_dMana = 50;
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
            for (int i = 0; i <= 5; i++)
            {
                g_sEnemy[i].m_dHealth = 0;
            }
            g_sBossP1.m_dHealth = g_sBossP1.m_dHealth - 50;
        }
        else if (g_udElapsedTime > 3)
        {
            l = 1;
            g_sChar.m_dHealth = 50;
            g_sChar.m_dMana = 0;
        }
    }
}

void focusAttack()
{
    int move;
    if (g_sFocus == true)
    {
        if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED && (g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 3 || g_sChar.m_cLocation.X > 2))
        {
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

void focusUlt()
{
    if (g_sFocus == true)
    {
        if (g_skKeyEvent[K_SPACE].keyReleased)
        {
            su++;
        }

    }
}


void setdamage()
{
    for (int i = 0; i < 5; i++)
    {
        if (g_sEnemy[i].m_cLocation.X == g_sChar.m_cLocation.X && g_sEnemy[i].m_cLocation.Y == g_sChar.m_cLocation.Y || g_sEnemy[i].m_cLocation.X == g_sChar.m_cLocation.X + 1 && g_sEnemy[i].m_cLocation.Y == g_sChar.m_cLocation.Y)
        {
            if (g_hElapsedTime > 0.5)
            {
                if (g_sEnemy[i].m_dHealth > 0 && !g_sUltimate && !g_sRage && !g_sAttackState && !g_sInvulnerable && !g_sFocus)
                {
                    g_sChar.m_dHealth--;
                }
                g_hElapsedTime = 0;
                if (g_sAttackState == true)
                {
                    g_sEnemy[i].m_dHealth = g_sEnemy[i].m_dHealth - 1; //program recognises as 2 damage
                }
                if (g_sFocus == true)
                {
                    g_sEnemy[i].m_dHealth = g_sEnemy[i].m_dHealth - 5;
                }
            }
        }
    }
    if ((g_sBossP1.m_cLocation.X == g_sChar.m_cLocation.X || g_sBossP1.m_cLocation.X + 1 == g_sChar.m_cLocation.X || g_sBossP1.m_cLocation.X + 2 == g_sChar.m_cLocation.X) && g_sBossP1.m_cLocation.Y == g_sChar.m_cLocation.Y)
    {
        if (g_hElapsedTime > 0.2)
        {
            if (g_sBossP1.m_dHealth > 0 && !g_sUltimate && !g_sRage && !g_sAttackState && !g_sInvulnerable && !g_sFocus)
            {
                g_sChar.m_dHealth = g_sChar.m_dHealth - 2;
            }
            g_hElapsedTime = 0;
        }
        if (g_sAttackState == true)
        {
            g_sBossP1.m_dHealth = g_sBossP1.m_dHealth - 1; //program recognises as 2 damage
        }
        if (g_sFocus == true)
        {
            g_sBossP1.m_dHealth = g_sBossP1.m_dHealth - 10;
        }
    }
    for (int i = 0; i < 5; i++)
    {
        if (g_sChar.m_cLocation.X + 2 >= g_sEnemy[i].m_cLocation.X && g_sChar.m_cLocation.X < g_sEnemy[i].m_cLocation.X && g_sAttackState) //right slash attack
        {
            if (g_hElapsedTime > 0.2)
            {
                g_sEnemy[i].m_dHealth = g_sEnemy[i].m_dHealth - 1;
                g_hElapsedTime = 0;
            }
        }
        if (g_sChar.m_cLocation.X - 2 <= g_sEnemy[i].m_cLocation.X && g_sChar.m_cLocation.X > g_sEnemy[i].m_cLocation.X && g_sAttackState) //left slash attack
        {
            if (g_hElapsedTime > 0.2)
            {
                g_sEnemy[i].m_dHealth = g_sEnemy[i].m_dHealth - 1;
                g_hElapsedTime = 0;
            }
        }
    }
    //  Knockback slash for boss not active
    //if (g_sChar.m_cLocation.X - 2 <= g_sBossP1.m_cLocation.X && g_sChar.m_cLocation.X > g_sBossP1.m_cLocation.X && g_sAttackState.m_bActive) //left slash attack
    //{
    //    if (g_hElapsedTime > 0.2)
    //    {
    //        g_sBossP1.m_dHealth = g_sBossP1.m_dHealth - 1;
    //        g_hElapsedTime = 0;
    //    }
    //}
    //if (g_sChar.m_cLocation.X + 2 >= g_sBossP1.m_cLocation.X && g_sChar.m_cLocation.X < g_sBossP1.m_cLocation.X && g_sAttackState.m_bActive) //left slash attack
    //{
    //    if (g_hElapsedTime > 0.2)
    //    {
    //        g_sBossP1.m_dHealth = g_sBossP1.m_dHealth - 1;
    //        g_hElapsedTime = 0;
    //    }
    //}
    for (int i = 0; i < 5; i++)  //check projectile damage
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
        }
        else if (g_sProj.m_cLocation.X == g_sBossP1.m_cLocation.X && g_sProj.m_cLocation.Y == g_sBossP1.m_cLocation.Y && (g_sProj.m_cLocation.X > g_sChar.m_cLocation.X || g_sProj.m_cLocation.X < g_sChar.m_cLocation.X) && c6 != 0)
        {
            if (!g_sRage)
            {
                g_sBossP1.m_dHealth--;
            }
            else
            {
                g_sBossP1.m_dHealth = g_sBossP1.m_dHealth - 2;
            }
            j = 0;
            g_sProj.m_cLocation = g_sChar.m_cLocation;
        }
        if (g_sEnemy[i].m_dHealth <= 0)
        {
            c5[i] = 0;
            enemyColor[i] = 0x1A;
        }
    }
    if (g_sBossP1.m_dHealth <= 0)
    {
        c6 = 0;
        c7 = 0;
        boss1 = std::string(3, c6) + c7;
        c8 = 0;
        c9 = 0;
        boss2 = std::string(3, c8) + c9;
        bossColor = 0x1A;
    }
    if (g_sChar.m_dHealth <= 0)
    {
        g_sChar.m_dHealth = 0;
    }
}

void setUltimate(int M)
{
    if (g_sChar.m_dMana < M && !g_sUltimate && !g_sRage && !g_sFocus) //checks if Player mana is less than 50 and is not in rage mode
    {
        if (g_uElapsedTime > 0.5) //increase mana by 1 every 1 second
        {
            g_uElapsedTime = 0;
            g_sChar.m_dMana++;
        }
        /*if (g_sEnemy.m_dHealth == 0)
        {
            g_sChar.m_dMana++;
        }*/
    }
    else if (g_sUltimate || g_sRage || g_sFocus)
    {
        if (g_uElapsedTime > 0.2)
        {
            g_uElapsedTime = 0;
            g_sChar.m_dMana--;
        }
        if (g_sChar.m_dMana <= 0)
        {
            g_sChar.m_dMana = 0;
            g_sRage = !g_sRage;
            //g_sUltimate = !g_sUltimate;
            //g_sFocus = false;
        }
    }
    if (g_sChar.m_dMana == M && g_skKeyEvent[K_SPACE].keyReleased)
    {
        //g_sUltimate = !g_sUltimate;
        g_sRage = !g_sRage;
        //g_sFocus = !g_sFocus;
        //g_sChar.m_cLocation.X = x / 2 + 7;
        //g_sChar.m_cLocation.Y = y / 2;
        su = 0;
    }
}

void moveEnemy(int n, double t, int d)
{

    if (c5[0] != 0 && !g_sUltimate && !g_sFocus)
    {
        if (g_stun > 2)
        {
            if (g_eElapsedTime > t)
            {
                if (i < k)
                {
                    g_sEnemy[0].m_cLocation.X = g_sEnemy[0].m_cLocation.X + 1;
                    g_eElapsedTime = 0;
                    i++;
                }
                else
                {
                    g_sEnemy[0].m_cLocation.X = g_sEnemy[0].m_cLocation.X - 1;
                    g_eElapsedTime = 0;
                    if (g_sEnemy[0].m_cLocation.X <= d)
                    {
                        i = 0;
                    }
                }
            }
        }

        for (int k = 0; k < 5; k++)
        {
            if (g_mouseEvent.mousePosition.X > g_sChar.m_cLocation.X && g_sChar.m_cLocation.X + 2 >= g_sEnemy[k].m_cLocation.X && g_sChar.m_cLocation.X < g_sEnemy[k].m_cLocation.X && g_sAttackState == true) //push right
            {
                g_sEnemy[k].m_cLocation.X = g_sEnemy[k].m_cLocation.X + 1;
                i = i + 1;
                stun++;
            }
            else if (g_mouseEvent.mousePosition.X < g_sChar.m_cLocation.X && g_sChar.m_cLocation.X - 1 <= g_sEnemy[k].m_cLocation.X && g_sChar.m_cLocation.X > g_sEnemy[k].m_cLocation.X && g_sAttackState == true) //push left
            {
                g_sEnemy[k].m_cLocation.X = g_sEnemy[k].m_cLocation.X - 1;
                i = i - 1;
                stun++;
                if (g_sEnemy[k].m_cLocation.X - 1 == d)
                {
                    n++;
                }
                else if (g_sEnemy[k].m_cLocation.X - 2 == d)
                {
                    n = n + 2;
                }
            }
            else if (stun > 1)
            {
                g_stun = 0;
                stun = 0;
            }
            if (slam == 6 && g_sChar.m_cLocation.X + 2 == g_sEnemy[k].m_cLocation.X)
            {
                i = i + 3;

            }
            if (slam == 6 && g_sChar.m_cLocation.X - 2 == g_sEnemy[k].m_cLocation.X)
            {
                i = i - 3;
            }
        }
    }
}

void moveBoss(int n, double t, double t2, int d)
{
    if (g_sBossP1.m_dHealth > 0 && !g_sUltimate && !g_sFocus)
    {
        if (g_sBossP1.m_dHealth <= 25)
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
                    g_sBossP1.m_cLocation.X = g_sBossP1.m_cLocation.X + 2;
                    g_sBossP2.m_cLocation.X = g_sBossP2.m_cLocation.X + 2;
                    g_bElapsedTime = 0;
                    k++;
                    g_delay2 = 0;
                }
            }
            else
            {
                if (g_delay2 > t2) //go to left
                {
                    g_sBossP1.m_cLocation.X = g_sBossP1.m_cLocation.X - 2;
                    g_sBossP2.m_cLocation.X = g_sBossP2.m_cLocation.X - 2;
                    g_bElapsedTime = 0;
                    g_delay1 = 0;
                    if (g_sBossP1.m_cLocation.X == d)
                    {
                        k = 0;
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

void processUserInput() {
    // process inputs depending on game states
    if (g_skKeyEvent[K_ESCAPE].keyReleased) {
        switch (g_eGameState) {
        case S_GAME:
            g_eGameState = S_MENU;
            g_bPlayGame = false;
            break;
        case S_SPLASHSCREEN:
            g_bQuitGame = true;
            shutdown();
            break;
        case S_MENU:
            g_eGameState = S_GAME;
            g_bPlayGame = true;
            break;
        }
    }
    if (g_skKeyEvent[K_SPACE].keyReleased) {
        switch (g_eGameState) {
        case S_GAME:
            break;
        case S_SPLASHSCREEN:
            g_bPlayGame = true;
            break;
        case S_MENU:
            g_bQuitGame = true;
            shutdown();
            break;
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
        }
    }
    if (g_skKeyEvent[K_ENTER].keyReleased) {
        switch (g_eGameState) {
        case S_GAME:
            break;
        case S_SPLASHSCREEN:
            break;
        case S_MENU:
            saveGame();
            g_eGameState = S_GAME;
            break;
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
        }
    }
    if (g_skKeyEvent[K_RIGHT].keyReleased) {
        switch (g_eGameState) {
        case S_GAME:
            break;
        case S_SPLASHSCREEN:
            if (canDo == 0) {
                characterSelect++;
                if (characterSelect > 3)
                    characterSelect = 0;
                canDo = 1;
            }
            break;
        case S_MENU:
            break;
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
        renderSplashScreen();
        break;
    case S_GAME: 
        renderGame(); 
        renderFramerate();
        break;
    case S_MENU: 
        renderMenu();
        break;
    }
    // renderFramerate();      // renders debug information, frame rate, elapsed time, etc
    // renderInputEvents();    // renders status of input events
    renderToScreen();       // dump the contents of the buffer to the screen, one frame worth of game
}

void resetToStart() {
    g_sChar.m_cLocation.X = g_sCharSpawn.m_cLocation.X;
    g_sChar.m_cLocation.Y = g_sCharSpawn.m_cLocation.Y;
    if (characterSelect == 0) {
        g_sChar.m_dHealth = 2000;
        g_sChar.m_dMana = 100;
    }
    if (characterSelect == 1) {
        g_sChar.m_dHealth = 1000;
        g_sChar.m_dMana = 50;
    }
    if (characterSelect == 2) {
        g_sChar.m_dHealth = 2500;
        g_sChar.m_dMana = 25;
    }
    if (characterSelect == 3) {
        g_sChar.m_dHealth = 1500;
        g_sChar.m_dMana = 0;
    }
}

void deletePlatforms() {
    delete[] g_aPlatformsX;
    delete[] g_aPlatformsY;
}

void clearScreen() {
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x1F);
}

void renderToScreen() {
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

void renderSplashScreen() {
    // renders the splash screen
    COORD c = g_Console.getConsoleSize();
    string ch;
    c.Y /= 3;
    c.X = c.X / 2 - 9;
    g_Console.writeToBuffer(c, "Start Menu", 0x03);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 13;
    g_Console.writeToBuffer(c, "Press <Space> to play", 0x09);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 13;
    g_Console.writeToBuffer(c, "Press <Arrow Keys> to switch characters", 0x09);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 13;
    if (characterSelect == 0)
        ch = "Dwem Guy";
    if (characterSelect == 1)
        ch = "Seraph";
    if (characterSelect == 2)
        ch = "Gin";
    if (characterSelect == 3)
        ch = "Thorfinn";
    g_Console.writeToBuffer(c, ch, 0x09);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 13;
    g_Console.writeToBuffer(c, "Press <Esc> to quit", 0x09);
}

void renderGame() {
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
    renderHUD();
    // renderInputEvents();
}

void renderHUD() {
    const WORD colors[] = {
        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6, 0x00
    };
    COORD c;
    c.X = 0;
    c.Y = 1;
    colour(colors[1]);
    std::string health = std::to_string(g_sChar.m_dHealth);
    health = health.substr(0, 4);
    std::string healthDisplay = "Health = " + health;
    g_Console.writeToBuffer(c, healthDisplay, colors[0]);
    
    c.X = 0;
    c.Y = 2;
    colour(colors[1]);
    std::string mana = std::to_string(g_sChar.m_dMana);
    mana = mana.substr(0, 4);
    std::string manaDisplay = "Ultimate = " + mana;
    g_Console.writeToBuffer(c, manaDisplay, colors[0]);

    c.X = 0;
    c.Y = 3;
    colour(colors[1]);
    int elapsed = g_dElapsedTime;
    std::string elapsedTime = "Elapsed time = " + std::to_string(elapsed);
    g_Console.writeToBuffer(c, elapsedTime, colors[0]);
}

void renderMenu() {
    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - 10;
    g_Console.writeToBuffer(c, "Pause menu", 0x0F);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 13;
    g_Console.writeToBuffer(c, "Press <Space> to save and exit", 0x09);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 13;
    g_Console.writeToBuffer(c, "Press <Esc> continue", 0x09);
}

void renderSavedGame() {
    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - 10;
    c.Y--;
    g_Console.writeToBuffer(c, "Saved game", 0x0F);
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
                    g_Console.writeToBuffer(x, y, " ", BACKGROUND_RED);
                }
            }
            y++;
        }
    }
    map.close();
    if (oneTime == 0)
        loadLevelData(1);
    oneTime = 1;
    renderPlatform();
}

void renderCharacter() 
{
    // Draw the location of the character and weapon

    // Draw the location of the character and weapon
    WORD charColor = 0x2E; //non - ultimate mode color
    if (g_sRage == true)
    {
        charColor = 0x4E; //Ultimate mode
    }
    if (g_sProj.m_cLocation.X > g_sChar.m_cLocation.X || g_sProj.m_cLocation.X < g_sChar.m_cLocation.X)
    {
        g_Console.writeToBuffer(g_sProj.m_cLocation, c4, 0x1F);
    }
    if (g_sChar.m_dHealth > 0)
    {
        g_Console.writeToBuffer(g_sChar.m_cLocation, c3, charColor);
    }
    for (int i = 0; i < 5; i++)
    {
        g_Console.writeToBuffer(g_sEnemy[i].m_cLocation, c5[i], enemyColor[i]);
    }
    g_Console.writeToBuffer(g_sBossP1.m_cLocation, boss1, bossColor);
    g_Console.writeToBuffer(g_sBossP2.m_cLocation, boss2, bossColor);
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
        if (g_sChar.m_cLocation.X+1 == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
            hit = true;
        }
        if (g_sChar.m_cLocation.X-1 == g_aPlatformsX[i] && g_sChar.m_cLocation.Y == g_aPlatformsY[i]) {
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
    ss << 1.0 / g_dDeltaTime << "FPS";
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
    int n1 = 0;
    int y = 0;
    std::string levelFile;
    std::string line2;
    if (number == 0)
        levelFile = "tutorial.txt";
    if (number == 1)
        levelFile = "levelone.txt";
    if (number == 2)
        levelFile = "leveltwo.txt";
    if (number == 3)
        levelFile = "levelthree.txt";
    if (number == 4)
        levelFile = "levelfour.txt";
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
            }
            y++;
        }
    }
    levelPtr.close();
}

void LEMoveChar() {
    std::ostringstream ss;
    if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
        if (g_mouseEvent.mousePosition.X == g_sChar.m_cLocation.X && g_mouseEvent.mousePosition.Y == g_sChar.m_cLocation.Y) {
            g_sChar.m_cLocation.X = g_mouseEvent.mousePosition.X;
            g_sChar.m_cLocation.Y = g_mouseEvent.mousePosition.Y;
        }
        if (g_mouseEvent.mousePosition.X == (g_sChar.m_cLocation.X + 1) && g_mouseEvent.mousePosition.Y == g_sChar.m_cLocation.Y) {
            g_sChar.m_cLocation.X = g_mouseEvent.mousePosition.X - 1;
            g_sChar.m_cLocation.Y = g_mouseEvent.mousePosition.Y;
        }
    }
}