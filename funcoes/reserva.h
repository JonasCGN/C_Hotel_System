#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<conio.h>
#include<time.h>

int realizar_reserva(struct Cliente *cliente,struct Quarto *quarto,struct Reserva *reserva,int nR);
void menu_consultarReservas(struct Reserva *reserva,int nR);
int consultar_reservas(struct Reserva *reserva,int n,int opc);
int quantidade_Reservas(struct Reserva *reserva);
void excluir_reserva(struct Reserva *reserva,int n);
int consultarDadosRes1(struct Reserva *reserva,int opc);

/*
Realizar reserva, Excluir Reserva, Realizar check-in, Realizar pagamento, Consultar 
reservas e Valores recebidos
*/
/*
Permitir a reserva de quartos, verificando disponibilidade e 
registrando dados como nome do cliente, número do quarto, datas, status de 
pagamento e valor total da reserva. Caso o quarto solicitado pelo cliente esteja 
reservado, uma mensagem de erro deve ser mostrada e, em seguida, uma lista 
quartos disponíveis devem ser exibidos. Se o cliente escolher outro quarto, a 
função deve ser reiniciada e concluída, produzindo um código automáƟco de 
reserva. 
*/

int realizar_reserva(struct Cliente *cliente,struct Quarto *quarto,struct Reserva *reserva,int nR){
    srand(time(NULL));

    FILE *arq=fopen("../database/arquivo4.bin","a+b");

    int nC = quantidadeCliente(cliente) - 1;
    int nQ = quantidadeQuartos(quarto) - 1;
    
    int i=0,j=0,z=0,deE,dsS,deE2=0,dsS2=0,ger=1000;
    char num[4],opc,nome[50];

    mostrar_clientes(cliente,-1);
    mostrar_quarto(quarto,-1);
    
    int qtdQ = 0;
    char qtdQ_char[3];

    printf("-------------------------------------------\n");
    printf("Quantos quartos voce deseja reservar:");
    verifica_n_int(qtdQ_char,0);
    printf("\n-----------------------------------------\n");
    qtdQ = atoi(qtdQ_char);

    nR += qtdQ;
    i = (nR - qtdQ);

    while(i < nR){


        int erro = 0,quartoLivre=0,found = 0,foundC=0;

        printf("Digite o nome do cliente:");
        verifica_letra(nome,1);

        for(z=0;z < nC;z++){
            if(strcmp((cliente+z)->nCT,nome) == 0){
                foundC = 1;
                break;
            }
        }

        if(!foundC){
            printf("\nVoce ainda nao possui cadastro, faca o cadastro depois volte!");
            break;
        }else{

            printf("\nDigite o numero do quarto para reserva-lo:");
            recebeNumeroQuarto(num);
            
            for(j=0;j < nQ;j++){
                if(strcmp((quarto+j)->numero,num) == 0){
                    found = 1;
                    break;
                }
            
            }

            if(!found)
                printf("O quarto com o respectivo numero, nao existe!");
            else{

                if(i > 0){
                    for(int a=0;a<i;a++){
                        if(strcmp((reserva+a)->numeroQuarto,num) == 0){
                            deE2 = dataAtoi((reserva+a)->dataEntrada.dia,(reserva+a)->dataEntrada.mes,(reserva+a)->dataEntrada.ano);;
                            dsS2 = dataAtoi((reserva+a)->dataSaida.dia,(reserva+a)->dataSaida.mes,(reserva+a)->dataSaida.ano);;
                        }
                    }
                }
                do{
                    printf("\nInforme a data de entrada que voce deseja reservar o quarto %s:", num);
                    printf("\nInsira os dados no formato DD/MM/AAAA:");
                    recebeData((reserva+i)->dataEntrada.dia,(reserva+i)->dataEntrada.mes,(reserva+i)->dataEntrada.ano);

                    printf("\nInforme a data de saida que voce deseja reservar o quarto %s:", num);
                    printf("\nInsira os dados no formato DD/MM/AAAA:");
                    recebeData((reserva+i)->dataSaida.dia,(reserva+i)->dataSaida.mes,(reserva+i)->dataSaida.ano);

                    deE = dataAtoi((reserva+i)->dataEntrada.dia,(reserva+i)->dataEntrada.mes,(reserva+i)->dataEntrada.ano);
                    dsS = dataAtoi((reserva+i)->dataSaida.dia,(reserva+i)->dataSaida.mes,(reserva+i)->dataSaida.ano);

                    if(deE > dsS)
                        printf("\nPor favor insira a data de entrada menor que a de saida\n");
                    
                }while(deE > dsS);

                // printf("\nData Entrada Depois %d - Data Saida Depois %d\n",deE,dsS);
                // printf("Data Entrada Antes %d - Data Saida Antes %d\n",deE2,dsS2);

                if(!(strcmp((quarto+j)->status,"Livre")==0)){
                    if(verificaEntreData(deE,dsS,deE2,dsS2) == 0){
                        erro = 1;
                        printf("\nError, o seguinte quarto ja esta reservado nesta data!!\n");
                        limparTela();
                    }
                }

                if(erro){
                    for(int qL=0;qL<nQ;qL++){
                        if(strcmp((quarto+qL)->status,"Livre")==0){
                            mostrar_quarto(quarto,qL);
                            quartoLivre = 1;
                        }
                    }
                    if(!quartoLivre){
                        printf("\nNao ha mais quartos disponiveis!");
                        break;
                    }else{
                        printf("Deseja escolher outro quarto(S/N):");
                        opc = getche();
                        
                        if(opc == 'N' || opc == 'n'){
                            printf("Saindo...");
                            break;
                        }else if(opc == 'S' || opc == 's'){
                            realizar_reserva(cliente,quarto,reserva,nR);
                        }
                    }
                }
                ger += (rand() % 1000) + 10000;
                
                sprintf((reserva+i)->codigoReserva,"%d",ger);
                
                valorDiaria((reserva+i)->valorT,(quarto+j)->valorDiaria, (dsS == deE) ? 1 : (dsS - deE));
                strcpy((reserva+i)->statusPagamento,"Pendente");
                strcpy((quarto+j)->status,"Reservado");

                fwrite((reserva+i)->codigoReserva,sizeof(char),sizeof((reserva+i)->codigoReserva),arq);
                fwrite((quarto+j)->numero,sizeof(char),sizeof((quarto+j)->numero),arq);
                fwrite((cliente+z)->nCT,sizeof(char),sizeof((cliente+z)->nCT),arq);
                fwrite((cliente+z)->cpf,sizeof(char),sizeof((cliente+z)->cpf),arq);
                fwrite((reserva+i)->dataEntrada.dia,sizeof(char),sizeof((reserva+i)->dataEntrada.dia),arq);
                fwrite((reserva+i)->dataEntrada.mes,sizeof(char),sizeof((reserva+i)->dataEntrada.mes),arq);
                fwrite((reserva+i)->dataEntrada.ano,sizeof(char),sizeof((reserva+i)->dataEntrada.ano),arq);
                fwrite((reserva+i)->dataSaida.dia,sizeof(char),sizeof((reserva+i)->dataSaida.dia),arq);
                fwrite((reserva+i)->dataSaida.mes,sizeof(char),sizeof((reserva+i)->dataSaida.mes),arq);
                fwrite((reserva+i)->dataSaida.ano,sizeof(char),sizeof((reserva+i)->dataEntrada.ano),arq);
                fwrite((reserva+i)->valorT,sizeof(char),sizeof((reserva+i)->valorT),arq);
                fwrite((reserva+i)->statusPagamento,sizeof(char),sizeof((reserva+i)->statusPagamento),arq);

                cadastrarRegistradoQuartos(quarto,nQ);

                printf("Seu quarto esta pronto para reserva, essas sao a informacoes dele:\n");

                if(found){
                    mostrar_quarto(quarto,j);
                }
                i++;
                
                reserva = (struct Reserva *)realloc(reserva,i*sizeof(struct Reserva));

                limparTela();

            }
        }
    }
    limparTela();

    fclose(arq);
    return i;
}

void mostrarReservas(struct Reserva *reserva,int i){
    int n = quantidade_Reservas(reserva) - 1;
    if(i == -1){
        for(i=0;i<n;i++){
            printf("\n-----------Reserva %d-----------\n",i+1);
            printf("Codigo de reserva:%s\nNumero do Quarto:%s\nNome do Cliente:%s\nCPF do cliente:%s\n",(reserva+i)->codigoReserva,(reserva+i)->numeroQuarto,(reserva+i)->nomeCliente,(reserva+i)->cpfCliente);
            printf("Data de Entrada:%s/%s/%s\n",(reserva+i)->dataEntrada.dia,(reserva+i)->dataEntrada.mes,(reserva+i)->dataEntrada.ano);
            printf("Data de Saida:%s/%s/%s\n",(reserva+i)->dataSaida.dia,(reserva+i)->dataSaida.mes,(reserva+i)->dataSaida.ano);
            printf("Valor Total:%s\nStatus do pagamento:%s\n",(reserva + i)->valorT,(reserva + i)->statusPagamento);
            printf("-------------------------------\n"); 
        }
    }else{
        printf("\n-----------Reserva %d-----------\n",i+1);
            printf("Codigo de reserva:%s\nNumero do Quarto:%s\nNome do Cliente:%s\nCPF do cliente:%s\n",(reserva+i)->codigoReserva,(reserva+i)->numeroQuarto,(reserva+i)->nomeCliente,(reserva+i)->cpfCliente);
            printf("Data de Entrada:%s/%s/%s\n",(reserva+i)->dataEntrada.dia,(reserva+i)->dataEntrada.mes,(reserva+i)->dataEntrada.ano);
            printf("Data de Saida:%s/%s/%s\n",(reserva+i)->dataSaida.dia,(reserva+i)->dataSaida.mes,(reserva+i)->dataSaida.ano);
            printf("Valor Total:%s\nStatus do pagamento:%s\n",(reserva + i)->valorT,(reserva + i)->statusPagamento);
            printf("-------------------------------\n"); 
    }
}

int quantidade_Reservas(struct Reserva *reserva){
    int i=0;

    FILE *arq;
    arq = fopen("../database/arquivo4.bin","rb");

    while(!feof(arq)){
        fread((reserva+i)->codigoReserva,sizeof(char),sizeof((reserva+i)->codigoReserva),arq);
        fread((reserva+i)->numeroQuarto,sizeof(char),sizeof((reserva+i)->numeroQuarto),arq);
        fread((reserva+i)->nomeCliente,sizeof(char),sizeof((reserva+i)->nomeCliente),arq);
        fread((reserva+i)->cpfCliente,sizeof(char),sizeof((reserva+i)->cpfCliente),arq);
        fread((reserva+i)->dataEntrada.dia,sizeof(char),sizeof((reserva+i)->dataEntrada.dia),arq);
        fread((reserva+i)->dataEntrada.mes,sizeof(char),sizeof((reserva+i)->dataEntrada.mes),arq);
        fread((reserva+i)->dataEntrada.ano,sizeof(char),sizeof((reserva+i)->dataEntrada.ano),arq);
        fread((reserva+i)->dataSaida.dia,sizeof(char),sizeof((reserva+i)->dataSaida.dia),arq);
        fread((reserva+i)->dataSaida.mes,sizeof(char),sizeof((reserva+i)->dataSaida.mes),arq);
        fread((reserva+i)->dataSaida.ano,sizeof(char),sizeof((reserva+i)->dataEntrada.ano),arq);
        fread((reserva+i)->valorT,sizeof(char),sizeof((reserva+i)->valorT),arq);
        fread((reserva+i)->statusPagamento,sizeof(char),sizeof((reserva+i)->statusPagamento),arq);
        i++;
    }

    fclose(arq);

    return i;
}

void cadastrarRegistradosReservas(struct Reserva *reserva,int n){
    FILE *arq;
    arq = fopen("../database/arquivo4.bin","wb");

    for(int i=0;i < n ;i++){
        if(!(strcmp((reserva+i)->codigoReserva,"") == 0)){
            fwrite((reserva+i)->codigoReserva,sizeof(char),sizeof((reserva+i)->codigoReserva),arq);
            fwrite((reserva+i)->numeroQuarto,sizeof(char),sizeof((reserva+i)->numeroQuarto),arq);
            fwrite((reserva+i)->nomeCliente,sizeof(char),sizeof((reserva+i)->nomeCliente),arq);
            fwrite((reserva+i)->cpfCliente,sizeof(char),sizeof((reserva+i)->cpfCliente),arq);
            fwrite((reserva+i)->dataEntrada.dia,sizeof(char),sizeof((reserva+i)->dataEntrada.dia),arq);
            fwrite((reserva+i)->dataEntrada.mes,sizeof(char),sizeof((reserva+i)->dataEntrada.mes),arq);
            fwrite((reserva+i)->dataEntrada.ano,sizeof(char),sizeof((reserva+i)->dataEntrada.ano),arq);
            fwrite((reserva+i)->dataSaida.dia,sizeof(char),sizeof((reserva+i)->dataSaida.dia),arq);
            fwrite((reserva+i)->dataSaida.mes,sizeof(char),sizeof((reserva+i)->dataSaida.mes),arq);
            fwrite((reserva+i)->dataSaida.ano,sizeof(char),sizeof((reserva+i)->dataEntrada.ano),arq);
            fwrite((reserva+i)->valorT,sizeof(char),sizeof((reserva+i)->valorT),arq);
            fwrite((reserva+i)->statusPagamento,sizeof(char),sizeof((reserva+i)->statusPagamento),arq);
        }
    }

    fclose(arq);
}

void menu_consultarReservas(struct Reserva *reserva,int nR){
    char opc;

    mostrarReservas(reserva,-1);

    do{
        printf("\nComo voce deseja procurar a reserva\n");
        printf("1-Codigo de reserva\n2-Nome do Cliente\n0-Sair\n");
        opc = recebeUmNumero(opc);

        limparTela();

        if(opc != '0'){
            consultar_reservas(reserva,nR,opc);
            
            printf("Ainda deseja continuar(S/N):");
            opc = getche();
            if(opc == 'N' || opc == 'n'){
                printf("\nSaindo...");
                break;
            }
        }else{
            printf("\nSaindo...");
        }
    }while(opc != '0');

    limparTela();
}

int consultar_reservas(struct Reserva *reserva,int n,int opc){
    int found=0,i;
    char nome[50];

    switch(opc){
        case '1':
            printf("Digite o codigo da reserva:");
            verifica_n_int(nome,0);

            for(i=0;i<n;i++){
                if(strcmp((reserva+i)->codigoReserva,nome) == 0){
                    mostrarReservas(reserva,i);
                    found=1;
                }
            }
            if(!found)
                printf("\nNao foram encontrados nenhuma reserva com este codigo!\n");
        break;
        case '2':
            printf("Digite o nome cliente para procurar reserva:");
            verifica_letra(nome,1);

            for(i=0;i<n;i++){
                if(strcmp((reserva+i)->nomeCliente,nome)==0){
                    mostrarReservas(reserva,i);
                    found=1;
                }
            }
            if(!found)
                printf("\nNao foram encontrados nenhuma reserva com este codigo!\n");
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

int consultarDadosRes1(struct Reserva *reserva,int opc){
    int n = quantidade_Reservas(reserva) - 1; 
    int found=0,i;
    char nome[50];

    switch(opc){
        case '1':
            printf("Digite o codigo da reserva:");
            verifica_n_int(nome,0);

            for(i=0;i < n;i++){
                if(strcmp((reserva+i)->codigoReserva,nome) == 0 && strcmp((reserva+i)->statusPagamento,"Pendente")==0){
                    found=1;
                    break;
                }
            }
            if(!found)
                printf("\nNao foram encontrados nenhuma reserva com este codigo!\n");
        break;

        case '2':
            printf("Digite o nome cliente para procurar reserva:");
            verifica_letra(nome,1);

            for(i=0;i < n;i++){
                printf("%s\n",(reserva+i)->statusPagamento);
                if(strcmp((reserva+i)->nomeCliente,nome) == 0 && strcmp((reserva+i)->statusPagamento,"Pendente")==0){
                    found=1;
                    break;
                }
            }
            if(!found){
                printf("\nNao foram encontrados nenhuma reserva com este Nome!\n");
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

int consultarDadosRes2(struct Reserva *reserva,int opc){
    int n = quantidade_Reservas(reserva) - 1; 
    int found=0,i;
    char nome[50];

    switch(opc){
        case '1':
            printf("Digite o codigo da reserva:");
            verifica_n_int(nome,0);

            for(i=0;i < n;i++){
                if(strcmp((reserva+i)->codigoReserva,nome) == 0 && !(strcmp((reserva+i)->statusPagamento,"Check-In")==0)){
                    found=1;
                    break;
                }
            }
            if(!found)
                printf("\nNao foram encontrados nenhuma reserva com este codigo!\n");
        break;

        case '2':
            printf("Digite o nome cliente para procurar reserva:");
            verifica_letra(nome,1);

            for(i=0;i < n;i++){
                printf("%s\n",(reserva+i)->statusPagamento);
                if(strcmp((reserva+i)->nomeCliente,nome) == 0 && !(strcmp((reserva+i)->statusPagamento,"Check-In")==0)){
                    found=1;
                    break;
                }
            }
            if(!found){
                printf("\nNao foram encontrados nenhuma reserva com este Nome!\n");
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

void excluir_reserva(struct Reserva *reserva,int n){
    int i;
    char opc2,opc;
    
    mostrarReservas(reserva,-1);


    do{
        printf("Como voce deseja procurar o cliente para remove-lo:\n");
        printf("1-Codigo de Reserva\n2-Nome do Cliente\n0-Sair\n");
        opc2 = recebeUmNumero(opc2);

        printf("\n");

        
        if(opc2 != '0'){

            i = consultarDadosRes2(reserva,opc2);

            strcpy((reserva+i)->codigoReserva,"");
            
            cadastrarRegistradosReservas(reserva,n);
            
            reserva = (struct Reserva *)realloc(reserva,(n-1)*sizeof(struct Reserva));

            printf("\nReserva Removida com Sucesso!!\n");

            printf("Ainda deseja continuar removendo(S/N):");
            opc = getche();
                if(opc == 'N' || opc == 'n'){
                    printf("Saindo...");
                    break;
            }
            limparTela();
        }else{
            printf("Saindo...");
        }
    }while(opc2 != '0');

    limparTela();
}