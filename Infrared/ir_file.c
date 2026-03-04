/* See COPYING.txt for license details. */

/*
 * ir_file.c
 *
 * IR signal file parser for .ir files stored on SD card.
 *
 * M1 Project
 */

/*************************** I N C L U D E S **********************************/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "stm32h5xx_hal.h"
#include "main.h"
#include "ff.h"
#include "m1_file_util.h"
#include "m1_log_debug.h"
#include "irmp.h"
#include "ir_file.h"

/*************************** D E F I N E S ************************************/

#define M1_LOGDB_TAG        "IR_FILE"

/** Maximum length of a single line in the .ir file. */
#define IR_LINE_BUF_SIZE    512

//************************** S T R U C T U R E S *******************************

/** Mapping between .ir file protocol name strings and IRMP protocol numbers. */
typedef struct {
    const char *file_name;      /* name as it appears in the .ir file         */
    uint8_t     irmp_protocol;  /* IRMP_xxx_PROTOCOL number                   */
} IR_Protocol_Map_t;

/***************************** V A R I A B L E S ******************************/

/*
 * Protocol name mapping table.
 * The .ir file format uses Flipper-style names which sometimes differ from
 * the names in irmp_protocol_names[].  This table maps between the two
 * name spaces.
 *
 * Add new entries here as needed when .ir files use additional protocols.
 */
static const IR_Protocol_Map_t ir_protocol_map[] = {
    /* .ir file name        IRMP protocol number                              */
    { "NEC",                IRMP_NEC_PROTOCOL         },
    { "NECext",             IRMP_NEC_PROTOCOL         },  /* NEC extended     */
    { "NEC42",              IRMP_NEC42_PROTOCOL       },
    { "NEC42ext",           IRMP_NEC42_PROTOCOL       },
    { "Samsung32",          IRMP_SAMSUNG32_PROTOCOL   },
    { "Samsung",            IRMP_SAMSUNG_PROTOCOL     },
    { "Samsung48",          IRMP_SAMSUNG48_PROTOCOL   },
    { "RC5",                IRMP_RC5_PROTOCOL         },
    { "RC5X",               IRMP_RC5_PROTOCOL         },
    { "RC6",                IRMP_RC6_PROTOCOL         },
    { "SIRC",               IRMP_SIRCS_PROTOCOL       },
    { "SIRC15",             IRMP_SIRCS_PROTOCOL       },
    { "SIRC20",             IRMP_SIRCS_PROTOCOL       },
    { "Kaseikyo",           IRMP_KASEIKYO_PROTOCOL    },
    { "Panasonic",          IRMP_PANASONIC_PROTOCOL   },
    { "Denon",              IRMP_DENON_PROTOCOL       },
    { "Sharp",              IRMP_DENON_PROTOCOL       },  /* Denon/Sharp      */
    { "JVC",                IRMP_JVC_PROTOCOL         },
    { "Nokia",              IRMP_NOKIA_PROTOCOL       },
    { "Thomson",            IRMP_THOMSON_PROTOCOL     },
    { "Bose",               IRMP_BOSE_PROTOCOL        },
    { "Apple",              IRMP_APPLE_PROTOCOL       },
    { "Grundig",            IRMP_GRUNDIG_PROTOCOL     },
    { "Nikon",              IRMP_NIKON_PROTOCOL       },
    { "LEGO",               IRMP_LEGO_PROTOCOL        },
    { "LG",                 IRMP_NEC_PROTOCOL         },  /* LG uses NEC      */
    { "LGair",              IRMP_LGAIR_PROTOCOL       },
    { "Gree",               IRMP_GREE_PROTOCOL        },
    { "Onkyo",              IRMP_ONKYO_PROTOCOL       },
    { "Metz",               IRMP_METZ_PROTOCOL        },
    { "SamsungAH",          IRMP_SAMSUNGAH_PROTOCOL   },
    { "Mitsubishi_Heavy",   IRMP_MITSU_HEAVY_PROTOCOL },
    { "Roomba",             IRMP_ROOMBA_PROTOCOL      },
    { "RCMM",              IRMP_RCMM32_PROTOCOL      },
};

#define IR_PROTOCOL_MAP_SIZE  (sizeof(ir_protocol_map) / sizeof(ir_protocol_map[0]))

/********************* F U N C T I O N   P R O T O T Y P E S ******************/

static bool ir_parse_hex_bytes(const char *str, uint32_t *out);
static char *ir_trim(char *s);

/*************** F U N C T I O N   I M P L E M E N T A T I O N ****************/


/*============================================================================*/
/**
  * @brief  Trim leading and trailing whitespace/CR/LF from a string in place.
  * @param  s  String to trim
  * @retval Pointer to the trimmed string (same buffer)
  */
/*============================================================================*/
static char *ir_trim(char *s)
{
    while (*s && (isspace((unsigned char)*s)))
        s++;

    if (*s == '\0')
        return s;

    char *end = s + strlen(s) - 1;
    while (end > s && (isspace((unsigned char)*end)))
        *end-- = '\0';

    return s;
}


/*============================================================================*/
/**
  * @brief  Parse a space-separated hex byte string like "EE 87 00 00"
  *         into a uint32_t (little-endian: first byte = LSB).
  * @param  str  Input string
  * @param  out  Output value
  * @retval true on success
  */
/*============================================================================*/
static bool ir_parse_hex_bytes(const char *str, uint32_t *out)
{
    uint32_t val = 0;
    int byte_idx = 0;
    char *tok;
    char buf[32];

    strncpy(buf, str, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    tok = strtok(buf, " ");
    while (tok && byte_idx < 4)
    {
        unsigned long b = strtoul(tok, NULL, 16);
        val |= (b & 0xFF) << (byte_idx * 8);
        byte_idx++;
        tok = strtok(NULL, " ");
    }

    *out = val;
    return (byte_idx > 0);
}


/*============================================================================*/
/**
  * @brief  Parse the raw timing "data:" line into an allocated uint32_t array.
  * @param  str        The data string with space-separated decimal values
  * @param  data_out   Pointer to receive malloc'd array
  * @param  count_out  Pointer to receive number of entries
  * @retval true on success
  */
/*============================================================================*/
static bool ir_parse_raw_data(const char *str, uint32_t **data_out, uint16_t *count_out)
{
    /* First pass: count the tokens */
    uint16_t count = 0;
    const char *p = str;

    while (*p)
    {
        while (*p && isspace((unsigned char)*p))
            p++;
        if (*p == '\0')
            break;
        count++;
        while (*p && !isspace((unsigned char)*p))
            p++;
    }

    if (count == 0 || count > IR_RAW_DATA_MAX)
    {
        *data_out = NULL;
        *count_out = 0;
        return false;
    }

    uint32_t *arr = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!arr)
    {
        *data_out = NULL;
        *count_out = 0;
        return false;
    }

    /* Second pass: parse values */
    char buf[IR_LINE_BUF_SIZE];
    strncpy(buf, str, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    char *tok = strtok(buf, " ");
    uint16_t idx = 0;
    while (tok && idx < count)
    {
        arr[idx++] = (uint32_t)strtoul(tok, NULL, 10);
        tok = strtok(NULL, " ");
    }

    *data_out = arr;
    *count_out = count;
    return true;
}


/*============================================================================*/
/**
  * @brief  Look up an IRMP protocol number by the .ir file protocol name.
  * @param  name  Protocol name from the .ir file (e.g. "NECext", "RC5")
  * @retval IRMP protocol number, or 0 (IRMP_UNKNOWN_PROTOCOL) if not found
  */
/*============================================================================*/
uint8_t ir_file_get_protocol_by_name(const char *name)
{
    for (size_t i = 0; i < IR_PROTOCOL_MAP_SIZE; i++)
    {
        if (strcmp(name, ir_protocol_map[i].file_name) == 0)
            return ir_protocol_map[i].irmp_protocol;
    }

    M1_LOG_W(M1_LOGDB_TAG, "Unknown IR protocol: %s\r\n", name);
    return IRMP_UNKNOWN_PROTOCOL;
}


/*============================================================================*/
/**
  * @brief  Get the .ir file protocol name for a given IRMP protocol number.
  * @param  protocol  IRMP protocol number
  * @retval Protocol name string, or "Unknown"
  */
/*============================================================================*/
const char *ir_file_get_protocol_name(uint8_t protocol)
{
    for (size_t i = 0; i < IR_PROTOCOL_MAP_SIZE; i++)
    {
        if (ir_protocol_map[i].irmp_protocol == protocol)
            return ir_protocol_map[i].file_name;
    }
    return "Unknown";
}


/*============================================================================*/
/**
  * @brief  Load and parse an .ir file from SD card.
  *
  *         File format example:
  *           Filetype: IR signals file
  *           Version: 1
  *           #
  *           name: Power
  *           type: parsed
  *           protocol: NECext
  *           address: EE 87 00 00
  *           command: 5D A0 00 00
  *           #
  *           name: Vol_up
  *           type: raw
  *           frequency: 38000
  *           duty_cycle: 0.330000
  *           data: 504 3432 502 483 ...
  *
  * @param  dir_path   Directory path (e.g. "0:/IR/TV/Samsung")
  * @param  file_name  File name (e.g. "UN55.ir")
  * @param  ir_file    Pointer to caller-allocated IR_File_t to fill
  * @retval true on success, false on error
  */
/*============================================================================*/
bool ir_file_load(const char *dir_path, const char *file_name, IR_File_t *ir_file)
{
    FIL fil;
    FRESULT fres;
    char filepath[128];
    char line[IR_LINE_BUF_SIZE];
    bool header_valid = false;
    int cur_btn = -1;   /* index of the button currently being parsed */

    if (!ir_file)
        return false;

    memset(ir_file, 0, sizeof(IR_File_t));

    /* Build full path */
    fu_path_combine(filepath, sizeof(filepath), dir_path, file_name);

    M1_LOG_I(M1_LOGDB_TAG, "Loading: %s\r\n", filepath);

    /* Open file */
    fres = f_open(&fil, filepath, FA_READ);
    if (fres != FR_OK)
    {
        M1_LOG_E(M1_LOGDB_TAG, "f_open failed: %d\r\n", fres);
        return false;
    }

    /* Parse line by line */
    while (f_gets(line, sizeof(line), &fil) != NULL)
    {
        char *trimmed = ir_trim(line);

        /* Skip empty lines */
        if (trimmed[0] == '\0')
            continue;

        /* Comment line starting with # acts as a signal separator */
        if (trimmed[0] == '#')
            continue;

        /* Look for "key: value" */
        char *colon = strchr(trimmed, ':');
        if (!colon)
            continue;

        *colon = '\0';
        char *key = ir_trim(trimmed);
        char *value = ir_trim(colon + 1);

        /* --- Header fields --- */
        if (strcmp(key, "Filetype") == 0)
        {
            if (strcmp(value, IR_FILE_FILETYPE) != 0)
            {
                M1_LOG_E(M1_LOGDB_TAG, "Bad filetype: %s\r\n", value);
                f_close(&fil);
                return false;
            }
            header_valid = true;
            continue;
        }

        if (strcmp(key, "Version") == 0)
        {
            /* Accept version 1 */
            if (strcmp(value, IR_FILE_VERSION) != 0)
            {
                M1_LOG_W(M1_LOGDB_TAG, "Version mismatch: %s\r\n", value);
            }
            continue;
        }

        /* --- Per-button fields --- */
        if (strcmp(key, "name") == 0)
        {
            if (!header_valid)
            {
                M1_LOG_E(M1_LOGDB_TAG, "Button before header!\r\n");
                f_close(&fil);
                return false;
            }

            if (ir_file->button_count >= IR_FILE_MAX_BUTTONS)
            {
                M1_LOG_W(M1_LOGDB_TAG, "Too many buttons, skipping\r\n");
                /* Keep parsing but don't store more */
                cur_btn = -1;
                continue;
            }

            cur_btn = ir_file->button_count;
            ir_file->button_count++;

            strncpy(ir_file->buttons[cur_btn].name, value,
                    IR_FILE_BUTTON_NAME_LEN - 1);
            ir_file->buttons[cur_btn].name[IR_FILE_BUTTON_NAME_LEN - 1] = '\0';
            continue;
        }

        /* All remaining fields require a current button */
        if (cur_btn < 0)
            continue;

        IR_Button_t *btn = &ir_file->buttons[cur_btn];

        if (strcmp(key, "type") == 0)
        {
            if (strcmp(value, "parsed") == 0)
                btn->type = IR_SIGNAL_PARSED;
            else if (strcmp(value, "raw") == 0)
                btn->type = IR_SIGNAL_RAW;
            else
                M1_LOG_W(M1_LOGDB_TAG, "Unknown type: %s\r\n", value);
            continue;
        }

        if (strcmp(key, "protocol") == 0)
        {
            btn->signal.parsed.protocol = ir_file_get_protocol_by_name(value);
            continue;
        }

        if (strcmp(key, "address") == 0)
        {
            ir_parse_hex_bytes(value, &btn->signal.parsed.address);
            continue;
        }

        if (strcmp(key, "command") == 0)
        {
            ir_parse_hex_bytes(value, &btn->signal.parsed.command);
            continue;
        }

        if (strcmp(key, "frequency") == 0)
        {
            btn->signal.raw.frequency = (uint32_t)strtoul(value, NULL, 10);
            continue;
        }

        if (strcmp(key, "duty_cycle") == 0)
        {
            btn->signal.raw.duty_cycle = (float)strtod(value, NULL);
            continue;
        }

        if (strcmp(key, "data") == 0)
        {
            ir_parse_raw_data(value, &btn->signal.raw.data,
                              &btn->signal.raw.data_count);
            continue;
        }

    } /* while f_gets */

    f_close(&fil);

    M1_LOG_I(M1_LOGDB_TAG, "Loaded %d buttons\r\n", ir_file->button_count);

    return (header_valid && ir_file->button_count > 0);
}


/*============================================================================*/
/**
  * @brief  Free dynamically allocated raw timing data inside an IR_File_t.
  * @param  ir_file  Pointer to a previously loaded IR_File_t
  */
/*============================================================================*/
void ir_file_free(IR_File_t *ir_file)
{
    if (!ir_file)
        return;

    for (uint16_t i = 0; i < ir_file->button_count; i++)
    {
        if (ir_file->buttons[i].type == IR_SIGNAL_RAW)
        {
            if (ir_file->buttons[i].signal.raw.data)
            {
                free(ir_file->buttons[i].signal.raw.data);
                ir_file->buttons[i].signal.raw.data = NULL;
            }
        }
    }

    ir_file->button_count = 0;
}
