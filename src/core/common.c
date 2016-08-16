#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "letmecreate/core/common.h"


int write_str_file(const char *path, const char *str)
{
    int fd = -1;

    if (path == NULL) {
        fprintf(stderr, "Cannot open file using null path.\n");
        return -1;
    }

    if (str == NULL) {
        fprintf(stderr, "Cannot write null string to file %s.\n", path);
        return -1;
    }

    if ((fd = open(path, O_WRONLY)) < 0) {
        fprintf(stderr, "Failed to open file %s\n", path);
        return -1;
    }

    if (write(fd, str, strlen(str)+1) < 0) {
        fprintf(stderr, "Failed to write to file %s\n", path);
        close(fd);
        return -1;
    }

    close(fd);

    return 0;
}

int write_int_file(const char *path, uint32_t value)
{
    char str[MAX_STR_LENGTH];

    if (snprintf(str, MAX_STR_LENGTH, "%d", value) < 0) {
        fprintf(stderr, "Failed to convert integer %d to string.\n", value);
        return -1;
    }

    return write_str_file(path, str);
}

int read_str_file(const char *path, char *str, uint32_t max_str_length)
{
    int fd = -1;

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(stderr, "Failed to open file %s\n", path);
        return -1;
    }

    if (read(fd, str, max_str_length) < 0) {
        fprintf(stderr, "Failed to read from file %s\n", path);
        close(fd);
        return -1;
    }

    close(fd);

    return 0;
}

int read_int_file(const char *path, uint32_t *value)
{
    char str[MAX_STR_LENGTH];

    if (read_str_file(path, str, MAX_STR_LENGTH) < 0)
        return -1;

    errno = 0;
    *value = strtoul(str, NULL, 10);
    if (errno != 0) {
        fprintf(stderr, "Failed to convert string %s to integer", str);
        return -1;
    }

    return 0;
}

int export_pin(const char *dir_path, uint32_t pin_no)
{
    int fd = -1;
    char path[MAX_STR_LENGTH];
    char str[MAX_STR_LENGTH];

    if (snprintf(path, MAX_STR_LENGTH, "%s/export", dir_path) < 0) {
        fprintf(stderr, "Failed to create path while trying to export pin no %d", pin_no);
        return -1;
    }

    if (snprintf(str, MAX_STR_LENGTH, "%d", pin_no) < 0) {
        fprintf(stderr, "Failed to convert integer %d to string.\n", pin_no);
        return -1;
    }

    if ((fd = open(path, O_WRONLY)) < 0) {
        fprintf(stderr, "Failed to open file %s\n", path);
        return -1;
    }

    if (write(fd, str, strlen(str)+1) < 0) {
        fprintf(stderr, "Failed to write %s to file %s.\n", str, path);
        close(fd);
        return -1;
    }

    close(fd);

    return 0;
}

int unexport_pin(const char *dir_path, uint32_t pin_no)
{
    int fd = -1;
    char path[MAX_STR_LENGTH];
    char str[MAX_STR_LENGTH];

    if (snprintf(path, MAX_STR_LENGTH, "%s/unexport", dir_path) < 0) {
        fprintf(stderr, "Failed to create path while trying to export pin no %d", pin_no);
        return -1;
    }

    if (snprintf(str, MAX_STR_LENGTH, "%d", pin_no) < 0) {
        fprintf(stderr, "Failed to convert integer %d to string.\n", pin_no);
        return -1;
    }

    if ((fd = open(path, O_WRONLY)) < 0) {
        fprintf(stderr, "Failed to open file %s\n", path);
        return -1;
    }

    if (write(fd, str, strlen(str)+1) < 0) {
        fprintf(stderr, "Failed to write %s to file %s.\n", str, path);
        close(fd);
        return -1;
    }

    close(fd);

    return 0;
}
