//Max O'Ganian and Nicole Futoryansky

#include "FEHLCD.h"
#include <string.h>
#include <math.h>
#include <vector>

//define constants
#define SCREEN_WIDTH 319
#define SCREEN_HEIGHT 239

//enum for the type of the ball
enum BallType {Blue, Red, Cue, Eight };

class Ball {
    private:
        float posX, posY, velX, velY;
        int radius = 5;
        BallType type;
    public:
        Ball(float px, float py, float vx, float vy, BallType t);
        void render(); //

        //getters and setters
        float getPosX();
        void setPosX(float px);
        float getPosY();
        void setPosY(float py);
        float getVelX();
        void setVelX(float vx);
        float getVelY();
        void setVelY(float vy);
        int getRadius();

        //updates position
        void update();


};

class Board {
    private:
        std::vector<Ball> balls;
    public:
        Board();
        void render();

        //checks for collisions with the walls
        void checkWalls();
};

//global vars
char state = 'm';       

//function decs
void drawMenu(), retMenu(), playing(Ball&, Ball&);
float dist(float, float, float, float);

int main() {
    //local vars to main
    bool running = true;

    Ball ball1 = Ball(100, 100, 1, 1, Red);
    Ball ball2 = Ball(200, 300, -2, -1, Blue);

    while (running) {
        if(state == 'm'){
            drawMenu();
        }
        else if(state == 'p'){
            //placeholder
            // LCD.WriteAt("PLAYING", 100, 20);
            playing(ball1, ball2);
        }
        else if(state == 't'){
            //placeholder
            LCD.WriteAt("PLAYING TIMED", 100 ,20);
        }
        else if(state == 'i'){
            //placeholder
            LCD.WriteAt("HERES HOW TO PLAY THE GAME", 20, 20);
        }
        else if(state == 'e'){
            running = false;
        }
        
        //for all exept the menu, be able to return to the menu
        if (state != 'm'){
            retMenu();
        }

        //only update lcd in one place
        LCD.Update();
    }
    return 0;
}

void drawMenu(){
    //set variables to keep everything in here tidy
    int wordX = 60, wordL = 195, wordH = 15, playY = 60, timeY = 90, instY = 120, exitY = 150;
    float x, y;

    //draw the options to the screen
    LCD.Clear(BLACK);
    LCD.WriteAt("Choose an Option", wordX, 30);
    LCD.WriteAt("------PLAY------", wordX, playY);
    LCD.WriteAt("---PLAY TIMED---", wordX, timeY);
    LCD.WriteAt("--INSTRUCTIONS--", wordX, instY);
    LCD.WriteAt("------EXIT------", wordX, exitY);

    //wait for the screen to be touched
    while(!LCD.Touch(&x, &y)){}

    //first check if the click is within the x range of the options
    if(x > wordX && x < (wordX  + wordL)){
        //Then check if the touch is within any of the buttons
        if(y > playY && y < (playY + wordH)){
            //if the touch is within, change state to the appropriate var
            state = 'p';
        }
        else if (y > timeY && y < (timeY + wordH)){
            state = 't';
        }
        else if (y > instY && y < (instY + wordH)){
            state = 'i';
        }
        else if (y > exitY && y < (exitY + wordH)){
            state = 'e';
        }
    }

    //clear the screen
    LCD.Clear(BLACK);

    //dont continue until the mouse is released, to avoid issues with next steps
    while(LCD.Touch(&x,&y)){}
}

void retMenu(){
    //variables to track mouse and to keep positions straight
    int rectX = 2, rectY = 2, rectW = 16, rectH = 19;
    float x, y;
    
    //draw the return button in the top left
    LCD.DrawRectangle(rectX,rectY,rectW,rectH);
    LCD.WriteAt("M", rectX+1,rectY+2);

    //get the postion of the touch, use if pressed
    if(LCD.Touch(&x, &y)){
        //if the mouse is clicked in the rectangle, go to the menu
        if(x > rectX && x < (rectX + rectW) && y > rectY && y < (rectY + rectH)){
            state = 'm';
        }
    }
}


void playing(Ball& b1, Ball& b2) {   

    // float hyp = dist(b1.getPosX(), b1.getPosY(), b2.getPosX(), b2.getPosY());
    
    // if (hyp < b1.getRadius()*2){
    //     float tX = b2.getVelX();
    //     float tY = b2.getVelY();

    //     b2.setVelX(b1.getVelX());
    //     b2.setVelY(b1.getVelY());

    //     b1.setVelX(tX);
    //     b1.setVelY(tY);
    // }

    // b1.update();
    // b2.update();

    // LCD.Clear(BLACK);
    // LCD.DrawCircle((int) b1.getPosX(), (int) b1.getPosY(), (int) b1.getRadius());
    // LCD.DrawCircle((int) b2.getPosX(), (int) b2.getPosY(), (int) b2.getRadius());
    

    Board board = Board();
    board.render();
   
    
}


float dist(float x, float y, float x1, float y1){
    //calculate the magnitude of the diatance between two points
    float hyp = sqrt(pow(abs(x1-x), 2.0) + pow(abs(y1-y), 2.0));
    return hyp;
}

Ball::Ball(float px, float py, float vx, float vy, BallType t) {
    posX = px;
    posY = py;
    velX = vx;
    velY = vy;
    type = t;
}

void Ball::update() {
    posX+=velX;
    posY+=velY;
}

//The ball draws itself
void Ball::render() {
    //determine which color the ball should be drawn in
    switch (type) {
        case Red:
            LCD.SetFontColor(MAROON);
            break;
        case Blue:
            LCD.SetFontColor(DARKBLUE);
            break;
        case Cue:
            LCD.SetFontColor(WHITE);
            break;
        case Eight:
            LCD.SetFontColor(BLACK);
            break;
    }
    LCD.FillCircle((int) posX, (int) posY, (int) radius);

    LCD.SetFontColor(WHITE); //set font color back to white
}

float Ball::getPosX() {
    return posX;
}

void Ball::setPosX(float px) {
    posX = px;
} 

float Ball::getPosY() {
    return posY;
}

void Ball::setPosY(float py) {
    posY = py;
} 

float Ball::getVelX() {
    return velX;
}

void Ball::setVelX(float vx) {
    velX = vx;
} 

float Ball::getVelY() {
    return velY;
}

void Ball::setVelY(float vy) {
    velY = vy;
} 

int Ball::getRadius() {
    return radius;
}

//initalize
Board::Board() {
    Ball rb1 = Ball(200, 80, 0, 0, Red);
    balls.push_back(rb1);
    Ball rb2 = Ball(200, 95, 0, 0, Red);
    balls.push_back(rb2);
    Ball rb3 = Ball(200, 110, 0, 0, Red);
    balls.push_back(rb3);
    Ball rb4 = Ball(200, 125, 0, 0, Red);
    balls.push_back(rb4);
    Ball rb5 = Ball(200, 140, 0, 0, Red);
    balls.push_back(rb5);
    Ball rb6 = Ball(200, 165, 0, 0, Red);
    balls.push_back(rb6);
    Ball rb7 = Ball(200, 140, 0, 0, Red);
    balls.push_back(rb7);
    Ball bb1 = Ball(250, 80, 0, 0, Blue);
    balls.push_back(bb1);
    Ball bb2 = Ball(250, 95, 0, 0, Blue);
    balls.push_back(bb2);
    Ball bb3 = Ball(250, 110, 0, 0, Blue);
    balls.push_back(bb3);
    Ball bb4 = Ball(250, 125, 0, 0, Blue);
    balls.push_back(bb4);
    Ball bb5 = Ball(250, 140, 0, 0, Blue);
    balls.push_back(bb5);
    Ball bb6 = Ball(250, 165, 0, 0, Blue);
    balls.push_back(bb6);
    Ball bb7 = Ball(250, 140, 0, 0, Blue);
    balls.push_back(bb7);
    Ball cueBall = Ball(150, 120, 0, 0, Cue);
    balls.push_back(cueBall);
    Ball eightBall = Ball(150, 150, 0, 0, Eight);
    balls.push_back(eightBall);
}

//Draws the board and all of the balls on it
void Board::render() {
    LCD.SetFontColor(SADDLEBROWN); //the brown border
    LCD.FillRectangle(7, 72, 306, 156);
    LCD.SetFontColor(DARKGREEN); //the green table
    LCD.FillRectangle(11, 76, 298, 148);
    LCD.SetFontColor(WHITE); //the player name labels
    LCD.WriteAt("Player 1", 5, 5);
    LCD.WriteAt("Player 2", 190, 5);
    LCD.DrawRectangle(109, 4, 17, 17); //the white boxes around the player colors
    LCD.DrawRectangle(294, 4, 17, 17);
    LCD.SetFontColor(DARKBLUE); //the blue color box
    LCD.FillRectangle(110, 5, 15, 15);
    LCD.SetFontColor(MAROON); //the red color box
    LCD.FillRectangle(295, 5, 15, 15);
    //Draw pockets
    LCD.SetFontColor(BLACK);
    LCD.FillCircle(9, 74, 8);
    LCD.FillCircle(9, 225, 8);
    LCD.FillCircle(310, 74, 8);
    LCD.FillCircle(310, 225, 8);
    LCD.FillCircle(160, 74, 8);
    LCD.FillCircle(160, 225, 8);

    LCD.SetFontColor(WHITE); //set font color back to white

    //render each ball
    for (int i = 0; i < balls.size(); i++) {
       balls.at(i).render();
    }
}

void Board::checkWalls() {
}
