/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define ENDSTDIN	255
#define CR		13
#define STARTCHR 64
#define SPACE 32

int main() {
    stdio_init_all();
    //stdio_usb_init();

    bool onoff = 0;
    const uint LED_PIN = 25;
    char input[1024];
    char chr;
    int lp = 0;
    bool begin_message = false;

    memset(input, 0, sizeof(input));

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    char* fs;
    char* fc;
    char* duration;
    char* N;
    char mode;
    int i = 0;

    while (stdio_usb_connected) {
        
        chr = getchar_timeout_us(0);

        // Need to check whether we are starting transmission or not
        
        while(chr != ENDSTDIN)
        {

            // chec startchr

            if (chr == STARTCHR) {
                chr = getchar_timeout_us(0);
                begin_message = true;
                continue;
            } 

            // parse the five params in the config params sent through port
        
            while (chr != STARTCHR && begin_message) {
                input[i++] = chr;
                chr = getchar_timeout_us(0);
            }

            input[i] = 0;
            printf("%s\n", input);
            i = 0;
            chr = getchar_timeout_us(0);
            break;
            // sample rate
            

            // center freq

            // sampling time

            // num of samps

            // mode (H, R, D)

            // input[lp++] = chr;

            // if (chr == CR || lp == (sizeof(input) - 1))
            // {
            //     input[lp] = 0;	//terminate string
                
            //     printf("%s/n", input);
            //     //memset(strg, 0, sizeof(strg));
            //     lp = 0;		//reset string buffer pointer
            //     gpio_put(LED_PIN, onoff);
            //     onoff = !onoff;
            //     break;
            // }

            // chr = getchar_timeout_us(0);
        }

        // gpio_put(LED_PIN, onoff);
        // onoff = !onoff;
        sleep_ms(100);
}

    return 0;
}