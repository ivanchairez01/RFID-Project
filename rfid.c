#include "rfid.h"

static cyhal_spi_t *rfid_spi_obj;
static cyhal_gpio_t rfid_cs_pin;

volatile bool rx_ready = false;





bool rfid_init(cyhal_spi_t *spi_obj, cyhal_gpio_t cs_pin){

    if(spi_obj == NULL || cs_pin == NC){
        CY_ASSERT(0);
    }

    rfid_spi_obj = spi_obj;
    rfid_cs_pin = cs_pin;

    

    cyhal_gpio_write(rfid_cs_pin, 1);

    return true;

}




void rfid_read_reg(uint8_t address, uint8_t *output){
    cy_rslt_t rslt;
    uint8_t tx_data[2] = {address | 0x80, 0x00};
    uint8_t rx_data[2] = {0x00, 0x00};

    cyhal_gpio_write(rfid_cs_pin, 0);

    rslt = cyhal_spi_transfer(
        rfid_spi_obj,
        tx_data,
        2,
        rx_data,
        2,
        0xFF
    );
    CY_ASSERT(rslt == CY_RSLT_SUCCESS);

    cyhal_gpio_write(rfid_cs_pin, 1);

    *output = rx_data[1];
}

void rfid_write_byte(uint8_t address, uint8_t data){
    cy_rslt_t rslt;
    uint8_t tx_data[2] = {address, data};
    uint8_t rx_data[2] = {0x00, 0x00};

    cyhal_gpio_write(rfid_cs_pin, 0);
    
    rslt = cyhal_spi_transfer(
        rfid_spi_obj,
        tx_data,
        2,
        rx_data,
        2,
        0xFF
    );

    CY_ASSERT(rslt == CY_RSLT_SUCCESS);

    cyhal_gpio_write(rfid_cs_pin, 1);
}

void rfid_direct_cmd(uint8_t cmd){
    uint8_t tx_data[1] = {cmd};
    uint8_t rx_data[1] = {0x00};
    cy_rslt_t rslt;

    cyhal_gpio_write(rfid_cs_pin, 0);

    rslt = cyhal_spi_transfer(
        rfid_spi_obj,
        tx_data,
        1,
        rx_data,
        1,
        0x00);

    CY_ASSERT(rslt == CY_RSLT_SUCCESS);

    cyhal_gpio_write(rfid_cs_pin, 1);

}

void rfid_power_on(){

    uint8_t rx_data;
    rfid_direct_cmd(CMD_SET_DEFAULT);
    rfid_direct_cmd(CMD_STOP_ACTIVITIES);
    vTaskDelay(pdMS_TO_TICKS(10));
    

    rfid_write_byte(OPERATION_REG, 0x02); //Set en, moving to RD mode

    vTaskDelay(pdMS_TO_TICKS(5));
  
    //Waiting for osc_ok (oscillator is stable when get osc_ok)
    do{
        rfid_read_reg(0x3E, &rx_data);
    } while((rx_data & 0x01) != 1);

    rfid_write_byte(0x01, 0x00);

    rfid_write_byte(OPERATION_REG, 0x3A);
    vTaskDelay(pdMS_TO_TICKS(10));
    rfid_direct_cmd(CMD_ADJUST_REGULATORS);
    vTaskDelay(pdMS_TO_TICKS(10));
    
    rfid_write_byte(0x12, 0x01);

    //rfid_write_byte(0x13, 0x60);
    //rfid_write_byte(0x16, 0x0C);

    
    /*rfid_write_byte(0x1B,0x00);
    rfid_write_byte(0x1C,0x02);

    
    
    rfid_write_byte(0x1E,0x30);
    rfid_write_byte(0x1F,0xFF);
    rfid_write_byte(0x20,0xFF);

    rfid_write_byte(0x39,0x80);
    rfid_write_byte(0x3A,0x00);*/

    //Squelch and AGC
    //rfid_write_byte(0x0A, 0x00);
    

   
}


void rfid_read(uint8_t *fifo){
    //uint8_t irq_status;
    uint8_t output;

    while(1){
        rfid_direct_cmd(CMD_STOP_ACTIVITIES);
        rfid_direct_cmd(CMD_CLEAR_RX_GAIN);
        rfid_write_byte(0x1E,0x30);
        rfid_write_byte(0x1F,0xFF);
        rfid_write_byte(0x20,0xFF);
        rfid_write_byte(0x1B,0x01);
        rfid_write_byte(0x1C,0x01);
        rfid_write_byte(0x16, 0x38);        //Receive with no crc
        rfid_write_byte(0x13, 0x40);
        
        //rfid_write_byte(0x07,0x58);
        rfid_write_byte(TX_FRAME_1, 0x00);
        rfid_write_byte(TX_FRAME_2, 0x07); 


        rfid_direct_cmd(CMD_CLEAR_FIFO);
        rfid_write_byte(FIFO, 0x52);    //ISO14443A REQA command

        vTaskDelay(pdMS_TO_TICKS(5));
        //Transmit data from FIFO
        rfid_direct_cmd(CMD_TRANSMIT_DATA); //Transmit REQA
        

        do
            rfid_read_reg(0x3C, &output);
        while((output & 0x02) == 0);

       

        
        //rfid_direct_cmd(0x72);
        for(int i = 0; i < 30; i++){

            rfid_read_reg(0x11, &output);
             if (output & 0x08) {   // Bit 3 is set
                fifo[0] = output;
                return;
            }
            vTaskDelay(pdMS_TO_TICKS(0.1));

        }

        rfid_read_reg(0x36, &output);



        rfid_write_byte(TX_FRAME_1, 0x00);
        rfid_write_byte(TX_FRAME_2, 0x10); 

        rfid_write_byte(0x16, 0x3C);        //Receive with no crc
        rfid_write_byte(0x13, 0x60);

        rfid_write_byte(FIFO, 0x93);
        rfid_write_byte(FIFO, 0x20);

        rfid_direct_cmd(CMD_TRANSMIT_DATA);


         do
            rfid_read_reg(0x3C, &output);
        while((output & 0x02) == 0);

       

        
        //rfid_direct_cmd(0x72);
        for(int i = 0; i < 30; i++){

            rfid_read_reg(0x11, &output);
             if (output & 0x08) {   // Bit 3 is set
                fifo[0] = output;
                return;
            }
            vTaskDelay(pdMS_TO_TICKS(0.1));

        }

        rfid_read_reg(0x36, &output);



        

        /*rfid_write_byte(0x16, 0x3C);
        rfid_write_byte(0x13, 0x60);

        rfid_write_byte(TX_FRAME_1, 0x00);
        rfid_write_byte(TX_FRAME_2, 0x18); 


        rfid_direct_cmd(CMD_CLEAR_FIFO);
        rfid_write_byte(FIFO, 0x93);    //ISO14443A REQA command
        rfid_write_byte(FIFO, 0x20);

        vTaskDelay(pdMS_TO_TICKS(1));
        //Transmit data from FIFO
        rfid_direct_cmd(CMD_TRANSMIT_DATA); //Transmit REQA
        

        do
            rfid_read_reg(0x3C, &output);
        while((output & 0x02) == 0);

        rfid_direct_cmd(0x72);


        */
        
        

        //rfid_write_byte(0x00, 0x12);
        
        /*
        //Polling for got_rx, giving it 5ms
        int retry = 5;
        bool got_rx = false;
        while(retry--){
            rfid_read_reg(0x3C, &output);

            if(output & 0x08){
                got_rx = true;
                break;
            }
            vTaskDelay(pdMS_TO_TICKS(1));

        }
        if(!got_rx){
            continue;
        }
        */


        //rfid_read_reg(0x36, &output);   //Read fifo length
        if(output == 0){
            return;
        }

        fifo[0] = output;

       

        rfid_read_reg(0x3C, &output);       //Clear irq for next read
        return;
    }   
    
}





