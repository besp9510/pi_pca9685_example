// Raspberry Pi PCA9685 Example
//
// Copyright (c) 2022 Benjamin Spencer
// ============================================================================
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// ============================================================================

// Include C standard libraries:
#include <stdlib.h> // C Standard library
#include <stdio.h>  // C Standard I/O libary
#include <time.h>   // C Standard date and time manipulation
#include <stdint.h> // C Standard integer types

#include <pi_i2c.h>     // Pi I2C library!
#include <pi_lw_gpio.h> // Pi GPIO library!

#include "pca9685_registers.h" // PCA9685 register definitions

// Round to the nearest integer
#define ROUND(n) (((n - (int)n) >= 0.5) ? (int)(n + 1) : (int)(n))

// Turn the device on and off
#define DEVICE_POWER_GPIO 4 // UPDATE

// Testing PCA9685 "16-channel, 12-bit PWM Fm+ I2C-bus LED controller" per
// the datasheet (can find under doc/pca9685.pdf)

void reboot_device(void) {
    printf("Attempting to reboot the device!\n");

    // Toggle GPIO off and on to reboot device:
    gpio_clear(DEVICE_POWER_GPIO);
    sleep(1);
    gpio_set(DEVICE_POWER_GPIO);

    printf("Reboot done\n");
}

int i2c_error_handler(int errno) {
    // An I2C error may be fatal or maybe something that we can recover from
    // or even ignore all together:
    switch (errno) {
        case -ENACK:
            printf("I2C Error! Encountered ENACK\n");
            // Try rebooting device:
            reboot_device();
            break;
        case -EBADXFR:
            printf("I2C Error! Encountered EBADXFR\n");
            // Try rebooting device:
            reboot_device();
            break;
        case -EBADREGADDR:
            printf("I2C Error! Encountered EBADREGADDR\n");
            // Try rebooting device:
            reboot_device();
            break;
        case -ECLKTIMEOUT:
            printf("I2C Error! Encountered ECLKTIMEOUT\n");
            // Try rebooting device:
            reboot_device();
            break;
        case -ENACKRST:
            printf("I2C Error! Encountered ENACKRST\n");
            // Try rebooting device:
            reboot_device();
            break;
        case -EBUSLOCKUP:
            printf("I2C Error! Encountered EBUSLOCKUP\n");
            // Try rebooting device:
            reboot_device();
            break;
        case -EBUSUNKERR:
            printf("I2C Error! Encountered EBUSUNKERR\n");
            // Try rebooting device:
            reboot_device();
            break;
        case -EFAILSTCOND:
            printf("I2C Error! Encountered EFAILSTCOND\n");
            // Try rebooting device:
            reboot_device();
            break;
        case -EDEVICEHUNG:
            printf("I2C Error! Encountered EDEVICEHUNG\n");
            // Try rebooting device:
            reboot_device();
            break;
        default:
            break;
    };

    return 0;
}

int scan_for_device(uint8_t device_addr) {
    // Address book passed to returned by the function:
    int address_book[127];

    int ret;

    if ((ret = scan_bus_i2c(address_book)) < 0) {
        i2c_error_handler(ret);
    };

    // Check and see if PCA9685 was detected on the bus (if not then we can't
    // really continue with the test):
    if (address_book[device_addr] != 1) {
        printf("Device was not detected at 0x%X\n", device_addr);
        return -1;
    };

    printf("Device was detected at 0x%X\n", device_addr);

    return 0;
}

// Set device configuration by writing to a register address
int configure_device(int device_addr, int reg_addr, int *configs,
                     int num_configs) {
    int reg_value[1] = {0};

    int i;
    int ret;

    printf("Configuring device 0x%X\n", device_addr);

    // Get current register value to apply the options to:
    if ((ret = read_i2c(device_addr, reg_addr, reg_value, 0x01)) < 0) {
        i2c_error_handler(ret);
        return ret;
    };

    printf("Register 0x%X currently reads 0x%X\n", reg_addr, reg_value[0]);

    // Go through all input options to come up with the final register value:
    for (i = 0; i < num_configs; i++) {
        // AND the current value of the register with a mask to clear the
        // bit of the option being sent then OR it with the option to arrive
        // to the final value of the register:
        reg_value[0] = (reg_value[0] & ~(0x01 << (configs[i] >> 8)))
                        | (configs[i] << (configs[i] >> 8));
    };

    printf("Setting register 0x%X to 0x%X\n", reg_addr, reg_value[0]);

    if ((ret = write_i2c(device_addr, reg_addr, reg_value, 0x01)) < 0) {
        i2c_error_handler(ret);
        return ret;
    };

    printf("Device configured\n");

    return 0;
}

// Set LED PWM duty cycle (12 bit resolution: 0 = 0%; 4095 = 100%):
int set_pwm_duty_cycle(int device_addr, int led_id, int duty_cycle) {
    int led_on_l = 0;
    int led_on_h = 0;
    int led_off_l = 0;
    int led_off_h = 0;

    int led_delay_time = 0;
    int led_on_counts = 0;
    int led_off_time = 0;

    int ret;

    printf("Setting duty cycle on PCA9685 device 0x%X to %d\n",
            device_addr, duty_cycle);

    // Assume 10% delay time:
    led_delay_time = ROUND(0.10 * 4096) - 1;

    // See page 16 for example on how to calculate LEDn_ON & LEDn_OFF

    // Calculate LED off time if less than 100%:
    if (duty_cycle < 1) {
        // Find LED off time based on the LED on counts
        led_on_counts = duty_cycle;
        led_off_time = led_delay_time + led_on_counts;

        // Deal with frame count wrapping:
        if ((led_delay_time + led_on_counts > 4096)) {
            led_off_time = led_off_time - 4096;
        };
    } else {
        // LED full on:
        led_delay_time = led_delay_time | (0x01 << 12);
    };

    // Lower gets 8 LSBs and higher gets 4 MSBs
    led_on_l = led_delay_time;
    led_on_h = led_delay_time >> 8;
    led_off_l = led_off_time;
    led_off_h = led_off_time >> 8;

    // Package up into a nice array:
    int led_register_values[4] = {led_on_l, led_on_h, led_off_l, led_off_h};

    printf("Calculated LED%d ON/OFF registers for duty cycle = %.3f%%\n",
           led_id, duty_cycle * 100);
    printf("led_delay_time = %d\n", (uint8_t) led_delay_time);
    printf("led_on_counts = %d\n", (uint8_t) led_on_counts);
    printf("led_off_time = %d\n", (uint8_t) led_off_time);
    printf("led_on_l = 0x%x\n", (uint8_t) led_on_l);
    printf("led_on_h = 0x%x\n", (uint8_t) led_on_h);
    printf("led_off_l = 0x%x\n", (uint8_t) led_off_l);
    printf("led_off_h = 0x%x\n", (uint8_t) led_off_h);

    printf("Setting register 0x%X to 0x%X\n", (6 + led_id * 4),
           (6 + led_id * 4 + 3));

    if ((ret = write_i2c(device_addr, (6 + led_id * 4),
         led_register_values, 4)) < 0) {
        i2c_error_handler(ret);
        return ret;
    };

    printf("Duty cycle set\n");

    return duty_cycle_actual;
}

// Set frequency (can only occur when device in low power mode):
int set_frequency(int device_addr, int frequency) {
    // Internal clock:
    int clock_frequency = 25; // [MHz]

    int prescale_value[1] = {0};

    int ret;

    printf("Setting frequency on device 0x%X\n", device_addr);

    // Valid prescale values range from 0x03 (1526 Hz) to 0xFF (24 Hz):
    if (frequency >= 1526) {
        prescale_value[0] = 0x03;
    } else if (frequency <= 24) {
        prescale_value[0] = 0xFF;
    } else {
        // Calculate the prescale value based on equation (1) on page 25:
        prescale_value[0] = (clock_frequency / (4096 * frequency)) - 1;
    };

    printf("Calculated prescale value for desired frequency = %d\n", frequency);
    printf("prescale_value = 0x%X\n", prescale_value[0]);

    if ((ret = write_i2c(device_addr, PRE_SCALE, prescale_value, 1)) < 0) {
        i2c_error_handler(ret);
        return ret;
    };

    printf("Frequency set\n");

    return 0;
}

int main(void) {
    // PCA9685 device address (page 8):
    int pca9685_addr = 0x70;

    // Use the default I2C pins:
    // Ensure that Raspian I2C interface is disabled via rasp-config otherwise
    // risk unpredictable behavior!
    int sda_pin = 2; // UPDATE
    int scl_pin = 3; // UPDATE

    int duty_cycle = 2048;            // UPDATE
    int frequency = 1526;             // UPDATE
    int led_id = 15;                  // UPDATE
    int speed_grade = I2C_FULL_SPEED; // UPDATE

    int ret;

    printf("Begin test_pca9685.c\n");
    printf("Configuring pi_i2c:\n");
    printf("sda_pin = %d\n", sda_pin);
    printf("sda_pin = %d\n", scl_pin);
    printf("speed_grade = %d Hz\n", speed_grade);

    // Configure at standard mode:
    if ((ret = config_i2c(sda_pin, scl_pin, speed_grade)) < 0 ) {
        printf("config_i2c() failed to configure and returned %d\n", ret);
        return ret;
    };

    // Check to see if the device is present prior to interacting with device:
    if ((ret = scan_for_device(pca9685_addr)) < 0) {
        return ret;
    };

    // Set the following settings:
    // - Use internal clock
    // - Register auto-increment enabled
    // - Do not respond to subaddress 1, 2, or 3
    // - Responds to all call
    // - Low power
    int config[7] = {INT_CLOCK, AI, ALLCALL, NO_SUB1, NO_SUB2,
                     NO_SUB3, LOW_POWER};

    if ((ret = configure_device(pca9685_addr, MODE1, config, 7)) < 0) {
        return ret;
    };

    // Set the frequency:
    if ((ret = set_frequency(pca9685_addr, frequency)) < 0) {
        return ret;
    };

    // Set the following settings:
    // - Normal power
    config[0] = NORMAL_MODE;

    if ((ret = configure_device(pca9685_addr, MODE1, config, 1)) < 0) {
        return ret;
    };

    // Set the duty cycle:
    if ((ret = set_pwm_duty_cycle(pca9685_addr, led_id, duty_cycle)) < 0) {
        return ret;
    };

    // Wait some time to watch the output
    sleep(5);

    printf("Finished test\n");

    // Set the following settings:
    // - Low power mode
    config[0] = LOW_POWER;

    if ((ret = configure_device(pca9685_addr, MODE1, config, 1)) < 0) {
        return ret;
    };

    printf("Device now in a low power mode\n");

    return 0;
}
