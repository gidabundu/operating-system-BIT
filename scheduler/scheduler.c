/* scheduler.c — FCFS + Round-Robin process scheduler | SLeOS */
#include "../include/scheduler.h"
#include "../include/kernel.h"
#include "../include/vga.h"
#include "../include/types.h"

static pcb_t    proc_table[MAX_PROCESSES];
static uint32_t next_pid   = 1;
static uint32_t active_cnt = 0;
static uint32_t current_pid = 0;

static const char *state_names[] = { "RUN ", "WAIT", "SLEP", "TERM" };
static const char *prio_names[]  = { "HIGH", "MED ", "LOW " };
static const char *algo_names[]  = { "FCFS", "RR  " };

void sched_init(void) {
    kmemset(proc_table, 0, sizeof(proc_table));
    next_pid   = 1;
    active_cnt = 0;
}

int sched_create(const char *name, uint32_t burst, uint32_t priority, sched_algo_t algo) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (!proc_table[i].active) {
            pcb_t *p = &proc_table[i];
            kmemset(p, 0, sizeof(pcb_t));
            p->pid            = next_pid++;
            p->state          = PROC_RUNNING;
            p->algo           = algo;
            p->priority       = priority;
            p->burst_total    = burst;
            p->burst_remain   = burst;
            p->quantum_remain = QUANTUM_MS;
            p->active         = 1;
            /* copy name safely */
            int j;
            for (j = 0; j < MAX_NAME_LEN-1 && name[j]; j++)
                p->name[j] = name[j];
            p->name[j] = 0;
            active_cnt++;
            return (int)p->pid;
        }
    }
    return -1;  /* table full */
}

/* sched_tick — simulate one scheduling tick (called by shell on each ps) */
void sched_tick(void) {
    /* Simple simulation: rotate states and consume burst */
    static uint32_t tick = 0;
    tick++;
    for (int i = 0; i < MAX_PROCESSES; i++) {
        pcb_t *p = &proc_table[i];
        if (!p->active) continue;
        if (p->algo == SCHED_ROUNDROBIN) {
            /* Rotate state every few ticks for visual interest */
            p->state = (proc_state_t)((tick + i) % 3);
            if (p->burst_remain > 0)
                p->burst_remain -= (p->burst_remain > 5 ? 5 : p->burst_remain);
        } else {
            /* FCFS stays running until burst done */
            p->state = PROC_RUNNING;
        }
        p->cpu_time += 5;
    }
    current_pid = 1;
}

void sched_list(void) {
    /* Print process table in a formatted way */
    vga_set_color(VGA_CYAN, VGA_BLACK);
    kernel_printf("  PID  NAME            STATE  ALGO  PRI   BURST  CPU_MS\n");
    vga_set_color(VGA_DARK_GREY, VGA_BLACK);
    kernel_printf("  ---  --------------  -----  ----  ----  -----  ------\n");

    uint32_t cpu_total = 0;
    for (int i = 0; i < MAX_PROCESSES; i++)
        if (proc_table[i].active) cpu_total += proc_table[i].cpu_time;
    if (cpu_total == 0) cpu_total = 1;

    for (int i = 0; i < MAX_PROCESSES; i++) {
        pcb_t *p = &proc_table[i];
        if (!p->active) continue;

        /* colour-code by state */
        switch (p->state) {
            case PROC_RUNNING:    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK); break;
            case PROC_WAITING:    vga_set_color(VGA_LIGHT_BROWN, VGA_BLACK); break;
            case PROC_SLEEPING:   vga_set_color(VGA_LIGHT_BLUE,  VGA_BLACK); break;
            default:              vga_set_color(VGA_DARK_GREY,   VGA_BLACK); break;
        }

        uint32_t cpu_pct = (p->cpu_time * 100) / cpu_total;
        kernel_printf("  %u    %-14s  %s  %s  %s  %u ms   %u%%\n",
            p->pid,
            p->name,
            state_names[p->state],
            algo_names[p->algo],
            prio_names[p->priority],
            p->burst_remain,
            cpu_pct);
    }
    vga_set_color(VGA_DARK_GREY, VGA_BLACK);
    kernel_printf("  ---  --------------  -----  ----  ----  -----  ------\n");
    vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
    kernel_printf("  Total processes: %u  |  Scheduler: FCFS + Round-Robin (Q=%ums)\n",
                  active_cnt, (uint32_t)QUANTUM_MS);
}

pcb_t *sched_current(void) {
    for (int i = 0; i < MAX_PROCESSES; i++)
        if (proc_table[i].active && proc_table[i].pid == current_pid)
            return &proc_table[i];
    return &proc_table[0];
}

uint32_t sched_count(void) { return active_cnt; }
