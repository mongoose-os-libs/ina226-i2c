# INA226 I2C Driver

A Mongoose library for Texas Instruments' popular I2C current sensor.

## Implementation details

The `INA226` is a simple I2C device that measures a voltage drop over a
shunt resistor, as well as the voltage on the bus. It is able to measure
voltage up to 36V (in 1.25mV increments) on the bus and the voltage drop
over the shunt resistor in 2.5uV increments with a range of 81.92mV, and
is very precise.

Using a 0.1Ohm shunt resistor, the maximum current that can be measured
is 0.8A.

## API Description

Create an `INA226` object using `mgos_ina226_create()`, set the shunt
resistor value using `mgos_ina226_set_shunt_resistance()`, then call
`mgos_ina226_get_bus_voltage()` to get the bus voltage, call 
`mgos_ina226_get_shunt_voltage()` to get the voltage drop over
the shunt resistor, and `mgos_ina226_get_current()` to get the current
flowing through the circuit.

## Example application

```
#include "mgos.h"
#include "mgos_config.h"
#include "mgos_ina226.h"

static void ina226_timer_cb(void *user_data) {
  struct mgos_ina226 *sensor = (struct mgos_ina226 *)user_data;
  float bus, shunt, current, res;

  if (!sensor) return;

  mgos_ina226_get_bus_voltage(sensor, &bus);
  mgos_ina226_get_shunt_resistance(sensor, &res);
  mgos_ina226_get_shunt_voltage(sensor, &shunt);
  mgos_ina226_get_current(sensor, &current);
  LOG(LL_INFO, ("Vbus=%.3f V Vshunt=%.0f uV Rshunt=%.3f Ohm Ishunt=%.1f mA",
      bus, shunt*10e6, res, current*10e3));
  }
}

enum mgos_app_init_result mgos_app_init(void) {
  struct mgos_ina226 *sensor;

  sensor = mgos_ina226_create(mgos_i2c_get_global(), mgos_sys_config_get_ina226_i2caddr());
  if (!sensor) {
    LOG(LL_ERROR, ("Could not create INA226 sensor"));
    return false;
  }

  mgos_set_timer(1000, true, ina226_timer_cb, sensor);

  return MGOS_APP_INIT_SUCCESS;
}
```

# Disclaimer

This project is not an official Google project. It is not supported by Google
and Google specifically disclaims all warranties as to its quality,
merchantability, or fitness for a particular purpose.
