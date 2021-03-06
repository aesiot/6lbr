/*
 * Copyright (c) 2013, CETIC.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/**
 * \file
 *         6LBR-Demo Project Configuration
 * \author
 *         6LBR Team <6lbr@cetic.be>
 */

#ifndef CETIC_6LBR_DEMO_PROJECT_CONF_H
#define CETIC_6LBR_DEMO_PROJECT_CONF_H

/* Remove rf_channel forced by platform */
#ifndef USER_RF_CHANNEL
#undef RF_CHANNEL
#endif

/* Undefine hardcoded platform configuration */
#undef QUEUEBUF_CONF_NUM
#undef UIP_CONF_BUFFER_SIZE
#undef UIP_CONF_RECEIVE_WINDOW
#undef WEBSERVER_CONF_CFS_CONNS
#undef WEBSERVER_CONF_CFS_PATHLEN
#undef NETSTACK_CONF_MAC
#undef NETSTACK_CONF_RDC
#undef SKY_CONF_MAX_TX_POWER
#undef RPL_CONF_INIT_LINK_METRIC
#undef UIP_CONF_DS6_NBR_NBU
#undef UIP_CONF_MAX_ROUTES

/* include the project config */
#ifdef USER_PROJECT_CONF_H
#include USER_PROJECT_CONF_H
#endif

/* Platform related configuration */
#if !IGNORE_CETIC_CONTIKI_PLATFORM
#include "platform/contiki/6ln-conf-contiki.h"
#endif

#ifdef CETIC_6LN_PLATFORM_CONF
#include CETIC_6LN_PLATFORM_CONF
#endif

/*---------------------------------------------------------------------------*/
/* Radio                                                                     */
/*---------------------------------------------------------------------------*/

#ifndef IEEE802154_CONF_PANID
#define IEEE802154_CONF_PANID 0xABCD
#endif

#ifndef RF_CHANNEL
#define RF_CHANNEL 26
#endif

#undef CC2538_RF_CONF_CHANNEL
#define CC2538_RF_CONF_CHANNEL RF_CHANNEL
#undef CC2420_CONF_CHANNEL
#define CC2420_CONF_CHANNEL RF_CHANNEL
#undef RF_CORE_CONF_CHANNEL
#define RF_CORE_CONF_CHANNEL RF_CHANNEL

/*---------------------------------------------------------------------------*/
/* Security                                                                  */
/*---------------------------------------------------------------------------*/

#if WITH_TSCH

/* Netstack layers */
#undef NETSTACK_CONF_MAC
#define NETSTACK_CONF_MAC     tschmac_driver
#undef NETSTACK_CONF_RDC
#define NETSTACK_CONF_RDC     nordc_driver
#undef NETSTACK_CONF_FRAMER
#define NETSTACK_CONF_FRAMER  framer_802154

/* IEEE802.15.4 frame version */
#undef FRAME802154_CONF_VERSION
#define FRAME802154_CONF_VERSION FRAME802154_IEEE802154E_2012

/* TSCH and RPL callbacks */
#define RPL_CALLBACK_PARENT_SWITCH tsch_rpl_callback_parent_switch
#define RPL_CALLBACK_NEW_DIO_INTERVAL tsch_rpl_callback_new_dio_interval
#define TSCH_CALLBACK_JOINING_NETWORK tsch_rpl_callback_joining_network
#define TSCH_CALLBACK_LEAVING_NETWORK tsch_rpl_callback_leaving_network

/* TSCH logging. 0: disabled. 1: basic log. 2: with delayed
 * log messages from interrupt */
#undef TSCH_LOG_CONF_LEVEL
#define TSCH_LOG_CONF_LEVEL 2

/* Start TSCH at init */
#undef TSCH_CONF_AUTOSTART
#define TSCH_CONF_AUTOSTART 1

/* 6TiSCH minimal schedule length.
 * Larger values result in less frequent active slots: reduces capacity and saves energy. */
#undef TSCH_SCHEDULE_CONF_DEFAULT_LENGTH
#define TSCH_SCHEDULE_CONF_DEFAULT_LENGTH 3

/* Hardware configuration */

/* Needed for CC2538 platforms only */
/* For TSCH we have to use the more accurate crystal oscillator
 * by default the RC oscillator is activated */
#undef SYS_CTRL_CONF_OSC32K_USE_XTAL
#define SYS_CTRL_CONF_OSC32K_USE_XTAL 1

/* Needed for cc2420 platforms only */
/* Disable DCO calibration (uses timerB) */
#undef DCOSYNCH_CONF_ENABLED
#define DCOSYNCH_CONF_ENABLED 0

/* Enable SFD timestamps (uses timerB) */
#undef CC2420_CONF_SFD_TIMESTAMPS
#define CC2420_CONF_SFD_TIMESTAMPS 1

#if CONTIKI_TARGET_CC2538DK || CONTIKI_TARGET_ZOUL || \
  CONTIKI_TARGET_OPENMOTE_CC2538
#define TSCH_CONF_HW_FRAME_FILTERING    0
#endif /* CONTIKI_TARGET_CC2538DK || CONTIKI_TARGET_ZOUL \
       || CONTIKI_TARGET_OPENMOTE_CC2538 */

#if CONTIKI_TARGET_COOJA
#define COOJA_CONF_SIMULATE_TURNAROUND 0
#endif /* CONTIKI_TARGET_COOJA */

#endif

/*---------------------------------------------------------------------------*/
/* Security                                                                  */
/*---------------------------------------------------------------------------*/

#ifndef WITH_ADAPTIVESEC
#define WITH_ADAPTIVESEC 1
#endif

#if WITH_LLSEC

#if WITH_ADAPTIVESEC

#include "noncoresec-autoconf.h"

#else /* WITH_ADAPTIVESEC */

#undef NETSTACK_CONF_FRAMER
#define NETSTACK_CONF_FRAMER noncoresec_framer
#undef NETSTACK_CONF_LLSEC
#define NETSTACK_CONF_LLSEC noncoresec_driver

#undef LLSEC802154_CONF_ENABLED
#define LLSEC802154_CONF_ENABLED 1
#ifndef NONCORESEC_CONF_SEC_LVL
#define NONCORESEC_CONF_SEC_LVL 6
#endif

#undef AES_128_CONF
#define AES_128_CONF aes_128_driver

#define LLSEC_ANTIREPLAY_ENABLED 1
#define LLSEC_REBOOT_WORKAROUND_ENABLED 1

#endif /* WITH_ADAPTIVESEC */

#else /* WITH_LLSEC */

#undef NETSTACK_CONF_LLSEC
#define NETSTACK_CONF_LLSEC nullsec_driver

#endif /* WITH_LLSEC */

/*---------------------------------------------------------------------------*/
/* 6LoWPAN                                                                   */
/*---------------------------------------------------------------------------*/

#define CETIC_6LBR_6LOWPAN_CONTEXT_0  { 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

/*---------------------------------------------------------------------------*/
/* COAP                                                                      */
/*---------------------------------------------------------------------------*/

#define COAP_SERVER_PORT 5683

/* Uncomment to remove /.well-known/core resource to save code */
//#define WITH_WELL_KNOWN_CORE            0

/* COAP content type definition */
#ifndef COAP_CONF_DATA_FORMAT
#define COAP_CONF_DATA_FORMAT coap_data_format_text
#endif
#define REST_MAX_CHUNK_SIZE     64

/*---------------------------------------------------------------------------*/
/* DNS                                                                      */
/*---------------------------------------------------------------------------*/

#define RESOLV_CONF_SUPPORTS_DNS_SD   0

#define RESOLV_CONF_SUPPORTS_MDNS     0

/*---------------------------------------------------------------------------*/
/* UDP-CLIENT                                                                */
/*---------------------------------------------------------------------------*/

#define CETIC_6LBR_NODE_INFO_PORT 3000

/* Report LQI and RSSI only for parent node */
#define UDP_CLIENT_STORE_RADIO_INFO 1

/*---------------------------------------------------------------------------*/
/* MULTICAST-CLIENT                                                          */
/*---------------------------------------------------------------------------*/

#if MULTICAST_CLIENT

#define MCAST_SINK_UDP_PORT 3001

#undef UIP_MCAST6_CONF_ENGINE
#define UIP_MCAST6_CONF_ENGINE  UIP_MCAST6_ENGINE_ESMRF

#undef UIP_MCAST6_ROUTE_CONF_ROUTES
#define UIP_MCAST6_ROUTE_CONF_ROUTES    1

#endif

/*---------------------------------------------------------------------------*/
/* DTLS                                                                      */
/*---------------------------------------------------------------------------*/

#define DTLS_PEER_MAX 3

#define DTLS_CONF_CONN_TIMEOUT 10

//#define DTLS_CONF_PSK_KEY "secretPSK"
//#define DTLS_CONF_PSK_KEY_LENGTH 9

/*---------------------------------------------------------------------------*/
/* DTLS ECHO                                                                 */
/*---------------------------------------------------------------------------*/

#define DTLS_ECHO_PORT 20220

/*---------------------------------------------------------------------------*/
/* UIP Buffers                                                               */
/*---------------------------------------------------------------------------*/

#ifndef QUEUEBUF_CONF_NUM
#define QUEUEBUF_CONF_NUM          5
#endif

#ifndef UIP_CONF_BUFFER_SIZE
#if CONTIKI_TARGET_SKY || CONTIKI_TARGET_Z1
#define UIP_CONF_BUFFER_SIZE    260
#else
#define UIP_CONF_BUFFER_SIZE    600
#endif
#endif

#ifndef UIP_CONF_RECEIVE_WINDOW
#define UIP_CONF_RECEIVE_WINDOW  60
#endif

/*---------------------------------------------------------------------------*/
/* WEBSERVER                                                                 */
/*---------------------------------------------------------------------------*/

#ifndef WEBSERVER_CONF_CFS_CONNS
#define WEBSERVER_CONF_CFS_CONNS 2
#endif

/* Reserve space for a file name (default is to not use file name) */
#ifndef WEBSERVER_CONF_CFS_PATHLEN
#define WEBSERVER_CONF_CFS_PATHLEN 80
#endif

/*---------------------------------------------------------------------------*/
/* RADIO                                                                     */
/*---------------------------------------------------------------------------*/

#ifndef NETSTACK_CONF_MAC
#define NETSTACK_CONF_MAC     		csma_driver
#endif

#ifndef NETSTACK_CONF_RDC
#define NETSTACK_CONF_RDC     		nullrdc_driver
#endif

#ifndef SKY_CONF_MAX_TX_POWER
#define SKY_CONF_MAX_TX_POWER 	31
#endif

/*---------------------------------------------------------------------------*/
/* RPL & Network                                                             */
/*---------------------------------------------------------------------------*/

#if UIP_CONF_IPV6_RPL

#undef UIP_CONF_ND6_SEND_NS
#define UIP_CONF_ND6_SEND_NS   0

#undef UIP_CONF_ND6_SEND_NA
#define UIP_CONF_ND6_SEND_NA   1

#else

#undef UIP_CONF_ROUTER
#define UIP_CONF_ROUTER        0

#undef UIP_CONF_ND6_SEND_NS
#define UIP_CONF_ND6_SEND_NS   1

#undef UIP_CONF_ND6_SEND_NA
#define UIP_CONF_ND6_SEND_NA   1

#endif

#ifndef RPL_CONF_INIT_LINK_METRIC
#define RPL_CONF_INIT_LINK_METRIC			2
#endif

#define RPL_MAX_DAG_PER_INSTANCE	2
#define RPL_MAX_INSTANCES		1

// Always use infinite upward route
#define RPL_CONF_DEFAULT_ROUTE_INFINITE_LIFETIME    1

//Select RPL MOP
#undef RPL_CONF_MOP
#if RPL_NON_STORING
#define RPL_CONF_MOP RPL_MOP_NON_STORING
#elif MULTICAST_CLIENT
#define RPL_CONF_MOP RPL_MOP_STORING_MULTICAST
#else
#define RPL_CONF_MOP RPL_MOP_STORING_NO_MULTICAST
#endif

#ifndef RPL_CONF_WITH_DAO_ACK
// Enable DAO-Ack
#define RPL_CONF_WITH_DAO_ACK       1
#endif

#ifndef RPL_CONF_RPL_REPAIR_ON_DAO_NACK
#define RPL_CONF_RPL_REPAIR_ON_DAO_NACK    0
#endif

#ifndef RPL_CONF_DIO_REFRESH_DAO_ROUTES
#define RPL_CONF_DIO_REFRESH_DAO_ROUTES     0
#endif

/* Z1 platform has limited RAM */

#if !CUSTOM_CONFIG

#if defined CONTIKI_TARGET_Z1

#define RPL_CONF_MAX_PARENTS_PER_DAG    12
#define NEIGHBOR_CONF_MAX_NEIGHBORS     12

#ifndef UIP_CONF_DS6_NBR_NBU
#define UIP_CONF_DS6_NBR_NBU     12
#endif

#ifndef UIP_CONF_MAX_ROUTES
#define UIP_CONF_MAX_ROUTES   12
#endif

#else /* CONTIKI_TARGET_Z1 */

#define RPL_CONF_MAX_PARENTS_PER_DAG    24
#define NEIGHBOR_CONF_MAX_NEIGHBORS     24

#ifndef UIP_CONF_DS6_NBR_NBU
#define UIP_CONF_DS6_NBR_NBU     24
#endif

#endif /* CONTIKI_TARGET_Z1 */

#if RPL_NON_STORING

#undef UIP_CONF_MAX_ROUTES
#define UIP_CONF_MAX_ROUTES 0

#undef RPL_NS_CONF_LINK_NUM
#define RPL_NS_CONF_LINK_NUM   0

#else /* RPL_NON_STORING */

#undef RPL_NS_CONF_LINK_NUM
#define RPL_NS_CONF_LINK_NUM   0

#ifndef UIP_CONF_MAX_ROUTES
#define UIP_CONF_MAX_ROUTES   24
#endif

#endif /* RPL_NON_STORING */

#endif

/*---------------------------------------------------------------------------*/
/* Init                                                                      */
/*---------------------------------------------------------------------------*/


#define UIP_DS6_CONF_NO_STATIC_ADDRESS 1

/*---------------------------------------------------------------------------*/
/* Miscellaneous                                                             */
/*---------------------------------------------------------------------------*/

/* Disable ENERGEST to save code */

#if defined CONTIKI_TARGET_SKY || (defined CONTIKI_TARGET_Z1 && ! MSP430_20BITS)
#undef ENERGEST_CONF_ON
#define ENERGEST_CONF_ON 0
#endif

#undef UART1_CONF_RX_WITH_DMA
#define UART1_CONF_RX_WITH_DMA 0

#define SICSLOWPAN_CONF_ADDR_CONTEXT_0 \
    uint8_t context_0[8] = CETIC_6LBR_6LOWPAN_CONTEXT_0; \
  memcpy(addr_contexts[0].prefix, context_0, sizeof(addr_contexts[0].prefix));

#endif /* CETIC_6LBR_DEMO_PROJECT_CONF_H */
