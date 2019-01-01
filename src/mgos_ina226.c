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

#include "mgos_ina226_internal.h"

// A poor man's detect -- read config register and compare to power on default
static bool mgos_ina226_detect(struct mgos_i2c *i2c, uint8_t i2caddr) {
  int man_id, die_id;

  man_id = mgos_i2c_read_reg_w(i2c, i2caddr, MGOS_INA3221_REG_MANID);
  die_id = mgos_i2c_read_reg_w(i2c, i2caddr, MGOS_INA3221_REG_DIEID);

  if (man_id != 0x5449) {
    return false;
  }
  if ((die_id & 0xfff0) != 0x2260) {
    return false;
  }
  return true;
}

static bool mgos_ina226_reset(struct mgos_ina226 *s) {
  uint16_t val;

  if (!s) {
    return false;
  }

  // Reset
  if (!mgos_i2c_write_reg_w(s->i2c, s->i2caddr, MGOS_INA226_REG_CONFIG, 0x8000)) {
    return false;
  }
  mgos_usleep(2000);

  // Set config register:
  val  = AVG_1 << 9;     // Samples
  val |= CT_1100US << 6; // Bus conversion time
  val |= CT_1100US << 3; // Shunt conversion time
  val |= MGOS_INA226_CONF_MODE_CONT_SHUNT_BUS;

  if (!mgos_i2c_write_reg_w(s->i2c, s->i2caddr, MGOS_INA226_REG_CONFIG, val)) {
    return false;
  }
  mgos_usleep(2000);

  return mgos_ina226_set_shunt_resistance(s, MGOS_INA226_DEFAULT_SHUNT_OHMS);
}

struct mgos_ina226 *mgos_ina226_create(struct mgos_i2c *i2c, uint8_t i2caddr) {
  struct mgos_ina226 *sensor = NULL;

  if (!i2c) {
    return NULL;
  }

  if (!mgos_ina226_detect(i2c, i2caddr)) {
    LOG(LL_ERROR, ("I2C 0x%02x is not an INA226", i2caddr));
    return NULL;
  }

  sensor = calloc(1, sizeof(struct mgos_ina226));
  if (!sensor) {
    return NULL;
  }

  memset(sensor, 0, sizeof(struct mgos_ina226));
  sensor->i2caddr = i2caddr;
  sensor->i2c     = i2c;

  if (!mgos_ina226_reset(sensor)) {
    LOG(LL_INFO, ("Could not reset INA226 at I2C 0x%02x", sensor->i2caddr));
    free(sensor);
    return NULL;
  }
  LOG(LL_INFO, ("INA226 initialized at I2C 0x%02x", sensor->i2caddr));
  return sensor;
}

void mgos_ina226_destroy(struct mgos_ina226 **sensor) {
  if (!*sensor) {
    return;
  }

  free(*sensor);
  *sensor = NULL;
  return;
}

bool mgos_ina226_get_bus_voltage(struct mgos_ina226 *sensor, float *volts) {
  int16_t val;

  if (!sensor || !volts) {
    return false;
  }
  val = mgos_i2c_read_reg_w(sensor->i2c, sensor->i2caddr, MGOS_INA226_REG_BUSVOLTS);
  if (val == -1) {
    return false;
  }
  LOG(LL_DEBUG, ("Vbus = %d", val));
  *volts = val * 1.25 / 10e3;    // 1.25mV per LSB
  return true;
}

bool mgos_ina226_get_shunt_voltage(struct mgos_ina226 *sensor, float *volts) {
  int16_t val;

  if (!sensor || !volts) {
    return false;
  }
  val = mgos_i2c_read_reg_w(sensor->i2c, sensor->i2caddr, MGOS_INA226_REG_SHUNTVOLTS);
  if (val == -1) {
    return false;
  }
  LOG(LL_DEBUG, ("Vshunt = %d", val));
  *volts = val * 2.5 / 10e6;   // 2.5uV per LSB
  return true;
}

bool mgos_ina226_get_current(struct mgos_ina226 *sensor, float *ampere) {
  float shunt_volts;

  if (!sensor || !ampere) {
    return false;
  }
  if (!mgos_ina226_get_shunt_voltage(sensor, &shunt_volts)) {
    return false;
  }
  *ampere = shunt_volts / sensor->shunt_resistance;
  LOG(LL_DEBUG, ("Rshunt=%.2fOhms, Vshunt=%.3fV, Ishunt=%.3fA", sensor->shunt_resistance, shunt_volts, *ampere));
  return false;
}

bool mgos_ina226_set_shunt_resistance(struct mgos_ina226 *sensor, float ohms) {
  if (!sensor) {
    return false;
  }
  sensor->shunt_resistance = ohms;
  return true;
}

bool mgos_ina226_get_shunt_resistance(struct mgos_ina226 *sensor, float *ohms) {
  if (!sensor || !ohms) {
    return false;
  }
  *ohms = sensor->shunt_resistance;
  return true;
}

// Mongoose OS library initialization
bool mgos_ina226_i2c_init(void) {
  return true;
}
