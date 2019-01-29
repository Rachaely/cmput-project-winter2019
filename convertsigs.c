#define _GNU_SOURCE
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_TEXT 20

int ascii, element_pos = 0, test = 0;
//ascii is the ascii number of the letter sent though signals, 8 bit
//element_pos points the 8 bit of binary ascii from left to right
void handler(int signal)
{
    
    test++;
    if (signal == SIGUSR1)
    {
        
        printf("1");
        ascii += 1<<(7-element_pos);
        
        
        
    }
    else if (signal == SIGUSR2)
    {
        printf("0");
        
        
    }
    if (++element_pos == 8){
            element_pos = 0;
        } 
    
    printf("pointer is %d, ascii is %d\n",element_pos, ascii);
    if (element_pos == 0)
    {   
        
        putchar((char)ascii);
        ascii = 0;
        element_pos = 0;
        
    }


}

int main(int argc, char const *argv[])
{
    printf("Own PID: %d\n", getpid());
    // print pid of this process

    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_flags = SA_NODEFER;
    //copy form lab

    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    pid_t pid;
    scanf("%d", &pid);
    //get the pid want to commnicate with

    while (1)
    {
        ascii = 0;
        
        char sent_message[MAX_TEXT];
        
        scanf("%s", sent_message);
        
        //check if user enter a word
        if (sent_message[0] >= 33 && sent_message[0] <= 126)
        {   
            for (int i = 0; i < strlen(sent_message)+1; i++)
            
            //for each letter in the message
            {
                
                for (int j = 7; j >= 0; --j)
                //encode letter into binary, order of bit is from left to right
                {
                    if (sent_message[i] & (1 << j))
                    {
                        printf("1");
                        kill(pid, SIGUSR1);
                    }
                    else
                    {
                        printf("0");
                        kill(pid, SIGUSR2);
                    }
                    usleep(300);

                    //putchar((sent_message[i]&(1<<j))?'1':'0');
                }
            }
        }
        
        
    }

    return 0;
}
