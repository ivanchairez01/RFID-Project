

#include "task_rfid.h"
#include "cyhal_hw_types.h"
#include "main.h"
#include "task_console.h"
#include "rfal_platform.h"

static cyhal_spi_t *rfid_spi_obj = NULL;
static cyhal_gpio_t rfid_cs_pin = NC;

uint8_t fifo[32];


typedef enum {
    RFID_CMD_READ = 0,
    
} rfid_command_t;

static BaseType_t cli_handler_rfid(
    char *pcWriteBuffer,
    size_t xWriteBufferLen,
    const char *pcCommandString);


QueueHandle_t q_rfid;

static const CLI_Command_Definition_t rfid_READ =
    {
        "RFID",                      /* command text */
        "\r\nRFID read\r\n", /* command help text */
        cli_handler_rfid,            /* The function to run. */
        1                             /* The user can enter 1 parameters */
};



/* FreeRTOS CLI Handler for the, grabbed from blink task*/
static BaseType_t cli_handler_rfid(
    char *pcWriteBuffer,
    size_t xWriteBufferLen,
    const char *pcCommandString)
{
  const char *pcParameter;
  rfid_command_t cmd;

  BaseType_t xParameterStringLength, xReturn;

  /* Remove compile time warnings about unused parameters, and check the
  write buffer is not NULL.  NOTE - for simplicity, this example assumes the
  write buffer length is adequate, so does not check for buffer overflows. */
  (void)pcCommandString;
  (void)xWriteBufferLen;
  configASSERT(pcWriteBuffer);

  /* Obtain the parameter string. */
  pcParameter = FreeRTOS_CLIGetParameter(
      pcCommandString,        /* The command string itself. */
      1,                      /* Return the 1st parameter. */
      &xParameterStringLength /* Store the parameter string length. */
  );
  /* Sanity check something was returned. */
  configASSERT(pcParameter);

  /* Copy ONLY the parameter to pcWriteBuffer */
  memset(pcWriteBuffer, 0x00, xWriteBufferLen);
  strncat(pcWriteBuffer, pcParameter, xParameterStringLength);

  if (strcmp(pcWriteBuffer, "read") == 0)
  {
    cmd = RFID_CMD_READ;
  }
  else
  {
    /* Return a string indicating an invalid parameter. */
    memset(pcWriteBuffer, 0x00, xWriteBufferLen);
    sprintf(pcWriteBuffer, "Error... invalid parameter\n\r");
    xReturn = pdFALSE;
    return xReturn;
  }

  //  Send the command to the IMU task
  xQueueSendToBack(q_rfid, &cmd, portMAX_DELAY);

  /* Nothing to return, so zero out the pcWriteBuffer. */
  memset(pcWriteBuffer, 0, xWriteBufferLen);

  /* Indicate that the command has completed */
  xReturn = pdFALSE;

  return xReturn;
}



void rfid_irq_isr(void *callback_arg, cyhal_gpio_event_t event)
{
    uint8_t irq_status = 0;

    // Read IRQ status register; reading clears bits automatically
    rfid_read_reg(0x3C, &irq_status);

    if(irq_status & 0x08){
     ;
    }

    
}


bool task_rfid_resources_init(cyhal_spi_t *spi_obj, cyhal_gpio_t cs_pin){
    cy_rslt_t rslt;
    rfid_spi_obj = spi_obj;
    rfid_cs_pin = cs_pin;

    q_rfid = xQueueCreate(5, sizeof(rfid_command_t));

    rslt = cyhal_gpio_init(rfid_cs_pin, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);
    rslt = cyhal_gpio_init(MOD_1_PIN_IO_0, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, true);
    CY_ASSERT(rslt == CY_RSLT_SUCCESS);
    


     // Configure IRQ pin as input with rising-edge trigger
    cyhal_gpio_init(P9_5, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, 1);

    static cyhal_gpio_callback_data_t int_cb_data = {
        .callback     = rfid_irq_isr,			//Using Io expander task interrupt handler now
        .callback_arg = NULL,      // No argument needed
        .next         = NULL,      // Must be NULL (HAL will populate)
        .pin          = NC         // Must be NC (HAL will populate)
    };

    cyhal_gpio_register_callback(P9_5, &int_cb_data);
    
    cyhal_gpio_enable_event(P9_5, CYHAL_GPIO_IRQ_RISE, 0, true); // priority 3
   


    FreeRTOS_CLIRegisterCommand(&rfid_READ);



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

   return true;

}


void task_rfid(void *arg){

    uint8_t fifo[32] = {0};

    //rfid_init(rfid_spi_obj, rfid_cs_pin);
    //rfid_power_on();
    rfid_command_t cmd;
    q_rfid = xQueueCreate(5, sizeof(rfid_command_t));

    FreeRTOS_CLIRegisterCommand(&rfid_READ);



    

    //rfid_read(fifo);

    

    while(1){
        xQueueReceive(q_rfid, &cmd, portMAX_DELAY);
    
        if(cmd == RFID_CMD_READ){
            while(1){
                /*rfid_read(fifo);
                if(fifo[0] & 0x08){
                    break;
                }
                task_print("got here\n\r");
                task_print("Fifo: %02X , \n\r", fifo[0]);
                */

            platformLog("Welcome to X-NUCLEO-NFC09A1\r\n");

            /* Initialize RFAL */
            if( !demoIni() )
            {
                /*
                * in case the rfal initialization failed signal it by flashing all LED
                * and stopping all operations
                */
                platformLog("Initialization failed..\r\n");
                while(1)
                {
                platformLedToggle(PLATFORM_LED_FIELD_PIN );
                platformLedToggle(PLATFORM_LED_A_PIN );
                platformLedToggle(PLATFORM_LED_B_PIN );
                platformLedToggle(PLATFORM_LED_F_PIN );
                platformLedToggle(PLATFORM_LED_V_PIN );
                platformLedToggle(PLATFORM_LED_AP2P_PIN );
                platformDelay(100);
                }
            }
            else
            {
                platformLog("Initialization succeeded..\r\n");
                for (int i = 0; i < 6; i++)
                {
                platformLedToggle(PLATFORM_LED_FIELD_PIN );
                platformLedToggle(PLATFORM_LED_A_PIN );
                platformLedToggle(PLATFORM_LED_B_PIN );
                platformLedToggle(PLATFORM_LED_F_PIN );
                platformLedToggle(PLATFORM_LED_V_PIN );
                platformLedToggle( PLATFORM_LED_AP2P_PIN );
                platformDelay(200);
                }

                platformLedOff( PLATFORM_LED_FIELD_PIN );
                platformLedOff(PLATFORM_LED_A_PIN );
                platformLedOff(PLATFORM_LED_B_PIN );
                platformLedOff(PLATFORM_LED_F_PIN );
                platformLedOff(PLATFORM_LED_V_PIN );
                platformLedOff(PLATFORM_LED_AP2P_PIN );
            }
            /* USER CODE END 2 */

            /* Infinite loop */
            /* USER CODE BEGIN WHILE */
            while (1)
            {
                /* USER CODE END WHILE */

                /* USER CODE BEGIN 3 */
                /* Run Demo Application */

                platformLog("Polling...\r\n");
                
                demoCycle();
            }
               
            }
        }
        

    

        


    }

        

        




}



