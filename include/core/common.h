#ifndef __LETMECREATE_CORE_COMMON_H__
#define __LETMECREATE_CORE_COMMON_H__

#include <stdint.h>

#define MAX_STR_LENGTH          (255)

int write_str_file(const char *path, const char *str);
int write_int_file(const char *path, const uint32_t value);
int read_str_file(const char *path, char *str, const uint32_t max_str_length);
int read_int_file(const char *path, uint32_t *value);
int export_pin(const char *dir_path, const uint32_t pin_no);
int unexport_pin(const char *dir_path, const uint32_t pin_no);

#endif
