#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_VEML7700.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define the OLED display size
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1

// Create sensor and display objects
Adafruit_BME280 bme;
Adafruit_VEML7700 veml = Adafruit_VEML7700();
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define light pin and thresholds
const int lightPin = 1; // change as per your setup
const float tempThreshold = 30.0; // temperature threshold in Celsius
const float lightLowThreshold = 50.0; // light threshold for low light in lux
const float lightHighThreshold = 100.0; // light threshold for high light in lux

void setup() {
  Serial.begin(9600);
  delay(5000);
  Serial.println("11111111111");

  // Initialize VEML7700
  if (!veml.begin()) {
    Serial.println("Could not find a valid VEML7700 sensor, check wiring!");
    //while (1);
  }
  else{
    Serial.println("VEML7700 working fine!");
  }

  // Initialize BME280
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    //while (1);
  }
  else{
    Serial.println("working fine!");
  }

  // Initialize OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    //for(;;);
  }
    else{
    Serial.println("SSD1306 working fine!");
  }


  display.display();
  delay(2000);
  display.clearDisplay();

  // Initialize light pin
  pinMode(lightPin, OUTPUT);
}

void loop() {
  // Read sensors
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F; // Convert to hPa
  float light = veml.readLux();

  Serial.println(temperature);

  // Display sensor data on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Temp: "); display.print(temperature); display.println(" C");
  display.print("Hum: "); display.print(humidity); display.println(" %");
  display.print("Pres: "); display.print(pressure); display.println(" hPa");
  display.print("Light: "); display.print(light); display.println(" Lux");
  display.display();


  // Check light intensity and control light
  if (light < lightLowThreshold) {
    digitalWrite(lightPin, LOW); // open the light
  } else if (light > lightHighThreshold) {
    digitalWrite(lightPin, HIGH); // Stop opening the light
  }



  // Wait a bit before reading again
  delay(1000);
}