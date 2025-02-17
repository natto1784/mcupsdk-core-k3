# Enet Lwip Socket Example{#EXAMPLES_ENET_LWIP_CPSW_SOCKET}

[TOC]

# Introduction

\note LwIP features are made available as is from public lwIP project. SDK configuration may only enable and exercise a subset of these features.

This example shows about how to implement simple TCP Client on LwIP networking stack using BSD-Socket API coupled with ethernet driver (ENET)

On @VAR_SOC_NAME, we can do ethernet based communication using CPSW as HW mechanism
  - CPSW is a standard ethernet switch + port HW
  - It uses ethernet driver underneath with LwIP TCP/IP networking stack
  - CPSW can be configured in two modes: Switch or MAC. For more details, \ref ENET_LWIP_CPSW_OPERATING_MODES

The example does below
- Initializes the ethernet driver for the underlying HW
- Initializes the LwIP stack for TCP/UDP IP and Starts TCP Socket Client task.
- TCP Socket Client gets server IP using UART terminal menu from USER and connect to server IP on port 8888
- TCP Socket Client connects to server, sends data and expects the data from server.

\cond SOC_AM263X || SOC_AM263PX
NOTE: DSCP priority mapping is configured in the example but for the host port to recieve different priority pkts on the same dma channel, user needs to enable channel override (enChOverrideFlag) flag in dmacfg. Refer enet_lwip_cpsw example.
\endcond

# Supported Combinations

\cond SOC_AM64X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0_freertos
 Toolchain      | ti-arm-clang
 Board          | @VAR_BOARD_NAME_LOWER
 Example folder | examples/networking/lwip/enet_cpsw_socket

Note: To run the example on any core other than r5fss0-0, user needs to change the DMA channel resource ownership accordingly using the resource partioning tool in \ref RESOURCE_ALLOCATION_GUIDE and build the new SBL.
\endcond

\cond SOC_AM243X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0_freertos
 Toolchain      | ti-arm-clang
 Boards         | @VAR_BOARD_NAME_LOWER, @VAR_LP_BOARD_NAME_LOWER
 Example folder | examples/networking/lwip/enet_cpsw_socket

Note: To run the example on any core other than r5fss0-0, user needs to change the DMA channel resource ownership accordingly using the resource partioning tool in \ref RESOURCE_ALLOCATION_GUIDE and build the new SBL.
\endcond

\cond SOC_AM273X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0_freertos
 Toolchain      | ti-arm-clang
 Boards         | @VAR_BOARD_NAME_LOWER
 Example folder | examples/networking/lwip/enet_cpsw_socket

\endcond

\cond SOC_AWR294X

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0_freertos
 Toolchain      | ti-arm-clang
 Boards         | @VAR_BOARD_NAME_LOWER
 Example folder | examples/networking/lwip/enet_cpsw_socket

\endcond

\cond SOC_AM263X || SOC_AM263PX

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | r5fss0-0_freertos
 Toolchain      | ti-arm-clang
 Boards         | @VAR_BOARD_NAME_LOWER, @VAR_LP_BOARD_NAME_LOWER
 Example folder | examples/networking/lwip/enet_cpsw_socket

\endcond

\cond SOC_AM62DX

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | mcu-r5fss0-0_freertos
 Toolchain      | ti-arm-clang
 Board          | @VAR_BOARD_NAME_LOWER
 Example folder | examples/networking/lwip/enet_cpsw_socket
 
\endcond

# Configuring Syscfg

- Following Syscfg option allows flexibility to configure memory foot print based on required use case like: Number of DMA descriptors and buffering.

- Supported Options with default configuration

<table>
<tr>
    <th>Feature
    <th>Section
    <th>Description
    <th>Remarks/Default Setting
</tr>

<tr>
    <td>Mdio Manual Mode Enable
    <td>TI Networking / Enet (CPSW)
    <td>Flag to enable MDIO manual mode in example. Driver support for Manual mode is enabled, so this parameter configures manual mode in the example.
    <td>Default is true. If your silicon is affected with errata <a href="https://www.ti.com/lit/er/sprz457e/sprz457e.pdf" target="_blank">i2329— MDIO interface corruption</a>, then TI suggests to use MDIO_MANUAL_MODE as software workaround.
</tr>

\cond SOC_AM64X || SOC_AM243X || SOC_AM263X || SOC_AM263PX || SOC_AM62DX
<tr>
    <td>Disable Mac Port1, Disable Mac Port2
    <td>TI Networking / Enet (CPSW)
    <td>Select which port to disable.
    <td>Default is Port1 enabled. If both Port1 and Port 2 are enabled, any port can be used and  if operating in switch mode, it enables traffic switching between the two ports.
</tr>
\endcond

<tr>
    <td>Enable Packet Pool Allocation
    <td>TI Networking / Enet (CPSW)
    <td>Flag to enable packet buffer memory allocation from enet utils library. It should be disabled to avoid utils memory wastage, in case application allots packet via other mechanism.
    <td>Default is true. If enabled size of pkt pool size depends on 'Large Pool Packet Size', 'Large Pool Packet Count', 'Medium Pool Packet Size', 'Medium Pool Packet Count', 'Small Pool Packet Size' and 'Small Pool Packet Count'. EnetMem_allocEthPkt API uses this memory to allocate the DMA Ethernet packet.
</tr>

<tr>
    <td>Only Enable Packet Info Allocation
    <td>TI Networking / Enet (CPSW)
    <td>Flag to allocate only the DMA Packet Info structures, this does not include the buffer memory. This is useful when the buffer memory is internally allocated by the application. (Ex- Lwip pools)
    <td>Default is true. If enabled "PktInfoMem Only Count" determines the number of additional DMA Packet Info structures allocated. EnetMem_allocEthPktInfoMem uses this memory to allocate empty DMA Packet Info structures.
</tr>

<tr>
    <td>Number of Tx Packet
    <td>TI Networking / Enet (CPSW) / DMA channel config
    <td>No of Tx packets required for DMA channel
    <td>Default is 16. For LwIP example, the Tx packet buffer memory is internally allocated in lwippools.h. Only the DMA Pkt Info structures are allocated via sysCfg, so this number should match the "PktInfoMem Only Count" described in the above item. To increase the Tx packet count, user needs to update the number correspondingly at "PktInfoMem Only Count" and lwippools.h and build the libs.
</tr>

<tr>
    <td>Number of Rx Packet
    <td>TI Networking / Enet (CPSW) / DMA channel config
    <td>No of Rx packets required for DMA channel
    <td>Default is 32. It contributes to the size of Pkt Mem Pool, DMA ring buffer and accessories size. Rx packet buffer memory is completely mananged with application sysCfg, this is done by using Rx custom Pbuf in LwIP.
</tr>

<tr>
    <td>Netif instance
    <td>TI Networking / Enet (CPSW) / LWIP Interface config
    <td>No of netifs allocated by the example
    <td>Only one netif should be set to default when more than one netif is allocated.
</tr>
</table>

# TCP Server using ncat tool

Ncat is a general-purpose command-line tool for reading, writing, redirecting, and encrypting data across a network. It aims to be your network Swiss Army knife, handling a wide variety of security testing and administration tasks. Ncat is suitable for interactive use or as a network-connected back end for other tools.
 - Ncat is started as server to which EVM connects.
 - Version used for this example Version 7+ ( https://nmap.org/ncat )

# Steps to Run the Example

## To Configure Static IP
Please refer to \ref NETWORKING_LWIP_STATIC_IP.

## Build the example

- When using CCS projects to build, import the CCS project for the required combination
  and build it using the CCS project menu (see \ref CCS_PROJECTS_PAGE).
- When using makefiles to build, note the required combination and build using
  make command (see \ref MAKEFILE_BUILD_PAGE)

## HW Setup

\note Make sure you have setup the EVM with cable connections as shown here, \ref EVM_SETUP_PAGE.
      In addition do below steps.

\cond SOC_AM64X

### AM64X-EVM

#### For CPSW based example

- Connect a ethernet cable to the EVM from host PC as shown below

  \imageStyle{am64x_evm_lwip_example_00.png,width:30%}
  \image html am64x_evm_lwip_example_00.png Ethernet cable for CPSW based ethernet

\endcond

\cond SOC_AM243X

### AM243X-EVM

#### For CPSW based example

- Connect a ethernet cable to the EVM from host PC as shown below

  \imageStyle{am64x_evm_lwip_example_00.png,width:30%}
  \image html am64x_evm_lwip_example_00.png Ethernet cable for CPSW based ethernet

### AM243X-LP

\note AM243X-LP has two ethernet Ports which can be configured as both CPSW ports.

#### For CPSW based examples

- Connect a ethernet cable to the AM243X-LP from host PC as shown below

  \imageStyle{am243x_lp_lwip_example_00.png,width:30%}
  \image html am243x_lp_lwip_example_00.png Ethernet cable for CPSW based ethernet

\endcond

## Create a network between EVM and host PC

- The EVM will get an IP address using DHCP, so make sure to connect the other end of the cable
to a network which has a DHCP server running.

- To get started one can create a simple local network
  between the EVM and the host PC by using a home broadband/wifi router as shown below.
  Most such routers run a DHCP server

  \imageStyle{lwip_example_01.png,width:30%}
  \image html lwip_example_01.png Local network between PC and EVM

- To check the router connection with host PC, recommend to disconnect all other networking conenctions
  on the PC, sometimes you may need to disable firewall SW, and make sure the router is able
  to assign a IP address to your host PC

- After we run the example on the EVM (next step), the EVM will similarly be assigned a IP address, and then host
  can communicate with the EVM using the assigned IP address.

- To enable static IP, set the static IP in the ipAddr variable in the App_setupNetif() before passing it as arguement
  to initiate the netif, and stop the dhcp from starting in the App_allocateIPAddress() function.

## Run the example

\attention If you need to reload and run again, a CPU power-cycle is MUST

- Launch a CCS debug session and run the example executable, see \ref CCS_LAUNCH_PAGE
- You will see logs in the UART terminal as shown in the next section.
- Note the IP address seen in the log, this is what we will use to communicate with the EVM.

## Steps to execute

1. Start TCP server using 'ncat' cmds as shown below. Below steps have been tried with a Linux Ubuntu 18.04 host PC running bash shell

2. Install 'ncat' if not installed by doing below
    \code
    $sudo apt install ncat
    \endcode

3. Invoke 'ncat' to start TCP echo server  on port 8888 as below
    \code
    $ncat -e /bin/cat -kv -l 8888
    \endcode

4. Then, run the example on EVM.
5. Enter host PC IP address when example prompts on UART terminal.


## Sample output for CPSW example

\code

==========================
  CPSW LWIP SIMPLE SOCKET
==========================
Enabling clocks!
EnetAppUtils_reduceCoreMacAllocation: Reduced Mac Address Allocation for CoreId:1 From 4 To 2
Mdio_open:282
EnetPhy_bindDriver:1718
PHY 0 is alive
Starting lwIP, local interface IP is dhcp-enabled
Host MAC address: f4:84:4c:fd:a6:00
[LWIPIF_LWIP] Enet has been started successfully
[LWIPIF_LWIP] NETIF INIT SUCCESS
Enet IF UP Event. Local interface IP:0.0.0.0
Waiting for network UP ...
Cpsw_handleLinkUp:1369
MAC Port 1: link up
Network Link UP Event
Waiting for network UP ...
Enet IF UP Event. Local interface IP:192.168.1.10
Network is UP ...
 UDP socket Menu:
 Enter server IPv4 address:(example: 192.168.101.100)
192.168.1.20
<<< Iteration 1 >>>>
 Connecting to: 192.168.1.20:8888
Connected to host
Message to host: Greetings from Texas Instruments!
Message from host: Greetings from Texas Instruments!
Message to host: This is a sample message
Closed Socket connection
<<< Iteration 2 >>>>
 Connecting to: 192.168.1.20:8888
Connected to host
Message to host: Greetings from Texas Instruments!
Message from host: Greetings from Texas Instruments!
Message to host: This is a sample message
Closed Socket connection
     43.914s : CPU load =   1.17 %
<<< Iteration 3 >>>>
 Connecting to: 192.168.1.20:8888
Connected to host
Message to host: Greetings from Texas Instruments!
Message from host: Greetings from Texas Instruments!
Message to host: This is a sample message
Closed Socket connection
<<< Iteration 4 >>>>
 Connecting to: 192.168.1.20:8888
Connected to host
Message to host: Greetings from Texas Instruments!
Message from host: Greetings from Texas Instruments!
Message to host: This is a sample message
Closed Socket connection
<<< Iteration 5 >>>>
 Connecting to: 192.168.1.20:8888
Connected to host
Message to host: Greetings from Texas Instruments!
Message from host: Greetings from Texas Instruments!
Message to host: This is a sample message
Closed Socket connection
     48.915s : CPU load =   1.49 %

\endcode


## Troubleshooting issues

\cond SOC_AM62DX
- If you see MAC address as `00:00:00:00:00:00`, likely you are using a very early Si sample which does not
  have MAC address "fused" in, in this case do below steps

   - Open file `source/networking/.meta/enet_cpsw/templates/am62dx/enet_soc_cfg.c.xdt`
   - Uncomment below line
        \code
        #define ENET_MAC_ADDR_HACK (TRUE)
        \endcode
   - Rebuild the libraries and examples (\ref MAKEFILE_BUILD_PAGE)
\endcond

- If you see a valid, non-zero MAC address and continuosly seieing "Waiting for network UP..." prints in UART terminal
   - Make sure you see `Enet IF UP Event.` message, if not check the ethernet cable
   - Check the local network and check if the DHCP server is indeed running as expected
   - When using a home broadband/wifi router, its possible to check the clients connected to the DHCP server via a web
     browser. Check your router user manual for more details.

# See Also
\cond SOC_AM64X || SOC_AM243X || SOC_AM62DX
\ref NETWORKING
\endcond