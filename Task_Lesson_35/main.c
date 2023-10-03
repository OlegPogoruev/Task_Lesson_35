
#include "main.h"

#include <stdlib.h>
#include <sys/wait.h>


#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


#include <time.h>

#define BUFLEN 32	//Max length of buffer

///* When a SIGUSR1 signal arrives, printf time. */
void come_signal (int sig)
{
    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);
//    printf("Date: %d.%d.%d\n", now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
    printf("Time: %d:%d:%d\n", now->tm_hour, now->tm_min, now->tm_sec);
}

/* The child process executes this function. */
void child_function (void)
{
    int sig;
    
    struct sigaction usr_action;
    sigset_t block_mask;
    
    /* Establish the signal handler. */
    sigemptyset(&block_mask);	         // reset mask
    sigaddset(&block_mask, (~(SIGUSR1)));  // add signal

    usr_action.sa_handler = come_signal;
    usr_action.sa_mask = block_mask;
    usr_action.sa_flags = 0;
    /* Perform initialization. */

    sigaction (SIGUSR1, &usr_action, NULL);
    sigaction (SIGUSR2, &usr_action, NULL);
    printf ("proccess init %d.\n", (int) getpid ());  /* Perform initialization. */
    /* Busy wait signal.   */
    sigemptyset(&block_mask);	         // reset mask
    sigaddset(&block_mask, SIGUSR2);  // add signal
    sigwait(&block_mask, &sig);	//отправляем маску
    printf("Child End\n");
    exit(EXIT_SUCCESS);
}


void get_input_string(char *str_in);

int main(void)
{
    
    
    pid_t child_id;
    
    /* Create the child process. */
    child_id = fork ();
    if (child_id == 0)
    {
        child_function ();          /* Does not return. */
    }
    
    printf("time - printf local time\n");
    printf("exit - exit program\n");
    char buf[BUFLEN];
    sleep(2);
    
    
    while(1)
    {
        get_input_string(buf);
        
        if (strcmp (buf, "time")==0)
        {kill(child_id, SIGUSR1);}   /* send signal */
        else
            if (strcmp (buf, "exit")==0)
            {
//                kill(child_id, SIGUSR1);   /* send signal */
                kill(child_id, SIGUSR2);   /* send signal */
//                sleep(2);
                wait(NULL);
                break;
            }
            else{printf("Error input\n");}
    }
    
    
    
    printf("The End\n");
    return 0;
}



void get_input_string(char *str_in)
{
    int ch = 0;
    int temp_count = 0;
    while ((ch = getchar()) != '\n')
    {
        (*(str_in + temp_count)) = ch;
        ++temp_count;
        if(temp_count > (BUFLEN - 2 ))
        {
//          (*(str_in + temp_count)) = '\n';
//          ++temp_count;
          break;
        }
    }
//    (*(str_in + temp_count)) = '\n';
    (*(str_in + temp_count)) = 0;    
}








