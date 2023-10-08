 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Mahmoud Tarek
 ******************************************************************************/

#include "Port.h"
#include "tm4c123gh6pm_registers.h"
#include "Port_Regs.h"

#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and Port Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 || (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Det.h does not match the expected version"
#endif

#endif
   
STATIC const Port_ConfigChannel * Port_PortChannels = NULL_PTR;
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;   

/************************************************************************************
* Service Name: Port_Init 
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): ConfigPtr - Pointer to configuration set
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Initializes the Port Driver module
************************************************************************************/
void Port_Init(const Port_ConfigType* ConfigPtr)
{
    uint8 Current_Pin_Number = 0;
    Port_PortChannels = ConfigPtr->Channels; /* address of the first Channels structure --> Channels[0] */
    Port_Status       = PORT_INITIALIZED;
    volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
    volatile uint32 delay = 0;
    
#if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* check if the input configuration pointer is not a NULL_PTR */
	if (NULL_PTR == ConfigPtr)
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,
		     PORT_E_PARAM_CONFIG);
	}
	else
#endif
   
    
for(Current_Pin_Number=0;Current_Pin_Number < PORT_NUMBER_OF_PINS;Current_Pin_Number++)
    {  
    switch(Port_PortChannels[Current_Pin_Number].port_num)
    {
        case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break;
	case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
        case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
        case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
    }
    
    /* Enable clock for PORT and allow time for clock to start*/
    SYSCTL_REGCGC2_REG |= (1<<Port_PortChannels[Current_Pin_Number].port_num);
    delay = SYSCTL_REGCGC2_REG;
    
    if( ((Port_PortChannels[Current_Pin_Number].port_num == 3) && (Port_PortChannels[Current_Pin_Number].pin_num == 7)) || ((Port_PortChannels[Current_Pin_Number].port_num == 5) && (Port_PortChannels[Current_Pin_Number].pin_num == 0)) ) /* PD7 or PF0 */
    {
        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
    }
    else if( (Port_PortChannels[Current_Pin_Number].port_num == 2) && (Port_PortChannels[Current_Pin_Number].pin_num <= 3) ) /* PC0 to PC3 */
    {
        /* Do Nothing ...  this is the JTAG pins */
    }
    else
    {
        /* Do Nothing ... No need to unlock the commit register for this pin */
    }
if((Port_PortChannels[Current_Pin_Number].mode == PORT_PIN_MODE_0_DIO)||(Port_PortChannels[Current_Pin_Number].mode == PORT_PIN_MODE_0_ADC))                /* If condition for DIO and ADC modes */
    {
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(PORT_PIN_MODE_MASK << (Port_PortChannels[Current_Pin_Number].pin_num * 4));     /* Clear the PMCx bits for this pin */
    
    if(Port_PortChannels[Current_Pin_Number].direction == PORT_PIN_OUT)
    {
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
        
        if(Port_PortChannels[Current_Pin_Number].initial_value == STD_HIGH)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);          /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);        /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
        }
    }
    else if(Port_PortChannels[Current_Pin_Number].direction == PORT_PIN_IN)
    {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
        
        if(Port_PortChannels[Current_Pin_Number].resistor == PULL_UP)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
        }
        else if(Port_PortChannels[Current_Pin_Number].resistor == PULL_DOWN)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);     /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);     /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);   /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
        }
    }
    else
    {
        /* Do Nothing */
    }
    if(Port_PortChannels[Current_Pin_Number].mode == PORT_PIN_MODE_0_DIO)
    {
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */  
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
    }
    else
    {
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);       /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);        /* set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
    }
   }
   
else  /* All modes other than DIO and ADC */
   {
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */ 
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
    
    if(Port_PortChannels[Current_Pin_Number].mode == PORT_PIN_MODE_1)
    {
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(PORT_PIN_MODE_MASK << (Port_PortChannels[Current_Pin_Number].pin_num * 4));     /* Clear the PMCx bits for this pin */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |=  (PORT_PIN_MODE_1 << (Port_PortChannels[Current_Pin_Number].pin_num * 4));     /* Insert mode number */
    }
    else if(Port_PortChannels[Current_Pin_Number].mode == PORT_PIN_MODE_2)
    {
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(PORT_PIN_MODE_MASK << (Port_PortChannels[Current_Pin_Number].pin_num * 4));     /* Clear the PMCx bits for this pin */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |=  (PORT_PIN_MODE_2 << (Port_PortChannels[Current_Pin_Number].pin_num * 4));     /* Insert mode number */  
    }
    else if(Port_PortChannels[Current_Pin_Number].mode == PORT_PIN_MODE_3)
    {
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(PORT_PIN_MODE_MASK << (Port_PortChannels[Current_Pin_Number].pin_num * 4));     /* Clear the PMCx bits for this pin */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |=  (PORT_PIN_MODE_3 << (Port_PortChannels[Current_Pin_Number].pin_num * 4));     /* Insert mode number */  
    }
    else if(Port_PortChannels[Current_Pin_Number].mode == PORT_PIN_MODE_4)
    {
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(PORT_PIN_MODE_MASK << (Port_PortChannels[Current_Pin_Number].pin_num * 4));     /* Clear the PMCx bits for this pin */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |=  (PORT_PIN_MODE_4 << (Port_PortChannels[Current_Pin_Number].pin_num * 4));     /* Insert mode number */  
    }
    else if(Port_PortChannels[Current_Pin_Number].mode == PORT_PIN_MODE_5)
    {
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(PORT_PIN_MODE_MASK << (Port_PortChannels[Current_Pin_Number].pin_num * 4));     /* Clear the PMCx bits for this pin */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |=  (PORT_PIN_MODE_5 << (Port_PortChannels[Current_Pin_Number].pin_num * 4));     /* Insert mode number */  
    }
    else if(Port_PortChannels[Current_Pin_Number].mode == PORT_PIN_MODE_6)
    {
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(PORT_PIN_MODE_MASK << (Port_PortChannels[Current_Pin_Number].pin_num * 4));     /* Clear the PMCx bits for this pin */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |=  (PORT_PIN_MODE_6 << (Port_PortChannels[Current_Pin_Number].pin_num * 4));     /* Insert mode number */  
    }
    else if(Port_PortChannels[Current_Pin_Number].mode == PORT_PIN_MODE_7)
    {
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(PORT_PIN_MODE_MASK << (Port_PortChannels[Current_Pin_Number].pin_num * 4));     /* Clear the PMCx bits for this pin */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |=  (PORT_PIN_MODE_7 << (Port_PortChannels[Current_Pin_Number].pin_num * 4));     /* Insert mode number */  
    }
    else if(Port_PortChannels[Current_Pin_Number].mode == PORT_PIN_MODE_8)
    {
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(PORT_PIN_MODE_MASK << (Port_PortChannels[Current_Pin_Number].pin_num * 4));     /* Clear the PMCx bits for this pin */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |=  (PORT_PIN_MODE_8 << (Port_PortChannels[Current_Pin_Number].pin_num * 4));     /* Insert mode number */  
    }
    else
    {
      /* Do Nothing */
    }
    
    if(Port_PortChannels[Current_Pin_Number].direction == PORT_PIN_OUT)
    {
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
        
        if(Port_PortChannels[Current_Pin_Number].initial_value == STD_HIGH)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);          /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);        /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
        }
    }
    else if(Port_PortChannels[Current_Pin_Number].direction == PORT_PIN_IN)
    {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
        
        if(Port_PortChannels[Current_Pin_Number].resistor == PULL_UP)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
        }
        else if(Port_PortChannels[Current_Pin_Number].resistor == PULL_DOWN)
        {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);     /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
        }
        else
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);     /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);   /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
        }
    }
    else
    {
        /* Do Nothing */
    }
   }
  }
}

#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
/************************************************************************************
* Service Name: Port_SetPinDirection 
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Pin - Port Pin ID number 
*                  Direction - Port Pin Direction 
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin direction during run time
************************************************************************************/
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
  volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
  boolean error = FALSE;
  #if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* Check if the Driver is initialized before using this function */
	if (PORT_NOT_INITIALIZED == Port_Status)
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_SET_PIN_DIRECTION_SID, PORT_E_UNINIT);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
	/* Check if the used channel is within the valid range */
	if (PORT_NUMBER_OF_PINS <= Pin)
	{

		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_SET_PIN_DIRECTION_SID, PORT_E_PARAM_PIN);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
  #endif
        
/* In-case there are no errors */
 if(FALSE == error)
 {   
    switch(Port_PortChannels[Pin].port_num)
    {
        case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break;
	case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
        case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
        case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
    }
     /* If this pin is selected as changeable by the port_init function */
    if(Port_PortChannels[Pin].direction_change == DIRECTION_CHANGEABLE)
    {
      if(Direction == PORT_PIN_OUT)
      {
	SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[Pin].pin_num);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
      }
      else
      {
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[Pin].pin_num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */ 
      }
    }
    else
    {
      Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_SET_PIN_DIRECTION_SID, PORT_E_DIRECTION_UNCHANGEABLE);
		error = TRUE;
    }
 }
 else
 {
		/* No Action Required */
 }
}

#endif

#if (PORT_SET_PIN_MODE_API == STD_ON)
/************************************************************************************
* Service Name: Port_SetPinMode  
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Pin - Port Pin ID number 
*                  Mode - New Port Pin mode to be set on port pin 
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin mode during run time
************************************************************************************/
void Port_SetPinMode(Port_PinType Pin,Port_PinModeType Mode)
{
  volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
  boolean error = FALSE;
  #if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* Check if the Driver is initialized before using this function */
	if (PORT_NOT_INITIALIZED == Port_Status)
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_SET_PIN_MODE_SID, PORT_E_UNINIT);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
	/* Check if the used channel is within the valid range */
	if (PORT_NUMBER_OF_PINS <= Pin)
	{

		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_SET_PIN_MODE_SID, PORT_E_PARAM_PIN);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
        /* Check if the given mode is within the valid range */
	if (PORT_NUMBER_OF_MODES < Mode)
	{

		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_SET_PIN_MODE_SID, PORT_E_PARAM_INVALID_MODE);
		error = TRUE;
	}
	else
	{
		/* No Action Required */
	}
  #endif
        
/* In-case there are no errors */
 if(FALSE == error)
 {   
    switch(Port_PortChannels[Pin].port_num)
    {
        case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break;
	case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
        case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
        case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
    }
    /* If this pin is selected as changeable by the port_init function */
  if(Port_PortChannels[Pin].mode_change == MODE_CHANGEABLE)
  {
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PortChannels[Pin].pin_num);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */ 
    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PortChannels[Pin].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PortChannels[Pin].pin_num);               /* Enable Alternative function for this pin by setting the corresponding bit in GPIOAFSEL register */
    
    if(Port_PortChannels[Pin].mode == PORT_PIN_MODE_1)
    {
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(PORT_PIN_MODE_MASK << (Port_PortChannels[Pin].pin_num * 4));     /* Clear the PMCx bits for this pin */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |=  (PORT_PIN_MODE_1 << (Port_PortChannels[Pin].pin_num * 4));     /* Insert mode number */
    }
    else if(Port_PortChannels[Pin].mode == PORT_PIN_MODE_2)
    {
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(PORT_PIN_MODE_MASK << (Port_PortChannels[Pin].pin_num * 4));     /* Clear the PMCx bits for this pin */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |=  (PORT_PIN_MODE_2 << (Port_PortChannels[Pin].pin_num * 4));     /* Insert mode number */  
    }
    else if(Port_PortChannels[Pin].mode == PORT_PIN_MODE_3)
    {
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(PORT_PIN_MODE_MASK << (Port_PortChannels[Pin].pin_num * 4));     /* Clear the PMCx bits for this pin */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |=  (PORT_PIN_MODE_3 << (Port_PortChannels[Pin].pin_num * 4));     /* Insert mode number */  
    }
    else if(Port_PortChannels[Pin].mode == PORT_PIN_MODE_4)
    {
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(PORT_PIN_MODE_MASK << (Port_PortChannels[Pin].pin_num * 4));     /* Clear the PMCx bits for this pin */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |=  (PORT_PIN_MODE_4 << (Port_PortChannels[Pin].pin_num * 4));     /* Insert mode number */  
    }
    else if(Port_PortChannels[Pin].mode == PORT_PIN_MODE_5)
    {
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(PORT_PIN_MODE_MASK << (Port_PortChannels[Pin].pin_num * 4));     /* Clear the PMCx bits for this pin */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |=  (PORT_PIN_MODE_5 << (Port_PortChannels[Pin].pin_num * 4));     /* Insert mode number */  
    }
    else if(Port_PortChannels[Pin].mode == PORT_PIN_MODE_6)
    {
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(PORT_PIN_MODE_MASK << (Port_PortChannels[Pin].pin_num * 4));     /* Clear the PMCx bits for this pin */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |=  (PORT_PIN_MODE_6 << (Port_PortChannels[Pin].pin_num * 4));     /* Insert mode number */  
    }
    else if(Port_PortChannels[Pin].mode == PORT_PIN_MODE_7)
    {
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(PORT_PIN_MODE_MASK << (Port_PortChannels[Pin].pin_num * 4));     /* Clear the PMCx bits for this pin */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |=  (PORT_PIN_MODE_7 << (Port_PortChannels[Pin].pin_num * 4));     /* Insert mode number */  
    }
    else if(Port_PortChannels[Pin].mode == PORT_PIN_MODE_8)
    {
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(PORT_PIN_MODE_MASK << (Port_PortChannels[Pin].pin_num * 4));     /* Clear the PMCx bits for this pin */
    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |=  (PORT_PIN_MODE_8 << (Port_PortChannels[Pin].pin_num * 4));     /* Insert mode number */  
    }
    else
    {
      /* Do Nothing */
    }
    }
  else
   {
      Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_SET_PIN_MODE_SID, PORT_E_MODE_UNCHANGEABLE);
		error = TRUE;
   }
  }
 else
 {
    /* No Action Required */
 }
}
#endif

#if (PORT_VERSION_INFO_API == STD_ON)
/************************************************************************************
* Service Name: Port_GetVersionInfo 
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): ConfigPtr - Pointer to configuration set
* Parameters (inout): None
* Parameters (out): versioninfo -  Pointer to where to store the version 
information of this module
* Return value: None
* Description: Initializes the Port Driver module
************************************************************************************/
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* Check if input pointer is not Null pointer */
	if(NULL_PTR == versioninfo)
	{
		/* Report to DET  */
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
	}
	else
#endif /* (PORT_DEV_ERROR_DETECT == STD_ON) */
	{
		/* Copy the vendor Id */
		versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
		/* Copy the module Id */
		versioninfo->moduleID = (uint16)PORT_MODULE_ID;
		/* Copy Software Major Version */
		versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
		/* Copy Software Minor Version */
		versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
		/* Copy Software Patch Version */
		versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
	}
}
#endif

/************************************************************************************
* Service Name: Port_RefreshPortDirection 
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Refreshes port direction
************************************************************************************/
void Port_RefreshPortDirection(void)
{
  uint8 Current_Pin_Number = 0;
  volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
  boolean error = FALSE;
  #if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* Check if the Driver is initialized before using this function */
	if (PORT_NOT_INITIALIZED == Port_Status)
	{
		Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
				PORT_SET_PIN_MODE_SID, PORT_E_UNINIT);
		error = TRUE;
	}
  #endif
    if(FALSE == error)
    {
      /* Loop for looping on all the pins */
for(Current_Pin_Number=0;Current_Pin_Number < PORT_NUMBER_OF_PINS;Current_Pin_Number++)
{  
    switch(Port_PortChannels[Current_Pin_Number].port_num)
    {
        case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break;
	case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
        case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
        case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
    }
        /* If the pin direction is set as changeable enter the condition*/
       if(Port_PortChannels[Current_Pin_Number].direction_change == DIRECTION_UNCHANGEABLE)
       {
         if(Port_PortChannels[Current_Pin_Number].direction == PORT_PIN_OUT)
          {
	       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
          }
         else
          {
               CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PortChannels[Current_Pin_Number].pin_num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */ 
          }
       }
       else
       {
         /* Do Nothing */
       }
}
    }
    else
    {
      /* Do Nothing */
    }
}