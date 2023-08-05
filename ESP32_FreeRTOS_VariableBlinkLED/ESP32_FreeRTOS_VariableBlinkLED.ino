// Serial blink rate update

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Define variable
static const int ledPin = LED_BUILTIN;
static int incomingByte = 0;
static int delayTime = 0;
// Task handles
//static TaskHandle_t task_1 = NULL;
//static TaskHandle_t task_2 = NULL;

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
  while (1) {
    if (Serial.available() > 0) {
      // read the incomming byte:

      incomingByte = Serial.parseInt();

      Serial.print("Update TimeInterval to: ");
      Serial.println(incomingByte);
      if (incomingByte > 0) {
        delayTime = incomingByte;
        Serial.println(delayTime);
      } else {
        Serial.println("Delay should be greater than zero");
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
}

void loop() {
  // put your main code here, to run repeatedly:

}
