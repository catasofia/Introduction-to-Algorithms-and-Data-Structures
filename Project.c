
/*
Ficheiro: projeto.c 
Catarina Sousa, Nº 93695
Projeto IAED1819 -> Criacao de Eventos
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Constantes definidas*/

#define MAX_EVENTO 100
#define MAX_STR 65
#define MAX_SALA 10
#define DIM_PARTIC 3
#define MAX_DURACAO 1440
#define MAX_INPUT 400


/* Estruturas */

typedef struct Data{
    int ano, mes, dia;
}Data;


typedef struct Evento{
    char descricao[MAX_STR], responsavel[MAX_STR], participantes[DIM_PARTIC][MAX_STR];
    int duracao, sala, nParticipantes, inicio;
    Data data;
}Evento;


/*Variaveis globais*/

int contador[MAX_SALA];
Evento evento[MAX_SALA][MAX_EVENTO];
Evento vetorSort[MAX_SALA * MAX_EVENTO];

/*Prototipos*/

int converteHoras(int horas, int duracao);
int verificaSobreposicao(int fimC, int fimI, int cont, int sala, Evento novoEv);
int comparaDatas(Evento a, Evento b);
int verificaParticipantes(Evento novoEv, char comando, int indice, int cont);
int verificaSala(Evento novoEv, int sala);
int verificaErro(Evento novoEv, int sala);
int less(Evento a, Evento b);
void ordenaEventosSala(int sala, int n);
int listaSalaEventos();
void ordenaTodosEventos(int tam);
void listaEventos();
int adicionaEvento(Evento NvEvento, int sala);
void leInputEvento();
int apagaEvento(int sala, int indice);
int lerInputApagaEvento();
int alteraHora();
int alteraDuracao();
int mudaSala();
int adicionapartic();
int removepartic();


/*Codigo principal*/

int main(){
    int i;
    char comando;
    for(i = 0 ; i < MAX_SALA; i++){
        contador[i] = 0;/* inicializa o contador das salas a 0*/
    }
    while (1){
        comando = getchar();
    switch (comando){
        case 'a':
        leInputEvento();
        break;
        
        case 'l':
        listaEventos();
        break;
        
        case 's':
        listaSalaEventos();
        break;
        
        case 'r':
        lerInputApagaEvento();
        break;
            
        case 'i':
        alteraHora();
        break;
        
        case 't':
        alteraDuracao();
        break;
        
        case 'm':
        mudaSala();
        break;

        case 'A':
        adicionapartic();
        break;
        
        case 'R':
        removepartic();
        break;
        
        case 'x':
        exit(0);
        break;
    }
    }
    return 0;
}



int converteHoras(int horas, int duracao){
    /*Funcao que converte horas e retorna a hora final do evento
    Input -> int horas, int minutos
    Output -> int*/
    int horasCompletas, minutos, nMinutos, Horafinal, Minutofinal;
    minutos = horas % 100;
    horasCompletas = (horas / 100);
    nMinutos = minutos + duracao;
    Minutofinal = (nMinutos % 60);
    Horafinal = horasCompletas + nMinutos / 60;
    return Horafinal*100 + Minutofinal;
}


int verificaSobreposicao(int fimC, int fimI, int cont, int sala, Evento novoEv){
    /*Funcao que verifica se o evento a adicionar sobrepoe as horas com os outros eventos
    Retorna 0 se nao for possivel agendar o evento e diferente de 0 se for*/
    if ((novoEv.inicio >= evento[sala][cont].inicio && novoEv.inicio < fimI) 
        || (evento[sala][cont].inicio < fimC && fimC < fimI)
        || (novoEv.inicio >= evento[sala][cont].inicio && fimC <= fimI)
        || (novoEv.inicio <= evento[sala][cont].inicio && fimC >= fimI)){
            return 0;
    }
    return -1;
}


int comparaDatas(Evento a, Evento b){
    /*Funcao que recebe dois eventos e retorna 0 se os eventos corresponderem a mesma data*/
    if (a.data.ano == b.data.ano && a.data.mes == b.data.mes && a.data.dia == b.data.dia){
        return 0;
    }
    else{
        return -1;
    }
}


int verificaParticipantes(Evento novoEv, char comando, int indice, int cont){
    /*Funcao que recebe um evento e retorna -1 se os participantes coinciderem e nao for possivel
    adicionar o evento recebido
    Verifica se os participantes e o responsavel estao em algum outro evento a mesma hora
    Retorna -1 se o participante estiver ocupado e 0 se nao estiver*/ 
    int i, e, j, k, l, fimC, fimI, flag;
    flag = 0;
    for (i = 0; i < MAX_SALA; i++){
        for(e = 0; e < contador[i]; e++){
            if(comparaDatas(evento[i][e], novoEv) == 0){
                fimI = converteHoras(evento[i][e].inicio, evento[i][e].duracao);
                fimC = converteHoras(novoEv.inicio, novoEv.duracao);
                if(verificaSobreposicao(fimC, fimI, e, i, novoEv) == 0){
                    if (comando == 'A'){
                        if (strcmp(novoEv.responsavel, evento[i][e].responsavel) == 0){
                            printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n",
                            novoEv.descricao, novoEv.responsavel);
                            flag = -1; /*Flag atualiza para -1 quando os participantes coincidem*/
                        }
                        for(j = 0; j < novoEv.nParticipantes; j++){                   
                            if(strcmp(novoEv.participantes[j], evento[i][e].responsavel) == 0){
                                printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", novoEv.descricao, novoEv.participantes[j]);        
                                flag = -1;
                            }
                        }    
                        for(k = 0; k < evento[i][e].nParticipantes; k++){
                            if(strcmp(novoEv.responsavel, evento[i][e].participantes[k]) == 0){
                                printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", novoEv.descricao, novoEv.responsavel);
                                flag = -1;
                            }  
                            for(j = 0; j < novoEv.nParticipantes; j++){                   
                                if(strcmp(novoEv.participantes[j], evento[i][e].participantes[k]) == 0){
                                    printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", novoEv.descricao, novoEv.participantes[j]);
                                    flag = -1;
                                }
                            }
                        }
                    }
                    else if (comando == 'B'){
                        /*Compara os participantes exceto com todos os eventos, exceto o proprio*/
                        if (strcmp(novoEv.participantes[novoEv.nParticipantes - 1], evento[i][e].responsavel) == 0 
                        && indice != i && cont != e){
                            flag = -1;
                        }
                        for(l = 0; l < evento[i][e].nParticipantes; l++){
                            if (strcmp(novoEv.participantes[novoEv.nParticipantes - 1], evento[i][e].participantes[l]) == 0
                            && indice != i && cont != e){
                                flag = -1;
                            }
                        }
                    }
                }    
            }
        }
    }
    return flag;
}


int verificaSala(Evento novoEv, int sala){
    /*Funcao que verifica se a sala esta disponivel para adicionar o evento
    Retorna 0 se puder adicionar o evento*/
    int i,  fimC, fimI;
    for(i = 0; i < contador[sala]; i++){
        if(comparaDatas(evento[sala][i], novoEv) == 0){
            fimI = converteHoras(evento[sala][i].inicio, evento[sala][i].duracao);
            fimC = converteHoras(novoEv.inicio, novoEv.duracao);
            if(verificaSobreposicao(fimC, fimI, i, sala, novoEv) == 0){
                return -1;
            }
        }
    }
    return 0;
}

int verificaErro(Evento novoEv, int sala){
    /*Funcao que verifica as condicoes necessarias para adicionar um evento
    Recorre a funcao verificaSala e a funcao verificaParticipantes
    Retorna 0 se for possivel adicionar o evento*/
    if (verificaSala(novoEv, sala) != 0){
            printf("Impossivel agendar evento %s. Sala%d ocupada.\n", novoEv.descricao, novoEv.sala);
            return -1;
            }
    else if (verificaParticipantes(novoEv, 'A', 11, 101) != 0){ 
        return -1;
    }
    return 0;
}           


int less(Evento a, Evento b){
    /*Funcao que recebe dois valores e retrona 0 se a data do primeiro evento for menor que
    a data do segundo*/
    if (a.data.ano  == b.data.ano && a.data.mes < b.data.mes){
        return 0;
    }
    else if (a.data.ano == b.data.ano && a.data.mes == b.data.mes && a.data.dia < b.data.dia){
        return 0;
    }
    else if (a.data.ano < b.data.ano){
        return 0;
    }
    else if(a.data.ano == b.data.ano && a.data.mes == b.data.mes && a.data.dia == b.data.dia
            && a.inicio < b.inicio){
                return 0;
            }
    return -1;
}



void ordenaEventosSala(int sala, int n){
    /*Funcao que recebe uma sala e o numero de eventos desta e ordena os eventos
    por ordem cronologica
    Recorre a funcao less para verificar qual e o primeiro projeto a acontecer*/ 
    int i, j;
    Evento key; 
    for (i = 0; i < n; i++){ 
        key = evento[sala][i]; 
        j = i - 1; 
        while (j >= 0 && (less(evento[sala][j], key) != 0)){ 
            evento[sala][j + 1] = evento[sala][j]; 
            j = j - 1; 
        } 
        evento[sala][j + 1] = key; 
    } 
} 

int listaSalaEventos(){
    /*funcao que lista os eventos, recorrendo a funcao ordenaEventosSala, por ordem
    cronologica.*/
    int sala, i, e;
    scanf("%d", &sala);
    sala--; /*Subtrai 1 a sala para comecar na posicao 0 do vetor*/
    ordenaEventosSala(sala, contador[sala]);
    
    for(i = 0; i < contador[sala]; i++){
        printf("%s %02d%02d%d %04d %d Sala%d %s\n*", evento[sala][i].descricao, evento[sala][i].data.dia, 
        evento[sala][i].data.mes,evento[sala][i].data.ano, evento[sala][i].inicio, 
        evento[sala][i].duracao, evento[sala][i].sala, evento[sala][i].responsavel);
        for(e = 0; e < evento[sala][i].nParticipantes ; e++){
            printf(" %s", evento[sala][i].participantes[e]);
        }
        printf("\n");
    }
    return 0;
}

void ordenaTodosEventos(int tam){
    /*Funcao que recebe o numero total de eventos existentes e ordena todos por
    ordem cronologica recorrendo a funcao less para verificar qual o primeiro a acontecer*/
    int i, j;
    Evento aux;
    for(i = 0; i < tam; i++){
        aux = vetorSort[i];
        j = i - 1;
        while(j >= 0 && less(aux, vetorSort[j]) == 0){
            vetorSort[j + 1] = vetorSort[j];
            j--;
        }
        vetorSort[j + 1] = aux;
    }
}



void listaEventos(){
    /*Funcao que lista os eventos, recorrendo a funcao ordenaTodosEventos, por ordem
    cronologica*/
    int i, e, j, c = 0;
    for (i = 0; i < MAX_SALA; i++){
        for(e = 0; e < contador[i]; e++){
            vetorSort[c] = evento[i][e];
            c++;
        }
    }
    ordenaTodosEventos(c);
    for (j = 0; j < c; j++){
        printf("%s %02d%02d%d %04d %d Sala%d %s\n*", vetorSort[j].descricao, vetorSort[j].data.dia, vetorSort[j].data.mes,
        vetorSort[j].data.ano, vetorSort[j].inicio, vetorSort[j].duracao, vetorSort[j].sala, vetorSort[j].responsavel);
        for(e = 0; e < vetorSort[j].nParticipantes ; e++){
            printf(" %s", vetorSort[j].participantes[e]);
        }
        printf("\n");
    }        
} 


int adicionaEvento(Evento NvEvento, int sala){
    /*Funcao que, apos verificar os erros, se bem sucedido, adiciona o evento
    a sala respetiva, na posicao [sala - 1] do vetor para comecar a adicionar
    na posicao 0*/
    if(verificaErro(NvEvento, sala - 1) != -1){        
        evento[sala - 1][contador[sala - 1]] = NvEvento;
        contador[sala - 1]++;
        return 0;
    }
    return -1;
}


void leInputEvento(){
    /*Funcao que recebe o input e separa sempre que atinge ":"*/   
    int i = 0, sala, data;
    char* token; 
    char linhas[MAX_INPUT];
    Evento e;
    getchar();
    fgets(linhas, MAX_INPUT, stdin); /*recebe o input dado*/
    token = strtok(linhas, ":");
    strcpy(e.descricao, token);

    token = strtok(NULL, ":");
    data = atoi(token);
    e.data.ano = data % 10000; /*guarda o ano na estrutura*/
    e.data.mes = (data / 10000) % 100; /*guarda o mes na estrutura*/
    e.data.dia = data / 10000 / 100; /*guarda o dia na estrutua*/

    token = strtok(NULL, ":");
    e.inicio = atoi(token);

    token = strtok(NULL, ":");
    e.duracao = atoi(token);

    token = strtok(NULL, ":");
    e.sala = atoi(token);
    sala = atoi(token);

    token = strtok(NULL, ":");
    strcpy(e.responsavel, token);

    token = strtok(NULL,":\n");
    while (token != NULL && i < 3 && i>=0){
        strcpy(e.participantes[i], token);
        token = strtok(NULL,":\n");
        i++;
    }
    e.nParticipantes = i;
    adicionaEvento(e, sala);
}



int apagaEvento(int sala, int indice){
    /*Funcao que recebe uma sala e a posicao do evento a apagar e retorna o contador da sala*/
    int i;
    for(i = indice; i < contador[sala]; i++){
        evento[sala][i] = evento[sala][i + 1]; /*desloca os eventos a partir do indice recebido
        para a posicao anterior*/
    }
    contador[sala]--; /*atualiza o contador*/
    return contador[sala];
}

int lerInputApagaEvento(){
    /*Funcao que le o input dado e retorna 0 se for possivel apagar o evento e -1
    se o evento nao existir*/
    int i, e;
    char descricao[MAX_STR], linhas[MAX_INPUT];
    char* token;
    getchar();
    fgets(linhas, MAX_STR, stdin);
    token = strtok(linhas, "\n");
    strcpy(descricao, token);
    for(i = 0; i < MAX_SALA; i++){
        for(e = 0; e < contador[i]; e++){
            if(strcmp(evento[i][e].descricao, descricao) == 0){
                contador[i] = apagaEvento(i, e);
                return 0;
            }
        }
    }
    printf("Evento %s inexistente.\n", descricao);
    return -1;
}

int alteraHora(){
    /*Funcao que altera hora de um evento se possivel, verificando as condicoes necessarias*/
    int i, e, NovoInicio, Inicio;
    char* token;
    char linhas[MAX_INPUT];
    Evento NvEvento, EventoAux;
    getchar();
    fgets(linhas, MAX_INPUT, stdin);
    token = strtok(linhas, ":\n");
    strcpy(NvEvento.descricao, token);
    token = strtok(NULL, ":\n");
    NovoInicio = atoi(token); 
    for(i = 0; i < MAX_SALA; i++){
        for(e = 0; e < contador[i]; e++){
            if (strcmp(NvEvento.descricao, evento[i][e].descricao) == 0){
                Inicio = evento[i][e].inicio; /*Guarda o inicio atual*/
                EventoAux = evento[i][e];
                apagaEvento(i, e);
                EventoAux.inicio = NovoInicio;
                if ((adicionaEvento(EventoAux, EventoAux.sala)) == 0){ /*Verifica as condicoes para adicionar um evento novo
                com o novo inicio*/
                    return 0;
                }
                else{
                    /*Nao e possivel adicionar o evento, logo volta a adicionar o evento com o inicio antigo*/
                    EventoAux.inicio = Inicio;
                    adicionaEvento(EventoAux, EventoAux.sala);
                    return -1;
                }
            }
        }
    }
    printf("Evento %s inexistente.\n", NvEvento.descricao);
    return -1;
}

int alteraDuracao(){
    /*Funcao que altera a duracao de um evento se possivel, verificando as condicoes necessarias*/
    int i, e, NovaDuracao, duracao;
    char* token;
    char linhas[MAX_INPUT];
    Evento NvEvento, EventoAux;
    getchar();
    fgets(linhas, MAX_INPUT, stdin);
    token = strtok(linhas, ":\n");
    strcpy(NvEvento.descricao, token);
    token = strtok(NULL, ":\n");
    NovaDuracao = atoi(token);
    for(i = 0; i < MAX_SALA; i++){
        for(e = 0; e < contador[i]; e++){
            if(strcmp(NvEvento.descricao, evento[i][e].descricao) == 0){
                duracao = evento[i][e].duracao; /*guarda a duracao atual*/
                EventoAux = evento[i][e];
                apagaEvento(i, e);
                EventoAux.duracao = NovaDuracao;
                if(adicionaEvento(EventoAux, EventoAux.sala) == 0){
                    /*Verifica as condicoes para adicionar o 
                evento com a nova duracao*/
                    return 0;
                }
                else{
                    /*Nao e possivel adicionar o evento com a nova duracao logo adiciona o evento com a duracao antiga*/
                    EventoAux.duracao = duracao;
                    adicionaEvento(EventoAux, EventoAux.sala);
                    return -1;
                }
            }
        }
    }
    printf("Evento %s inexistente.\n", NvEvento.descricao);
    return -1;
}

int mudaSala(){
    /*Funcao que verifica as condicoes necessarias para alterar a sala de um evento existente*/
    int i, e, novaSala, SalaAntiga;
    char* token;
    char linhas[MAX_INPUT];
    Evento NvEvento, EventoAux;
    getchar();
    fgets(linhas, MAX_INPUT, stdin);
    token = strtok(linhas, ":\n");
    strcpy(NvEvento.descricao, token);
    token = strtok(NULL, ":\n");
    novaSala = atoi(token);
    for(i = 0; i < MAX_SALA; i++){
        for(e = 0; e < contador[i]; e++){
            if(strcmp(NvEvento.descricao, evento[i][e].descricao) == 0){
                SalaAntiga = evento[i][e].sala; /*guarda a sala inicial*/
                EventoAux = evento[i][e];
                apagaEvento(i, e);
                EventoAux.sala = novaSala;
                if (adicionaEvento(EventoAux, novaSala) != 0){
                    /*Verifica as condicoes para adicionar um evento
                com a nova sala*/
                    EventoAux.sala = SalaAntiga;
                    adicionaEvento(EventoAux, EventoAux.sala);
                    /*se nao for possivel adicionar com a nova sala,
                    adiciona com a sala antiga*/
                    return -1;
                }
                else{
                    return 0;
                }
            }
        }
    }
    printf("Evento %s inexistente.\n", NvEvento.descricao);
    return -1;
}



int adicionapartic(){
    /*Funcao que, verificando as condicoes necessarias, adiciona um novo participante a um evento*/
    char linhas[344], participante[MAX_STR];
    char* token;
    int i, e, flag;
    Evento ev, EventoAux1, EventoAux2;
    flag = 0;
    getchar();
    fgets(linhas, MAX_INPUT, stdin);
    token = strtok(linhas, ":\n");
    strcpy(ev.descricao, token);
    token = strtok(NULL, ":\n");
    strcpy(participante, token);
    for(i = 0; i < MAX_SALA; i++){
        for(e = 0; e < contador[i]; e++){
            if (strcmp(ev.descricao, evento[i][e].descricao) == 0){
                if (evento[i][e].nParticipantes == 3){
                    /*verifica se o numero de participantes e o maximo possivel*/
                    printf("Impossivel adicionar participante. Evento %s ja tem 3 participantes.\n", ev.descricao);
                    return -1;
                }
                else{
                    /*Guarda em 2 eventos auxiliares para mais tarde adicionar consoante
                    as verificacoes*/
                    EventoAux1 = evento[i][e]; 
                    EventoAux2 = evento[i][e];
                    apagaEvento(i, e);
                    for(i = 0; i < EventoAux1.nParticipantes; i++){  
                        if (strcmp(EventoAux1.participantes[i], participante) == 0){
                            flag++;
                        }
                    }
                    if (flag == 0){                 
                        strcpy(EventoAux1.participantes[EventoAux1.nParticipantes], participante);
                        EventoAux1.nParticipantes++;
                        if (verificaParticipantes(EventoAux1, 'B', i, e) == 0){
                            adicionaEvento(EventoAux1, EventoAux1.sala);
                            /*Possivel adicionar um novo participante*/
                            return 0;
                        }
                        else{
                            printf("Impossivel adicionar participante. Participante %s tem um evento sobreposto.\n", participante);
                            adicionaEvento(EventoAux2, EventoAux2.sala);
                            return -1;
                        }
                    }
                    else{
                        adicionaEvento(EventoAux2, EventoAux2.sala);
                        return -1;
                    }
                }
            }
        }
    }
    printf("Evento %s inexistente.\n", ev.descricao);
    return -1;
}


int removepartic(){
    /*Funcao que remove um participante de um participante, verificando se e possivel recorrendo
    a funcoes auxiliares par isso*/
    char participante[MAX_STR], linhas[MAX_INPUT];
    char* token;
    int i, e, j, c, indice, flag;
    Evento ev;
    flag = 0;
        getchar();
        fgets(linhas, MAX_INPUT, stdin);
        token = strtok(linhas, ":\n");
        strcpy(ev.descricao, token);
        token = strtok(NULL, ":\n");
        strcpy(participante, token);
        for(i = 0; i < MAX_SALA; i++){
            for(e = 0; e < contador[i]; e++){
                if(strcmp(ev.descricao, evento[i][e].descricao) == 0){
                    for(j = 0; j < evento[i][e].nParticipantes; j++){
                        if(strcmp(evento[i][e].participantes[j], participante) == 0){
                            indice = j;
                            flag++;
                        }
                    }
                    if (flag == 0){
                        return -1;
                    }
                    else if(evento[i][e].nParticipantes == 1){
                        printf("Impossivel remover participante. Participante %s e o unico participante no evento %s.\n", participante, evento[i][e].descricao);
                        return -1;
                    }
                    else{
                        for(c = indice; c < evento[i][e].nParticipantes; c++){
                            strcpy(evento[i][e].participantes[c], evento[i][e].participantes[c + 1]);
                            /*coloca, a partir do indice do participante removido, 
                            todos os participantes na posicao anterior*/
                        }
                        evento[i][e].nParticipantes--; /*atualiza o contador de participantes*/
                        return 0;
                    }
                }
            }                
        }
    printf("Evento %s inexistente.\n", ev.descricao);
    return -1;
}




