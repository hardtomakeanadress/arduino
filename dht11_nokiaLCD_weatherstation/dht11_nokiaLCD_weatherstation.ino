#include "U8glib.h"
#include "DHT.h"
#include "LowPower.h"
 
#define DHTPIN 2
#define LIGHTPIN A0
#define DHTTYPE DHT11

float h,t,light;
//#define backlight_pin 11
 
DHT dht(DHTPIN, DHTTYPE);
 
U8GLIB_PCD8544 u8g(3, 4, 7, 5, 6);  // CLK=8, DIN=4, CE=7, DC=5, RST=6

void draw(void) {
  int cursor_position = 50;

  if (light < 10) {
    cursor_position = 55;
  }
  
  u8g.setFont(u8g_font_profont11); 
  u8g.drawStr(0, 14, "Temp:");  
  u8g.drawStr(0, 28, "Hum:");
  u8g.drawStr(0, 42, "Light:");
  u8g.setPrintPos(50, 14 ); 
  u8g.print(t, 0);  
  u8g.drawStr(70, 14, "C");
  u8g.setPrintPos(50, 28);
  u8g.print(h, 0);  
  u8g.drawStr(70, 28, "%");
  u8g.setPrintPos(cursor_position, 42);
  u8g.print(light, 0);
  u8g.drawStr(70, 42, "%");
}
 
void setup(void) {
//  analogReference(INTERNAL); // for future testing
  dht.begin();
  pinMode(LIGHTPIN, INPUT);
}

void run_asleep() {
  u8g.firstPage();  
  do {
    u8g.setFont(u8g_font_profont11);
    u8g.drawStr(0, 20, "Zzz...sleeping");
  } 
  while( u8g.nextPage() );
}
  
void run_awake() {
  delay(1000);
  h = dht.readHumidity(); 
  t = dht.readTemperature();

  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  delay(500);  
}
 
void loop(void) {
  
  light = map(analogRead(LIGHTPIN), 0, 1023, 99, 0);
  
  if (light > 3) {
    run_awake() ;
  }
  else{
      run_asleep();
    }

  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}
