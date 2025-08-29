# Evaluation on heart rate monitor (hardware-breakpoint triggers)

This project is used to present the results of Figure 7(a). For proof-of-concept (PoC), we have implanted an out-of-bounds read vulnerability, vul_func (CVE-2018-16601). To run this program, you need to connect the heart rate sensor (MAX30102) to the STM32F401RE development board. 

We have provided the data obtained from using this project in the experiment (for plotting Figure 7a) in the path \Evaluation\Effectiveness\HeartRateSensor\data.txt.
----------------


## Usage

- 1. Connect the STM32F401RE to the update host.
- 2. Open the Keil project.
- 3. Compile the Keil project and flash it into the development board.
- 4. Open the the serial terminal tool *UartAssist* (located in *VulDevice\SerialTerminalTool*) and observe the output information.
