#include <stdio.h>
#include <string.h>
#include "util.h"
#include "prog1.h"

#define max(a, b) a > b ? a : b
#define min(a, b) b < a ? b : a


typedef struct table *Table_;
struct table {string id; int value; Table_ tail;};
typedef struct  {int i; Table_ t;} IntAndTable;

Table_ Table(string id, int value, struct table *tail) {
    Table_ t = malloc(sizeof(*t)); 
    t->id=id; 
    t->value=value; 
    t->tail=tail; 
    return t;
}

// Table_ interpStm(A_stm s, Table_ t){

// }

// IntAndTable interpExp(A_exp e, Table_ t){

// }

int counter(A_expList explist) {
    int count = 0;
    while (explist) {
        count++;
        if (explist->kind == A_lastExpList) 
            break;
        explist = explist->u.pair.tail;
    }
    return count;
}


int maxargs(A_stm stm){
    if(!stm) return 0;
    
    if(stm -> kind == A_assignStm && stm -> u.assign.exp -> kind == A_eseqExp){
        return maxargs(stm -> u.assign.exp -> u.eseq.stm);
    } else if(stm -> kind == A_compoundStm){
        int res1 = maxargs(stm -> u.compound.stm1);
        int res2 = maxargs(stm -> u.compound.stm2);
        return max(res1, res2);
    } else if(stm -> kind == A_printStm) {
        return counter(stm -> u.print.exps);
    }
    return 0;
    
}

Table_ update(Table_ t, string key, int val){
    return Table(key, val, t);
}

void printer(A_expList expList){

}

int lookup(Table_ t, string key){
    while(t){
        if(strcmp(t->id , key) != 0){
            return t -> value;
        }
        t = t -> tail;
    }
    return -1;
}

void interp(A_stm stm){
    if(!stm) return;
    
    if(stm -> kind == A_assignStm && stm -> u.assign.exp -> kind == A_eseqExp){
        return interp(stm -> u.assign.exp -> u.eseq.stm);
    } else if(stm -> kind == A_compoundStm){
        interp(stm -> u.compound.stm1);
        interp(stm -> u.compound.stm2);
    } else if(stm -> kind == A_printStm) {
        return printer(stm -> u.print.exps);
    }
    
}

int main() {
    printf("%d", maxargs(prog()));

    return 0;
}
