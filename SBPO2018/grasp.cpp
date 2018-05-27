

//#define DEBUG
#define NUMBER_VALUES 11

//int NU_VALUES = 11;
double cons_X[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
double prob_X[NUMBER_VALUES];
int cont[NUMBER_VALUES];
double score[NUMBER_VALUES];
double avg[NUMBER_VALUES];
int qtd_max_iteracoes = 500;



/**
Dado um vetor representando um subconjunto de L e um inteiro p,
ele devolve o indice de L do elemento na posi��o p nesse vetor.
**/
int get_subconjunto(vector<bool> vetor, int p){
    int q = 0;
    for (int j = 0; j < vetor.size(); j++){
        if(vetor[j] == true){
            if(q == p) return j;

            q++;
        }
    }
    return -1;
}


int melhor_solucao_construcao = -1;
/**Fun��o que controe uma solu��o vi�vel para o grasp.**/

Solucao construcao(double alfa){
    Solucao solucao;//Solu��o sendo construida.

    vector<bool> lrc;//Lista Restrita de Candidatos contendo os subconjuntos que n�o est�o na solu��o e que podem entrar.
    //Custo incremental representando o valor da interse��o se o subconjunto j entrar na solu��o.
    int custo_incremental[tam_L], c_min, c_max;

    for (int j = 0; j < tam_L; j++){
        solucao.is_elem.push_back(false);
        lrc.push_back(false);

        custo_incremental[j] = conjuntosPrincipal[j].vectorBits.count();//Inicialmente o custo incremental � igual a quantidade de elementos do subconjunto. Supondo que a solu��o tenha apenas esse subconjunto.
        if(j == 0){
            c_min = custo_incremental[j];
            c_max = custo_incremental[j];
        }else{
            if(c_min > custo_incremental[j]) c_min = custo_incremental[j];
            if(c_max < custo_incremental[j]) c_max = custo_incremental[j];
        }
    }
    solucao.vectorBits.set();

    int i = 0;
    while(i < k){
        /*
        Inferior indica o menor valor de custo que um sunconjunto deve ter para entra na lista lrc, ou seja,
        somente subconjuntos com custo incremental maior ou igual a inferior entrar�o na lista, e que n�o estejam na solu��o.
        */
        int inferior = c_min + alfa*(c_max - c_min);
        int tam_lrc = 0;//Quantidade de subconjuntos na lista lrc. Uso para selecionar um subconjunto da lista de forma aleat�ria.
        for (int j = 0; j < tam_L; j++){
            //Crio a lista lrc.
            if(solucao.is_elem[j] == false && custo_incremental[j] >= inferior){
                lrc[j] = true;
                tam_lrc++;
            }else lrc[j] = false;
        }

        /*
        Seleciona de forma aleat�ria um subconjunto da lista lrc.
        Chamo o "randValor(0, tam_lrc - 1)" que escolhe um n�mero de 0 a tamanho da lista lrc.
        Depois chamo a fun��o "int get_subconjunto(vector<bool> vetor, int p)" que devolve o �ndice do subconjunto escolhido da lista lrc.
        */
        int s = get_subconjunto(lrc, randValor(0, tam_lrc - 1));

        #ifdef DEBUG
            cout << "subconjunto escolhido = " << s << endl;
            cout << "custo incremental     = " << custo_incremental[s] << endl;
        #endif

        solucao.is_elem[s] = true;//Indico que est� na solu��o
        solucao.elem.push_back(s);//Add no vetor
        solucao.vectorBits = Interseccao(solucao.vectorBits, conjuntosPrincipal[s].vectorBits);//Gero a interse��o da solu��o parcial.

        if(i + 1 == k) break;

        //Atualizo os custos incrementais.
        int t = 0;
        for (int j = 0; j < tam_L; j++){
            if(solucao.is_elem[j] == false){
                bitset<nElem> vetor_aux = Interseccao(solucao.vectorBits, conjuntosPrincipal[j].vectorBits);
                custo_incremental[j] = vetor_aux.count();

                if(t == 0){
                    c_min = custo_incremental[j];
                    c_max = custo_incremental[j];
                }else{
                    if(c_min > custo_incremental[j]) c_min = custo_incremental[j];
                    if(c_max < custo_incremental[j]) c_max = custo_incremental[j];
                }
                t++;
            }
        }

        i++;
    }
    return solucao;
}
/**
Realiza a busca local. Recebe uma solu��o vi�vel e busca uma troca que melhore a solu��o.
A troca um subconjunto da solu��o por outro n�o pertecente a solu��o.
Devolve a primeira solu��o que melhora.
**/
void busca_local(Solucao &solucao){
    Solucao solucao_linha;//Solu��o ap�s a troca
    for (int j = 0; j < solucao.elem.size(); j++){
        int s = solucao.elem[j];//Subconjunto da solu��o que ir� ser removido na troca.
        int s_linha;//Subconjunto que ira entrar na troca.
        for (int jj = 0; jj < tam_L; jj++){
            if(solucao.is_elem[jj] == false){
                s_linha = jj;

                solucao_linha.limpar();//Zera os dados da solu��o.

                /*Aqui eu gero a interse��o sem o subconjunto s que ir� ser removido na troca.*/
                for (int q = 0; q < solucao.elem.size(); q++){
                    if(q != j){
                        solucao_linha.vectorBits = Interseccao(solucao_linha.vectorBits, conjuntosPrincipal[solucao.elem[q]].vectorBits);
                        solucao_linha.elem.push_back(solucao.elem[q]);
                        solucao_linha.is_elem[solucao.elem[q]] = true;
                    }
                }
                //Realizo a troca.
                solucao_linha.vectorBits = Interseccao(solucao_linha.vectorBits, conjuntosPrincipal[s_linha].vectorBits);
                solucao_linha.elem.push_back(s_linha);
                solucao_linha.is_elem[s_linha] = true;

                if(solucao_linha.vectorBits.count() > solucao.vectorBits.count()){//Comparo se com a troca houve melhoria na solu��o.
                    solucao.vectorBits = solucao_linha.vectorBits;
                    solucao.elem = solucao_linha.elem;
                    solucao.is_elem = solucao_linha.is_elem;
                    return;
                }
            }
        }
    }
}

/**
Diferen�a simetrica de sol1 para sol2, ou seja, quantos movimentos(trocas) s�o necess�rios para transformar sol1 na sol2.
**/
int diferenca_simetrica(const Solucao &sol1, const Solucao &sol2){
    int iguais = 0;
    for(int j = 0; j < sol1.elem.size(); j++){
        int q = sol1.elem[j];
        if(sol2.is_elem[q] == true) iguais++;
    }
    return (k - iguais);
}

/**
Path Relinking ele recebe duas solu��es, origem e destino, e realiza trocas que transforma a solu��o origem na destino,
sendo que a cada troca verificar se melhorou o valor da solu��o.
**/
vector<Solucao> solucoes_elites;//Lista contendo as possiveis solu��es destinos.
int max_elite = 10;//Tamanho m�ximo da lista elite.
Solucao path_relinking(const Solucao &solucao){
    Solucao origem = solucao;//A solu��o passado como par�metro ser� a solu��o origem.

    //Seleciono a solu��o destino de forma aleat�ria.
    int s = randValor(0, solucoes_elites.size() - 1);
    Solucao destino = solucoes_elites[s], melhor_solucao;

    //Melhor solu��o inicial ser� entre a solu��o origem e destino.
    int valor_melhor_solucao;
    if(origem.vectorBits.count() > destino.vectorBits.count()){
        melhor_solucao = origem;
        valor_melhor_solucao = origem.vectorBits.count();
    }
    else{
        melhor_solucao = destino;
        valor_melhor_solucao = destino.vectorBits.count();
    }

    int difsi = diferenca_simetrica(origem, destino);
    while(difsi > 0){//Enquanto a solu��o origem n�o foi transformada na solu��o destino, continue.
        /*Bitset com a Interse��o dos subconjuntos que est�o ao mesmo tempo da origem e destino. Ou caras que ir�o permanecer na origem*/
        bitset<nElem> inter_origem_destino;

        inter_origem_destino.set();
        vector<int> only_origem, only_destino;//Subconjuntos que precisam sair/entrar na origem, respectivamente.

        //Gero as lista que utilizo.
        for(int j = 0; j < destino.elem.size(); j++){
            int q = destino.elem[j];
            if(origem.is_elem[q] == false) only_destino.push_back(q);
        }
        for(int j = 0; j < origem.elem.size(); j++){
            int q = origem.elem[j];

            if(destino.is_elem[q] == false) only_origem.push_back(q);
            else inter_origem_destino = Interseccao(inter_origem_destino, conjuntosPrincipal[q].vectorBits);//Interse��o com os subconjuntos da origem que tbm est�o no destino.
        }

        int melhor_movimento = -1, origem_sair, origem_entrar;//Valor do melhor movimento, subconjunto de vai sair e entrar na origem para o melhor movimento.
        bitset<nElem> bitset_movimento;
        for(int i = 0; i < only_origem.size(); i++){
            int p = only_origem[i];//Elemento que ir� sair da origem.

            //Aqui gero a interse��o com todos os elementos da origem sem o subconjunto p.
            bitset<nElem> vetor_aux = inter_origem_destino;
            for(int ii = 0; ii < only_origem.size(); ii++){
                if(p != only_origem[ii]){
                    vetor_aux = Interseccao(vetor_aux, conjuntosPrincipal[ only_origem[ii] ].vectorBits);
                }
            }
            //Para cada subconjunto que est� na solu��o destino e n�o na origem, calculo o custo do movimento.
            for(int j = 0; j < only_destino.size(); j++){
                int q = only_destino[j];

                bitset<nElem> movimento = Interseccao(vetor_aux, conjuntosPrincipal[ q ].vectorBits);
                int aux = movimento.count();
                if(melhor_movimento < aux){
                    melhor_movimento = movimento.count();
                    origem_sair = p;
                    origem_entrar = q;
                    bitset_movimento = movimento;
                }
            }
        }
        //Efetivo o melhor movimento.
        origem.vectorBits = bitset_movimento;
        origem.is_elem[origem_entrar] = true;
        origem.is_elem[origem_sair] = false;
        for(int i = 0; i < origem.elem.size(); i++){
            if(origem.elem[i] == origem_sair){
                origem.elem[i] = origem_entrar;
                break;
            }
        }

        if(origem.vectorBits.count() > valor_melhor_solucao){//Melhorou?
            melhor_solucao = origem;
            valor_melhor_solucao = origem.vectorBits.count();
        }
        difsi = diferenca_simetrica(origem, destino);

        //printf("difsi: %d \n", difsi);
    }
    return melhor_solucao;
}

int get_alfa(){
    double prob_acumulativa[11];
    prob_acumulativa[0] = prob_X[0];
    for(int w = 1; w < 11; w++) prob_acumulativa[w] = prob_acumulativa[w - 1] + prob_X[w];

    double x;
    #ifdef __linux__
        x = drand48();//Linux
    #elif _WIN32
        x = (double)rand()/RAND_MAX;//Windons
    #else
		#error "OS not supported!"
    #endif

    for(int i = 0; i < 11; i++) if(x <= prob_acumulativa[i]) return i;
}


void atualizacao_probabilidade(int Z_max){
	double Q[NUMBER_VALUES];
	double sigma = 0.0;

	#ifdef DEBUG
        cout << "ATUALIZACAO VETOR DE PROBABILIDADES " << endl;
	#endif


	bool falha = false;

	for(int k = 0; k < NUMBER_VALUES; k++){
		if(cont[k] == 0) {
			#ifdef DEBUG
                printf("FALHOU\n");
			#endif
			falha = true;
			break;
		}
		avg[k] = score[k]/cont[k];
		//Q[k] = melhor_solucao.vectorBits.count()/avg[k];
		Q[k] = avg[k]/Z_max;

		sigma += Q[k];

		#ifdef DEBUG
            printf("valor %d media %lf q %lf\n", k, avg[k], Q[k]);
		#endif

	}
	if(!falha){
        #ifdef DEBUG
            printf("PROCESSO BEM SUCEDIDO\n");
        #endif
        for(int k = 0; k < NUMBER_VALUES; k++){
            prob_X[k] = Q[k]/sigma;
            #ifdef DEBUG
                printf("prob[%d] = %lf\n", k, Q[k]/sigma);
            #endif

        }
	}else{
		#ifdef DEBUG
            printf("PROCESSO FALHOU\n");
		#endif
	}
}

Solucao grasp(){
    #ifdef DEBUG
        cout << "-----------GRASP------" << endl;
    #endif

    solucoes_elites.clear();

    int custo_melhor_solucao = 0;

    Solucao melhor_solucao;

    melhor_solucao_construcao = -1;

    bool apicar_buscar_path = true;

    //Inicializando os valores iniciais do grasp reativo
    for(int i = 0; i < NUMBER_VALUES; i++){
			cont[i] = 0;
			score[i] = 0.0;
			prob_X[i] = 1.0/NUMBER_VALUES;
    }

    for(int i = 0; i < qtd_max_iteracoes; i++){

        #ifdef DEBUG
			cout << "Iteracao: " << i << endl;
        #endif

        int indice_alfa = get_alfa();
        double alfa = cons_X[indice_alfa];

        #ifdef DEBUG
            cout << "alpha = " << alfa << endl;
            cout << "FASE CONSTRUCAO " << endl;
        #endif


        Solucao solucao = construcao(alfa);


        #ifdef DEBUG
            cout << "Solucao size construcao : "  << solucao.vectorBits.count() << endl;
        #endif



        //No artigo eles relatam que s� aplicam a busca local e path relinking
        //se a solu��o obtida pela constru��o � pelo menos 80% da melhor solu��o encontrada at� agora.
        if(solucao.vectorBits.count() >= 0.8*melhor_solucao.vectorBits.count()) {

					busca_local(solucao);

					/*
					Aqui implemento a maneira que a solu��o encontrada na busca local ser� inserida na lista elite.
					Quando a quantidade m�xima n�o � alcan�ada, adiciona na lista elite.
					Quando a quantidade m�xima j� � alcan�ada, trocamos a solu��o da lista elite mais parecida com a solu��o
					e que tenha custo pior que da solu��o.

					Nao coloquei isso dentro da propria fun��o path_relinking, porque achei que ficaria mais complicado o retorno
					da fun��o path_relinking.
					*/

					if(solucoes_elites.size() >= 1){
							solucao = path_relinking(solucao);
					}
					//Atualizo a lista elite.
					if(solucoes_elites.size() < max_elite) solucoes_elites.push_back(solucao);
					else{
							int menor_dif = k + 12;
							int posicao_menor_dif = -1;
							for(int j = 0; j < solucoes_elites.size(); j++){
									if(solucao.vectorBits.count() > solucoes_elites[j].vectorBits.count()){
											int dif = diferenca_simetrica(solucao, solucoes_elites[j]);
											if(menor_dif > dif){
													menor_dif = dif;
													posicao_menor_dif = j;
											}
									}
							}
							if(posicao_menor_dif != -1){
									solucoes_elites[posicao_menor_dif] = solucao;
							}
                    }
        }

        if(i == 0) melhor_solucao = solucao;
        else if(solucao.vectorBits.count() > melhor_solucao.vectorBits.count()){
                melhor_solucao = solucao;
                if(melhor_solucao.vectorBits.count() == tam_R) break;
        }

        cont[indice_alfa]++;
		score[indice_alfa] += solucao.vectorBits.count();

        #ifdef DEBUG
            printf("cont[%d] = %d, score[%d] = %lf\n", indice_alfa, cont[indice_alfa], indice_alfa, score[indice_alfa]);
        #endif


        #ifdef DEBUG
            cout << "Solucao size final : "  << solucao.vectorBits.count() << endl;
            cout << "--------------------------------------" << endl;
        #endif
    }
    return melhor_solucao;
}



Solucao grasp_reativo(){
    #ifdef DEBUG
        cout << "-----------GRASP------" << endl;
    #endif

    solucoes_elites.clear();

    int custo_melhor_solucao = 0;

    Solucao melhor_solucao;

    melhor_solucao_construcao = -1;

    bool apicar_buscar_path = true;

    //Inicializando os valores iniciais do grasp reativo
    for(int i = 0; i < NUMBER_VALUES; i++){
        cont[i] = 0;
        score[i] = 0.0;
        prob_X[i] = 1.0/NUMBER_VALUES;
    }

    for(int i = 0; i < qtd_max_iteracoes; i++){
        #ifdef DEBUG
			cout << "Iteracao: " << i << endl;
        #endif

        int indice_alfa = get_alfa();
        double alfa = cons_X[indice_alfa];

        #ifdef DEBUG
            cout << "alpha = " << alfa << endl;
            cout << "FASE CONSTRUCAO " << endl;
        #endif

        Solucao solucao = construcao(alfa);

        #ifdef DEBUG
            cout << "Solucao size construcao : "  << solucao.vectorBits.count() << endl;
        #endif



        //No artigo eles relatam que s� aplicam a busca local e path relinking
        //se a solu��o obtida pela constru��o � pelo menos 80% da melhor solu��o encontrada at� agora.
        if(solucao.vectorBits.count() >= 0.8*melhor_solucao.vectorBits.count()) {

					busca_local(solucao);

					/*
					Aqui implemento a maneira que a solu��o encontrada na busca local ser� inserida na lista elite.
					Quando a quantidade m�xima n�o � alcan�ada, adiciona na lista elite.
					Quando a quantidade m�xima j� � alcan�ada, trocamos a solu��o da lista elite mais parecida com a solu��o
					e que tenha custo pior que da solu��o.

					Nao coloquei isso dentro da propria fun��o path_relinking, porque achei que ficaria mais complicado o retorno
					da fun��o path_relinking.
					*/

					if(solucoes_elites.size() >= 1){
							solucao = path_relinking(solucao);
					}
					//Atualizo a lista elite.
					if(solucoes_elites.size() < max_elite) solucoes_elites.push_back(solucao);
					else{
							int menor_dif = k + 12;
							int posicao_menor_dif = -1;
							for(int j = 0; j < solucoes_elites.size(); j++){
									if(solucao.vectorBits.count() > solucoes_elites[j].vectorBits.count()){
											int dif = diferenca_simetrica(solucao, solucoes_elites[j]);
											if(menor_dif > dif){
													menor_dif = dif;
													posicao_menor_dif = j;
											}
									}
							}
							if(posicao_menor_dif != -1){
                                solucoes_elites[posicao_menor_dif] = solucao;
							}
					}

        }

        if(i == 0) melhor_solucao = solucao;
        else if(solucao.vectorBits.count() > melhor_solucao.vectorBits.count()){
            melhor_solucao = solucao;
            if(melhor_solucao.vectorBits.count() == tam_R) break;
        }

        cont[indice_alfa]++;
		score[indice_alfa] += solucao.vectorBits.count();

        #ifdef DEBUG
			printf("cont[%d] = %d, score[%d] = %lf\n", indice_alfa, cont[indice_alfa], indice_alfa, score[indice_alfa]);
		#endif


        if(i % NUMBER_VALUES == 0){
            atualizacao_probabilidade(melhor_solucao.vectorBits.count());
        }

        #ifdef DEBUG
            cout << "Solucao size final : "  << solucao.vectorBits.count() << endl;
            cout << "--------------------------------------" << endl;
        #endif
    }
    return melhor_solucao;
}

void atualizacao_probabilidade2(int Z_min){
	double Q[NUMBER_VALUES];
	double sigma = 0.0;

	#ifdef DEBUG
	cout << "ATUALIZACAO VETOR DE PROBABILIDADES " << endl;
	#endif


	bool falha = false;

	for(int k = 0; k < NUMBER_VALUES; k++){
		if( cont[k] == 0 && avg[k] > 0.0000001 ) {
			#ifdef DEBUG
			printf("FALHOU\n");
			#endif
			falha = true;
			break;
		}
		avg[k] = score[k]/cont[k];
		Q[k] = Z_min/avg[k];

		sigma += Q[k];

		#ifdef DEBUG
		printf("valor %d media %lf q %lf\n", k, avg[k], Q[k]);
		#endif

	}
	if(!falha){
			#ifdef DEBUG
			printf("PROCESSO BEM SUCEDIDO\n");
			#endif
			for(int k = 0; k < NUMBER_VALUES; k++){
				prob_X[k] = Q[k]/sigma;

				#ifdef DEBUG
				printf("prob[%d] = %lf\n", k, Q[k]/sigma);
				#endif

			}
	}else{
		#ifdef DEBUG
		printf("PROCESSO FALHOU\n");
		#endif
	}


}

Solucao grasp_reativo2(){

    #ifdef DEBUG
        cout << "-----------GRASP------" << endl;
    #endif

    solucoes_elites.clear();

    int custo_melhor_solucao = 0;

    Solucao melhor_solucao;

    melhor_solucao_construcao = -1;

    bool apicar_buscar_path = true;


    //Inicializando os valores iniciais do grasp reativo
    for(int i = 0; i < NUMBER_VALUES; i++){
			cont[i] = 0;
			score[i] = 0.0;
			prob_X[i] = 1.0/NUMBER_VALUES;
    }

    for(int i = 0; i < qtd_max_iteracoes; i++){
        #ifdef DEBUG
            cout << "Iteracao: " << i << endl;
        #endif

        int indice_alfa = get_alfa();
        double alfa = cons_X[indice_alfa];

        #ifdef DEBUG
            cout << "alpha = " << alfa << endl;
            cout << "FASE CONSTRUCAO " << endl;
        #endif

        Solucao solucao = construcao(alfa);

        #ifdef DEBUG
            cout << "Solucao size construcao : "  << solucao.vectorBits.count() << endl;
        #endif



        //No artigo eles relatam que s� aplicam a busca local e path relinking
        //se a solu��o obtida pela constru��o � pelo menos 80% da melhor solu��o encontrada at� agora.
        if(solucao.vectorBits.count() >= 0.8*melhor_solucao.vectorBits.count()) {

					busca_local(solucao);

					/*
					Aqui implemento a maneira que a solu��o encontrada na busca local ser� inserida na lista elite.
					Quando a quantidade m�xima n�o � alcan�ada, adiciona na lista elite.
					Quando a quantidade m�xima j� � alcan�ada, trocamos a solu��o da lista elite mais parecida com a solu��o
					e que tenha custo pior que da solu��o.

					Nao coloquei isso dentro da propria fun��o path_relinking, porque achei que ficaria mais complicado o retorno
					da fun��o path_relinking.
					*/

					if(solucoes_elites.size() >= 1){
							solucao = path_relinking(solucao);
					}
					//Atualizo a lista elite.
					if(solucoes_elites.size() < max_elite) solucoes_elites.push_back(solucao);
					else{
							int menor_dif = k + 12;
							int posicao_menor_dif = -1;
							for(int j = 0; j < solucoes_elites.size(); j++){
									if(solucao.vectorBits.count() > solucoes_elites[j].vectorBits.count()){
											int dif = diferenca_simetrica(solucao, solucoes_elites[j]);
											if(menor_dif > dif){
													menor_dif = dif;
													posicao_menor_dif = j;
											}
									}
							}
							if(posicao_menor_dif != -1){
									solucoes_elites[posicao_menor_dif] = solucao;
							}
					}

				}

        if(i == 0) melhor_solucao = solucao;
        else if(solucao.vectorBits.count() > melhor_solucao.vectorBits.count()){
                melhor_solucao = solucao;
                if(melhor_solucao.vectorBits.count() == tam_R) break;
        }

        cont[indice_alfa]++;
        score[indice_alfa] += solucao.vectorBits.count();

        #ifdef DEBUG
            printf("cont[%d] = %d, score[%d] = %lf\n", indice_alfa, cont[indice_alfa], indice_alfa, score[indice_alfa]);
        #endif


        if(i%NUMBER_VALUES == 0){
            atualizacao_probabilidade2(melhor_solucao.vectorBits.count());
        }

        #ifdef DEBUG
            cout << "Solucao size final : "  << solucao.vectorBits.count() << endl;
            cout << "--------------------------------------" << endl;
        #endif
    }
    return melhor_solucao;
}
