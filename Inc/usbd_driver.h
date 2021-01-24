#ifndef USBD_DRIVER_H_
#define USBD_DRIVER_H_

// CMSIS don't provides direct address but Base addr, Offset.
// So we have to make macro

#include "stm32f4xx.h"  // Macro definition
#include "stm32f429xx.h"  // USB_OTG_GlobalTypeDef,USB_OTG_DeviceTypeDef

// (Register name) -> (Region) -> (Field)
/* Make Region's Base address */
#define USB_OTG_HS_GLOBAL  ((USB_OTG_GlobalTypeDef *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_GLOBAL_BASE))
#define USB_OTG_HS_DEVICE  ((USB_OTG_DeviceTypeDef *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_DEVICE_BASE))
#define USB_OTG_HS_PCGCCTL ((uint32_t *)(USB_OTG_HS_PERIPH_BASE + USB_OTG_PCGCCTL_BASE))

/* Declaration of Functions */
void initialize_gpio_pins();
void initialize_core();
void connect();
void disconnect();

#endif /* USBD_DRIVER_H_ */
