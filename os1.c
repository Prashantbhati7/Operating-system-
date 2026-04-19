// #include <stdio.h>
// #include <unistd.h>
// int main()
// {
//     pid_t pid;
//     pid = fork(); // create a child process
//     if (pid < 0)
//     {
//         printf("Fork failed\n");
//     }
//     else if (pid == 0)
//     {
//         // Child process
//         printf("Child Process\n");
//         printf("PID = %d\n", getpid());
//         printf("PPID = %d\n", getppid());
//     }
//     else
//     {
//         // Parent process
//         printf("Parent Process\n");
//         printf("PID = %d\n", getpid());
//         printf("Child PID = %d\n", pid);
//     }
//     return 0;
// }

#include <stdio.h>
#include <unistd.h>
int main()
{
    if (fork()) // First fork
    {
        if (!fork()) // Second fork (inside parent branch)
        {
            fork(); // Third fork
            printf("1"); // Child prints "1"
        }
        else
        {
            printf("2"); // Parent prints "2"
        }
    }
    else
    {
        printf("3"); // Child of first fork prints "3"
    }
    printf("4");
    return 0;
    // All processes print "4"
}