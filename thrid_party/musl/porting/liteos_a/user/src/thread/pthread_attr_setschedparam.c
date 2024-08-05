#include "pthread_impl.h"

int pthread_attr_setschedparam(pthread_attr_t *restrict a, const struct sched_param *restrict param)
{
    if ((a->_a_policy == SCHED_RR || a->_a_policy == SCHED_FIFO) &&
        (param->sched_priority < 0 || param->sched_priority > PTHREAD_PRIORITY_LOWEST)) {
        return EINVAL;
    }
    a->_a_prio = param->sched_priority;
    a->_a_deadline = param->sched_deadline;
    a->_a_period = param->sched_period;
    return 0;
}
