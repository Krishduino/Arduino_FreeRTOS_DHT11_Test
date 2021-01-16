#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float h;
float t;

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

static void meTask1(void *pvParameters);
static void meTask2(void *pvParameters);
static void idleTask(void *pvParameters);
void lcdSetup();

void setup()
{
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  lcdSetup();
  xTaskCreate(meTask1, "Task1", 512, NULL, 1, NULL);
  xTaskCreate(meTask2, "Task2", 128, NULL, 2, NULL);
  //xTaskCreate(idleTask, "Task0", 100, NULL, 0, NULL);
}

static void meTask1(void *pvParameters)
{
  while (1)
  {
    (void)pvParameters;
    dht.begin();
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    Serial.println("In Task1");
    h = dht.readHumidity();
    t = dht.readTemperature();
    if (isnan(h))
    {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    Serial.println(h);
    Serial.println(t);
  }
}

static void meTask2(void *pvParameters)
{
  while (1)
  {
    (void)pvParameters;
    Serial.println("In Task2");
    vTaskDelay(2200 / portTICK_PERIOD_MS);
    lcd.setCursor(6,0);
    lcd.print(t);
    lcd.setCursor(7,1);
    lcd.print(h);
  }
}

// static void idleTask(void *pvParameters)
// {
//   while (1)
//   {
//     (void)pvParameters;
//     //Serial.println("In Idle now");
//     vTaskDelay(50 / portTICK_PERIOD_MS);
//   }
// }

void lcdSetup()
{
  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.clear();
  lcd.home();
  lcd.setCursor(1,0);
  lcd.print("TEMP:");
  lcd.setCursor(1,1);
  lcd.print("Humid:");
}
void loop()
{
}