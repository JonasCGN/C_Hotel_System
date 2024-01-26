#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<ctype.h>

int cadastrar_cliente(struct Cliente *cliente,int n);
int quantidadeCliente(struct Cliente *cliente);
int consultar_cliente(struct Cliente *cliente,int n,char opc);
void menuConsultar(struct Cliente *cliente,int n);
void editar_cliente(struct Cliente *cliente,int n,int i,int opc);
void excluir_cliente(struct Cliente *cliente,int n);
void menueditar(struct Cliente *cliente,int n);
void cadastrar_registrados(struct Cliente *cliente,int n);


/* armazenando informações como nome, CPF, RG, telefone, endereço e e-mail*/
int cadastrar_cliente(struct Cliente *cliente,int n){
    FILE *arq;
    arq = fopen("../database/arquivo2.bin","a+b");
    
    int qtdC = 0,i;
    char qtdC_char[3],opc;
    
    printf("-----------------------------------------\n");
    printf("Quantos clientes voce deseja cadastrar:");
    verifica_n_int(qtdC_char,0);
    printf("\n-----------------------------------------\n");
    qtdC = atoi(qtdC_char);

    n += qtdC;

    i = (n - qtdC);

    while(i < n){
    /*Lembrar de ajeitar isso*/
        printf("Digite o nome do cliente %d:",i+1);
        verifica_letra((cliente+i)->nCT,1);
        printf("\n");
        printf("Digite o CPF do cliente %d:",i+1);
        verifica_n_int((cliente+i)->cpf,1);
        printf("\n");
        printf("Digite o RG do cliente %d:",i+1);
        verifica_n_int((cliente+i)->rg,2);
        printf("\n");
        printf("Digite o telefone do cliente %d:",i+1);
        verifica_n_int((cliente+i)->tel,3);
        printf("\n");
        printf("Digite o endereco do cliente %d:", i+1);
        gets((cliente+i)->endereco);
        fflush(stdin);
        printf("Digite o endereco de e-mail do cliente %d:",i+1);
        do{
            gets((cliente+i)->email);
            if(verifica_email((cliente+i)->email) == 0)
                printf("Error, nao coloque espaço no email:");
        }while(verifica_email((cliente+i)->email) == 0);
        
        fflush(stdin);
        
        fwrite((cliente+i)->nCT,sizeof(char),sizeof((cliente+i)->nCT),arq);
        fwrite((cliente+i)->cpf,sizeof(char),sizeof((cliente+i)->cpf),arq);
        fwrite((cliente+i)->rg,sizeof(char),sizeof((cliente+i)->rg),arq);
        fwrite((cliente+i)->tel,sizeof(char),sizeof((cliente+i)->tel),arq);
        fwrite((cliente+i)->endereco,sizeof(char),sizeof((cliente+i)->endereco),arq);
        fwrite((cliente+i)->email,sizeof(char),sizeof((cliente+i)->email),arq);
        
        printf("\nCliente Cadastrado Com Sucesso!!\n");

        i++;
        
        cliente = (struct Cliente *)realloc(cliente,i*sizeof(struct Cliente));
        
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

/*Retorna a quantidade de cLiente ja cadastrado*/
int quantidadeCliente(struct Cliente *cliente){

    int i=0;

    FILE *arq;
    arq = fopen("../database/arquivo2.bin","rb");

    while(!feof(arq)){
        fread((cliente+i)->nCT,sizeof(char),sizeof((cliente+i)->nCT),arq);
        fread((cliente+i)->cpf,sizeof(char),sizeof((cliente+i)->cpf),arq);
        fread((cliente+i)->rg,sizeof(char),sizeof((cliente+i)->rg),arq);
        fread((cliente+i)->tel,sizeof(char),sizeof((cliente+i)->tel),arq);
        fread((cliente+i)->endereco,sizeof(char),sizeof((cliente+i)->endereco),arq);
        fread((cliente+i)->email,sizeof(char),sizeof((cliente+i)->email),arq);

        i++;
    }
    fclose(arq);
    
    return i;
}

void mostrar_clientes(struct Cliente *cliente, int i){
    int n = quantidadeCliente(cliente) - 1;
    if(i == -1){
        for(i=0;i<n;i++){
            printf("\n------------Cliente %d------------\nNome:%s\nCPF:%s\nRG:%s\nTelefone:%s\nEndereco:%s\nEmail:%s\n---------------------------------\n", i+1,(cliente+i)->nCT,(cliente+i)->cpf,(cliente+i)->rg,(cliente+i)->tel,(cliente+i)->endereco,(cliente+i)->email);
        }
    }else{
        printf("\n------------Cliente %d------------\nNome:%s\nCPF:%s\nRG:%s\nTelefone:%s\nEndereco:%s\nEmail:%s\n---------------------------------\n", i+1,(cliente+i)->nCT,(cliente+i)->cpf,(cliente+i)->rg,(cliente+i)->tel,(cliente+i)->endereco,(cliente+i)->email);
    }
}

void menuConsultar(struct Cliente *cliente,int n){
    char opc;

    do{

        mostrar_clientes(cliente,-1);

        printf("\n------------------------------------------\n");
        printf("Como voce deseja procurar o cliente\n");
        printf("1-Nome\n2-CPF\n3-RG\n4-Telefone\n5-Endereco\n6-Email\n0-Sair\n");
        printf("\n------------------------------------------\n");
        opc = recebeUmNumero(opc);


        limparTela();

        consultar_cliente(cliente,n,opc);

    }while(opc != '0');
}

int consultar_cliente(struct Cliente *cliente,int n,char opc){
    char nome[50];
    int i,found = 0;
    switch(opc){
        case '1':
            printf("Qual o nome do cliente que voce deseja ver:");
            verifica_letra(nome,0);

            for(i=0;i<n;i++){
                if(strcmp(strtok((cliente+i)->nCT,"\n"),nome)==0){
                    mostrar_clientes(cliente,i);
                    found = 1;
                    break;
                }
            }
            if(!found)
                printf("\nNao foram encontrados nenhum cliente com este nome!\n");
        break;
        case '2':
            printf("Qual o CPF do cliente que voce deseja ver:");
            verifica_n_int(nome,1);


            for(i=0;i<n;i++){
                if(strcmp(strtok((cliente+i)->cpf,"\n"),nome)==0){
                    mostrar_clientes(cliente,i);                    
                    found = 1;
                    break;
                }
            }
            if(!found)
                printf("\nNao foram encontrados nenhum cliente com este CPF!\n");
        break;
        case '3':
            printf("Qual o RG do cliente que voce deseja ver:");
            verifica_n_int(nome,2);


            for(i=0;i<n;i++){
                if(strcmp(strtok((cliente+i)->rg,"\n"),nome)==0){
                    mostrar_clientes(cliente,i);                    
                    found = 1;
                    break;
                }
            }
            if(!found)
                printf("\nNao foram encontrados nenhum cliente com este RG!\n");
        break;
        case '4':
            printf("Qual o Numero do cliente que voce deseja ver:");
            verifica_n_int(nome,3);

            for(i=0;i<n;i++){
                if(strcmp(strtok((cliente+i)->tel,"\n"),nome)==0){
                    mostrar_clientes(cliente,i);                    
                    found = 1;
                    break;
                }
            }
            if(!found)
                printf("\nNao foram encontrados nenhum cliente com este telefone!\n");
        break;
        case '5':
            printf("Qual o Endereco do cliente que voce deseja ver:");
            gets(nome);

            for(i=0;i<n;i++){
                if(strcmp(strtok((cliente+i)->endereco,"\n"),nome)==0){
                    mostrar_clientes(cliente,i);                    
                    found = 1;                    
                    break;
                }
            }
            if(!found)
                printf("\nNao foram encontrados nenhum cliente com este endereco!\n");
        break;
        case '6':
            printf("Qual o Email do cliente que voce deseja ver:");
            gets(nome);

            for(i=0;i<n;i++){
                if(strcmp(strtok((cliente+i)->email,"\n"),nome)==0){
                    mostrar_clientes(cliente,i);                    
                    found = 1;                    
                    break;
                }
            }
            if(!found)
                printf("\nNao foram encontrados nenhum cliente com este email!\n");
        break;
        case '0':
            printf("Saindo...");
        break;
        default:
            printf("Opcao Invalida!!\n");
    }

    return i;
       
}

void menueditar(struct Cliente *cliente,int n){
    char opc,opc2;

    int i;

    mostrar_clientes(cliente,-1);

    do{
        printf("O que voce deseja editar:");
        printf("\n1-Nome\n2-CPF\n3-RG\n4-Telefone\n5-Endereco\n6-Email\n7-Alterar Tudo\n0-Sair\n");
        opc = recebeUmNumero(opc);

        
        limparTela();
        if(opc != '0'){
            printf("\n-----------------------------------------\n");
            printf("Como voce deseja procurar o cliente\n");
            printf("1-Nome\n2-CPF\n3-RG\n4-Telefone\n5-Endereco\n6-Email\n0-Sair\n");
            printf("-----------------------------------------\n");
            opc2 = recebeUmNumero(opc2);

            
            limparTela();

            if(opc2 != '0'){
                i = consultar_cliente(cliente,n,opc2);
                

                editar_cliente(cliente,n,i,opc);

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
        }else{
            printf("Saindo...");
        }
    }while(opc != '0');

}

void cadastrar_registrados(struct Cliente *cliente,int n){
    FILE *arq;
    arq = fopen("../database/arquivo2.bin","wb");

    for(int i=0;i<n;i++){
        if(!(strcmp((cliente+i)->nCT,"") == 0)){
            fwrite((cliente+i)->nCT,sizeof(char),sizeof((cliente+i)->nCT),arq);
            fwrite((cliente+i)->cpf,sizeof(char),sizeof((cliente+i)->cpf),arq);
            fwrite((cliente+i)->rg,sizeof(char),sizeof((cliente+i)->rg),arq);
            fwrite((cliente+i)->tel,sizeof(char),sizeof((cliente+i)->tel),arq);
            fwrite((cliente+i)->endereco,sizeof(char),sizeof((cliente+i)->endereco),arq);
            fwrite((cliente+i)->email,sizeof(char),sizeof((cliente+i)->email),arq);
        }
    }

    fclose(arq);
}

void editar_cliente(struct Cliente *cliente,int n,int i,int opc){
    FILE *arq;
    arq = fopen("../database/arquivo2.bin","r+b");
    
    switch(opc){
        case '1':
            printf("Digita o novo Nome da pessoa para altera-lo:");
            verifica_letra((cliente+i)->nCT,1);
            

            cadastrar_registrados(cliente,n);                
        break;  
        case '2':

            printf("Digita o novo CPF da pessoa para altera-lo:");
            verifica_n_int((cliente+i)->cpf,1);

            cadastrar_registrados(cliente,n);                

        break;
        case '3':
            printf("Digita o RG da pessoa  para altera-lo:");
            verifica_n_int((cliente+i)->rg,2);

            cadastrar_registrados(cliente,n); 
        break;
        case '4':
            printf("Digita o Telefone da pessoa  para altera-lo:");
            verifica_n_int((cliente+i)->tel,3);

            cadastrar_registrados(cliente,n); 
        break;
        case '5':
            printf("Digita o Endereco da pessoa  para altera-lo:");
            gets((cliente+i)->endereco);

            cadastrar_registrados(cliente,n); 
        break;
        case '6':
            printf("Digita o Email da pessoa  para altera-lo:");
            gets((cliente+i)->email);

            cadastrar_registrados(cliente,n); 
        break;
        case '7':
            printf("Digite o novo nome:");
            verifica_letra((cliente+i)->nCT,1);
            printf("\n");
            printf("Digite o novo CPF:");
            verifica_n_int((cliente+i)->cpf,1);
            printf("\n");
            printf("Digite o novo RG:");
            verifica_n_int((cliente+i)->rg,2);
            printf("\n");
            printf("Digite o novo telefone:");
            verifica_n_int((cliente+i)->tel,3);
            printf("\n");
            printf("Digite o novo endereco:");
            gets((cliente+i)->endereco);
            fflush(stdin);
            printf("Digite o novo e-mail:");
            gets((cliente+i)->email);
            fflush(stdin);

            cadastrar_registrados(cliente,n); 
        break;
        case '0':
            printf("Saindo...");
        break;
        default:
            printf("Opcao Invalida!!\n");
    }
        printf("\n\n-----Dados Editados com Sucesso!-----\n");

    mostrar_clientes(cliente,i);

    fclose(arq);    
    
}

void excluir_cliente(struct Cliente *cliente,int n){
    int i;
    char opc2,opc;
    
    mostrar_clientes(cliente,-1);

    do{
        printf("Como voce deseja procurar o cliente para remove-lo:\n");
        printf("1-Nome\n2-CPF\n3-RG\n4-Telefone\n5-Endereco\n6-Email\n0-Sair\n");
        opc2 = recebeUmNumero(opc2);

        printf("\n");

        limparTela();
        
        if(opc2 != '0'){

            i = consultar_cliente(cliente,n,opc2);

            strcpy((cliente+i)->nCT,"");
            
            cadastrar_registrados(cliente,n);
            
            cliente = (struct Cliente *)realloc(cliente,(n-1)*sizeof(struct Cliente));

            printf("\nCliente Removido com Sucesso!!\n");

            printf("Ainda deseja continuar removendo(S/N):");
            opc = getche();
                if(opc == 'N' || opc == 'n'){
                    printf("Saindo...");
                    break;
            }
        }else{
            printf("Saindo...");
        }
    }while(opc2 != '0');
}