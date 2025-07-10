#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
    printf("The amount of free memory available is: %d\n", kfreemem());
    return 0;
}