#include <stdio.h>

extern int dlclose_test_b_var_b;
extern int dlclose_test_c_var_c;

void dlclose_test_a(void)
{
    printf ("calling dlclose_test_a, %d, %d\n", dlclose_test_b_var_b, dlclose_test_c_var_c);
}