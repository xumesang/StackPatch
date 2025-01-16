# StackPatch -ESP32S3


## Usage
## panc_handler_asm.S
We have modified the panc_handler_asm.S in the esp-idf component (which integrates FreeRTOS), specifically the xt_panic exception handling function. You can use it to replace the corresponding file in ESP-idf.

## instrument.py
You can use it to perform instrumentation for functions in esp-idf.