#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<conio.h>
#include<time.h>

void realizar_checkin(struct Reserva *reserva, struct Financeiro *financeiro,struct Quarto *quarto);
void realizar_pagamento(struct Reserva *reserva, struct Financeiro *financeiro, struct Quarto *quarto);
void resgataDados(struct Reserva *reserva, struct Financeiro *financeiro,int i,int j);
void cadastrarPagamentosRegistrados(struct Financeiro *financeiro,int n);
void inserirDadosPagamentos(struct Financeiro *financeiro, int i,FILE *arq);
int consultarDadosPag(struct Financeiro *financeiro,char opc,char *v);
int consultarDadosRes(struct Reserva *reserva,int opc,char *v);
int quantidadePagamentos(struct Financeiro *financeiro);
void valores_recebidos(struct Financeiro *financeiro,int i);

void realizar_checkin(struct Reserva *reserva, struct Financeiro *financeiro,struct Quarto *quarto){
    FILE *arq=fopen("../database/arquivo3.bin","a+b");
    int n = quantidade_Reservas(reserva) - 1;

    struct tm *dHA;
    time_t segundos;
    time(&segundos);

    int nQ = quantidadeQuartos(quarto) - 1;
    char opc;
    int i=-1;

    do{
        int nR = quantidadePagamentos(financeiro) - 1;
        int j = nR;
        
        printf("\nComo voce deseja procurar a reserva para check-in\n");
        printf("1-Codigo de reserva\n2-Nome do Cliente\n0-Sair\n");
        opc = recebeUmNumero(opc);

        if(opc != '0'){
            
            i = consultar_reservas(reserva,n,opc);
            
            printf("\n--------%d--------\n",i);

            limparTela();

            if(i >= 0){
                for(int j=0;j < nQ ;j++){
                    if(strcmp((reserva+i)->numeroQuarto,(quarto+j)->numero)==0){
                        strcpy((quarto+j)->status,"Ocupado");
                        break;
                    }
                }

                cadastrarRegistradoQuartos(quarto,nQ);

                strcpy((financeiro+j)->numeroQuarto,(quarto+j)->numero);

                dHA = localtime(&segundos);
                sprintf((financeiro+j)->horaEntrada.horas,"%d",dHA->tm_hour);
                sprintf((financeiro+j)->horaEntrada.min,"%d",dHA->tm_min);
                sprintf((financeiro+j)->horaEntrada.segundos,"%d",dHA->tm_sec);
                sprintf((financeiro+j)->horaSaida.horas,"23");
                sprintf((financeiro+j)->horaSaida.min,"59");
                sprintf((financeiro+j)->horaSaida.segundos,"59");

                resgataDados(reserva,financeiro,i,j);

                inserirDadosPagamentos(financeiro,j,arq);

                printf("Ainda deseja continuar(S/N):");
                opc = getche();
                if(opc == 'N' || opc == 'n'){
                    printf("\nSaindo...");
                    break;
                }
            }
        }else{
            printf("\nSaindo...");
        }

    }while(opc != '0');
    
    limparTela();
    fclose(arq);
    
}

void resgataDados(struct Reserva *reserva, struct Financeiro *financeiro,int i,int j){
    sprintf((financeiro+j)->codigoReserva,"%s",(reserva+i)->codigoReserva);
    sprintf((financeiro+j)->cpfCliente,"%s",(reserva+i)->cpfCliente);
    sprintf((financeiro+j)->dataEntrada.dia,"%s",(reserva+i)->dataEntrada.dia);
    sprintf((financeiro+j)->dataEntrada.mes,"%s",(reserva+i)->dataEntrada.mes);
    sprintf((financeiro+j)->dataEntrada.ano,"%s",(reserva+i)->dataEntrada.ano);
    sprintf((financeiro+j)->dataSaida.dia,"%s",(reserva+i)->dataSaida.dia);
    sprintf((financeiro+j)->dataSaida.mes,"%s",(reserva+i)->dataSaida.mes);
    sprintf((financeiro+j)->dataSaida.ano,"%s",(reserva+i)->dataSaida.ano);
    sprintf((financeiro+j)->valorT,"%s",(reserva+i)->valorT);
    sprintf((financeiro+j)->statusPagamento,"%s",(reserva+i)->statusPagamento);
}

int consultarDadosPag(struct Financeiro *financeiro,char opc,char *nome){
    int i;
    int found=0;

    int n = quantidadePagamentos(financeiro) - 1;

    switch(opc){
        case '1':
            for(i=0;i<n;i++){
                if(strcmp((financeiro+i)->codigoReserva,nome)==0){
                    found=1;
                    break;
                }
            }
            if(!found)
                printf("\nNao foram encontrados nenhuma reserva com este codigo!\n");
        break;
        case '2':
            for(i=0;i<n;i++){
                if(strcmp((financeiro+i)->cpfCliente,nome)==0){
                    found=1;
                    break;
                }
            }
            if(!found)
                printf("\nNao foram encontrados nenhuma reserva com este CPF!\n");
        break;
        case '0':

        break;
        default:
            printf("Opcao Invalida");
    }
    if(found)
        return i;
    else
        return -1;
}

int consultarDadosRes(struct Reserva *reserva,int opc,char *nome){
    int n = quantidade_Reservas(reserva) - 1; 
    int found=0,i;

    switch(opc){
        case '1':
            for(i=0;i < n;i++){
                if(strcmp((reserva+i)->codigoReserva,nome)==0){
                    found=1;
                    break;
                }
            }
            if(!found)
                printf("\nNao foram encontrados nenhuma reserva com este codigo!\n");
        break;
        case '2':

                for(i=0;i < n;i++){
                    if(strcmp((reserva+i)->cpfCliente,nome)==0){
                        found=1;
                        break;
                    }
                }
                if(!found){
                    printf("\nNao foram encontrados nenhuma reserva com este CPF!\n");
                    break;
                }
        break;
        case '0':
            printf("Saindo...");
        default:
            printf("Opcao invalida");

    }
    if(found)
        return i;
    else
        return -1;
}

void realizar_pagamento(struct Reserva *reserva, struct Financeiro *financeiro, struct Quarto *quarto){
    struct tm *dHA;
    time_t segundos;
    time(&segundos);
    
    int nR = quantidade_Reservas(reserva) - 1;
    int nP = quantidadePagamentos(financeiro) - 1;
    int nQ = quantidadeQuartos(quarto) - 1;
    char opc,nome[12];
    int i,j;
    do{
        
        printf("\nComo voce deseja procurar a reserva para fazer o pagamento:\n");
        printf("1-Codigo de reserva\n2-CPF do cliente\n0-Sair\n");
        opc = recebeUmNumero(opc);

        if(opc != '0'){            

            if(opc == '1'){
                printf("Digite o Codigo de Reserva:");
                verifica_n_int(nome,0);

                j = consultarDadosPag(financeiro,opc,nome);
                i = consultarDadosRes(reserva,opc,nome);
            }else if(opc == '2'){
                printf("Digite o CPF do Cliente:");
                verifica_n_int(nome,1);

                j = consultarDadosPag(financeiro,opc,nome);
                i = consultarDadosRes(reserva,opc,nome);
            }

            printf("O valor a ser pago eh %s:", (financeiro+j)->valorT);
            
            limparTela();

            if(i >= 0){
                for(int j=0;j < nQ ;j++){
                    if(strcmp((reserva+i)->numeroQuarto,(quarto+j)->numero)==0){
                        strcpy((quarto+j)->status,"Livre");
                        break;
                    }
                }
                strcpy((reserva+i)->statusPagamento,"Pago");
                strcpy((financeiro+i)->statusPagamento,"Pago");


                dHA = localtime(&segundos);
                sprintf((financeiro+j)->horaSaida.horas,"%d",dHA->tm_hour);
                sprintf((financeiro+j)->horaSaida.min,"%d",dHA->tm_min);
                sprintf((financeiro+j)->horaSaida.segundos,"%d",dHA->tm_sec);

                resgataDados(reserva,financeiro,i,j);
                
                cadastrarRegistradoQuartos(quarto,nQ);
                cadastrarPagamentosRegistrados(financeiro,nP);
                cadastrarRegistradosReservas(reserva,nR);

                printf("Ainda deseja continuar(S/N):");
                opc = getche();
                if(opc == 'N' || opc == 'n'){
                    printf("\nSaindo...");
                    break;
                }
            }
        }else{
            printf("\nSaindo...");
        }

    }while(opc != '0');
    
    limparTela();
}

void inserirDadosPagamentos(struct Financeiro *financeiro, int i,FILE *arq){
    fwrite((financeiro+i)->codigoReserva,sizeof(char),sizeof((financeiro+i)->codigoReserva),arq);
    fwrite((financeiro+i)->numeroQuarto,sizeof(char),sizeof((financeiro+i)->numeroQuarto),arq);
    fwrite((financeiro+i)->cpfCliente,sizeof(char),sizeof((financeiro+i)->cpfCliente),arq);
    fwrite((financeiro+i)->dataEntrada.dia,sizeof(char),sizeof((financeiro+i)->dataEntrada.dia),arq);
    fwrite((financeiro+i)->dataEntrada.mes,sizeof(char),sizeof((financeiro+i)->dataEntrada.mes),arq);
    fwrite((financeiro+i)->dataEntrada.ano,sizeof(char),sizeof((financeiro+i)->dataEntrada.ano),arq);
    fwrite((financeiro+i)->dataSaida.dia,sizeof(char),sizeof((financeiro+i)->dataSaida.dia),arq);
    fwrite((financeiro+i)->dataSaida.mes,sizeof(char),sizeof((financeiro+i)->dataSaida.mes),arq);
    fwrite((financeiro+i)->dataSaida.ano,sizeof(char),sizeof((financeiro+i)->dataEntrada.ano),arq);
    fwrite((financeiro+i)->horaEntrada.horas,sizeof(char),sizeof((financeiro+i)->horaEntrada.horas),arq);
    fwrite((financeiro+i)->horaEntrada.min,sizeof(char),sizeof((financeiro+i)->horaEntrada.min),arq);
    fwrite((financeiro+i)->horaEntrada.segundos,sizeof(char),sizeof((financeiro+i)->horaEntrada.segundos),arq);
    fwrite((financeiro+i)->horaSaida.horas,sizeof(char),sizeof((financeiro+i)->horaSaida.horas),arq);
    fwrite((financeiro+i)->horaSaida.min,sizeof(char),sizeof((financeiro+i)->horaSaida.min),arq);
    fwrite((financeiro+i)->horaSaida.segundos,sizeof(char),sizeof((financeiro+i)->horaSaida.segundos),arq);
    fwrite((financeiro+i)->valorT,sizeof(char),sizeof((financeiro+i)->valorT),arq);
    fwrite((financeiro+i)->statusPagamento,sizeof(char),sizeof((financeiro+i)->statusPagamento),arq);

}

int quantidadePagamentos(struct Financeiro *financeiro){
    int i=0;

    FILE *arq;
    arq = fopen("../database/arquivo3.bin","rb");

    while(!feof(arq)){
        fread((financeiro+i)->codigoReserva,sizeof(char),sizeof((financeiro+i)->codigoReserva),arq);
        fread((financeiro+i)->numeroQuarto,sizeof(char),sizeof((financeiro+i)->numeroQuarto),arq);
        fread((financeiro+i)->cpfCliente,sizeof(char),sizeof((financeiro+i)->cpfCliente),arq);
        fread((financeiro+i)->dataEntrada.dia,sizeof(char),sizeof((financeiro+i)->dataEntrada.dia),arq);
        fread((financeiro+i)->dataEntrada.mes,sizeof(char),sizeof((financeiro+i)->dataEntrada.mes),arq);
        fread((financeiro+i)->dataEntrada.ano,sizeof(char),sizeof((financeiro+i)->dataEntrada.ano),arq);
        fread((financeiro+i)->dataSaida.dia,sizeof(char),sizeof((financeiro+i)->dataSaida.dia),arq);
        fread((financeiro+i)->dataSaida.mes,sizeof(char),sizeof((financeiro+i)->dataSaida.mes),arq);
        fread((financeiro+i)->dataSaida.ano,sizeof(char),sizeof((financeiro+i)->dataEntrada.ano),arq);
        fread((financeiro+i)->horaEntrada.horas,sizeof(char),sizeof((financeiro+i)->horaEntrada.horas),arq);
        fread((financeiro+i)->horaEntrada.min,sizeof(char),sizeof((financeiro+i)->horaEntrada.min),arq);
        fread((financeiro+i)->horaEntrada.segundos,sizeof(char),sizeof((financeiro+i)->horaEntrada.segundos),arq);
        fread((financeiro+i)->horaSaida.horas,sizeof(char),sizeof((financeiro+i)->horaSaida.horas),arq);
        fread((financeiro+i)->horaSaida.min,sizeof(char),sizeof((financeiro+i)->horaSaida.min),arq);
        fread((financeiro+i)->horaSaida.segundos,sizeof(char),sizeof((financeiro+i)->horaSaida.segundos),arq);
        fread((financeiro+i)->valorT,sizeof(char),sizeof((financeiro+i)->valorT),arq);
        fread((financeiro+i)->statusPagamento,sizeof(char),sizeof((financeiro+i)->statusPagamento),arq);
        i++;
    }

    fclose(arq);

    return i;
}

void cadastrarPagamentosRegistrados(struct Financeiro *financeiro,int n){
    FILE *arq;
    arq = fopen("../database/arquivo3.bin","wb");

    for(int i=0;i < n ;i++){
        fwrite((financeiro+i)->codigoReserva,sizeof(char),sizeof((financeiro+i)->codigoReserva),arq);
        fwrite((financeiro+i)->numeroQuarto,sizeof(char),sizeof((financeiro+i)->numeroQuarto),arq);
        fwrite((financeiro+i)->cpfCliente,sizeof(char),sizeof((financeiro+i)->cpfCliente),arq);
        fwrite((financeiro+i)->dataEntrada.dia,sizeof(char),sizeof((financeiro+i)->dataEntrada.dia),arq);
        fwrite((financeiro+i)->dataEntrada.mes,sizeof(char),sizeof((financeiro+i)->dataEntrada.mes),arq);
        fwrite((financeiro+i)->dataEntrada.ano,sizeof(char),sizeof((financeiro+i)->dataEntrada.ano),arq);
        fwrite((financeiro+i)->dataSaida.dia,sizeof(char),sizeof((financeiro+i)->dataSaida.dia),arq);
        fwrite((financeiro+i)->dataSaida.mes,sizeof(char),sizeof((financeiro+i)->dataSaida.mes),arq);
        fwrite((financeiro+i)->dataSaida.ano,sizeof(char),sizeof((financeiro+i)->dataEntrada.ano),arq);
        fwrite((financeiro+i)->horaEntrada.horas,sizeof(char),sizeof((financeiro+i)->horaEntrada.horas),arq);
        fwrite((financeiro+i)->horaEntrada.min,sizeof(char),sizeof((financeiro+i)->horaEntrada.min),arq);
        fwrite((financeiro+i)->horaEntrada.segundos,sizeof(char),sizeof((financeiro+i)->horaEntrada.segundos),arq);
        fwrite((financeiro+i)->horaSaida.horas,sizeof(char),sizeof((financeiro+i)->horaSaida.horas),arq);
        fwrite((financeiro+i)->horaSaida.min,sizeof(char),sizeof((financeiro+i)->horaSaida.min),arq);
        fwrite((financeiro+i)->horaSaida.segundos,sizeof(char),sizeof((financeiro+i)->horaSaida.segundos),arq);
        fwrite((financeiro+i)->valorT,sizeof(char),sizeof((financeiro+i)->valorT),arq);
        fwrite((financeiro+i)->statusPagamento,sizeof(char),sizeof((financeiro+i)->statusPagamento),arq);
    }

    fclose(arq);
}

void valores_recebidos(struct Financeiro *financeiro,int i){
    int n = quantidadePagamentos(financeiro) - 1;
    if(i == -1){
        for(i=0;i<n;i++){
            printf("\n----------------------Financas %d----------------------\n",i+1);
            printf("Codigo de reserva:%s\nNumero do Quarto:%s\nCPF do cliente:%s\n",(financeiro+i)->codigoReserva,(financeiro+i)->numeroQuarto,(financeiro+i)->cpfCliente);
            printf("Data de Entrada:%s/%s/%s - Hora de Entrada: %s:%s:%s\n",(financeiro+i)->dataEntrada.dia,(financeiro+i)->dataEntrada.mes,(financeiro+i)->dataEntrada.ano,(financeiro+i)->horaEntrada.horas,(financeiro+i)->horaEntrada.min,(financeiro+i)->horaEntrada.segundos);
            printf("Data de Saida:%s/%s/%s - Hora de Saida: %s:%s:%s\n",(financeiro+i)->dataSaida.dia,(financeiro+i)->dataSaida.mes,(financeiro+i)->dataSaida.ano,(financeiro+i)->horaSaida.horas,(financeiro+i)->horaSaida.min,(financeiro+i)->horaSaida.segundos);
            printf("Valor Total:%s\nStatus do pagamento:%s\n",(financeiro + i)->valorT,(financeiro + i)->statusPagamento);
            printf("------------------------------------------------------\n"); 
        }
    }else{
        printf("\n----------------------Financas %d----------------------\n",i+1);
        printf("Codigo de reserva:%s\nNumero do Quarto:%s\nCPF do cliente:%s\n",(financeiro+i)->codigoReserva,(financeiro+i)->numeroQuarto,(financeiro+i)->cpfCliente);
        printf("Data de Entrada:%s/%s/%s - Hora de Entrada: %s:%s:%s\n",(financeiro+i)->dataEntrada.dia,(financeiro+i)->dataEntrada.mes,(financeiro+i)->dataEntrada.ano,(financeiro+i)->horaEntrada.horas,(financeiro+i)->horaEntrada.min,(financeiro+i)->horaEntrada.segundos);
        printf("Data de Saida:%s/%s/%s - Hora de Saida: %s:%s:%s\n",(financeiro+i)->dataSaida.dia,(financeiro+i)->dataSaida.mes,(financeiro+i)->dataSaida.ano,(financeiro+i)->horaSaida.horas,(financeiro+i)->horaSaida.min,(financeiro+i)->horaSaida.segundos);
        printf("Valor Total:%s\nStatus do pagamento:%s\n",(financeiro + i)->valorT,(financeiro + i)->statusPagamento);
        printf("-------------------------------------------------------\n"); 
    }
    limparTela();
}