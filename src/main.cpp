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

static void meTask1(void *pvParameters);
static void meTask2(void *pvParameters);
static void idleTask(void *pvParameters);

void setup()
{
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  xTaskCreate(meTask1, "Task1", 512, NULL, 1, NULL);
  // xTaskCreate(meTask2, "Task2", 1000, NULL, 2, NULL);
  xTaskCreate(idleTask, "Task0", 100, NULL, 0, NULL);
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

// static void meTask2(void *pvParameters)

// {
//   while (1)
//   {

//     Serial.println("In Task2");
//     vTaskDelay(2200 / portTICK_PERIOD_MS);
//     Serial.print(F("Humidity: "));
//     Serial.print(h);
//     Serial.print(F("%  Temperature: "));
//     Serial.print(t);
//     Serial.print(F("°C "));
//   }
// }

static void idleTask(void *pvParameters)
{
  while (1)
  {
    (void)pvParameters;
    //Serial.println("In Idle now");
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void loop()
{
}