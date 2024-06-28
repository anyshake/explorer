// #include "FreeRTOS.h"
// #include "semphr.h"
// #include "task.h"

// #include "ds3231/time.hpp"
// #include "ds3231/types.hpp"

// #include "gnss/parse.hpp"
// #include "gnss/types.hpp"
// #include "gnss/utils.hpp"

// #include "mcu_utils/delay.hpp"
// #include "mcu_utils/led.hpp"
// #include "mcu_utils/uart.hpp"

// typedef struct {
//     SemaphoreHandle_t i2c_bus_mutex;
//     ds3231_time_t rtc_time;
//     SemaphoreHandle_t rtc_time_mutex;
//     gnss_time_t gnss_time;
//     SemaphoreHandle_t gnss_time_mutex;
//     gnss_location_t gnss_location;
//     SemaphoreHandle_t gnss_location_mutex;
// } global_task_parameters_t;

// void setup() {
//     static global_task_parameters_t params;

//     params.rtc_time_mutex = xSemaphoreCreateMutex();
//     params.gnss_location_mutex = xSemaphoreCreateMutex();
//     params.gnss_time_mutex = xSemaphoreCreateMutex();
//     params.i2c_bus_mutex = xSemaphoreCreateMutex();

//     if (params.rtc_time_mutex == NULL || params.gnss_location_mutex == NULL
//     ||
//         params.gnss_time_mutex == NULL || params.i2c_bus_mutex == NULL) {
//         mcu_utils_led_blink(MCU_PIN_STATE, 0, 0);
//     }

//     mcu_utils_i2c_init();

//     gnss_init(GNSS_UART_BAUDRATE);
//     mcu_utils_uart_init(MCU_UART_BAUDRATE);

//     static TaskHandle_t handle_read_peripherals = NULL;
//     xTaskCreate(read_peripherals, "read_peripherals",
//     configMINIMAL_STACK_SIZE,
//                 &params, tskIDLE_PRIORITY, &handle_read_peripherals);

//     static TaskHandle_t handle_read_gnss = NULL;
//     xTaskCreate(read_gnss, "read_gnss", configMINIMAL_STACK_SIZE * 4,
//     &params,
//                 tskIDLE_PRIORITY, &handle_read_gnss);

//     static TaskHandle_t handle_send_packet = NULL;
//     xTaskCreate(send_packet, "send_packet", configMINIMAL_STACK_SIZE,
//     &params,
//                 tskIDLE_PRIORITY, &handle_send_packet);

//     mcu_utils_led_blink(MCU_PIN_STATE, 5, 0);
//     vTaskStartScheduler();
// }

// void loop() {
//     ;
// }

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial mySerial(PA12, PA11);  // RX, TX

void setup() {
    Wire.begin();
    Serial.begin(9600);
    mySerial.begin(9600);

    byte error, address;
    int nDevices;

    nDevices = 0;
    for (address = 1; address < 127; address++) {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.print(address, HEX);
            Serial.println("  !");

            nDevices++;
        } else if (error == 4) {
            Serial.print("Unknown error at address 0x");
            if (address < 16)
                Serial.print("0");
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");
}

void loop() {
    if (mySerial.available()) {
        Serial.write(mySerial.read());
    }
}
