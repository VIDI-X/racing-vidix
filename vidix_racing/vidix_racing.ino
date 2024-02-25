#include "Primitives.h"

int PinTipkalo_L_R = 34;
int PinTipkalo_U_D = 35;
int PinTipkalo_B = 33;
int PinTipkalo_A = 32;
int PinTipkalo_SEL = 27;
int PinTipkalo_VOL = 0;
int PinTipkalo_ST = 39;
int PinTipkalo_MEN = 13;
// ILI9341 TFT LCD deklaracija pinova
#define TFT_CS 5
#define TFT_DC 21
// stvaranje objekta ekrana koji će se zvati tft
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
int menuOptions;
int start;


// Constants for car movement
const float accelerationRate = 0.1;   // Adjust as needed
const float decelerationRate = 0.05;  // Adjust as needed
float maxSpeed = 2;             // Maximum speed of the car
const float turnRate = 0.01;          // Rate of turning

// Variables to track car movement
static float speed = 0.00000001;
static float turnAngle = 0;





//EXAMPLE
Object car = Object(&CUBE);
Camera camera = Camera();
//

void setup() {
  Serial.begin(9600);
  pinsetup();
  menuScreen();
  menuOptions = 0;
  start = 1;

  //EXAMPLE
  camera.position = { 0, 5, -5 };
  camera.rotation(0) = 0.5;
  car.scale = { 1, 1, 2 };
  //
}

void loop() {
  menuselection();
  startpoint();

  Matrix<4> forward = { 0, 0, 1, 0 };
  //Serial.println(speed);
  forward = Normalize(car.getObjectToWorldMatrix() * forward);
  //float mag = sqrt(forward(0) * forward(0) + forward(1) * forward(1) + forward(2) * forward(2));
  //forward /= mag;

  Matrix<3> cameraOffset = { forward(0) * -5, forward(1) * -5, forward(2) * -5 };
  
  camera.position = car.position + cameraOffset;
  camera.position(1) += 5;
  camera.rotation(1) = car.rotation(1);

  forward *= speed;

  if (stazaOuter.inside({ car.position(0), car.position(2) }) & !stazaInner.inside({ car.position(0), car.position(2) })) {
    camera.drawPolygon(stazaOuter, tft, ILI9341_BLUE);
    camera.drawPolygon(stazaInner, tft, ILI9341_BLUE);
    maxSpeed=2;
  } else {
    camera.drawPolygon(stazaOuter, tft, ILI9341_RED);
    camera.drawPolygon(stazaInner, tft, ILI9341_RED);
    maxSpeed=0.75;
  }

  camera.drawObject(car, tft, ILI9341_WHITE);
  delay(40);
  camera.drawObject(car, tft, ILI9341_BLACK);
  camera.drawPolygon(stazaInner, tft, ILI9341_BLACK);
  camera.drawPolygon(stazaOuter, tft, ILI9341_BLACK);

  if (digitalRead(PinTipkalo_A) == LOW) {

    speed += accelerationRate;
    if (speed > maxSpeed) {
      speed -= accelerationRate*2;
    }
  } else {
    speed -= decelerationRate;
    if (speed < 0) {
      speed = 0.00000001;
    }
  }

  car.position(2) += forward(2);
  car.position(1) += forward(1);
  car.position(0) += forward(0);

  if (digitalRead(PinTipkalo_B) == LOW) {
    speed -= decelerationRate * 1.5;
    if (speed < 0) {
      speed = 0.00000001;
    }
  }

  if (analogRead(PinTipkalo_L_R) > 4000) {
    car.rotation(1) += 0.2;
  } else if (analogRead(PinTipkalo_L_R) > 1800 and analogRead(PinTipkalo_L_R) < 2000) {
    car.rotation(1) -= 0.2;
  }
}

void menuScreen() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
}

void menuselection() {
  if (start == 0) {
    tft.setCursor(65, 60);  // polozaj pocetka ispisa tekst
    tft.setTextSize(4);     // velicina teksta
    tft.print("WELLCOME");
    tft.setCursor(65, 120);
    tft.print("PLAY");
    tft.setCursor(65, 180);
    tft.print("QUIT");
    if (analogRead(PinTipkalo_U_D) > 4000) {
      menuOptions--;
      if (menuOptions == 0) {
        menuOptions = 2;
      }
    }

    else if (analogRead(PinTipkalo_U_D) > 1900 and analogRead(PinTipkalo_U_D) < 2000) {
      menuOptions++;
      if (menuOptions == 3) {
        menuOptions = 1;
      }
    }

    if (menuOptions == 1) {
      tft.setTextColor(ILI9341_WHITE);
      tft.setCursor(65, 180);
      tft.print("QUIT");
      tft.setTextColor(ILI9341_BLUE);
      tft.setCursor(65, 120);
      tft.print("PLAY");
      delay(500);
    }

    else if (menuOptions == 2) {
      tft.setTextColor(ILI9341_WHITE);
      tft.setCursor(65, 120);
      tft.print("PLAY");
      tft.setTextColor(ILI9341_BLUE);
      tft.setCursor(65, 180);
      tft.print("QUIT");
      delay(500);
    }
  }
}

void pinsetup() {
  pinMode(PinTipkalo_L_R, INPUT_PULLUP);
  pinMode(PinTipkalo_U_D, INPUT_PULLUP);
  pinMode(PinTipkalo_B, INPUT_PULLUP);
  pinMode(PinTipkalo_A, INPUT_PULLUP);
  pinMode(PinTipkalo_SEL, INPUT_PULLUP);
  pinMode(PinTipkalo_VOL, INPUT_PULLUP);
  pinMode(PinTipkalo_ST, INPUT_PULLUP);
  pinMode(PinTipkalo_MEN, INPUT_PULLUP);
}

void startpoint() {
  if (start == 0) {
    if ((digitalRead(PinTipkalo_A) == LOW) && (menuOptions == 1)) {
      start = 1;
      tft.fillScreen(ILI9341_BLACK);
    }
  }
}