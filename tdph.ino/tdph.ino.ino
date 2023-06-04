#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SimpleTimer.h>

SimpleTimer timer;

float calibration_value = 21.34 - 0.7;
int phval = 0; 
unsigned long int avgval; 
int buffer_arr[10],temp;

float ph_act;
// for the OLED display

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#include <EEPROM.h>
#include <GravityTDS.h>
 
#define TdsSensorPin A1
GravityTDS gravityTds;
 
float temperature = 25,tdsValue = 0;
 
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() 
{
  Wire.begin();
 Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE); 
  gravityTds.setPin(TdsSensorPin);
  gravityTds.setAref(5.0);  //reference voltage on ADC, default 5.0V on Arduino UNO
  gravityTds.setAdcRange(1024);  //1024 for 10bit ADC;4096 for 12bit ADC
  gravityTds.begin();

 unsigned long int x =500;
timer.setInterval(x);
}
void loop() {
  timer.reset(); // Initiates SimpleTimer
 for(int i=0;i<10;i++) 
 { 
 buffer_arr[i]=analogRead(A0);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*5.0/1024/7; 
  ph_act = -5.70 * volt-0.5 + calibration_value;

 Serial.print("pH Val: ");
 Serial.print(ph_act);
 Serial.println("");
 delay(1000);

    // temperature = readTemperature();  //add your temperature sensor and read it
  gravityTds.setTemperature(temperature);  // set the temperature and execute temperature compensation
  gravityTds.update();  //sample and calculate
  tdsValue = gravityTds.getTdsValue();  // then get the value
  Serial.print(tdsValue,0);
  Serial.println("ppm");
  delay(1000);
   display_pHValue();
}

void display_pHValue()
{
     // display on Oled display

   // Oled display
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,0); // column row
  display.print("pH:");

  display.setTextSize(2);
  display.setCursor(55, 0);
  display.print(ph_act);
    
  display.setTextSize(2);
  display.setCursor(0,2); // column row
  display.print("TDS:");

  display.setTextSize(2);
  display.setCursor(55, 0);
  display.print(tdsValue);
  display.display();

 

}