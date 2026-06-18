/* scheduler.h — process scheduler interface */
#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "types.h"

#define MAX_PROCESSES   16
#define QUANTUM_MS      20
#define MAX_NAME_LEN    16

typedef enum {
    PROC_RUNNING = 0,
    PROC_WAITING,
    PROC_SLEEPING,
    PROC_TERMINATED
} proc_state_t;

typedef enum {
    SCHED_FCFS = 0,
    SCHED_ROUNDROBIN
} sched_algo_t;

typedef struct {
    uint32_t    pid;
    char        name[MAX_NAME_LEN];
    proc_state_t state;
    sched_algo_t algo;
    uint32_t    priority;       /* 0=HIGH 1=MED 2=LOW */
    uint32_t    burst_total;    /* total CPU burst (ms) */
    uint32_t    burst_remain;   /* remaining burst */
    uint32_t    quantum_remain; /* RR quantum countdown */
    uint32_t    cpu_time;       /* total CPU time used */
    uint32_t    wait_time;      /* total wait time */
    uint8_t     active;
} pcb_t;

void    sched_init(void);
int     sched_create(const char *name, uint32_t burst, uint32_t priority, sched_algo_t algo);
void    sched_tick(void);
void    sched_list(void);
pcb_t  *sched_current(void);
uint32_t sched_count(void);

#endif
