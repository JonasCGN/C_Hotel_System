#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int cadastrar_quarto(struct Quarto *quarto,int n);
int quantidadeQuartos(struct Quarto *quarto);
void menu_consultar_quarto(struct Quarto *quarto,int n);
void mostrar_quarto(struct Quarto *quarto,int i);
void consultar_quarto(struct Quarto *quarto,int n,char opc);
void cadastrarRegistradoQuartos(struct Quarto *quarto,int n);
void menu_editarQuarto(struct Quarto *quarto,int n);
void editar_quarto(struct Quarto *quarto,int n,int i,int opc);
void excluir_quarto(struct Quarto *quarto,int n);


int cadastrar_quarto(struct Quarto *quarto,int n) {
    
    FILE *arq;
    arq = fopen("../database/arquivo1.bin", "a+b");
    int qtdQ = 0,i;
    char qtdQ_char[3],opc;

    printf("-------------------------------------------\n");
    printf("Quantos quartos voce deseja cadastrar:");
    verifica_n_int(qtdQ_char,0);
    printf("\n-----------------------------------------\n");
    qtdQ = atoi(qtdQ_char);

    n += qtdQ;
    i = (n - qtdQ);

    while(i < n){
        printf("Informe o numero do seu quarto: ");
        recebeNumeroQuarto((quarto+i)->numero);

        strcpy((quarto+i)->status, "Livre");

        printf("\nDigite o tipo de quarto do %s: ", (quarto+i)->numero);
        verifica_letra((quarto+i)->tipo,2);

        printf("\nDigite o valor da diaria para o quarto %s: ", (quarto+i)->numero);
        verifica_n_float((quarto+i)->valorDiaria);

        fwrite((quarto+i)->numero,sizeof(char),sizeof((quarto+i)->numero),arq);
        fwrite((quarto+i)->status,sizeof(char),sizeof((quarto+i)->status),arq);
        fwrite((quarto+i)->tipo,sizeof(char),sizeof((quarto+i)->tipo),arq);
        fwrite((quarto+i)->valorDiaria,sizeof(char),sizeof((quarto+i)->valorDiaria),arq);
        
        printf("\nQuarto Cadastrado Com Sucesso!!\n");

        i++;

        quarto = (struct Quarto *)realloc(quarto,i*sizeof(struct Cliente));

        if(i != n){
            printf("Ainda deseja continuar cadastrando(S/N):");
            opc = getche();
            if(opc == 'N' || opc == 'n'){
                printf("Saindo...");
                break;
            }
        }

        limparTela();

    }

    fclose(arq);

    return i;
}

int quantidadeQuartos(struct Quarto *quarto){
    int i=0;

    FILE *arq;
    arq = fopen("../database/arquivo1.bin","rb");

    while(!feof(arq)){
        fread((quarto+i)->numero,sizeof(char),sizeof((quarto+i)->numero),arq);
        fread((quarto+i)->status,sizeof(char),sizeof((quarto+i)->status),arq);
        fread((quarto+i)->tipo,sizeof(char),sizeof((quarto+i)->tipo),arq);
        fread((quarto+i)->valorDiaria,sizeof(char),sizeof((quarto+i)->valorDiaria),arq);
        i++;
    }
    fclose(arq);

    return i;
}

void menu_consultar_quarto(struct Quarto *quarto,int n){
    char opc;

    mostrar_quarto(quarto,-1);

    do{
        printf("\nComo voce deseja procurar o cliente\n");
        printf("1-Numero\n2-Status\n3-Tipo\n4-Valor Diaria\n0-Sair\n");
        opc = recebeUmNumero(opc);

    
        limparTela();

        if(opc != '0'){
            consultar_quarto(quarto,n,opc);
            
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
}

void mostrar_quarto(struct Quarto *quarto,int i){
    int n = quantidadeQuartos(quarto) - 1;
    if(i == -1){
        for(i=0;i<n;i++){
            printf("\n-----------Quarto %d-----------\n",i+1);
            printf("Numero do quarto:%s\nStatus do quarto:%s\nTipo do quarto:%s\nValor diaria do quarto:%s\n",(quarto+i)->numero,(quarto+i)->status,(quarto+i)->tipo,(quarto+i)->valorDiaria);
            printf("-------------------------------\n"); 
        }
    }else{
        printf("\n-----------Quarto %d-----------\n",i+1);
        printf("Numero do quarto:%s\nStatus do quarto:%s\nTipo do quarto:%s\nValor diaria do quarto:%s\n",(quarto+i)->numero,(quarto+i)->status,(quarto+i)->tipo,(quarto+i)->valorDiaria);
        printf("-------------------------------\n"); 
    }
}

void consultar_quarto(struct Quarto *quarto,int n,char opc){
    int found=0;
    char nome[10];

    switch(opc){
        case '1':
            printf("Digite o numero do quarto que voce deseja ver:");
            recebeNumeroQuarto(nome);

            for(int i=0;i<n;i++){
                if(strcmp((quarto+i)->numero,nome)==0){
                    mostrar_quarto(quarto,i);
                    found=1;
                    break;
                }
            }
            if(!found)
                printf("\nNao foram encontrados nenhum quarto com este numero!\n");
            
        break;
        case '2':
            printf("Digite o status do quarto que voce deseja ver:");
            verifica_letra(nome,0);

            for(int i=0;i<n;i++){
                if(strcmp((quarto+i)->status,nome)==0){
                    mostrar_quarto(quarto,i);
                    found=1;
                }
            }
            if(!found)
                printf("\nNao foram encontrados nenhum quarto com este status!\n");
        break;
        case '3':
            printf("Digite o tipo do quarto que voce deseja ver:");
            verifica_letra(nome,0);

            for(int i=0;i<n;i++){
                if(strcmp((quarto+i)->tipo,nome)==0){
                    mostrar_quarto(quarto,i);
                    found=1;
                }
            }
            if(!found)
                printf("\nNao foram encontrados nenhum quarto com este tipo!\n");
        break;
        case '4':
            printf("Digite o valor da diaria do quarto que voce deseja ver:");
            verifica_n_float(nome);

            for(int i=0;i<n;i++){
                if(strcmp((quarto+i)->valorDiaria,nome)==0){
                    mostrar_quarto(quarto,i);
                    found=1;
                }
            }
            if(!found)
                printf("\nNao foram encontrados nenhum quarto com este valor de diaria!\n");
        break;
        case '0':
            printf("\nSaindo...");
        break;
    }
}

void cadastrarRegistradoQuartos(struct Quarto *quarto,int n){
    FILE *arq;
    arq = fopen("../database/arquivo1.bin","wb");

    for(int i=0;i<n;i++){
        if(!(strcmp((quarto+i)->numero,"") == 0)){
            fwrite((quarto+i)->numero,sizeof(char),sizeof((quarto+i)->numero),arq);
            fwrite((quarto+i)->status,sizeof(char),sizeof((quarto+i)->status),arq);
            fwrite((quarto+i)->tipo,sizeof(char),sizeof((quarto+i)->tipo),arq);
            fwrite((quarto+i)->valorDiaria,sizeof(char),sizeof((quarto+i)->valorDiaria),arq);
        }
    }

    fclose(arq);
}

void menu_editarQuarto(struct Quarto *quarto,int n){
    char opc,num[3];

    int i,found = 0;

    mostrar_quarto(quarto,-1);

    do{
        printf("O que voce deseja editar:");
        printf("\n1-Tipo\n2-Valor da Diaria\n3-Alterar Tudo\n0-Sair\n");
        opc = recebeUmNumero(opc);

        
        limparTela();

        if(opc != '0'){
            printf("\n-----------------------------------------------------\n");
            printf("Por favor, digite o numero do quarto que deseja editar:\n");
            printf("-------------------------------------------------------\n");
            verifica_n_int(num,0);
            
            limparTela();

            for(i=0;i<n;i++){
                if(strcmp((quarto+i)->numero,num)==0){
                    mostrar_quarto(quarto,i);
                    found=1;
                    break;
                }
            }
            if(!found)
                printf("\nNao foram encontrados nenhum quarto com este numero!\n");

            editar_quarto(quarto,n,i,opc);

            limparTela();
            printf("Ainda deseja continuar editando(S/N):");
            opc = getche();
            printf("\n");
            if(opc == 'N' || opc == 'n'){
                printf("Saindo...");
                break;
            }
        }else{
            printf("Saindo...");
        }

        limparTela();
    }while(opc != '0');
}

void editar_quarto(struct Quarto *quarto,int n,int i,int opc){
    FILE *arq;
    arq = fopen("../database/arquivo1.bin","r+b");
    
    switch(opc){
        case '1':
            printf("Digita o novo tipo do quarto %s:", (quarto+i)->numero);
            verifica_letra((quarto+i)->tipo,2);

            cadastrarRegistradoQuartos(quarto,n);
        break;
        case '2':
            printf("Digita o novo valor da diaria do quarto %s:",(quarto+i)->numero);
            verifica_n_float((quarto+i)->valorDiaria);

            cadastrarRegistradoQuartos(quarto,n); 
        break;
        case '3':
            printf("Digita o novo tipo do quarto %s:", (quarto+i)->numero);
            verifica_n_int((quarto+i)->tipo,2);
            printf("\n");
            printf("Digita o novo valor da diaria do quarto %s:",(quarto+i)->numero);
            verifica_n_int((quarto+i)->valorDiaria,3);
            printf("\n");

            cadastrarRegistradoQuartos(quarto,n); 
        break;
        case '0':
            printf("Saindo...");
        break;
        default:
            printf("Opcao Invalida!!\n");
    }
        printf("\n\n-----Dados Editados com Sucesso!-----\n");

    mostrar_quarto(quarto,i);

    fclose(arq);    
}

void excluir_quarto(struct Quarto *quarto,int n){
    int i,found=0;
    char opc,num[3];
    
    mostrar_quarto(quarto,-1);

    while(1){
        printf("\n--------------------------------------------------------\n");
        printf("Por favor, digite o numero do quarto que deseja editar:\n");
        printf("----------------------------------------------------------\n");
        verifica_n_int(num,0);

        for(i=0;i<n;i++){
            if(strcmp((quarto+i)->numero,num)==0){
                mostrar_quarto(quarto,i);
                found=1;
                break;
            }
        }
        if(!found)
            printf("\nNao foram encontrados nenhum quarto com este numero!\n");

        strcpy((quarto+i)->numero,"");
        
        cadastrarRegistradoQuartos(quarto,n);
        
        quarto = (struct Quarto *)realloc(quarto,(n-1)*sizeof(struct Cliente));

        printf("\nQuarto Removido com Sucesso!!\n");

        printf("Ainda deseja continuar removendo(S/N):");
        opc = getche();
            if(opc == 'N' || opc == 'n'){
                printf("Saindo...");
                break;
        }
    
        limparTela();
    }
}