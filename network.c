#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "analysis.h"

void checkPing(const char* file, char* currentDns) {
    FILE* archivoIp;
    
    archivoIp = fopen(file, "r"); //Abrimos el archivo

    char ipbuf[256] = "";
    if (archivoIp == NULL) {
        perror("Failed: ");
        return;
    }
    else {
        //Creamos un archivo temporal y escribimos primero el actual DNS que estamos usando
        FILE* tempFile = NULL;
        


        while (fgets(ipbuf, sizeof(ipbuf), archivoIp)) {
            ipbuf[strcspn(ipbuf, "\n")] = 0;
            
            //Preparamos el comando y la ruta donde vamos a guardar la informacion del archivo
            ping(ipbuf);

            printf("\n");
            char found[256];
            if (findInFile("result.txt", "Media", found)) {//Buscamos normal con nuestra funcion findInFile generica
                printf("Contesta %s \n", ipbuf);

                tempFile = fopen("RespondingIps.txt", "w"); //Aqui abrimos el archivo temporal
                if (tempFile == NULL)
                {

                    printf("Unable to create file.\n");

                }
                
                //Aqui añadimos las ips que esten funcionando en el archivo temporal
                fputs(ipbuf, tempFile);
                fputs("\n", tempFile);
                fclose(tempFile);
            }
            else {
                printf("No contesta %s \n", ipbuf);
            }

            fflush(stdout);
        }
        
    }

    fclose(archivoIp); //Cerrar el archivo
  
}
void modifyAdapter(char* adapter, char* currentDns) {
    FILE* archivoIp;
    archivoIp = fopen("RespondingIps.txt", "r"); //Abrimos el archivo 

    char ipbuf[256] = "";
    if (archivoIp == NULL) {
        perror("Failed: ");
        return;
    }
    else {
        long usedDns = extractVelocity(currentDns);
        printf("%d", usedDns);
        printf("\n");
        long bestAverage = usedDns; //Indicamos como valor máximo un LONG_MAX para que el valor de entrada nunca sea más rápido que el valor con el que se compara
        char fasterDNS[256];
        while (fgets(ipbuf, sizeof(ipbuf), archivoIp)) {
            ipbuf[strcspn(ipbuf, "\n")] = 0;

            long average = extractVelocity(ipbuf);
            printf("%d", average);

            if (average != -1 && average < bestAverage) {
                bestAverage = average;
                strcpy(fasterDNS, ipbuf);
            } 

            printf("\n");

            fflush(stdout);
        }

        if (bestAverage < usedDns) {
            //Para evitar problemas con nuestro ordenador no lanzamos el comando realmente, solo lo simulamos con un printf
            printf("interface ip set dns \"%s\" static %s", adapter, fasterDNS);
            printf("Para evitar problemas con nuestro ordenador no lanzamos el comando realmente, solo lo simulamos con un printf");
        }
        else {
            printf("Mejor DNS no disponible");
        }
    }

    fclose(archivoIp); //Cerrar el archivo
}

//Funcion para mostrar la informacion de los adaptadores de red
void checkNet(const char* adapter, char* currentDns) {
    //Preparamos el comando y la ruta donde vamos a guardar la informacion del archivo
    
    char* tempName = tmpnam(NULL); 
    char confName[] = "netsh interface ipv4 show config name = \"%s\" > %s";
    char cmd[256];
    sprintf(cmd, confName, adapter, tempName);
    system(cmd); //Lanzamos el comando /



    char line[256];
    if (findInFile(tempName, "Servidores DNS", line)) {
        char dns[256];
        extractString(line, ":", dns); //Este contiene un espacio en blanco
        dns[strcspn(dns, "\n")] = 0;
        strcpy(currentDns, dns); //Devolvemos el valor de esa linea
        printf("Este es el actual DNS configurado: %s \n", dns);

    }

    
    fflush(stdout);
    remove(tempName); 
}


//Funcion para comparar la velocidad de los DNS que hayamos insertado por pantalla
long extractVelocity(char* dns) {
    char* tempName = tmpnam(NULL);

    char ping[] = "ping -c1 -s %s > %s";
    char cmd[256];

    //Preparamos el comando, escribiendo en cmd el comando y ejecutandolo con cmd
    sprintf(cmd, ping, dns, tempName);
    printf(cmd);
    system(cmd);

    printf("\n");
    char delimiter[] = "= "; //Le decimos cual es el delimitador que debe buscar dentro de la linea que nos interesa
    char line[256];//Aquí guardamos la informacion original de la linea, antes de extraer la parte que nos interesa
    long average = -1;
    if (findInFile(tempName, "Media = ", line)) { //Buscamos normal con nuestra funcion findInFile generica
        average = extractValue(line, delimiter); //Con la funcion extractValue, nos va a extraer solamente el numero integer y lo almacenamos
    }

    remove(tempName);

    return average;
}