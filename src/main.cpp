#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#define NULL __null

LiquidCrystal_I2C lcd(0x27,20,4);

void setup() {
  // put your setup code here, to run once:
  initDisplay(lcd);
}

void loop() {
  lcd.setCursor(selected, 0);
  //Check if any button is pressed

  //temp
  delay(1000);
}


//#####################################################################################
//Enigma code

    int rotorRef [26] = {1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,9 ,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26};
    int rotorI   [26] = {5 ,11,13,6 ,12,7 ,4 ,17,22,26,14,20,15,23,25,8 ,24,21,19,16,1 ,9 ,2 ,18,3 ,10};
    int rotorII  [26] = {};
    int rotorIII [26] = {};
    int rotorIV  [26] = {};

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

    int pairs [16][2] = {

    };
    int selected = 0;


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

    void addPair(int a ,int b){
      //TODO check if pair already exists
      pairs[selected][0] = a;
      pairs[selected][1] = b;
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

    void setRotorDisplay(){
        
    }

    void displayPairs(){
        
    }