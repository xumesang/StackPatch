# NetworkService
This repository contains two network server routines, *http_server* and *coap_server*, which are used to build firmware for flashing onto the development board. It also includes two network clients, *http_client* and *coap_client*, which are used by the update host to send network data packets to the development board and measure the packet latency. Additionally, there is an script  *instrument.py* for deploying hooks in the network server.

---

# 
The measurement results of packet latency are related to the specific network latency. Therefore, the results you measure may not be completely consistent with those in our paper. 
