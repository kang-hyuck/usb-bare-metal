#include "usbd_driver.h"


void initialize_usb_pins()
{
	/* Enable Clock for GPIOB */
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);

	/* Select AFR(alternate function register) GPIOB14(OTG D-), GPIOB15(OTG D+) */
	MODIFY_REG(GPIO->AFR[1],   // AFR[1]:High,  AFR[0]:LOW
			GPIO_AFRH_AFSEL14 | GPIO_AFRH_AFSEL15,   // Clear this region (Mask Bits)
			_VAL2FLD(GPIO_AFRH_AFSEL14, 0xC) | _VAL2FLD(GPIO_AFRH_AFSEL15, 0xC)
	);

	/* Set GPIOB in alternate function mode */
	MODIFY_REG(GPIOB->MODER,
			GPIO_MODER_MODE14 | GPIO_MODER_MODE15,   // MODER: Mode Register
			_VAL2FLD(GPIO_MODER_MODE14,2) | _VAL2FLD(GPIO_MODER_MODE15,2)  // 2:Use Pin alternate function mode,  1:Output mode, 0:Input mode
	);
}


