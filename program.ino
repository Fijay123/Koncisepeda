#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
// ============== Untuk Inisialisasi ===============
SoftwareSerial ss(D5, D6);

FirebaseData firebaseData;
const char* ssid = "vivo 1918";
const char* password = "12345678";

#define kontak D7
#define stater D0

String kondisi_hidup, kondisi_mati;

TinyGPS gps;
float flat, flon;

void setup() {
  Serial.begin(9600);
  ss.begin(9600);

  pinMode(kontak, OUTPUT);
  pinMode(stater, OUTPUT);

  digitalWrite(kontak, HIGH);
  digitalWrite(stater, HIGH);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Sukses terkoneksi wifi!");
  Serial.println("IP Address:");
  Serial.println(WiFi.localIP());
  //========= link dan token database firebase ==========
  Firebase.begin("https://koncisepeda-8dd4b.firebaseio.com/", "hGYUPoNiu9AQiLXr3Vo9jshHFgqkpppB3pFk4OTx");
  Firebase.setString(firebaseData, "/kontrol/kondisi", "mati");
  set_lat_long();
}

unsigned long milll;
//============ Pembacaan sensor GPS ===============
void loop() {
  read_gps();
  read_kondisi();
  if (Firebase.getString(firebaseData, "/kontrol/kondisi")) { // misal database diberikan nama relay1
    if  (firebaseData.dataType() == "string")
    {
      String Status = firebaseData.stringData();
      //====== konfigurasi relay =======
      Serial.println(Status);
      if (Status == kondisi_hidup)
      {
        digitalWrite(kontak, LOW);
        delay(3000);
        digitalWrite(stater, LOW);
        delay(1000);
        digitalWrite(kontak, LOW);
        digitalWrite(stater, HIGH);
        Firebase.setString(firebaseData, "/kontrol/kondisi", "hidup");
        while (1) {
          read_gps();
          read_kondisi();
          digitalWrite(kontak, LOW);
          digitalWrite(stater, HIGH);
          if (Firebase.getString(firebaseData, "/kontrol/kondisi")) { //misal database diberikan nama relay1
            if  (firebaseData.dataType() == "string")
            {
              String Status = firebaseData.stringData();
              Serial.println(Status);
              if (Status == kondisi_mati)
              {
                Firebase.setString(firebaseData, "/kontrol/kondisi", "mati");
                break;
              }
            }
          }
        }
      }
      else {
        digitalWrite(kontak, HIGH);
        digitalWrite(stater, HIGH);

      }
    }
  }
}
//========== pembacaan GPS ============
void read_gps() {
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    if (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // hilangkan koment jika mau melihat data yang dikirim dari modul GPS
      if (gps.encode(c)) // Parsing semua data
        newData = true;
    }
  }
  //======== pengambilan titik lokasi ===============
    if (newData)
  {
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());

    Firebase.setString(firebaseData, "/Location/latitude/-M7GcF1T_-qoXjpcGdhC", String(flat));
    Firebase.setString(firebaseData, "/Location/longitude/-M7GcF1WKGZsNz0wQejf", String(flon));
  }

  gps.stats(&chars, &sentences, &failed);
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0)    Serial.println("** Tidak ada Data Masuk, Periksa Wiring **");

}
void set_lat_long() {
  if (Firebase.getString(firebaseData, "/Location/latitude/-M7GcF1T_-qoXjpcGdhC")) {
    if  (firebaseData.dataType() == "string")
    {
      flat = firebaseData.stringData().toFloat();
    }
  }
  if (Firebase.getString(firebaseData, "/Location/longitude/-M7GcF1WKGZsNz0wQejf")) {
    if  (firebaseData.dataType() == "string")
    {
      flon = firebaseData.stringData().toFloat();
    }
  }
  Serial.println("data= " + String(flat, 8) + "," + String(flon, 8));
}
void read_kondisi() {
  if (Firebase.getString(firebaseData, "/seting/hidup")) { // misal database diberikan nama relay1
    if  (firebaseData.dataType() == "string")
    {
      String Status = firebaseData.stringData();
      if (kondisi_hidup != Status) {
        kondisi_hidup = Status;
      }
    }
  }
  if (Firebase.getString(firebaseData, "/seting/mati")) { // misal database diberikan nama relay1
    if  (firebaseData.dataType() == "string")
    {
      String Status = firebaseData.stringData();
      if (kondisi_hidup != Status) {
        kondisi_mati = Status;
      }
    }
  }
}
