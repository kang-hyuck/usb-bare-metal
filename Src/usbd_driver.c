#include "usbd_driver.h"


void initialize_gpio_pins()
{
	/* Enable Clock for GPIOB */
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);  // Set GPIOBEN Region

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


void initialize_core()
{
	/* Enable Clock for USB core */
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_OTGHSEN);  // Set OTGHSEN Region

	/* Set USB core to device mode, full-speed PHY */
	MODIFY_REG(USB_OTG_HS->GUSBCFG,
			USB_OTG_GUSBCFG_FDMOD | USB_OTG_GUSBCFG_PHYSEL | USB_OTG_GUSBCFG_TRDT,
			USB_OTG_GUSBCFG_FDMOD | USB_OTG_GUSBCFG_PHYSEL | _VAL2FLD(USB_OTG_GUSBCFG_TRDT, 0x09)
	);

	/* device initialize section (Configure device speed) */
	MODIFY_REG(USB_OTG_HS_DEVICE->DCFG,
			USB_OTG_DCFG_DSPD,
			_VAL2FLD(USB_OTG_DCFG_DSPD, 0x03)
	);

	/* Enable VBUS sensing device */
	SET_BIT(USB_OTG_HS->GCCFG, USB_OTG_GCCFG_VBUSBSEN);

	/* Unmask USB core interrupt bits */
	SET_BIT(USB_OTG_HS->GINTMSK,
		USB_OTG_GINTMSK_USBRST | USB_OTG_GINTMSK_ENUMDNEM | USB_OTG_GINTMSK_SOFM |
		USB_OTG_GINTMSK_USBSUSPM | USB_OTG_GINTMSK_WUIM | USB_OTG_GINTMSK_IEPINT |
		USB_OTG_GINTSTS_OEPINT | USB_OTG_GINTMSK_RXFLVLM
	);

	/* Clear all pending registers */
	WRITE_REG(USB_OTG_HS->GINTSTS, 0xFFFFFFFF);

	/* Unmasks "USB global interrupt" which will activate all unmasked interrupts */
	SET_BIT(USB_OTG_HS->GAHBCFG, USB_OTG_GAHBCFG_GINT);
}

