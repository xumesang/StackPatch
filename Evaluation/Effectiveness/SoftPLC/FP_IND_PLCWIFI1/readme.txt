-------------------------------------------------------------------------------
-                    (C) COPYRIGHT 2017 STMicroelectronics                    -
- File:    readme.txt                                                         -
- Author:  System Lab Automation and Motion Control Team                      -
- Date:    4-Sept-2017                                                        -
- Version: V1.0.0                                                             -

-----------
IND-PLCWiFi1/readme.txt
-----------


This application allows to programa PLC with different configuration in terms of inputs and outputs available,using an App developed for mobile devices working with (Android and iOS), and PC 
working with Windows7.
This application, running on Nucleo-F401 board, create a server socket network called "Ladder Demo Board", where a portable device or PC can be connected, and using the "ST-PLC App", 
program a Ladder circuit configuration.

-----------
Board Settings 
-----------

WiFi board X-NUCLEO-IDW01M1, Digital Input/Output X-NUCLEO-PLC01A1, Digital Output X-NUCLEO-OUT01A1.

-----------
Hardware and Software environment
-----------

This application, running on Nucleo-F401 board, create a server socket network, where a portable device or PC can be connected, and using the "ST-PLC App", program a Ladder circuit configuration.
Based on the hardware configuration used,X-NUCLEO-PLC01A1,X-NUCLEO-OUT01A1, connected toghether withc X-NUCLEO-IDW01M1,the application firmware must be compiled, selecting different configuration,
identified with:

- "PLC01A1" if only the expansion board X-NUCLEO-PLC01A1 is connected to the expansion board X-NUCLEO-IDW01M1
- "PLC01A1_OUT01A1" if the expansion boards X-NUCLEO-PLC01A1,X-NUCLEO-OUT01A1 are connected to the expansion board X-NUCLEO-IDW01M1
- "OUT01A1" if only the expansion board X-NUCLEO-OUT01A1 is connected to the expansion board X-NUCLEO-IDW01M1

-----------
How to use it?
-----------    
In order to make the program work, you must do the following:

 - WARNING: before opening the project with any toolchain be sure your folder
   installation path is not too in-depth since the toolchain may report errors
   after building.
   
 - Open IAR toolchain and compile the project (see the release note for detailed information about the version).
   Alternatively you can use the Keil uVision toolchain (see the release note for detailed information about the version).
   Alternatively you can use the System Workbench for STM32 (see the release note for detailed information about the version).

 - Program the firmware on the STM32 Nucleo board: you can copy (or drag and drop) the binary file to the USB mass storage location created when you plug the STM32Nucleo 
   board to your PC. 

 - Alternatively, you can download the pre-built binaries in "Binary" 
   folder included in the distributed package. 
   
 - IMPORTANT NOTE: To avoid issues with USB connection (mandatory if you have USB 3.0), it is   
   suggested to update the ST-Link/V2 firmware for STM32 Nucleo boards to the latest version.
   Please refer to the readme.txt file in the Applications directory for details.

-----------
Connectivity test
-----------
Connet your mobile device, joining to the "Ladder Demo Board" network, open ST PLC APP on your devices, open connection settings,insert IP address (available in the Settings of your device) and Port number 32000; 
after this perform a test connection.
Once the connection is hestablished, try to select your hardware configuration in the App, design your project and dowload it in your PLC.
for more detail refer to the App User Manual
    
_______________________________________________________________________________
- (C) COPYRIGHT 2017 STMicroelectronics                   ****END OF FILE**** -
