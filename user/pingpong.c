#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(){
    int p1[2], p2[2];

    /*p1 for parent-to-child communication
      p2 for child-to-parent communication*/

    pipe(p1);
    pipe(p2);

    if (fork() != 0){
        // Parent Process

        //A char is sent from parent to child process

        write(p1[1], ".", 1);
        close(p1[1]);

        char ch;

        /*Create an character ch and
        read p2[0] and pass it to ch*/

        read(p2[0], &ch, 1);
        printf("%d: received pong\n", getpid());

        wait(0);
    }

    else{
        //Child Process

        char ch;

        read(p1[0], &ch, 1);
        printf("%d: received ping\n", getpid());

        write(p2[1], &ch, 1);
        close(p2[1]);
    }

    exit(0);

    return 0;
}