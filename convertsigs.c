#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_TEXT 20

int ascii, element_pointer;

void handler(int signal)
{

    if (signal == SIGUSR1)
    {
        printf("1");
        ascii += 1 << (7 - element_pointer);

        element_pointer++;
    }
    else if (signal == SIGUSR2)
    {
        printf("0");
        element_pointer++;
    }
    if (element_pointer == 8)
    {
        printf(" final is %c\n", ascii);
        ascii = 0;
        element_pointer = 0;
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
        element_pointer = 0;
        char sent_message[MAX_TEXT];
        
        fgets(sent_message, MAX_TEXT, stdin);

        //check if user enter a word
        if (sent_message[0] >= 33 && sent_message[0] <= 126)
        {
            for (int i = 0; i < strlen(sent_message) + 1; i++)
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

                    //putchar((sent_message[i]&(1<<j))?'1':'0');
                }
            }
        }
    }

    return 0;
}