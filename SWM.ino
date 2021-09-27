#include<Arduino.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>//OLED LIB

#define TFT_DC    D1     // TFT DC  pin is connected to NodeMCU pin D1 (GPIO5)
#define TFT_RST   D4     // TFT RST pin is connected to NodeMCU pin D2 (GPIO4)
#define TFT_CS    D8     // TFT CS  pin is connected to NodeMCU pin D8 (GPIO15)

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

volatile int  flow_pulses1;    // Measures flow meter pulses
volatile int  flow_pulses2;
 int  lper_hour1;      // Calculated litres/hour **                     
int  lper_hour2;
byte flowmeter1 =D2;  // Flow Meter Pin number **
 byte flowmeter2 =D3;
 long int currentTime;
long int cloopTime;
//int consumption=0;
int time_count=0;
float totallitres1=0;
float totallitres2=0;
float totallitres3=0;
int count=0;
WiFiClient client; //mcu is the client, thingspeak as server
int channel_id = 1488683;
char writeapi_key[]= "4VAYZ9BKS2V02OV4";

void ICACHE_RAM_ATTR flow1 ()                  // Interrupt function
{ 
   flow_pulses1++;
} 

void ICACHE_RAM_ATTR flow2 ()                  // Interrupt function
{ 
   flow_pulses2++;
} 
void setup()
{ 
  tft.init(240, 240, SPI_MODE2);
  tft.setRotation(2);// ROTATION

   pinMode(flowmeter1, INPUT);
    pinMode(flowmeter2, INPUT);
   //pinMode(flowmeter2, INPUT);

   tftPrintTest(); //DISPLAY SWM 
   delay(2500);
   
   Serial.begin(9600); 
// wifi setup
   WiFi.begin("iPhone", "qwertyuio");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print("..");
  }
  Serial.println();
  Serial.println("NodeMCU is connected!");
  ThingSpeak.begin(client);

   //attachInterrupt(0, flow, RISING); // Setup Interrupt 
   attachInterrupt(digitalPinToInterrupt(flowmeter1), flow1, RISING);
   attachInterrupt(digitalPinToInterrupt(flowmeter2), flow2, RISING);

   sei();                            // Enable interrupts  
   currentTime = millis();
   cloopTime = currentTime;
   //Serial.println("total V");
   //Serial.print(totallitres,DEC);
} 

void loop ()    
{
   currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {     
      cloopTime = currentTime;              // Updates cloopTime
      //Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min. (Results in +/- 3% range)
      lper_hour1 = (flow_pulses1 * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flow rate in L/hour 
      lper_hour2 = (flow_pulses2 * 60 / 7.5);
      totallitres1= totallitres1+ (lper_hour1 *0.00027);
      totallitres2= totallitres2+ (lper_hour2 *0.00027);
      totallitres3=totallitres1+totallitres2;
      flow_pulses1 = 0;// Reset Counter
      flow_pulses2 = 0;
      
      //Serial.print(lper_hour, DEC);            // Print litres/hour
      //Serial.println(" L/hour");

      // sending data to cloud 
      ThingSpeak.writeField(channel_id , 1 , lper_hour1 , writeapi_key);
      ThingSpeak.writeField(channel_id , 2 , (long)totallitres1 , writeapi_key);
      ThingSpeak.writeField(channel_id , 3 , lper_hour2 , writeapi_key);
      ThingSpeak.writeField(channel_id , 4 , (long)totallitres2 , writeapi_key);
      ThingSpeak.writeField(channel_id , 5 , (long)totallitres3 , writeapi_key);
      //delay(1000);
      count++;
      if (count==2)  // 5--->15 SEC
      {
        
        Serial.print("Pipe1 Consumption:  ");
        Serial.print(totallitres1, 2);
        Serial.println("L");
        Serial.print("Pipe2 Consumption:  ");
        Serial.print(totallitres2, 2);
        Serial.println("L");
       Serial.print("Total Consumption:  ");
        Serial.print(totallitres3, 2);
        Serial.println("L");
       //LED display
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextSize(3);
        tft.setCursor(10, 30);
         tft.setTextColor(ST77XX_WHITE);
        tft.println("TOTAL VOLUME");
        tft.println("   CONSUMED");
        tft.setCursor(50, 150);
        tft.setTextSize(4);
        tft.setTextColor(ST77XX_BLUE);
        tft.print(totallitres3, 2);
        tft.print("  L");
        
        
        count=0;
      }

      
      
   }
  


   
}

void tftPrintTest() 
{
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_WHITE);// screen background color
  tft.setCursor(25, 35);//cursor pointer(x-axis,y-axis)
  tft.setTextColor(ST77XX_BLUE);//text color
  tft.setTextSize(3);// text size
  tft.println("SMART WATER");
  //tft.setCursor(25, 60);
  tft.println("    METER");
  
}
