#include "FEHLCD.h"

int main() {
    LCD.Write("Hello World");
    while (1) {
            LCD.Update();
            // Never end
        }
    return 0;
}