#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LedControl.h>
#include <string.h>
#define NULL __null

LedControl lc = LedControl(5,17,8,1);

LiquidCrystal_I2C lcd(0x27,20,4);

int rotorCount = 7;

int selected = 0;
int selectedR[] = {1,2,3};

bool setupMode = false;

int rotorA = 0;
int rotorB = 0;
int rotorC = 0;
int reflectorA = 0;

char totranslate[] = "";
char translated[] = "";


//#####################################################################################
//Enigma code

    //                   A ,B ,C ,D ,E ,F ,G ,H ,I ,J ,K ,L ,M ,N ,O ,P ,Q ,R ,S ,T ,U ,V ,W ,X ,Y ,Z
    int rotorRef [26] = {1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,9 ,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26};
    int rotorI   [26] = {5 ,11,13,6 ,12,7 ,4 ,17,22,26,14,20,15,23,25,8 ,24,21,19,16,1 ,9 ,2 ,18,3 ,10};
    int rotorII  [26] = {1 ,10,4 ,11,19,9 ,18,21,24,2 ,12,8 ,23,20,13,3 ,17,7 ,26,14,16,25,6 ,22,17,5 };
    int rotorIII [26] = {2 ,4 ,6 ,8 ,10,12,3 ,16,18,20,24,22,26,14,25,5 ,9 ,23,7 ,1 ,11,13,21,19,17,15};
//    int rotorIV  [26] = {E 	S 	O 	V 	P 	Z 	J 	A 	Y 	Q 	U 	I	R 	H 	X 	L 	N 	F 	T 	G 	K 	D 	C	M 	W 	B};
//    int rotorV   [26] = {V 	Z 	B 	R 	G 	I 	T 	Y 	U 	P 	S 	D	N 	H 	L 	X 	A 	W 	M 	J 	Q 	O 	F	E 	C 	K}
//    int rotorVI  [26] = {J 	P 	G 	V 	O 	U 	M 	F 	Y 	Q 	B 	E	N 	H 	Z 	R 	D 	K 	A 	S 	X 	L 	I	C 	T 	W}

    int reflectorB [13][2] = {
        {1,25},
        {2,18},
        {3,21},
        {4,8 },
        {5,17},
        {6,19},
        {7,12},
        {9,16},
        {10,24},
        {11,14},
        {13,15},
        {20,26},
        {22,23}
    };

    int pairs [13][2] = {
        {NULL, NULL},
        {NULL, NULL},
        {NULL, NULL},
        {NULL, NULL},
        {NULL, NULL},
        {NULL, NULL},
        {NULL, NULL},
        {NULL, NULL},
        {NULL, NULL},
        {NULL, NULL},
        {NULL, NULL},
        {NULL, NULL},
        {NULL, NULL}
    };
    



    int getReflector(int reflector[][2], int posA, int posB){
        return reflector[posA - 1][posB - 1];
    }

    int getRotor(int rotor[], int pos){
        return rotor[pos - 1];
    }

    int translateIn(int rotor[], int symbolIn, int rotorPos){
            if(symbolIn > rotorPos){
                return getRotor(rotor, symbolIn - rotorPos);
            }

            else{
                return getRotor(rotor, 26 + symbolIn - rotorPos);
            }
    }

    void setPair(int a ,int b){
      //TODO check if pair already exists
      pairs[selected][0] = a;
      pairs[selected][1] = b;
    }

    void clearPair(){
        // selected
        for (int i = selected - 1; i < 15; i++){
            pairs[i][0] = pairs[i+1][0];
            pairs[i][1] = pairs[i+1][1];
        }
            pairs[15][0] = NULL;
            pairs[15][1] = NULL;

    }

    //TODO probably still broken
    int translateOut(int rotor[], int symbolIn, int rotorPos){
        int insertPos;
        //May have broken something
        //Checks if the position is retractable from the rotor
        if(getRotor(rotor, symbolIn) > rotorPos){
            insertPos = getRotor(rotor, symbolIn) - rotorPos;
        }
        else{
            insertPos = 26 - (getRotor(rotor, symbolIn) - rotorPos);
        }
    }

    int reflect(int reflector[][2],int symbolIn){
        int x[2];
        for(int i = 0; i < 13; i++){
            
            for(int j = 0; j < 2; j++){
                
                //looks for the symbol in the reflector
                if(getReflector(reflector,i,j) == symbolIn){
                    x[0] = i;
                    x[1] = j;
                    break;
                }
            }

            //Tests if the symbol is found yet
            if(x != NULL){
                break;
            }
        }

        if(x[1] == 0){
            return getReflector(reflector,x[0],1);
        }
        else{
            return getReflector(reflector,x[0],0);
        }
    }
    
    int encrypt(int rotor1[], int rotor2[], int rotor3[], int reflector[][2] ,int pos1, int pos2, int pos3, int symbolIn){
        int symbol;
        symbol = translateIn(rotor1, symbolIn, pos1);
        symbol = translateIn(rotor2, symbol, pos2);
        symbol = translateIn(rotor3, symbol, pos3);
        symbol = reflect(reflector, symbol);
        symbol = translateOut(rotor3, symbol, pos3);
        symbol = translateOut(rotor2, symbol, pos2);
        return translateOut(rotor1, symbol, pos1);
    }

//##################################################################################
//Functional code


    void initDisplay(LiquidCrystal_I2C lcd){
        lcd.init();
        lcd.backlight();
    }

    void rotorCharacter(int val, int addr){
        if(val <= 9){
            switch(addr){
                case 0: lc.setChar(0,0,0,false);
                        lc.setChar(0,1,val,false);
                    break;
                case 1: lc.setChar(0,2,0,false);
                        lc.setChar(0,3,val,false);
                    break;
                case 2: lc.setChar(0,4,0,false);
                        lc.setChar(0,5,val,false);
                    break;
            }
        } else if(val <= 19){
            switch(addr){
                case 0: lc.setChar(0,0,1,false);
                        lc.setChar(0,1,val - 10,false);
                    break;
                case 1: lc.setChar(0,2,1,false);
                        lc.setChar(0,3,val - 10,false);
                    break;
                case 2: lc.setChar(0,4,1,false);
                        lc.setChar(0,5,val - 10,false);
                    break;
            }            
        } else{
            switch(addr){
                case 0: lc.setChar(0,0,2,false);
                        lc.setChar(0,1,val - 20,false);
                    break;
                case 1: lc.setChar(0,2,2,false);
                        lc.setChar(0,3,val - 20,false);
                    break;
                case 2: lc.setChar(0,4,2,false);
                        lc.setChar(0,5,val - 20,false);
                    break;  
            }
        }   
    }

    void rotorRotor(int val,int addr){
        lc.setChar(0, addr*2 + 1, val + 1, false);
    }

    void updateRotorDisplay(){
        if(setupMode){
        rotorRotor(selectedR[0],0);
        rotorRotor(selectedR[1],1);
        rotorRotor(selectedR[2],2);

        }else{
        rotorCharacter(rotorA, 0);
        rotorCharacter(rotorB, 1);
        rotorCharacter(rotorC, 2);
        }


        //Setting the reflector display
        switch(reflectorA){
            case 0: lc.setChar(0,6,10,false);
            case 1: lc.setChar(0,6,11,false);
        }
    }

    void updateDisplay(){
        lcd.clear();
        lcd.cursor_off();

        lcd.setCursor(0,0);
        lcd.printstr(translated);
        lcd.setCursor(0,1);
        lcd.printstr(totranslate);

        lcd.setCursor(selected, 0);
        lcd.cursor_on();
    }

    void displayPairs(){
        lcd.clear();
        lcd.cursor_off();

        String bottom = "";
        String top = "";

        for(int i; i <= 12; i++){
            if(pairs[i][0] == NULL){
                break;
            }
            bottom += pairs[i][1];
            top += pairs[i][0];
        }

        lcd.setCursor(0,0);
        lcd.printstr(bottom.toCharArray);
        lcd.setCursor(0,1);
        lcd.printstr(top.toCharArray);
    }

    bool buttonPressed(){

    }

void setup() {
    // put your setup code here, to run once:

    //Max thingy stuff
    lc.shutdown(0,false);
    lc.setIntensity(0,8);
    lc.clearDisplay(0);
}

void loop() {
    //TODO add the buttons
    //Moves the selector
        if( LEFT == HIGH){
        if(selected != 0){
                selected--;
            }
        }
        if( RIGHT == HIGH){
            if(selected != 16){
                selected++;
            }
        }
    
    
    if(setupMode){
        displayPairs();

        //ChangeRotor@Setup
            if(ROTORAUP == HIGH){
                if(selectedR[0] <! rotorCount){
                    selectedR[0]++;
                }
            }
            if(ROTORADOWN == HIGH){
                if(selectedR[0] != 0){
                    selectedR[0]--;
                }
            }
            if(ROTORBUP == HIGH){
                if(selectedR[1] <! rotorCount){
                    selectedR[1]++;
                }
            }
            if(ROTORBDOWN == HIGH){
                if(selectedR[1] != 0){
                    selectedR[1]--;
                }
            }
            if(ROTORCUP == HIGH){
                if(selectedR[2] <! rotorCount){
                    selectedR[2]++;
                }
            }
            if(ROTORCDOWN == HIGH){
                if(selectedR[2] != 0){
                    selectedR[2]--;
                }
            }

        //TODO add button
        //Exits setupMode
        if(SETUP == HIGH){
            setupMode = 0;
        }



    }else{
        updateDisplay();

        if(ROTORAUP == HIGH){
            if(rotorA <! 26){
                rotorA++;
            }
        }
        if(ROTORADOWN == HIGH){
            if(rotorA != 0){
                rotorA--;
            }
        }

        //TODO add button
        //Enters setupMode
        if(SETUP == HIGH){
            setupMode = 1;
        }

    }

    
    updateRotorDisplay();
    while(buttonPressed){}
}