/*

  version one of this code had an issue with the delayTime read from the serial
  being set to zero immediately after probably due to the '\n'
  This version seeks to change that by reading a character at a time

*/
#include <stdlib.h>
// Use core 1 for this practise
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Set variables
static const int ledPin = LED_BUILTIN;

static int delayTime = 1000; // initial delay
static const uint8_t buf_len = 20;

// Tasks
// Task 1
void blinkLed(void *parameter) {
  while (1) {
    digitalWrite(ledPin, HIGH);
    vTaskDelay(delayTime / portTICK_PERIOD_MS);
    digitalWrite(ledPin, LOW);
    vTaskDelay(delayTime / portTICK_PERIOD_MS);
  }
}

// Task 2
void readSerial(void *parameter) {
  char c;
  char buf[buf_len];
  uint8_t idx = 0;

  //clear whole buffer
  memset(buf, 0, buf_len);
  while (1) {
    if (Serial.available() > 0) {
      // read the incoming byte:
      c = Serial.read();

      // Update delay variable and reset the buffer if we get a newline character
      if (c == '\n') {
        delayTime = atoi(buf);
        Serial.print("updated LED delay to: ");
        Serial.println(delayTime);
        memset(buf, 0, buf_len);
        idx = 0;
      } else {
        // only append if index is not over message limit
        if (idx < buf_len - 1) {
          buf[idx] = c;
          idx++;
        }
      }
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  while (!Serial) {
    ; // wait for serial port to connect
  }
  Serial.println("___FREERTOS LED Variable BLINK___");
  xTaskCreatePinnedToCore(
    blinkLed,
    "Blink Led",
    1024,
    NULL,
    1,
    NULL,
    app_cpu);
  xTaskCreatePinnedToCore(
    readSerial,
    "Read Serial",
    1024,
    NULL,
    1,
    NULL,
    app_cpu);
  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}
