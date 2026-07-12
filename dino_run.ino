#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool gameover = false;
const int buttonpin = 18;
int playerY = 1;
bool gamestart = true;

#define BUZZER 19
const int melodyFreq[]   = {523, 659, 784, 659, 784, 1047, 880, 988, 784};
const int melodyDur[]    = {120, 120, 120, 100, 100,  250, 150, 150, 300};
const int totalNotes     = 9;
int currentNoteIndex     = 0;
unsigned long noteStartTime = 0;
bool isNotePlaying       = false;



byte player[] = { //meslox is great and powerful
  B00110,
  B01101,
  B00111,
  B00110,
  B10111,
  B11111,
  B01111,
  B00101
};

byte enemy[] = {
  B01110,
  B01110,
  B00100,
  B01110,
  B10101,
  B00100,
  B01010,
  B10001  
};

byte play[] = {
  B00000,
  B01000,
  B01100,
  B01110,
  B01100,
  B01000,
  B00000,
  B00000
};

void playMusicStep() {
  unsigned long currentMillis = millis();
  
  if (!isNotePlaying) {
    ledcWriteTone(BUZZER, melodyFreq[currentNoteIndex]);

    ledcWrite(BUZZER, 12);
    
    noteStartTime = currentMillis;
    isNotePlaying = true;
  } 
  else {
    if (currentMillis - noteStartTime >= (unsigned long)melodyDur[currentNoteIndex]) {
      ledcWriteTone(BUZZER, 0);
      
      
      if (currentMillis - noteStartTime >= (unsigned long)(melodyDur[currentNoteIndex] + 20)) {
        isNotePlaying = false;
        currentNoteIndex++; 
        
        if (currentNoteIndex >= totalNotes) {
          currentNoteIndex = 0;
        }
      }
    }
  }
}

void playTone(int freq, int durationMs) {

  ledcWriteTone(BUZZER, freq);
  delay(durationMs);
}



void adjust(int &posx_a ,int &posy_a,int posx_b ,int posx_c,int posx_d,int posx_e,int posx_f,int &score){

      if (posx_a < 0){ 
      posx_a = (random(0, 2) == 0) ? 18 : 17;
      posy_a = random(0,2);
      score++;


      if (posx_a == posx_b - 1 ||
      posx_a == posx_c - 1 ||
      posx_a == posx_d - 1 ||
      posx_a == posx_e - 1 ||
      posx_a == posx_f- 1 ) posx_a +=1;

      if (posx_a == posx_b + 1 ||
      posx_a == posx_c + 1 ||
      posx_a == posx_d + 1 ||
      posx_a == posx_e + 1 ||
      posx_a == posx_f + 1 ) posx_a -=1;


      if (posx_a == posx_b || 
      posx_a == posx_c ||
      posx_a == posx_d ||
      posx_a == posx_e ||
      posx_a == posx_f){
        if (posx_a == 18){
          posx_a = 17;
          }else posx_a = 18;
        }
    }
  
  }

int main() {

  lcd.createChar(1,player);
  lcd.createChar(2,enemy);
  lcd.createChar(3, play);

  int value = (random(0, 2) == 0) ? 45 : 43;
  int posX1 = 15;
  int posX2 = (random(0, 2) == 0) ? 18 : 17;
  int posX3 = (random(0, 2) == 0) ? 21 : 20;
  int posX4 = (random(0, 2) == 0) ? 24 : 23;
  int posX5 = (random(0, 2) == 0) ? 27 : 26;
  int posX6 = (random(0, 2) == 0) ? 30 : 29;

  int posY1 = random(0,2);
  int posY2 = random(0,2);
  int posY3 = random(0,2);
  int posY4 = random(0,2);
  int posY5 = random(0,2);
  int posY6 = random(0,2);  //lucy is great

  int playerX = 3;
  int score = 0;

  int speed = 300;
     

  while (!gameover) {
    //Serial.begin(115200);
    //Serial.println("ok");

    if (digitalRead(buttonpin) == LOW){
        playerY = 0;
        Serial.println("low");
      }else{
          playerY = 1;
          Serial.println("high");
      }

    playMusicStep();

    // move obstacles
    posX1--;
    posX2--;
    posX3--;
    posX4--;
    posX5--;
    posX6--;

    adjust(posX1,posY1,posX2,posX3,posX4,posX5,posX6,score);
    
    adjust(posX2,posY2,posX1,posX3,posX4,posX5,posX6, score);
    
    adjust(posX3,posY3,posX2,posX1,posX4,posX5,posX6, score);
    
    adjust(posX4,posY4,posX2,posX3,posX1,posX5,posX6, score);
    
    adjust(posX5,posY5,posX2,posX3,posX4,posX1,posX6, score);

    adjust(posX6,posY6,posX2,posX3,posX4,posX5,posX1, score);



    if ((playerX == posX2 && playerY == posY2) ||
        (playerX == posX4 && playerY == posY4) ||
        (playerX == posX6 && playerY == posY6)) {

      gameover = true;
      ledcWriteTone(BUZZER, 0);
      playTone(900, 180);
      playTone(700, 250);

      ledcWriteTone(BUZZER, 0);

    }

    if ((playerX == posX1 && playerY == posY1) ||
        (playerX == posX3 && playerY == posY3) ||
        (playerX == posX5 && playerY == posY5)) {

      gameover = true;
      ledcWriteTone(BUZZER, 0);
      playTone(900, 180);
      playTone(700, 250);

      ledcWriteTone(BUZZER, 0);
    }
    
    

    lcd.clear();

    // draw obstacles
    lcd.setCursor(posX1, posY1);
    lcd.write(2);

    lcd.setCursor(posX2, posY2);
    lcd.write(2);

    lcd.setCursor(posX3, posY3);
    lcd.write(2);;

    lcd.setCursor(posX4, posY4);
    lcd.write(2);

    lcd.setCursor(posX5, posY5);
    lcd.write(2);

    lcd.setCursor(posX6, posY6);
    lcd.write(2);

    lcd.setCursor(playerX, playerY);
    //lcd.print("O");
    lcd.write(1);

    //lcd.setCursor(0,0);
    //lcd.print(String(score));

    if (score == 12) speed = 200;
    if (score == 20) speed = 180;
    if (score == 30) speed = 150;
    if (score == 50) speed = 130;
    if (score == 80) speed = 100;
    
    

    delay(speed);
    
  }
  return score;
}

void setup() {
  Wire.begin(21, 22);
  pinMode(buttonpin, INPUT_PULLUP);
  randomSeed(analogRead(0));

  lcd.init();
  lcd.backlight();
  //lcd.createChar(1,player);
  Wire.begin(21, 22);
  pinMode(buttonpin, INPUT_PULLUP);
  randomSeed(analogRead(0));

  // Initialize the audio pin for ESP32
  ledcAttach(BUZZER, 2000, 8);

  ledcAttach(BUZZER, 2000, 8);

  lcd.init();
  lcd.backlight();


  ledcAttach(BUZZER, 2000, 8);
  
  ledcWriteTone(BUZZER, 1650);
  delay(180);

  ledcWriteTone(BUZZER, 2100);
  delay(280);
  
  ledcWriteTone(BUZZER, 0);

  for (int pos = 0 ;pos <= 4; pos++){
    lcd.clear();
    lcd.setCursor(pos, 0);
    lcd.print("CHAINSAW");
    delay(600);

  }
  lcd.setCursor(2,1);
  lcd.print("Entertainment");
  
  delay(2000);


}

void loop() {
  int finalscore;





  lcd.clear();
  
  lcd.setCursor(4, 0);
  lcd.print("DINO RUN");
  lcd.setCursor(1, 1);
  lcd.write(1);
  lcd.print("     ");
  //lcd.setCursor(1, 2);
  lcd.write(3);
  while(true){
      if (digitalRead(buttonpin) == LOW ){

        for(int pos = 0 ;pos <= 3;pos++ ){
              lcd.setCursor(1, pos);
              lcd.write(1);
          }
        gamestart = false;
        break;
      }
    }
  
  if (!gameover && !gamestart) {
    finalscore = main();
   
  }

  
  if (!gamestart){
    ledcWriteTone(BUZZER, 0);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("GAME OVER!");
    lcd.setCursor(3, 1);
    lcd.print("Score: " + String(finalscore)+" ");
    lcd.write(1);
    delay(1000);
    if (digitalRead(buttonpin) == LOW ) gameover = false;
  
    while (true){
      if (digitalRead(buttonpin) == LOW ){
        gameover = false;
        break;
      }
      
      }
  }
}
