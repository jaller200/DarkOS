#ifndef __CPU_H_INCLUDED__
#define __CPU_H_INCLUDED__

int initialize_cpu();   // Initialize the processors
void shutdown_cpu();    // Shutdown the processors

char * cpu_get_vendor();

#endif