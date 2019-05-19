
/*------------------------------------------------------------\
|            Autor : Catarina Sofia dos Santos Sousa          |
|                        N.93695                              |
|            Sistema de gestao de contactos                   |
\------------------------------------------------------------*/


#ifndef MAIN_H
#define MAIN_H





/*----------------------------------------------\
|                  Constantes                   |
\----------------------------------------------*/


#define MAX_NOME 1024
#define MAX_EMAIL 512
#define MAX_TELE 64
#define MAX_HASH 1031



/*----------------------------------------------\
|                  Estruturas                   |
\----------------------------------------------*/


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

typedef struct HashTable{
    struct Node *node;
    struct HashTable *next;
    struct HashTable *prev;
}HashTable;


typedef struct HashTableDominio{
    char *dominio;
    struct HashTableDominio *next;
}HashTableDominio;



HashTableDominio *headDominios;
Node *head;
HashTable *tabela_contacto[MAX_HASH];
HashTableDominio *tabela_dominio[MAX_HASH];


/*----------------------------------------------\
|                  PROTOTIPOS                   |
\----------------------------------------------*/

void inicializar();
void free_node(struct Node *contacto_node);
void free_nodes_lista();
void free_dominios_tabela();
void free_contactos_tabela();
int hash(char *string);
int existe_nome(char *nome);
struct HashTable *existe_contacto_nome(char *nome, int posicao);
void insere_tabela_contacto(int key, struct Node *nvcontacto);
void insere_tabela_dominio(int key, char *string);
void adiciona_contacto(char *nome, char *local, char *dominio, char *telefone);
void apaga_contacto_lista(struct HashTable *del_contacto);
void apaga_elemento_hashtable(struct HashTable *del_elemento, char *nome);
int conta_ocorrencias(char *dominio);
void comando_a();
void comando_l();
void comando_p();
void comando_r();
void comando_e();
void comando_c();




#endif