#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define TFT_CS         D8
#define TFT_RST        D3                                           
#define TFT_DC         D4
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme;

void setup() {

  Serial.begin(9600);
  while (!Serial);
  Serial.println(F("BME680 test"));

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(2);

  delay(500);
}

void loop() {
  Serial.print("Temperature = ");
  Serial.print(bme.temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bme.pressure / 100.0);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(bme.humidity);
  Serial.println(" %");

  Serial.print("Gas = ");
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(" KOhms");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.println();

  // Temperature
  tft.setTextSize(1);
  tft.setTextColor(ST7735_RED);
  tft.setCursor(5, 5);
  tft.print("Temperatur:");

  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(5, 15);
  tft.print(bme.temperature);

  tft.setTextSize(2);
  tft.setTextColor(ST7735_BLUE);
  tft.setCursor(70, 15);
  tft.print("C");

  // Pressure
  tft.setTextSize(1);
  tft.setTextColor(ST7735_RED);
  tft.setCursor(5, 40);
  tft.print("Lufttryck:");

  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(5, 50);
  tft.print(bme.pressure / 100.0);

  tft.setTextSize(2);
  tft.setTextColor(ST7735_BLUE);
  tft.setCursor(85, 50);
  tft.print("hPa");

  float tryck = bme.pressure / 100.0;

  if(tryck >= 1001){
    tft.setTextSize(1);
    tft.setTextColor(ST7735_YELLOW);
    tft.setCursor(5, 67);
    tft.print("HOGTRYCK");
  } else if (tryck < 1000){
    tft.setTextSize(1);
    tft.setTextColor(ST7735_YELLOW);
    tft.setCursor(5, 67);
    tft.print("LAGTRYCK");
  }

  // Humidity
  tft.setTextSize(1);
  tft.setTextColor(ST7735_RED);
  tft.setCursor(5, 85);
  tft.print("Luftfuktighet:");

  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(5, 95);
  tft.print(bme.humidity);

  tft.setTextSize(2);
  tft.setTextColor(ST7735_BLUE);
  tft.setCursor(70, 95);
  tft.print("%");

  // Gas
  tft.setTextSize(1);
  tft.setTextColor(ST7735_RED);
  tft.setCursor(5, 120);
  tft.print("TVOC (kohms):");

  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(5, 130);
  tft.print(bme.gas_resistance / 1000.0);

  float gas = bme.gas_resistance / 1000.0;

  if(gas > 90)
  {
    tft.setTextSize(1);
    tft.setTextColor(ST7735_GREEN);
    tft.setCursor(5, 147);
    tft.print("REN LUFT");
  } else if(gas > 60 || gas < 89){
    tft.setTextSize(1);
    tft.setTextColor(ST7735_GREEN);
    tft.setCursor(5, 147);
    tft.print("MINDRE REN LUFT");
  } else if(gas >30 || gas < 59){
    tft.setTextSize(1);
    tft.setTextColor(ST7735_GREEN);
    tft.setCursor(5, 147);
    tft.print("SMUTSIG LUFT");
  } else if(gas > 0 || gas < 29){
    tft.setTextSize(1);
    tft.setTextColor(ST7735_GREEN);
    tft.setCursor(5, 147);
    tft.print("FARLIG LUFT");
  } else {
    tft.setTextSize(1);
    tft.setTextColor(ST7735_GREEN);
    tft.setCursor(5, 147);
    tft.print("NO READING");
  }
  delay(5000);

  tft.fillScreen(ST7735_BLACK);
}
