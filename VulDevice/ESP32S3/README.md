# StackPatch implementation on the ESP32S3 board


## Description
## panc_handler_asm.S
To direct the control from the vulnerable code to hot patches, we modify the panc_handler_asm.S in the ESP-IDF component (which integrates FreeRTOS), specifically the xt_panic exception handling function. Users can use it to replace their own exception handlers in the ESP-IDF component.

## instrument.py
Users can use it to perform instrumentation operations for functions in ESP-IDF.