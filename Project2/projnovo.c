
/*---------------------------------------------------\ 
|           Catarina Sofia dos Santos Sousa          |
|                        N.93695                     |
|           Sistema de gestao de contactos           |    
\---------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 1024
#define MAX_EMAIL 512
#define MAX_TELE 64
#define MAX_HASH 1031


typedef struct Email{
    char *local;
    char *dominio;
}*Email;

typedef struct Contacto{
    char *nome;
    char *telefone;
}*Contacto;

typedef struct Node{
    Contacto Contacto;
    Email Email;
    struct Node *next;
    struct Node *prev;
}Node;


Node *head;

int hash(char *string){
    int h, a = 31415, b = 27183;
    for(h = 0; *string != '\0'; string++, a = a*b % (MAX_HASH-1)){
        h = (a * h + *string) % MAX_HASH;
    }
    return h;
}

void adiciona_contacto(char *nome, char *local, char *dominio, char *telefone){
    Node *temp;
    struct Node *lista = (struct Node*) malloc(sizeof(struct Node));
    lista->Contacto = (Contacto) malloc(sizeof(Contacto));
    lista->Email = (Email) malloc(sizeof(Email));
    lista->Contacto->nome = (char*) malloc(sizeof(char) * (strlen(nome) + 1));
    lista->Email->local = (char*) malloc(sizeof(char) * (strlen(local) + 1));
    lista->Email->dominio = (char*) malloc(sizeof(char) * (strlen(dominio) + 1));
    lista->Contacto->telefone = (char*) malloc(sizeof(char) * (strlen(telefone) + 1));
    strcpy(lista->Contacto->nome, nome);
    strcpy(lista->Email->local, local);
    strcpy(lista->Email->dominio, dominio);
    strcpy(lista->Contacto->telefone, telefone);
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
    char *nome, *email, *telefone, *local, *dominio;
    char buffernome[MAX_NOME], bufferemail[MAX_EMAIL], buffertelefone[MAX_TELE];
    scanf("%s %s %s", buffernome, bufferemail, buffertelefone);
    nome = (char*) malloc(sizeof(char*) * (strlen(buffernome) + 1));
    strcpy(nome, buffernome);
    email = (char*) malloc(sizeof(char*) * (strlen(bufferemail) + 1));
    strcpy(email, bufferemail);
    telefone = (char*) malloc(sizeof(char*) * (strlen(buffertelefone) + 1));
    strcpy(telefone, buffertelefone);
    local = (char*) malloc(sizeof(char*) * (strlen(bufferemail) + 1));
    dominio = (char*) malloc(sizeof(char*) * (strlen(bufferemail) + 1));
    token = strtok(email, "@");
    strcpy(local, token);
    token = strtok(NULL, "\n");
    strcpy(dominio, token);
    adiciona_contacto(nome, local, dominio, telefone);
}



void comando_l(){
    struct Node *contactos = head;
   while(contactos != NULL) {        
      printf("%s %s@%s %s\n",contactos->Contacto->nome, contactos->Email->local,
      contactos->Email->dominio, contactos->Contacto->telefone);
      contactos = contactos->next;
   }
} 



void inicializar(){
    head = NULL;
}

int main(){
    char comando;
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
            /*case 'p':
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
                break;  */
            case 'x':
                exit(0);
                break;
        }
    }
    return 0;
}
