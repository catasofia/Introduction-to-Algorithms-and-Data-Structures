
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 1024
#define MAX_EMAIL 512
#define MAX_TELE 64
#define MAX_HASH 1031


int contador;

typedef struct Email{
    char local[MAX_EMAIL];
    char dominio[MAX_EMAIL];
}Email;

typedef struct Contacto{
    char nome[MAX_NOME];
    Email email;
    char telefone[MAX_TELE];
}Contacto;

typedef struct Node{
    Contacto Contacto;
    Email Email;
    struct Node *next;
    
}Node;


Node *head;


 

int verifica_nome(char nomeprocura[MAX_NOME]){
    Node *temp = head;
    while(temp != NULL){
        if(strcmp(temp->Contacto.nome, nomeprocura) == 0){
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}



int verifica_mail(char dominio[MAX_EMAIL]){
    int contaocorrencia = 0;
    Node *temp = head;
    while(temp != NULL){
        if(strcmp(temp->Email.dominio, dominio) == 0){
            contaocorrencia++;
        }
        temp = temp->next;
    }
    return contaocorrencia;
}


struct Node *encontra_contacto(char nome[MAX_EMAIL]){
    Node *temp = head;
    while(temp != NULL){
        if(strcmp(temp->Contacto.nome, nome) == 0){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}


void adiciona_contacto(char nome[MAX_NOME], char local[MAX_EMAIL], char dominio[MAX_EMAIL],char telefone[MAX_TELE]){
    Node *temp;
    struct Node *lista = (struct Node*) malloc(sizeof(struct Node));
    strcpy(lista->Contacto.nome, nome);
    strcpy(lista->Email.local, local);
    strcpy(lista->Email.dominio, dominio);
    strcpy(lista->Contacto.telefone, telefone);
    lista->next = NULL;
    if (head == NULL){
        head = lista;   
    }
    else{
        temp = head;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = lista;
    }
}



void comando_a(){
    char *token;
    char nome[MAX_NOME], email[MAX_EMAIL], local[MAX_EMAIL], dominio[MAX_EMAIL], telefone[MAX_TELE];
    scanf("%s %s %s", nome, email, telefone);
    token = strtok(email, "@");
    strcpy(local, token);
    token = strtok(NULL, "\n");
    strcpy(dominio, token);
    if (verifica_nome(nome) == 0){
        contador++;
        adiciona_contacto(nome, local, dominio, telefone);
    }
    else{
        printf("Nome existente.\n");
    }
}



void comando_l(){
    struct Node *contactos = head;
   while(contactos != NULL) {        
      printf("%s %s@%s %s\n",contactos->Contacto.nome, contactos->Email.local, contactos->Email.dominio, contactos->Contacto.telefone);
      contactos = contactos->next;
   }
} 




void comando_p(){
    char nome[MAX_NOME];
    Node *contacto;
    scanf("%s", nome);
    if (verifica_nome(nome) == 0){
        printf("Nome inexistente.\n");
    }
    else{
        contacto = encontra_contacto(nome);
        printf("%s %s@%s %s\n", contacto->Contacto.nome, contacto->Email.local, contacto->Email.dominio, contacto->Contacto.telefone);
    }
}
 

void comando_c(){
    int nr;
    char dominio[MAX_EMAIL];
    scanf("%s", dominio);
    nr = verifica_mail(dominio);
    printf("%s:%d\n", dominio, nr);
}


void remove_contacto(char nomeprocura[MAX_NOME]){
    struct Node *temp = head, *prev;
    if (strcmp(temp->Contacto.nome, nomeprocura) == 0){
        head = temp->next;
        free(temp);
        return;
    }

    while (strcmp(temp->Contacto.nome, nomeprocura) != 0){
        prev = temp;
        temp = temp->next;
    }
    prev->next = temp->next;
    free(temp);
}

void comando_r(){
    char nome[MAX_NOME];
    scanf("%s", nome);
    if(verifica_nome(nome) == 0){
        printf("Nome inexistente.\n");
    }
    else{
        remove_contacto(nome);
    }
} 

void comando_e(){
    char nome[MAX_NOME], novomail[MAX_EMAIL], novolocal[MAX_EMAIL], novodominio[MAX_EMAIL];
    char *token;
    Node *contacto;
    scanf("%s %s", nome, novomail);
    if (verifica_nome(nome) == 0){
        printf("Nome inexistente.\n");
    }
    else{
        token = strtok(novomail, "@");
        strcpy(novolocal, token);
        token = strtok(NULL, "\n");
        strcpy(novodominio, token);
        contacto = encontra_contacto(nome);
        strcpy(contacto->Email.local, novolocal);
        strcpy(contacto->Email.dominio, novodominio);
    }
}

void inicializar(){
    head = NULL;
}




int main(){
    char comando;
    contador = 0;
    inicializar();
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
                exit(0);
                break;
        }
    }
    return 0;
}


