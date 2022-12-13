#include <Arduino.h>                                // Thu vien giup viet code Arduino tren VS Code

#include <ESP8266WiFi.h>                            // Thu vien giup ESP8266 ket noi WiFi
#define wifiSSID "VIETphone"
#define wifiPASS "12345678"

#include <FirebaseESP8266.h>                        // Thu vien giup ESP8266 ket noi Firebase thong qua internet
FirebaseData fbdb;                                      // Tao class chua data tu Firebase

#include <MQ135.h>                                  // Thu vien giup giao tiep voi sensor MQ135
#define PIN_MQ135 A0                              //must analog port
MQ135 mq135_sensor = MQ135(A0);                  // Thiet lap ket noi MQ135 voi chan analog cua ESP8266
float mq135_temperature = 20.0;
float mq135_humidity = 40.0;

void connectWiFi() {                                    // Ham ket noi WiFi
    Serial.println("Connecting to WiFi...");
    WiFi.begin(wifiSSID, wifiPASS);
    while(!(WiFi.status() == WL_CONNECTED)) 
    {
        delay(500);
        Serial.print("...");
    }
    Serial.println("");
    Serial.println("Successfully connected");
}

void connectFirebase() {
   Firebase.begin("https://iot-co2-729a7-default-rtdb.asia-southeast1.firebasedatabase.app/",  "qDY2Y2XXp8lFz46vzbBUbe0fIu3ty9XrdxMtz2yd");
}

void setup() {
  Serial.begin(115200);
  connectWiFi();
  connectFirebase();
}

void loop() {
    //doc gia tri sensor MQ135

  //float approxedPPM = mq135_sensor.getPPM();
  // Serial.print("PPM: "); 
  // Serial.print(approxedPPM);

  float rZero = mq135_sensor.getRZero();
  float ppmSelf = PARA * pow((mq135_sensor.getCorrectedResistance(mq135_temperature, mq135_humidity)/rZero), -PARB);
  Serial.print("PPM calculate myself: ");
  Serial.println(ppmSelf);

  // float correctedPPM = mq135_sensor.getCorrectedPPM(mq135_temperature, mq135_humidity);
  // Serial.print("Corrected PPM: ");
  // Serial.print(correctedPPM);
  // Serial.println(" ppm");
  delay(500);
  
  // push len firebase gia tri sensor sau moi 5s = 0.5s + 4.5s
  // trong thuc te thi khong can 5s, co the la 1h

  // Firebase.pushFloat(fbdb, "CO2_quality", correctedPPM);
  // Firebase.setFloat(fbdb, "CO2_RealtimeValue/realtimeValue", correctedPPM);

  Firebase.pushFloat(fbdb, "CO2_quality", ppmSelf);
  Firebase.setFloat(fbdb, "CO2_RealtimeValue/realtimeValue", ppmSelf);

  delay(4500);

}