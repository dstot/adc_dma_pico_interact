# Power Detector & Pico Set-up and Usage

# Overview
This project interacts with the ADC and DMA features of the Raspberry Pi Pico, specifically 
for outputting raw samples and data for modulated noise signal processing

# Cloning Pico SDK and Examples

First, clone the Pico SDK to have all necessary support for future projects. 

$ cd ~/
$ mkdir pico
$ cd pico

$ git clone https://github.com/raspberrypi/pico-sdk.git --branch master
$ cd pico-sdk
$ git submodule update --init
$ cd ..
$ git clone https://github.com/raspberrypi/pico-examples.git --branch master

Now, you should have successfully cloned the SDK and Examples directories in your location of choice


# Toolchain Installation Steps

Packages/Versions necessary (can be installed with 'sudo apt install'):
    - Python 3.8 or above
    - C/C++ compiler
    - CMake
    - GNU Embedded Toolchain for Arm 

# 

    1. Create a directory to house this project. This should be located alongside the pico-sdk directory
    2. Clone this repo in the directory made with the step above. 
    
        $ cd adc_test
        $ mkdir build
        $ cd build
        $ cmake ..
        $ cd adc
        $ cd dma_capture
        $ make

    Now, the .uf2 file should be built and ready to be uploaded to the Pico. Check that the file exists with the 'ls' command
