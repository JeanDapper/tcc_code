#include "Preamble.h"

PushButton motorButton(motorButtonPin);
bool motorButtonState = false;
PushButton functionalTestButton(functionalTestButtonPin);
bool functionalTestButtonState = false;

Compass compass(Compass::UNDEFINED);
float robotOrientation;

GPSModule gpsModule;
float currentLatitude = 0.0,
      currentLongitude = 0.0,
      targetLatitude = 0.0,
      targetLongitude = 0.0;

PID pid(1.0, 0.0, 0.0);
float adjustment;

MotorControl motorControl(leftForwardPin, leftBackwardPin, rightForwardPin, rightBackwardPin, maxPWM);

LCD_SSD1306 lcd;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  Serial2.begin(9600);

  pinMode(rightBackwardPin, OUTPUT);
  pinMode(rightForwardPin, OUTPUT);
  pinMode(leftBackwardPin, OUTPUT);
  pinMode(leftForwardPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  lcd.begin();
  lcd.clear();
  lcd.setFontSize(FONT_SIZE_SMALL);
  lcd.println("Tracked Robot");
  lcd.println("Jean Carlos Dapper");
  
  Wire.begin();   
  if (!compass.initialize()) {   
      for (;;) {  
          delay(100);  
      }  
  }  
  
  compass.startCalibration();
  
  lcd.println("Calibrating...");
  lcd.println("Rotate the sensor");
  lcd.println("Finishing in:");
  
  lcd.setFontSize(FONT_SIZE_LARGE);
  
  long calibrationEndTime = millis() + 60000L; 
  while (millis() < calibrationEndTime) {
      int calibrationTimeRemaining = (calibrationEndTime - millis())/1000;
     
      compass.readDirection();
      
      lcd.setCursor(53, 6);
      lcd.print(calibrationTimeRemaining);
      lcd.print("s");
      
      if(calibrationTimeRemaining < 10){
          lcd.clear(70, 40, 20, 24);
      }     
      delay(20);  
  }  
  compass.endCalibration();

  lcd.clear();
  lcd.setFontSize(FONT_SIZE_SMALL);
}  


void loop() {
  if (motorButton.buttonChanged()) {
      motorButtonState = !motorButtonState;
  }
  digitalWrite(ledPin, motorButtonState);
  
  if (functionalTestButton.buttonChanged()){
      functionalTestButtonState = !functionalTestButtonState;
  }
  
  getBluetoothData();
  robotOrientation = compass.readDirection();
  gpsModule.getCurrentLocation(currentLatitude, currentLongitude);
  gpsModule.smartDelay(0);
  float azimuthAngle = azimuthCalculator(currentLatitude, currentLongitude, targetLatitude, targetLongitude);
  adjustment = pid.compute(azimuthAngle, robotOrientation);
  
  if (!functionalTestButtonState){
      if (motorButtonState){
          motorControl.automaticControl(adjustment);
      }
      else{
          motorControl.stopMotors();
      }
  }
  
  lcd.clear(127, 0, 128, 0);
  lcd.println("Tracked Robot");
  lcd.println("Jean Carlos Dapper");
  lcd.print("Orientation: ");
  lcd.println(robotOrientation);
  lcd.print("Latitude:    ");
  lcd.println(currentLatitude);
  lcd.print("Longitude:   ");
  lcd.println(currentLongitude);
  lcd.print("Azimuth:     ");
  lcd.println(azimuthAngle);
  lcd.print("PID:         ");
  lcd.println(adjustment);
  

  /*Serial.println("Orientation    Lat    Lng    Azimuth    PID");
  Serial.print(robotOrientation);
  Serial.print("  ");*/
  Serial.println(currentLatitude, 9);
  Serial.print("  ");
  Serial.print(currentLongitude, 9);
  /*Serial.print("  ");
  Serial.print(azimuthAngle);
  Serial.print("  ");
  Serial.print(adjustment)*/
}
