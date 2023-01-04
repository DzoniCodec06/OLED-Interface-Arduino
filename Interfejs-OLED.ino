// Program for progress bar using OLED and Rotary Encoder 

// Libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

// Variables

// Rotary Encoder
// Pins
const int CLK = 5;
const int DT = 4;
const int SW = 3;
// Values
int currentStateCLK;
int lastStateCLK;
int stateDT;
int buttonPressed;
int counter = 0;

// OLED
Adafruit_SSD1306 display(128, 64, &Wire, -1);
// Drawing line
const int PROGMEM start_pos = 10;

// Buzzer pin
const int PROGMEM buzzer = 6;


void draw_select(int x, int y) { // y coordinate depends on what position your text is /SR y koordinata zavisi od pozicije texta koga zelite da oznacite
  display.drawLine(x, y, x+10, y+5, WHITE);
  display.drawLine(x+10, y+5, x, y+10, WHITE);
}

void draw_menu() {
  display.setCursor(40, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("MENU");
  display.setCursor(40, 25);
  display.print("Time");
  display.setCursor(40, 45);
  display.print("Power"); 
  if (counter == 0) draw_select(start_pos, 25); 
  else if (counter == 1) draw_select(start_pos, 45); 
}

void setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) for(;;);
  Serial.begin(9600);
  delay(100);
  display.clearDisplay();
  draw_menu();
  display.display();
  
  // Encoder
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  // Buzzer
  pinMode(buzzer, OUTPUT);

  noTone(buzzer);

  lastStateCLK = digitalRead(CLK);
}

void loop() {
  currentStateCLK = digitalRead(CLK); 

  if (currentStateCLK != lastStateCLK && currentStateCLK == HIGH) {
    
    stateDT = digitalRead(DT);
    
    if (stateDT == currentStateCLK) {       // CCW Rotation ( Decresing  -- ) 
      display.clearDisplay();
      if (counter == 1) counter = 0;
      else counter = 0;
      draw_menu();
      display.display(); 
    } else if (stateDT != currentStateCLK) {  // CW Rotation ( Increasing  ++ ) 
      display.clearDisplay();
      if (counter == 0) counter = 1;
      else counter = 1;
      draw_menu();
      display.display(); 
    }
  }
  lastStateCLK = currentStateCLK;

  buttonPressed = digitalRead(SW);
  
  if (buttonPressed == LOW){
    tone(buzzer, 1000);
    Serial.println("Pressed");
    delay(75);
    noTone(buzzer);
    delay(150);
  } 
  
  delay(1);
}
