#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

int main(){

    int p1[2], p2[2];

    /*p1 for parent-to-child communication
      p2 for child-to-parent communication*/

    pipe(p1);
    pipe(p2);

    if(fork() != 0){
        //Parent process

        close(p1[0]);
        close(p2[1]);

        //In parent process, p1[1] and p2[0] is running
        
        char ping[1] = "B";

        clock_t start, end;
        double exchange;

        start = clock();

        //Assume that running time here is 100000

        for(int i = 0; i < 100000; ++i){
            write(p1[1], ping, 1);
            read(p2[0], ping, 1);
        }

        end = clock();

        wait(0);

        close(p1[1]);
        close(p2[0]);

        exchange = ((double)(end - start) / CLOCKS_PER_SEC) / 100000;

        printf("average exchange time: %f s\n", exchange * 10);
        printf("exchanges per second: %ld times\n", (unsigned long)(100000 / (exchange * 10)));
    }

    else{
        //Child Process

        close(p1[1]);
        close(p2[0]);

        char pong[1];

        for(int i = 0; i < 100000; ++i){
            read(p1[0], pong, 1);
            write(p2[1], pong, 1);
        }

        close(p1[0]);
        close(p2[1]);
        exit(0);
    }

    return 0;
}
