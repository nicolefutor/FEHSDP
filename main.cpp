//Max O'Ganian and Nicole Futoryansky

#include "FEHLCD.h"
#include <string.h>
#include <math.h>
#include <vector>

//define constants
#define SCREEN_WIDTH 319
#define SCREEN_HEIGHT 239

class Ball {
    private:
        float posX, posY, velX, velY;
        int radius = 10;
    public:
        Ball(float px, float py, float vx, float vy);
        void render();
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
};

//global vars
char state = 'm';       

//function decs
void drawMenu(), retMenu(), playing(Ball&, Ball&, bool*);
float dist(float, float, float, float);

int main() {
    //local vars to main
    bool running = true;
    bool hap = false;
    
    //intit all ball objects
    Ball ball1 = Ball(190, 100, 0, 1.0);
    Ball ball2 = Ball(200, 200, 0, -.20);

    while (running) {
        if(state == 'm'){
            drawMenu();
        }
        else if(state == 'p'){
            //placeholder
            LCD.WriteAt("PLAYING", 100, 20);
            playing(ball1, ball2, &hap);
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


void playing(Ball& b1, Ball& b2, bool *hap) {   

    float hyp = dist(b1.getPosX(), b1.getPosY(), b2.getPosX(), b2.getPosY()), x, y;
    //printf("%i\n", *hap);
    
    //LCD.Touch(&x, &y);

    if (hyp < b1.getRadius()*2 && *hap == false){
        *hap = true;
        printf("INSIDE\n");
        float V1x, V2x, V1y, V2y;
        
        float mag1 = sqrt(pow(b1.getVelX(), 2.0) + pow(b1.getVelY(), 2.0));
        float mag2 = sqrt(pow(b2.getVelX(), 2.0) + pow(b2.getVelY(), 2.0));
        
        float deltX = abs(b1.getPosX() - b2.getPosX());
        float deltY = abs(b1.getPosY() - b2.getPosY());

        //float perX = acos(deltX/hyp);

        float perY = asin(deltY/hyp);
        float perX = M_PI_2 - perY;

        float scalX = perX*(1.0/M_PI_2);
        float scalY = perY*(1.0/M_PI_2);
        
        //float perX = deltX/b1.getRadius();

        V1x = -1*perX*mag2;
        V2x = 1*perX*mag1;

        V1y = -1*perY*mag2;
        V2y = 1*perY*mag1;

        b1.setVelX(V1x);
        b2.setVelX(V2x);

        b1.setVelY(V1y);
        b2.setVelY(V2y);
    }
    
    //printf("xPer = %f\n", perX);
    //printf("yPer = %f\n", scalY);

    //b1.setPosX(x);
    //b1.setPosY(y);

    b1.update();
    b2.update();

    LCD.Clear(BLACK);
    b1.render();
    b2.render();
}


float dist(float x, float y, float x1, float y1){
    //calculate the magnitude of the diatance between two points
    float hyp = sqrt(pow(abs(x1-x), 2.0) + pow(abs(y1-y), 2.0));
    return hyp;
}

Ball::Ball(float px, float py, float vx, float vy) {
    posX = px;
    posY = py;
    velX = vx;
    velY = vy;
}

void Ball::update() {
    posX+=velX;
    posY+=velY;
}

void Ball::render() {
    LCD.DrawCircle((int) posX, (int) posY, (int) radius);
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


Board::Board() {

}

void Board::render() {
    for (int i = 0; i < balls.size(); i++) {
       balls.at(i).render();
    }
}
