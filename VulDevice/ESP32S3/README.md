# StackPatch implementation on the ESP32S3 board


## Description
## panc_handler_asm.S
To direct the control from the vulnerable code to hot patches, we modify the panc_handler_asm.S in the esp-idf component (which integrates FreeRTOS), specifically the xt_panic exception handling function. You can use it to replace the corresponding file in ESP-idf.

## instrument.py
You can use it to perform instrumentation for functions in esp-idf.