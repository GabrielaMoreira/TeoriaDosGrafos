/* Esse trabalho cont�m funcionalidades do c++11, portanto precisa ser compilado com o comando -std=c++11 (pode ser adicionado em "Compiler Options" na IDE, ou adicionado a linha de comando no terminal */

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

typedef struct g{

	vector <vector <int> > listaAdj;
	vector <vector <int> > matrizAdj;
	vector <vector <int> > matrizInc;
	vector <pair <int,int> > listaArestas;
	
}Grafo;

// A lista de Adjacencia � inicializada na leitura do arquivo (ver funcao main), e as outras estruturas sao inicializadas a partir dela.

void inicializaMatrizAdj(Grafo *G){
	
	int nNos = G->listaAdj.size(); // mesmo tamanho da lista
	
	G->matrizAdj.resize(nNos);
	for(int i=0; i<nNos; i++)
		G->matrizAdj[i].resize(nNos); // redimensiona e zera a matriz
		
	
	for(int no=0; no<nNos; no++){
		vector<int> linha = G->listaAdj[no];
	
		for(int conexao : linha)
			G->matrizAdj[no][conexao]++; // incrementa para cada conexao do no
			
	}
}

bool ehDirecionado(Grafo G){ // verifica se o grafo G do arquivo � direcionado ou n�o.
	int tamanho = G.matrizAdj.size();
	if(tamanho == 0){
		cout << "Matriz nao inicalizada" << endl;
		return false;
	}
	
	for(int i=0; i<tamanho; i++)
		for(int j=i+1; j<tamanho; j++)
			if(G.matrizAdj[i][j] != G.matrizAdj[j][i]) // se a matriz nao � sim�trica, ent�o G � direcionado.
				return true;
				
	return false;	
}

void inicializaListaArestas(Grafo *G){
	int nNos = G->listaAdj.size();
	G->listaArestas.resize(0);
	
	// A Lista de Arestas � uma lista de pares, onde o primeiro elemento do par � o v�rtice de sa�da e o segundo � o v�rtice de entrada
	
	pair <int,int> aresta;
	
	if(ehDirecionado(*G)){ // Caso G seja direcionado, todas as conexoes formam arestas �nicas
	
		for(int i=0; i<nNos; i++)
			for(int j=0; j<G->listaAdj[i].size(); j++){ // cria uma aresta com cada conexao de cada n� na lista de adjac�ncia, e coloca a aresta na lista
				aresta.first = i;
				aresta.second = G->listaAdj[i][j];
				G->listaArestas.push_back(aresta);
			}
	
	} else { // Caso G seja n�o-direcionado, olhamos apenas a parte superior da matriz de adjac�ncia, ou ter�amos arestas repetidas
		
		for(int i=0; i<nNos; i++)
			for(int j=i; j<nNos; j++){
				for(int k=0;k<G->matrizAdj[i][j];k++){ // cria uma aresta para cada conexao da matriz de adjacencia, e coloca a aresta na lista
					aresta.first = i;
					aresta.second = j;
					G->listaArestas.push_back(aresta);
				}
			}
	}
				
}

void inicializaMatrizInc(Grafo *G){
	
	int nNos = G->listaAdj.size();
	int nArestas = G->listaArestas.size();
	
	G->matrizInc.resize(nArestas); // redimensionando a matriz de Incid�ncia: n�mero de linhas � o n�mero de arestas, n�mero de colunas em cada linha � o n�mero de n�s do grafo
	for(int i=0; i<nArestas; i++)
		G->matrizInc[i].resize(nNos);
		
	int nSaida;
	if(ehDirecionado(*G)) // se for direcionado, preenchemos com -1 e 1. Caso n�o, s� incrementamos 1.
		nSaida = -1;
	else
		nSaida = 1;
	
	for(int i=0;i<nArestas;i++){ // para cada aresta na lista, preenche uma linha da matriz com as conex�es
		G->matrizInc[i][G->listaArestas[i].first]++;
		G->matrizInc[i][G->listaArestas[i].second]+=nSaida;		
	}
}

int grauNo(int no, Grafo G){
	int dIn = 0;
	int dOut = G.listaAdj[no].size(); // quantas conex�es o n� tem � o mesmo que o n�mero de elementos referentes a ele na lista de adjacencia (que seria a mesma coisa que a soma da linha da matriz)
	
	if(ehDirecionado(G)) // se for direcionado, conta o grau de entrada (dIn) e soma com o grau de sa�da. 
		for(int i=0;i<G.matrizAdj.size();i++)
			dIn += G.matrizAdj[i][no];
			
	return (dIn + dOut); // se for n�o-direcionado, dIn = 0 e dOut = d
}

void inserirNo(Grafo *G){
	char cmanter='?', ch = '?'; // vari�veis para a entrada de dados
	bool manter = false;
	
	int conexao, no = G->listaAdj.size();
	cout << "Criando no " << no << ". Digite as conexoes: ";
	fflush(stdin);
	
	vector<int> linha;
	linha.resize(0);
	
	while(ch != '\n'){ // l� at� uma quebra de linha
	
		cin >> conexao;
		linha.push_back(conexao);
			
		cin >> noskipws >> ch; // noskipws = no skip whitespaces -> usado para verificar se houve quebra de linha
	}
	
	G->listaAdj.push_back(linha); // insere o novo n� na lista
		
	if(!ehDirecionado(*G)){
		cout << "Conexoes bidirecionais? <1 - Sim, 0 - Nao>: "; // o usu�rio pode escolher manter o grafo direcionado, fazendo uma unica inser��o com conex�es bidirecionais
		cin >> cmanter;
		if (cmanter=='1')
			manter = true;
		else
			manter = false;
	}
	
	for(int i=0; i<no; i++) // insere uma coluna vazia (ou seja, um 0 ao final de cada linha da matriz)
		G->matrizAdj[i].push_back(0); 
		
	if(manter){
		for(int conexao : linha){
			G->listaAdj[conexao].push_back(no); // cria uma conexao dos n�s antigos para o novo n� para cada conexao inserida, tanto na matriz quanto na lista
			G->matrizAdj[conexao][no]++;
		}
	}
	
	vector<int> linhaM;
	linhaM.resize(no+1);
	
	for(int conexao : linha) // insere as novas conex�es em uma linha de matriz
		linhaM[conexao]++; 
	
	G->matrizAdj.push_back(linhaM); // insere a linha no final da matriz
	
}

void removerNo(Grafo *G){
	int no;
	cout << "No a ser removido: ";
	fflush(stdin);
	cin >> no;
	
	// remove da matriz
	G->matrizAdj.erase(G->matrizAdj.begin() + no); // apaga linha
	
	for (int i=0;i<G->matrizAdj.size();i++) // apaga coluna (n-�simo elemento de cada linha)
		G->matrizAdj[i].erase(G->matrizAdj[i].begin() + no);
	
	cout << "ok\n";
	// remove da lista
	
	G->listaAdj.erase(G->listaAdj.begin() + no); // remove a linha referente ao n�
	
	for(int i=0;i<G->listaAdj.size();i++)
		for(int j=0;j<G->listaAdj[i].size();j++){
			if(G->listaAdj[i][j] == no) // remove as conex�es para aquele n�
				G->listaAdj[i].erase(G->listaAdj[i].begin() + j);
			else if (G->listaAdj[i][j] > no) // decrementa os n�s com �ndice maior que o n� removido para manter a consist�ncia
				G->listaAdj[i][j]--;
		}
}

/**** In�cio Fun��es de leitura ****/

void leGrafo(string grafo, Grafo *G){
	
	int no,conexao;
	char ch,op = '?';
	
	ifstream arquivo (grafo + ".txt");
	
    if (arquivo.is_open()){
        
        while(arquivo >> no){ // l� n�s enquanto houverem linhas no arquivo
		
			arquivo >> ch;
			arquivo >> noskipws >> ch; 
			// noskipws = no skip whitespaces -> usado para verificar se houve quebra de linha
			
			vector<int> linha;
			linha.resize(0);
			
			while(ch != '\n'){ // l� conex�es at� uma quebra de linha
				
				arquivo >> conexao;
				linha.push_back(conexao); // coloca todas as conexoes em um vetor din�mico "linha"
					
				arquivo >> noskipws >> ch;
			}
			
			G->listaAdj.push_back(linha); // insere o vetor com as conexoes na lista de adjac�ncia
		}
		
        arquivo.close();
        
        cout << "Grafo " << grafo << " carregado." << endl;
        
	}
}

/**** Fim Fun��es de Leitura ****/

/**** In�cio Fun��es de impress�o ****/

void imprimeMatriz(vector < vector<int> > matriz){

	for(auto linha : matriz){
		for(int n : linha){
			cout << n << " ";
		}
		cout << endl;
	}
}

void imprimeListaAdj(Grafo G){
	for(int no=0; no<G.listaAdj.size(); no++){
		cout << no << ": ";
		for(int conexao : G.listaAdj[no])
			cout << conexao << " ";
		cout << endl;
	}
}

void imprimeGrauDeCadaNo(Grafo G){	
	for(int i=0;i<G.listaAdj.size();i++)
		cout << "d(" << i << ") = " << grauNo(i,G) << endl;
	cout << endl;
}

void imprimeComplemento(Grafo G){
	int n = G.matrizAdj.size();
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			if(i==j)
				cout << 0 << " ";
			else
				cout << !G.matrizAdj[i][j] << " ";
		}
		cout << endl;
	}
}

void imprimeListaArestas(Grafo G){
	for (auto aresta : G.listaArestas)
		cout << "(" << aresta.first << "," << aresta.second << ") ";
	cout << endl;
}

void mostraMenu(){
	cout << endl;
	cout << "Menu" << endl;
	cout << "[0] Encerrar\n[1] Matriz de Adjacencia\n[2] Lista de Adjacencia\n[3] Grau de Cada No\n[4] Inserir No\n[5] Remover No\n[6] Complemento do Grafo\n[7] Matriz de Incidencia\n[8] Lista de Arestas" << endl;	
	cout << endl;
}

/**** Fim Fun��es de impress�o ****/

int main(){
	int no,conexao;
	char ch,op = '?';
	
	Grafo G1, G2;
	
	G1.listaAdj.resize(0);
	G2.listaAdj.resize(0);
	
	leGrafo("G1",&G1);
	leGrafo("G2",&G2);
	cout << G1.listaAdj.size() << endl;
	
	/* Inicializa as outras estruturas de dados para G1 */
	inicializaMatrizAdj(&G1);
	inicializaListaArestas(&G1);
	inicializaMatrizInc(&G1);
	
	/* Se houver G2, inicializa as outras estruturas */
	if(G2.listaAdj.size()>0){
				
		inicializaMatrizAdj(&G2);
		inicializaListaArestas(&G2);
		inicializaMatrizInc(&G2);
		
	}
	
	/* Intera��o do menu em loop */
	while(op != '0'){
		mostraMenu();
		fflush(stdin);
		cin >> op;
		switch (op){
			case '1':
				cout << "Matriz de Adjacencia: " << endl;
				imprimeMatriz(G1.matrizAdj);
				break;
			case '2':
				cout << "Lista de Adjacencia: " << endl;
				imprimeListaAdj(G1);
				break;
			case '3':
				cout << "Grau de cada no:" << endl;	
				imprimeGrauDeCadaNo(G1);
				break;
			case '4':
				inserirNo(&G1);
				cout << "No inserido!" << endl;	
				break;
			case '5':
				removerNo(&G1);
				cout << "No removido!" << endl;	
				break;
			case '6':
				cout << "Matriz de Adjacencia do complemento de G1: " << endl;
				imprimeComplemento(G1);
				break;
			case '7':
				cout << "Matriz de Incidencia: " << endl;
				imprimeMatriz(G1.matrizInc);
				break;
			case '8':
				cout << "Lista de Arestas: " << endl;
				imprimeListaArestas(G1);
				break;
				
				
		}
	}
	
	/* Salva altera��es */
	ofstream saida ("G1.txt");
	
    if (saida.is_open()){
    	for(int no=0; no<G1.listaAdj.size(); no++){
			saida << no << ":";
			for(int conexao : G1.listaAdj[no])
				saida << " " << conexao;
			saida << endl;
		}
		saida.close();
	}
	
	
	
	return 0;
}
