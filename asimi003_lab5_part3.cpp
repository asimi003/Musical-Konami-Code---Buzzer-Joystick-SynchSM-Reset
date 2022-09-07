/*        Your Name & E-mail: Angelica Simityan   asimi003@ucr.edu

 *         Discussion Section: 021

 *         Assignment: Lab # 5 Exercise # 3

 *         Exercise Description: [optional - include for your own benefit]

 *

 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.

 *

 *         Demo Link:

 https://youtube.com/shorts/scYqN8JyBWs?feature=share

 */

enum States{INIT, S0, S1} SM1_state;
unsigned long lastRan;
const unsigned long period = 500;

// Array of Output Pin variables, set to the pins being used
const int b_size = 4;
const int b[b_size] = {8, 9, 10, 11};
const int xAxis = A0; // Recall that A0 is really a number under the hood.
int b_buf = 0;
int btn_state = 0;
int code_true = 0;
const int button = 2;
int count = 0;


const int buzzer = 3;

int code[10] = {0,0,1,1,2,2,3,3,4,4};

int record[10] = {0,0,0,0,0,0,0,0,0,0};

const char sound_pattern_size = 5;
const double sound_pattern[sound_pattern_size] = {261.63,349.23,493.88,440.00, 311.13}; // C4, D4



const int xAxis_median = 100; // Adjust this if needed

// Reset the Output Buffer. 
void resetBuffer() {
    for(int i = 0; i < b_size; i++) {
        // Note this is an arduino function call to the pins
        digitalWrite(b[i], LOW);
    }
}
// Writes to the buffer. Note this function ORs the current value with the new value
// Note that size is an optional argument with default size the same as the buffer
void writeBuffer(unsigned char b_temp, int size = b_size)
{
    for (int i = (size - 1); i >= 0; i--) {
        if ((b_temp >> i) & 0x01) {
        digitalWrite(b[i], HIGH);
        }
    }

}

int SM1_Tick(int state){
    //Read thing
    switch(state){ // State transitions
      case INIT:
         //State Transition
            state = S0;
        break;
      case S0:
        code_true =0;
        for(int i = 0; i<10;i++){
          if(record[i] == code[i]){
            code_true++;
          }
        }
          if (code_true == 10){
            state = S1;
            for(int i = 0; i<10;i++){
            record[i] = 0;
            }
          }
          else{
            state = S0;
          }
         //State Transition
        break;
      case S1:
            if (digitalRead(button) == 0){
                state = S0;
               
            }
            else{
                state = S1;
            }
             //State Transition
        break;
    }
    switch(state){ // State Action
      case INIT:
         //State Action
        break;
      case S0:
            //State Action
            resetBuffer();
            // if the joystick or button are in center position
            if(btn_state == 5){

              if(analogRead(A0) > 850)
              {
                 
                 btn_state =2;
              }
              else if (analogRead(A0) < 250)
              {
                
                btn_state = 3;
              }
              else if (analogRead(A1) < 200)
              {
                
                btn_state = 0;
              }
              else if (analogRead(A1) > 600)
              {
                
                btn_state = 1;
              }
              else if (digitalRead(button) == 0){
                btn_state = 4;
              }
            }


            
            //If the joystick or button are still pressed
            else
            {
              
              //checks if the up down is on center position
              if(btn_state == 0 || btn_state == 1){
                if(analogRead(A1) < 850 && analogRead(A1) >250){
                      for(int i = 0; i<10;i++){
                        if(i != 9){
                          record[i] = record[i+1];
                        }
                        else{
                          record[i]=btn_state;
                        }
                       }
                  btn_state = 5;
                  noTone(buzzer);
                }
                //if not then play sound
                else{
                      tone(buzzer, sound_pattern[btn_state]);
                    }
              }

 
              
              //checks if the left or right are on center position
             if(btn_state == 2 || btn_state == 3){
                if(analogRead(A0) < 850 && analogRead(A0) >250){
                  for(int i = 0; i<10;i++){
                    if(i != 9){
                      record[i] = record[i+1];
                    }
                    else{
                      record[i]=btn_state;
                    }
                  }
                  btn_state = 5;
                  noTone(buzzer);
                }
              
              //if not play sound
              else{
                     tone(buzzer, sound_pattern[btn_state]);
                  }

             }
             //checks if the button is released
             if(btn_state ==4){
              if(digitalRead(button) == 1){
                for(int i = 0; i<10;i++){
                    if(i != 9){
                      record[i] = record[i+1];
                    }
                    else{
                      record[i]=btn_state;
                    }
                  }
              btn_state = 5;
              noTone(buzzer);
              
             }
              
            
            else{
                     tone(buzzer, sound_pattern[btn_state]);
                 }
            }
         }
         for(int i = 0; i<10;i++){
          Serial.print(record[i]);
          Serial.print(", ");
          
         }
         Serial.println(" ");
           writeBuffer(b_buf);
        
         //State Action
        break;
      case S1:
            //State Action
          resetBuffer();
          tone(buzzer, sound_pattern[code[count]]);
           
            count++;
            if (count > 10){
              count = 0;
            }
          b_buf=0;
            
         
         //State Action
        break;
    }

    return state;
}


void setup() {
    // OUTPUT is a defined macro in Arduino!
    for(int i = 0; i < b_size; i++)
        {
            pinMode(b[i], OUTPUT);
           
        }
        pinMode(button, INPUT_PULLUP);
    // We will also read from the serial monitor
    Serial.begin(9600);
}

void loop() {
    
    if ((millis() - lastRan) > period) {     
      SM1_state = (States)SM1_Tick(SM1_state);        
      lastRan = millis();   
     }  
}
