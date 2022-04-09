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

// PCA9685 register address, defaults, and masks from data sheet 

// Register addresses (page 10 to 13):
#define MODE1 0x00      // Mode register 1
#define MODE2 0x01      // Mode register 1
#define SUBADR1 0x02    // I2C bus subaddress 1
#define SUBADR2 0x03    // I2C bus subaddress 1
#define SUBADR3 0x04    // I2C bus subaddress 1
#define ALLCALLADR 0x05 // I2C bus subaddress 1
#define PRE_SCALE 0xFE  // prescalar for PWM output frequency
#define TestMode 0xFF   // defines the test mode to be entered

#define LED0_ON_L 0x06   // LED0 output and brightness controll byte 0
#define LED0_ON_H 0x07   // LED0 output and brightness controll byte 1
#define LED0_OFF_L 0x08  // LED0 output and brightness controll byte 2
#define LED0_OFF_H 0x09  // LED0 output and brightness controll byte 3
#define LED1_ON_L 0x0A   // LED1 output and brightness controll byte 0
#define LED1_ON_H 0x0B   // LED1 output and brightness controll byte 1
#define LED1_OFF_L 0x0C  // LED1 output and brightness controll byte 2
#define LED1_OFF_H 0x0D  // LED1 output and brightness controll byte 3
#define LED2_ON_L 0x0E   // LED2 output and brightness controll byte 0
#define LED2_ON_H 0x0F   // LED2 output and brightness controll byte 1
#define LED2_OFF_L 0x10  // LED2 output and brightness controll byte 2
#define LED2_OFF_H 0x11  // LED2 output and brightness controll byte 3
#define LED3_ON_L 0x12   // LED3 output and brightness controll byte 0
#define LED3_ON_H 0x13   // LED3 output and brightness controll byte 1
#define LED3_OFF_L 0x14  // LED3 output and brightness controll byte 2
#define LED3_OFF_H 0x15  // LED3 output and brightness controll byte 3
#define LED4_ON_L 0x16   // LED4 output and brightness controll byte 0
#define LED4_ON_H 0x17   // LED4 output and brightness controll byte 1
#define LED4_OFF_L 0x18  // LED4 output and brightness controll byte 2
#define LED4_OFF_H 0x19  // LED4 output and brightness controll byte 3
#define LED5_ON_L 0x1A   // LED5 output and brightness controll byte 0
#define LED5_ON_H 0x1B   // LED5 output and brightness controll byte 1
#define LED5_OFF_L 0x1C  // LED5 output and brightness controll byte 2
#define LED5_OFF_H 0x1D  // LED5 output and brightness controll byte 3
#define LED6_ON_L 0x1E   // LED6 output and brightness controll byte 0
#define LED6_ON_H 0x1F   // LED6 output and brightness controll byte 1
#define LED6_OFF_L 0x20  // LED6 output and brightness controll byte 2
#define LED6_OFF_H 0x21  // LED6 output and brightness controll byte 3
#define LED7_ON_L 0x22   // LED7 output and brightness controll byte 0
#define LED7_ON_H 0x23   // LED7 output and brightness controll byte 1
#define LED7_OFF_L 0x24  // LED7 output and brightness controll byte 2
#define LED7_OFF_H 0x25  // LED7 output and brightness controll byte 3
#define LED8_ON_L 0x26   // LED8 output and brightness controll byte 0
#define LED8_ON_H 0x27   // LED8 output and brightness controll byte 1
#define LED8_OFF_L 0x28  // LED8 output and brightness controll byte 2
#define LED8_OFF_H 0x29  // LED8 output and brightness controll byte 3
#define LED9_ON_L 0x2A   // LED9 output and brightness controll byte 0
#define LED9_ON_H 0x2B   // LED9 output and brightness controll byte 1
#define LED9_OFF_L 0x2C  // LED9 output and brightness controll byte 2
#define LED9_OFF_H 0x2D  // LED9 output and brightness controll byte 3
#define LED10_ON_L 0x2E  // LED10 output and brightness controll byte 0
#define LED10_ON_H 0x2F  // LED10 output and brightness controll byte 1
#define LED10_OFF_L 0x30 // LED10 output and brightness controll byte 2
#define LED10_OFF_H 0x31 // LED10 output and brightness controll byte 3
#define LED11_ON_L 0x32  // LED11 output and brightness controll byte 0
#define LED11_ON_H 0x33  // LED11 output and brightness controll byte 1
#define LED11_OFF_L 0x34 // LED11 output and brightness controll byte 2
#define LED11_OFF_H 0x35 // LED11 output and brightness controll byte 3
#define LED12_ON_L 0x36  // LED12 output and brightness controll byte 0
#define LED12_ON_H 0x37  // LED12 output and brightness controll byte 1
#define LED12_OFF_L 0x38 // LED12 output and brightness controll byte 2
#define LED12_OFF_H 0x39 // LED12 output and brightness controll byte 3
#define LED13_ON_L 0x3A  // LED12 output and brightness controll byte 0
#define LED13_ON_H 0x3B  // LED12 output and brightness controll byte 1
#define LED13_OFF_L 0x3C // LED12 output and brightness controll byte 2
#define LED13_OFF_H 0x3D // LED12 output and brightness controll byte 3
#define LED14_ON_L 0x3E  // LED14 output and brightness controll byte 0
#define LED14_ON_H 0x3F  // LED14 output and brightness controll byte 1
#define LED14_OFF_L 0x40 // LED14 output and brightness controll byte 2
#define LED14_OFF_H 0x41 // LED14 output and brightness controll byte 3
#define LED15_ON_L 0x42  // LED15 output and brightness controll byte 0
#define LED15_ON_H 0x43  // LED15 output and brightness controll byte 1
#define LED15_OFF_L 0x44 // LED15 output and brightness controll byte 2
#define LED15_OFF_H 0x45 // LED15 output and brightness controll byte 3

#define ALL_LED_ON_L 0xFA  // load all the LEDn_ON registers, byte 0
#define ALL_LED_ON_H 0xFB  // load all the LEDn_ON registers, byte 0
#define ALL_LED_OFF_L 0xFC // load all the LEDn_ON registers, byte 0
#define ALL_LED_OFF_H 0xFD // load all the LEDn_ON registers, byte 0

// Register defaults (page 21 to 26):
#define MODE1_DEFAULT 0x11         // (= 00010001)
#define MODE2_DEFAULT 0x04         // (= 00000100)
#define SUBADR1_DEFAULT 0xE2       // (= 11100010)
#define SUBADR2_DEFAULT 0xE4       // (= 11100100)
#define SUBADR3_DEFAULT 0xE8       // (= 11101000)
#define ALLCALLADR_DEFAULT 0xE0    // (= 11100000)
#define LEDN_ON_L_DEFAULT 0x00     // (= 00000000)
#define LEDN_ON_H_DEFAULT 0x00     // (= 00000000)
#define LEDN_OFF_L_DEFAULT 0x00    // (= 00000000)
#define LEDN_OFF_H_DEFAULT 0x08    // (= 00010000)
#define ALL_LED_ON_L_DEFAULT 0x00  // (= 00000000)
#define ALL_LED_ON_H_DEFAULT 0x08  // (= 00010000)
#define ALL_LED_OFF_L_DEFAULT 0x00 // (= 00000000)
#define ALL_LED_OFF_H_DEFAULT 0x08 // (= 00010000)
#define PRE_SCALE_DEFAULT 0x1E     // (= 00011110)

// Register setting masks (page 24-30):
// (OR the bit location of the setting at the end of the byte)
#define NORMAL_MODE 0x00 | (0x04 << 8) // Normal mode
#define LOW_POWER 0x01 | (0x04 << 8)   // Low power mode.
                                       // Oscillator off. No PWM control.
#define RESTART 0x01 | (0x07 << 8)     // Restart enabled
#define INT_CLOCK 0x00 | (0x06 << 8)   // Use internal clock
#define EXT_CLOCK 0x01 | (0x06 << 8)   // Use EXTCLK pin clock

#define AI 0x01 | (0x05 << 8)    // Regsister auto-increment enabled
#define NO_AI 0x00 | (0x05 << 8) // Regsister auto-increment enabled

#define ALLCALL 0x01               // Responds to LED all call I2C addr
#define NO_ALLCALL 0x00            // Does not respond to LED all call
                                   // I2C address
#define SUB1 0x01 | (0x03 << 8)    // Responds to I2C subaddress 1
#define NO_SUB1 0x00 | (0x03 << 8) // Does not respond to I2C sub addr 1
#define SUB2 0x01 | (0x02 << 8)    // Responds to I2C subaddress 2
#define NO_SUB2 0x00 | (0x02 << 8) // Does not respond to I2C sub addr 2
#define SUB3 0x01 | (0x01 << 8)    // Responds to I2C sub addr 3
#define NO_SUB3 0x00 | (0x01 << 8) // Does not respond to I2C sub addr 3

#define LED_OUTPUT_ENABLE 0x00 | (0x04 << 4)  // Use LED ON and OFF counts
#define LED_OUTPUT_DISABLE 0x01 | (0x04 << 4) // LED always OFF