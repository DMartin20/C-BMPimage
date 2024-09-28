#ifndef MYFUNC_H_
#define MYFUNC_H_

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <ctype.h>
#include <dirent.h>
#include <omp.h>
int s;

#define PORT_NO 3333
#define INITIAL_CAPACITY 2
#define MULTIPLIER 1.5
#define BUF_SIZE 500

void SignalHandler(int sig)
{
    if (sig == SIGINT)
    {
        printf("\nKilépés...\n");
        exit(0);
    }
    if (sig == SIGUSR1)
    {
        printf("Hiba!\n");
        printf("A fájlon keresztüli küldés nem elérhető!\n");
        exit(4);
    }
    if (sig == SIGALRM)
    {
        printf("Hiba!\n");
        printf("A szerver nem válaszolt időben!.\n");
        printf("Kilépés...\n");
        exit(6);
    }
}

int Measurement(int **Values)
{
    double rnum;
    int SIZE;
    time_t now;
    time(&now);

    SIZE = now % 900;
    if (SIZE < 100)
    {
        SIZE = 100;
    }
    *Values = malloc(SIZE * sizeof(int));
    (*Values)[0] = 0;
    srand(time(NULL));
    for (int i = 1; i < SIZE; i++)
    {
        rnum = (rand() / (double)RAND_MAX) * 100;

        if (rnum <= 42.8571)
        {
            (*Values)[i] = (*Values)[i - 1] + 1;
        }
        if (rnum > 42.8571 && rnum <= 42.8571 + 35.4838)
        {
            (*Values)[i] = (*Values)[i - 1] - 1;
        }
        else if (rnum > 42.8571 + 35.4838 && rnum <= 100)
        {
            (*Values)[i] = (*Values)[i - 1];
        }
    }

    return SIZE;
}

void BMPcreator(int *Values, int NumValues)
{
    unsigned long int fsize = (62 + (NumValues * NumValues));
    unsigned int pad = 0;
    int file = open("mybmp.BMP", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IROTH);
    unsigned char **tomb = (unsigned char **)calloc(NumValues, sizeof(unsigned char *));

    unsigned char f_headder[] = {0x42, 0x4d, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x0f,
                                 0x00, 0x00, 0x61, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff};

    for (int i = 0; i < NumValues; ++i)
    {
        tomb[i] = (unsigned char *)calloc(NumValues, sizeof(unsigned char));
    }
    for (int i = 0; i < NumValues; ++i)
    {
        int elem = (NumValues / 2) + Values[i];
        if (elem > NumValues)
        {
            elem = NumValues - 1;
        }
        else if (elem < -(NumValues / 2))
        {
            elem = 0;
        }
        tomb[elem][i] = 0x01;
    }

    write(file, f_headder, sizeof(unsigned char) * 2);
    write(file, &fsize, sizeof(int));
    write(file, &f_headder[2], sizeof(unsigned char) * 12);
    write(file, &NumValues, sizeof(int));
    write(file, &NumValues, sizeof(int));
    write(file, &f_headder[14], sizeof(unsigned char) * 36);

    for (int i = 0; i < NumValues; i++)
    {
        int x = 7;
        for (int j = 0; j < NumValues; ++j)
        {
            if (tomb[i][j] == 0x01)
            {
                int ps = 1 << x;
                pad = pad + ps;
            }
            if (x == 24)
            {
                write(file, &pad, sizeof(int));
                pad = 0;
                x = 7;
                continue;
            }
            if (x % 8 == 0)
            {
                x = (j % 32) + 9;
            }
            --x;
        }
        write(file, &pad, sizeof(int));
        pad = 0;
    }
    free(tomb);
}

void Help()
{
    printf("-----------------------------------\n");
    printf("A program a következő parancssori argumentumokkal működik:\n");
    printf(": --help -- segítség a futtatáshoz\n");
    printf(": --version -- program verzió információk\n");
    printf("-----------------------------------\n");
    printf("Üzemmódok:\n");
    printf(": -send -- a program küldő üzemmódban indul\n");
    printf(": -receive -- a program fogadó üzemmódban indul\n");
    printf("-----------------------------------\n");
    printf("Kommunikáció módjára vonatkozó argumegtumok:\n");
    printf(": -file -- kommunikáció fájlkezeléssel\n");
    printf(": -socket -- kommunikáció hálózaton keresztül\n");
    printf("-----------------------------------\n");
    printf("A program alapértelmezett indítási paraméterei:\n");
    printf(": ./chart -send -file\n");
    printf("-----------------------------------\n");
    printf("Egy paraméter csak egyszer szerepelhet, sorrendjük tetszőleges!\n");
}

void SendViaSocket(int *Values, int NumValues)
{
    /************************ Declarations **********************/
    int s;                        // socket ID
    int bytessent, bytesreceived; // received/sent bytes
    int flag;                     // transmission flag
    char on;                      // sockopt option
    unsigned int server_size;     // length of the sockaddr_in server
    struct sockaddr_in server;    // address of server
    int received;
    int basicarray[NumValues];
    /************************ Initialization ********************/
    on = 1;
    flag = 0;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT_NO);
    server_size = sizeof server;

    /************************ Creating socket *******************/
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0)
    {
        fprintf(stderr, "Socket készítési hiba!.\n");
        exit(2);
    }
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

    /************************ Sending data **********************/
    printf("--------------------------------------\n");

    printf("Tömbméret (%d) küldése.", NumValues);
    bytessent = sendto(s, &NumValues, sizeof(NumValues), flag, (struct sockaddr *)&server, server_size);
    if (bytessent <= 0)
    {
        fprintf(stderr, "Küldési hiba!.\n");
        exit(3);
    }

    signal(SIGALRM, SignalHandler);
    alarm(1);

    printf(" %i byte elküldve a szervernek.\n", bytessent);

    /************************ Receive data **********************/
    bytesreceived = recvfrom(s, &received, sizeof(received), flag, (struct sockaddr *)&server, &server_size);
    if (bytesreceived != bytessent)
    {
        fprintf(stderr, "A küldött és fogadott adatok méretei különböznek.\n");
        exit(4);
    }
    printf("A szerver által kapott tömbméret (%s:%d) : %d\n",
           inet_ntoa(server.sin_addr), ntohs(server.sin_port), received);

    // sending array
    for (int i = 0; i < NumValues; i++)
    {
        basicarray[i] = Values[i];
    }

    printf("Tömb küldése a szervernek...\n");

    bytessent = sendto(s, basicarray, sizeof(basicarray), flag, (struct sockaddr *)&server, server_size);
    if (bytessent <= 0)
    {
        fprintf(stderr, "Küldési hiba!.\n");
        exit(3);
    }
    printf(" %i byte elküldve a szervernek.\n", bytessent);

    printf("--------------------------------------\n");

    /************************ Closing ***************************/
    close(s);
}

void ReceiveViaSocket()
{
    /************************ Declarations **********************/
    int bytes;                 // received/sent bytes
    int err;                   // error code
    int flag;                  // transmission flag
    char on;                   // sockopt option
    unsigned int server_size;  // length of the sockaddr_in server
    unsigned int client_size;  // length of the sockaddr_in client
    struct sockaddr_in server; // address of server
    struct sockaddr_in client; // address of client
    int arraysize;
    int *array;

    /************************ Initialization ********************/
    on = 1;
    flag = 0;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT_NO);
    server_size = sizeof server;
    client_size = sizeof client;
    signal(SIGINT, SignalHandler);

    /************************ Creating socket *******************/
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0)
    {
        fprintf(stderr, "Socket készítési hiba!.\n");
        exit(2);
    }
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof on);

    /************************ Binding socket ********************/
    err = bind(s, (struct sockaddr *)&server, server_size);
    if (err < 0)
    {
        fprintf(stderr, "Socket binding hiba!.\n");
        exit(3);
    }

    while (1)
    { // Continuous server operation
        /************************ Receive data **********************/
        printf("\n Várakozás az üzenetre...\n");
        bytes = recvfrom(s, &arraysize, sizeof(arraysize), flag, (struct sockaddr *)&client, &client_size);

        if (bytes < 0)
        {
            fprintf(stderr, "Üzenet fogadási hiba!.\n");
            exit(5);
        }
        printf("--------------------------------------\n");

        printf("%d byte fogadva a klienstől (%s:%d).\n A kapott tömbméret: %d\n",
               bytes, inet_ntoa(client.sin_addr), ntohs(client.sin_port), arraysize);

        /************************ Sending data **********************/
        sendto(s, &arraysize, sizeof(arraysize), flag, (struct sockaddr *)&client, client_size);
        if (bytes <= 0)
        {
            fprintf(stderr, "Küldési hiba.\n");
            exit(3);
        }
        printf(" Nyugta el küldve a kliensnek!");

        // receiveing array
        int basicarray[arraysize];

        array = malloc(arraysize * sizeof(int));
        printf("\n Várakozás az adattömbre...\n");
        bytes = recvfrom(s, basicarray, sizeof(basicarray), flag, (struct sockaddr *)&client, &client_size);
        if (bytes < 0)
        {
            fprintf(stderr, "Üzenet fogadási hiba!.\n");
            exit(5);
        }
        printf("%d byte fogadva a klienstől (%s:%d).\n A tömb fogadva.\n",
               bytes, inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        for (int i = 0; i < arraysize; i++)
        {
            array[i] = basicarray[i];
        }
        printf("BMP fájl generálása...\n");
        BMPcreator(array, arraysize);
        printf("BMP élkészült\n");
        printf("--------------------------------------\n");

        free(array);
    }
}

/*int FindPID() {
    char cmd[100] = "pgrep chart"; // command to search for "chart" processes
    char pid_str[10];
    pid_t pid1 = -1, pid2 = -1;
    FILE* pipe = popen(cmd, "r");
    if (pipe == NULL) {
        perror("popen");
        return -1; // error occurred
    }
    while (fgets(pid_str, sizeof(pid_str), pipe) != NULL) {
        pid_t pid = atoi(pid_str);
        if (pid1 == -1) {
            pid1 = pid;
        } else if (pid2 == -1) {
            pid2 = pid;
            break;
        }
    }
    pclose(pipe); // close the pipe
    if (pid1 == -1 || pid2 == -1) {
        return -1;
    }
    if (pid1 == getpid()) {
        return pid2;
    }
    else if (pid2 == getpid())
    {
        return pid1;
    }

}*/

int FindPID()
{
    DIR *dir;
    struct dirent *entry;
    char path[BUF_SIZE];
    char line[BUF_SIZE];
    char name[BUF_SIZE];
    char pid_str[BUF_SIZE];
    pid_t pid;
    FILE *fp;

    int fpid = getpid();

    dir = opendir("/proc");
    if (!dir)
    {
        fprintf(stderr, "Hiba a /proc megnyitásakor\n");
        exit(3);
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (isdigit(entry->d_name[0]))
        {
            snprintf(path, sizeof(path), "/proc/%s/status", (*entry).d_name);
            fp = fopen(path, "r");

            if (fp == NULL)
            {
                fprintf(stderr, "Hiba /proc/  /status megnyitásakor");
                closedir(dir);
                exit(3);
            }

            if (fgets(line, sizeof(line), fp) == NULL)
            {
                fprintf(stderr, " /proc/  /status üres");
                fclose(fp);
                closedir(dir);
                exit(3);
            }

            sscanf(line, "Name:\t%s", name);

            if (strcmp(name, "chart") == 0)
            {
                while (fgets(line, sizeof(line), fp))
                {
                    if (sscanf(line, "Pid:\t%s", pid_str) == 1)
                    {
                        pid = atoi(pid_str);
                        if (pid != fpid)
                        {
                            return pid;
                        }
                    }
                }
            }
            fclose(fp);
        }
    }
    closedir(dir);
    return -1;
}

void ReceiveViaFile(int sig)
{
    FILE *file = fopen("Measurement.txt", "r");

    // Fájl ellenőrzése
    if (file == NULL)
    {
        printf("Hiba: Nem lehet megnyitni a Measurement fájlt.\n");
        exit(EXIT_FAILURE);
    }

    // Memóriaterület foglalása az adatok tárolásához
    int *measurements = (int *)malloc(sizeof(int));
    if (measurements == NULL)
    {
        fprintf(stderr, "Hiba: Nem sikerült memóriát foglalni.\n");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    char line[50];
    // Fájl sorainak beolvasása és adatok tárolása
    while (fgets(line, sizeof(line), file))
    {
        measurements[index] = atoi(line);
        index++;
        measurements = (int *)realloc(measurements, sizeof(int) * (index + 1));
        if (measurements == NULL)
        {
            fprintf(stderr, "Hiba: Nem sikerült memóriát újraallokálni.\n");
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);

    // BMP fájl létrehozása az adatokkal
    BMPcreator(measurements, index);
    printf("BMP fájl elkészült.\n");

    // Memóriaterület felszabadítása
    free(measurements);

    printf("Várakozás a kliensre...\n");
}

void SendViaFile(int *Values, int NumValues)
{
    FILE *f;
    f = fopen("Measurement.txt", "w");
    for (int i = 0; i < NumValues; i++)
    {
        fprintf(f, "%d\n", Values[i]);
    }

    fclose(f);

    int pid = FindPID();

    if (pid == -1)
    {
        printf("Hiba!\n");
        printf("Nem létezik fogadó folyamat.\n");
        printf("Leállítás...\n");
        exit(-1);
    }
    else
    {
        kill(pid, SIGUSR1);
        printf("A fájl elküldése sikeres!\n");
    }
}

#endif
