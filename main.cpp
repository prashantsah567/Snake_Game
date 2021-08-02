#include <cstdlib> //general utility library for random number generator, dynamic memory management, integer arithmetics, searching, sorting etc..,
#include <curses.h> // input and output, also for many other useful functions like initscr(),noecho(), etc..,
#include <unistd.h> // sleep function
using namespace std;

class Game
{
public:
    int score;
    bool gameOver,autoMode;
    enum direction {STOP=0, LEFT, RIGHT, UP, DOWN} dir;
    Game();
    void checkInput();
};
Game::Game()
{
    score = 0;
    dir = STOP;
    gameOver = false;
    autoMode = false;
}
void Game::checkInput()
{
    keypad(stdscr, TRUE); // Check whether there is input, function from ncurses
    halfdelay(1); //waits one tenths of a second for a user to enter key- basically controlling the speed of the snake

    switch(getch())
    {
    case KEY_LEFT:
        dir = LEFT;
        break;
    case KEY_RIGHT:
        dir = RIGHT;
        break;
    case KEY_DOWN:
        dir = UP;
        break;
    case KEY_UP:
        dir = DOWN;
        break;
    case 'a':
        autoMode = true;
        break;
    }
}

class Snake : public Game
{
public:
    int headX, headY;
    int width = 70,height=20;
    int fruitX, fruitY;
    int tailX[100], tailY[100];
    int numTail=0;
    Snake();
    void print();
    void tail();
    void direction();
    void die();
    void fruit();
    void autoPilot();
};
Snake::Snake()
{
    headX = width/2; // put the snake in the middle
    headY = height/2;
    fruitX = (rand() % width)+1; //generating random number with modulus of width so it won't generate outside the box
    fruitY = (rand() % height)+1; //same as the width
}

void Snake::print()
{
    clear();
    //draw everything
    for (int i=0; i<height+2; i++)
    {
        for (int j=0; j<width+2; j++)
        {
            //Draw map of 22 rows and 72 columns
            if (i==0 || i==21)
            {
                mvprintw(i, j, "#");
            }
            else if (j==0 || j==71)
            {
                mvprintw(i, j, "#");
            }
            //Draw snake head which would be in the center of the box
            else if (i==headY && j==headX)
            {
                mvprintw(i, j, "o");
            }
            //Draw fruit which would be randomly generated
            else if (i==fruitY && j==fruitX)
            {
                mvprintw(i, j, "$");
            }
            else
            {   //Draw snake body
                for (int k=0; k<numTail; k++)
                {
                    if (i==tailY[k] && j==tailX[k])
                    {
                        mvprintw(i,j, "o");
                    }
                }
            }
        }
    }
    //printing some information using mvprintw so that we can put information at certain co-oridnates
    mvprintw(1, 61, "Score= %d", score);
    mvprintw(23, 0, "Snake Game: 1.Use direction keys to eat fruit.");
    mvprintw(24, 0, "            2.Press A enter the autopilot mode.");
    mvprintw(25, 0, "fruitX = %d", fruitX);
    mvprintw(25, 12, "fruitY = %d", fruitY);
    mvprintw(26, 0, "headX= %d", headX);
    mvprintw(26, 12, "headY= %d", headY);
    refresh();
}
void Snake::tail()
{
    int firstX, firstY,secondX,secondY;
    firstX = tailX[0];
    firstY = tailY[0];
    tailX[0] = headX;
    tailY[0] = headY;
    for (int i=1; i<numTail; i++)
    {
        secondX = tailX[i];
        secondY = tailY[i];
        tailX[i] = firstX;
        tailY[i] = firstY;
        firstX = secondX;
        firstY = secondY;
    }
}
void Snake::direction()
{
    switch (dir)
    {
    case LEFT:
        headX--;
        break;
    case RIGHT:
        headX++;
        break;
    case UP:
        headY++;
        break;
    case DOWN:
        headY--;
        break;
    default:
        break;
    }
}
void Snake::die()
{
    //this will tell if the snake eat them self
    for (int i=0; i<numTail; i++)
    {
        if(tailX[i]==headX && tailY[i]==headY)
        {
            gameOver=true;
        }
    }
    // if snake goes outside from the map, game will be over
    if (headX>width || headX<1 || headY<1 || headY>height)
    {
        gameOver=true;
    }
}

void Snake::fruit()//add one more fruit if snake eat the fruit
{
    if (headX==fruitX && headY==fruitY)
    {
        fruitX = (rand() % width)+1;
        fruitY = (rand() % height)+1;
        numTail++;
        score ++;
    }
}
//let snake eat fruit automatically
void Snake::autoPilot()
{
    if(fruitY < headY)
    {
        dir=DOWN;
    }
    if(fruitY > headY)
    {
        dir=UP;
    }
    if(fruitY == headY)
    {
        if(fruitX > headX)
        {
            dir=RIGHT;
        }
        if(fruitX < headX)
        {
            dir=LEFT;
        }
    }
}


int main()
{

    Game game;//initialize
    Snake snake;
    initscr();//begin to use curses
    curs_set(0); //use to hide the cursor

    while(!snake.gameOver)
    {
        snake.print();
        snake.checkInput();
        if(snake.autoMode)
        {
            snake.autoPilot();
        }
        snake.tail();
        snake.direction();
        snake.fruit();
        snake.die();
    }

    sleep(2); //suspend the execution of the program for 2 sec
    endwin(); //to exit the program from ncurses library- basically it cleans up all allocated resources from ncureses
    return 0;
}


