#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <cstdint>
#include "messageDispatcher.hpp"
#include <signal.h>
#include <time.h>

// Global variables to track timing and messages
static volatile sig_atomic_t running = 1;
static size_t g_message_count = 0;
static struct timespec g_start;

void signal_handler(int signum) {
    if (signum == SIGINT) {
        running = 0;
    }
}

int main(int argc, char* argv[]) {
    // Add signal handler setup
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, nullptr);

    const char* filename = argc > 1 ? argv[1] : "itch_feed.bin";
    const char* log_filename = argc > 2 ? argv[2] : "itch_feed_log.csv";

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return 1;
    }

    int log_fd = open(log_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
    clock_gettime(CLOCK_MONOTONIC, &g_start);
    
    while (running && offset + 2 < file_size) {
        // Read the 2-byte length prefix
        uint16_t length = (static_cast<uint16_t>(mem[offset]) << 8) | static_cast<uint16_t>(mem[offset + 1]);
        
        // Skip the length bytes
        offset += 2;
        
        if (offset + length > file_size) {
            std::cerr << "Message extends beyond file end at offset " << offset << "\n";
            break;
        }

        char type = mem[offset];

        __builtin_prefetch(mem + offset + 64);  // Optional prefetch for performance
        itch::dispatch(type, mem + offset, length, log_fd);  // Dispatch to handler with CSV logging
        offset += length;
        g_message_count++;
    }

    // Calculate and display stats regardless of how we exit
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed_seconds = (end.tv_sec - g_start.tv_sec) + 
                           (end.tv_nsec - g_start.tv_nsec) / 1e9;
    
    std::cout << "\nProcessed " << g_message_count << " messages in " 
              << elapsed_seconds << " seconds\n";
    std::cout << "Average speed: " << (g_message_count / elapsed_seconds) 
              << " messages/second\n";
    std::cout << "Average time per message: " << (elapsed_seconds * 1e6 / g_message_count) 
              << " microseconds\n";

    munmap(const_cast<char*>(mem), file_size);
    close(fd);
    close(log_fd);
}
