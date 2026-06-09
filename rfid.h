#ifndef RFID_H_
#define RFID_H_

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "spi.h"

//I/0 Pins



//Direct Commands
#define CMD_SET_DEFAULT         0x60
#define CMD_STOP_ACTIVITIES     0x62
#define CMD_CLEAR_FIFO          0x64
#define CMD_CLEAR_RX_GAIN       0x66
#define CMD_ADJUST_REGULATORS   0x68
#define CMD_TRANSMIT_DATA       0x6A
#define CMD_NFC_FIELD_ON        0x6E
#define CMD_CALIBRATE_WU        0x74
#define CMD_SENSE_RF            0x7C
#define CMD_CALIBRATE_RC        0xEE


//FIFO COMMANDS

#define FIFO  0x5F

//Registers
#define STATUS_REG_1        0x40      //Holds odc_ok, agd_ok, wut_on, efd_on, efd_out
#define IRQ_STATUS_REG_1    0x3C
#define IRQ_STATUS_REG_2    0x3D
#define IRQ_STATUS_REG_3    0x3E
#define OPERATION_REG       0x00
#define PROTOCOL_REG        0x12
#define TRANSMISSION_PROTCOL 0x13
#define TX_MOD_1            0x04
#define RX_ANALOG_1         0x06
#define TX_FRAME_1          0x34
#define TX_FRAME_2          0x35
#define FIFO_STATUS         0x36



//Registers
#define OPERATION_REG       0x00
#define GENERAL_CONFIG_REG  0x01
#define REGULATOR_CONFIG    0x02
#define TX_MOD1_REG         0x04
#define TX_MOD2_REG         0x05
/*#define 
#define
#define
#define
#define
#define
#define
#define
#define
#define
#define
#define
#define
#define
#define*/







bool rfid_init(cyhal_spi_t *spi_obj, cyhal_gpio_t cs_pin);

void rfid_read_reg(uint8_t address, uint8_t *output);

void rfid_write_byte(uint8_t address, uint8_t data);

void rfid_direct_cmd(uint8_t cmd);

void rfid_power_on();

void rfid_read(uint8_t *fifo);

void rfid_wu(uint8_t *fifo);



#endif