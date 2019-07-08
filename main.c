#include <xc.h>

#define TRUE 1
#define FALSE 0
#define INPUT 1
#define OUTPUT 0
#define ON 1
#define OFF 0
#define PUSH_BUTTON_INCREMENT RD0
#define PUSH_BUTTON_MOVE RD1
#define PUSH_BUTTON_PLAY RD2
#define PUSH_BUTTON_RESET RD3
#define RELAY RC6
#define TRIGGER_INTERVAL_TIME 1
#define TRIGGER_PLAY_TIME 10
#define TRIGGER_MAX_TIME TRIGGER_INTERVAL_TIME * 6

char isPlay = FALSE;

char digitHour0 = 0;
int triggerHour0 = 0;

char digitHour1 = 0;
int triggerHour1 = TRIGGER_INTERVAL_TIME;

char digitMinute0 = 0;
int triggerMinute0 = TRIGGER_INTERVAL_TIME * 2;

char digitMinute1 = 0;
int triggerMinute1 = TRIGGER_INTERVAL_TIME * 3;

char digitSecond0 = 0;
int triggerSecond0 = TRIGGER_INTERVAL_TIME * 4;

char digitSecond1 = 0;
int triggerSecond1 = TRIGGER_INTERVAL_TIME * 5;

int triggerPlayTime = 0;

char cursor = 0;
char triggerPosition = 0;
char triggerIncrement = 0;

void reset();
void managePlayResetEvent();
void moveCursor();
void manageMoveEvent();
void incrementDigit(char* digit, const char limit);
void increment();
void manageIncrementEvent();
void writeDigits();
void writeDigit(char number);
void managePlay();
void setup();
void loop();

void reset(){
    isPlay = FALSE;
    
    digitHour0 = 0;
    digitHour1 = 0;
    
    digitMinute0 = 0;
    digitMinute1 = 0;
    
    digitSecond0 = 0;
    digitSecond1 = 0;

    cursor = 0;
    triggerPosition = 0;
    triggerPlayTime = 0;
    
    RELAY = OFF;
}

void managePlayResetEvent(){
    if (PUSH_BUTTON_PLAY == ON)
        isPlay = TRUE;
    else if (PUSH_BUTTON_RESET == ON)
        reset();
}

void moveCursor(){
    if (isPlay)
        return;
    
    if (cursor == 5)
        cursor = 0;
    else
        cursor++;
}

void manageMoveEvent(){
    if (isPlay)
        return;
    
    if (PUSH_BUTTON_MOVE == ON && triggerPosition == 0){
        triggerPosition = 1;
        moveCursor();
    } else if (PUSH_BUTTON_MOVE == OFF && triggerPosition == 1)
        triggerPosition = 0;
}

void incrementDigit(char* digit, const char limit){
    if (*digit == limit)
        *digit = 0;
    else
        (*digit)++;
}

void increment(){
    switch (cursor){
        case 0:
            incrementDigit(&digitHour0, 9);
            break;
        case 1:
            incrementDigit(&digitHour1, 9);
            break;
        case 2:
            incrementDigit(&digitMinute0, 5);
            break;
        case 3:
            incrementDigit(&digitMinute1, 9);
            break;
        case 4:
            incrementDigit(&digitSecond0, 5);
            break;
        case 5:
            incrementDigit(&digitSecond1, 9);
    }
}

void manageIncrementEvent(){
    if (isPlay)
        return;
    
    if (PUSH_BUTTON_INCREMENT == ON && triggerIncrement == 0){
        triggerIncrement = 1;
        increment();
    } else if (PUSH_BUTTON_INCREMENT == OFF && triggerIncrement == 1)
        triggerIncrement = 0;
}

void writeDigits(){
    PORTB = 0b00000000;
    if (triggerHour0 == TRIGGER_MAX_TIME){
        RC0 = OFF;
        RC1 = ON;
        RC2 = ON;
        RC3 = ON;
        RC4 = ON;
        RC5 = ON;
        triggerHour0 = 0;
        writeDigit(digitHour0);
    } else
        triggerHour0++;
    
    if (triggerHour1 == TRIGGER_MAX_TIME){
        RC0 = ON;
        RC1 = OFF;
        RC2 = ON;
        RC3 = ON;
        RC4 = ON;
        RC5 = ON;
        triggerHour1 = 0;
        writeDigit(digitHour1);
    } else
        triggerHour1++;
    
    if (triggerMinute0 == TRIGGER_MAX_TIME){
        RC0 = ON;
        RC1 = ON;
        RC2 = OFF;
        RC3 = ON;
        RC4 = ON;
        RC5 = ON;
        triggerMinute0 = 0;
        writeDigit(digitMinute0);
    } else
        triggerMinute0++;
    
    if (triggerMinute1 == TRIGGER_MAX_TIME){
        RC0 = ON;
        RC1 = ON;
        RC2 = ON;
        RC3 = OFF;
        RC4 = ON;
        RC5 = ON;
        triggerMinute1 = 0;
        writeDigit(digitMinute1);
    } else
        triggerMinute1++;
    
    if (triggerSecond0 == TRIGGER_MAX_TIME){
        RC0 = ON;
        RC1 = ON;
        RC2 = ON;
        RC3 = ON;
        RC4 = OFF;
        RC5 = ON;
        triggerSecond0 = 0;
        writeDigit(digitSecond0);
    } else
        triggerSecond0++;
    
    if (triggerSecond1 == TRIGGER_MAX_TIME){
        RC0 = ON;
        RC1 = ON;
        RC2 = ON;
        RC3 = ON;
        RC4 = ON;
        RC5 = OFF;
        triggerSecond1 = 0;
        writeDigit(digitSecond1);
    } else
        triggerSecond1++;
}

void writeDigit(char number){
    switch (number){
        case 0:
            PORTB = 0b00111111;
            break;
        case 1:
            PORTB = 0b00000110;
            break;
        case 2:
            PORTB = 0b01011011;
            break;
        case 3:
            PORTB = 0b01001111;
            break;
        case 4:
            PORTB = 0b01100110;
            break;
        case 5:
            PORTB = 0b01101101;
            break;
        case 6:
            PORTB = 0b01111101;
            break;
        case 7:
            PORTB = 0b00000111;
            break;
        case 8:
            PORTB = 0b01111111;
            break;
        case 9:
            PORTB = 0b01101111;
    }
}

void setup(){
    TRISB = OUTPUT;
    TRISC = OUTPUT;
    TRISD0 = INPUT;
    TRISD1 = INPUT;
    TRISD2 = INPUT;
    TRISD3 = INPUT;
    TRISD4 = INPUT;
}

void managePlay(){
    if (isPlay == FALSE)
        return;
    
    if (digitHour0 == 0 && 
        digitHour1 == 0 &&
        digitMinute0 == 0 &&
        digitMinute1 == 0 &&
        digitSecond0 == 0 &&
        digitSecond1 == 0){
        RELAY = OFF;
        reset();
    } else
        RELAY = ON;
    
    if (triggerPlayTime != TRIGGER_PLAY_TIME){
        triggerPlayTime++;
        return;
    } 
    
    triggerPlayTime = 0;
    if (digitSecond1 > 0){
        digitSecond1--;
        return;
    }
    
    digitSecond1 = 9;
    if (digitSecond0 > 0){
        digitSecond0--;
        return;
    }
    
    digitSecond0 = 5;
    if (digitMinute1 > 0){
        digitMinute1--;
        return;
    }
    
    digitMinute1 = 9;
    if (digitMinute0 > 0){
        digitMinute0--;
        return;
    }
    
    digitMinute0 = 5;
    if (digitHour1 > 0){
        digitHour1--;
        return;
    }
    
    digitHour1 = 9;
    if (digitHour0 > 0)
        digitHour0--;
}

void loop(){
    managePlayResetEvent();
    manageMoveEvent();
    manageIncrementEvent();
    managePlay();
    writeDigits();
}

void main(void) {
    setup();
    
    while (TRUE)
       loop(); 
}
