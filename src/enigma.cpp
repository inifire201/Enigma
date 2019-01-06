//here goes all the non phisical techical stuff

class Enigma{
    //TODO add the rest of these rotors
    //                   A ,B ,C ,D ,E ,F ,G ,H ,I ,J ,K ,L ,M ,N ,O ,P ,Q ,R ,S ,T ,U ,V ,W ,X ,Y ,Z
    int rotorRef [26] = {1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,9 ,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26};
    int rotorI   [26] = {5 ,11,13,6 ,12,7 ,4 ,17,22,26,14,20,15,23,25,8 ,24,21,19,16,1 ,9 ,2 ,18,3 ,10};
    int rotorII  [26] = {};
    int rotorIII [26] = {};
    int rotorIV  [26] = {};


    int rotorset1;
    int rotorset2;
    int rotorset3;
    int reflectorset;

    int translate(int rotor[], int symbolIn, int position){
        //TODO add position functionality
        int pos;
        for(int i=0; rotorRef[i] == symbolIn; i++){
            pos = i;
        }
        
        if(rotorRef[pos] > rotor[pos]){
            return symbolIn - pos;
        }
        else{
            return symbolIn + 26 - pos;
        }
    }

}