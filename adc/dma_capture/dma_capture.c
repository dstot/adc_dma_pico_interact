/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tusb.h>
#include "pico/stdlib.h"
// For ADC input:
#include "hardware/adc.h"
#include "hardware/dma.h"

// This example uses the DMA to capture many samples from the ADC.
//
// - We are putting the ADC in free-running capture mode at 0.5 Msps
//
// - A DMA channel will be attached to the ADC sample FIFO
//
// - Configure the ADC to right-shift samples to 8 bits of significance, so we
//   can DMA into a byte buffer


// Channel 0 is GPIO26
#define CAPTURE_CHANNEL 0
#define CAPTURE_DEPTH 1000

uint8_t capture_buf[CAPTURE_DEPTH];

#define L 5
unsigned char str[L+1];

unsigned char *readLine() {
  unsigned char u, *p;
  for(p=str, u=getchar(); u!='\r' && p-str<L; u=getchar())  putchar(*p++=u);
  *p = 0;  return str;
}

int main() {
    stdio_init_all();
    stdio_usb_init();

    // FILE *file;
    char* command_list;

    while (!tud_cdc_connected()) { 
        sleep_ms(10000);  
    }
    
    printf("tud_cdc_connected()\n");

    for(;;) {
        int n=atoi(readLine());
        printf("\n%d\n",n*n*n);
    }

    return 0;

    // while the usb is connected to the PC, do stuff
    while (stdio_usb_connected) {

        //if ()
        // Init GPIO for analogue use: hi-Z, no pulls, disable digital input buffer.
        adc_gpio_init(26 + CAPTURE_CHANNEL);

        adc_init();
        adc_select_input(CAPTURE_CHANNEL);
        adc_fifo_setup(
            true,    // Write each completed conversion to the sample FIFO
            true,    // Enable DMA data request (DREQ)
            1,       // DREQ (and IRQ) asserted when at least 1 sample present
            false,   // We won't see the ERR bit because of 8 bit reads; disable.
            true     // Shift each sample to 8 bits when pushing to FIFO
        );

        //file = fopen("output.txt", w+);

        // Divisor of 0 -> full speed. Free-running capture with the divider is
        // equivalent to pressing the ADC_CS_START_ONCE button once per `div + 1`
        // cycles (div not necessarily an integer). Each conversion takes 96
        // cycles, so in general you want a divider of 0 (hold down the button
        // continuously) or > 95 (take samples less frequently than 96 cycle
        // intervals). This is all timed by the 48 MHz ADC clock.

        // This is how to alter sample rate

        // fs = fs;
        uint16_t time_per_samp = 96/48e6;
        

        adc_set_clkdiv(0);

        printf("Arming DMA\n");
        sleep_ms(10000);
        // Set up the DMA to start transferring data as soon as it appears in FIFO
        uint dma_chan = 0;
        dma_channel_config cfg = dma_channel_get_default_config(dma_chan);

        // Reading from constant address, writing to incrementing byte addresses
        channel_config_set_transfer_data_size(&cfg, DMA_SIZE_8);
        channel_config_set_read_increment(&cfg, false);
        channel_config_set_write_increment(&cfg, true);

        // Pace transfers based on availability of ADC samples
        channel_config_set_dreq(&cfg, DREQ_ADC);

        dma_channel_configure(dma_chan, &cfg,
            capture_buf,    // dst
            &adc_hw->fifo,  // src
            CAPTURE_DEPTH,  // transfer count
            true            // start immediately
        );

        printf("Starting capture\n");
        adc_run(true);

        // Once DMA finishes, stop any new conversions from starting, and clean up
        // the FIFO in case the ADC was still mid-conversion.
        dma_channel_wait_for_finish_blocking(dma_chan);
        printf("Capture finished\n");
        adc_run(false);
        adc_fifo_drain();

        // Print samples to stdout so you can display them in pyplot, excel, matlab


        float vref = 3.3; // Vref for the pico
        float bitnum = 256; // 2^8 because 8 bits

        float conversion_factor = vref/bitnum; // to output correct voltage 

        while (1) {
            for (int i = 0; i < CAPTURE_DEPTH; ++i) {
                printf("%3f \n", capture_buf[i]*conversion_factor);
            }
        }

        while(1) {
            sleep_ms(10000);
        }
    }
}


