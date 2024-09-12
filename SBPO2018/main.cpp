#include <iostream>
#include <bitset>
#include <vector>
#include <list>
#include <algorithm>
#include <time.h>
#include <stdio.h>
#include <string>

using namespace std;

#include "estruturasDados.cpp"
#include "grasp.cpp"
#include "heuristicas.cpp"
#include "vns.cpp"

void printSolution(const Solucao& solucao) {
    for (int i = 0; i < solucao.vectorBits.size(); i++) {
        if (solucao.vectorBits[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");
    for (int i = 0; i < solucao.is_elem.size(); i++) {
        if (solucao.is_elem[i]) {
            printf("%d ", i);
        }
    }
    // for (int i = 0; i < solucao.elem.size(); i++) {
    //     printf("%d ", solucao.elem[i]);
    // }
    printf("\n");
}

void execute_heuristics_greedy(){
    double tempo;
    clock_t t1, t2;
    FILE *txt;
    int valor_solucao = 0;
    double media_solucao = 0, media_tempo = 0;
    int melhor_solucao = -1, pior_solucao = tam_R;

    t1 = clock();
    Solucao solucaoH1 = heuristica1();
    valor_solucao = solucaoH1.vectorBits.count();
    t2 = clock();
    tempo = (t2 - t1)/ (double) CLOCKS_PER_SEC;
    printf("heur1 1\n");
    printSolution(solucaoH1);

    t1 = clock();
    Solucao solucaoH2 = heuristica2_path_relinking2();
    valor_solucao = solucaoH2.vectorBits.count();
    t2 = clock();
    tempo = (t2 - t1)/ (double) CLOCKS_PER_SEC;
    printf("heur2 1\n");
    printSolution(solucaoH2);
}

void execute_VNS() {
    double tempo;
    clock_t t1, t2;
    FILE *txt;
    int valor_solucao = 0;
    double media_solucao = 0, media_tempo = 0;
    int melhor_solucao = -1, pior_solucao = tam_R;

	Solucao solucaoH2 = heuristica2_path_relinking2();
    int nsol = 4;
    printf("vns %d\n", nsol);
    for(int i = 1; i <= nsol; i++){
        t1 = clock();
        Solucao solucao = solucaoH2;
        int sol_heuristica = solucao.vectorBits.count();
        if(sol_heuristica < tam_R) solucao = VNS_SO_Grasp_Aceitacao(solucao);
        valor_solucao = solucao.vectorBits.count();
        printSolution(solucao);
        t2 = clock();
        tempo = (t2 - t1)/ (double) CLOCKS_PER_SEC;

        media_tempo += tempo;
        media_solucao += valor_solucao;

        if(melhor_solucao < valor_solucao) melhor_solucao = valor_solucao;
        if(pior_solucao > valor_solucao) pior_solucao = valor_solucao;
    }
    media_tempo /= 10.0;
    media_solucao /= 10.0;
}

void execute_grasp(){
    double tempo;
    clock_t t1, t2;
    FILE *txt;
    int valor_solucao = 0;
    double media_solucao = 0, media_tempo = 0;
    int melhor_solucao = -1, pior_solucao = tam_R;

    media_solucao = 0;
    media_tempo = 0;
    melhor_solucao = -1;
    pior_solucao = tam_R;

    int nsol = 4;
    printf("grasp %d\n", nsol);
    for(int i = 1; i <= 10; i++){
        t1 = clock();
        Solucao sol = grasp_reativo();
        valor_solucao = sol.vectorBits.count();
        printSolution(sol);
        t2 = clock();
        tempo = (t2 - t1)/ (double) CLOCKS_PER_SEC;

        media_tempo += tempo;
        media_solucao += valor_solucao;

        if(melhor_solucao < valor_solucao) melhor_solucao = valor_solucao;
        if(pior_solucao > valor_solucao) pior_solucao = valor_solucao;
    }
    media_tempo /= 10.0;
    media_solucao /= 10.0;
}

int execute_all(){
    // string caminho = "cocktails_iba.txt";
    // int kmin = 64;
    // int kmax = 92;

    string caminho = "cocktails_all.txt";
    int kmin = 326;
    int kmax = 344;

    for (k = kmin; k <= kmax; ++k) {
        int tem_solucao = Ler_arquivo(caminho, 2);
        if(tem_solucao){
                printf("k=%d\n", k);
                fprintf(stderr, "k=%d\n", k);
                fprintf(stderr, " heuristic\n");
                execute_heuristics_greedy();
                fprintf(stderr, " vns\n");
                execute_VNS();
                // fprintf(stderr, " grasp\n");
                // execute_grasp();
        }
        conjuntosPrincipal.clear();
        conjuntosPrincipalOrdenado.clear();
        //conjuntosPrincipal.erase(conjuntosPrincipal.begin(),conjuntosPrincipal.end());
        //conjuntoR.erase(conjuntoR.begin(),conjuntoR.end());
    }
    return 0;
}

int main(){
    #ifdef __linux__
            srand48(time(NULL));
    #elif _WIN32
            srand(time(NULL));
    #else
            #error "OS not supported!"
    #endif
    execute_all();

    return 0;
}
