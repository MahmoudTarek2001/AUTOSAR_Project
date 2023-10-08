 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Cfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Mahmoud Tarek
 ******************************************************************************/

#ifndef PORT_CFG_H
#define PORT_CFG_H

/*
 * Module Version 1.0.0
 */
#define PORT_CFG_SW_MAJOR_VERSION              (1U)
#define PORT_CFG_SW_MINOR_VERSION              (0U)
#define PORT_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (3U)

/* Pre-compile option for Development Error Detect */
#define PORT_DEV_ERROR_DETECT                (STD_ON)

/* Pre-compile option for Version Info API */
#define PORT_VERSION_INFO_API                (STD_ON)

/* Pre-compile option for presence of PortSetPinDirection Api */
#define PORT_SET_PIN_DIRECTION_API           (STD_ON)
   
/* Pre-processor switch to enable / disable the use of the function
Port_SetPinMode */
#define PORT_SET_PIN_MODE_API                (STD_ON)
   
/* Number of the configured Port Channels */
#define PORT_NUMBER_OF_PINS                  (39U)
   
/* Number of the configured Port Channels */
#define PORT_NUMBER_OF_MODES                  (9U)

/* Number of modes for Port Channels */
#define PORT_PIN_MODE_MASK                    (Port_PinModeType)0x0000000F /* Mask for changing modes */
#define PORT_PIN_MODE_0_DIO                   (Port_PinModeType)0x00000000 /* Mode0 for DIO */
#define PORT_PIN_MODE_1                       (Port_PinModeType)0x00000001 /* Mode1 */
#define PORT_PIN_MODE_2                       (Port_PinModeType)0x00000002 /* Mode2 */
#define PORT_PIN_MODE_3                       (Port_PinModeType)0x00000003 /* Mode3 */
#define PORT_PIN_MODE_4                       (Port_PinModeType)0x00000004 /* Mode4 */
#define PORT_PIN_MODE_5                       (Port_PinModeType)0x00000005 /* Mode5 */
#define PORT_PIN_MODE_6                       (Port_PinModeType)0x00000006 /* Mode6 */
#define PORT_PIN_MODE_7                       (Port_PinModeType)0x00000007 /* Mode7 */
#define PORT_PIN_MODE_8                       (Port_PinModeType)0x00000008 /* Mode8 */
#define PORT_PIN_MODE_0_ADC                   (Port_PinModeType)0x00000009 /* Mode0 for ADC */

/* PORT Configured Port ID's  */
#define PortConf_A_PORT_NUM                   (Port_PortType)0 /* PORTA */
#define PortConf_B_PORT_NUM                   (Port_PortType)1 /* PORTB */
#define PortConf_C_PORT_NUM                   (Port_PortType)2 /* PORTC */
#define PortConf_D_PORT_NUM                   (Port_PortType)3 /* PORTD */
#define PortConf_E_PORT_NUM                   (Port_PortType)4 /* PORTE */
#define PortConf_F_PORT_NUM                   (Port_PortType)5 /* PORTF */

/* PORT Configured Channel ID's */
#define PortConf_PIN0_CHANNEL_NUM             (Port_PinType)0 /* Pin 0 */
#define PortConf_PIN1_CHANNEL_NUM             (Port_PinType)1 /* Pin 1 */
#define PortConf_PIN2_CHANNEL_NUM             (Port_PinType)2 /* Pin 2 */
#define PortConf_PIN3_CHANNEL_NUM             (Port_PinType)3 /* Pin 3 */
#define PortConf_PIN4_CHANNEL_NUM             (Port_PinType)4 /* Pin 4 */
#define PortConf_PIN5_CHANNEL_NUM             (Port_PinType)5 /* Pin 5 */
#define PortConf_PIN6_CHANNEL_NUM             (Port_PinType)6 /* Pin 6 */
#define PortConf_PIN7_CHANNEL_NUM             (Port_PinType)7 /* Pin 7 */

/* Channel Index in the array of structures in Port_PBcfg.c */
#define PortConf_0_CHANNEL_ID_INDEX            (Port_PinType)0  /* Channel ID 0 */
#define PortConf_1_CHANNEL_ID_INDEX            (Port_PinType)1  /* Channel ID 1 */
#define PortConf_2_CHANNEL_ID_INDEX            (Port_PinType)2  /* Channel ID 2 */
#define PortConf_3_CHANNEL_ID_INDEX            (Port_PinType)3  /* Channel ID 3 */
#define PortConf_4_CHANNEL_ID_INDEX            (Port_PinType)4  /* Channel ID 4 */
#define PortConf_5_CHANNEL_ID_INDEX            (Port_PinType)5  /* Channel ID 5 */
#define PortConf_6_CHANNEL_ID_INDEX            (Port_PinType)6  /* Channel ID 6 */
#define PortConf_7_CHANNEL_ID_INDEX            (Port_PinType)7  /* Channel ID 7 */
#define PortConf_8_CHANNEL_ID_INDEX            (Port_PinType)8  /* Channel ID 8 */
#define PortConf_9_CHANNEL_ID_INDEX            (Port_PinType)9  /* Channel ID 9 */
#define PortConf_10_CHANNEL_ID_INDEX           (Port_PinType)10 /* Channel ID 10 */
#define PortConf_11_CHANNEL_ID_INDEX           (Port_PinType)11 /* Channel ID 11 */
#define PortConf_12_CHANNEL_ID_INDEX           (Port_PinType)12 /* Channel ID 12 */
#define PortConf_13_CHANNEL_ID_INDEX           (Port_PinType)13 /* Channel ID 13 */
#define PortConf_14_CHANNEL_ID_INDEX           (Port_PinType)14 /* Channel ID 14 */
#define PortConf_15_CHANNEL_ID_INDEX           (Port_PinType)15 /* Channel ID 15 */
#define PortConf_16_CHANNEL_ID_INDEX           (Port_PinType)16 /* Channel ID 16 */
#define PortConf_17_CHANNEL_ID_INDEX           (Port_PinType)17 /* Channel ID 17 */
#define PortConf_18_CHANNEL_ID_INDEX           (Port_PinType)18 /* Channel ID 18 */
#define PortConf_19_CHANNEL_ID_INDEX           (Port_PinType)19 /* Channel ID 19 */
#define PortConf_20_CHANNEL_ID_INDEX           (Port_PinType)20 /* Channel ID 20 */
#define PortConf_21_CHANNEL_ID_INDEX           (Port_PinType)21 /* Channel ID 21 */
#define PortConf_22_CHANNEL_ID_INDEX           (Port_PinType)22 /* Channel ID 22 */
#define PortConf_23_CHANNEL_ID_INDEX           (Port_PinType)23 /* Channel ID 23 */
#define PortConf_24_CHANNEL_ID_INDEX           (Port_PinType)24 /* Channel ID 24 */
#define PortConf_25_CHANNEL_ID_INDEX           (Port_PinType)25 /* Channel ID 25 */
#define PortConf_26_CHANNEL_ID_INDEX           (Port_PinType)26 /* Channel ID 26 */
#define PortConf_27_CHANNEL_ID_INDEX           (Port_PinType)27 /* Channel ID 27 */
#define PortConf_28_CHANNEL_ID_INDEX           (Port_PinType)28 /* Channel ID 28 */
#define PortConf_29_CHANNEL_ID_INDEX           (Port_PinType)29 /* Channel ID 29 */
#define PortConf_30_CHANNEL_ID_INDEX           (Port_PinType)30 /* Channel ID 30 */
#define PortConf_31_CHANNEL_ID_INDEX           (Port_PinType)31 /* Channel ID 31 */
#define PortConf_32_CHANNEL_ID_INDEX           (Port_PinType)32 /* Channel ID 32 */
#define PortConf_33_CHANNEL_ID_INDEX           (Port_PinType)33 /* Channel ID 33 */
#define PortConf_34_CHANNEL_ID_INDEX           (Port_PinType)34 /* Channel ID 34 */
#define PortConf_35_CHANNEL_ID_INDEX           (Port_PinType)35 /* Channel ID 35 */
#define PortConf_36_CHANNEL_ID_INDEX           (Port_PinType)36 /* Channel ID 36 */
#define PortConf_37_CHANNEL_ID_INDEX           (Port_PinType)37 /* Channel ID 37 */
#define PortConf_38_CHANNEL_ID_INDEX           (Port_PinType)38 /* Channel ID 38 */
#define PortConf_39_CHANNEL_ID_INDEX           (Port_PinType)39 /* Channel ID 39 */

#endif /* DIO_CFG_H */
