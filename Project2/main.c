
/*------------------------------------------------------------\
|            Autor : Catarina Sofia dos Santos Sousa          |
|                        N.93695                              |
|                Sistema de gestao de contactos               |
|                            IAED                             |
\------------------------------------------------------------*/



#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
    char comando;
    inicializar(); /* primeira funcao a ser chamada para inicializar a lista e as hashtables*/
    while (1){
        comando = getchar();
        switch(comando){
            case 'a':
                getchar();
                comando_a();
                break;
            case 'l':
                comando_l();
                break;
            case 'p':
                getchar();
                comando_p();
                break;
            case 'r':
                getchar();
                comando_r();
                break;
            case 'e':
                getchar();
                comando_e();
                break;
            case 'c':
                getchar();
                comando_c();
                break;
            case 'x':
                free_nodes_lista();
                free_dominios_tabela();
                free_contactos_tabela();
                exit(0);
                break;
        }
    }
    return 0;
}

void inicializar(){
    /*Funcao que inicializa a head (primeiro elemento) da lista e as HashTables a NULL*/
    int i;
    head = NULL;
    for(i = 0; i < MAX_HASH; i++){
        tabela_contacto[i] = NULL;
        tabela_dominio[i] = NULL;
    }
}


void free_node(struct Node *contacto_node){
    /*Funcao que faz free de um node*/
    free(contacto_node->Contacto->nome);
    free(contacto_node->Contacto->telefone);

    free(contacto_node->Email->local);
    free(contacto_node->Email->dominio);

    free(contacto_node->Contacto);
    free(contacto_node->Email);

    free(contacto_node);
}


void free_nodes_lista(){
    /*Funcao que faz free a todos os nodes existentes na lista*/
    Node *aux;
    Node *temp = head;
    while(temp != NULL){
        aux = temp;
        temp = temp->next;
        free_node(aux);
    }
}


void free_dominios_tabela(){
    /*Funcao que faz free a todos os dominios da HashTable de dominios*/
    HashTableDominio *aux;
    int i;
    for(i = 0; i < MAX_HASH; i++){
        if (tabela_dominio[i] != NULL){
            while(tabela_dominio[i] != NULL){
            aux = tabela_dominio[i]->next;
            free(tabela_dominio[i]->dominio);
            free(tabela_dominio[i]);
            tabela_dominio[i] = aux;
            }
        }
    }
}


void free_contactos_tabela(){
    /*Funcao que faz free a todos os contactos da HashTable de contactos*/
    int i;
    HashTable *aux;
    for(i = 0; i < MAX_HASH; i++){
        if (tabela_contacto[i] != NULL){
            while(tabela_contacto[i] != NULL){
            aux = tabela_contacto[i]->next;
            free(tabela_contacto[i]);
            tabela_contacto[i] = aux;
            }
        }
    }
}


int hash(char *string){
    /*Funcao hash que recebe uma string e devolve um int correspondente a essa string recebida*/
    int h, a = 31415, b = 27183;
    for(h = 0; *string != '\0'; string++, a = a*b % (MAX_HASH-1)){
        h = (a * h + *string) % MAX_HASH;
    }
    return h;
}


int existe_nome(char *nome){
    /*Funcao que recebe um char*(nome) e verifica se o nome ja existe na HashTable de 
        contactos. Devolve 0 se o nome ja existir e 1 caso contrario*/
    int indice;
    HashTable *temp;
    indice = hash(nome);
    temp = tabela_contacto[indice];
    if (temp != NULL){
        while(temp != NULL){
            if (strcmp(temp->node->Contacto->nome, nome) == 0){
                return 0;
            }
            temp = temp->next;
        }
    }
    return 1;
}


struct HashTable *existe_contacto_nome(char *nome, int posicao){
    /*Funcao que recebe um char*(nome) e um int(indice da hashtable) e devolve o elemento da 
        hashtable correspondente a esse nome*/
    if(tabela_contacto[posicao] != NULL){
        HashTable *temp = tabela_contacto[posicao];
        if (strcmp(nome, temp->node->Contacto->nome) == 0){
            return temp;
        }
        else{
            while(temp != NULL){
                if (strcmp(temp->node->Contacto->nome, nome) == 0){
                    return temp;
                }
                temp = temp->next;
            }
        }
    }
    return NULL;
}


void insere_tabela_contacto(int key, struct Node *nvcontacto){
    /*Funcao que recebe um inteiro(key) e um Node(novo contacto) e adiciona a Hashtable de 
        contactos no indice "key" */
    HashTable *node_ht_contacto = malloc(sizeof(HashTable)); 
    node_ht_contacto->node = nvcontacto;

    if(tabela_contacto[key] == NULL){
        node_ht_contacto->next = NULL;
        node_ht_contacto->prev = NULL;
        tabela_contacto[key] = node_ht_contacto;
    }
    else{
        node_ht_contacto->next = tabela_contacto[key];
        node_ht_contacto->prev = NULL;
        tabela_contacto[key]->prev = node_ht_contacto;
        tabela_contacto[key] = node_ht_contacto;
    }
}


void insere_tabela_dominio(int key, char *string){
    /*Funcao que recebe um inteiro(key) e um char*(dominio) e adiciona a Hashtable de 
        dominios no indice "key" */
    struct HashTableDominio *temp = (struct HashTableDominio*) malloc(sizeof(struct HashTableDominio));
    temp->dominio = (char *) malloc(sizeof(char) * (strlen(string) + 1));
    
    strcpy(temp->dominio, string);
    
    if(tabela_dominio[key] == NULL){
        temp->next = NULL;
        tabela_dominio[key] = temp;
    }
    else{
        temp->next = tabela_dominio[key];
        tabela_dominio[key] = temp;
    }
}


void adiciona_contacto(char *nome, char *local, char *dominio, char *telefone){
    /*Funcao que recebe 4 chars (nome, local, dominio e telefone) e adiciona a lista
        ligada e as HashTables (contactos e dominios)*/
    int keynome, keydominio;
    Node *temp;
    struct Node *nvcontacto = (struct Node*) malloc(sizeof(struct Node));

    nvcontacto->Contacto = (Contacto) malloc(sizeof(struct Contacto));
    nvcontacto->Email = (Email) malloc(sizeof(struct Email));
    nvcontacto->Contacto->nome = (char*) malloc(sizeof(char) * (strlen(nome) + 1));
    nvcontacto->Email->local = (char*) malloc(sizeof(char) * (strlen(local) + 1));
    nvcontacto->Email->dominio = (char*) malloc(sizeof(char) * (strlen(dominio) + 1));
    nvcontacto->Contacto->telefone =  (char*) malloc(sizeof(char) * (strlen(telefone) + 1));

    strcpy(nvcontacto->Contacto->nome, nome);
    strcpy(nvcontacto->Email->local, local);
    strcpy(nvcontacto->Email->dominio, dominio);
    strcpy(nvcontacto->Contacto->telefone, telefone);
    nvcontacto->next = NULL;

    if (existe_nome(nome) != 0){
        if (head == NULL){
            head = nvcontacto;
            nvcontacto->prev = NULL;
        }
        else{
            temp = head;
            while(temp->next != NULL){
                temp = temp->next;
            }
            
            temp->next = nvcontacto;
            nvcontacto->prev = temp;
        }
        keynome = hash(nome);
        insere_tabela_contacto(keynome, nvcontacto);

        keydominio = hash(dominio);
        insere_tabela_dominio(keydominio, dominio);

    }
    else{
        free_node(nvcontacto);
        printf("Nome existente.\n");
    }
    free(nome);
    free(local);
    free(dominio);
    free(telefone);
}


void apaga_contacto_lista(struct HashTable *del_contacto){
    /*Funcao que recebe um elemento da HashTable de contactos e apaga-o da lista de contactos*/
    Node *contacto_anterior;
    Node *contacto_seguinte;
    
    contacto_seguinte = del_contacto->node->next;
    contacto_anterior = del_contacto->node->prev;
    
    if (head == del_contacto->node){
        if (contacto_seguinte == NULL){
            head = NULL;
        }
        else{
            contacto_seguinte->prev = NULL;    
            head = contacto_seguinte;
        }
    }
    else{
        if (contacto_seguinte != NULL){
            contacto_seguinte->prev = contacto_anterior;
        }
        contacto_anterior->next = contacto_seguinte;
    }
    free_node(del_contacto->node);
}


void apaga_elemento_hashtable(struct HashTable *del_elemento, char *nome){
    /*Funcao que recebe um elemento da HashTable e um char* (nome do elemento) e apaga-o
    da HashTable */
    int valorhash;
    HashTable *elemento_seguinte;
    HashTable *elemento_anterior;
    valorhash = hash(nome);
    elemento_seguinte = del_elemento->next;
    elemento_anterior = del_elemento->prev;

    if (elemento_anterior == NULL){
        if (elemento_seguinte == NULL){
            tabela_contacto[valorhash] = NULL;
        }
        else{
            elemento_seguinte->prev = NULL;
            tabela_contacto[valorhash] = elemento_seguinte;
        }
    }
    else{
        elemento_anterior->next = elemento_seguinte;
    }
}




/*-------->>>FUNCAO QUE TENTEI FAZER SEM VERIFICAR A LISTA LIGADA TODA<<<--------*/
/* int conta_ocorrencias(char *dominio, int indice){
    int nr = 0;
    if (tabela_dominio[indice] == NULL){
        return nr;
    }
    else{
        HashTableDominio *temp = tabela_dominio[indice];
        while(temp != NULL){
            if (strcmp(temp->dominio, dominio) == 0){
                nr++;
            }
            temp = temp->next;        
        }
    }
    return nr;
}
 */




int conta_ocorrencias(char *dominio){
    /*Funcao que recebe um char*(dominio) e devolve o numero de vezes que o dominio existe*/
    int contaocorrencia = 0;
    Node *temp = head;
    while(temp != NULL){
        if(strcmp(temp->Email->dominio, dominio) == 0){
            contaocorrencia++;
        }
        temp = temp->next;
    }
    return contaocorrencia;
}


void comando_a(){
    /*Funcao que recebe o input do comando a e, recorrendo a funcao adiciona contacto,
        adiciona o input recebido*/
    char *token;
    char *nome, *email, *telefone, *local, *dominio;
    char buffernome[MAX_NOME], bufferemail[MAX_EMAIL], buffertelefone[MAX_TELE];
    
    scanf("%s %s %s", buffernome, bufferemail, buffertelefone);
    
    nome = (char *) malloc(sizeof(char) * (strlen(buffernome) + 1));
    strcpy(nome, buffernome);
    
    email = (char *) malloc(sizeof(char) * (strlen(bufferemail) + 1));
    strcpy(email, bufferemail);
    
    telefone =  (char *) malloc(sizeof(char) * (strlen(buffertelefone) + 1));
    strcpy(telefone, buffertelefone);
    
    local = (char *) malloc(sizeof(char) * (strlen(bufferemail) + 1));
    dominio = (char *) malloc(sizeof(char) * (strlen(bufferemail) + 1));
    
    token = strtok(email, "@");
    strcpy(local, token);
    token = strtok(NULL, "\n");
    strcpy(dominio, token);
    free(email);
    adiciona_contacto(nome, local, dominio, telefone);
}


void comando_l(){
    /*Funcao que imprime todos os contactos na lista*/
    struct Node *contactos = head;
    while(contactos != NULL){        
        printf("%s %s@%s %s\n",contactos->Contacto->nome, contactos->Email->local,
        contactos->Email->dominio, contactos->Contacto->telefone);
        contactos = contactos->next;
    }
} 


void comando_p(){
    /*Funcao que recebe, recebendo do input, um nome, procura o contacto, recorrendo a funcao
    existe_contacto_nome e devolve as informacoes(nome, email e telefone) correspondentes*/
    int valorhash;
    char nome[MAX_NOME];
    char *nomeprocura;
    HashTable *temp;
    
    scanf("%s", nome);
    nomeprocura = (char *) malloc(sizeof(char*) * (strlen(nome) + 1));
    strcpy(nomeprocura, nome);
    
    if (existe_nome(nomeprocura) != 0){
        printf("Nome inexistente.\n");
        free(nomeprocura);
    }
    else{
        valorhash = hash(nomeprocura);
        temp = existe_contacto_nome(nomeprocura, valorhash);
        printf("%s %s@%s %s\n", temp->node->Contacto->nome, temp->node->Email->local, temp->node->Email->dominio, temp->node->Contacto->telefone);
        free(nomeprocura);

    }
}


void comando_r(){
    /*Funcao que recebe do input um nome e apaga o contacto, recorrendo as funcoes
        apaga_contacto_lista e apaga_elemento_hashtable */
    int keynome;
    char buffernome[MAX_NOME];
    char *nome_procura;
    HashTable *temp;
    
    scanf("%s", buffernome);
    nome_procura = (char *) malloc(sizeof(char*) * (strlen(buffernome) + 1));
    strcpy(nome_procura, buffernome);
    
    if(existe_nome(nome_procura) != 0){
        printf("Nome inexistente.\n");
        free(nome_procura);
    }
    
    else{
        keynome = hash(nome_procura);
        temp = existe_contacto_nome(nome_procura, keynome);
        apaga_contacto_lista(temp);
        apaga_elemento_hashtable(temp, nome_procura);
        free(nome_procura);
        free(temp);
    }
}


void comando_e(){
    /*Funcao que recebe do input um nome e um novo email e substitui no contacto correspondente 
        ao nome recebido o email antigo pelo recebido.*/
    int keynome;
    char buffernome[MAX_NOME], bufferemail[MAX_EMAIL], local[MAX_EMAIL], dominio[MAX_EMAIL];
    char *nomeptr, *token;
    HashTable *temp;
    
    scanf("%s %s", buffernome, bufferemail);
    
    token = strtok(bufferemail, "@");
    strcpy(local, token);
    token = strtok(NULL, "\0");
    strcpy(dominio, token);
    
    nomeptr = (char *) malloc(sizeof(char *) * (strlen(buffernome) + 1));
    strcpy(nomeptr, buffernome);
    
    keynome = hash(nomeptr);
    if (existe_nome(nomeptr) != 0){
        printf("Nome inexistente.\n");
    }
    else{
        temp = existe_contacto_nome(nomeptr, keynome);
        free(temp->node->Email->local);
        free(temp->node->Email->dominio);
        temp->node->Email->local = (char *) malloc(sizeof(char *) * (strlen(local) + 1));
        strcpy(temp->node->Email->local, local);
        temp->node->Email->dominio = (char *) malloc(sizeof(char *) * (strlen(dominio) + 1));
        strcpy(temp->node->Email->dominio, dominio);
    }
    free(nomeptr);
}


void comando_c(){
    /*Funcao que recebe do input um char*(dominio) e recorrendo a funcao conta_ocorrencias
        imprime as vezes que o dominio ocorre.*/
    int nr;
    char dominio[MAX_NOME];
    char *dominioprocura;
    
    scanf("%s", dominio);
    dominioprocura = (char *) malloc(sizeof(char*) * (strlen(dominio) + 1));
    strcpy(dominioprocura, dominio);

    nr = conta_ocorrencias(dominioprocura);
    printf("%s:%d\n", dominioprocura, nr);

    free(dominioprocura);
}



