#pragma once

/*!
* EEPROM位址 - 清除標記
*/
#define EEPADDR_WIPE_ALL (0)

/*!
* EEPROM位址 - 版本號
*/
#define EEPADDR_VERSION (1)
#define EEPADDR_VERSION_SIZE (4)

/*!
* EEPROM位址 - MAC位址
*/
#define EEPADDR_MAC (5)
#define EEPADDR_MAC_SIZE (6)

/*!
* EEPROM位址 - Server位址
*/
#define EEPADDR_SERVER_ADDRESS (11)
#define EEPADDR_SERVER_ADDRESS_SIZE (4)

/*!
* EEPROM位址 - 連接埠號
*/
#define EEPADDR_SERVER_PORT (15)
#define EEPADDR_SERVER_PORT_SIZE (2)

