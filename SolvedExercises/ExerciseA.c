#include <wiringPi.h> //Include WiringPi library
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define RED 12
#define GREEN 13
#define BLUE 18
#define BUTTON1 23
#define BUTTON2 24

#define CRED  "\x1B[1;31m"
#define GRN   "\x1B[1;32m"
#define BLU   "\x1B[1;34m"
#define RESET "\x1B[1;0m"

int flag = 0;

int LED_Menu(void);
void Button(void);
void PWM_Follower(void);

void myInterrupt0 (void) 
{ 
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    digitalWrite(RED, 0);
    digitalWrite(GREEN, 0);
    digitalWrite(BLUE, 0);
    flag = 1;
}


int main(){

int menu = 0;

wiringPiSetupGpio();

wiringPiISR (BUTTON2, INT_EDGE_FALLING, &myInterrupt0) ;

pinMode(RED, OUTPUT);
pinMode(GREEN, OUTPUT);
pinMode(BLUE, OUTPUT);
pinMode(BUTTON1, INPUT);
pinMode(BUTTON2, INPUT);
pullUpDnControl(BUTTON1, PUD_UP);
pullUpDnControl(BUTTON2, PUD_UP);

while(1){

printf("Select a mode:\n");
printf("1. LED\n");
printf("2. Button\n");
printf("3. PWM Light Follower\n");

scanf("%d",&menu);

    switch(menu)
    {
        case 1:
            LED_Menu();
            break;
        
        case 2:
            Button();
            break;

        case 3:
            PWM_Follower();
            break;
    }
}

return 0;
}


int LED_Menu(void){

    char choice;

    while(1){

        printf("SELECT A COLOR:\n");
        printf("1. RED\n");
        printf("2. GREEN\n");
        printf("3. BLUE\n");;
        printf("0. OFF\n");
        printf("Q. EXIT\n");

        scanf("%s", &choice);
            switch(toupper(choice))
            {
                case '1':
                    system("clear");
                    printf(CRED "RED\n\n" RESET);
                    digitalWrite(RED, 1);
                    break;

                case '2':
                    system("clear");
                    printf(GRN "GREEN\n\n" RESET);
                    digitalWrite(GREEN, 1);
                    break;
                    
                case '3':
                    system("clear");
                    printf(BLU "BLUE\n\n" RESET);
                    digitalWrite(BLUE, 1);
                    break;

                case '0':
                    system("clear");
                    printf("OFF\n\n");
                    digitalWrite(RED, 0);
                    digitalWrite(GREEN, 0);
                    digitalWrite(BLUE, 0);
                    break;
                
                case 'Q':
                    system("clear");
                    return 0;

            }
    }
}

void Button(void){

    int i = 0;

    do{
        if(digitalRead(BUTTON1)){     //If Button is HIGH
            continue;
        }
        else{                        //If Button is LOW
            delay(500);
            i++;
            printf("Number of times pressed: %d\n", i);
            switch(i){
                case 1:
                    digitalWrite(RED, HIGH);
                    delay(500);
                    digitalWrite(RED, LOW);
                    break;
                case 2:
                    digitalWrite(GREEN, HIGH);
                    delay(500);
                    digitalWrite(GREEN, LOW);
                    break;
                case 3:
                    digitalWrite(BLUE, HIGH);
                    delay(500);
                    digitalWrite(BLUE, LOW);
                    break;
            }
        }
    }while(i != 3);

}

void PWM_Follower(void){

    pinMode(RED, PWM_OUTPUT);
    pinMode(BUTTON2, INPUT);
    pullUpDnControl(BUTTON2, PUD_UP);

    while(1){

        if (flag == 1)
        {
            flag = 0;
            break;
        }
        else{

            for (int i = 0; i < 1024; i++)
            {
                pwmWrite(RED, i);
                delay(1);

                if(i == 1023){

                    pwmWrite(RED, 0);
                    pinMode(RED, INPUT);
                    pinMode(GREEN, PWM_OUTPUT);
                    break;
                }        
            }
            for (int j = 0; j < 1024; j++)
            {
                pwmWrite(GREEN, j);
                delay(1);

                if(j == 1023){

                    pwmWrite(GREEN, 0);
                    pinMode(GREEN, INPUT);
                    pinMode(BLUE, PWM_OUTPUT);
                    break;
                }        
            }
            for (int k = 0; k < 1024; k++)
            {
                pwmWrite(BLUE, k);
                delay(1);

                if(k == 1023){
                    pwmWrite(BLUE, 0);
                    pinMode(BLUE, INPUT);
                    pinMode(RED, PWM_OUTPUT);
                    break;
                }        
            }
        }
    }
}

