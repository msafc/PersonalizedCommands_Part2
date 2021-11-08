#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>



void ping(char* ipbuf) {
    char ping[] = "ping -c1 -s %s > result.txt";
    char cmd[256];
    sprintf(cmd, ping, ipbuf);
    printf(cmd);
    system(cmd);
}
/*
    Funcion para buscar la palabra dentro del archivo en
    el que vamos a grabar la salida de los comandos
*/
bool findInFile(const char* path, const char* word, char* output) {
    FILE* fileToSearch;
    fileToSearch = fopen(path, "r");

    if (fileToSearch == NULL) {
        perror("Failed: ");
        return false;
    }
    else {
        char line[256];
        while (fgets(line, sizeof(line), fileToSearch)) {
            /*
            La funcion strstr está pensada para buscar
            una palabra dentro de un string que se le de
            Y lo guardamos en un char, que será el valor
            que devolveremos.
            */
            char* ret = strstr(line, word);
            if (ret != NULL) {
                fclose(fileToSearch); //Hay que cerrar el archivo para seguir ejecutando el codigo
                strcpy(output, ret); //Devolvemos el valor de esa linea
                return true;
            }
        }
    }

    fclose(fileToSearch);
    /*
     La propia funcion strstr determina que si no
     encuentra lo que se le pide, devuelve un valor nulo
     */
    return false;
}

//Con esta funcion extraeremos un valor dentro de una linea, marcandole un delimitador
long extractValue(char* line, char* delimiter) {
    char* token;

    //La funcion strtok nos divide una linea string por partes que esten separadas por un delimitador
    token = strtok(line, delimiter);
    token = strtok(NULL, delimiter);

    //La funcion strtoul nos extrae el numero integer de una linea, pero debe ser un valor
    //que se encuentre al inicio de una linea, por eso lo hemos separado previamente
    char* ptr;
    long ret;
    ret = strtoul(token, &ptr, 10);

    return ret; //Devolvemos ese valor
}

char* extractString(char* line, char* delimiter, char* output) {
    char* token;

    //La funcion strtok nos divide una linea string por partes que esten separadas por un delimitador
    token = strtok(line, delimiter);
    token = strtok(NULL, delimiter);


    strcpy(output, token);

    return token;
}

//Funcion para imprimir por pantalla lo que lee de un archivo
void showFile(const char* file) {
    FILE* archivoIp;
    archivoIp = fopen(file, "r"); //Abrimos el archivo

    //Revisamos si el archivo no es nulo, si lo es, devuelve error, 
    //si no, nos imprime por pantalla linea por linea lo que va leyendo
    if (archivoIp == NULL) {
        perror("Failed: ");
        fclose(archivoIp); //Cerrar el archivo
        return;
    }
    else {
        char ipbuf[256];
        fgets(ipbuf, sizeof(ipbuf), archivoIp);
        printf(ipbuf);
        while (fgets(ipbuf, sizeof(ipbuf), archivoIp)) {
            printf(ipbuf);
            fflush(stdout);
        }
    }

    fclose(archivoIp); //Cerrar el archivo
}

//La funcion que nos permite introducir una linea con espacios por consola
int leecad(char* cad, int n) {
    int i, c;
    c = getchar();

    if (c == EOF) {
        cad[0] = '\0';
        return 0;
    }

    if (c == '\n')
        i = 0;

    else {
        cad[0] = c;
        i = 1;
    }

    for (; i < n - 1 && (c = getchar()) != EOF && c != '\n'; i++)

        cad[i] = c;

    cad[i] = '\0';
    if (c != '\n' && c != EOF)

        while ((c = getchar()) != '\n' && c != EOF);

    return 1;

};