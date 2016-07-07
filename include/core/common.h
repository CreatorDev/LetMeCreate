/**
 * @file include/core/common.h
 * @author Francois Berder
 * @date 2016
 * @copyright 3-clause BSD
 */


#ifndef __LETMECREATE_CORE_COMMON_H__
#define __LETMECREATE_CORE_COMMON_H__

#include <stdint.h>

#define MAX_STR_LENGTH          (255)

/** Index of Mikrobus interfaces */
enum MIKROBUS_INDEX {
    MIKROBUS_1,
    MIKROBUS_2
};

/**
 * @brief Write a string to a device file.
 *
 * @param[in] path Path to the file (must not be null)
 * @param[in] str String to write (must not be null)
 * @return 0 if successful, -1 otherwise
 */
int write_str_file(const char *path, const char *str);

/**
 * @brief Write an integer to a device file.
 *
 * @param[in] path Path to the file (must not be null)
 * @param[in] value Value to write
 * @return 0 if successful, -1 otherwise
 */
int write_int_file(const char *path, uint32_t value);

/**
 * @brief Read a string from a device file.
 *
 * @param[in] path Path to the file (must not be null)
 * @param[out] str
 * @param[in] max_str_length Maximum number of characters to read
 * @return 0 if successful, -1 otherwise
 */
int read_str_file(const char *path, char *str, uint32_t max_str_length);

/**
 * @brief Read integer from a device file.
 *
 * @param[in] path Path to the file (must not be null)
 * @param[out] value
 * @return 0 if successful, -1 otherwise
 */
int read_int_file(const char *path, uint32_t *value);

/**
 * @brief Export a pin.
 *
 * @param[in] dir_path Path to the directory containing export/unexport files.
 * @param[in] pin_no Index of the pin to export
 * @return 0 if successful, -1 otherwise
 */
int export_pin(const char *dir_path, uint32_t pin_no);

/**
 * @brief Unexport a pin.
 *
 * @param[in] dir_path Path to the directory containing export/unexport files.
 * @param[in] pin_no Index of the pin to unexport
 * @return 0 if successful, -1 otherwise
 */
int unexport_pin(const char *dir_path, uint32_t pin_no);

#endif
