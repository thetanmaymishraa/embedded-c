#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For sleep function

#define LED_PIN 13  // Adjust the GPIO pin based on your hardware

void setup() {
    // Export the GPIO pin
    system("echo 13 > /sys/class/gpio/export");
    system("echo out > /sys/class/gpio/gpio13/direction");
}

void loop() {
    while (1) {
        // Turn the LED on
        system("echo 1 > /sys/class/gpio/gpio13/value");
        sleep(1);  // Wait for 1 second

        // Turn the LED off
        system("echo 0 > /sys/class/gpio/gpio13/value");
        sleep(1);  // Wait for 1 second
    }
}

int main() {
    setup();
    loop();
    return 0;
}
