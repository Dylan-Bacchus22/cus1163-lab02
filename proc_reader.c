#include "proc_reader.h"

int list_process_directories(void) {
    DIR *dir = opendir("/proc");
      if (dir == NULL) {
        perror("opendir failed");
        return -1;
      }
    struct dirent *entry;
    int process_count = 0;
    
    printf("Process directories in /proc:\n");
    printf("%-8s %-20s\n", "PID", "Type");
    printf("%-8s %-20s\n", "---", "----");

    
    while ((entry = readdir(dir)) != NULL) {
        if(is_number(entry -> d_name))
        {
            printf("PID: %s\n", entry->d_name);
            process_count++;
        }
    }
    if (closedir(dir) == -1) {
    perror("closedir failed");
    return -1;
}

    // TODO: Print the total count of process directories found
    printf("Found %d process directories\n", process_count);
    return 0; // Replace with proper error handling
}

int read_process_info(const char* pid) {
    char filepath[256];

  
    snprintf(filepath, sizeof(filepath), "/proc/%s/status", pid);
    printf("\n--- Process Information for PID %s ---\n", pid);

    
     if (read_file_with_syscalls(filepath) == -1) {
        fprintf(stderr, "Failed to read %s\n", filepath);
        return -1;
    }
    
    snprintf(filepath, sizeof(filepath), "/proc/%s/cmdline", pid);
    printf("\n--- Command Line ---\n");


    if (read_file_with_syscalls(filepath) == -1) {
        fprintf(stderr, "Failed to read %s\n", filepath);
        return -1;
    }
    
    printf("\n"); // Add extra newline for readability
    
    return 0; // Replace with proper error handling
}

int show_system_info(void) {
    int line_count = 0;
    const int MAX_LINES = 10;

    printf("\n--- CPU Information (first %d lines) ---\n", MAX_LINES);

    FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
    if(cpuinfo == NULL) {
        perror("fopen /proc/cpuinfo failed");
        return -1;
    }
    char line[256];
    while(fgets(line, sizeof(line), file) != NULL && line_count < MAX_LINES){
        printf("%s", line);
        line_count++;
    }
    if (fclose(cpuinfo) != 0) {
        perror("fclose /proc/cpuinfo failed");
        return -1;
    }
    printf("\n--- Memory Information (first %d lines) ---\n", MAX_LINES);


    FILE *meminfo = fopen("/proc/meminfo", "r");
    if(meminfo == NULL)
    {
        perror("fopen /proc/meminfo failed");
        return -1;
    }
    while (fgets(line, sizeof(line), meminfo) != NULL &&
           line_count < MAX_LINES) {
        printf("%s", line);
        line_count++;
    }
    if(fclose(meminfo) != 0) 
    {
        perror("fclose /proc/meminfo failed");
        return -1;
    }
    return 0; // Replace with proper error handling
}

void compare_file_methods(void) {
    const char* test_file = "/proc/version";

    printf("Comparing file reading methods for: %s\n\n", test_file);

    printf("=== Method 1: Using System Calls ===\n");
    read_file_with_syscalls(test_file);

    printf("\n=== Method 2: Using Library Functions ===\n");
    read_file_with_library(test_file);

    printf("\nNOTE: Run this program with strace to see the difference!\n");
    printf("Example: strace -e trace=openat,read,write,close ./lab2\n");
}

int read_file_with_syscalls(const char* filename) {
    int fd;
    char buffer[256];
    ssize_t bytes_read;
    
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open failed");
        return -1;
    }

    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        printf("%s", buffer);
    }

    if (bytes_read == -1) {
        perror("read failed");
        close(fd); 
        return -1;
    }

    if (close(fd) == -1) {
        perror("close failed");
        return -1;
    }

    return 0; // Replace with proper error handling
}

int read_file_with_library(const char* filename) {
     FILE *file;
    char buffer[256];

    file = fopen(file, "r");
    if (file == NULL) {
        perror("fopen failed");
        return -1;
    }
    
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    if (fclose(file) != 0) {
        perror("fclose failed");
        return -1;
    }

    return 0; // Replace with proper error handling
}

int is_number(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }
    while (*str) {
        if (!isdigit((unsigned char)*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}
