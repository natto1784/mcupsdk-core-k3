# Enet Lwip TCP Server Example on CPSW{#EXAMPLES_ENET_LWIP_CPSW_TCPSERVER}

[TOC]

# Introduction

\note LwIP features are made available as is from public lwIP project. SDK configuration may only enable and exercise a subset of these features.

This example shows about how to implement a simple TCP Server on LwIP networking stack using netconn API coupled with ethernet driver (ENET).
\cond SOC_AM273X || SOC_AM263X || SOC_AM263PX
This example also demonstrates Rx Scatter-Gather, the buffer size is kept to 384 bytes. Send packets greater than 384 bytes to exercise scatter-gather on Rx.
\endcond

On @VAR_SOC_NAME, we can do ethernet based communication using CPSW as HW mechanism
  - CPSW is a standard ethernet switch + port HW
  - It uses ethernet driver underneath with LwIP TCP/IP networking stack
  - CPSW can be configured in Switch mode

The example does below
- Initializes the ethernet driver for the underlying HW
- Initializes the LwIP stack for TCP/UDP IP and Starts TCP Server task.
- TCP Server task waits for connection from client on port 8888. When connection is established, it waits for any message from client.
- TCP Server task sends back "Greetings from Texas Instruments!" message back to client and closes the connection.


# Supported Combinations

\cond SOC_AM62PX

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | wkup-r5fss0-0_freertos
 Toolchain      | ti-arm-clang
 Board          | @VAR_BOARD_NAME_LOWER
 Example folder | examples/networking/lwip/enet_cpsw_tcpserver

Note: To run the example on any core other than r5fss0-0, user needs to change the DMA channel resource ownership accordingly using the resource partioning tool in \ref RESOURCE_ALLOCATION_GUIDE and build the new SBL.
\endcond

\cond SOC_AM62DX

 Parameter      | Value
 ---------------|-----------
 CPU + OS       | mcu-r5fss0-0_freertos
 Toolchain      | ti-arm-clang
 Board          | @VAR_BOARD_NAME_LOWER
 Example folder | examples/networking/lwip/enet_cpsw_tcpserver

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

# TCP Client using ncat tool

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
      Since the example runs on wkup R5, we cannot use CCS loading. Use example flashing methods shown in \ref EVM_SETUP_PAGE.

\cond SOC_AM62PX

### AM62PX-SK

#### For CPSW based example

- Connect a ethernet cable to the EVM from host PC as shown below

  \imageStyle{am62px_sk_cpsw_example.jpg,width:40%}
  \image html am62px_sk_cpsw_example.jpg Ethernet cable for CPSW based ethernet

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


## Sample output for CPSW example

\code

==========================
  CPSW LWIP TCP ECHO SERVER
==========================
EnetPhy_bindDriver:1718
PHY 3 is alive
PHY 12 is alive
Starting lwIP, local interface IP is dhcp-enabled
Host MAC address: 34:08:e1:77:fb:0c
[LWIPIF_LWIP] Enet has been started successfully
[LWIPIF_LWIP] NETIF INIT SUCCESS
Enet IF UP Event. Local interface IP:0.0.0.0
Waiting for network UP ...
Waiting for network UP ...
Cpsw_handleLinkUp:1320
MAC Port 1: link up
Network Link UP Event
Waiting for network UP ...
Enet IF UP Event. Local interface IP:192.168.1.10
Network is UP ...
     12. 81s : CPU load =   1.77 %
accepted new connection 700C2DA0
     17. 82s : CPU load =   1.60 %
     22. 83s : CPU load =   1.65 %
     27. 84s : CPU load =   1.64 %
accepted new connection 700C2DA0
     32. 85s : CPU load =   1.63 %
     37. 86s : CPU load =   1.59 %
     42. 87s : CPU load =   1.61 %
     47. 88s : CPU load =   1.68 %
     52. 89s : CPU load =   1.63 %

\endcode

## Steps to execute

1. Run example on EVM

2. Try to reach the EVM using ping as shown below, using a command shell on the host PC
    \code
    $ping 192.168.1.10
    \endcode
    "192.168.1.10" should be replaced with IP of EVM.

3. Start TCP client using 'ncat' cmds as shown below. Below steps have been tried with a Linux Ubuntu 18.04 host PC running bash shell

   Install 'ncat' if not installed by doing below
    \code
    $sudo apt install ncat
    \endcode

   Invoke 'ncat' to connect to EVM IP
    \code
    $ncat 192.168.1.10 8888
    $
    \endcode
   "192.168.1.10" should be replaced with EVM IP.

5. Send any msg in ncat terminal and wait for reply from EVM.

6. Close the connection using Ctrl + C.

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

\ref NETWORKING
