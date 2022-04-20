#include "HX711.h"
#include <Wire.h>
//#include "printf.h"
#include "TFT_eSPI.h"
#include "Seeed_FS.h"
#include "RawImage.h"

TFT_eSPI tft;

//#define LB2KG  0.45352
#define CALWEIGHT 500
#define DEFAULT_CALIFACTOR 0

#define id 1
#define number_of_digits 1

//#define DOUT  2
//#define CLK 3
//#define button A0
//#define button WIO_5S_PRESS;

HX711 scale(PIN_WIRE_SDA,PIN_WIRE_SCL);

long currentOffset;
float calibration_factor;


void setup() {
  //serial
  Serial.begin(115200);
  Serial.println("System started");
  pinMode(WIO_5S_PRESS, INPUT_PULLUP);
  pinMode(WIO_KEY_A, INPUT_PULLUP);
//  printf_begin();

//  // lcd
//  lcd.begin();
//  lcd.backlight();
//  lcd.clear();
//  //EEPROM.write(0x00,0xFF);
//  // button pin
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("Auto Callibration Demo", 50, 120);
  delay(2000);
  calibration_factor = DEFAULT_CALIFACTOR;
  
  
  
//  // eeprom
//  if (EEPROM.read(0x00) != 0x01) 
//  {
//    Serial.println("NOT INIT !!!!");
//    currentOffset = 0;
//    calibration_factor = DEFAULT_CALIFACTOR;     
//    // show instructions
//    lcd.setCursor(0, 0);
//    lcd.print("MUST CALIBIRATE");
//    lcd.setCursor(0, 1);
//    lcd.print("Press Button");
//    //wait for button press
//    while (digitalRead(button));
//    lcd.clear();
    //scale
  //  scale.begin();
    delay(100);
    Serial.println("calibration_factor = " + String(calibration_factor));
    scale.set_scale(calibration_factor);
  //  }
//  else
//  {
//    EEPROM.get(0x01,currentOffset);
//    EEPROM.get(0x01+sizeof(long),calibration_factor);   
//    Serial.println("currentOffset = " + String(currentOffset));
//    Serial.println("calibration_factor = " + String(calibration_factor));
//  }

  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("Must Calibrate", 50, 120);
  tft.drawString("Press Button", 50, 150);
  
  //scale
//  scale.begin();


//  // if button is pressed (LOW) start calibiration
//  if (digitalRead(WIO_5S_PRESS) == LOW){
//    tft.fillScreen(TFT_BLACK);
//    tft.setTextSize(2);
//    tft.drawString("Clear Scale", 50, 120);
//    delay(2000);
//  }
//    // show instructions
////    lcd.setCursor(0, 0);
////    lcd.print("Clear Scale");
//    // wait till person leaves the button
////    while (digitalRead(WIO_5S_PRESS) == LOW);
////    //short delay
////    delay(200);
//    
//    // set tare and save value
//    scale.tare();
//    currentOffset = scale.get_offset();
//    Serial.println(currentOffset);
//
//    // show on lcd
////    lcd.clear();
////    lcd.print("Place 3Kg");
////    lcd.setCursor(0, 1);
////    lcd.print("Press Button");
//    tft.fillScreen(TFT_BLACK);
//    tft.setTextSize(2);
//    tft.drawString("Place 3kg", 50, 120);
//    tft.drawString("Press Button", 50, 150);
//    //wait for button press
//    while (digitalRead(WIO_5S_PRESS) == LOW);
////    lcd.clear();
////    lcd.print("Please wait ");
//    Serial.println("calibirte");
//    tft.fillScreen(TFT_BLACK);
//    tft.setTextSize(2);
//    tft.drawString("Please Wait...", 50, 120);
//    // calibiation
//    boolean done = false;
//    uint8_t flipDirCount = 0;
//    int8_t direction = 1;
//    uint8_t dirScale = 100;
//    double data = abs(scale.get_units());
//    double prevData = data;
//    char runningSign[] = {'-','\\','|','/'};
//    uint8_t runningSignIdx = 0;
//    while (!done)
//    {
//      // get data
//      data = abs(scale.get_units());
//      Serial.println("data = " + String(data, 2));
//      Serial.println("abs = " + String(abs(data - CALWEIGHT), 4));
//      Serial.println("calibration_factor = " + String(calibration_factor));
//      // if not match
//      if (abs(data - CALWEIGHT) >= 0.01)
//      {
//        if (abs(data - CALWEIGHT) < abs(prevData - CALWEIGHT) && direction != 1 && data < CALWEIGHT)
//        {
//          direction = 1;
//          flipDirCount++;
//        }
//        else if (abs(data - CALWEIGHT) >= abs(prevData - CALWEIGHT) && direction != -1 && data > CALWEIGHT)
//        {
//          direction = -1;
//          flipDirCount++;
//        }
//
//        if (flipDirCount > 2)
//        {
//          if (dirScale != 1)
//          {
//            dirScale = dirScale / 10;
//            flipDirCount = 0;
//            Serial.println("dirScale = " + String(dirScale));
//          }
//        }
//        // set new factor 
//        calibration_factor += direction * dirScale;
//        scale.set_scale(calibration_factor);
//        // show still running 
////        lcd.setCursor(15, 1);
////        lcd.print(runningSign[runningSignIdx]);
////        runningSignIdx = (runningSignIdx+1)%4;
//        //short delay
//        delay(5);
//        // keep old data 
//        prevData = data;
//      }
//      // if match
//      else
//      {
//        Serial.println("NEW currentOffset = " + String(currentOffset));
//        Serial.println("NEW calibration_factor = " + String(calibration_factor));
////        EEPROM.put(0x00,0x01); // set init
////        EEPROM.put(0x01,currentOffset);
////        EEPROM.put(0x01+sizeof(long),calibration_factor);  
////        done = true;
////        lcd.clear();
//      }
//
//    } // end while
//  } //end if button pressed

  scale.set_offset(currentOffset);
//
//  tft.fillScreen(TFT_BLACK);
//  tft.setTextSize(2);
//  tft.setCursor(110,50);
//  tft.println(scale.get_units());
//  tft.setCursor(170,70);
//  tft.println(calibration_factor);
//
//  lcd.setCursor(0, 0);
//  lcd.print("Scaleit");
//  lcd.setCursor(13, 1);
//  lcd.print("KG");
//

  // radio
//  radio.begin();
//  radio.openWritingPipe(addresses[1]); // 00002
//  radio.openReadingPipe(1, addresses[0]); // 00001
//  radio.setPALevel(RF24_PA_MIN);
//  radio.setDataRate(RF24_250KBPS);
//  radio.setChannel(100);
//  radio.stopListening();
//  radio.printDetails();
//
//  
  Serial.println("setup done ...");

}

void loop() {
    // if button is pressed (LOW) start calibiration
  if (digitalRead(WIO_5S_PRESS) == LOW){
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    tft.drawString("Clear Scale", 50, 120);
    delay(2000);
    // show instructions
//    lcd.setCursor(0, 0);
//    lcd.print("Clear Scale");
    // wait till person leaves the button
//    while (digitalRead(WIO_5S_PRESS) == LOW);
//    //short delay
//    delay(200);
    
    // set tare and save value
    scale.tare();
    currentOffset = scale.get_offset();
    Serial.println(currentOffset);

    // show on lcd
//    lcd.clear();
//    lcd.print("Place 3kg");
//    lcd.setCursor(0, 1);
//    lcd.print("Press Button");
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    tft.drawString("Place 500 g", 50, 90);
    tft.drawString("Press Button", 50, 120);
    tft.setCursor(50, 150);

    //wait for button press
    while (digitalRead(WIO_5S_PRESS) == HIGH);
//    lcd.clear();
//    lcd.print("Please wait ");
    Serial.println("calibirte");
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    tft.drawString("Please Wait...", 50, 120);
    // calibiation
    boolean done = false;
    uint8_t flipDirCount = 0;
    int8_t direction = 1;
    float dirScale = 100;
    double data = abs(scale.get_units());
    double prevData = data;
    char runningSign[] = {'-','\\','|','/'};
    uint8_t runningSignIdx = 0;
    while (!done)
    {
      // get data
      data = abs(scale.get_units());
      Serial.println("data = " + String(abs(scale.get_units())));
      Serial.println("data - calWeight = " + String(abs(data - CALWEIGHT),5));
      Serial.println("prevData - calWeight = " + String(abs(prevData - CALWEIGHT),5));
      Serial.println("direction = " + String(direction));
      Serial.println("calibration_factor = " + String(calibration_factor));
      Serial.println("flipDirCount = " + String(flipDirCount));
      Serial.println("dirScale = " + String(dirScale));
      Serial.println("-------------------------------------");
      // if not match
      if (abs(data - CALWEIGHT) >= 0.001)
      {
        if (abs(data - CALWEIGHT) < abs(prevData - CALWEIGHT) && direction != 1 && data < CALWEIGHT)
        {
          direction = 1;
          flipDirCount++;
        }
        else if (abs(data - CALWEIGHT) >= abs(prevData - CALWEIGHT) && direction != -1 && data > CALWEIGHT)
        {
          direction = -1;
          flipDirCount++;
        }
        else if (abs(data - CALWEIGHT) >= abs(prevData - CALWEIGHT) && direction != 1 && data < CALWEIGHT)
        {
          direction = 1;
          flipDirCount++;
        }
        else if (abs(data - CALWEIGHT) < abs(prevData - CALWEIGHT) && direction != -1 && data > CALWEIGHT)
        {
          direction = -1;
          flipDirCount++;
        }
        if (flipDirCount > 2)
        {
          if (dirScale != 0.1)
          {
            dirScale = dirScale / 10;
            flipDirCount = 0;
            Serial.println("dirScale = " + String(dirScale));
          }
        }
        // set new factor 
        calibration_factor += direction * dirScale;
        scale.set_scale(calibration_factor);
        // show still running 
//        lcd.setCursor(15, 1);
//        lcd.print(runningSign[runningSignIdx]);
//        runningSignIdx = (runningSignIdx+1)%4;
        //short delay
        delay(5);
        // keep old data 
        prevData = data;
      }
      // if match
      else
      {
        Serial.println("NEW currentOffset = " + String(currentOffset));
        Serial.println("NEW calibration_factor = " + String(calibration_factor));
//        EEPROM.put(0x00,0x01); // set init
//        EEPROM.put(0x01,currentOffset);
//        EEPROM.put(0x01+sizeof(long),calibration_factor);  
        done = true;
//        lcd.clear();
      }

    } // end while
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(110,50);
    tft.println(calibration_factor);
    tft.setCursor(110, 80);
    tft.println(scale.get_units());
  } //end if button pressed


//  // get data
//  double data = abs(scale.get_units());
//  // issue with abs missing at the 4th digit after the dot (bug!!)
//  if (0.0000 - data > 0.0001) 
//    data = 0.00; //reset to zero
//    
//  
//  // serial
//  Serial.print(data, number_of_digits);
//  Serial.println(" Kg");
//
//  // lcd
////  lcd.setCursor(8, 1);
////  lcd.print(data, number_of_digits);
//
//
//
//  // nrf
////  double data2[2] = {id, data};
////  radio.write(&data2, sizeof(data2));
//
//  // short delay
//  delay(1000);
}
