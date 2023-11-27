#include "FEHLCD.h"
#include <string.h>

//global vars
char state = 'm';

//function decs
void drawMenu();

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
        }
        else if(state == 't'){
            //placeholder
            LCD.Write("PLAYING TIMED");
        }
        else if(state == 'i'){
            //placeholder
            LCD.Write("HERES HOW TO PLAY THE GAME");
        }
        else if(state == 'e'){
            running = false;
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
    LCD.SetBackgroundColor(BLACK);
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