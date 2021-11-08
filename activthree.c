#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "network.h"

// Compilar con: gcc main.c network.c analysis.c -o network
// C:\Users\Usuario\source\repos\Projectsl\Project\DNSips.txt
int main() {
    //Pedimos que se introduzca la ruta del archivo que se desea
    printf("Inserta una ruta valida en la que tengas tu documento con IPs: \n");
    char ipfile[256] = "C:/Users/Usuario/source/repos/Projectsl/Project/DNSips.txt";
    scanf("%s", &ipfile);

    //Mostramos por pantalla el contenido del archivo
    showFile(ipfile);
    printf("\n");

    //Primero mostramos los adaptadores de red
    char netsh[] = "netsh interface ipv4 show interfaces";
    system(netsh);

    //Introducimos el adaptador de red del que deseemos ver más datos
    char adapter[100] = "Ethernet 5";
    leecad(adapter, 100);


    //Llamamos a la funcion checkNet y desde ahí ya nos va a imprimir los resultados de la búsqueda
    char currentDns[256] = "";
    checkNet(adapter, currentDns);
    
    //Lanzamos los pings con esta funcion 
    checkPing(ipfile, currentDns);

    //Esta funcion modificara al adaptador si es necesario
    modifyAdapter(adapter, currentDns);

    //Eliminamos el archivo temporal
    remove("RespondingIps.txt");
    return 0;
}
