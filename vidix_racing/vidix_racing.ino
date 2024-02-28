#include "Primitives.h"

#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

float x_coord = 30;
float y_coord = 31;
float rotation = 32;

// dolazne vrijednosti od drugog igraca
float ulazni_x_coord;
float ulazni_y_coord;
float ulazni_rotacija;

// pomocna varijabla, debug
String success;

typedef struct struct_message {
    float x_coord;
    float y_coord;
    float rotation;
} struct_message;

struct_message coords;

struct_message incomingReadings;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail"); //debug
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  //Serial.print("Bytes received: ");
  //Serial.println(len);
  ulazni_x_coord = incomingReadings.x_coord;
  ulazni_y_coord = incomingReadings.y_coord;
  ulazni_rotacija = incomingReadings.rotation;
}




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
int startSP;





// Constants for car movement
const float accelerationRate = 0.1;   // Adjust as needed
const float decelerationRate = 0.05;  // Adjust as needed
float maxSpeed = 2;                   // Maximum speed of the car
const float turnRate = 0.05;          // Rate of turning

// Variables to track car movement
static float speed = 0.00000001;
static float turnAngle = 0;


Object deblo = Object(&DEBLO);
Object krosnja = Object(&KROSNJA);
Object car = Object(&CUBE);
Object car2 = Object(&CUBE);

Camera camera = Camera();

void setup() {

  Serial.begin(115200);
  pinsetup();
  menuScreen();
  menuOptions = 0;
  startSP = 0;
  //EXAMPLE
  camera.position = { 0, 5, -1 };
  camera.rotation(0) = 0.5;
  car.scale = { 1, 1, 2 };
  car.position = { 0, 0, 0 };
  car.rotation(1) = 1.3;


  car2.scale = { 1, 1, 2 };
  car2.position = { -5, 0, -10 };
  car2.rotation(1) = 1.3;






  deblo.scale = { 1, 1, 1 };
  deblo.position = { -35, 0, -15 };
  krosnja.scale = { 1, 1, 1 };
  krosnja.position = { -35, 0, -15 };


  WiFi.mode(WIFI_STA);

  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // dodavanje protivnika    
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // funkcija koja se poziva kada dođu novi podatci
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

  if (startSP == 0) {
    menuselection();
    startpoint();
  } else {
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
      maxSpeed = 2;
    } else {
      camera.drawPolygon(stazaOuter, tft, ILI9341_RED);
      camera.drawPolygon(stazaInner, tft, ILI9341_RED);
      maxSpeed = 0.75;
    }


    //camera.drawObject(deblo, tft, ILI9341_BROWN);
    //camera.drawObject(krosnja, tft, ILI9341_GREEN);
    camera.drawObject(car, tft, ILI9341_WHITE);
    camera.drawObject(car2, tft, ILI9341_WHITE);
    delay(40);
    //camera.drawObject(deblo, tft, ILI9341_BLACK);
    //camera.drawObject(krosnja, tft, ILI9341_BLACK);
    camera.drawObject(car, tft, ILI9341_BLACK);
    camera.drawObject(car2, tft, ILI9341_BLACK);

    camera.drawPolygon(stazaInner, tft, ILI9341_BLACK);
    camera.drawPolygon(stazaOuter, tft, ILI9341_BLACK);

    if (digitalRead(PinTipkalo_A) == LOW) {

      speed += accelerationRate;
      if (speed > maxSpeed) {
        speed -= accelerationRate * 2;
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
        speed = 0;
      }
    }

    if (analogRead(PinTipkalo_L_R) > 4000) {
      car.rotation(1) += turnRate * speed;
    } else if (analogRead(PinTipkalo_L_R) > 1800 and analogRead(PinTipkalo_L_R) < 2000) {
      car.rotation(1) -= turnRate * speed;
    }

    //posalji svoje koordinate
    send_my_pos(car.position(2), car.position(0), car.rotation(1));

    //primjeni promjene na neprijatelju
    car2.position(2) = ulazni_x_coord;
    car2.position(0) = ulazni_y_coord;
    car2.rotation(1) = ulazni_rotacija;
  }
}









void menuScreen() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
}





void menuselection() {
  if (startSP == 0) {

    if (menuOptions == 0) {
      tft.setTextColor(ILI9341_WHITE);
      tft.setCursor(65, TFT_WIDTH / 6);  // polozaj pocetka ispisa tekst
        // velicina teksta
      tft.print("WELLCOME");
      tft.setCursor(65, TFT_WIDTH / 6 * 2);
      tft.print("SINGLEPLAYER");
      tft.setCursor(65, TFT_WIDTH / 6 * 3);
      tft.print("MULTIPLAYER");
    }



    if (digitalRead(PinTipkalo_A) == LOW) {
      Serial.print("a");
      menuOptions--;
      while (digitalRead(PinTipkalo_A) == LOW)
        ;
      if (menuOptions == 0) {
        menuOptions = 2;
      }
    }

    if (digitalRead(PinTipkalo_B) == LOW) {
      Serial.print("b");

      menuOptions++;
      while (digitalRead(PinTipkalo_B) == LOW)
        ;
      if (menuOptions == 3) {
        menuOptions = 1;
      }
    }

    if (menuOptions == 1) {
      tft.setTextColor(ILI9341_WHITE);

      tft.setCursor(65, TFT_WIDTH / 6 * 3);
      tft.print("MULTIPLAYER");
      tft.setTextColor(ILI9341_BLUE);
      tft.setCursor(65, TFT_WIDTH / 6 * 2);
      tft.print("SINGLEPLAYER");
      delay(5);
    }

    else if (menuOptions == 2) {
      tft.setTextColor(ILI9341_WHITE);
      tft.setCursor(65, TFT_WIDTH / 6 * 2);
      tft.print("SINGLEPLAYER");

      tft.setTextColor(ILI9341_BLUE);
      tft.setCursor(65, TFT_WIDTH / 6 * 3);
      tft.print("MULTIPLAYER");
      delay(5);
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
  if (startSP == 0) {
    if ((digitalRead(PinTipkalo_ST) == LOW) && (menuOptions == 1)) {
      startSP = 1;
      tft.fillScreen(ILI9341_BLACK);
    }
  }
}


void send_my_pos(float x_coord, float y_coord, float rotation){
  coords.x_coord = x_coord;
  coords.y_coord = y_coord;
  coords.rotation = rotation;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &coords, sizeof(coords));
}