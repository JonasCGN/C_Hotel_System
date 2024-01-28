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
void valores_recebidos(struct Financeiro *financeiro);


void realizar_checkin(struct Reserva *reserva, struct Financeiro *financeiro,struct Quarto *quarto){
    FILE *arq=fopen("../database/arquivo3.bin","a+b");

    struct tm *dHA;
    time_t segundos;

    int nP = quantidadePagamentos(financeiro) - 1;
    int nQ = quantidadeQuartos(quarto) - 1;
    int nR = quantidade_Reservas(reserva) -1;
    char opc;
    int i=-1,j = nP,p;

    do{ 
        time(&segundos);
        printf("\nComo voce deseja procurar a reserva para check-in\n");
        printf("1-Codigo de reserva\n2-Nome do Cliente\n0-Sair\n");
        opc = recebeUmNumero(opc);

        if(opc != '0'){
            printf("\n");
            i = consultarDadosRes1(reserva,opc);
            
            limparTela();
            if(strcmp((reserva+i)->statusPagamento,"Pendente") == 0){
                if(i >= 0){
                    for(p=0;p < nQ ;p++){
                        if(strcmp((reserva+i)->numeroQuarto,(quarto+p)->numero) == 0 && !(strcmp((reserva+i)->statusPagamento,"Pago")==0)){
                            strcpy((quarto+p)->status,"Ocupado");
                            break;
                        }
                    }
    
                    cadastrarRegistradoQuartos(quarto,nQ);
    
                    strcpy((reserva+i)->statusPagamento,"Check-In");
    
                    strcpy((financeiro+j)->numeroQuarto,(quarto+p)->numero);
    
                    dHA = localtime(&segundos);

                    sprintf((financeiro+j)->horaEntrada.horas,"%d",dHA->tm_hour);
                    sprintf((financeiro+j)->horaEntrada.min,"%d",dHA->tm_min);
                    sprintf((financeiro+j)->horaEntrada.segundos,"%d",dHA->tm_sec);
                    sprintf((financeiro+j)->horaSaida.horas,"23");
                    sprintf((financeiro+j)->horaSaida.min,"59");
                    sprintf((financeiro+j)->horaSaida.segundos,"59");
    
                    resgataDados(reserva,financeiro,i,j);
    
                    inserirDadosPagamentos(financeiro,j,arq);
                    
                    cadastrarRegistradosReservas(reserva,nR);
                }
            
                printf("Ainda deseja continuar(S/N):");
                opc = getche();
                if(opc == 'N' || opc == 'n'){
                    printf("\nSaindo...");
                    break;
                }
            }else if(strcmp((reserva+i)->statusPagamento,"Check-In") == 0){
                printf("A reserva ja foi feita Check-In");
            }else if(strcmp((reserva+i)->statusPagamento,"Pago") == 0){
                printf("A reserva ja foi realizado o pagamento");
            }else if(i == -1 && opc == '1'){
                printf("Reserva com o codigo de reserva nao encontrado!\n");
            }else if(i == -1 && opc == '2'){
                printf("Reserva com o nome do cliente nao encontrado!\n");
            }
        }else{
            printf("\nSaindo...");
        }
        limparTela();

    }while(opc != '0');
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
        break;
        case '2':
            for(i=0;i<n;i++){
                if(strcmp((financeiro+i)->cpfCliente,nome)==0){
                    found=1;
                    break;
                }
            }                
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

/*
Okay..., só funciona se achar o codigo ou o nome do cliente, e o status de pagamento que esteja pendente retorna 1,
se as condicoes nao forem aceita, retorna -1
*/

int consultarDadosRes(struct Reserva *reserva,int opc,char *nome){
    int n = quantidade_Reservas(reserva) - 1; 
    int found=0,i;

    switch(opc){
        case '1':
            for(i=0;i < n;i++){
                if(strcmp((reserva+i)->codigoReserva,nome) == 0){
                    found=1;
                    break;
                }
            }
        break;
        case '2':

                for(i=0;i < n;i++){
                    if(strcmp((reserva+i)->cpfCliente,nome) == 0){
                        found=1;
                        break;
                    }
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
    //J para financeiro,i para reservas, q para quartos
    int i,j,q;
    do{
        
        printf("\nComo voce deseja procurar a reserva para fazer o pagamento:\n");
        printf("1-Codigo de reserva\n2-CPF do cliente\n0-Sair\n");
        opc = recebeUmNumero(opc);

        if(opc != '0'){            

            if(opc == '1'){
                printf("Digite o Codigo de Reserva:");
                verifica_n_int(nome,0);

                i = consultarDadosRes(reserva,opc,nome);
                j = consultarDadosPag(financeiro,opc,nome);
            }else if(opc == '2'){
                printf("Digite o CPF do Cliente:");
                verifica_n_int(nome,1);

                i = consultarDadosRes(reserva,opc,nome);
                j = consultarDadosPag(financeiro,opc,nome);
            }
            
            if(strcmp((reserva+i)->statusPagamento,"Pendente")==0  || strcmp((financeiro+j)->statusPagamento,"Pendente") == 0){
                printf("\nPor favor faca o Check-In antes de Realizar o Pagamento");
            }else if(strcmp((reserva+i)->statusPagamento,"Pago")==0  || strcmp((financeiro+j)->statusPagamento,"Pago") == 0){
                printf("\nEsta reserva foi Concluida!");
            }else if((i == -1 || j == -1) && opc == 1){
                printf("\nNao foram encontrados nenhuma reserva com este codigo para pagamento!\n");
            }else if((i == -1 || j == -1) && opc == 2){
                printf("\nNao foram encontrados nenhuma reserva com este CPF para pagamento!\n");
            }else{
                printf("\nO valor a ser pago eh R$%s:", (financeiro+j)->valorT);
                
                limparTela();

                for(q=0;q < nQ ;q++){
                    if(strcmp((reserva+i)->numeroQuarto,(quarto+q)->numero)==0){
                        printf("\n---------%s--------\n", (quarto+q)->numero);
                        strcpy((quarto+q)->status,"Livre");
                        break;
                    }
                }

                resgataDados(reserva,financeiro,i,j);

                strcpy((reserva+i)->statusPagamento,"Pago");
                strcpy((financeiro+j)->statusPagamento,"Pago");
                strcpy((financeiro+j)->numeroQuarto,(quarto+q)->numero);

                dHA = localtime(&segundos);
                sprintf((financeiro+j)->horaSaida.horas,"%d",dHA->tm_hour);
                sprintf((financeiro+j)->horaSaida.min,"%d",dHA->tm_min);
                sprintf((financeiro+j)->horaSaida.segundos,"%d",dHA->tm_sec);

                
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

void mostrarValores(struct Financeiro *financeiro,int i){
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

/*
Fazer valores recebidos
*/
void valores_recebidos(struct Financeiro *financeiro){
    int n = quantidadePagamentos(financeiro) - 1;
    char opc;
    char dia[3],mes[3],ano[5];
    int data1,data2,dataC,found=0;
    
    do{
        printf("----Menu Valores Recebidos----\n");
        printf("1 - Todos\n2 - Intervalo de Tempo\n0 - Voltar\n");
        printf("------------------------------\n");
        opc = recebeUmNumero(opc);
        /*Uaual*/
        switch (opc)
        {
        case '1':
            mostrarValores(financeiro,-1);
        break;
        case '2':
            printf("\nDigite o inicio do intervalo da data:");
            recebeData(dia,mes,ano);
            data1 = dataJuliana(atoi(dia),atoi(mes),atoi(ano));
            printf("\nDigite o fim do intervalo da data:");
            recebeData(dia,mes,ano);
            data2 = dataJuliana(atoi(dia),atoi(mes),atoi(ano));

            for(int i=0;i<n;i++){
                dataC = dataJuliana(atoi((financeiro+i)->dataSaida.dia),atoi((financeiro+i)->dataSaida.mes),atoi((financeiro+i)->dataSaida.ano));
                if((dataC >= data1 && dataC <= data2)){
                    mostrarValores(financeiro,i);
                    found = 1;
                }
                if(!found)
                    printf("Nao ha valores recebidos durante o intervalo dado!");
            }
        break;
        case '0':
            printf("Voltando...");
        break;
        default:
            printf("Opcao Invalida");
            break;
        }
        limparTela();
    }while(opc != '0');
}
