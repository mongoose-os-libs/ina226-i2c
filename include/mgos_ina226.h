/*
 * Copyright 2018 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include "mgos.h"
#include "mgos_i2c.h"

struct mgos_ina226;

/*
 * Initialize a INA226 on the I2C bus `i2c` at address specified in `i2caddr`
 * parameter (default INA226 is on address 0x40). The sensor will be polled for
 * validity, upon success a new `struct mgos_ina226` is allocated and
 * returned. If the device could not be found, NULL is returned.
 */
struct mgos_ina226 *mgos_ina226_create(struct mgos_i2c *i2c, uint8_t i2caddr);

/*
 * Destroy the data structure associated with a INA226 device. The reference
 * to the pointer of the `struct mgos_ina226` has to be provided, and upon
 * successful destruction, its associated memory will be freed and the pointer
 * set to NULL.
 */
void mgos_ina226_destroy(struct mgos_ina226 **sensor);


/* Measure the bus voltage on the device. Return the measured voltage in `*volts`,
 * in units of Volts.
 * Returns true on success and false on failure, in which case `*volts` is unspecified.
 */
bool mgos_ina226_get_bus_voltage(struct mgos_ina226 *sensor, float *volts);

/* Measure the shunt voltage on the device. Return the measured voltage in `*volts`,
 * in units of Volts.
 * Returns true on success and false on failure, in which case `*volts` is unspecified.
 */
bool mgos_ina226_get_shunt_voltage(struct mgos_ina226 *sensor, float *volts);

/* Set the shunt resistor value in units of Ohms.
 * Typical values is ohms=0.100 (which yields a 3.2A range on the current sensor).
 * Returns true on success and false on failure.
 */
bool mgos_ina226_set_shunt_resistance(struct mgos_ina226 *sensor, float ohms);

/* Get the shunt resistor value in units of Ohms.
 * Returns true on success and false on failure, in which case `*ohms` is unspecified.
 */
bool mgos_ina226_get_shunt_resistance(struct mgos_ina226 *sensor, float *ohms);

/* Get the current measured in units of Amperes.
 * Returns true on success and false on failure, in which case `*ampere` is unspecified.
 * Note: Current is simply (Vshunt / Rshunt) due to Ohms law.
 */
bool mgos_ina226_get_current(struct mgos_ina226 *sensor, float *ampere);
