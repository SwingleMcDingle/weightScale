#include "HX711.h" //Including HX711 module library
#include "TFT_eSPI.h"
#include "Seeed_FS.h" //Including SD card library
#include"RawImage.h"
//#include "Free_Fonts.h" // check if the header is included in the seed lcd library
TFT_eSPI tft;

int32_t y = 55;
uint32_t firstStringColor = 0xC618;
uint32_t secondStringColor = 0xC618;
uint32_t thirdStringColor = 0xC618;
int selectionCounter = 95;
int pageNumber = 0;  
int SENSOR = D0;

HX711 scale(PIN_WIRE_SDA, PIN_WIRE_SCL);

#define CALWEIGHT 502.5
#define DEFAULT_CALIFACTOR 0
#define BUZZER_PIN WIO_BUZZER

long currentOffset;
float calibration_factor;  
int total = 0;
int good = 0;
int lastVal = 1;
uint32_t updateTime = 0;

void setup() {
  scale.tare();
  calibration_factor = DEFAULT_CALIFACTOR;
  
  //initialize serial communication
  Serial.begin(115200);
  pinMode(WIO_KEY_A, INPUT_PULLUP);
  pinMode(WIO_KEY_B, INPUT_PULLUP);
  pinMode(WIO_KEY_C, INPUT_PULLUP);
  pinMode(WIO_5S_UP, INPUT_PULLUP);
  pinMode(WIO_5S_DOWN, INPUT_PULLUP);
  pinMode(WIO_5S_PRESS, INPUT_PULLUP);
  pinMode(SENSOR, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  if (!SD.begin(SDCARD_SS_PIN, SDCARD_SPI)) {
        while (1);
    }
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(0xFFFF);
    drawImage<uint8_t>("rgb332/notplaArduino.bmp", 0, 30);                                 //Display this 8-bit image in sd card from (0, 0)
    delay(2000);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.drawString("Weight Sensor Demo", 50, 120);
    delay(1000);
    tft.fillScreen(TFT_BLACK);   // clear screen
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.drawString("MENU", 150, 2);
    tft.drawString("Weight Calibration", 20, 50);
    tft.drawString("Reset Counter", 20, 70);
    tft.drawString("Product select", 20, 90);
    mainMenu("MENU",0xffff,150,"Weight Calibration",firstStringColor,"Reset counter",secondStringColor,"Product selection",thirdStringColor);

    updateTime = millis();
}

void loop() {
  tft.fillCircle(10, y, 3, TFT_WHITE);
  if (digitalRead(WIO_5S_DOWN) == LOW) {
    y += 20;
    tft.fillCircle(10, y-20, 3, TFT_BLACK);
    if( y > selectionCounter){
       y = 55;
      }
   }
   if(digitalRead(WIO_5S_UP) == LOW){
    y -= 20;
    tft.fillCircle(10, y+20, 3, TFT_BLACK);
    if( y < 55){
      y = 95;
    }
   }
  delay(200);
  
  WeightCalMenu();
  weightCalSettings();
  returnToWeightCal();
  beginCalibration();
  weightTest();
  Weight();
//  liveWeight();
  returnFromCalError();
  returnToBeginCalibration();
  returnFromWeightTest();
  returnToMmenuFromWeightCal();
  
  resetCounterMenu();
  returnToMmenuFromResetCounterMenu();
  
  productSelectionMenu();
  smallOoho();
  largeOoho();
//  Sensor();
  smallOohoCounter();
  largeOohoCounter();
  backFromLargeOoho();
  backFromSmallOoho();
  returnToMmenuFromProductSelectionMenu();
  
//  Serial.println(selectionCounter);
}

int mainMenu(char pageTitle[],uint32_t pageTitleColor,uint32_t titlePosition,char textline1[],uint32_t firstStringColor,char textLine2[],uint32_t secondStringColor,char textLine3[],uint32_t thirdStringColor){
  highlightSelection();
  if (textLine3 == ""){
    selectionCounter = 75;
  }
  else{
    selectionCounter = 95;
  }
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(pageTitleColor);
  tft.drawString(pageTitle, titlePosition, 2);
  tft.setTextColor(firstStringColor);
  tft.drawString(textline1, 20, 50);
  tft.setTextColor(secondStringColor);
  tft.drawString(textLine2, 20, 70);
  tft.setTextColor(thirdStringColor);
  tft.drawString(textLine3, 20, 90);
  return 0;
}

void WeightCalMenu(){
  if(digitalRead(WIO_5S_PRESS) == LOW && y == 55 && pageNumber == 0){
    delay(200);
    pageNumber = 1;
    mainMenu("Weight Calibration",0xffff,60,"Weight Cal. settings",firstStringColor,"back",secondStringColor,"",thirdStringColor);
  }
}

void returnToMmenuFromWeightCal(){
  if(digitalRead(WIO_5S_PRESS) == LOW && pageNumber == 1 && y == 75){
    delay(200);
    mainMenu("MENU",0xffff,150,"Weight Calibration",firstStringColor,"Reset counter",secondStringColor,"Product selection",thirdStringColor);
    pageNumber = 0;
    y = 55;
  }
}

void resetCounterMenu(){
  if(digitalRead(WIO_5S_PRESS) == LOW && y == 75 && pageNumber == 0){
    delay(200);
    pageNumber = 2;
    mainMenu("",0xffff,0,"Counter cleared!",firstStringColor,"back",secondStringColor,"",thirdStringColor);
  }
}

void returnToMmenuFromResetCounterMenu(){
  if(digitalRead(WIO_5S_PRESS) == LOW && pageNumber == 2 && y == 75){
    delay(200);
    mainMenu("MENU",0xffff,150,"Weight Calibration",firstStringColor,"Reset counter",secondStringColor,"Product selection",thirdStringColor);
    pageNumber = 0;
    y = 55;
  }
}

void productSelectionMenu(){
  if(digitalRead(WIO_5S_PRESS) == LOW && y == 95 && pageNumber == 0){
    delay(200);
    pageNumber = 3;
    mainMenu("Product Selection",0xffff,60,"25g - Small Ooho",firstStringColor,"70g - Large Ooho",secondStringColor,"back",thirdStringColor);
    y = 55;
  }
}

void returnToMmenuFromProductSelectionMenu(){
  if(digitalRead(WIO_5S_PRESS) == LOW && pageNumber == 3 && y == 95){
    delay(200);
    mainMenu("MENU",0xffff,150,"Weight Calibration",firstStringColor,"Reset counter",secondStringColor,"Product selection",thirdStringColor);
    pageNumber = 0;
    y = 55;
  }
}

void highlightSelection(){
  if (y == 55){
    firstStringColor = 0xffff;
    secondStringColor = 0xC618;
    thirdStringColor = 0xC618;
  }
  else if (y == 75){
    secondStringColor = 0xffff;
    firstStringColor = 0xC618;
    thirdStringColor = 0xC618;
  }
  else if (y == 95){
    thirdStringColor = 0xffff;
    firstStringColor = 0xC618;
    secondStringColor = 0xC618;
  }
  else{
    firstStringColor = 0xC618;
    secondStringColor = 0xC618;
    thirdStringColor = 0xC618;
  }
}

// CALIBRATION MENU //

void weightCalSettings(){
  if(digitalRead(WIO_5S_PRESS) == LOW && y == 55 && pageNumber == 1){
    delay(200);
    mainMenu("Weight Cal. Settings",0xffff,10,"Begin Calibration",firstStringColor,"Weight Test",secondStringColor,"Back",thirdStringColor);
    pageNumber = 4;
    y = 55;
  }
}

void beginCalibration() {
    // if button is pressed (LOW) start calibiration
  if (digitalRead(WIO_5S_PRESS) == LOW && pageNumber == 4 && y == 55){
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    tft.drawString("Clear Scale", 50, 120);
    delay(3000);

    // set tare and save value
    scale.tare();
    currentOffset = scale.get_offset();
    Serial.println(currentOffset);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    tft.drawString("Place 500 g", 50, 90);
    tft.drawString("Press Button", 50, 120);
    tft.setCursor(50, 150);

    //wait for button press
    while (digitalRead(WIO_5S_PRESS) == HIGH);
    Serial.println("calibrate");
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(2);
    tft.drawString("Please Wait...", 50, 120);
    // calibiation
    boolean done = false;
    uint8_t flipDirCount = 0;
    int8_t direction = 1;
    uint8_t iter = 0;
    float dirScale = 100;
    double data = abs(scale.get_units());
    double prevData = data;
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
      Serial.println("iter = " + String(iter));
      Serial.println("-------------------------------------");
      
      // if not match
      iter += 1;
      if (abs(data - CALWEIGHT) >= 0.005)
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
          if (dirScale != 0.01)
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
        //short delay
        delay(5);
        // keep old data 
        prevData = data;

        //error if iteration exceeds 100
        if(iter > 100)
        {
          delay(200);
          done = true;
          tft.fillScreen(TFT_BLACK);
          mainMenu("ERROR", 0xffff, 50,"Calibration Error",firstStringColor,"Reset",secondStringColor,"",thirdStringColor);
          pageNumber = 7;
          y = 75;
        }
      }

      // if match
      else
      {
        Serial.println("NEW currentOffset = " + String(currentOffset));
        Serial.println("NEW calibration_factor = " + String(calibration_factor));
        done = true;
        delay(200);
        tft.fillScreen(TFT_BLACK);
        mainMenu("Calibration Complete", 0xffff,50,"Cal.Factor: ",firstStringColor,"Weight: ",secondStringColor,"Back",thirdStringColor);
        pageNumber = 5;
        y = 95;
        tft.setCursor(170,50);
        tft.println(calibration_factor);
        tft.setCursor(110, 70);
        tft.println(data);
      }
    } // end while
  } //end if button pressed
}

void weightTest(){
  if(digitalRead(WIO_5S_PRESS) == LOW && pageNumber == 4 && y == 75){
    delay(200);
    mainMenu("Weight Test",0xffff,10,"Weight: ",firstStringColor,"Back",secondStringColor,"",thirdStringColor);
    pageNumber = 6;
    y = 55;
  }
}

void Weight(){
  if (pageNumber == 6){
    tft.setCursor(110, 50);
    tft.println(abs(scale.get_units()));

    scale.power_down();
    delay(500);
    scale.power_up();
    tft.fillRect(110,50,100,20,TFT_BLACK);
  }
}

void returnFromCalError(){
  if(digitalRead(WIO_5S_PRESS) == LOW && pageNumber == 7 && y == 75){
    delay(200);
    mainMenu("Weight Cal. Settings",0xffff,10,"Begin Calibration",firstStringColor,"Weight Test",secondStringColor,"Back",thirdStringColor);
    pageNumber = 4;
    y = 55;
  }
}

void returnToBeginCalibration(){
  if(digitalRead(WIO_5S_PRESS) == LOW && pageNumber == 5 && y == 95){
    delay(200);
    mainMenu("Weight Cal. Settings",0xffff,10,"Begin Calibration",firstStringColor,"Weight Test",secondStringColor,"Back",thirdStringColor);
    pageNumber = 4;
    y = 55;
  }
}

void returnFromWeightTest(){
  if(digitalRead(WIO_5S_PRESS) == LOW && pageNumber == 6){
    delay(200);
    mainMenu("Weight Cal. Settings",0xffff,10,"Begin Calibration",firstStringColor,"Weight Test",secondStringColor,"Back",thirdStringColor);
    pageNumber = 4;
    y = 55;
  }
}

void returnToWeightCal(){
  if(digitalRead(WIO_5S_PRESS) == LOW && pageNumber == 4 && y == 95){
    delay(200);
    mainMenu("Weight Calibration",0xffff,60,"Weight Cal. settings",firstStringColor,"Back",secondStringColor,"",thirdStringColor);
    pageNumber = 1;
    y = 55;
  }
} 

// RECIPE MENU FUNCTIONS //

void largeOoho(){
  if(digitalRead(WIO_5S_PRESS) == LOW && pageNumber == 3 && y == 75){
    delay(200);
    mainMenu("70g - Large Ooho",0xffff,60,"Total = ",firstStringColor,"Good = ",secondStringColor,"Back",thirdStringColor);
    pageNumber = 8;
    y = 55;
  }
}

void smallOoho(){
  if(digitalRead(WIO_5S_PRESS) == LOW && pageNumber == 3 && y == 55){
    delay(200);
    mainMenu("25g - Small Ooho",0xffff,60,"Produced = ",firstStringColor,"Good = ",secondStringColor,"Back",thirdStringColor);
    pageNumber = 9;
    y = 55;
  }
}

void Sensor(){
  if(pageNumber == 8 || pageNumber == 9){
    int val = digitalRead(SENSOR);
    if(val == 0){
      Serial.println("Obstacle Detected");
    }
    else{
      Serial.println("No Obstacle");
    }
  }
}

void largeOohoCounter(){
  if(pageNumber == 8){
    int val = digitalRead(SENSOR);
    float data = abs(scale.get_units());
    y = 95;
    
    Serial.println("Cal.Factor = " + String(calibration_factor));
    Serial.println("Weight = " + String(data));
    Serial.println("val = " + String(val));
    Serial.println("lastVal = " + String(lastVal));
    Serial.println("Total = " + String(total));
    Serial.println("Good = " + String(good));
    Serial.println("-----------------------------");
    
    // 0 - Ooho Detected
    // 1 - No Object
    if(lastVal != val){
      if(490 < data && 510 > data && val == 0){
        total += 1;
        good += 1;
        Serial.println("Ooho Detected");
        Serial.println("In Spec");
        
        tft.fillRect(150,50,100,20,TFT_BLACK); 
        tft.fillRect(125,70,100,20,TFT_BLACK);
      }
      else if(data < 490 && val  == 0 || data > 510 && val == 0){
        Serial.println("Ooho Detected");
        Serial.println("Out of Spec");
        total += 1;
        good += 0;
  
        tft.fillRect(150,50,100,20,TFT_BLACK); 
        tft.fillRect(125,70,100,20,TFT_BLACK);
      }
      else{
        Serial.println("No Object");
        total += 0;
        good += 0;

        tft.fillRect(150,50,100,20,TFT_BLACK); 
        tft.fillRect(125,70,100,20,TFT_BLACK);  
      }
      lastVal = val;
      
      tft.setCursor(150,50);
      tft.println(total);
      tft.setCursor(125,70);
      tft.println(good);

      delay(500);
    
      scale.power_down();
      scale.power_up();
    } //Execute this logic if sensor reading has changed
    else{
      lastVal = val;
  
      tft.setCursor(150,50);
      tft.println(total);
      tft.setCursor(125,70);
      tft.println(good);

      delay(500);
    
      scale.power_down();
      scale.power_up();
    }// Keep the values the same if is the same
    }
  }
}


void smallOohoCounter(){
  if(pageNumber == 9){
    double data = abs(scale.get_units());
    scale.set_scale(calibration_factor);
    data = abs(scale.get_units());
    Serial.println("Cal.Factor = " + String(calibration_factor));
    Serial.println("Weight = " + String(data));

    tft.setCursor(125, 70);
    tft.println(data);
    
    delay(500);
    tft.fillRect(125,70,100,20,TFT_BLACK);

    if(495 < data && 505 > data){
      total += 1;
      
      scale.power_down();
      delay(100);
      scale.power_up();
    }
    tft.setCursor(150, 50);
    tft.println(total);

    delay(500);
    tft.fillRect(150,50,100,20,TFT_BLACK);
  }
}


void backFromLargeOoho(){
  if(digitalRead(WIO_5S_PRESS) == LOW && pageNumber == 8 && y == 95){
    delay(200);
    mainMenu("Product Selection",0xffff,60,"25g",firstStringColor,"70g",secondStringColor,"Back",thirdStringColor);
    pageNumber = 3;
    y = 55; 
  }
}

void backFromSmallOoho(){
  if(digitalRead(WIO_5S_PRESS) == LOW && pageNumber == 9 && y == 95){
    delay(200);
    mainMenu("Product Selection",0xffff,60,"25g",firstStringColor,"70g",secondStringColor,"Back",thirdStringColor);
    pageNumber = 3;
    y = 55; 
  }
}
