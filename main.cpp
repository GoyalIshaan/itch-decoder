#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <cstdint>
#include <cstring>
#include "messageLengths.hpp"
#include <fstream>
#include "messageDispatcher.hpp"

int main(int argc, char* argv[]) {
    const char* filename = argc > 1 ? argv[1] : "itch_feed.bin";

    const char* log_filename = argc > 2 ? argv[2] : "itch_feed_log.csv";

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return 1;
    }

    int log_fd = open(log_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd == -1) {
        perror("Failed to open log file");
        close(fd);
        return 1;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("Failed to get file size");
        close(fd);
        return 1;
    }
    
    size_t file_size = st.st_size;

    const char* mem = static_cast<const char*>(mmap(nullptr, file_size, PROT_READ, MAP_SHARED, fd, 0));


    if (mem == MAP_FAILED) {
        perror("Failed to mmap file");
        close(fd);
        return 1;
    }        

    size_t offset = 0;
    while (offset < file_size) {
        char type = mem[offset];
        size_t len = itch::getMessageLength(type);

        if (len == 0 || offset + len > file_size) {
            std::cerr << "Corrupt message at offset " << offset << ", type=" << type << "\n";
            break;
        }

        __builtin_prefetch(mem + offset + 64);  // Optional prefetch for performance

        itch::dispatch(type, mem + offset, len, log_fd);  // Dispatch to handler with CSV logging

        offset += len;
    }

    munmap(const_cast<char*>(mem), file_size);
    close(fd);
}
