if (TARGET tinyusb_device)
        add_executable(rx_data_collect
                rx_data_collect.c
                )

        target_link_libraries(rx_data_collect pico_stdlib hardware_adc)
        
        # enable usb output, disable uart output
        pico_enable_stdio_usb(rx_data_collect 1)
        pico_enable_stdio_uart(rx_data_collect 0)

        # create map/bin/hex file etc.
        pico_add_extra_outputs(rx_data_collect)

        # add url via pico_set_program_url
        # example_auto_set_url(rx_data_collect)
elseif(PICO_ON_DEVICE)
    message(WARNING "not building rx_data_collect because TinyUSB submodule is not initialized in the SDK")
endif()        
