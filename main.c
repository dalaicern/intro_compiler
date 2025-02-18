#include <stdio.h>
#include "util.h"
#include "prog1.h"


int maxargs(A_stm stm){
    if(stm->kind == A_printStm){
        return 1;
    }

    
}
int main() {
    A_stm program = prog();

    return 0;
}
