#include <fcntl.h>
#include <unistd.h>
#include "parse_options.h"
#include "utils.h"

void close_files(const std::vector<int> &fd_list) {
    for (const auto &v: fd_list)
        while (close(v) == -1) {
            if (errno == EINTR) {
                continue;
            } else {
                print_error(errno);
                break;
            }
        }
}

void open_files(const std::vector<std::string> &filenames, std::vector<int> &fd_list) {
    for (const auto &v: filenames) {
        bool proceed = true;
        while (proceed) {
            int fd = open(v.c_str(), O_RDONLY);
            if (fd == -1) {
                if (errno == EINTR) {
                    continue;
                } else {
                    print_error(errno);
                    close_files(fd_list);
                    exit(EXIT_FAILURE);
                }
            } else {
                fd_list.emplace_back(fd);
                proceed = false;
            }
        }
    }
}

void read_files(const std::vector<int> &fd_list, const bool &special_mode) {
    constexpr size_t buffer_capacity = 16384;
    constexpr size_t buffer_size = 4096;
    char buffer[buffer_size];
    for (const auto &v: fd_list) {
        ssize_t bytes_read = 666; // just random value to start the while loop
        while (bytes_read > 0) {
            bytes_read = read(v, buffer, buffer_size);
            if (bytes_read == -1) {
                if (errno == EINTR) {
                    continue;
                } else {
                    print_error(errno);
                    close_files(fd_list);
                    exit(EXIT_FAILURE);
                }
            } else {
                if (special_mode) {
                    for (size_t i = 0; i < bytes_read; ++i) {
                        if (!(isspace(buffer[i]) || isprint(buffer[i]))) {
                            insert(buffer, buffer_size, buffer_capacity, hex(buffer[i]), i);
                        }
                    }
                }
                write_to_stdout(buffer, bytes_read);
            }
        }
    }
}


int main(int argc, const char *argv[]) {
    auto options = parse_options(argc, argv);
    auto special_mode = options.first;
    auto filenames = options.second;
    if (filenames->empty()) return 0;
    std::vector<int> fd_list;
    open_files(*filenames, fd_list);
    read_files(fd_list, special_mode);
    close_files(fd_list);
    return 0;
}