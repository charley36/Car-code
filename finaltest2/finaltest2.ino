#include <TinyGPS++.h>
#include <math.h>
#include <SoftwareSerial.h>
static const int RXPin = 8, TXPin = 9;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
SoftwareSerial Genotronex(10, 11);
int BluetoothData;
int mp1 = 2;
int mp2 = 3;
int mp3 = 4;
int mp4 = 5;
int fsensor = 7;
int ssensor = 6;
int fobstacle = HIGH;
int lobstacle = HIGH;
int loop1 = 1;
float ang1 = 0;
float anglex = 0;
static const double PC_LAT = 53.266303, PC_LNG = -2.153080;

void setup()
{
  Genotronex.begin(9600);
  Genotronex.println("Bluetooth On please press 1 to start");
  pinMode(mp1, OUTPUT);
  pinMode(mp2, OUTPUT);
  pinMode(mp3, OUTPUT);
  pinMode(mp4, OUTPUT);
  pinMode(fsensor, INPUT);
  pinMode(ssensor, INPUT);
  Serial.begin(9600);
  ss.begin(GPSBaud);
}

void loop(){
do{
if (Genotronex.available())
  BluetoothData=Genotronex.read();
  if(BluetoothData=='1'){
  loop1 =-1;}
  else {delay(100);}
}while (loop1 == 1);
Genotronex.println("Begin!");
delay(1000);
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }

if (PC_LAT > gps.location.lat())
  {
  do {turnright();}while (ang1 != 0);
  if (PC_LNG > gps.location.lng())
  do {turnleft();}while (ang1 != 0 - anglex);
  else
  do {turnright();}while (ang1 != 0 + anglex);
  }
else
{
  do {turnright();}while (ang1!= 180);
  if (PC_LNG > gps.location.lng())
  do {turnright();}while (ang1 != 180 + anglex);
  else
  do {turnleft();}while (ang1 != 180 - anglex);
  }

  
  fobstacle = digitalRead(fsensor);
  if (fobstacle == LOW){
    stopmoving();
    lineup();
    obleft();
    if (fobstacle == LOW){
      stopmoving();
      lineup();
      obleft();
      if (fobstacle == LOW){
        stopmoving();
        lineup();
        obleft();
        if (fobstacle == LOW){
          stopmoving();
          lineup();
          obleft();
          }
        else
        turnleft();
        }
      else
      turnleft();
      }
    else {
      stopmoving(); 
      turnleft(); 
     }
  }
  else
  {
    moveforward();
  }
delay(100);  
}

void displayInfo()
{ 
  if (gps.location.isValid())
  {

    float resultLat = (gps.location.lat() - PC_LAT);
    float resultLng = (gps.location.lng() - PC_LNG);
    float anglex = atan(resultLng/resultLat);
    printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.deg()) : "*** ", 6);
    float ang1 = (gps.course.deg());
  }
  Serial.println();
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
}
void lineup()
{
  lobstacle = digitalRead(ssensor);
  do
  (digitalWrite(mp1, HIGH), digitalWrite(mp4, HIGH), lobstacle = digitalRead(ssensor)); while (lobstacle == HIGH);
  digitalWrite(mp1, LOW);
  delay(100);
}
void obleft()
{
  do{
      moveforward();
      lobstacle = digitalRead(ssensor);
      fobstacle = digitalRead(fsensor);
    }while (lobstacle == LOW and fobstacle == HIGH);
}
void turnright()
{
  digitalWrite(mp3, LOW);
  digitalWrite(mp1, HIGH);
  delay(1000);
}
void turnleft()
{
  digitalWrite(mp3, HIGH);
  digitalWrite(mp1, LOW);
  delay(1000);
}
void moveforward()
{
  digitalWrite(mp1, HIGH);
  digitalWrite(mp2, LOW);
  digitalWrite(mp3, HIGH);
  digitalWrite(mp4, LOW);
}
void stopmoving()
{
  digitalWrite(mp1, LOW);
  digitalWrite(mp3, LOW);
}
