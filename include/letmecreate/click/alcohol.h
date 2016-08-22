#ifndef __LETMECREATE_CLICK_ALCOHOL_H__
#define __LETMECREATE_CLICK_ALCOHOL_H__

#include <stdint.h>

/**
 * @brief Get a measure from Alcohol Click.
 *
 * The Alcohol click has a sensor which outputs a voltage depending on the
 * concentration of alcohol. This function returns the voltage read on ADC pin.
 *
 * param[in] mikrobus_index Index of the mikrobus
 * @param[out] measure Pointer to a 16-bit variable to retrieve the output from the click (must not be null)
 *
 * @return 0 if successful, otherwise it returns -1.
 */
int alcohol_click_get_measure(uint8_t mikrobus_index, uint16_t *measure);

#endif
