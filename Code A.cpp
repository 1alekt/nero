#include "Firebase_Arduino_WiFiNINA.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#define DATABASE_URL "nero01-21684-default-rtdb.firebaseio.com"
#define DATABASE_SECRET "ob9Ae9cy05mEE4Amxipb4EhOqby3F4IGOYcZy3Al"
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
OneWire ourWire(2); // Se establece el pin 2 como bus OneWire
DallasTemperature sensors(&ourWire);
const int analogInPin = A0;
int buf[10];
int temp = 0;
unsigned long int inValue;

String path = "/datos";
String jsonStr;
FirebaseData fbdo;
void dataToFirebase(float temp, float ph) {
  String tempStr = String(temp);
  String phStr = String(ph);
  // Construimos el JSON usando las variables tempStr y phStr
  String jsonStr = "{\"ph\":\"" + phStr + "\",\"temp\":\"" + tempStr + "\"}";

  if (Firebase.updateNode(fbdo, path +  fbdo.pushName(), jsonStr)) {
    Serial.println(F("ok"));
    Serial.print(F("path: "));
    Serial.println(fbdo.dataPath());
    Serial.print(F("type: "));
    Serial.println(fbdo.dataType());
    Serial.print(F("value: "));
  } else {
    Serial.print(F("error, "));
    Serial.println(fbdo.errorReason());
  }
  Serial.println();
  fbdo.clear();
}
void setup() {
  // put your setup code here, to run once:
  delay(1000);
 Serial.begin(9600);
 sensors.begin();
 Firebase.begin(DATABASE_URL, DATABASE_SECRET, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);
  
}
void loop() {
  // put your main code here, to run repeatedly:
sensors.requestTemperatures(); // Se envía el comando para leer la temperatura
  float tempC = sensors.getTempCByIndex(0); // Se obtiene la temperatura en ºC
  Serial.print("Temperatura = ");
  Serial.print(tempC);
  Serial.println(" °C");
  delay(100);
  // Para evitar demasiadas variaciones, leemos varios valores y luego sacamos el promedio
  for (int i = 0; i < 10; i++) {
    buf[i] = analogRead(analogInPin);
    delay(10);
  }
  // Luego realizamos un ordenamiento ascendente de los valores leídos
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buf[j] < buf[i]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  // Realizamos el cálculo del promedio y la conversión a voltaje en mV
  inValue = 0;
  for (int i = 2; i < 8; i++) {
    inValue += buf[i];
  }
  float pHVol = (float)inValue * 100.0 * 5.0 / 1024.0 / 6.0;
  Serial.print("Voltaje mV = ");
  Serial.println(pHVol, 2); // Mostrar solo 2 decimales
  float pH = -0.0554 * pHVol + 22.236;
  Serial.print("pH = ");
  Serial.println(pH, 2); // Mostrar solo 2 decimales
  delay(100);
 dataToFirebase( tempC, pH);
 delay(5000);
}