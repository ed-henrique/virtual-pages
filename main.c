#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    // Open the file
    int fd = open("./big-file.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Get the file size
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("Error getting file size");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Get the system page size
    long page_size = sysconf(_SC_PAGESIZE);
    if (page_size == -1) {
        perror("Error getting page size");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Map the file to memory
    void *mapped = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Access the file data through the memory map
    printf("File contents:\n%.*s\n", (int)sb.st_size, (char *)mapped);

    // Calculate the number of pages used
    long num_pages = (sb.st_size + page_size - 1) / page_size;  // Round up
    printf("File size: %lld bytes\n", (long long)sb.st_size);
    printf("Page size: %ld bytes\n", page_size);
    printf("Number of pages used: %ld\n", num_pages);

    // Unmap the file
    if (munmap(mapped, sb.st_size) == -1) {
        perror("Error unmapping file");
    }

    // Close the file descriptor
    close(fd);

    return 0;
}
