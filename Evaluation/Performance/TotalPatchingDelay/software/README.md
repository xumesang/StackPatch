# StackPatch Implementation on STM32F401RE 

This project is used to measure the total patching latency of StackPatch, with measurements conducted separately for the software triggering methods. 

The project *bootloader* and *software_bkpt_test* are used to measure the software-breakpoint triggering methods. For instructions on how to run the firmware in SRAM, please refer to StackPatch\VulDevice\STM32F401RE\srampatch\README.md. 

Whether the program runs on Flash or SRAM as the execution medium, StackPatch supports triggering via the hook method. However, it is important to note that the project *hook* provided in this repository corresponds to the experimental scenario where the program runs on Flash.


----------------


