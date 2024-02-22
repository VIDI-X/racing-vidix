#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include "Engine.h"

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

//EXAMPLE
int numOfVerts = 3, numOfTris = 1;
Matrix<3> verteces[] = { { -10, -10, 0 }, {10, -10, 0}, {0, 10, 0} };
Matrix<3, 1, int> triangles[] = { { 0, 1, 2 } };
Matrix<2, 100, float> transformedVertices;
Object triangle = Object(numOfVerts, verteces, numOfTris, triangles);
Camera camera = Camera();
//

void setup() {
  Serial.begin(9600);
  pinsetup();
  menuescreen();
  menueoptions = 0;
  start = 1;

  //EXAMPLE
  camera.position = {120, 160, 10};
  //
}

void loop() {
  triangle.rotation(1) += 0.1;
  triangle.position(0) = 10;
  triangle.position(1) = 10;
  //triangle.scale = (float)10;
  menueselection();
  startpoint();

  //EXAMPLE
  if (start == 1) {
    // Matrix<3> verts[triangle.numOfVerts];
    for (int i = 0; i < triangle.numOfVerts; i++) {
      // Dobivanje pozicije vrha u svemirskoj koordinati
      Matrix<4> verticeHomogeneous = { triangle.verteces[i](0), triangle.verteces[i](1), triangle.verteces[i](2), 1 };
      Matrix<4> transformedVertice = triangle.getObjectToWorldMatrix() * verticeHomogeneous;

      // Primjena perspektivne transformacije
      Matrix<4> projectedVertice = camera.getOrthoMat(tft.width(), tft.height()) * transformedVertice;

      // Normalizacija koordinata
      //projectedVertice /= projectedVertice(3);
      projectedVertice *= (float)1000.0;


      transformedVertices(0, i) = projectedVertice(0);
      transformedVertices(1, i) = projectedVertice(1);

      // Crtanje točke na ekranu
      tft.drawPixel(projectedVertice(0), projectedVertice(1), ILI9341_BLUE);
     
      Serial.println(projectedVertice(0));
      Serial.println(projectedVertice(1));
      
    }

    tft.fillScreen(ILI9341_BLACK);

tft.fillTriangle(transformedVertices(0,0), transformedVertices(1,0), transformedVertices(0,1), transformedVertices(1,1), transformedVertices(0,2), transformedVertices(1,2), ILI9341_WHITE);

   



    if (analogRead(PinTipkalo_U_D) > 4000) {
      camera.position(2)++;
    }

    else if (analogRead(PinTipkalo_U_D) > 1800 and analogRead(PinTipkalo_U_D) < 2000) {
     camera.position(2)--;
    }
if (analogRead(PinTipkalo_L_R) > 4000) {
      camera.position(1)++;
    }

    else if (analogRead(PinTipkalo_L_R) > 1800 and analogRead(PinTipkalo_L_R) < 2000) {
     camera.position(1)--;
    }

  }


  //
}

void menuescreen() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
}

void menueselection() {
  if (start == 0) {
    tft.setCursor(65, 60);  // polozaj pocetka ispisa tekst
    tft.setTextSize(4);     // velicina teksta
    tft.print("WELLCOME");
    tft.setCursor(65, 120);
    tft.print("PLAY");
    tft.setCursor(65, 180);
    tft.print("QUIT");
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
      tft.fillScreen(ILI9341_BLACK);
    }
  }
}
