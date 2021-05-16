#include <iostream>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <time.h>

using namespace std;

struct DSU // система непересекающихся множеств
{
	int *parent; // родитель он же представитель - единственный из множества
	int *rnk; // ранг, он же длина
	int n; // кол-во изначальных эл-тов

	DSU(int n) // конструктор, создаём для каждого эл-та свое множество
	{
		this->n = n;
		parent = new int[n + 1];
		rnk = new int[n + 1];

		for (int i = 0; i <= n; ++i)
		{
			rnk[i] = 0;
			parent[i] = i;
		}
	}

	int DSUfind(int u) // ищем родителя
	{
		if (u == parent[u])
			return parent[u];
		else
			return DSUfind(parent[u]);
	}

	void DSUunion(int x, int y) // объединяем два множества
	{
		x = DSUfind(x);
		y = DSUfind(y);
		if (x != y)
		{
			if (rnk[x] < rnk[y])
			{
				rnk[y] += rnk[x];
				parent[x] = y;
			}
			else
			{
				rnk[x] += rnk[y];
				parent[y] = x;
			}
		}
	}
};

class graph //класс графа
{
	private:
		
		int V; // число вершин
		int E; // число ребёр
		vector <pair <int, pair <int, int> > > edges; // сам граф, по сути храним массив-вектор пары, где эл-т пары - инт(вес ребра) и еще пара(вершины на концах ребра)
		vector <pair <int, pair <int, int> > > MST; // тоже граф, но только уже то конкретное наше минимальное остовное дерево(такой же структруры)
	
	public:

		graph(int V, int E) // конструктор, добавляем кол-во ребёр + кол-во вершин
		{
			this->V = V; 
			this->E = E;
		}
		
		void addEdge(int u, int v, int w) // пихаем ребро в граф
		{
			edges.push_back({ w, {u, v} });
		}

		int KruskalMST() // сам алг-тм Крускала
		{
			int MSTweight = 0; // вес всех рёбер в МОД
			sort(edges.begin(), edges.end()); // сортируем рёбра по весу
			DSU dsu(this->V); // создаём СНМ по кол-ву вершин в нашем графе

			vector<pair <int, pair<int, int> > >::iterator it; // итератор, с помощью него проходим по всем рёбрам(они уже отсортированы)
			for (it = edges.begin(); it != edges.end(); ++it)
			{
				int u = it->second.first;
				int v = it->second.second;

				int setU = dsu.DSUfind(u);
				int setV = dsu.DSUfind(v);

				if (setU != setV)
				{
					int w = it->first;
					MST.push_back({ w, {u, v} });
					MSTweight += it->first;

					dsu.DSUunion(setU, setV);
				}
			}

			return MSTweight;
		}
		
		void printMST() // выводим наше МОД с помощью итератора, который проходит от начала и до конца
		{
			vector<pair <int, pair<int, int> > >::iterator it; // итератор, с помощью него проходим от начала до конца МОД
			for (it = MST.begin(); it != MST.end(); ++it)
				cout << it->second.first << " - " << it->second.second << endl; // выводим эл-ты пары второго эл-та пары - то есть вершины на обоих концах рёбер!
		}

};

int main()
{
	setlocale(0, "");
	
	int V;
	int E;

	cout << "Введите кол-во вершин: ";
	cin >> V;

	cout << "\nВведите кол-во рёбер: ";
	cin >> E;

	graph g(V, E);

	int u;
	int v;

	int w;

	unsigned short choice;
	cout << "\nРандомное заполнение : 1\nВручную : 2\n";
	cin >> choice;
	system("cls");
	
	if (choice == 1)
	{
		srand(time(NULL));
		
		for (int i = 0; i < E; ++i)
		{
			u = rand() % V;
			v = rand() % V;

			w = rand() % 100;

			g.addEdge(u, v, w);
		}
	}

	if (choice == 2)
	{
		cout << "Ввод графа осуществляется следующим образом:\nВводится ребро, сначала его вес, потом две вершины, которые у него по бокам.\n";

		for (int i = 0; i < E; ++i)
		{
			cout << "\nВвод ребра номер " << i + 1 << ": ";
			cin >> w;
			cin >> u;
			cin >> v;

			g.addEdge(u, v, w);
		}
	}

	unsigned time1 = clock();
	int weightofMST = g.KruskalMST();
	unsigned time2 = clock();
	unsigned time = time2 - time1;

	cout << "Минимальное остовное дерево: \n";
	g.printMST();

	cout << "\n\nВес минимального остовного дерева равен " << weightofMST << ".";
	cout << "\nВремя работы алгоритма " << time << " мс.";

	_getch();
	return 0;
}