#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

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
int menueoptions;
int start;

void setup() {
  pinsetup();
  menuescreen();
  menueoptions = 0;
  start = 0;
}

void loop() {
  menueselection();
  startpoint();
}

void menuescreen() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(65, 60);  // polozaj pocetka ispisa tekst
  tft.setTextSize(4);     // velicina teksta
  tft.print("WELLCOME");
  tft.setCursor(65, 120);
  tft.print("PLAY");
  tft.setCursor(65, 180);
  tft.print("QUIT");
}

void menueselection() {
  if (start == 0) {
    if (analogRead(PinTipkalo_U_D) > 4000) {
      menueoptions--;
      if (menueoptions == 0) {
        menueoptions = 2;
      }
    }

    else if (analogRead(PinTipkalo_U_D) > 1900 and analogRead(PinTipkalo_U_D) < 2000) {
      menueoptions++;
      if (menueoptions == 3) {
        menueoptions = 1;
      }
    }

    if (menueoptions == 1) {
      tft.setTextColor(ILI9341_WHITE);
      tft.setCursor(65, 180);
      tft.print("QUIT");
      tft.setTextColor(ILI9341_BLUE);
      tft.setCursor(65, 120);
      tft.print("PLAY");
      delay(500);
    }

    else if (menueoptions == 2) {
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
    if ((digitalRead(PinTipkalo_A) == LOW) && (menueoptions == 1)) {
      start = 1;
      tft.fillScreen(ILI9341_WHITE);
    }
  }
}
