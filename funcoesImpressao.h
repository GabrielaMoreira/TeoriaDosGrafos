#include "estruturas.h"
using namespace std;

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
		cout << "(" << aresta.x << "," << aresta.y << ") [" << aresta.peso << "]\n";
	cout << endl;
}

void mostraMenu(){
	cout << endl;
	cout << "Menu" << endl;
	cout << "[0] Encerrar\n[1] Matriz de Adjacencia\n[2] Lista de Adjacencia\n[3] Grau de Cada No\n[4] Inserir No\n[5] Remover No\n[6] Complemento do Grafo\n[7] Matriz de Incidencia\n[8] Lista de Arestas\n[9] Conexidade do Grafo\n[10] Quantidade de Componentes\n[11] Circuito Euleriano\n[12] Dijkstra\n[13] BFS\n[14] DFS\n[15] Kruskal" << endl;	
	cout << endl;
}

