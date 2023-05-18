#include "cpu_util.h"

#include "test_util.h"

#include <stdio.h>
#include <stdint.h>
        

void run_tests()
{
    int pass = 0;
    int fail = 0;
    uint8_t flags = 0xa0;
    // Tests for ALU_NOP
    uint16_t result;
    result = alu(ALU_NOP, 17, 6, &flags);
    if (test(0, result, 0xa0, flags, 1, "Testing NOP"))
        pass++;
    else
        fail++;
        
    // Tests for ALU_PASS
    result = alu(ALU_PASS, 17, 0, &flags);
    if (test(17, result, 0xa0, flags, 2, "Testing PASS"))
        pass++;
    else
        fail++;
        
    // Tests for ALU_ADD
    result = alu(ALU_ADD, 33, 50, &flags);
    if (test(83, result, 0x00, flags, 3, "Testing ADD"))
        pass++;
    else
        fail++;
        
    result = alu(ALU_ADD, 200, 56, &flags);
    if (test(0, result, 0xb0, flags, 4, "Testing ADD"))
        pass++;
    else
        fail++;
        
    // Tests for ALU_ADD16
    
        
    printf("PASSED: %d; FAILED: %d\n", pass, fail);
}

int main()
{
    printf("Testing ALU...\n");
    run_tests();
    return 0;
}
