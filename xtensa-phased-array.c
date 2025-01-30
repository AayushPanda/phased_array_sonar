#include "driver/gpio.h"
#include "driver/timer.h"
#include "driver/dac.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "esp_pm.h"
#include <math.h>

// Configuration
#define TX_LENGTH 6                 // 6 bits per wave
#define OUTPUT_PORT GPIO_NUM_21     // Output port for TX array
#define ANGLE_OUTPUT_PIN DAC_CHANNEL_1  // DAC pin for angle output (GPIO25)
#define NUM_ANGLES 60               // Number of angle steps to compute
#define ANGLE_DELAY_MS 50           // Delay between angle changes in milliseconds

const int c = 343;                  // Speed of sound in m/s
const float pitch = 0.01;           // Distance between TXR nodes in m
const int f = 120;                  // Interrupt frequency in kHz
const int clkspeed = 240000;        // 240MHz typical clock for ESP32

// Global variables
uint8_t phases[TX_LENGTH];
uint8_t counter = 0;
volatile int currentAngle = 0;
volatile bool updateAngle = false;

// GPIO output array (8 pins starting from OUTPUT_PORT)
#define GPIO_OUTPUT_PIN_SEL ((1ULL<<OUTPUT_PORT) | (1ULL<<(OUTPUT_PORT+1)) | \
                            (1ULL<<(OUTPUT_PORT+2)) | (1ULL<<(OUTPUT_PORT+3)) | \
                            (1ULL<<(OUTPUT_PORT+4)) | (1ULL<<(OUTPUT_PORT+5)) | \
                            (1ULL<<(OUTPUT_PORT+6)) | (1ULL<<(OUTPUT_PORT+7)))

// Generate bits with phase shift
void genChannelBits(uint8_t arr[], int bitShift) {
    for (int i = 0; i < TX_LENGTH; i++) {
        if (i % 6 <= 2) {
            arr[(bitShift + i) % TX_LENGTH] = 1;
        }
    }
}

// Generate composite signal pattern
void genCompositeSignal(uint8_t arr[], int bitDelay) {
    for (int i = 0; i < TX_LENGTH; i++) arr[i] = 0;

    for (int txr = 0; txr < 8; txr++) {
        uint8_t tmp[TX_LENGTH] = {0};
        genChannelBits(tmp, (txr * bitDelay) % TX_LENGTH);
        for (int i = 0; i < TX_LENGTH; i++) {
            arr[i] |= tmp[i] << txr;
        }
    }
}

// Calculate delay from angle
int calculateBitDelayForAngle(float angle) {
    float angleRad = angle * M_PI / 180.0;
    float wavelength = c / (f * 1000.0);
    float delay = sin(angleRad) * pitch / wavelength * TX_LENGTH;
    return (int)(delay + 0.5);
}

// Timer ISR
static void IRAM_ATTR timer_isr(void* arg) {
    uint8_t value = phases[counter];

    // Clear 8 GPIOs
    GPIO.out_w1tc = (0xFF << OUTPUT_PORT);

    // Set GPIOs according to value
    GPIO.out_w1ts = ((uint32_t)value << OUTPUT_PORT);

    counter++;
    if (counter == TX_LENGTH) {
        counter = 0;

        if (updateAngle) {
            updateAngle = false;

            float angleDegrees = -60.0 + (currentAngle * 120.0 / (NUM_ANGLES - 1));
            int bitDelay = calculateBitDelayForAngle(angleDegrees);
            genCompositeSignal(phases, bitDelay);

            uint8_t dacValue = (uint8_t)((currentAngle * 255) / (NUM_ANGLES - 1));
            dac_output_voltage(ANGLE_OUTPUT_PIN, dacValue);
        }
    }

    TIMERG0.int_clr_timers.t0 = 1;
    TIMERG0.hw_timer[0].config.alarm_en = 1;
}

// Angle sweep task
void angle_sweep_task(void *pvParameters) {
    while (1) {
        for (currentAngle = 0; currentAngle < NUM_ANGLES; currentAngle++) {
            updateAngle = true;
            while (updateAngle) {
                vTaskDelay(1);
            }
            vTaskDelay(pdMS_TO_TICKS(ANGLE_DELAY_MS));
        }
    }
}

// Timer setup
void setup_timer() {
    timer_config_t config = {
        .alarm_en = true,
        .counter_en = false,
        .intr_type = TIMER_INTR_LEVEL,
        .counter_dir = TIMER_COUNT_UP,
        .auto_reload = true,
        .divider = 2
    };

    timer_init(TIMER_GROUP_0, TIMER_0, &config);
    timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);

    uint64_t alarm_value = clkspeed / (f * 2 * TX_LENGTH);
    timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, alarm_value);

    timer_isr_register(TIMER_GROUP_0, TIMER_0, timer_isr, NULL, ESP_INTR_FLAG_IRAM, NULL);
    timer_enable_intr(TIMER_GROUP_0, TIMER_0);
    timer_start(TIMER_GROUP_0, TIMER_0);
}

// Initial setup
void setup() {
    gpio_config_t io_conf = {
        .pin_bit_mask = GPIO_OUTPUT_PIN_SEL,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    dac_output_enable(ANGLE_OUTPUT_PIN);
    genCompositeSignal(phases, 0);
    setup_timer();

    xTaskCreate(angle_sweep_task, "angle_sweep", 2048, NULL, 5, NULL);

    esp_pm_config_esp32_t pm_config = {
        .max_freq_mhz = 240,
        .min_freq_mhz = 240,
        .light_sleep_enable = false
    };
    esp_pm_configure(&pm_config);
}

// App entry
void app_main() {
    setup();
}



