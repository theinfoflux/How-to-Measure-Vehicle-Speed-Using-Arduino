#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define IR1 2
#define IR2 3

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

float distance = 0.10; // distance between sensors in meters (10 cm)

unsigned long startTime = 0;
unsigned long endTime = 0;

bool started = false;

unsigned long timeout = 3000; // 3 seconds timeout

void showZero()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print("0 Km/hr");
  display.setCursor(0, 35);
  display.print("0 m/s");
  display.display();
}

void setup()
{
  Serial.begin(9600);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(2000);

  display.clearDisplay();
  display.setTextColor(WHITE);

  showZero(); // show 0 at start
}

void loop()
{

  if (digitalRead(IR1) == LOW && !started)
  {
    startTime = millis();
    started = true;
  }

  if (digitalRead(IR2) == LOW && started)
  {
    endTime = millis();
    started = false;

    float timeTaken = (endTime - startTime) / 1000.0;

    if (timeTaken > 0)
    {
      float speed = distance / timeTaken;
      float speedKmph = speed * 3.6;

      Serial.print("Speed in m/s: ");
      Serial.println(speed);
      Serial.print("Speed in km/hr: ");
      Serial.println(speedKmph);

      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 10);
      display.print(speedKmph);
      display.print(" Km/hr");
      display.setCursor(0, 35);
      display.print(speed);
      display.print(" m/s");
      display.display();
    }

    delay(3000);
    showZero(); // return to zero
  }

  // Timeout condition
  if (started && (millis() - startTime > timeout))
  {
    started = false;
    showZero();
  }
}
