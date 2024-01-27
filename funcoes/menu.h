#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<conio.h>
#include<time.h>

void menu_quarto(struct Quarto *quarto);
void menu_cliente(struct Cliente *cliente);
void menu_reservas(struct Cliente *cliente,struct Quarto *quarto,struct Reserva *reserva,struct Financeiro *financeiro);

void menu_quarto(struct Quarto *quarto){
    int n = quantidadeQuartos(quarto) - 1;

    char opc;

    do{
        printf("----------Menu do Quarto----------\n1 - Cadastrar Quarto\n2 - Consultar Quarto\n3 - Editar Quarto\n4 - Excluir Quarto\n0 - Voltar\n----------------------------------\n");
        opc = recebeUmNumero(opc);
        
        printf("\n");
        limparTela();
        switch(opc){
            case '1':

            n = cadastrar_quarto(quarto,n);
            
            break;
            case '2':
            if(n>0){
                menu_consultar_quarto(quarto,n);
            }else{
                printf("Não ha quartos cadastrados para serem consultados");
            }
            break;
            case '3':
            if(n>0){

             menu_editarQuarto(quarto,n);

            }else{
                printf("Nao ha quartos cadastrados para serem editados");
            }
            break;
            case '4':
            if(n>0){
                excluir_quarto(quarto,n);
            }else{
                printf("Nao ha quartos cadastrados para serem excluidos");
            }
            break;
            case '0':
                printf("Saindo...");
            break;
            default:
                printf("Opcao Invalida");
        }
        limparTela();
    }while(opc != '0');
}


void menu_cliente(struct Cliente *cliente){
    int n = quantidadeCliente(cliente) - 1;
    char opc;
    
    do{
        printf("----------Menu do Cliente----------\n1 - Cadastrar Cliente\n2 - Consultar Cliente\n3 - Editar Cliente\n4 - Excluir Cliente\n0 - Voltar\n----------------------------------\n");
        opc = recebeUmNumero(opc);
        printf("\n");
        limparTela();

        switch(opc){
            case '1':
                n = cadastrar_cliente(cliente,n);
            break;
            case '2':
                if(n>0){
                menuConsultar(cliente,n);

                }else{
                    printf("Nao ha cliente cadastrados para serem consultados");

                }
            break;
            case '3':
                if(n>0){
                menueditar(cliente,n);
                }else{
                    printf("Nao ha clienes cadastrados para uma consulta");
                }
            break;
            case '4':
            if(n>0){
                excluir_cliente(cliente,n);
            }else{
                printf("Nao ha clientes para serem excluidos");
            }
            break;
            case '0':
                printf("Saindo...");
            break;
            default:
                printf("Opcao Invalida");
        }
        limparTela();
    }while(opc != '0');
}

void menu_reservas(struct Cliente *cliente,struct Quarto *quarto,struct Reserva *reserva,struct Financeiro *financeiro){   
    int nR = quantidade_Reservas(reserva) - 1;

    char opc;
    do{
        printf("----------Menu do Reserva----------\n");
        printf("1 - Realizar Reserva\n2 - Realizar Check-In\n3 - Realizar pagamento\n4 - Consultar Reservas\n5 - Excluir Reserva\n6 - Valores Recebidos \n0 - Voltar\n");
        printf("----------------------------------\n");
        opc = recebeUmNumero(opc);

        limparTela();

        switch(opc){
            case '1':
                nR = realizar_reserva(cliente,quarto,reserva,nR);
            break;
            case '2':
                realizar_checkin(reserva,financeiro,quarto);
            break;
            case '3':
                realizar_pagamento(reserva,financeiro,quarto);
            break;
            case '4':
                menu_consultarReservas(reserva,nR);
            break;
            case '5':
                excluir_reserva(reserva,nR);
            break;
            case '6':
                valores_recebidos(financeiro,-1);
            break;
            case '0':
                printf("Saindo...");
            break;
            default:
                printf("Opcao Invalida!");
        }
    }while(opc != '0');
}
