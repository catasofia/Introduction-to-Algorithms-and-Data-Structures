
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 1024
#define MAX_EMAIL 512
#define MAX_TELE 64


int contador;


typedef struct Contacto{
    char nome[MAX_NOME];
    char email[MAX_EMAIL];
    char telefone[MAX_TELE];
}Contacto;

typedef struct Node{
    Contacto Contacto;
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



verifica_mail(char dominio[MAX_EMAIL]){
    int contaocorrencia = 0;
    Node *temp = head;
    while(temp != NULL){
        if(strcmp(temp->Contacto.email, dominio) == 0){
            contaocorrencia++;
        }
        temp = temp->next;
    }
    return contaocorrencia;
}



char separa_email(char mail[MAX_EMAIL]){
    char *token, dominio[MAX_EMAIL];
    token = strtok(mail, "@");
    token = strtok(NULL, "\n");
    strcpy(dominio, token);
    return dominio;
}



void adiciona_contacto(char nome[MAX_NOME], char email[MAX_EMAIL], char telefone[MAX_TELE]){
    Node *temp;
    struct Node *lista = (struct Node*) malloc(sizeof(struct Node));
    strcpy(lista->Contacto.nome, nome);
    strcpy(lista->Contacto.email, email);
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
    char nome[MAX_NOME], email[MAX_EMAIL], telefone[MAX_TELE];
    scanf("%s %s %s", nome, email, telefone);
    if (verifica_nome(nome) == 0){
        contador++;
        adiciona_contacto(nome, email, telefone);
        separa_email(email);
    }
    else{
        printf("Nome existente.\n");
    }
}



void comando_l(){
    struct Node *contactos = head;
   while(contactos != NULL) {        
      printf("%s %s %s\n",contactos->Contacto.nome, contactos->Contacto.email, contactos->Contacto.telefone);
      contactos = contactos->next;
   }
} 



/* 
void comando_p(){
    char nome[MAX_NOME];
    scanf("%s", nome);
    if (verifica_nome(nome) == 0){
        printf("Nome inexistente.\n");
    }
    else{

    }
}
 */

void comando_c(){
    int nr;
    char mail[MAX_EMAIL], dominio[MAX_EMAIL];
    scanf("%s", mail);
    dominio = separa_email(mail);
    nr = verifica_mail()
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
                break;*/
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


