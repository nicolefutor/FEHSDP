//Max O'Ganian and Nicole Futoryansky
//scractches and the 8 ball
//correct color bassed off start
//win
//timed - showing clock
//credit screen


#include "FEHLCD.h"
#include <string.h>
#include <string>
#include <math.h>
#include <vector>
#include <iostream>

//define constants
#define SCREEN_WIDTH 319
#define SCREEN_HEIGHT 239

#define FRIC .008
//#define FRIC 0


//enum for the type of the ball
enum BallType {Blue, Red, Cue, Eight};

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
        BallType getBallType();
        //updates position
        void update();
};

class Pocket {
    public:
        Pocket(float xI, float yI);
        float x, y, r;
        void render();
};

class Player{
    public:
        bool hasScored;
        int score;
        BallType col;
        Player();

};

class Board {

    public:
        std::vector<Ball> balls;
        std::vector<Ball*> colBalls;
        std::vector<Pocket> pockets;
        std::vector<Player> players;

        Board();
        void render();

        //checks for collisions with the walls
        void checkWalls();

        void collisions();
        void update();

        void scoring();

        //returns true if there are any balls still moving
        bool checkMovingBalls();
};

//global vars
char state = 'm';       
bool turn = true; //start true because will be automattically switched
bool isFirst = true;

//bool play1 = true, hasScored = false, isFirst = true;
//BallType play1Col = Cue;

//function decs
void drawMenu(), retMenu(), playing(Board&), reset(Board&), drawPlayerMenus(Player, Player);
float dist(float, float, float, float);

void takeInput(Board&, Ball&);

bool running = true;

int main() {
    //local vars to main
    bool hap = false;

    Board board = Board();

    Player p1 = Player();
    Player p2 = Player();

    while (running) {
        if(state == 'm'){
            drawMenu();
            reset(board);
            turn = true;
            isFirst = true;
        }
        else if(state == 'p'){
            //placeholder
            // LCD.WriteAt("PLAYING", 100, 20);
            playing(board);
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

  
void playing(Board& board) {      
    board.collisions();
    board.checkWalls();
    board.scoring();
    
    if (!board.checkMovingBalls()) {
        
        printf("STATIONARYYYYYYY\n");
        printf("hasScored = %i\n", board.players.at(turn).hasScored);

        if(!board.players.at(turn).hasScored){
            turn = !turn;
        }
        printf("turn = %i\n", turn);
        
        board.players.at(turn).hasScored = false;
        
        drawPlayerMenus(board.players.at(0), board.players.at(1));//update the plyer menu with whose turn it is

        //find which ball is the cue ball
        int cueId = -1;
        for (int i = 0; i < board.balls.size(); i++){
            if(board.balls.at(i).getBallType() == Cue){
                cueId = i;
            }
        }
        //get user input
        takeInput(board, board.balls.at(cueId));
    }

    board.update();
    board.render();
    
}

void drawPlayerMenus(Player p1, Player p2) {
    LCD.SetFontColor(WHITE); //the player name labels
    if(!turn){
        LCD.SetFontColor(GREEN);
    }
    LCD.WriteAt("Player 1", 25, 5);
    LCD.SetFontColor(WHITE);
    if(turn){
        LCD.SetFontColor(GREEN);
    }
    LCD.WriteAt("Player 2", 190, 5);
    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(129, 4, 18, 18); //the white boxes around the player colors
    LCD.DrawRectangle(294, 4, 18, 18);
    
    if(p1.col == Blue){
        LCD.SetFontColor(DARKBLUE); //the player color box
    }else if(p1.col == Red){
        LCD.SetFontColor(MAROON);
    }else{
        LCD.SetFontColor(BLACK);
    }
    LCD.FillRectangle(130, 5, 16, 16);
    
    if(p1.col == Blue){
        LCD.SetFontColor(MAROON); //the player 2 color box
    }else if(p1.col == Red){
        LCD.SetFontColor(DARKBLUE);
    }else{
        LCD.SetFontColor(BLACK);
    }
    LCD.FillRectangle(295, 5, 16, 16);
    LCD.SetFontColor(WHITE);
    
    //display the score of each player
    //first convert score to string
    std::string p1S = std::to_string(p1.score);
    std::string p2S = std::to_string(p2.score);
    
    LCD.WriteAt(p1S, 131, 6);
    LCD.WriteAt(p2S, 296, 6);
}

void reset(Board& board){
    board = Board();
    
    board.render();
    drawPlayerMenus(board.players.at(0), board.players.at(1));
}

Player::Player(){
    hasScored = false;
    col = Cue;
    score = 0;
}

Pocket::Pocket(float xI, float yI){
    x = xI;
    y = yI;
    r = 7;
}

void Pocket::render(){
    LCD.SetFontColor(BLACK);
    LCD.FillCircle(x,y,r);
}

void Board::scoring(){
    //handles collisions with the pockets, returns whether or not someone has scored their color
    for(int i = 0; i < pockets.size(); i++){
        for(int j = 0; j < balls.size(); j++){
            //these to keep things tidy
            Ball* b = &(balls.at(j));
            Pocket* p = &(pockets.at(i));
            Player* play = &(players.at(turn));

            float hyp = dist(b->getPosX(), b->getPosY(), p->x, p->y);
            
            if(hyp < (b->getRadius() + p->r)){
                if(b->getBallType() != Cue && (play->col == b->getBallType() || isFirst)){ //if the player scores a ball, other than the cue, and it is the correct color, they score
                    play->hasScored = true;
                    play->score++;
                    //printf("---------------------hasScored = %i ----------------\n", play->hasScored);
                }
                printf("DELETEEEE--------------\n");
                balls.erase(balls.begin()+j);
            }
            
            //if first score set the color for the player
            if(play->hasScored && isFirst){
                play->col = b->getBallType();
                
                if(b->getBallType() == Red){
                    players.at(!turn).col = Blue;
                }else{
                    players.at(!turn).col = Red;
                }
                isFirst = false;
            }
        }
    }

}
void Board::collisions(){
    //This function handles all the collisions between balls
    colBalls.clear();

    /*This moves balls out from inside each other, so they are no longer collided, also adds any balls that have collided into an array
        Doing so allows for simaltaneus collisions
    */
    for(int i = 0; i < balls.size(); i++){
        for(int j = 0; j < balls.size(); j++){
            if(i != j){
                //these are to save typing
                Ball* b1 = &balls.at(i);
                Ball* b2 = &balls.at(j);
                
                //dist between balls
                float hyp = dist(b1->getPosX(), b1->getPosY(), b2->getPosX(), b2->getPosY());
                
                if (hyp < b1->getRadius()*2){
                    colBalls.push_back(b1);
                    colBalls.push_back(&(balls.at(j)));

                    //find by how much the balls overlap
                    float overlap = 0.5 * (hyp - b1->getRadius());
                    
                    //move ball 1 away from collision
				    b1->setPosX(b1->getPosX() + (overlap * (b1->getPosX() - b2->getPosX()) / hyp));
					b1->setPosY(b1->getPosY() + (overlap * (b1->getPosY() - b2->getPosY()) / hyp));

					//move ball 2 away from collision
					b2->setPosX(b2->getPosX() - (overlap * (b1->getPosX() - b2->getPosX()) / hyp));
                    b2->setPosY(b2->getPosY() - (overlap * (b1->getPosY() - b2->getPosY()) / hyp));
                    
                }
            }
        }
    }
    //printf("%i\n", *hap);
    
    /*This then updates the velocity of the balls involved in the collisions*/
    for(int i = 0; i < colBalls.size(); i+=2){
        //these will save alot of typing
        Ball* b1 = colBalls.at(i);
        Ball* b2 = colBalls.at(i+1);

        //printf("COLISIIIIOOONNN\n");

        //calculate the sistance between balls
        float hyp = dist(b1->getPosX(), b1->getPosY(), b2->getPosX(), b2->getPosY());
        

        //this math from https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/BiggerProjects/Balls/OneLoneCoder_Balls1.cpp4
        //adapted to fit the fact that all our balls have identical mass
        float nx = (b2->getPosX() - b1->getPosX()) / hyp;
	    float ny = (b2->getPosY() - b1->getPosY()) / hyp;

        // Tangent
        float tx = -ny;
        float ty = nx;

        // Dot Product Tangent
        float dpTan1 = b1->getVelX() * tx + b1->getVelY() * ty;
        float dpTan2 = b2->getVelX() * tx + b2->getVelY() * ty;

        // Dot Product Normal
        float dpNorm1 = b1->getVelX() * nx + b1->getVelY() * ny;
        float dpNorm2 = b2->getVelX() * nx + b2->getVelY() * ny;

        // Update ball velocities
        b1->setVelX(tx * dpTan1 + nx * dpNorm2);
        b1->setVelY(ty * dpTan1 + ny * dpNorm2);
        b2->setVelX(tx * dpTan2 + nx * dpNorm1);
        b2->setVelY(ty * dpTan2 + ny * dpNorm1);
    }
    //LCD.Touch(&x, &y);
    

}

void takeInput(Board& board, Ball& cueBall) {
    float x,y;
    int sMeterX = 117, sMeterY = 56, sMeterW = 125, sMeterH = 12;
    int hitX = 255, hitY = 50, hitH = 21, hitW = 44;
    int tableX = 7, tableY = 77, tableW = 306, tableH = 156;
    float magnitude = 0;
    float unitVelX, unitVelY;
    bool magChosen = false, angleChosen = false, ballHit = false;;

    LCD.WriteAt("Strength: ", 5, 55);
    LCD.DrawRectangle(sMeterX, sMeterY, sMeterW, sMeterH);

    LCD.DrawRectangle(hitX, hitY, hitW, hitH);
    LCD.WriteAt("Hit", hitX+4, hitY+3);

    while (!ballHit) {
        //Wait for touchscreen press
        while(!LCD.Touch(&x,&y));
        //Wait for touchscreen release
        while(LCD.Touch(&x,&y));
        //if they press wihin the region of the strength meter
        if (x > sMeterX && x < (sMeterX+sMeterW) && y > sMeterY && y < (sMeterY+sMeterH)) { 
            LCD.SetFontColor(BLACK); //draw black over the old rectangle
            LCD.FillRectangle(sMeterX+1, sMeterY+1, sMeterW-2, sMeterH-2);
            LCD.SetFontColor(PURPLE); //draw a old rectangle up to where the user clicked in the strength meter
            LCD.FillRectangle(sMeterX+1, sMeterY+1, x-sMeterX, sMeterH-2);
            LCD.SetFontColor(WHITE); //set font color back to white

            double percentage = 1.0*((x - sMeterX) / sMeterW);
            magnitude = percentage * 5;
            magChosen = true;
        }
        //if they press within the pool table
        else if (x > tableX && x < (tableX+tableW) && y > tableY && y < (tableY+tableH)) {
            board.render(); //rerender board to get rid of line drawn last time
            int dx = x - cueBall.getPosX();
            int dy = y - cueBall.getPosY();
            float angle = atan2(dy,dx) * 180 / 3.14159;
            dx *= -1;
            dy *= -1;

            angle += 180;
            unitVelX = cos(angle * 3.14159 / 180);
            unitVelY = sin(angle * 3.14159 / 180);
            angleChosen = true;

            //draw line to show the angle they selected
            LCD.SetFontColor(NAVY);
            LCD.DrawLine(cueBall.getPosX(), cueBall.getPosY(), x, y);
        }
        // if they press within the hit button
        else if (magChosen && angleChosen) {
            cueBall.setVelX(unitVelX*magnitude);
            cueBall.setVelY(unitVelY*magnitude);
            ballHit = true; //will exit the function and go back to playing()
        }
    }
    //erase the input ui
    LCD.SetFontColor(BLACK);
    LCD.FillRectangle(5, 50, 310, 30);
    LCD.SetFontColor(WHITE);

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

    //basically use the velocity to determine the direction friction needs to act
    float aX = -velX*FRIC;
    float aY = -velY*FRIC;

    velX += aX;
    velY += aY;

    //if velocity is very small make it zero, to avoid weirdness
    if(abs(velX) < .01){
        velX = 0;
    }
    if(abs(velY) < .01){
        velY = 0;
    }
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

BallType Ball::getBallType() {
    return type;
}
//initalize the balls at their starting positions
Board::Board() {
    Ball rb1 = Ball(200, 150, 0, 0, Red);
    balls.push_back(rb1);
    Ball rb2 = Ball(211, 156, 0, 0, Red);
    balls.push_back(rb2);
    Ball rb3 = Ball(222, 138, 0, 0, Red);
    balls.push_back(rb3);
    Ball rb4 = Ball(233, 144, 0, 0, Red);
    balls.push_back(rb4);
    Ball rb5 = Ball(233, 168, 0, 0, Red);
    balls.push_back(rb5);
    Ball rb6 = Ball(244, 126, 0, 0, Red);
    balls.push_back(rb6);
    Ball rb7 = Ball(244, 162, 0, 0, Red);
    balls.push_back(rb7);
    Ball bb1 = Ball(211, 144, 0, 0, Blue);
    balls.push_back(bb1);
    Ball bb2 = Ball(222, 162, 0, 0, Blue);
    balls.push_back(bb2);
    Ball bb3 = Ball(233, 156, 0, 0, Blue);
    balls.push_back(bb3);
    Ball bb4 = Ball(233, 132, 0, 0, Blue);
    balls.push_back(bb4);
    Ball bb5 = Ball(244, 138, 0, 0, Blue);
    balls.push_back(bb5);
    Ball bb6 = Ball(244, 150, 0, 0, Blue);
    balls.push_back(bb6);
    Ball bb7 = Ball(244, 174, 0, 0, Blue);
    balls.push_back(bb7);
    Ball cueBall = Ball(100, 150, 0, 0, Cue);
    balls.push_back(cueBall);
    Ball eightBall = Ball(222, 150, 0, 0, Eight);
    balls.push_back(eightBall);

    Pocket p1 = Pocket(9, 79);
    pockets.push_back(p1);

    Pocket p2 = Pocket(9, 230);
    pockets.push_back(p2);

    Pocket p3 = Pocket(310, 79);
    pockets.push_back(p3);
    
    Pocket p4 = Pocket(310, 230);
    pockets.push_back(p4);

    Pocket p5 = Pocket(160, 79);
    pockets.push_back(p5);
    
    Pocket p6 = Pocket(160, 230);
    pockets.push_back(p6);

        
    Player play1 = Player();
    players.push_back(play1);

    Player play2 = Player();
    players.push_back(play2);

}


void Board::update(){
    for(int i = 0; i < balls.size(); i++){
        balls.at(i).update();
    }
}

//Draws the board and all of the balls on it
void Board::render() {
    LCD.SetFontColor(SADDLEBROWN); //the brown border
    LCD.FillRectangle(7, 77, 306, 156);
    LCD.SetFontColor(DARKGREEN); //the green table
    LCD.FillRectangle(11, 81, 298, 148);
    //Draw pockets
    for(int i = 0; i < pockets.size(); i++){
        pockets.at(i).render();
    }

    LCD.SetFontColor(WHITE); //set font color back to white

    //render each ball
    for (int i = 0; i < balls.size(); i++) {
       balls.at(i).render();
    }
}

void Board::checkWalls() {
for (int i = 0; i < balls.size(); i++) {
        //left side
        if (balls.at(i).getPosX() - (balls.at(i).getRadius()) < 11) {
            balls.at(i).setVelX(balls.at(i).getVelX()*(-1));
            balls.at(i).setPosX(11+balls.at(i).getRadius()+1);//this ensures the ball doesnt get stuck in the wall
        }
        //right side
        if (balls.at(i).getPosX()+(balls.at(i).getRadius()) > 309){
            balls.at(i).setVelX(balls.at(i).getVelX()*(-1));
            balls.at(i).setPosX(309-balls.at(i).getRadius());//this ensures the ball doesnt get stuck in the wall
        }
        //top
        if (balls.at(i).getPosY()-(balls.at(i).getRadius()-1) < 82){
            balls.at(i).setVelY(balls.at(i).getVelY()*(-1));
            balls.at(i).setPosY(82+balls.at(i).getRadius());//this ensures the ball doesnt get stuck in the wall
        }
        //bottom
        if (balls.at(i).getPosY()-(balls.at(i).getRadius()+1) > 218) {
            balls.at(i).setVelY(balls.at(i).getVelY()*(-1));
            balls.at(i).setPosY(218+balls.at(i).getRadius()+1);//this ensures the ball doesnt get stuck in the wall
        }
            
    }
}

bool Board::checkMovingBalls() {
    bool stillMoving = false;
    for (Ball ball : balls) {
        if (ball.getVelX() != 0 || ball.getVelY() != 0) {
            stillMoving = true;
        }
    }
    return stillMoving;
}
