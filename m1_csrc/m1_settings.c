/* See COPYING.txt for license details. */

/*
*
*  m1_settings.c
*
*  M1 RFID functions
*
* M1 Project
*
*/

/*************************** I N C L U D E S **********************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "stm32h5xx_hal.h"
#include "main.h"
#include "m1_settings.h"
#include "m1_buzzer.h"
#include "m1_fw_update_bl.h"
#include "m1_power_ctl.h"

/*************************** D E F I N E S ************************************/

#define SETTING_ABOUT_CHOICES_MAX		2 //5

#define ABOUT_BOX_Y_POS_ROW_1			10
#define ABOUT_BOX_Y_POS_ROW_2			20
#define ABOUT_BOX_Y_POS_ROW_3			30
#define ABOUT_BOX_Y_POS_ROW_4			40
#define ABOUT_BOX_Y_POS_ROW_5			50

#define DELAY_BEFORE_BANK_SWAP			1000 // ms

//************************** S T R U C T U R E S *******************************

/***************************** V A R I A B L E S ******************************/

/********************* F U N C T I O N   P R O T O T Y P E S ******************/

void menu_settings_init(void);
void menu_settings_exit(void);
void settings_system(void);
void settings_about(void);
void settings_swap_fw_bank(void);
static void settings_about_display_choice(uint8_t choice);

/*************** F U N C T I O N   I M P L E M E N T A T I O N ****************/


/*============================================================================*/
/**
  * @brief
  * @param
  * @retval
  */
/*============================================================================*/
void menu_settings_init(void)
{
	;
} // void menu_settings_init(void)



/*============================================================================*/
/**
  * @brief
  * @param
  * @retval
  */
/*============================================================================*/
void menu_settings_exit(void)
{
	;
} // void menu_settings_exit(void)



/*============================================================================*/
/**
  * @brief
  * @param
  * @retval
  */
/*============================================================================*/
void settings_lcd_and_notifications(void)
{
	S_M1_Buttons_Status this_button_status;
	S_M1_Main_Q_t q_item;
	BaseType_t ret;

    /* Graphic work starts here */
    u8g2_FirstPage(&m1_u8g2); // This call required for page drawing in mode 1
    do
    {
		u8g2_SetFont(&m1_u8g2, M1_DISP_MAIN_MENU_FONT_N);

		u8g2_DrawStr(&m1_u8g2, 6, 25, "LCD...");

    } while (u8g2_NextPage(&m1_u8g2));


	while (1 ) // Main loop of this task
	{
		;
		; // Do other parts of this task here
		;

		// Wait for the notification from button_event_handler_task to subfunc_handler_task.
		// This task is the sub-task of subfunc_handler_task.
		// The notification is given in the form of an item in the main queue.
		// So let read the main queue.
		ret = xQueueReceive(main_q_hdl, &q_item, portMAX_DELAY);
		if (ret==pdTRUE)
		{
			if ( q_item.q_evt_type==Q_EVENT_KEYPAD )
			{
				// Notification is only sent to this task when there's any button activity,
				// so it doesn't need to wait when reading the event from the queue
				ret = xQueueReceive(button_events_q_hdl, &this_button_status, 0);
				if ( this_button_status.event[BUTTON_BACK_KP_ID]==BUTTON_EVENT_CLICK ) // user wants to exit?
				{
					; // Do extra tasks here if needed

					xQueueReset(main_q_hdl); // Reset main q before return
					break; // Exit and return to the calling task (subfunc_handler_task)
				} // if ( m1_buttons_status[BUTTON_BACK_KP_ID]==BUTTON_EVENT_CLICK )
				else
				{
					; // Do other things for this task, if needed
				}
			} // if ( q_item.q_evt_type==Q_EVENT_KEYPAD )
			else
			{
				; // Do other things for this task
			}
		} // if (ret==pdTRUE)
	} // while (1 ) // Main loop of this task

} // void settings_lcd_and_notifications(void)



/*============================================================================*/
/**
  * @brief
  * @param
  * @retval
  */
/*============================================================================*/
void settings_buzzer(void)
{
	//buzzer_demo_play();
} // void settings_sound(void)



/*============================================================================*/
/**
  * @brief
  * @param
  * @retval
  */
/*============================================================================*/
void settings_power(void)
{
	;
} // void settings_power(void)



/*============================================================================*/
/**
  * @brief
  * @param
  * @retval
  */
/*============================================================================*/
void settings_system(void)
{
	S_M1_Buttons_Status this_button_status;
	S_M1_Main_Q_t q_item;
	BaseType_t ret;

    /* Graphic work starts here */
    u8g2_FirstPage(&m1_u8g2); // This call required for page drawing in mode 1
    do
    {
		u8g2_SetFont(&m1_u8g2, M1_DISP_MAIN_MENU_FONT_N);

		u8g2_DrawStr(&m1_u8g2, 6, 25, "SYSTEM...");

    } while (u8g2_NextPage(&m1_u8g2));

	while (1 ) // Main loop of this task
	{
		;
		; // Do other parts of this task here
		;

		// Wait for the notification from button_event_handler_task to subfunc_handler_task.
		// This task is the sub-task of subfunc_handler_task.
		// The notification is given in the form of an item in the main queue.
		// So let read the main queue.
		ret = xQueueReceive(main_q_hdl, &q_item, portMAX_DELAY);
		if (ret==pdTRUE)
		{
			if ( q_item.q_evt_type==Q_EVENT_KEYPAD )
			{
				// Notification is only sent to this task when there's any button activity,
				// so it doesn't need to wait when reading the event from the queue
				ret = xQueueReceive(button_events_q_hdl, &this_button_status, 0);
				if ( this_button_status.event[BUTTON_BACK_KP_ID]==BUTTON_EVENT_CLICK ) // user wants to exit?
				{
					; // Do extra tasks here if needed

					xQueueReset(main_q_hdl); // Reset main q before return
					break; // Exit and return to the calling task (subfunc_handler_task)
				} // if ( m1_buttons_status[BUTTON_BACK_KP_ID]==BUTTON_EVENT_CLICK )
				else
				{
					; // Do other things for this task, if needed
				}
			} // if ( q_item.q_evt_type==Q_EVENT_KEYPAD )
			else
			{
				; // Do other things for this task
			}
		} // if (ret==pdTRUE)
	} // while (1 ) // Main loop of this task

} // void settings_system(void)



/*============================================================================*/
/**
  * @brief
  * @param
  * @retval
  */
/*============================================================================*/
void settings_about(void)
{
	S_M1_Buttons_Status this_button_status;
	S_M1_Main_Q_t q_item;
	BaseType_t ret;
	uint8_t choice;

	/* Graphic work starts here */
	u8g2_FirstPage(&m1_u8g2);
	u8g2_SetDrawColor(&m1_u8g2, M1_DISP_DRAW_COLOR_TXT);
	u8g2_DrawBox(&m1_u8g2, 0, 52, 128, 12); // Draw an inverted bar at the bottom to display options
	u8g2_SetDrawColor(&m1_u8g2, M1_DISP_DRAW_COLOR_BG); // Write text in inverted color
	u8g2_SetFont(&m1_u8g2, M1_DISP_FUNC_MENU_FONT_N);
	u8g2_DrawXBMP(&m1_u8g2, 1, 53, 8, 8, arrowleft_8x8); // draw arrowleft icon
	u8g2_DrawStr(&m1_u8g2, 11, 61, "Prev.");
	u8g2_DrawXBMP(&m1_u8g2, 119, 53, 8, 8, arrowright_8x8); // draw arrowright icon
	u8g2_DrawStr(&m1_u8g2, 97, 61, "Next");
	m1_u8g2_nextpage(); // Update display RAM

	choice = 0;
	settings_about_display_choice(choice);

	while (1 ) // Main loop of this task
	{
		;
		; // Do other parts of this task here
		;

		ret = xQueueReceive(main_q_hdl, &q_item, portMAX_DELAY);
		if (ret==pdTRUE)
		{
			if ( q_item.q_evt_type==Q_EVENT_KEYPAD )
			{
				// Notification is only sent to this task when there's any button activity,
				// so it doesn't need to wait when reading the event from the queue
				ret = xQueueReceive(button_events_q_hdl, &this_button_status, 0);
				if ( this_button_status.event[BUTTON_BACK_KP_ID]==BUTTON_EVENT_CLICK ) // user wants to exit?
				{
					; // Do extra tasks here if needed
					xQueueReset(main_q_hdl); // Reset main q before return
					break; // Exit and return to the calling task (subfunc_handler_task)
				} // if ( this_button_status.event[BUTTON_BACK_KP_ID]==BUTTON_EVENT_CLICK )
				else if ( this_button_status.event[BUTTON_LEFT_KP_ID]==BUTTON_EVENT_CLICK ) // Previous?
				{
					choice--;
					if ( choice > SETTING_ABOUT_CHOICES_MAX )
						choice = SETTING_ABOUT_CHOICES_MAX;
					settings_about_display_choice(choice);
				} // else if ( this_button_status.event[BUTTON_LEFT_KP_ID]==BUTTON_EVENT_CLICK )
				else if ( this_button_status.event[BUTTON_RIGHT_KP_ID]==BUTTON_EVENT_CLICK ) // Next?
				{
					choice++;
					if ( choice > SETTING_ABOUT_CHOICES_MAX )
						choice = 0;
					settings_about_display_choice(choice);
				} // else if ( this_button_status.event[BUTTON_RIGHT_KP_ID]==BUTTON_EVENT_CLICK )
			} // if ( q_item.q_evt_type==Q_EVENT_KEYPAD )
			else
			{
				; // Do other things for this task
			}
		} // if (ret==pdTRUE)
	} // while (1 ) // Main loop of this task

} // void settings_about(void)



/*============================================================================*/
/**
  * @brief
  * @param
  * @retval
  */
/*============================================================================*/
static void settings_about_display_choice(uint8_t choice)
{
	uint8_t prn_name[20];

	u8g2_SetDrawColor(&m1_u8g2, M1_DISP_DRAW_COLOR_BG); // Set background color
	u8g2_DrawBox(&m1_u8g2, 0, 0, M1_LCD_DISPLAY_WIDTH, ABOUT_BOX_Y_POS_ROW_5 + 1); // Clear old content
	u8g2_SetDrawColor(&m1_u8g2, M1_DISP_DRAW_COLOR_TXT); // Set text color

	switch (choice)
	{
		case 0: // FW info
			u8g2_SetFont(&m1_u8g2, M1_DISP_SUB_MENU_FONT_B); // Set bold font
			u8g2_DrawStr(&m1_u8g2, 0, ABOUT_BOX_Y_POS_ROW_1, "FW version info:");
			u8g2_SetFont(&m1_u8g2, M1_DISP_SUB_MENU_FONT_N); // Set normal font
			sprintf(prn_name, "%d.%d.%d.%d", m1_device_stat.config.fw_version_major, m1_device_stat.config.fw_version_minor, m1_device_stat.config.fw_version_build, m1_device_stat.config.fw_version_rc);
			u8g2_DrawStr(&m1_u8g2, 0, ABOUT_BOX_Y_POS_ROW_2, prn_name);
			sprintf(prn_name, "Active bank: %d", (m1_device_stat.active_bank==BANK1_ACTIVE)?1:2);
			u8g2_DrawStr(&m1_u8g2, 0, ABOUT_BOX_Y_POS_ROW_3, prn_name);
			break;

		case 1: // Company info
			u8g2_SetFont(&m1_u8g2, M1_DISP_SUB_MENU_FONT_N); // Set small font
			u8g2_DrawStr(&m1_u8g2, 0, ABOUT_BOX_Y_POS_ROW_1, "VintageVolts M1 FW");
			u8g2_DrawStr(&m1_u8g2, 0, ABOUT_BOX_Y_POS_ROW_2, "http://www.vintagevolts.com");
			u8g2_DrawStr(&m1_u8g2, 0, ABOUT_BOX_Y_POS_ROW_4, "https://github.com/VintageVolts");
			u8g2_DrawStr(&m1_u8g2, 0, ABOUT_BOX_Y_POS_ROW_5, "/M1_VintageVolts");
			break;

		default:
			u8g2_DrawXBMP(&m1_u8g2, 23, 1, 82, 36, m1_device_82x36);
			break;
	} // switch (choice)

	m1_u8g2_nextpage(); // Update display RAM
} // static void settings_about_display_choice(uint8_t choice)



/*============================================================================*/
/**
  * @brief  Allow the user to swap between firmware banks.
  *         Shows the active bank, reads the other bank's FW version,
  *         and asks for confirmation before swapping + rebooting.
  * @param  None
  * @retval None
  */
/*============================================================================*/
void settings_swap_fw_bank(void)
{
	S_M1_Buttons_Status this_button_status;
	S_M1_Main_Q_t q_item;
	BaseType_t ret;
	char prn_buf[28];
	uint16_t cur_bank;
	uint8_t other_bank_valid = 0;
	S_M1_FW_CONFIG_t other_fw_config;

	cur_bank = m1_device_stat.active_bank;

	/* ---- Try to read the firmware config from the other bank ---- */
	{
		__IO uint32_t *bu_reg_read;
		uint16_t i, k;

		bu_reg_read = (__IO uint32_t *)(FW_CONFiG_ADDRESS + M1_FLASH_BANK_SIZE);
		k = FW_CONFiG_SIZE / 4 - 1; /* Scan for magic_number_2 */
		for (i = 0; i < k; i++)
		{
			if (*bu_reg_read == FW_CONFIG_MAGIC_NUMBER_2)
				break;
			bu_reg_read++;
		}
		if (i < k)
		{
			/* Also run a CRC check on the other bank's image */
			uint32_t crc32_add;
			bu_reg_read++; /* Move past magic_number_2 to CRC32 location */
			crc32_add = (uint32_t)bu_reg_read;
			crc32_add -= (FW_START_ADDRESS + M1_FLASH_BANK_SIZE);
			crc32_add /= 4; /* Convert bytes to words */
			if (bl_crc_check(crc32_add) == BL_CODE_OK)
			{
				i++; /* Include CRC32 slot */
				memcpy((uint8_t *)&other_fw_config,
				       (__IO uint8_t *)(FW_CONFiG_ADDRESS + M1_FLASH_BANK_SIZE),
				       i * 4);
				other_bank_valid = 1;
			}
		}
	}

	/* ---- Draw the initial screen ---- */
	u8g2_FirstPage(&m1_u8g2);
	u8g2_SetDrawColor(&m1_u8g2, M1_DISP_DRAW_COLOR_TXT);
	u8g2_SetFont(&m1_u8g2, M1_DISP_SUB_MENU_FONT_B);
	u8g2_DrawStr(&m1_u8g2, 0, ABOUT_BOX_Y_POS_ROW_1, "Swap FW Bank");

	u8g2_SetFont(&m1_u8g2, M1_DISP_SUB_MENU_FONT_N);

	/* Show current bank and version */
	sprintf(prn_buf, "Active: Bank %d", (cur_bank == BANK1_ACTIVE) ? 1 : 2);
	u8g2_DrawStr(&m1_u8g2, 0, ABOUT_BOX_Y_POS_ROW_2, prn_buf);
	sprintf(prn_buf, "Ver %d.%d.%d.%d",
		m1_device_stat.config.fw_version_major,
		m1_device_stat.config.fw_version_minor,
		m1_device_stat.config.fw_version_build,
		m1_device_stat.config.fw_version_rc);
	u8g2_DrawStr(&m1_u8g2, 0, ABOUT_BOX_Y_POS_ROW_3, prn_buf);

	if (other_bank_valid)
	{
		sprintf(prn_buf, "Other: Bank %d  v%d.%d.%d.%d",
			(cur_bank == BANK1_ACTIVE) ? 2 : 1,
			other_fw_config.fw_version_major,
			other_fw_config.fw_version_minor,
			other_fw_config.fw_version_build,
			other_fw_config.fw_version_rc);
		u8g2_DrawStr(&m1_u8g2, 0, ABOUT_BOX_Y_POS_ROW_4, prn_buf);
	}
	else
	{
		u8g2_DrawStr(&m1_u8g2, 0, ABOUT_BOX_Y_POS_ROW_4, "Other bank: no valid FW");
	}

	/* Bottom bar: BACK = cancel, OK = swap */
	u8g2_SetDrawColor(&m1_u8g2, M1_DISP_DRAW_COLOR_TXT);
	u8g2_DrawBox(&m1_u8g2, 0, 52, 128, 12);
	u8g2_SetDrawColor(&m1_u8g2, M1_DISP_DRAW_COLOR_BG);
	u8g2_SetFont(&m1_u8g2, M1_DISP_FUNC_MENU_FONT_N);
	if (other_bank_valid)
	{
		u8g2_DrawStr(&m1_u8g2, 2, 61, "BACK");
		u8g2_DrawStr(&m1_u8g2, 93, 61, "SWAP");
	}
	else
	{
		u8g2_DrawStr(&m1_u8g2, 2, 61, "BACK");
	}
	m1_u8g2_nextpage();

	/* ---- Event loop ---- */
	while (1)
	{
		ret = xQueueReceive(main_q_hdl, &q_item, portMAX_DELAY);
		if (ret == pdTRUE)
		{
			if (q_item.q_evt_type == Q_EVENT_KEYPAD)
			{
				ret = xQueueReceive(button_events_q_hdl, &this_button_status, 0);

				/* BACK or LEFT: cancel and return */
				if (this_button_status.event[BUTTON_BACK_KP_ID] == BUTTON_EVENT_CLICK ||
				    this_button_status.event[BUTTON_LEFT_KP_ID] == BUTTON_EVENT_CLICK)
				{
					xQueueReset(main_q_hdl);
					break;
				}

				/* OK or RIGHT: swap banks (only if the other bank is valid) */
				if (other_bank_valid &&
				    (this_button_status.event[BUTTON_OK_KP_ID] == BUTTON_EVENT_CLICK ||
				     this_button_status.event[BUTTON_RIGHT_KP_ID] == BUTTON_EVENT_CLICK))
				{
					/* Show "Swapping..." message */
					u8g2_SetDrawColor(&m1_u8g2, M1_DISP_DRAW_COLOR_BG);
					u8g2_DrawBox(&m1_u8g2, 0, 0, 128, 64);
					u8g2_SetDrawColor(&m1_u8g2, M1_DISP_DRAW_COLOR_TXT);
					u8g2_SetFont(&m1_u8g2, M1_DISP_MAIN_MENU_FONT_N);
					u8g2_DrawStr(&m1_u8g2, 10, 25, "Swapping banks...");
					u8g2_DrawStr(&m1_u8g2, 10, 40, "Rebooting...");
					m1_u8g2_nextpage();

					startup_config_write(BK_REGS_SELECT_DEV_OP_STAT, DEV_OP_STATUS_REBOOT);
					vTaskDelay(pdMS_TO_TICKS(DELAY_BEFORE_BANK_SWAP));
					m1_pre_power_down();
					bl_swap_banks(); /* This resets the MCU and does not return */
				}
			} /* Q_EVENT_KEYPAD */
		} /* if (ret == pdTRUE) */
	} /* while (1) */

} // void settings_swap_fw_bank(void)
