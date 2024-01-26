#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int julianaGregorianaAno(int);
int julianaGregorianaMes(int);
int dataJuliana(int,int,int);

//Retorna o ano
int julianaGregorianaAno(int data){
    char dataM[50];

    int dataC;

    int b = data + 68569;
    int n = (4 * b) / 146097;
    b = b - ((146097 * n + 3) / 4);
    int k = 4000 * (b + 1) / 1461001;
    b = b - (1461 * k) / 4 + 31;
    int j = (80 * b) / 2447;
    b = (j / 11);
    int ano = 100 * (n - 49) + k + b;
    
    sprintf(dataM,"%04d",ano);

    dataC = atoi(dataM);

    return dataC;
}

//Retorna o mes
int julianaGregorianaMes(int data){
    char dataM[50];

    int dataC;

    int b = data + 68569;
    int n = (4 * b) / 146097;
    b = b - ((146097 * n + 3) / 4);
    int k = 4000 * (b + 1) / 1461001;
    b = b - (1461 * k) / 4 + 31;
    int j = (80 * b) / 2447;
    b = (j / 11);
    int mes = j + 2 - (12 * b);
    
    sprintf(dataM,"%02d",mes);

    dataC = atoi(dataM);

    return dataC;
}

/*Pega data gregoriana e transforma para juliana*/
int dataJuliana(int dia,int mes,int ano){
    int data = (1461 * (ano + 4800 + (mes - 14)/12)) / 4 + 
    (367 * (mes - 2 - 12 * ((mes - 14) / 12))) / 12 - 
    (3 * ((ano + 4900 + (mes - 14) / 12) / 100)) / 4 +
    dia-32075;
    
    return data;
}