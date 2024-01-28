#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<conio.h>

/*
0-
1-Verifica CPF
2-Verifica RG
3-Verifica Numero de telefone
*/
void verifica_n_int(char *v,int vC){
    int i=0;
    char c;
    do{
        c = getch();

        if(c >= '0' && c <= '9'){
            printf("%c", c);
            *(v+i) = c;
            i++;
        }else if(c == 8 && i >= 1){
            *(v+i) = '\0';
            printf("\b \b");
            i--;
        }

        if(c == 13 && (i < 11 || i > 11) && vC == 1){
            printf("\nCPF Invalido:");
            i=0;
            c=14;
        }else if(c == 13 && (i < 11 || i > 11) && vC == 2){
            printf("\nRG Invalido:");
            i=0;
            c=14;
        }else if(c == 13 && (i < 11 || i > 11) && vC == 3){
            printf("\nNumero Invalido:");
            i=0;
            c=14;
        }

    }while(c != 13);

    *(v+i) = '\0';

}

void verifica_n_float(char *v){
    int i=0,cont = 0;
    char c;
    do{
        c = getch();
        if((c >= '0' && c <= '9') || c == '.'){
            if((c == '.' && cont == 0) && i != 0){
                printf("%c", c);
                *(v+i) = c;
                i++;
                cont = 1;
            }else if((c >= '0' && c <= '9')){
                printf("%c", c);
                *(v+i) = c;
                i++;
            }
        }else if(c == 8 && i >= 1){
            if(*(v+i-1) == '.'){
                cont=0;
            }

            *(v+i) = '\0';
            printf("\b \b");
            i--;
        }
    }while(c != 13);
}

int verifica_email(char *v){
    int n=strlen(v);
    for(int i=0;i<n;i++){
        if(*(v+i) == ' '){
            return 0;
        }
    }
    return 1;
}

/*
0 -
1 - Nome
2 - Status ou Tipo
*/
void verifica_letra(char *v,int opc){
    char c;
    int i=0;

    if(opc == 1){
        for(int j=0;j<50;j++){
            *(v+j) = '\0';
        }
    }else if(opc == 2){
        for(int j=0;j<12;j++){
            *(v+j) = '\0';
        }
    }

    do{
        c = getch();

        if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == ' ' || c == '.')){
            printf("%c", c);
            *(v+i) = c;
            i++;
        }else if(c == 8 && i >= 1){
            *(v+i) = '\0';
            printf("\b \b");
            i--;
        }else if(i == 0){
            c=14;
        }

    }while(c != 13);
    *(v+i) = '\0';
}

void verifica_arquivo(){
    FILE *arq;
    if((arq = fopen("../database/arquivo1.bin","rb"))){
        fclose(arq);
    }else{
        arq = fopen("../database/arquivo1.bin","wb");
        fclose(arq);
    }

    if((arq = fopen("../database/arquivo2.bin","rb"))){
        fclose(arq);
    }else{
        arq = fopen("../database/arquivo2.bin","wb");
        fclose(arq);
    }

    if((arq = fopen("../database/arquivo3.bin","rb"))){
        fclose(arq);
    }else{
        arq = fopen("../database/arquivo3.bin","wb");
        fclose(arq);
    }
    if((arq = fopen("../database/arquivo4.bin","rb"))){
        fclose(arq);
    }else{
        arq = fopen("../database/arquivo4.bin","wb");
        fclose(arq);
    }
}

void limparTela(){
    printf("\n");
    printf("Pressione qualquer tecla para continuar");
    getch();
    system("cls||clear");
}

char recebeUmNumero(char v){
    char temp;
    do{
        temp = getch();

        if(temp >= '0' && temp <= '9'){
            v = temp;
            printf("%c",v);
        }

    }while(!(temp >= '0' && temp <= '9'));

    return v;
}

void recebeNumeroQuarto(char *v){
    int i=0;
    char c;
    do{
        c = getch();

        if(c >= '0' && c <= '9'){
            printf("%c", c);
            *(v+i) = c;
            i++;
        }else if(c == 8 && i >= 1){
            *(v+i) = '\0';
            printf("\b \b");
            i--;
        }

    }while(c != 13 || (i < 3 || i > 3 ));

    *(v+i) = '\0';

}

/*
1 - Dia e Mes
2 - Ano
*/
void dia_mes_ano(char *v,int opc){
    int i=0;
    char c;
    if(opc == 1){
        do{
            c = getch();

            if(c >= '0' && c <= '9'){
                printf("%c", c);
                *(v+i) = c;
                i++;
            }else if(c == 8 && i >= 1){
                *(v+i) = '\0';
                printf("\b \b");
                i--;
            }

        }while(c != 13 || (i < 2 || i > 2 ));

        *(v+i) = '\0';
    }else if(opc == 2){
        do{
            c = getch();
            if(c >= '0' && c <= '9'){
                printf("%c", c);
                *(v+i) = c;
                i++;
            }else if(c == 8 && i >= 1){
                *(v+i) = '\0';
                printf("\b \b");
                i--;
            }
        }while(c != 13 || (i < 4 || i > 4 ));

        *(v+i) = '\0';
    }
}

int verificaData(int dia,int mes,int ano){
    int data = dataJuliana(dia,mes,ano);

    int mes1 = julianaGregorianaMes(data);
    int ano1 = julianaGregorianaAno(data);

    if(mes1 != mes){
        return 0;
    }
    if(ano1 != ano){
        return 0;
    }

    return 1;
}

//Retorna data juliana S2
int dataAtoi(char *dia,char *mes,char *ano){
    int data = dataJuliana(atoi(dia),atoi(mes),atoi(ano));

    return data;
}

void recebeData(char *dia,char *mes,char *ano){
    do{
        dia_mes_ano(dia,1);
        printf("/");
        dia_mes_ano(mes,1);
        printf("/");
        dia_mes_ano(ano,2);

        if(!(verificaData(atoi(dia),atoi(mes),atoi(ano))))
            printf("\nData invalida, Informe novamente a data no formato DD/MM/AAAA:");

    }while(!(verificaData(atoi(dia),atoi(mes),atoi(ano))));

}

void valorDiaria(char *vR,char *v,int dia){
    float valor = atof(v) * dia;
    printf("\n---------------------------\n");
    printf("Valor a pagar:R$%.2f",valor);
    printf("\n---------------------------\n");
    sprintf(vR,"%.2f",valor);
}

int verificaEntreData(int deE,int dsS,int deE2,int dsS2){
    if(((deE >= deE2 && deE <= dsS2) || (dsS >= deE2 && dsS <= dsS2)) || ((deE2 >= deE && deE2 <= dsS) || (dsS2 >= deE && dsS2 <= dsS))){
        return 0;
    }else{
        return 1;
    }
}

