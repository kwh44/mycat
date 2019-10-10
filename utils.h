//
// Created by kwh44 on 10/9/19.
//

#ifndef MYCAT_UTILS_H
#define MYCAT_UTILS_H

#include <string>
#include <cstring>


void print_error(int);

void write_to_stdout(char* buffer, size_t size) {
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

std::string hex(int n) {
    std::string hex_code{};
    hex_code.reserve(4);
    int i = 0, temp = 0;
    while (n != 0) {
        temp = n % 16;
        if (temp < 10) hex_code += char(temp + 48);
        else hex_code += char(temp + 55);
        n  /= 16;
        ++i;
    }
    hex_code += "x\\";
    return std::string(hex_code.rbegin(), hex_code.rend());
}

void
insert(char *buffer, const size_t &size, const size_t &capacity, std::string &&message_to_insert,
       const size_t &insert_index) {
    auto stride = message_to_insert.size();
    if (insert_index < size) for (auto i = size - 1; i <= size + stride; --i) buffer[i + stride] = buffer[i];
    for (size_t i = 0; i < stride; ++i) buffer[insert_index + i] = message_to_insert[i];
    buffer[size + stride] = '\0';
}

#endif //MYCAT_UTILS_H
