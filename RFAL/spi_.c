/******************************************************************************
  * @attention
  *
  * COPYRIGHT 2016 STMicroelectronics, all rights reserved
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
  * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
******************************************************************************/
/*! \file
 *
 *  \author 
 *
 *  \brief SPI communication handling implementation.
 *
 */
 
/* Includes ------------------------------------------------------------------*/

#include "spi.h"
#include "st25r200.h"

#define SPI_TIMEOUT   1000

#define ST25R200_MOSI_IDLE     0x00
#define ST25R200_BUF_LEN       (ST25R200_CMD_LEN + ST25R200_FIFO_DEPTH)  // Adjust size as needed


static cyhal_spi_t *pSpi = NULL;




void spiInit(cyhal_spi_t *hspi)
{
    pSpi = hspi;

    /* enabling SPI block will put SCLK to output, guaranteeing proper state when spiSelect() gets called */
    //__HAL_SPI_ENABLE(hspi);
}

void spiSelect()
{
    cyhal_gpio_write(P9_3, false);
}

void spiDeselect()
{
    cyhal_gpio_write(P9_3, true);
}

cy_rslt_t spiTxRx(const uint8_t *txData, uint8_t *rxData, uint16_t length)
{
    cy_rslt_t result;
    static uint8_t dummyTxBuf[ST25R200_BUF_LEN];  // Static buffer for dummy TX data
    static uint8_t dummyRxBuf[ST25R200_BUF_LEN];  // Static buffer for discarded RX data
    
    if (length > ST25R200_BUF_LEN)
    {
        return CY_RSLT_TYPE_ERROR;  // Buffer overflow protection
    }
    
    spiSelect();

    if ((txData != NULL) && (rxData == NULL))
    {
        // Transmit only - we don't care about received data
        result = cyhal_spi_transfer(&mSPI, txData, length, dummyRxBuf, length, 0xFF);
        spiDeselect();
        return result;
    }
    else if ((txData == NULL) && (rxData != NULL))
    {
        // Receive only - send dummy 0x00 bytes (MOSI IDLE state)
        memset(dummyTxBuf, ST25R200_MOSI_IDLE, length);
        result = cyhal_spi_transfer(&mSPI, dummyTxBuf, length, rxData, length, 0xFF);
        spiDeselect();

        return result;
    }
    else if ((txData != NULL) && (rxData != NULL))
    {
        // Full duplex transfer
        result = cyhal_spi_transfer(&mSPI, txData, length, rxData, length, 0xFF);
        spiDeselect();
        return result;
    }
    
    spiDeselect();

    return CY_RSLT_SUCCESS;  // Both NULL case
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
