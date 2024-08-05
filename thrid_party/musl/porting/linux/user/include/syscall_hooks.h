#ifndef SYSCALL_HOOKS_H
#define SYSCALL_HOOKS_H

int set_syscall_hooks(const char *hooks_table, int table_len, void *hooks_entry, int reset, int **tid_addr);

#endif