/**
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024-2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/*! \file
 *
 *  \author
 *
 *  \brief Platform header file. Defining platform independent functionality.
 *
 */

#ifndef RFAL_PLATFORM_H
#define RFAL_PLATFORM_H

#ifdef __cplusplus
extern "C" {
#endif

/*
******************************************************************************
* INCLUDES
******************************************************************************
*/
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "main.h"
#include "spi.h"
#include "spi_.h"
#include "timer.h"
#include "task_console.h"


/*
******************************************************************************
* GLOBAL DEFINES
******************************************************************************
*/
#define ST25R_SS_PIN             P9_3              /*!< GPIO pin used for ST25R SPI SS                */


#define ST25R_INT_PIN            P9_5            /*!< GPIO pin used for ST25R External Interrupt    */


#define ST25R_RESET_PIN          NC            /*!< GPIO pin used for ST25R Reset                 */



#ifdef LED_FIELD_Pin
#define PLATFORM_LED_FIELD_PIN   NC           /*!< LED 1 GPIO pin used as field LED              */
#endif

/*
#ifdef LED_FIELD_GPIO_Port
#define PLATFORM_LED_FIELD_PORT  LED_FIELD_GPIO_Port    
#endif
*/


#define PLATFORM_LED_A_PIN           NC           /*!< LED 2 GPIO pin used for LED A     */

#define PLATFORM_LED_B_PIN           NC            /*!< LED 3 GPIO pin used for LED B     */

#define PLATFORM_LED_F_PIN           NC             /*!< LED 5 GPIO pin used for LED F     */

#define PLATFORM_LED_V_PIN           NC            /*!< LED 4 GPIO pin used for LED V     */

#define PLATFORM_LED_AP2P_PIN        NC             /*!< LED 5 GPIO pin used for LED AP2P  */


#define PLATFORM_USER_BUTTON_PIN     NC               /*!< GPIO pin user button              */



/*
******************************************************************************
* GLOBAL MACROS
******************************************************************************
*/
#define platformProtectST25RComm()                    do{ globalCommProtectCnt++; __DSB(); cyhal_gpio_enable_event(ST25R_INT_PIN, CYHAL_GPIO_IRQ_RISE, 1, false); __DSB(); __ISB(); }while(0) /*!< Protect unique access to ST25R communication channel - IRQ disable */
#define platformUnprotectST25RComm()                  do{ if (--globalCommProtectCnt==0) { cyhal_gpio_enable_event(ST25R_INT_PIN, CYHAL_GPIO_IRQ_RISE, 1, true); } }while(0)                /*!< Unprotect unique access to ST25R communication channel - IRQ enable */

#define platformProtectST25RIrqStatus()               platformProtectST25RComm()                    /*!< Protect unique access to IRQ status var - IRQ disable on single thread environment (MCU) ; Mutex lock on a multi thread environment */
#define platformUnprotectST25RIrqStatus()             platformUnprotectST25RComm()                  /*!< Unprotect the IRQ status var - IRQ enable on a single thread environment (MCU) ; Mutex unlock on a multi thread environment         */

#define platformLedOff(pin )                   platformGpioClear(pin)                  /*!< Turns the given LED Off                     */
#define platformLedOn(pin )                    platformGpioSet(pin)                    /*!< Turns the given LED On                      */
#define platformLedToggle(pin )                platformGpioToggle(pin)                 /*!< Toggles the given LED                       */

#define platformGpioSet(pin )                  cyhal_gpio_write(pin, true)    /*!< Turns the given GPIO High                   */
#define platformGpioClear(pin )                cyhal_gpio_write(pin, false)  /*!< Turns the given GPIO Low                    */
#define platformGpioToggle(pin )               cyhal_gpio_toggle(pin)                 /*!< Toggles the given GPIO                      */
#define platformGpioIsHigh(pin )               (cyhal_gpio_read(pin) == true) /*!< Checks if the given LED is High             */
#define platformGpioIsLow(pin )                (!platformGpioIsHigh(pin))              /*!< Checks if the given LED is Low              */

#define platformTimerCreate( t )                      timerCalculateTimer(t)                        /*!< Create a timer with the given time (ms)     */
#define platformTimerIsExpired( timer )               timerIsExpired(timer)                         /*!< Checks if the given timer is expired        */
#define platformTimerGetRemaining( timer )            timerGetRemaining(timer)                      /*!< Gets the remaining time until expiration    */
#define platformDelay( t )                            cyhal_system_delay_ms(t)                                 /*!< Performs a delay for the given time (ms)    */

#define platformGetSysTick()                          xTaskGetTickCount()                                 /*!< Get System Tick ( 1 tick = 1 ms)            */

#define platformAssert( exp )                         assert_param( exp )                           /*!< Asserts whether the given expression is true*/
#define platformErrorHandle()                         Error_Handler()                               /*!< Global error handle\trap                    */


#define platformSpiSelect()                           platformGpioClear(ST25R_SS_PIN)/*!< SPI SS\CS: Chip|Slave Select                */
#define platformSpiDeselect()                         platformGpioSet(ST25R_SS_PIN)  /*!< SPI SS\CS: Chip|Slave Deselect              */
#define platformSpiTxRx( txBuf, rxBuf, len )          spiTxRx(txBuf, rxBuf, len)                    /*!< SPI transceive                              */


#define platformLog(...)                              task_print(__VA_ARGS__)                         /*!< Log  method                                 */

/*
******************************************************************************
* GLOBAL VARIABLES
******************************************************************************
*/
extern uint8_t globalCommProtectCnt;                      /* Global Protection Counter provided per platform - instantiated in main.c    */

/*
******************************************************************************
* USER SPECIFIC RFAL CONFIGURATION
******************************************************************************
*/

#define RFAL_FEATURE_LISTEN_MODE               false      /*!< Enable/Disable RFAL support for Listen Mode                               */
#define RFAL_FEATURE_WAKEUP_MODE               true       /*!< Enable/Disable RFAL support for the Wake-Up mode                          */
#define RFAL_FEATURE_LOWPOWER_MODE             true       /*!< Enable/Disable RFAL support for the Low Power mode                        */
#define RFAL_FEATURE_NFCA                      true       /*!< Enable/Disable RFAL support for NFC-A (ISO14443A)                         */
#define RFAL_FEATURE_NFCB                      true       /*!< Enable/Disable RFAL support for NFC-B (ISO14443B)                         */
#define RFAL_FEATURE_NFCF                      false      /*!< Enable/Disable RFAL support for NFC-F (FeliCa)                            */
#define RFAL_FEATURE_NFCV                      true       /*!< Enable/Disable RFAL support for NFC-V (ISO15693)                          */
#define RFAL_FEATURE_T1T                       true       /*!< Enable/Disable RFAL support for T1T (Topaz)                               */
#define RFAL_FEATURE_T2T                       true       /*!< Enable/Disable RFAL support for T2T                                       */
#define RFAL_FEATURE_T4T                       true       /*!< Enable/Disable RFAL support for T4T                                       */
#define RFAL_FEATURE_ST25TB                    true       /*!< Enable/Disable RFAL support for ST25TB                                    */
#define RFAL_FEATURE_ST25xV                    true       /*!< Enable/Disable RFAL support for ST25TV/ST25DV                             */
#define RFAL_FEATURE_DYNAMIC_ANALOG_CONFIG     false      /*!< Enable/Disable Analog Configs to be dynamically updated (RAM)             */
#define RFAL_FEATURE_DPO                       false      /*!< Enable/Disable RFAL Dynamic Power Output support                          */
#define RFAL_FEATURE_ISO_DEP                   true       /*!< Enable/Disable RFAL support for ISO-DEP (ISO14443-4)                      */
#define RFAL_FEATURE_ISO_DEP_POLL              true       /*!< Enable/Disable RFAL support for Poller mode (PCD) ISO-DEP (ISO14443-4)    */
#define RFAL_FEATURE_ISO_DEP_LISTEN            false      /*!< Enable/Disable RFAL support for Listen mode (PICC) ISO-DEP (ISO14443-4)   */
#define RFAL_FEATURE_NFC_DEP                   false      /*!< Enable/Disable RFAL support for NFC-DEP (NFCIP1/P2P)                      */

#define RFAL_FEATURE_ISO_DEP_IBLOCK_MAX_LEN    256U       /*!< ISO-DEP I-Block max length. Please use values as defined by rfalIsoDepFSx */
#define RFAL_FEATURE_NFC_DEP_BLOCK_MAX_LEN     254U       /*!< NFC-DEP Block/Payload length. Allowed values: 64, 128, 192, 254           */
#define RFAL_FEATURE_NFC_RF_BUF_LEN            258U       /*!< RF buffer length used by RFAL NFC layer                                   */

#define RFAL_FEATURE_ISO_DEP_APDU_MAX_LEN      512U       /*!< ISO-DEP APDU max length. Please use multiples of I-Block max length       */
#define RFAL_FEATURE_NFC_DEP_PDU_MAX_LEN       512U       /*!< NFC-DEP PDU max length.                                                   */

/*
******************************************************************************
* DEFAULT RFAL CONFIGURATION
******************************************************************************
*/
#include "rfal_defConfig.h"

#ifdef __cplusplus
}
#endif

#endif /* RFAL_PLATFORM_H */
