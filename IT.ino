#include <DHT.h>
#include <LiquidCrystal.h>
// #include <ArduinoJson.h>

// Pin configurations
#define DHTPIN 2          // DHT11 data pin connected to digital pin 2
#define DHTTYPE DHT11     // DHT11 sensor type
#define PH_PIN A0         // pH sensor connected to analog pin A0
#define SOIL_MOIST_PIN A1 // Soil moisture sensor connected to analog pin A1

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// LCD pin configuration: RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Variables
unsigned long int avgValue;  // Average value of pH sensor readings
float phValue, humidity, airTemperature, soilMoisture, soilTemperature;
int buf[10], temp;


// Create JSON document
// StaticJsonDocument<200> doc;

void setup() {
  // Begin serial communication for debugging
  Serial.begin(9600);
  
  // Begin DHT sensor
  dht.begin();
  
  // Begin LCD
  lcd.begin(16, 2);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {

  // pH Sensor Reading
  for (int i = 0; i < 10; i++) {
    buf[i] = analogRead(PH_PIN);
    delay(10);
  }
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  avgValue = 0;
  for (int i = 2; i < 8; i++) {
    avgValue += buf[i];
  }
  phValue = (float)avgValue * 5.0 / 1024 / 6;
  phValue = 3.5 * phValue;

  // Read DHT11 sensor
  float humidity = dht.readHumidity();
  float temperatureDHT = dht.readTemperature(); // Celsius by default
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float f = dht.readTemperature(true);
  
  // Check if readings are valid
  if (isnan(humidity) || isnan(temperatureDHT)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

    // Check if any reads failed and exit early (to try again).
  // if (isnan(h) || isnan(t) || isnan(f)) {
  //   Serial.println(F("Failed to read from DHT sensor!"));
  //   return;
  // }

    // Compute heat index in Fahrenheit (the default)
  // float hif = dht.computeHeatIndex(f, h);
   // Compute heat index in Celsius (isFahreheit = false)
  // float hic = dht.computeHeatIndex(t, h, false);

  // Read pH sensor (voltage to pH conversion may be needed)
  int phValue = analogRead(PH_PIN);
  float pH = map(phValue, 0, 1023, 0, 14); // Adjust range as needed
  
  // Read soil moisture sensor
  int soilMoistureValue = analogRead(SOIL_MOIST_PIN);
  int soilMoisturePercent = map(soilMoistureValue, 1023, 0, 0, 100); // Convert to percentage

// Simulated Soil Temperature (adjust if you have an actual soil temp sensor)
  soilTemperature = airTemperature - 2.0; // Assume soil temperature is slightly lower

  // Print values to Serial Monitor for debugging
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Temp (DHT): ");
  Serial.print(temperatureDHT);
  Serial.print("C, pH: ");
  Serial.print(pH);
  Serial.print(", Soil Moisture: ");
  Serial.print(soilMoisturePercent);
  Serial.println("%");

  // Serial.print(F("Humidity: "));
  // Serial.print(h);
  // Serial.print(F("%  Temperature: "));
  // Serial.print(t);
  // Serial.print(F("°C "));
  // Serial.print(f);
  // Serial.print(F("°F  Heat index: "));
  // Serial.print(hic);
  // Serial.print(F("°C "));
  // Serial.print(hif);
  // Serial.println(F("°F"));

  // Create JSON object with sensor readings
  // doc.clear();
  // doc["temperature"] = temperatureDHT;
  // doc["humidity"] = humidity;
  // doc["pH"] = pH;
  // doc["soilMoisture"] = soilMoisturePercent;
  // //doc["soilTemperature"] = soilTemperature;
    
  // Send JSON over serial
  // serializeJson(doc, Serial);
  // Serial.println();  // Add newline for parsing

  // Display data on LCD
  lcd.clear();
  
  // Display humidity and temperature (DHT11)
  lcd.setCursor(0, 0);
  lcd.print("H:");
  lcd.print(humidity);
  lcd.print("% T:");
  lcd.print(temperatureDHT);
  lcd.print("C");

  // Display pH and soil moisture on the second line
  lcd.setCursor(0, 1);
  lcd.print("pH:");
  lcd.print(pH);
  lcd.print(" SM:");
  lcd.print(soilMoisturePercent);
  lcd.print("%");

  // Delay before the next reading
  delay(2000);
}
