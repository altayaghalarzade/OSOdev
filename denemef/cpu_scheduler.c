#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Define a structure for a process
typedef struct {
    char id[4];
    int arrival_time;
    int priority;
    int burst_time;
    int ram;
    int cpu;
} Process;

// Function to read processes from input file
int read_processes(const char *filename, Process **processes) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open input file");
        return -1;
    }

    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        count++;
    }

    *processes = (Process *)malloc(count * sizeof(Process));
    if (!*processes) {
        perror("Could not allocate memory for processes");
        fclose(file);
        return -1;
    }

    fseek(file, 0, SEEK_SET);
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%d,%d,%d,%d,%d",
               (*processes)[i].id,
               &(*processes)[i].arrival_time,
               &(*processes)[i].priority,
               &(*processes)[i].burst_time,
               &(*processes)[i].ram,
               &(*processes)[i].cpu);
        i++;
    }

    fclose(file);
    return count;
}

void write_output(FILE *file, const char *format, ...) {
    va_list args;
    va_start(args, format);

    // Write to the file
    vfprintf(file, format, args);

    // Write to the console
    vprintf(format, args);

    va_end(args);
}

void schedule_processes(Process *processes, int count) {
    FILE *output_file = fopen("output.txt", "w");
    if (!output_file) {
        perror("Could not open output file");
        return;
    }

    int current_time = 0;
    int total_ram = 8192; // Example total RAM available
    int used_ram = 0;

    // Example processing logic to demonstrate output
    for (int i = 0; i < count; i++) {
        if (used_ram + processes[i].ram > total_ram) {
            write_output(output_file, "Process %s cannot be assigned due to insufficient RAM.\n", processes[i].id);
            continue;
        }

        int burst_time = processes[i].burst_time;
        used_ram += processes[i].ram;

        if (processes[i].priority == 0) {
            write_output(output_file, "Process %s is queued to be assigned to CPU-1.\n", processes[i].id);
            write_output(output_file, "Process %s is assigned to CPU-1.\n", processes[i].id);
            write_output(output_file, "Process %s is completed and terminated.\n", processes[i].id);
        } else if (processes[i].priority == 1) {
            write_output(output_file, "Process %s is placed in the que1 queue to be assigned to CPU-2.\n", processes[i].id);
            write_output(output_file, "Process %s is assigned to CPU-2.\n", processes[i].id);
            write_output(output_file, "The operation of process %s is completed and terminated.\n", processes[i].id);
        } else if (processes[i].priority == 2) {
            write_output(output_file, "Process %s is placed in the que2 queue to be assigned to CPU-2.\n", processes[i].id);
            write_output(output_file, "Process %s is assigned to CPU-2.\n", processes[i].id);
            write_output(output_file, "Process %s ran for %d seconds, its operation is completed and terminated.\n", processes[i].id, burst_time);
        } else if (processes[i].priority == 3) {
            write_output(output_file, "Process %s is placed in the que3 queue to be assigned to CPU-2.\n", processes[i].id);
            write_output(output_file, "Process %s is assigned to CPU-2.\n", processes[i].id);
            write_output(output_file, "Process %s ran for %d seconds, its operation is completed and terminated.\n", processes[i].id, burst_time);
        }

        used_ram -= processes[i].ram;
    }

 




    write_output(output_file, "-----------------------------------------------------------------------------------\n");

    write_output(output_file, "CPU-1 queue (priority-0) (FCFS) -> ");
    for (int i = 0; i < count; i++) {
        if (processes[i].priority == 0) {
            fprintf(output_file, "%s ", processes[i].id);
            printf("%s ", processes[i].id);
        }
    }
    fprintf(output_file, "\n");
    printf("\n");

    write_output(output_file, "CPU-2 queue (priority-1) (SJF) -> ");
    for (int i = 0; i < count; i++) {
        if (processes[i].priority == 1) {
            fprintf(output_file, "%s ", processes[i].id);
            printf("%s ", processes[i].id);
        }
    }
    fprintf(output_file, "\n");
    printf("\n");

    write_output(output_file, "CPU-2 queue (priority-2) (RR-q8) -> ");
    for (int i = 0; i < count; i++) {
        if (processes[i].priority == 2) {
            fprintf(output_file, "%s ", processes[i].id);
            printf("%s ", processes[i].id);
        }
    }
    fprintf(output_file, "\n");
    printf("\n");

    write_output(output_file, "CPU-2 queue (priority-3) (RR-q16) -> ");
    for (int i = 0; i < count; i++) {
        if (processes[i].priority == 3) {
            fprintf(output_file, "%s ", processes[i].id);
            printf("%s ", processes[i].id);
        }
    }
    fprintf(output_file, "\n");
    printf("\n");

    fclose(output_file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s input.txt\n", argv[0]);
        return 1;
    }

    Process *processes;
    int count = read_processes(argv[1], &processes);
    if (count < 0) {
        return 1;
    }

    schedule_processes(processes, count);

    free(processes);
    return 0;
}