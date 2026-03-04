/* See COPYING.txt for license details. */

/*
 * ir_file.h
 *
 * IR signal file parser for .ir files stored on SD card.
 * File format supports both "parsed" signals (protocol/address/command)
 * and "raw" signals (frequency/duty_cycle/data timings).
 *
 * Folder layout:  IR/<Category>/<Brand>/<Device>.ir
 *
 * M1 Project
 */

#ifndef IR_FILE_H_
#define IR_FILE_H_

#include <stdbool.h>
#include <stdint.h>
#include "m1_file_browser.h"

/*************************** D E F I N E S ************************************/

#define IR_FILE_EXTENSION           "ir"
#define IR_FILEPATH                  "0:/IR"

#define IR_FILE_MAX_BUTTONS          64      /* max buttons per .ir file       */
#define IR_FILE_BUTTON_NAME_LEN      32      /* max chars in a button name     */
#define IR_RAW_DATA_MAX             1024     /* max raw timing samples         */

#define IR_FILE_FILETYPE            "IR signals file"
#define IR_FILE_VERSION             "1"

//************************** S T R U C T U R E S *******************************

/** Signal type: parsed (protocol-based) or raw (timing-based). */
typedef enum {
    IR_SIGNAL_PARSED = 0,
    IR_SIGNAL_RAW
} IR_Signal_Type_t;

/** One parsed IR signal (protocol/address/command). */
typedef struct {
    uint8_t  protocol;          /* IRMP protocol number                       */
    uint32_t address;           /* 4-byte address, little-endian in file      */
    uint32_t command;           /* 4-byte command, little-endian in file      */
} IR_Parsed_Signal_t;

/** One raw IR signal (carrier + timing array). */
typedef struct {
    uint32_t frequency;         /* carrier frequency in Hz, e.g. 38000       */
    float    duty_cycle;        /* carrier duty cycle, e.g. 0.33             */
    uint16_t data_count;        /* number of timing entries                   */
    uint32_t *data;             /* dynamically allocated array of timings     */
} IR_Raw_Signal_t;

/** A single button entry from an .ir file. */
typedef struct {
    char             name[IR_FILE_BUTTON_NAME_LEN];
    IR_Signal_Type_t type;
    union {
        IR_Parsed_Signal_t parsed;
        IR_Raw_Signal_t    raw;
    } signal;
} IR_Button_t;

/** Handle returned by ir_file_load(), holds all buttons from one .ir file. */
typedef struct {
    uint16_t     button_count;
    IR_Button_t  buttons[IR_FILE_MAX_BUTTONS];
} IR_File_t;

/********************* F U N C T I O N   P R O T O T Y P E S ******************/

/**
 * @brief  Load and parse an .ir file from SD card.
 * @param  dir_path   Directory path, e.g. "0:/IR/TV/Samsung"
 * @param  file_name  File name, e.g. "UN55.ir"
 * @param  ir_file    Pointer to caller-allocated IR_File_t to fill in
 * @retval true on success, false on file error or bad format
 */
bool ir_file_load(const char *dir_path, const char *file_name, IR_File_t *ir_file);

/**
 * @brief  Free any dynamically allocated raw timing data inside an IR_File_t.
 *         Must be called when done with a loaded file.
 * @param  ir_file  Pointer to a previously-loaded IR_File_t
 */
void ir_file_free(IR_File_t *ir_file);

/**
 * @brief  Look up an IRMP protocol number by the .ir file's protocol name
 *         string (e.g. "NECext" -> IRMP_NEC_PROTOCOL).
 * @param  name  Protocol name from the .ir file
 * @retval IRMP protocol number, or 0 (IRMP_UNKNOWN_PROTOCOL) if not found
 */
uint8_t ir_file_get_protocol_by_name(const char *name);

/**
 * @brief  Get the .ir file protocol name string for a given IRMP protocol number.
 * @param  protocol  IRMP protocol number
 * @retval Protocol name string, or "Unknown" if not mapped
 */
const char *ir_file_get_protocol_name(uint8_t protocol);

#endif /* IR_FILE_H_ */
