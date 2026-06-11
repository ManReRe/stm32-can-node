/*
 * test_functions.c
 *
 *  Created on: Jun 11, 2026
 *      Author: Manuel Ángel Reyes
 */

#include "test_functions.h"
#include "cmsis_os.h"

//#define FUNC_BLINK_TEST
//#define FUNC_EXTIMCP_TEST
//#define FUNC_SPI2_LOOPBACK_TEST
//#define FUNC_SPI2_REG_TEST
//#define FUNC_MCP2515_RESET_TEST

/* External references from main.c */
extern SPI_HandleTypeDef hspi2;

#ifdef FUNC_BLINK_TEST
static void Test_Blink(void);
#endif

#ifdef FUNC_EXTIMCP_TEST
static void Test_ExtiMCP(void);
#endif

#ifdef FUNC_SPI2_LOOPBACK_TEST
static void Test_SPI2_Loopback(void);
#endif

#ifdef FUNC_SPI2_REG_TEST
static void Test_SPI2_Registers(void);
#endif

#ifdef FUNC_MCP2515_RESET_TEST
static void Test_MCP2515_Reset(void);
#endif

/**
 * @brief Initialize and run all configured tests once at startup
 * @param None
 * @retval None
 */
void Tests_Init(void)
{
#ifdef FUNC_BLINK_TEST
	Test_Blink();
#endif

#ifdef FUNC_EXTIMCP_TEST
	Test_ExtiMCP();
#endif

#ifdef FUNC_SPI2_LOOPBACK_TEST
	Test_SPI2_Loopback();
#endif

#ifdef FUNC_SPI2_REG_TEST
	Test_SPI2_Registers();
#endif

#ifdef FUNC_MCP2515_RESET_TEST
	Test_MCP2515_Reset();
#endif
}

#ifdef FUNC_BLINK_TEST
/**
 * @brief Blink test: toggles LED to verify GPIO operation
 */
static void Test_Blink(void)
{
	HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1);
}
#endif

#ifdef FUNC_EXTIMCP_TEST
/**
 * @brief External interrupt test: simulates INT on PC5
 */
static void Test_ExtiMCP(void)
{
	EXTI->SWIER |= EXTI_SWIER_SWIER5;   // Simulates INT on PC5
}
#endif

#ifdef FUNC_SPI2_LOOPBACK_TEST
/**
 * @brief SPI2 loopback test: transmits 0xA5 and verifies reception
 */
static void Test_SPI2_Loopback(void)
{
	uint8_t tx = 0xA5;
	uint8_t rx = 0;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); // CS LOW
	HAL_SPI_TransmitReceive(&hspi2, &tx, &rx, 1, 100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);   // CS HIGH

	if (rx == tx)
	{
		HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1); // LED OK
	}
}
#endif

#ifdef FUNC_SPI2_REG_TEST
/**
 * @brief SPI2 register test: writes and reads from register 0x2A
 */
static void Test_SPI2_Registers(void)
{
	uint8_t tx[3];
	uint8_t rx[3];

	// WRITE simulation
	tx[0] = 0x02;   // WRITE instruction
	tx[1] = 0x2A;   // Address
	tx[2] = 0x55;   // Data

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi2, tx, rx, 3, 100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

	osDelay(10);

	// READ simulation
	tx[0] = 0x03;   // READ instruction
	tx[1] = 0x2A;   // Address
	tx[2] = 0x00;   // Dummy

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi2, tx, rx, 3, 100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

	// Real validation
	if (rx[0] == tx[0] && rx[1] == tx[1] && rx[2] == tx[2])
	{
		HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1); // SPI OK
	}
}
#endif

#ifdef FUNC_MCP2515_RESET_TEST
/**
 * @brief MCP2515 reset test: sends reset command and verifies response
 */
static void Test_MCP2515_Reset(void)
{
	uint8_t cmd = 0xC0; // RESET
	uint8_t rx  = 0;

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi2, &cmd, &rx, 1, 100);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

	if (rx == cmd) {
		HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1); // SPI OK
	}

	osDelay(500);
}
#endif
