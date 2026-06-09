
#include "main.h"
#include "source/app_hw/task_console.h"
#include "spi.h"
#include "task_rfid.h"
#include "spi_.h"


/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "demo.h"
#include "rfal_platform.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/


/* USER CODE BEGIN PV */
uint8_t globalCommProtectCnt = 0;   /*!< Global Protection counter     */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
 

    /* Initialize the device and board peripherals */
  cybsp_init() ;


  __enable_irq();

  task_console_init();

  if (xTaskCreate(
       task_rfid, 
       "RFID", 
       10*configMINIMAL_STACK_SIZE, 
       NULL, 
       5,
       NULL) != pdPASS)
   {
       return false;
   }

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */



  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  spi_init(MODULE_SITE_1);

  
  /* USER CODE BEGIN 2 */

  spiInit(&mSPI);

 vTaskStartScheduler();

  
  /* USER CODE END 3 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */
  /*Configure GPIO pin Output Level */
  cyhal_gpio_write(LED_FIELD_Pin|LED_A_Pin|LED_V_Pin|ST25R_RST_Pin, false);

  /*Configure GPIO pin Output Level */
  cyhal_gpio_write(LED_B_Pin|LED5_Pin|LED_RX_Pin|SPI1_CS_Pin, false);

  /*Configure GPIO pin : B1_Pin */
  cyhal_gpio_init(B1_Pin, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);

  /*Configure GPIO pin : ST25R_IRQ_Pin */
  cyhal_gpio_init(ST25R_IRQ_Pin, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);

  /*Configure GPIO pins : LED_FIELD_Pin LED_A_Pin LED_V_Pin ST25R_RST_Pin */
  cyhal_gpio_init(LED_FIELD_Pin, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
  cyhal_gpio_init(LED_A_Pin, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
  cyhal_gpio_init(LED_V_Pin, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
  cyhal_gpio_init(ST25R_RST_Pin, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);



  /*Configure GPIO pins : LED_B_Pin LED5_Pin LED_RX_Pin SPI1_CS_Pin */
  cyhal_gpio_init(LED_B_Pin, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
  cyhal_gpio_init(LED5_Pin, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
  cyhal_gpio_init(LED_RX_Pin, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, false);
  cyhal_gpio_init(SPI1_CS_Pin, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);


  /* EXTI interrupt init*/
  cyhal_gpio_enable_event(ST25R_IRQ_Pin, CYHAL_GPIO_IRQ_RISE, 1, true); // priority 1

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
      platformLedToggle(PLATFORM_LED_FIELD_PIN );
      platformLedToggle(PLATFORM_LED_A_PIN );
      platformLedToggle(PLATFORM_LED_B_PIN );
      platformLedToggle(PLATFORM_LED_F_PIN );
      platformLedToggle(PLATFORM_LED_V_PIN );
      platformLedToggle(PLATFORM_LED_AP2P_PIN );
      platformDelay(100);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
