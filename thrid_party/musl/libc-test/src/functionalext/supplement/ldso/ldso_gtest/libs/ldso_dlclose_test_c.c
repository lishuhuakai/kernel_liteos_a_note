#include <stdio.h>

int dlclose_test_c_var_c = 2;
extern int dlclose_test_b_var_b;

void dlclose_test_c(void)
{
    printf ("calling dlclose_test_c, %d\n", dlclose_test_b_var_b);
}