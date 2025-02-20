#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prog1.h"

#define max(a, b) a > b ? a : b
#define min(a, b) b < a ? b : a


typedef struct table *Table_;
struct table {string id; int value; Table_ tail;};
typedef struct  {int i; Table_ t;} IntAndTable;

Table_ interpStm(A_stm, Table_);
IntAndTable interpExp(A_exp, Table_);
int operate(Table_, A_exp);
int lookup(Table_, string);

Table_ Table(string id, int value, struct table *tail) {
    Table_ t = checked_malloc(sizeof(*t)); 
    t->id=id; 
    t->value=value; 
    t->tail=tail; 
    return t;
}

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

int operate(Table_ t, A_exp exp){
    if((exp -> u.op.left->kind == A_numExp && exp -> u.op.right->kind == A_idExp)
            || (exp -> u.op.left->kind == A_idExp && exp -> u.op.right->kind == A_numExp)
            || (exp -> u.op.left->kind == A_numExp && exp -> u.op.right->kind == A_numExp)){
        int val1 = exp -> u.op.left->kind == A_numExp ? exp -> u.op.left->u.num : lookup(t, exp -> u.op.left->u.id);
        int val2 = exp -> u.op.right->kind == A_numExp ? exp -> u.op.right->u.num : lookup(t, exp -> u.op.right->u.id);
        
        // if(exp -> u.op.left->kind == A_idExp) 
        //     printf("id1: %s  ", exp -> u.op.left->u.id);
        // if(exp -> u.op.right->kind == A_idExp) 
        //     printf("id2: %s   ", exp -> u.op.right->u.id);

        // printf("operation done. %d %d\n", val1, val2);

        switch (exp -> u.op.oper) {
            case A_plus:
                return val1 + val2;
            case A_minus:
                return val1 - val2;
            case A_times:
                return val1 * val2;
            default:
                if(!val2) {
                    printf("ERROR: cannot divide by 0 !!!!!");
                    exit(1);
                }
                else return val1 / val2;
        }

    }

    printf("Syntax error");
    return -1;
}

int lookup(Table_ t, string key){
    while(t){
        if(strcmp(t->id , key) == 0){
            return t -> value;
        }
        t = t -> tail;
    }
    return -1;
}

void printTable(Table_ t){
    while(t){
        printf("%s %d | ", t -> id, t->value);
        t = t->tail;
    }
}


Table_ interpStm(A_stm stm, Table_ t){
    if(!stm) {
        fprintf(stderr, "Error: Null statement encountered.\n");
        return t;
    }
    
    if(stm -> kind == A_assignStm){
        // printf("1");
        IntAndTable assignee = interpExp(stm -> u.assign.exp, t);
        t = Table(stm -> u.assign.id, assignee.i, assignee.t);
        // printTable(t);

        return t;
    } else if(stm -> kind == A_compoundStm){
        // printf("2");
        t = interpStm(stm -> u.compound.stm1, t);
        t = interpStm(stm -> u.compound.stm2, t);
    } else if(stm -> kind == A_printStm) {
        // printf("3");
        A_expList explist = stm -> u.print.exps;
        while (explist) {
            IntAndTable exp_result = interpExp(explist->u.pair.head, t);
            t = exp_result.t;
            printf("%d ", exp_result.i);
            if (explist->kind == A_lastExpList) 
                break;
            explist = explist->u.pair.tail;
        }
    } else {
        fprintf(stderr, "Error: Unknown statement kind encountered.\n");
    }
    return t;
}

IntAndTable interpExp(A_exp exp, Table_ t){
    // printTable(t);
    // printf("\n");
    int res;

    if (!exp) {
        fprintf(stderr, "Error: Null expression encountered.\n");
        IntAndTable a = {-1, t};
        return a;
    }

    switch(exp -> kind){
        case A_numExp:
            res = exp -> u.num;
            break;
        case A_idExp:
            res = lookup(t, exp -> u.id);
            if (res == -1) {
                fprintf(stderr, "Error: Undefined identifier '%s'.\n", exp -> u.id);
            }
            break;
        case A_opExp:
            res = operate(t, exp);
            if (res == -1) {
                fprintf(stderr, "Error: Operation failed.\n");
            }
            break;
        case A_eseqExp:
            t = interpStm(exp -> u.eseq.stm, t);
            IntAndTable exp_result = interpExp(exp -> u.eseq.exp, t);
            res = exp_result.i;
            t = exp_result.t;
            break;
        default:
            fprintf(stderr, "Error: Unknown expression kind encountered.\n");
            res = -1;
            break;
    }
    IntAndTable a = {res, t};
    return a;
}

void interp(A_stm stm){
    Table_ t = NULL;
    interpStm(stm, t);
}

int main() {
    // printf("max args: %d\n", maxargs(prog()));
    interp(prog());

    return 0;
}
