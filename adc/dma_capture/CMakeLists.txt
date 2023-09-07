if (TARGET tinyusb_device)
	add_executable(adc_dma_capture
			dma_capture.c
			)

	target_link_libraries(adc_dma_capture
			pico_stdlib
			hardware_adc
			hardware_dma
			)
	
	# enable usb output, disable uart output
    pico_enable_stdio_usb(adc_dma_capture 1)
    pico_enable_stdio_uart(adc_dma_capture 0)

	# create map/bin/hex file etc.
	pico_add_extra_outputs(adc_dma_capture)

	# add url via pico_set_program_url
	# example_auto_set_url(adc_dma_capture)

elseif(PICO_ON_DEVICE)
    message(WARNING "not building hello_usb because TinyUSB submodule is not initialized in the SDK")
endif() 


