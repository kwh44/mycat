//
// Created by kwh44 on 10/9/19.
//

#ifndef MYCAT_UTILS_H
#define MYCAT_UTILS_H

#include <string>
#include <cstring>

void print_error(int);

void write_to_stdout(char *buffer, size_t size) {
    ssize_t bytes_written = 0;
    while (bytes_written < size) {
        ssize_t written = write(1, buffer + bytes_written, size - bytes_written);
        if (written == -1) {
            if (errno == EINTR) {
                continue;
            } else {
                print_error(errno);
                exit(EXIT_FAILURE);
            }
        } else {
            bytes_written += written;
        }
    }
}

void print_error(int error_code) {
    char *message = strerror(error_code);
    write_to_stdout(message, strlen(message));
}

std::string hex(unsigned n) {
    std::string hex_code("    ");
    unsigned i = 0, temp = 0;
    while (n != 0) {
        temp = n % 16;
        if (temp < 10) hex_code[i] = temp + 48;
        else hex_code[i] = temp + 55;
        n /= 16;
        ++i;
    }
    hex_code[2] = 'x';
    hex_code[3] = '\\';
    return hex_code;
}

void
insert(char *buffer, const size_t &size, const size_t &capacity, std::string &&message_to_insert,
       const size_t &insert_index) {
    auto stride = message_to_insert.size() - 1;
    if (insert_index < size) for (auto i = size - 1; i > insert_index; --i) buffer[i + stride] = buffer[i];
    for (size_t i = 0; i <= stride; ++i) buffer[insert_index + i] = message_to_insert[stride - i];

}

#endif //MYCAT_UTILS_H
