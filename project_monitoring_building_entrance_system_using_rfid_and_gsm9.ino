#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

#define echoPin A0
#define trigPin A1

long duration;
int distance;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

LiquidCrystal_I2C lcd(0x27,16,2);

#define RST_PIN         9          
#define SS_PIN          10   

MFRC522 mfrc522(SS_PIN, RST_PIN);   

String read_rfid;

String data[2];

String data2;

int e = 0;

bool state = false;

int ledgreen = 2;
int ledred = 3;

int button1 = 4;
int button2 = 5;

int valuebutton1 = 0;
int valuebutton2 = 0;

int state2 = 0;
int state3 = 0;
int state4 = 0;
int state5 = 0;
int state6 = 0;
int state7 = 0;
int state8 = 0;

int buzz = 6;
int lamp = 7;

int relay = 8;

int var = 1;
int var2 = 0;

int minitawal = 0;
int minitstop = 0;
                   
void setup() {
    Serial.begin(9600);
    mlx.begin();  
    lcd.init();       
    lcd.backlight(); 
    while (!Serial);            
    SPI.begin();                
    mfrc522.PCD_Init();

    pinMode (button1, INPUT);
    pinMode (button2, INPUT);
    pinMode (ledgreen, OUTPUT);
    pinMode (ledred, OUTPUT);
    pinMode (relay, OUTPUT);
    pinMode (buzz, OUTPUT);
    pinMode (lamp, OUTPUT);

    digitalWrite (ledgreen, LOW);
    digitalWrite (ledred, HIGH);

    pinMode(trigPin, OUTPUT); 
    pinMode(echoPin, INPUT);

    //digitalWrite(relay, HIGH); //buang // kalu guna relay
    //digitalWrite(lamp, HIGH);  //buang // kalu guna relay

    
}


void dump_byte_array(byte *buffer, byte bufferSize) {
    read_rfid="";
    for (byte i = 0; i < bufferSize; i++) {
        read_rfid=read_rfid + String(buffer[i], HEX);
    }
}

void ultrasonic_function (){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  //Serial.print("Distance: ");
  //Serial.print(distance);
  //Serial.println(" cm");
}



void loop() {
  
  tmElements_t tm;
  RTC.read(tm);

  valuebutton1 = digitalRead (button1);
  valuebutton2 = digitalRead (button2);

  if (valuebutton1 == HIGH){
    var++;
    state2 = 0;
    state4 = 0;
    var2 =0;
    delay(300);
    Serial.println (state2);
    }

  if (valuebutton2 == HIGH){
    var--;
    state2 = 0;
    state4 = 0;
    var2 = 0;
    delay(300);
    Serial.println (state2);
    }

   switch (var) {
    case 1:
    
    if (state2 == 0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("running system");
      lcd.setCursor(0,1);
      lcd.print("scan card...");
      Serial.println (var);
      state2 = 1;
      Serial.println (state2);
      }

    switch (var2){
      case 1:
      if (state4 == 0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("scan temp");
      lcd.setCursor(0,1);
      lcd.print("temp :");
      state4 = 1;
      }
      ultrasonic_function ();
      lcd.setCursor(6,1);
      lcd.print(mlx.readObjectTempC());
      delay(100);
      
      if (distance <= 15 && mlx.readObjectTempC() <= 37.5){
       state6 = 1;
      }

      if (state6 == 1 ){
        minitawal = tm.Minute;
        if (state7 == 0){
          digitalWrite(buzz, HIGH);
          minitstop = tm.Minute + 5;
          state7 = 1;
        } 

        if (minitawal == minitstop){
          digitalWrite(buzz, LOW);  
          digitalWrite(lamp, HIGH); // kalu pakai relay LOW
          state6 = 0;
          state7 = 0;
          state8 = 1;
        }
      }
       if (state8 == 1){
          digitalWrite(relay, HIGH); // kalu pakai relay LOW
          state5 = 1;
          state3 = 0;
       }    
      
      break;
      case 2:
        digitalWrite(relay, LOW); // kalu pakai relay HIGH
        digitalWrite(lamp, LOW); // kalu pakai relay HIGH
        state8 = 0;
        state5 = 0;
        state2 = 0;
        var = 1;
      break;
      
      }

    if ( ! mfrc522.PICC_IsNewCardPresent())
    return;
    if ( ! mfrc522.PICC_ReadCardSerial())
    return;
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);

    
   if (state5 == 0){
      data2 = read_rfid;

      for (int w = 0; w < 2; w++){
      Serial.println (data[w]);
      if (read_rfid == data[w]){
        lcd.setCursor(0,1);
        lcd.print("               ");
        lcd.setCursor(0,1);
        lcd.print("card is correct");
        state3 = 1;
        state4 = 0;
        delay (3000);
        var2 = 1;
        }

      if (read_rfid == data2 && state3 == 0) {
        lcd.setCursor(0,1);
        lcd.print("               ");
        lcd.setCursor(0,1);
        lcd.print("Wrong card");
        delay (2000);
        lcd.setCursor(0,1);
        lcd.print("               ");
        lcd.setCursor(0,1);
        lcd.print("try again");
        }
     }
   }
   
    if (state5 == 1){
      data2 = read_rfid;

      for (int w = 0; w < 2; w++){
      Serial.println (data[w]);
      if (read_rfid == data[w]){
        lcd.setCursor(0,1);
        lcd.print("               ");
        lcd.setCursor(0,1);
        lcd.print("card is correct");
        state3 = 1;
        state4 = 0;
        delay (3000);
        var2 = 2;
        }

      if (read_rfid == data2 && state3 == 0) {
        lcd.setCursor(0,1);
        lcd.print("               ");
        lcd.setCursor(0,1);
        lcd.print("Wrong card");
        delay (2000);
        lcd.setCursor(0,1);
        lcd.print("               ");
        lcd.setCursor(0,1);
        lcd.print("try again");
        }
    }

    }
    
    break;

    case 2:
    if (state2 == 0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("store data card");
      lcd.setCursor(0,1);
      lcd.print("scan card...");
      Serial.println (var);
      state2 = 1;
      Serial.println (state2);
      }
    
    if ( ! mfrc522.PICC_IsNewCardPresent())
    return;
    if ( ! mfrc522.PICC_ReadCardSerial())
    return;
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  
    data[e] = read_rfid;
    e += 1;
    digitalWrite (ledgreen, HIGH);
    digitalWrite (ledred, LOW);
    lcd.setCursor(0,1);
    lcd.print("               ");
    lcd.setCursor(0,1);
    lcd.print("done save data");
    delay (3000);
    digitalWrite (ledgreen, LOW);
    digitalWrite (ledred, HIGH);
    lcd.setCursor(0,1);
    lcd.print("               ");
    lcd.setCursor(0,1);
    lcd.print("scan card...");
    delay (3000);
    
    
    for (int f =0; f < 2; f++){
      Serial.println (data[f]);
    
    }
    break;
    
    }

}  

 
