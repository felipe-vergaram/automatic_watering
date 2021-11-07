#include <WiFi.h>
#include <HTTPClient.h>

typedef struct
  {
    int sensPin;
    int valvePin;
    float humidity;
    int valveStatus;
  }Sector;

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";
const char* UBIDOTS_TOKEN = "your_ubidots_token";
const char* serverName = "http://things.ubidots.com/api/v1.6/devices/riego";

char buff[80];
char buff_float[10];
int n_sectors = 1;
int humidity_threshold = 40; // Ajustar este valor empíricamente
int DELAY_TIME = 60*1000;

// Definir sectores de riego, en mi caso solo hay uno
Sector sect_arr[] = {
  {33, 4, 0.0, 0}
};


void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  for (int i=0; i<n_sectors; i++){
    pinMode(sect_arr[i].sensPin, INPUT);
    pinMode(sect_arr[i].valvePin, OUTPUT);
  }

}

void readSensor(Sector *sect){
  // Leer la temperatura de sect->sensPin y guardarla en sect->humidity
  // ESP32 tiene resolución de 12 bits, por lo que entrega entre 0 y 4095
  int reading = analogRead(sect->sensPin);
  sect->humidity = ((float)(4095-reading))*100/4095;
}

void updateValveStatus(Sector *sect){
  // Definir si la valvula solenoide debe estar encendida o apagada
  if (sect->humidity > humidity_threshold){
    sect->valveStatus = 0;
  }
  else{
    sect->valveStatus = 1;
  }
  //digitalWrite(sect->valvePin, sect->valveStatus);
}

void printSectorStatus(Sector *sect){
  dtostrf(sect->humidity, 6, 2, buff_float);
  sprintf(buff, "sensPin: %2i    valvePin: %2i    humidity: %s    valveStatus: %i", sect->sensPin, sect->valvePin, buff_float, sect->valveStatus);
  Serial.println(buff);
}

void postToUbidots(Sector sect_arr[], int n){
  if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);
      http.addHeader("Content-Type", "application/json");
      http.addHeader("X-Auth-Token", UBIDOTS_TOKEN);
      char http_buff[80] = "{";

      for (int i=0; i<n_sectors; i++){
        if (sect_arr[i].humidity < 10){
          dtostrf(sect_arr[i].humidity, 4, 2, buff_float);
        }
        else if (sect_arr[i].humidity <100){
          dtostrf(sect_arr[i].humidity, 5, 2, buff_float);
        }
        else{
          dtostrf(sect_arr[i].humidity, 6, 2, buff_float);
        }

        sprintf(buff, "\"sector%i\":\"%s\"", i, buff_float);
        strcat(http_buff, buff);
        if (i < n_sectors-1){
          strcat(http_buff, ",");
        }
      }
      strcat(http_buff, "}");
      //Serial.println(http_buff);

      int httpResponseCode = http.POST(http_buff);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }

}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i=0; i<n_sectors; i++){
    readSensor(&sect_arr[i]);
    updateValveStatus(&sect_arr[i]);
    printSectorStatus(&sect_arr[i]);
  }
  postToUbidots(sect_arr, n_sectors);

  delay(DELAY_TIME);

}
