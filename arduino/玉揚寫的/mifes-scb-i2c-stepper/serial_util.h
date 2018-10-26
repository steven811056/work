#pragma once

#define SERIAL_MSG_ENABLE true
#define SERIAL_MSG_DEBUG_ENABLE false

#define SERIAL_MSG Serial

#if SERIAL_MSG_ENABLE
#define SERIAL_MSG_START() SERIAL_MSG.print(';');
#define SERIAL_MSG_STR(msg) SERIAL_MSG.print(msg);
#define SERIAL_MSG_EQU(tag, val) SERIAL_MSG.print(tag);SERIAL_MSG.print("=");SERIAL_MSG.print(val);SERIAL_MSG.print(" ");
#define SERIAL_MSG_VAL(val) SERIAL_MSG.print(#val);SERIAL_MSG.print("=");SERIAL_MSG.print(val);SERIAL_MSG.print(" ");
#define SERIAL_MSG_LN(msg) SERIAL_MSG.println(msg);

#define SERIAL_MSG_VAL_LN(val) SERIAL_MSG_START() SERIAL_MSG_VAL(val) SERIAL_MSG_LN()
#define SERIAL_MSG_EQU_LN(tag, val) SERIAL_MSG_START() SERIAL_MSG_EQU(tag, val) SERIAL_MSG_LN()
#else
#define SERIAL_MSG_START()
#define SERIAL_MSG_STR(msg)
#define SERIAL_MSG_EQU(tag, val)
#define SERIAL_MSG_VAL(val)
#define SERIAL_MSG_LN(msg)

#define SERIAL_MSG_VAL_LN(val)
#define SERIAL_MSG_EQU_LN(tag, val)
#endif

#if (SERIAL_MSG_DEBUG_ENABLE)
#define SERIAL_MSGD_START() SERIAL_MSG.print(';');
#define SERIAL_MSGD_STR(msg) SERIAL_MSG.print(msg);
#define SERIAL_MSGD_EQU(tag, val) SERIAL_MSG.print(tag);SERIAL_MSG.print("=");SERIAL_MSG.print(val);SERIAL_MSG.print(" ");
#define SERIAL_MSGD_VAL(val) SERIAL_MSG.print(#val);SERIAL_MSG.print("=");SERIAL_MSG.print(val);SERIAL_MSG.print(" ");
#define SERIAL_MSGD_LN(msg) SERIAL_MSG.println(msg);

#define SERIAL_MSGD_VAL_LN(val) SERIAL_MSGD_START() SERIAL_MSGD_VAL(val) SERIAL_MSGD_LN()
#define SERIAL_MSGD_EQU_LN(tag, val) SERIAL_MSG_START() SERIAL_MSG_EQU(tag, val) SERIAL_MSG_LN()
#else
#define SERIAL_MSGD_START()
#define SERIAL_MSGD_STR(msg)
#define SERIAL_MSGD_EQU(tag, val)
#define SERIAL_MSGD_VAL(val)
#define SERIAL_MSGD_LN(msg)

#define SERIAL_MSGD_VAL_LN(val)
#define SERIAL_MSGD_EQU_LN(tag, val)
#endif