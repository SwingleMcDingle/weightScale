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
int32_t calScale = -1097;

HX711 scale(PIN_WIRE_SDA, PIN_WIRE_SCL);

void setup() {

  
  //initialize serial communication
  Serial.begin(115200);
  pinMode(WIO_KEY_A, INPUT_PULLUP);
  pinMode(WIO_KEY_B, INPUT_PULLUP);
  pinMode(WIO_KEY_C, INPUT_PULLUP);
  pinMode(WIO_5S_UP, INPUT_PULLUP);
  pinMode(WIO_5S_DOWN, INPUT_PULLUP);
  pinMode(WIO_5S_PRESS, INPUT_PULLUP);

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
    delay(500);
    tft.fillScreen(TFT_BLACK);   // clear screen
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.drawString("MENU", 150, 2);
    tft.drawString("Weight Calibration", 20, 50);
    tft.drawString("Reset Counter", 20, 70);
    tft.drawString("Product select", 20, 90);
    mainMenu("MENU",0xffff,150,"Weight Calibration",firstStringColor,"Reset counter",secondStringColor,"Product selection",thirdStringColor);
    
    Serial.begin(9600);
    scale.set_scale(-1097.f); //calibration factor 
    scale.tare();
      
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
  delay(200);
  
  WeightCalMenu();
  weightCalSetup();
  returnToMmenuFromWeightCal();
  resetCounterMenu();
  returnToMmenuFromResetCounterMenu();
  productSelectionMenu();
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
    mainMenu("Product Selection",0xffff,60,"25g",firstStringColor,"70g",secondStringColor,"back",thirdStringColor);
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

void weightCalSetup(){
 
  if(digitalRead(WIO_5S_PRESS) == LOW && y == 55 && pageNumber == 1){
    delay(200);
    pageNumber = 4;
    mainMenu("Weight Calibration Setup",0xffff,10,"Begin Calibration",firstStringColor,"",secondStringColor,"",thirdStringColor);
//    if(digitalRead(WIO_5S_PRESS) == LOW && y == 55 && pageNumber == 4){
      Serial.print("one reading:\t");
      Serial.print(scale.get_units(), 1);
      Serial.print("\t| average:\t");
      Serial.println(scale.get_units(10), 1);
  
      tft.setTextFont(3);
      tft.println("Reading:\t");
      tft.println(scale.get_units(),1);
  
      scale.power_down();
      delay(1000);
      scale.power_up();
//    }
  }
}
