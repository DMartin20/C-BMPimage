#include "myfunc.h"

int main(int argc, char *argv[])
{
    // --version, --help, -send, -recieve, -file, -socket,

    if (strcmp(argv[0], "./chart") != 0)
    {
        fprintf(stderr, "---------------HIBA!---------------\n A program neve nem megfelelő!\n");
        exit(7);
    }

    if (argc < 2)
    {
        signal(SIGINT, SignalHandler);
        signal(SIGUSR1, SignalHandler);
        printf("A program elindult -send -file módban\n");
        int *array;
        int SIZE = Measurement(&array);
        SendViaFile(array, SIZE);
        free(array);
        printf("A program befejezte a küldést és leáll!\n");
        exit(0);
    }

    else if (argc == 2)
    {
        if (strcmp(argv[1], "-send") == 0)
        {
            signal(SIGINT, SignalHandler);
            signal(SIGUSR1, SignalHandler);
            printf("A program elindult -send -file módban\n");
            int *array;
            int SIZE = Measurement(&array);
            SendViaFile(array, SIZE);
            free(array);
            printf("A program befejezte a küldést és leáll!\n");
            exit(0);
        }

        else if (strcmp(argv[1], "-file") == 0)
        {
            signal(SIGINT, SignalHandler);
            signal(SIGUSR1, SignalHandler);
            printf("A program elindult -send -file módban\n");
            int *array;
            int SIZE = Measurement(&array);
            SendViaFile(array, SIZE);
            free(array);
            printf("A program befejezte a küldést és leáll!\n");
            exit(0);
        }

        else if (strcmp(argv[1], "-receive") == 0)
        {
            signal(SIGINT, SignalHandler);
            signal(SIGUSR1, SignalHandler);
            printf("A program elindult -receive -file módban\n");
            printf("Várakozás a kliensre...\n");
            while (1)
            {
                signal(SIGUSR1, ReceiveViaFile);
            }
        }

        else if (strcmp(argv[1], "-socket") == 0) 
        {
            signal(SIGINT, SignalHandler);
            printf("A program elindult -send -socket módban\n");
            int *array;
            int SIZE = Measurement(&array);
            SendViaSocket(array, SIZE);
            free(array);
            printf("A program befejezte a küldést és leáll!\n");
            exit(0);
        }

        else if (!strcmp(argv[1], "--version"))
        {

#pragma omp parallel sections
            {
#pragma omp section
                {
                    printf("-----------------------------------\n");
                }
#pragma omp section
                {
                    printf("Verzió: 1.1.1 \n");
                }
#pragma omp section
                {
                    printf("Kiadás dátuma: 2024.04.20\n");
                }
#pragma omp section
                {
                    printf("Fejlesztő: Dobos Martin József\n");
                }
#pragma omp section
                {
                    printf("-----------------------------------\n");
                }
            }
        }

        else if (strcmp(argv[1], "--help") == 0)
        {
            Help();
            exit(0);
        }
        else
            Help();
    }

    else if (strcmp(argv[1], "-send") == 0 || strcmp(argv[2], "-send") == 0)
    {
        if (strcmp(argv[1], "-file") == 0 || strcmp(argv[2], "-file") == 0)
        {
            signal(SIGINT, SignalHandler);
            signal(SIGUSR1, SignalHandler);
            printf("A program elindult -send -file módban\n");
            int *array;
            int SIZE = Measurement(&array);
            SendViaFile(array, SIZE);
            free(array);
            printf("A program befejezte a küldést és leáll!\n");
            exit(0);
        }
        else if (strcmp(argv[1], "-socket") == 0 || strcmp(argv[2], "-socket") == 0)
        {
            signal(SIGINT, SignalHandler);
            printf("A program elindult -send -socket módban\n");
            int *array;
            int SIZE = Measurement(&array);
            SendViaSocket(array, SIZE);
            free(array);
            printf("A program befejezte a küldést és leáll!\n");
            exit(0);
        }
        else
            Help();
    }

    else if (strcmp(argv[1], "-receive") == 0 || strcmp(argv[2], "-receive") == 0)
    {
        if (strcmp(argv[1], "-file") == 0 || strcmp(argv[2], "-file") == 0)
        {
            signal(SIGINT, SignalHandler);
            signal(SIGUSR1, SignalHandler);
            printf("A program elindult -receive -file módban\n");
            printf("A program elindult -receive módban\n");
            printf("Várakozás a kliensre...\n");
            while (1)
            {
                signal(SIGUSR1, ReceiveViaFile);
            }
        }
        else if (strcmp(argv[1], "-socket") == 0 || strcmp(argv[2], "-socket") == 0)
        {
            signal(SIGINT, SignalHandler);
            printf("A program elindult -receive -socket módban\n");
            ReceiveViaSocket();
        }
        else
            Help();
    }

    else
    {
        fprintf(stderr, "---------------HIBA!---------------\n Nem megfelelő parancssori argumentumot adott meg!\n");
        Help();
        exit(8);
    }

    return EXIT_SUCCESS;
}
