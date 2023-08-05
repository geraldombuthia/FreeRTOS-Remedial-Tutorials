// use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else 
static const BaseType_t app_cpu = 1;
#endif

// Pins
static const int led_pin = LED_BUILTIN;

// TASK
void toggleLED(void *parameter) {
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void toggleLED2(void *parameter) {
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // Configure pin
  pinMode(led_pin, OUTPUT);

  // Task to run forever
  xTaskCreatePinnedToCore( //Use xTaskCreate() in vanilla FreeRTOS
              toggleLED, // Function  to be called
              "Toggle LED", // Name of the task
              1024,  // Stack size (bytes) or words in FreeRTOS
              NULL,  // Parameter to pass to function
              1,     // Task Priority (0 to ConfigMax_PRIORITIES - 1)
              NULL,  // Task handle
              app_cpu); // CPU to run on. Core one.  
    xTaskCreatePinnedToCore( //Use xTaskCreate() in vanilla FreeRTOS
              toggleLED2, // Function  to be called
              "Toggle LED 2", // Name of the task
              1024,  // Stack size (bytes) or words in FreeRTOS
              NULL,  // Parameter to pass to function
              1,     // Task Priority (0 to ConfigMax_PRIORITIES - 1)
              NULL,  // Task handle
              app_cpu); // CPU to run on. Core one.  
              
}

void loop() {
  // put your main code here, to run repeatedly:

}
