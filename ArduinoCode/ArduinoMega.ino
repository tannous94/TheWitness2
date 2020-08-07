#include <Wire.h>
#include <LiquidCrystal.h>

#define LAST 4

int s3 = 2;
int s2 = 3;
int s1 = 4;
int s0 = 5;
int sig = 18;

int seconds = 0;
String to_nodemcu = "STILL PLAYING.......";
bool non = true;

//int sen_pins[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
int sen_state[16] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
LiquidCrystal lcd(7,8,9,10,11,12);

  int muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  }; 

  void(* resetFunc) (void) = 0; //declare reset function @ address 0

int controlPin[] = {s0, s1, s2, s3};
int path[16] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int index = 0;

int leds[16][6] = {{A1, A2, A0, 0, 0, 0}, // G doesn't work
                  {A5, A4, A3, 0, 0, 0}, //good
                  {A6, A7, A8, 0, 0, 0}, //good
                  {A9, A10, A11, 0, 0, 0}, //good
                  
                  {A12, A13, A14, 0, 0, 0}, //good
                  {50, 52, 53, 0, 0, 0}, // good
                  {48, 49, 51, 0, 0, 0}, //good
                  {46, 47, 44, 0, 0, 0}, //not very good
                  
                  {42, 43, 45, 0, 0, 0}, // not good
                  {40, 41, 39, 0, 0, 0}, // good
                  {37, 38, 36, 0, 0, 0}, // good
                  {33, 35, 34, 0, 0, 0}, //good
                  
                  {22, 6, 23, 0, 0, 0},//good
                  {24, 26, 25, 0, 0, 0},//good
                  {29, 27, 28, 0, 0, 0},//good
                  {31, 32, 30, 0, 0, 0}};//good 
 /*{31, 32, 30, 0, 0, 0}, // kan 16
                  //good
                  {24, 26, 25, 0, 0, 0},//good
                  {29, 27, 28, 0, 0, 0},//good
                 {22, 6, 23, 0, 0, 0}};//good // kan 13 */


//int prev_led = 0;
int last_led = 0;
int end_led = 0;
int purple_blocks = 0; 
int purple_counter = 0;

String payload = "";
int flag = 1;

  int j = 3; // 3 is r, 4 is g, 5 is b... then reset to 2.
  String num = "";
  int val = -1;
  int color = -1;
  int done = 1;

  int acc_done = 1;
  String acc_info[4] = {"", "", "", ""}; // [0]=level, [1]=points, [2]=rank, [3]=time
  int q = 0;
  String num1 = "";

// function that executes whenever data is received from master
void receiveEvent(int howMany) {
 while (0 < Wire.available() /*&& flag*/) {
    char c = Wire.read();      /* receive byte as a character */
    if (c == '#') {
          for (int r = 0; r < 16; r++) {
            setCol(leds[r], 0, 0, 0);
          }
       
          to_nodemcu = "STILL PLAYING.......";
        for (int n = 0; n < 4; n++) {
            acc_info[n] = "";
        }
       seconds = 0;
       done = 1;
       acc_done = 1;
       q= 0 ;
       index = 0;
        last_led = 0;
        end_led = 0;
        purple_blocks = 0; 
        purple_counter = 0;
        for (int m = 0; m < 16; m++) {
           path[m] = -1;
        }
        for (int t = 0; t < 16; t++) {
            leds[t][3] = 0;
            leds[t][4] = 0;
            leds[t][5] = 0;
        }
        
        payload = "";
        j = 3; 
        num = "";
        num1 = "";
        val = -1;
        color = -1;
       
       lcd.clear();
       lcd.print("Level: ");
       lcd.setCursor(0, 1);
       lcd.print("Timer: ");
    } else {
    //Serial.print(c);           /* print the character */
     payload += c;
     //non = false;
    }
    //if (c == '}')
    //  flag = 0;
  }
 //Serial.println();             /* to newline */
}

// function that executes whenever data is requested from master
void requestEvent() {
   //Serial.println(payload);
   //payload = "";
  Wire.write(to_nodemcu.c_str());  /*send string on request */
  //to_nodemcu = "STILL PLAYING.......";
}

void get_start_point() {
    for(int i = 0; i < 16; i++) {
        if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 0) {
            last_led = i;
            //prev_led = i;
            path[0] = i;
            index = 1;
            setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
            //sen_state[i] = HIGH;
            break;
        }
    }
}

void get_end_point() {
    for(int i = 0; i < 16; i++) {
        if (leds[i][3] == 0 && leds[i][4] == 150 && leds[i][5] == 0) {
            end_led = i;
      setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
            break;
        }
    }
}

bool find_in_path(int led) {
    for (int i = 0; i < 16; i++) {
        if (path[i] == led)
            return true;
    }
    return false;
}

void get_purple_blocks() {
    for(int i = 0; i < 16; i++) {
        if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
      setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
            purple_blocks++;
        }
    }
}

void get_blue() {
    for(int i = 0; i < 16; i++) {
        if (leds[i][3] == 0 && leds[i][4] == 0 && leds[i][5] == 150) {
      setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
           // purple_blocks++;
        }
    }
}

/*void resetSensors() {
    pinMode(sig, OUTPUT);
     for (int k = 0; k < 16; k++) {
       for(int i = 0; i < 4; i++) {
           digitalWrite(controlPin[i], muxChannel[k][i]); // muxChannel[sen_pins[k]][i]
       }
       digitalWrite(sig, LOW);
    }
    pinMode(sig, INPUT);
}*/

void get_acc_info() {
  if (acc_done) {
            for (char& z : payload) {
              if (z == '}') {
                  acc_done = 0;
                        //Serial.println(payload);
                        Serial.println("ACC INF: ");
                        for (int i = 0; i < 4; i++) {
                            Serial.print(acc_info[i] + ", ");
                        }
                        Serial.println();
                        payload = "";
                  break;
              }
              
              if (z >= '0' && z <= '9') {
                num1 += z;
                continue;
              }
            
              if (z > '9' || z < '0') {
                if (num1 != "") {
                  acc_info[q] = num1;
                  num1 = "";
                  q++;
                }
              }
            
           
            }
      }
}


void setup()
{
   Wire.begin(8);                /* join i2c bus with address 8 */
 Wire.onReceive(receiveEvent); /* register receive event */
 Wire.onRequest(requestEvent); /* register request event */
 Serial.begin(9600);           /* start serial for debug */
  lcd.begin(16, 2);
  lcd.print("Level: ");
  lcd.setCursor(0, 1);
  lcd.print("Timer: ");
 
  for(int i = 0; i < 16; i++) {
    pinMode(leds[i][0], OUTPUT);
    pinMode(leds[i][1], OUTPUT);
    pinMode(leds[i][2], OUTPUT);
  }

  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(sig, INPUT);
  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);

//  for (int i = 0; i < 16; i++)
//    sen_state[i] = LOW;
  delay(500);
}

void loop()
{
 // if (non == false) {
   if (done) {
            for (char& z : payload) {
              if (z == '}') {
                  done = 0;
                  get_start_point();
                  get_end_point();
                  get_purple_blocks();
          get_blue();
                 // resetSensors();
//                  for (int i = 0; i < 16; i++) {
//                      setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
//                  }
                 // setCol(leds[path[0]], leds[path[0]][3],leds[path[0]][4],leds[path[0]][5]);
                        Serial.println(payload);
                        payload = "";
                  break;
              }
              
              if (z >= '0' && z <= '9') {
                num += z;
                continue;
              }
            
              if (val != -1 && (z > '9' || z < '0')) {
                if (num != "") {
                  color = atoi(num.c_str());
                  num = "";
                }
                if (color != -1) {
                  leds[val-1][j] = color;
                  color = -1;
                  j++;
                }
              if (z == ')') {
                  j = 3;
                  val = -1;
              }
                continue;
              }
            
              if (val == -1 && (z > '9' || z < '0')) {
                if (num != "") {
                  val = atoi(num.c_str());
                  num = "";
                }     
                continue;
              }
            }
//          for(int i = 0; i < 16; i++) {
//              Serial.print(leds[i][3]);
//              Serial.print("---");
//              Serial.print(leds[i][4]);
//              Serial.print("---");
//              Serial.println(leds[i][5]);
//          }
      }
  
      get_acc_info();

       lcd.setCursor(8, 0);
       lcd.print(acc_info[0]);
      
        readSenState();
        //sen_state[path[0]] = HIGH;
    
        delay(25);

      
       // light_me_up();
  //  if in path and not last
  // in order to make the sensors high if they're not the last and in path (for example the 0 1 2 3 7 6, and turn off the 2 situation) 
  /*  for(int i = 0; i < 16; i++) {
      if (sen_state[i] == LOW) {
        if (find_in_path(i) && i != last_led) {
          sen_state[i] = HIGH;
        }
        else 
        {
          continue;
        }
      }
    }*/


      for(int i = 0; i < 16; i++) {
//          if (path[i] != -1) {
//              setCol(leds[path[i]], 150, 0, 0);
//          } else {
//              setCol(leds[path[i]], leds[path[i]][3], leds[path[i]][4], leds[path[i]][5]);
//          }
        Serial.print(path[i]);
        Serial.print(",");
      }
      Serial.println();
           for(int i = 0; i < 16; i++) {
        Serial.print(sen_state[i]);
        Serial.print(",");
      }
      Serial.print("Last LED: ");
       Serial.print(last_led);
       Serial.print("---Index: ");
       Serial.println(index);
       Serial.print("P Blocks: ");
       Serial.print(purple_blocks);
       Serial.print(", P Count: ");
       Serial.println(purple_counter);
     //sen_state[path[0]] = HIGH;
      for(int i = 0; i < 16; i++) {
        // setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
        if (sen_state[i] == LOW) {
          if (leds[i][3] == 0 && leds[i][4] == 0 && leds[i][5] == 150) {
             //sen_state[i] == LOW;
          } else {
            switch(i) {
                case 0: if (last_led == 1 || last_led == 4) {
                  if (!find_in_path(i)) {
                        setCol(leds[i], 150, 0, 0);
                        delay(800);
                        path[index] = i;
                        index = index + 1;
                        last_led = i;
                        if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
                            purple_counter = purple_counter + 1;
                        }
                    
                  }
                } else {
          if (last_led == i) {
            if(i == path[index - 1] && index != 1) {
            index = index - 1;
            path[index] = -1;
            last_led = path[index - 1]; 
            if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
              purple_counter = purple_counter - 1;
            }
          }
          setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
          }
        }
        break;
                case 1: if (last_led == 0 || last_led == 5 || last_led == 2) {
                  if (!find_in_path(i)) {
                        setCol(leds[i], 150, 0, 0);
                        delay(800);
                        path[index] = i;
                        index = index + 1;
                        last_led = i;
                        if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
                            purple_counter = purple_counter + 1;
                        }
                    
                  }
                } else {
          if (last_led == i) {
            if(i == path[index - 1] && index != 1) {
            index = index - 1;
            path[index] = -1;
            last_led = path[index - 1]; 
            if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
              purple_counter = purple_counter - 1;
            }
          }
          setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
          }
        }  break;
                case 2: if (last_led == 1 || last_led == 3 || last_led == 6) {
                  if (!find_in_path(i)) {
                        setCol(leds[i], 150, 0, 0);
                        delay(800);
                        path[index] = i;
                        index = index + 1;
                        last_led = i;
                        if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
                            purple_counter = purple_counter + 1;
                        }
                    
                  }
                } else {
          if (last_led == i) {
            if(i == path[index - 1] && index != 1) {
            index = index - 1;
            path[index] = -1;
            last_led = path[index - 1]; 
            if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
              purple_counter = purple_counter - 1;
            }
          }
          setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
          }
        } break;
                case 3: if (last_led == 2 || last_led == 7) {
                  if (!find_in_path(i)) {
                        setCol(leds[i], 150, 0, 0);
                        delay(800);
                        path[index] = i;
                        index = index + 1;
                        last_led = i;
                        if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
                            purple_counter = purple_counter + 1;
                        }
                    
                  }
                } else {
          if (last_led == i) {
            if(i == path[index - 1] && index != 1) {
            index = index - 1;
            path[index] = -1;
            last_led = path[index - 1]; 
            if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
              purple_counter = purple_counter - 1;
            }
          }
          setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
          }
        }  break;
                case 4: if (last_led == 0 || last_led == 5 || last_led == 8) {
                  if (!find_in_path(i)) {
                        setCol(leds[i], 150, 0, 0);
                        delay(800);
                        path[index] = i;
                        index = index + 1;
                        last_led = i;
                        if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
                            purple_counter = purple_counter + 1;
                        }
                    
                  }
                } else {
          if (last_led == i) {
            if(i == path[index - 1] && index != 1) {
            index = index - 1;
            path[index] = -1;
            last_led = path[index - 1]; 
            if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
              purple_counter = purple_counter - 1;
            }
          }
          setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
          }
        }  break;
                case 5: if (last_led == 1 || last_led == 4 || last_led == 6 || last_led == 9) {
                    if (!find_in_path(i)) {
                        setCol(leds[i], 150, 0, 0);
                        delay(800);
                        path[index] = i;
                        index = index + 1;
                        last_led = i;
                        if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
                            purple_counter = purple_counter + 1;
                        }
                    
                  }
                } else {
          if (last_led == i) {
            if(i == path[index - 1] && index != 1) {
            index = index - 1;
            path[index] = -1;
            last_led = path[index - 1]; 
            if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
              purple_counter = purple_counter - 1;
            }
          }
          setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
          }
        } break;
                case 6: if (last_led == 2 || last_led == 5 || last_led == 7 || last_led == 10) {
                 if (!find_in_path(i)) {
                        setCol(leds[i], 150, 0, 0);
                        delay(800);
                        path[index] = i;
                        index = index + 1;
                        last_led = i;
                        if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
                            purple_counter = purple_counter + 1;
                        }
                    
                  }
                } else {
          if (last_led == i) {
            if(i == path[index - 1] && index != 1) {
            index = index - 1;
            path[index] = -1;
            last_led = path[index - 1]; 
            if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
              purple_counter = purple_counter - 1;
            }
          }
          setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
          }
        } break;
                case 7: if (last_led == 3 || last_led == 11 || last_led == 6) {
                  if (!find_in_path(i)) {
                        setCol(leds[i], 150, 0, 0);
                        delay(800);
                        path[index] = i;
                        index = index + 1;
                        last_led = i;
                        if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
                            purple_counter = purple_counter + 1;
                        }
                    
                  }
                } else {
          if (last_led == i) {
            if(i == path[index - 1] && index != 1) {
            index = index - 1;
            path[index] = -1;
            last_led = path[index - 1]; 
            if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
              purple_counter = purple_counter - 1;
            }
          }
          setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
          }
        } break;
                case 8: if (last_led == 4 || last_led == 9 || last_led == 12) {
                  if (!find_in_path(i)) {
                        setCol(leds[i], 150, 0, 0);
                        delay(800);
                        path[index] = i;
                        index = index + 1;
                        last_led = i;
                        if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
                            purple_counter = purple_counter + 1;
                        }
                    
                  }
                } else {
          if (last_led == i) {
            if(i == path[index - 1] && index != 1) {
            index = index - 1;
            path[index] = -1;
            last_led = path[index - 1]; 
            if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
              purple_counter = purple_counter - 1;
            }
          }
          setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
          }
        } break;
                case 9: if (last_led == 5 || last_led == 8 || last_led == 10 || last_led == 13) {
                   if (!find_in_path(i)) {
                        setCol(leds[i], 150, 0, 0);
                        delay(800);
                        path[index] = i;
                        index = index + 1;
                        last_led = i;
                        if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
                            purple_counter = purple_counter + 1;
                        }
                    
                  }
                } else {
          if (last_led == i) {
            if(i == path[index - 1] && index != 1) {
            index = index - 1;
            path[index] = -1;
            last_led = path[index - 1]; 
            if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
              purple_counter = purple_counter - 1;
            }
          }
          setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
          }
        } break;
                case 10: if (last_led == 6 || last_led == 9 || last_led == 11 || last_led == 14) {
                   if (!find_in_path(i)) {
                        setCol(leds[i], 150, 0, 0);
                        delay(800);
                        path[index] = i;
                        index = index + 1;
                        last_led = i;
                        if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
                            purple_counter = purple_counter + 1;
                        }
                    
                  }
                } else {
          if (last_led == i) {
            if(i == path[index - 1] && index != 1) {
            index = index - 1;
            path[index] = -1;
            last_led = path[index - 1]; 
            if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
              purple_counter = purple_counter - 1;
            }
          }
          setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
          }
        } break;
                case 11: if (last_led == 7 || last_led == 10 || last_led == 15) {
                   if (!find_in_path(i)) {
                        setCol(leds[i], 150, 0, 0);
                        delay(800);
                        path[index] = i;
                        index = index + 1;
                        last_led = i;
                        if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
                            purple_counter = purple_counter + 1;
                        }
                    
                  }
                } else {
          if (last_led == i) {
            if(i == path[index - 1] && index != 1) {
            index = index - 1;
            path[index] = -1;
            last_led = path[index - 1]; 
            if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
              purple_counter = purple_counter - 1;
            }
          }
          setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
          }
        } break;
                case 12: if (last_led == 8 || last_led == 13) {
                  if (!find_in_path(i)) {
                        setCol(leds[i], 150, 0, 0);
                        delay(800);
                        path[index] = i;
                        index = index + 1;
                        last_led = i;
                        if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
                            purple_counter = purple_counter + 1;
                        }
                    
                  }
                } else {
          if (last_led == i) {
            if(i == path[index - 1] && index != 1) {
            index = index - 1;
            path[index] = -1;
            last_led = path[index - 1]; 
            if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
              purple_counter = purple_counter - 1;
            }
          }
          setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
          }
        } break;
                case 13: if (last_led == 9 || last_led == 12 || last_led == 14) {
                   if (!find_in_path(i)) {
                        setCol(leds[i], 150, 0, 0);
                        delay(800);
                        path[index] = i;
                        index = index + 1;
                        last_led = i;
                        if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
                            purple_counter = purple_counter + 1;
                        }
                    
                  }
                } else {
          if (last_led == i) {
            if(i == path[index - 1] && index != 1) {
            index = index - 1;
            path[index] = -1;
            last_led = path[index - 1]; 
            if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
              purple_counter = purple_counter - 1;
            }
          }
          setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
          }
        } break;
                case 14: if (last_led == 10 || last_led == 13 || last_led == 15) {
                  if (!find_in_path(i)) {
                        setCol(leds[i], 150, 0, 0);
                        delay(800);
                        path[index] = i;
                        index = index + 1;
                        last_led = i;
                        if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
                            purple_counter = purple_counter + 1;
                        }
                    
                  }
                } else {
          if (last_led == i) {
            if(i == path[index - 1] && index != 1) {
            index = index - 1;
            path[index] = -1;
            last_led = path[index - 1]; 
            if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
              purple_counter = purple_counter - 1;
            }
          }
          setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
          }
        } break;
                case 15: if (last_led == 11 || last_led == 14) {
                    if (!find_in_path(i)) {
                        setCol(leds[i], 150, 0, 0);
                        delay(800);
                        path[index] = i;
                        index = index + 1;
                        last_led = i;
                        if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
                            purple_counter = purple_counter + 1;
                        }
                    
                  }
                } else {
          if (last_led == i) {
            if(i == path[index - 1] && index != 1) {
            index = index - 1;
            path[index] = -1;
            last_led = path[index - 1]; 
            if (leds[i][3] == 150 && leds[i][4] == 0 && leds[i][5] == 150) {
              purple_counter = purple_counter - 1;
            }
          }
          setCol(leds[i], leds[i][3], leds[i][4], leds[i][5]);
          }
        } break;
                default: break; 
               
            }
          } //setCol(leds[i], 150, 0, 0);
 //         delay(800);
//          for(int u = 0; u < 16; u++) {
//            sen_state[u] = LOW;
//          }
//         delay(200);
        }
      }

      // Load time from accounts elapsed time and put it at the beginning 
      Serial.println();
      Serial.print("Timer: ");
      Serial.println(seconds);
      seconds = seconds + 1;

      lcd.setCursor(8, 1);
       lcd.print(String(seconds));

      delay(200);
      if (find_in_path(end_led) && purple_blocks == purple_counter) {

        int lvl = atoi(acc_info[0].c_str());
        int pts = atoi(acc_info[1].c_str());
        int rank = atoi(acc_info[2].c_str());
        int m_time = atoi(acc_info[3].c_str());
        if (lvl == LAST) {
          for (int f = 0; f < 16; f++) {
            setCol(leds[f], 0, 150, 0);
            delay(1000);
          }
        } else {
        lvl = lvl + 1;
        pts = pts + 100;
        m_time = m_time + seconds;
        rank = (pts/m_time) + 1;
        to_nodemcu = String(lvl) + "|" + String(pts) + "|" + String(rank) + "|" + String(m_time) + ")";
       // acc_done = 1;
        //acc_info[4] = {"", "", "", ""};
        //q = 0;
        Serial.println("YOU WIN");
       // resetFunc();

       // reset all global variables:
        done = 1;
        last_led = 0;
        end_led = 0;
        purple_blocks = 0; 
        purple_counter = 0;
        for (int m = 0; m < 16; m++) {
           path[m] = -1;
           //sen_state[m] = LOW;
        }
    for (int r = 0; r < 16; r++) {
      setCol(leds[r], 0, 0, 0);
    }
        index = 0;
        flag = 1;
        payload = "";
        j = 3; 
        num = "";
        val = -1;
        color = -1;
       delay(25);
       acc_done = 1;
        for (int n = 0; n < 4; n++) {
            acc_info[n] = "";
        }
        q = 0;
       num1 = "";
       seconds = 0;

       delay(10000);
       resetFunc();
        }
      }

      // this is here in order to reset the payload string (since we only need it once)
//      Serial.println(payload);
//      payload = "";
  
 delay(800);
}

void readSenState() {
    for (int k = 0; k < 16; k++) {
       for(int i = 0; i < 4; i++)
           digitalWrite(controlPin[i], muxChannel[k][i]); // muxChannel[sen_pins[k]][i]
       sen_state[k] = digitalRead(sig);
    }
}

void setCol(int* led, int r, int g, int b) {

    analogWrite(led[0], r);

    analogWrite(led[1], g);

    analogWrite(led[2], b);
    delay(25);
}
