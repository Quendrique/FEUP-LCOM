#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 KBC.
 */

#define DELAY_US 20000
#define MAKECODE_LONG  0xe0

#define BIT(n) (0x01<<(n))

#define KBD_IRQ		1 /** @brief Keyboard's IRQ line */
#define MOUSE_IRQ	12 //not sure if correct

/* I/0 port addresses */

#define KBD_STAT_REG	0x64 /** @brief Address of the KBC's status register */
#define OUT_BUFFER		0x60 /** @brief Address of the KBC's out buffer */

/* Keyboard control */

/* Status register format */

#define KBD_STAT_PARITY		BIT(7)
#define KBD_STAT_TIMEOUT	BIT(6)
#define KBD_STAT_AUX		BIT(5)
#define KBD_STAT_INH		BIT(4)
#define KBD_STAT_A2			BIT(3)
#define KBD_STAT_SYS		BIT(2)
#define KBD_STAT_IBF		BIT(1) /* @brief Status of the input buffer */
#define KBD_STAT_OBF		BIT(0) /* @brief Status of the output buffer */


/* Key scancodes */

#define ESC_KEY_BREAKCODE	0x81

/* KBC commands */

#define READ_CMD			0x20
#define WRITE_CMD			0x60
#define CHECK_KBC			0xAA
#define CHECK_KBD_IF		0xAB
#define KBD_DIS				0xAD
#define KBD_ENB				0xAE
#define MOUSE_ENB			0xA8

#define MOUSE_DIS_INT_CMD	0x45
#define MOUSE_ENB_INT_CMD	0x47

/* KBC command byte format */

#define KBC_DIS_MOUSE 		BIT(5)
#define KBC_DIS_KBD			BIT(4)
#define KBC_ENB_MOUSE		BIT(1)
#define KBC_ENB_KBD			BIT(0)

/* Mouse commands and constants */

#define MOUSE_WRITE_BYTE	0xD4
#define ACK					0xFA
#define NACK				0xFE
#define ERROR				0xFC
#define DIS_DATA_REPORT		0xF5
#define ENB_DATA_REPORT		0xF4
#define	ENB_STREAM			0xEA
#define MOUSE_READ_DATA		0xEB
#define MOUSE_REMOTE_MODE	0xF0

/* Event handling */

#define NEGATIVE_X			BIT(4)
#define NEGATIVE_Y			BIT(5)
#define RDOWN_CONST			BIT(1)

/**@}*/

#endif /* _LCOM_I8024_H */
