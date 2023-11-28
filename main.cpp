//Max O'Ganian and Nicole Futoryansky

#include "FEHLCD.h"
#include <string.h>
#include <math.h>

//global vars
char state = 'm';

//function decs
void drawMenu(), retMenu(), playing();
float dist(float, float, float, float);

int main() {
    //local vars to main
    bool running = true;

    while (running) {
        if(state == 'm'){
            drawMenu();
        }
        else if(state == 'p'){
            //placeholder
            LCD.WriteAt("PLAYING", 100, 20);
            playing();
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

void playing() {
    int circX = 100, circY = 100, circR = 20;
    float x, y;

    LCD.DrawCircle(circX, circY, circR);

    LCD.Touch(&x,&y);

    float hyp = dist(circX, circY, x, y);
    if (hyp < circR){
        printf("%f\n", hyp);
    }

}

float dist(float x, float y, float x1, float y1){
    //calculate the magnitude of the diatance between two points
    float hyp = sqrt(pow(abs(x1-x), 2.0) + pow(abs(y1-y), 2.0));
    return hyp;
}