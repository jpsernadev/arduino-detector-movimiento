#include <RTClib.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

// Funciones
void getDateTime();

// Sensor
int detectPin = 2;
bool detect = false;
int led = 3;
int deactivatedLed = 4;
bool active = true;   // Para desactivar sensor con RFID

// RTC
int segundo,minuto,hora,dia,mes;
long anio; //variable año
DateTime HoraFecha;

// Funciones
void readDateTime();
void detection();

RTC_DS1307 rtc;
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);

  SPI.begin();        // Iniciar SPI bus
  mfrc522.PCD_Init();  // Iniciar MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Mostras detalles PCD - MFRC522 Card Reader

  rtc.begin();    // Inicializar RTC
  // SOLO USAR UNA VEZ PARA AJUSTAR HORA!!! Luego "comentar" y cargar.
  /*
  Serial.println("Estableciendo Hora y fecha...");
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__))); 
  Serial.println("DS1307 actualizado con hora y fecha de compilacion de este sketch.");
  Serial.print("Fecha: ");
  Serial.print(__DATE__);
  Serial.print(" Hora: ");
  Serial.println(__TIME__);
  */

  Serial.println("Iniciando alarma...\n");
  pinMode (detectPin, INPUT);
  pinMode (led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode (deactivatedLed, OUTPUT);
  digitalWrite (deactivatedLed, LOW);
}

void loop() {

  if (mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("RFID Tag detectado.");
    active = false;
    digitalWrite(deactivatedLed, HIGH);
    digitalWrite(led, LOW);
    Serial.println("DESACTIVADO SENSOR");    
  } 

  if (active == true) {
    detection();
  }
}

void detection() {
  detect = digitalRead(detectPin);
  if(detect == true) {
    digitalWrite(led, HIGH);
    Serial.println("Movimiento detectado");
    getDateTime();
  }
  else {
    digitalWrite(led, LOW);
  }
  delay(1000);
}

void getDateTime() {
  HoraFecha = rtc.now(); //obtenemos la hora y fecha actual
   
  segundo=HoraFecha.second();
  minuto=HoraFecha.minute();
  hora=HoraFecha.hour();
  dia=HoraFecha.day();
  mes=HoraFecha.month();
  anio=HoraFecha.year();

  //Enviamos por el puerto serie la hora y fecha.
  Serial.print("hora = ");
  Serial.print(hora);
  Serial.print(":");
  Serial.print(minuto);
  Serial.print(":");
  Serial.print(segundo);
  Serial.print("  Fecha = ");
  Serial.print(dia);
  Serial.print("/");
  Serial.print(mes);
  Serial.print("/");
  Serial.print(anio);
  Serial.println();
}
