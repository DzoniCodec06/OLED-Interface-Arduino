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

bool selected = false;
bool sound = true;

void draw_select(int x, int y) { // y coordinate depends on what position your text is /SR y koordinata zavisi od pozicije texta koga zelite da oznacite
  display.drawLine(x, y, x+10, y+5, WHITE);
  display.drawLine(x+10, y+5, x, y+10, WHITE);
}

void draw_menu() {
  display.setCursor(40, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("MENU");
   
  if (counter == 0){
    draw_select(start_pos, 25); 
    draw_items();
  }
  else if (counter == 1){
    draw_select(start_pos, 45); 
    draw_items();
  }
  else if (counter == 2) {
     draw_select(start_pos, 45); 
     draw_items();
  }
}

void draw_items() {
  if (counter < 2) {
    display.setCursor(40, 25);
    display.print("Time");
    display.setCursor(40, 45);
    display.print("Power");
  } else {
    display.setCursor(40, 25);
    display.print("Power");
    display.setCursor(40, 45);
    display.print("Buzzer");
  }
}

void disable_buzzer() {
  if (counter == 0) {
    display.clearDisplay();
    display.setCursor(20, 0);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.print("Disable");
    display.fillRect(15, 25, 45, 25, WHITE);
    display.setCursor(20, 30);
    display.setTextColor(BLACK);
    display.print("Yes");
    display.setCursor(80, 30);
    display.setTextColor(WHITE);
    display.print("No");
    display.display();  
  } else if (counter == 1) {
    display.clearDisplay();
    display.setCursor(20, 0);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.print("Disable");
    display.setCursor(20, 30);
    display.print("Yes");
    display.fillRect(75, 25, 30, 25, WHITE);
    display.setCursor(80, 30);
    display.setTextColor(BLACK);
    display.print("No");
    display.display();  
  }
  delay(10);
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
      if (!selected) {
        display.clearDisplay();
        if (counter > 0) counter -= 1;
        draw_menu();
        display.display(); 
      } else {
        if (counter == 1) counter = 0;
        disable_buzzer();
      }
    } else if (stateDT != currentStateCLK) {  // CW Rotation ( Increasing  ++ ) 
      if (!selected) {
        display.clearDisplay();
        if (counter < 2) counter += 1;
        draw_menu();
        display.display(); 
      } else {
        if (counter == 0) counter = 1;
        disable_buzzer();
      }
    }
  }
  lastStateCLK = currentStateCLK;

  buttonPressed = digitalRead(SW);
  
  if (buttonPressed == LOW){
    if (sound) tone(buzzer, 1000);
    Serial.println("Pressed");
    delay(75);
    noTone(buzzer);
    delay(150);
    if (counter == 2) {
      selected = true;
      counter = 0;
      disable_buzzer();
    } else if (selected && counter == 0) {
        selected = false;
        sound =  false;
        display.clearDisplay();
        draw_menu();
        display.display();
        return;
    } else if (selected && counter == 1) {
        counter = 0;
        selected = false;
        sound = true;
        display.clearDisplay();
        draw_menu();
        display.display();
        return;
    }
  } 
  delay(1);
}
