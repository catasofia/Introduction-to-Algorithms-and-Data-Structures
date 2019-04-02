

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EVENTO 100
#define MAX_STR 63
#define MAX_SALA 10
#define DIM_PARTIC 3
#define MAX_DURACAO 1440


int contador[MAX_SALA];

typedef struct Data{
    int ano, mes, dia;
}Data;


typedef struct Evento{
    char descricao[MAX_STR], responsavel[MAX_STR], participantes[DIM_PARTIC][MAX_STR];
    int duracao, sala, nParticipantes, inicio;
    Data data;
}Evento;


Evento evento[MAX_SALA][MAX_EVENTO];


int converteHoras(int horas, int duracao){
    int horasCompletas, minutos, nMinutos, Horafinal, Minutofinal;
    minutos = horas % 100;
    horasCompletas = (horas / 100);
    nMinutos = minutos + duracao;
    Minutofinal = (nMinutos % 60);
    Horafinal = horasCompletas + nMinutos / 60;
    return Horafinal*100 + Minutofinal;
}
/*
int verificaDescricao(Evento ev){
    int i, e;
    for(i = 1; i <= MAX_SALA; i++){
        for(e = 1; e <= contador[i]; e++){
            if (strcmp(ev.descricao, evento[i][e].descricao) == 0){
                return 0;
            }
        }
    }
    return -1;
}*/


int verificaSobreposicao(int fimC, int fimI, int cont, int sala, Evento novoEv){
    if ((novoEv.inicio > evento[sala][cont].inicio && novoEv.inicio < fimI) 
        || (evento[sala][cont].inicio < fimC && fimC < fimI)
        || (novoEv.inicio > evento[sala][cont].inicio && fimC < fimI)
        || (novoEv.inicio < evento[sala][cont].inicio && fimC > fimI)){
            return 0;
    }
    return -1;
}


int comparaDatas(Evento a, Evento b){
    if (a.data.ano == b.data.ano && a.data.mes == b.data.mes && a.data.dia == b.data.dia){
        return 0;
    }
    else{
        return -1;
    }
}


int verificaParticipantes(Evento novoEv){
    int i, e, j, k, fimC, fimI, flag;
    flag = 0;
    for (i = 0; i < MAX_SALA; i++){
        for(e = 0; e < contador[i]; e++){
            if(comparaDatas(evento[i][e], novoEv) == 0){
                fimI = converteHoras(evento[i][e].inicio, evento[i][e].duracao);
                fimC = converteHoras(novoEv.inicio, novoEv.duracao);
                if(verificaSobreposicao(fimC, fimI, e, i, novoEv) == 0){
                    if (strcmp(novoEv.responsavel, evento[i][e].responsavel) == 0){
                        printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", novoEv.descricao, novoEv.responsavel);
                        flag = -1;
                    }
                    for(j = 0; j < novoEv.nParticipantes; j++){                   
                        if(strcmp(novoEv.participantes[j], evento[i][e].responsavel) == 0){
                            printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", novoEv.descricao, novoEv.participantes[j]);        
                            flag = -1;
                        }    
                        for(k = 0; k < evento[i][e].nParticipantes; k++){
                            if(strcmp(novoEv.participantes[j], evento[i][e].participantes[k]) == 0){
                                printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", novoEv.descricao, novoEv.participantes[j]);
                                flag = -1;
                            }
                            if(strcmp(novoEv.responsavel, evento[i][e].participantes[k]) == 0){
                                printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", novoEv.descricao, novoEv.responsavel);
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


int verificaErro(Evento novoEv, int sala){
    int i,  fimC, fimI;
    for(i = 0; i < contador[sala]; i++){
        if(comparaDatas(evento[sala][i], novoEv) == 0){
            fimI = converteHoras(evento[sala][i].inicio, evento[sala][i].duracao);
            fimC = converteHoras(novoEv.inicio, novoEv.duracao);
            if(verificaSobreposicao(fimC, fimI, i, sala, novoEv) == 0){
                printf("Impossivel agendar evento %s. Sala%d ocupada.\n", novoEv.descricao, novoEv.sala);
                return -1;
            }
            else{ 
                if (verificaParticipantes(novoEv) != 0){
                    return -1;
                }
            }
        }
    }
    if (verificaParticipantes(novoEv) != 0){
        return -1;
    }
    return 0;
}           


int less(Evento a, Evento b){

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

void sortMat(int mat[SIZE][SIZE], int n){ 

    int temp[n * n]; 
    int k = 0; 
   
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < n; j++) 
            temp[k++] = mat[i][j]; 
  
    // sort temp[] 
    sort(temp, temp + k); 
      
    // copy the elements of temp[] one by one 
    // in mat[][] 
    k = 0; 
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < n; j++) 
            mat[i][j] = temp[k++]; 
} 

/*void ordenaEventos(int sala){
    int i, j;
    Evento prox, aux;
    for (i = 0; i < contador[sala]; i++){
        prox = evento[sala][i];
        j = i + 1;
        while(j > 0 && less(prox, evento[sala][j]) != 0){
            aux = prox;
            evento[sala][j + 1] = evento[sala][j];
            evento[sala][j - 1] = aux;
            j--;
        }
        
    }
}*/

void insertionSort(int sala, int n) { 
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



void adicionaEvento(){
    
    int i = 0, sala, data;
    char* token; 
    char linhas[342];
    Evento e;
    getchar();
    fgets(linhas, 342, stdin);
    token = strtok(linhas, ":");
    strcpy(e.descricao, token);

    token = strtok(NULL, ":");
    data = atoi(token);
    e.data.ano = data % 10000;
    e.data.mes = (data / 10000) % 100;
    e.data.dia = data / 10000 / 100;

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
    if (verificaErro(e, sala - 1) != -1){
        evento[sala - 1][contador[sala - 1]] = e;
        contador[sala - 1]++;
    }
}

int listaEventos(){
    return 0;
}

int listaSalaEventos(){
    int sala, i, e;
    scanf("%d", &sala);
    sala--;
    insertionSort(sala, contador[sala]);
    for(i = 0; i < contador[sala]; i++){
        printf("%s %02d%02d%d %04d %d Sala%d %s\n*", evento[sala][i].descricao, evento[sala][i].data.dia, evento[sala][i].data.mes,
        evento[sala][i].data.ano, evento[sala][i].inicio, evento[sala][i].duracao, evento[sala][i].sala, evento[sala][i].responsavel);
        for(e = 0; e < evento[sala][i].nParticipantes ; e++){
            printf(" %s", evento[sala][i].participantes[e]);
        }
        printf("\n");
    }
    return 0;
}


int apagaEvento(){
    int i, e, j;
    char descricao[MAX_STR], linhas[342];
    char* token;
    getchar();
    fgets(linhas, MAX_STR, stdin);
    token = strtok(linhas, "\n");
    strcpy(descricao, token);
    for(i = 0; i < MAX_SALA; i++){
        for(e = 0; e < contador[i]; e++){
            if(strcmp(evento[i][e].descricao, descricao) == 0){
                for(j = e; j < contador[i]; j++){
                    evento[i][j] = evento[i][j + 1];
                }
                contador[i]--;
                return 0;
            }
        }
    }
    printf("Evento %s inexistente.\n", descricao);
    return -1;
}

int alteraHora(){
    int i, e, NovoInicio, Inicio;
    char* token;
    char linhas[342];
    Evento NvEvento;
    getchar();
    fgets(linhas, 342, stdin);
    token = strtok(linhas, ":");
    strcpy(NvEvento.descricao, token);
    token = strtok(NULL, ":");
    NovoInicio = atoi(token);
    for(i = 0; i < MAX_SALA; i++){
        for(e = 0; e < contador[i]; e++){
            if (strcmp(NvEvento.descricao, evento[i][e].descricao) == 0){
                Inicio = evento[i][e].inicio;
                evento[i][e].inicio = NovoInicio;
                if (verificaErro(evento[i][e], evento[i][e].sala) != -1){
                    return 0;
                }
                else{
                    evento[i][e].inicio = Inicio;
                    return -1;
                }
            }
        }
    }
    printf("Evento %s inexistente.\n", NvEvento.descricao);
    return -1;
}

int alteraDuracao(){
    int i, e, NovaDuracao, duracao;
    char* token;
    char linhas[342];
    Evento NvEvento;
    getchar();
    fgets(linhas, 342, stdin);
    token = strtok(linhas, ":");
    strcpy(NvEvento.descricao, token);
    token = strtok(NULL, ":");
    NovaDuracao = atoi(token);
    for(i = 0; i < MAX_SALA; i++){
        for(e = 0; e < contador[i]; e++){
            if(strcmp(NvEvento.descricao, evento[i][e].descricao) == 0){
                duracao = evento[i][e].duracao;
                evento[i][e].duracao = NovaDuracao;
                if(verificaErro(evento[i][e], evento[i][e].sala) != -1){
                    evento[i][e].duracao = NovaDuracao;
                    return 0;
                }
                else{
                    evento[i][e].duracao = duracao;
                    return -1;
                }
            }
        }
    }
    printf("Evento %s inexistente.\n", NvEvento.descricao);
    return -1;
}

int mudaSala(){
    int i, e, novaSala, salaAntiga;
    char* token;
    char linhas[342];
    Evento NvEvento;
    getchar();
    fgets(linhas, 342, stdin);
    token = strtok(linhas, ":");
    strcpy(NvEvento.descricao, token);
    token = strtok(NULL, ":");
    novaSala = atoi(token);
    for(i = 0; i < MAX_SALA; i++){
        for(e = 0; e < contador[i]; i++){
            if(strcmp(NvEvento.descricao, evento[i][e].descricao) == 0){
                salaAntiga = evento[i][e].sala;
                evento[i][e].sala = novaSala;
                if (verificaErro(evento[i][e], evento[i][e].sala) != -1){
                    printf("Impossivel agendar evento %s. Sala%d ocupada.\n", NvEvento.descricao, novaSala);
                    evento[i][e].sala = salaAntiga;
                    return 0;
                }
            }
        }
    }
    printf("Evento %s inexistente.\n", NvEvento.descricao);
    return -1;
}



int removepartic(){
    char participante[MAX_STR], linhas[342];
    char* token;
    int i, e, j, c;
    Evento ev;
    getchar();
    fgets(linhas, 342, stdin);
    token = strtok(linhas, ":\n");
    strcpy(ev.descricao, token);
    token = strtok(NULL, ":\n");
    strcpy(participante, token);
    for(i = 0; i < MAX_SALA; i++){
        for(e = 0; e < contador[i]; e++){
            if(strcmp(ev.descricao, evento[i][e].descricao) == 0){
                for(j = 0; j < evento[i][e].nParticipantes; j++){
                    if(strcmp(evento[i][e].participantes[j], participante) == 0){
                        if(evento[i][e].nParticipantes == 1){
                            printf("Impossivel remover participante. Participante %s e o unico participante no evento %s.\n", participante, evento[i][e].descricao);
                            return -1;
                        }
                        else{
                            for(c = j; c < evento[i][e].nParticipantes; c++){
                                strcpy(evento[i][e].participantes[c],evento[i][e].participantes[c + 1]);
                            }
                            evento[i][e].nParticipantes--;
                            return 0;
                        }
                    }
                }                
            }
        }
    }
    printf("Evento %s inexistente.\n", evento[i][e].descricao);
    return -1;
}

int adicionapartic(){
    char linhas[342], participante[MAX_STR];
    char* token;
    int i, e;
    Evento ev;
    getchar();
    fgets(linhas, 342, stdin);
    token = strtok(linhas, ":");
    strcpy(ev.descricao, token);
    token = strtok(NULL, ":");
    strcpy(participante, token);
    for(i = 0; i < MAX_SALA; i++){
        for(e = 0; e < contador[i]; e++){
            if (strcmp(ev.descricao, evento[i][e].descricao) == 0){
                if (evento[i][e].nParticipantes == 3){
                    printf("Impossivel adicionar participante. Evento %s ja tem 3 participantes.\n", ev.descricao);
                    return -1;
                }
                else{
                    strcpy(evento[i][e].participantes[evento[i][e]. nParticipantes], participante);
                    evento[i][e].nParticipantes++;
                    if (verificaParticipantes(evento[i][e]) == 0){
                        return 0;
                    }
                    else{
                        return -1;
                    }
                }
            }
        }
    }
    printf("Evento %s inexistente.\n", ev.descricao);
    return -1;
}

int main(){
    int i;
    char comando;
    for(i = 0 ; i < MAX_SALA; i++){
        contador[i] = 0;
    }
    while (1){
        comando = getchar();
    switch (comando){
        case 'a':
        adicionaEvento();
        break;
        
        case 'l':
        listaEventos();
        break;
        
        case 's':
        listaSalaEventos();
        getchar();
        break;
        
        case 'r':
        apagaEvento();
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




