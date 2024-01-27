#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>

struct Cliente{
    char nCT[50];
    char cpf[12];
    char rg[12];
    char tel[12];
    char endereco[50];
    char email[50];
};

struct Quarto {
    char numero[4];
    char status[12];
    char tipo[12];
    char valorDiaria[10];
};

struct Data{
    char dia[3];
    char mes[3];
    char ano[5];
};

struct Hora{ 
    char horas[3];
    char min[3];
    char segundos[3];
};

struct Financeiro{
    char codigoReserva[6];
    char numeroQuarto[4];
    char cpfCliente[12];
    struct Data dataEntrada;
    struct Data dataSaida;
    struct Hora horaEntrada;
    struct Hora horaSaida;
    char valorT[10];
    char statusPagamento[9];
};

struct Reserva{
    char codigoReserva[6];
    char numeroQuarto[4];
    char nomeCliente[50];
    char cpfCliente[12];
    struct Data dataEntrada;
    struct Data dataSaida;
    char valorT[10];
    char statusPagamento[9];
};

#include "./funcoes/data.h"
#include "./funcoes/blindagens.h"
#include "./funcoes/cliente.h"
#include "./funcoes/quarto.h"
#include "./funcoes/reserva.h"
#include "./funcoes/renda.h"
#include "./funcoes/menu.h"

int main(){
    verifica_arquivo();
    
    struct Quarto *quarto = (struct Quarto *)calloc(10,sizeof(struct Quarto));
    struct Cliente *cliente = (struct Cliente *)calloc(10,sizeof(struct Cliente));
    struct Financeiro *financeiro = (struct Financeiro *)calloc(10,sizeof(struct Financeiro));
    struct Reserva *reserva = (struct Reserva *)calloc(10,sizeof(struct Reserva));
    
    char opc;
    
    do{
        printf("---------------Menu---------------\n1 - Menu Quarto\n2 - Menu Cliente\n3 - Menu Reservas\n0 - Sair\n----------------------------------\n");
        opc = recebeUmNumero(opc);
        printf("\n");

        limparTela();
        
        switch(opc){
            case '1':
                menu_quarto(quarto);
            break;
            case '2':
                menu_cliente(cliente);
            break;
            case '3':
                menu_reservas(cliente,quarto,reserva,financeiro);
            break;
            case '0':
                printf("Saindo...");
            break;
            default:
                printf("Opcao Invalida\n");
        }
    }while(opc != '0');

    free(quarto);
    free(cliente);
    free(financeiro);
    free(reserva);


    return 0;
}