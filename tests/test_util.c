#include "test_util.h"

#include <stdio.h>

bool test(int exp, int act, int flag_exp, int flag_act, int n, char *what)
{
    bool pass = true;
    printf("Test #%d. Testing %s\n", n, what);
    if (exp == act) {
        printf("Ret val correct\n");
    } else {
        printf("**FAIL... Exp: %d; Actual: %d\n", exp, act);
        pass = false;
    }
    
    if (flag_exp == flag_act) {
        printf("Flags correct\n");
    } else {
        printf("**FAIL... Exp: %x; Actual %x\n", flag_exp, flag_act);
        pass = false;
    }
    
    return pass;
}
