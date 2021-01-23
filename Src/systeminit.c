#include <stdint.h>  // uint8_t
#include "system_stm32f4xx.h"  // SystemInit(), SystemCoreClock
#include "stm32f4xx.h"  // SET_BIT(), CLEAR_BIT()
#include "Helpers/logger.h"

LogLevel system_log_level = LOG_LEVEL_DEBUG;
uint32_t SystemCoreClock = 72000000;  // 72MHz

// HCLK = 72 MHz
// PLL:  M=4, N=72, P=2, Q=3
// AHB prescaler = 1
// APB prescaler1 = 2,  APB prescaler2 = 1
// MCO1 prescaler = 2

void configure_clock()
{
	/* Configure Flash Latency */
	MODIFY_REG(FLASH->ACR,		// FLASH_ACR base register
			FLASH_ACR_LATENCY,	// Masking Value (defined 0xF)
			_VAL2FLD(FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_2WS)// Set Value (Write "2WS" value in LATAENCY Position)
			// (method 2) FLASH_ACR_LATENCY_2WS << FLASH_ACR_LATENCY_Pos
			);

	/* Enable HSE */
	SET_BIT(RCC->CR, RCC_CR_HSEON);  // (Register name, Field name)

	/* Waits until HSE is stable */
	while( !READ_BIT(RCC->CR, RCC_CR_HSERDY) );

	/* Configures PLL: source = HSE, PLLCLK = 72MHz. */
	MODIFY_REG(RCC->PLLCFGR,
		RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLQ | RCC_PLLCFGR_PLLSRC | RCC_PLLCFGR_PLLP,
		_VAL2FLD(RCC_PLLCFGR_PLLM, 4) | _VAL2FLD(RCC_PLLCFGR_PLLN, 72) | _VAL2FLD(RCC_PLLCFGR_PLLQ, 3) | RCC_PLLCFGR_PLLSRC_HSE
	);

	/* Enables PLL */
	SET_BIT(RCC->CR, RCC_CR_PLLON);

	/* Waits until PLL is stable */
	while( !READ_BIT(RCC->CR, RCC_CR_PLLRDY) );

	/* Switch system clock to PLL */
	MODIFY_REG(RCC->CFGR,
		RCC_CFGR_SW,
		_VAL2FLD(RCC_CFGR_SW, RCC_CFGR_SW_PLL)
	);

	/* Configures PPRE1 = 2, (PPRE2 = 1, HPRE = 1 by default). */
	MODIFY_REG(RCC->CFGR,
		RCC_CFGR_PPRE1,
		_VAL2FLD(RCC_CFGR_PPRE1, 4)
	);

	/* Waits until PLL is used */
	while(READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

	/* Disables HSI (to save power. don't need anymore)  */
	CLEAR_BIT(RCC->CR, RCC_CR_HSION);
}



void SystemInit(void)
{
	configure_clock();
}


