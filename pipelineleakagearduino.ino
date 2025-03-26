#include <DHT.h>
#define DHTPIN A2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
TinyGPS gps;
String msg;
float flat;
float flon;
int buzzer=10;
int gas=8;
int flow=9;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
SoftwareSerial mySerial(11, 12);
String uno;
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
dht.begin();
  pinMode( gas,INPUT);
   pinMode( flow,INPUT);
   pinMode( buzzer,OUTPUT);
 digitalWrite(buzzer, LOW);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Underground");
  lcd.setCursor(0, 1);
  lcd.print("Gas pipeline");
  delay(2000);
}
void loop() {
 
  int flow_value=digitalRead(flow);
   lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("flow_value: ");
  lcd.print(flow_value);
  delay(1000);
int gas_value=digitalRead(gas);
lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("gas_value: ");
  lcd.print(gas_value);
  delay(1000);
 int temperature = dht.readTemperature();
  int humidity = dht.readHumidity();
  if (!isnan(temperature) && !isnan(humidity))
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temperature);
  lcd.setCursor(0, 1);
  lcd.print("humi:");
  lcd.print(humidity);
  delay(1000);

  if (temperature > 32)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("high temperature");
     digitalWrite(buzzer, HIGH);
    delay(4000);
    digitalWrite(buzzer, LOW);
   msg="high temperature";
   SendMessage();
   delay(2000);
  }
  if (flow_value==1) {
    lcd.clear();
    lcd.print("pipe leakage  ");
    lcd.setCursor(0, 1);
    lcd.print("detected");

    digitalWrite(buzzer, HIGH);
    delay(4000);
    digitalWrite(buzzer, LOW);
    msg = "pipe leakage detected";
    SendMessage();
  } 
  else if(gas_value==0){
    lcd.clear();
    lcd.print("harm full gases ");
    lcd.setCursor(0, 1);
    lcd.print("detected");

    digitalWrite(buzzer, HIGH);
    delay(4000);
    digitalWrite(buzzer, LOW);
    msg = "harmfull gases detected";
    SendMessage();
  }
  


  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;) {
    while (Serial.available()) {
      char c = Serial.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c))  // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData) {
    //    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.println("LAT=");
    Serial.println(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    //    Serial.println(flat);
    Serial.println(" LON=");
    Serial.println(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    //    Serial.println(flon);
    delay(2000);
  }

 uno = String("a") + String(temperature) + String("b") + String(humidity) + String("c") + String(flow_value) +String ("d")+ String(gas_value)+String ("e");//+ String(vIN)+String ("f")+String( Amps_RMS)+String ("g");
  mySerial.println(uno);
  delay(1000);
}


void SendMessage() {
  //Serial.println("Sending Message...");

  Serial.println("AT");
  delay(1000);
  Serial.println("ATE0");
  delay(1000);
  Serial.println("AT+CMGF=1");
  delay(1000);
  Serial.println("AT+CMGS=\"+918260881544\"\r");
  delay(2500);

  String flatStr = String(flat, 6);
  String flonStr = String(flon, 6);

  Serial.println(msg);
  Serial.println("Location: Click to view");
  Serial.println("http://www.google.com/maps/place/" + flatStr + "," + flonStr);
  delay(6000);
  Serial.write(26);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Message sent..");


  delay(5000);  // Wait for call duration
  
}
