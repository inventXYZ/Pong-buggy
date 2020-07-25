/** This file:
 * represents the types of hardware we are using with code
 * lists what pins each device is connected to
 * defines functions for hardware
 */

/**
 * Defining the hardware types we are using
 */
// since a pin is always a positive number
// create an unsigned integer, aka uint

// use const because while the program is running the value won't change

struct LED {
    const uint pin;
    void blink(uint frequency, uint duration);
};

struct Buzzer {
    const uint pin;
    void play_sound(uint frequency, uint duration);
};

struct Button {
    const uint pin;
    bool pressed;  // bool means true or false
};

// libraries for the screen
#include <U8g2lib.h>
#include <Wire.h>

/**
 * Listing what pin each device is connected to
 */
// If you plugged things in differently, make sure to change the pin number value here

// green_LED is an LED that is connected to pin 16 of the Microcontroller
LED green_LED{.pin = 16};
LED red_LED{.pin = 17};

// buzzer is a Buzzer that is connected to pin 5 of the Microcontroller
Buzzer buzzer{.pin = 5};

Button down_button{.pin = 6};
Button up_button{.pin = 7};

// OLED display is connected through a QWIIC cable
U8G2_SSD1327_MIDAS_128X128_F_HW_I2C screen(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
// U8G2 is the display library
// SSD1327 is the name of the controller chip on the screen

// Midas is a company that makes rhe screen

// 128x128 means the display is 128 pixels wide and 128 pixels tall

// F means that we store a Full copy (a "buffer") of the image we are going to send to the screen
// then when we want the screen to display it, we use screen.sendBuffer()

// I2C is the protocol the screen uses to communicate with the Microcontroller

// U8G2_R0 means the content on the display is rotated (R) 0 degrees.
// If you want to rotate your display, you can use:
// U8G2_R0	No rotation, landscape
// U8G2_R1	90 degree clockwise rotation
// U8G2_R2	180 degree clockwise rotation
// U8G2_R3	270 degree clockwise rotation
// U8G2_MIRROR	No rotation, landscape, display content is mirrored

// /* reset=*/ U8X8_PIN_NONE means that there is no separate pin to reset the screen

/**
 * Defining functions that interact with the hardware
 */

void turn_on_communication_with_computer() {
    // Start communication with computer sending 115,200 bits of information each second
    SerialUSB.begin(115200);

    // takes a a few miliseconds for serial USB to start up
    delay(100);

    // Now that you've done this, if you ever want to send some data to the computer
    // you can write SerialUSB.println(*You can insert "a string" or variable name here*)
    // println means "print a line of text" on the computer screen
    SerialUSB.println("communicating with computer!");
}

void set_up_LEDs() {
    SerialUSB.println("Setting up LEDs");

    // initialize red_LED as an output.
    pinMode(red_LED.pin, OUTPUT);

    // initialize green_LED as an output.
    pinMode(green_LED.pin, OUTPUT);
}

void set_up_screen() {
    SerialUSB.println("Setting up screen");

    // start communicating with the screen
    screen.begin();

    // set the communication to send 1 Million bits per second
    // 1 Mbit/s
    // TODO: Explain I2C data range
    screen.setBusClock(1000000);

    // erase anything on the screen
    // and clear the buffer copy
    screen.clear();
}

void set_up_buttons() {
    SerialUSB.println("Setting up screen");

    // initialize up_button as input with a pull-up resistor.
    pinMode(up_button.pin, INPUT_PULLUP);

    // initialize down_button as input with a pull-up resistor.
    pinMode(down_button.pin, INPUT_PULLUP);
}

void set_up_buzzer() {
    SerialUSB.println("Setting up Buzzer");

    // initialize buzzer as an output.
    pinMode(buzzer.pin, OUTPUT);

    // Play a short startup sound
    buzzer.play_sound(1000 /*Hz*/, 100 /*ms*/);
}

/**
 * This function should update the current state of each button (pressed/not_pressed)
 * by reading the voltage of the pin each button is connected to
 * and update the button structs
 */
void read_buttons() {
    SerialUSB.println("Checking Up button");
    uint up_button_voltage = digitalRead(up_button.pin);

    // Explain what Voltage low range and voltage high range are
    if (up_button_voltage == LOW) {
        up_button.pressed = true;
    } else {
        up_button.pressed = false;
    }

    SerialUSB.println("Checking Down button");
    uint down_button_voltage = digitalRead(down_button.pin);
    if (down_button_voltage == LOW) {
        down_button.pressed = true;
    } else {
        down_button.pressed = false;
    }
}

/**
 * This function works for all LEDs.
 * It will start blinking the LED and start a timer to turn it off.
 * Using a timer means that the rest of the code will not freeze while the LED is blinking.
 *
 * Inputs: frequency in Hertz, duration in miliseconds
 *
 * You would use it like this:
 * red_LED.blink(1,1000); // blink 1 time in 1 second
 */
void LED::blink(uint frequency, uint duration) { tone(pin, frequency, duration); }

/**
 * This function works for all Buzzers.
 * It will start playing a sound on the buzzrer and start a timer to turn it off.
 * Using a timer means that the rest of the code will not freeze while the sound is playing.
 *
 * Inputs: frequency in Hertz, duration in miliseconds
 *
 * You would use it like this:
 * buzzer.play_sound(440, 50); // plays an A note for 50 miliseconds
 * buzzer.play_sound(10000, 1000); // plays a really high-pitched sound for 1 second
 */
void Buzzer::play_sound(uint frequency, uint duration) { tone(pin, frequency, duration); }