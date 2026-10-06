/**
 * I2C1 Generated Driver API Header File
 *
 * @file i2c1.h
 *
 * @defgroup i2c_client I2C1_CLIENT
 *
 * @brief This header file contains API prototypes and other data types for the I2C1 driver.
 *
 * @version I2C1 Driver Version 2.1.4
 *
 * @version I2C1 Package Version 6.2.0
 */

/*
\xa9 [2026] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip
    software and any derivatives exclusively with Microchip products.
    You are responsible for complying with 3rd party license terms
    applicable to your use of 3rd party software (including open source
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.?
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR
    THIS SOFTWARE.
*/

#ifndef I2C1_H
#define I2C1_H
/**
 * @misradeviation {@advisory,2.5} False positive - A few macros in this file
 * are used as arguments but are not getting recognized by the tool.
 * This rule ID has been disabled at the project level due to numerous
 * instances across various files.
 * Consequently, in the application project, this rule ID must be disabled
 * in the MPLAB-X IDE by navigating to Tools -> Options -> Embedded -> MISRA Check.
*/

#include <stdbool.h>
#include <stdint.h>
#include "i2c_client_types.h"

#define i2c1_interface I2C1

/**
 * @ingroup i2c_client
 * @brief Initializes the I2C1 module based on the configurable options in the MPLAB® Code Configurator (MCC) Melody UI.
 * @param None.
 * @return None. \n
 */
void I2C1_Initialize(void);

/**
 * @ingroup i2c_client
 * @brief Sets the registers to Power-on Reset (POR) values.
 * @param None.
 * @return None. \n
 */
void I2C1_Deinitialize(void);

/**
 * @ingroup i2c_client
 * @brief Writes data to a host on the bus.
 *        I2C1 must be intitialized with I2C1_Initialize() before calling this API.
 * @param [in] wrByte - I2C1 client write byte
 * @return None. \n
 */
void I2C1_WriteByte(uint8_t wrByte);

/**
 * @ingroup i2c_client
 * @brief Reads the data from a host on the bus.
 *        I2C1 must be intitialized with I2C1_Initialize() before calling this API.
 * @param None.
 * @return uint8_t -I2C1 client read byte \n
 */
uint8_t I2C1_ReadByte(void);

/**
 * @ingroup i2c_client
 * @brief Reads the requested address from a host on the bus.
 *        I2C1 must be intitialized with I2C1_Initialize() before calling this API.
 * @param None.
 * @return uint16_t -I2C1 client address \n
 */
uint16_t I2C1_ReadAddr(void);

/**
 * @ingroup i2c_client
 * @brief Returns the type of error occurred during the I2C Transmit and Receive.
 *        I2C1 must be intitialized with I2C1_Initialize() before calling this API.
 * @param None.
 * @return @ref I2C_CLIENT_ERROR_BUS_COLLISION - I2C Bus Collision Error \n
 * @return @ref I2C_CLIENT_ERROR_WRITE_COLLISION - I2C Write Collision Error \n
 * @return @ref I2C_CLIENT_ERROR_RECEIVE_OVERFLOW - I2C Receive overflow \n
 * @return @ref I2C_CLIENT_ERROR_NONE - No Error
 */
i2c_client_error_t I2C1_ErrorGet(void);

/**
 * @ingroup i2c_client
 * @brief Returns the I2C Transfer direction.
 *        I2C1 must be intitialized with I2C1_Initialize() before calling this API.
 * @param None.
 * @return @ref I2C_CLIENT_TRANSFER_DIR_WRITE  - I2C Host writes to client \n
 * @return @ref I2C_CLIENT_TRANSFER_DIR_READ   - I2C Host reads from client \n
 */
i2c_client_transfer_dir_t I2C1_TransferDirGet(void);

/**
 * @ingroup i2c_client
 * @brief Returns the I2C Host ACK status.
 *        I2C1 must be intitialized with I2C1_Initialize() before calling this API.
 * @param None.
 * @return @ref I2C_CLIENT_ACK_STATUS_RECEIVED_ACK   - I2C Host sends ACK to client \n
 * @return @ref I2C_CLIENT_ACK_STATUS_RECEIVED_NACK  - I2C Host sends NACK to client \n
 */
i2c_client_ack_status_t I2C1_LastByteAckStatusGet(void);

/**
 * @ingroup i2c_client
 * @brief Sets the pointer to be called "back" when the given I2C transfer events, Bus collision and Write collision occur.
 * @param handler - A pointer to a function
 * @return None.
 */
void I2C1_CallbackRegister(bool (*callback)(i2c_client_transfer_event_t clientEvent));

/**
 * @ingroup I2C1_client
 * @brief Common ISR function for all I2C1 interrupts.
 *        I2C1 must be intitialized with I2C1_Initialize() before calling this API.
 * @param None.
 * @return None. \n
 */
void I2C1_ISR(void);

/**
 * @ingroup I2C1_client
 * @brief Common ISR function for all I2C1 Error interrupts.
 *        I2C1 must be intitialized with I2C1_Initialize() before calling this API.
 * @param None.
 * @return None. \n
 */
void I2C1_ERROR_ISR(void);


#endif /* I2C1_H */
