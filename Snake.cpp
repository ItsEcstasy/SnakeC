/* 
[+]=======================================================================[+]
 |                      Tragic Snake - Tragedy                             |
 |                                                                         |
 |    I Would Note That This Isn't 'Really' a C++ Program                  |
 |    I'm Still Using A Very C Style, Storing Data In A C Way              |
 |    And Calling C Standard Library Functions                             |
 |                                                                         |
 |    This Is A Base For Both Me And You To Build On, On Our Own Time      |
 |    Therefore I've Tried To Comment As Much As Possible In Real Time     |
 |                                                                         |
 |              This Can Be Run On Your Linux Machines                     |
 |         yum install gcc-c++; g++ Snake.cpp -o Snake; ./Snake            |
 |                                                                         |
[+]=======================================================================[+]
*/
#include <iostream> //Standard
#include <stdlib.h> //Standard
#include <unistd.h> //For POSIX Access
#include <sys/ioctl.h> //For Display Window, TTY Window (Console Window) 
#include <termios.h> //For Line Buffering - See Below
#include <stdio.h> //Old Friend

#define CLRSCR "\e[1;1H\e[2J" //Command To Clear Terminal Screen - Change Accordingly

using namespace std; /* 
                        A NameSpace Is Used As Additional Information 
                        To Differentiate Between Similar Functions/Variables
                        That Have The Same Name In Different Libraries
                        Using 'namespace' You Can Define The Context
                        In Which Names Are Defined

                        Without Using The STD NameSpace, The Computer Will Try
                        To Call 'cout' Or 'cin' As If It Weren't Defined In A NameSpace
                        Trying To Call Something That Doesn't Exist = Error
                        So, Without Using 'namespace std;' When You Write For Example:
                        'cout << value;' You'd Have To Write 'std::cout << value;''

                      */
void ClearScreen(void)
{
    cout << CLRSCR;
}
//Create Boundaries
const int width = 50;
const int height = 25;
const char block = '#';

//Global Arrays For Data Records
int background[height][width]; //Background
int snake[50][2];              //Max Snake Length
int snakelength = 3;           //Snake Starting Length
int snakespeedx = 1;           //Horizontal Speed
int snakespeedy = 1;           //Vertical Speed
int food[2] = {0,0};           // Snake Food
int score = 0;                 //Score
int frame = 0;                 //Ran Frames
int lap = 200;                 //Waiting Time Betweeen Frames

//Declaring Global Temporary Variables To Save Memory
char key;                    //Keyboard Press
int px, py, nx, ny;          //Current Postions + New Positions
int i;                       //Increment
int h, w;                    //Height And Width Values
int x, y;                    //For Inserting New Coordinates
int movementx = snakespeedx; //Snake Movement
int movementy = 0;           //Snake Movement

//Initialise Background Borders Onto Array
void MakeBackground(void)
{
    //int i;
    // Insert Top Border
    for(i = 0; i < width; i ++)
    {
        background[0][i] = 1;
    }
    //Insert Left Border
    for(i = 0; i < height; i ++)
    {
        background[i][0] = 1;
    }
    //Insert Right Border
    for(i = 0; i < height; i ++)
    {
        background[i][width - 1] = 1;
    }
    //Insert Bottom Border
    for(i = 0; i < width; i ++)
    {
        background[height - 1][i] = 1;
    }
}
//Initialise Snake Coordinates
void MakeSnake(void)
{
    snake[0][0]=3; //Coordinates X
    snake[0][1]=3; //Coordinates Y

    snake[1][0]=3+1; //Coordinates X
    snake[1][1]=3; //Coordinates Y

    snake[2][0]=3+2; //Coordinates X
    snake[2][1]=3; //Coordinates Y

    snake[3][0]=3+3; //Coordinates X
    snake[3][1]=3; //Coordinates Y

    snake[4][0]=3+4; //Coordinates X
    snake[4][1]=3; //Coordinates Y
}
//Print Array Frame
void PrintFrame(void)
{
    for(h = 0; h < height; h ++)
    {
        for(w = 0; w < width; w ++)
        {
            i = background[h][w];
            if(i == 1) //If There's A Free Space In Our Matrix
            {          //It Must Be Part Of The Snake Or Border 
                cout << block;
            }
            else if (i == 2) //Because It Wouldn't Be Food
            {
                cout << "o"; //Food
            }
            else //Or Playable Space
            {
                cout << " "; //Playable Space
            }
        }
        cout << endl;
    }
    cout << "    \x1b[32m[\x1b[1;36m+\x1b[32m] Frame: " << frame <<" \x1b[32m|\x1b[1;36mTragic Snake\x1b[32m| Score: " << score << " [\x1b[1;36m+\x1b[32m]" << endl;
}
//Install Snake Coordinates Into Background Array = ( 1 To Draw And 0 To Erase)
void DrawSnake(const int rev)
{
    //int x, y;
    for(i = 0; i < snakelength; i ++)
    {
        x = snake[i][0];
        y = snake[i][1];
        if((x != 0) && (y != 0))
        {
            background[y][x] = rev;
        }
    }
}
//Cross Platform Sleep Function For Frames
void Sleep(int milliseconds) 
{
    clock_t time_end;
    time_end = clock() + milliseconds * CLOCKS_PER_SEC/1000;
    while (clock() < time_end)
    {
        //Wait
    }
}
//Check For Keyboard Press 
/*
  Reference Link:
    https://www.quora.com/With-which-function-can-I-replace-kbhit-in-C++-because-the-header-conio-h-doesnt-exist-in-linux

  Ubuntu Users:
    sudo apt-get install libncurses5-dev libncursesw5-dev

  Life Saver:
    http://www.flipcode.com/archives/_kbhit_for_Linux.shtml
*/
int bytesWaiting;
int _kbhit()
{
    static const int STDIN = 0;
    static bool initialized = false; //The Boolean Data Type Is Used To     Declare A Variable Whose Value Will Be Set As True (1) Or False (0)

    if (! initialized)
    {
        //Use Termios To Turn Off Line Buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}
//Reaction To Keyboard Press
void KeyReact(const char k)
{
    if(key == 'd' || key == '6')
    {   //Right Turn
        movementx = snakespeedx;
        movementy = 0;
    }
    else if(key == 'a' || key == '4')
    {   //Left Turn
        movementx = -snakespeedx;
        movementy = 0;
    }
    else if(key == 'w' || key == '8')
    {   //Turn Up
        movementx = 0;
        movementy = -snakespeedy;
    }
    else if(key == 's' || key == '2')
    {   //Turn Down
        movementx = 0;
        movementy = snakespeedy;
    }
    else if(key == 'q' || key == 'z'|| key == 'c')
    {   //Quit
        cout << "         \x1b[32m[\x1b[31m-\x1b[32m]          Game Quit       \x1b[32m[\x1b[31m-\x1b[32m]\x1b[0m" << endl;
        cout << "         \x1b[32m[\x1b[1;36m+\x1b[32m]    Thanks For Playing!  \x1b[32m[\x1b[1;36m+\x1b[32m]\x1b[0m" << endl;
        exit(0);
    }
}
//Update Snake
void UpdateSnake(void)
{   //int px, py, nx, ny;
    px = snake[0][0];
    py = snake[0][1];
    snake[0][0] = px + movementx;
    snake[0][1] = py + movementy;
    nx = snake[0][0];
    ny = snake[0][1];

    for(i = 1; i < snakelength; i ++)
    {
        nx = snake[i][0];
        ny = snake[i][1];
        snake[i][0] = px;
        snake[i][1] = py;
        px = nx;
        py = ny;
    }
}
//Create Snake Food
void CookFood(void)
{
    if (food[0] == 0)
    {
        x = rand() % (width - 10) + 1;
        y = rand() % (height - 10) + 1;
        food[0] = x;
        food[1] = y;
        background[y][x] = 2;
    }
}
//Check Snake & Food Status
void CaptureFood(void)
{
    x = food[0];
    y = food[1];

    if ((x == snake[0][0]) && (y == snake[0][1]))
    {
        background[y][x] = 0;
        food[0] = 0;
        score ++;
        snakelength ++;
        CookFood();
    }
}
//Check Snake is Not Touching Boundary
void CheckBoundary(void)
{   //int px, py;
    px = snake[0][0];
    py = snake[0][1];

    if((px == 0) || (px == (width - 1)) || (py == 0) || (py == (height - 1))) //If Any Part Of Snake = Part Of Boundary
    {
        cout << "         \x1b[32m[\x1b[31m-\x1b[32m]      You Hit A Wall!    \x1b[32m[\x1b[31m-\x1b[32m]" << endl;
        cout << "         \x1b[32m[\x1b[1;36m+\x1b[32m]    Thanks For Playing!  \x1b[32m[\x1b[1;36m+\x1b[32m]\x1b[0m" << endl;
        exit(0);
    }
}
//[+]=====================Visuals=====================[+]
void TitleScreen(void)
{
    ClearScreen();
    cout << "\x1b[32m _________________________________________________" << endl;
    cout << "\x1b[32m[\x1b[1;36m+\x1b[32m]_____________________________________________\x1b[1;36m[+\x1b[32m]" << endl;
    cout << "\x1b[32m| |  \x1b[31;36m_______                  __                \x1b[32m| |" << endl;
    cout << "\x1b[32m| | \x1b[1;36m|       :----.---.-.-----|__:----.          \x1b[32m| |" << endl;
    cout << "\x1b[32m| | \x1b[1;36m|.|   | |   _|  _  |  _  |  |  __|          \x1b[32m| |" << endl;
    cout << "\x1b[32m| | \x1b[1;36m`-|.  |-|__| |___._|___  |__|____|          \x1b[32m| |" << endl;
    cout << "\x1b[32m| |   \x1b[1;36m|:  |            |_____|                  \x1b[32m| |" << endl;
    cout << "\x1b[32m| |   \x1b[1;36m|::.|  _______             __             \x1b[32m| |" << endl;
    cout << "\x1b[32m| |   \x1b[1;36m`---' |   _   :-----.---.-|  |--.-----.   \x1b[32m| |" << endl;
    cout << "\x1b[32m| |         \x1b[1;36m|   1___|     |  _  |    <|  -__|   \x1b[32m| |" << endl;
    cout << "\x1b[32m| |         \x1b[1;36m|____   |__|__|___._|__|__|_____|   \x1b[32m| |" << endl;
    cout << "\x1b[32m| |         \x1b[1;36m|:  1   |                           \x1b[32m| |" << endl;
    cout << "\x1b[32m| |         \x1b[1;36m|::.. . |                           \x1b[32m| |" << endl;
    cout << "\x1b[32m| |         \x1b[1;36m`-------'                           \x1b[32m| |" << endl;
    cout << "\x1b[32m| |_____________________________________________\x1b[32m| |" << endl;
    cout << "\x1b[32m[\x1b[1;36m+\x1b[32m]_____________________________________________[\x1b[1;36m+\x1b[32m]" << endl;
}
void PlayPrompt(void)
{
    cout << "        \x1b[32m[\x1b[1;36m+\x1b[32m] \x1b[1;36mWould You Like To Play? Y/N:";
}
void LoadingScreen(void)
{
    ClearScreen();
    cout << "\x1b[32m[\x1b[1;36mLoading...\x1b[32m]" << endl;
}
void NotPlaying(void)
{
    ClearScreen();
    cout << "\x1b[31mFine Then... \x1b[0m" << endl;
}
//Frame Loop
void FrameLoop(void)
{
    ClearScreen();
    DrawSnake(1); //Draw Snake
    PrintFrame(); //Print Frame
    DrawSnake(0); //Erase Snake
}
//[+]==================================================[+]
//Main Game Loop
void GameLoop(void)
{
    x = 0;
    y = 0;

    while(x < 500)
    {
        Sleep(lap);
        if(_kbhit()) //If Keyboard Pressed
        {
            cin >> key; //Key That Was Pressed
            KeyReact(key); //React To Key
        }
        FrameLoop();       //Run Main Loop Function
        UpdateSnake();     //Update Snake Coordinates
        CheckBoundary();   //Check Snake Status
        CookFood();        //Make Sure Food is Available
        CaptureFood();     //Snake Eaten Food?
        frame ++;
    }
}
//[+]==================================================[+]
//Main Trigger Function
main()
{
    char StartGame;
    TitleScreen();
    PlayPrompt();
    scanf(" %c", &StartGame);
    if (StartGame=='Y'||StartGame =='y')
    {
        cout << CLRSCR;
        LoadingScreen();  
        sleep(2);
    }
    else 
    {  
        cout << CLRSCR;
        NotPlaying();
        exit(0);
    }
    MakeBackground(); //Install All Variables
    MakeSnake();      //Install Snake data
    GameLoop();       //Run Update Loop
}
/*[+]=================================================[+]
  [ ]         Thanks For Playing!- Tragedy            [ ]
  [+]=================================================[+]*/