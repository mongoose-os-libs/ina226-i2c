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
#include "mgos_ina226.h"

#ifdef __cplusplus
extern "C" {
#endif

// INA226 I2C address
#define MGOS_INA226_I2C_ADDR          (0x40)

// Registers
#define MGOS_INA226_REG_CONFIG        (0x00)
#define MGOS_INA226_REG_MANID         (0xfe)
#define MGOS_INA226_REG_DIEID         (0xff)
#define MGOS_INA226_REG_SHUNTVOLTS    (0x01)
#define MGOS_INA226_REG_BUSVOLTS      (0x02)
#define MGOS_INA226_REG_POWER         (0x03)
#define MGOS_INA226_REG_CURRENT       (0x04)
#define MGOS_INA226_REG_CALIB         (0x05)

enum mgos_ina226_conf_avg {
  AVG_1    = 0, // Default
  AVG_4    = 1,
  AVG_16   = 2,
  AVG_64   = 3,
  AVG_128  = 4,
  AVG_256  = 5,
  AVG_512  = 6,
  AVG_1024 = 7
};

enum mgos_ina226_conf_ct {
  CT_140US  = 0,
  CT_204US  = 1,
  CT_332US  = 2,
  CT_588US  = 3,
  CT_1100US = 4, // Default
  CT_2116US = 5,
  CT_4156US = 6,
  CT_8244US = 7,
};


#define MGOS_INA226_CONF_MODE_OFF                 (0)
#define MGOS_INA226_CONF_MODE_SINGLE_SHUNT        (1)
#define MGOS_INA226_CONF_MODE_SINGLE_BUS          (2)
#define MGOS_INA226_CONF_MODE_SINGLE_SHUNT_BUS    (3)
#define MGOS_INA226_CONF_MODE_POWERDOWN           (4)
#define MGOS_INA226_CONF_MODE_CONT_SHUNT          (5)
#define MGOS_INA226_CONF_MODE_CONT_BUS            (6)
#define MGOS_INA226_CONF_MODE_CONT_SHUNT_BUS      (7)

#define MGOS_INA226_DEFAULT_SHUNT_OHMS            (0.1) // Ohms

struct mgos_ina226 {
  struct mgos_i2c *i2c;
  uint8_t          i2caddr;
  float            shunt_resistance;
};

/* Mongoose OS initializer */
bool mgos_ina226_i2c_init(void);

#ifdef __cplusplus
}
#endif
