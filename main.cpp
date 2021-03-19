#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;

const int MAX_N = 10000;

long long dis[MAX_N];
int pre[MAX_N], len[MAX_N];
bool in_queue[MAX_N];

bool visited[MAX_N], on_stack[MAX_N];
std::vector<int> vec;

vector<int> detect_cycle(bool*& on_stack, int*& pre, bool*& visited, int n, int triggared) // detecting cycle.
{
	std::fill(visited, visited + n, false);
	std::fill(on_stack, on_stack + n, false);
	vector <int> empty_vector;
	empty_vector.clear();
	for (int i = 0; i < n; ++i) // algorithm.
		if (!visited[i])
		{
			for (int j = pre[triggared]; j != -1; j = pre[j])
				if (!visited[j])
				{
					visited[j] = true;
					vec.push_back(j);
					on_stack[j] = true;
				}
				else
				{
					if (on_stack[j])
					{
						bool flag_for_exit = false;
						vector<int> negative_cycle_vector;
						int previous = j;
						while (!flag_for_exit) {
							negative_cycle_vector.push_back(previous);
							previous = pre[previous];
							for (int tt = 0; tt  < negative_cycle_vector.size(); tt++) {
								if (negative_cycle_vector[tt] == previous)
								{
									flag_for_exit = true;
								}
							}
						}
						negative_cycle_vector.push_back(j);
						return negative_cycle_vector; // positive termination.
					}
					break;
				}
			for (int j : vec)
				on_stack[j] = false;
			vec.clear();
		}
	return empty_vector; // negative termination.
}
vector<int> spfa_early_terminate(long long* dis, int* pre, bool* in_queue, int n, vector <pair<int, int>>*& adj, bool* on_stack, bool* visited) // function for early termination.
{
	vector <int> empty_vectorr; // empty vector for early terminatation.
	vector <int> not_empty_vector;
	not_empty_vector.push_back(4);
	empty_vectorr.clear();
	std::fill(dis, dis + n, 0);
	std::fill(pre, pre + n, -1);
	std::fill(in_queue, in_queue + n, true);
	std::queue<int> queue;
	for (int i = 0; i < n; ++i)
		queue.push(i);
	int iter = 0;
	int trigger = -1;
	while (!queue.empty()) // algorithm.
	{
		int u = queue.front();
		queue.pop();
		in_queue[u] = false;
		int v;
		int w;
		for (int q = 0; q < adj[u].size(); q++)
		{
			v = adj[u][q].first;
			w = adj[u][q].second;
			if (dis[u] + w < dis[v])
			{
				pre[v] = u;
				dis[v] = dis[u] + w;
				iter++;
				trigger = v;
				if (iter == n)
				{	
					iter = 0;
					vector <int> neg_cycle = detect_cycle(on_stack, pre, visited, n, trigger);	
					if (!neg_cycle.empty()) // finding negative cycle and reversing them.
					{
						for (int bb = 0; bb < neg_cycle.size() - 1; bb++)
						{
							int weight_of_edge=0;
							vector<pair<int,int>> original_vector;
							for (int cc = 0; cc < adj[neg_cycle[neg_cycle.size() - 1 - bb]].size(); cc++) {
								if (adj[neg_cycle[neg_cycle.size() - 1 - bb]][cc].first == neg_cycle[neg_cycle.size() - 2 - bb])
								{
									weight_of_edge = adj[neg_cycle[neg_cycle.size() - 1 - bb]][cc].second;
								}
								else{
									original_vector.push_back(adj[neg_cycle[neg_cycle.size() - 1 - bb]][cc]);
								}
							}
							adj[neg_cycle[neg_cycle.size() - 1 - bb]] = original_vector;
							adj[neg_cycle[neg_cycle.size() - 2 - bb]].push_back(make_pair( neg_cycle[neg_cycle.size() - 1 - bb],-weight_of_edge) );		
						}
						return not_empty_vector;
					}
				}
				if (!in_queue[v])
				{
					queue.push(v);
					in_queue[v] = true;
				}
			}
		}
	}
	vector <int> neg_cyclee = detect_cycle(on_stack, pre, visited, n, trigger);
	if (!neg_cyclee.empty())
	{
		for (int bb = 0; bb < neg_cyclee.size() - 1; bb++)  // algorithm.
		{
			int weight_of_edge=0;
			vector<pair<int, int>> original_vector;
			for (int cc = 0; cc < adj[neg_cyclee[neg_cyclee.size() - 1 - bb]].size(); cc++) {
				if (adj[neg_cyclee[neg_cyclee.size() - 1 - bb]][cc].first == neg_cyclee[neg_cyclee.size() - 2 - bb]) {
					weight_of_edge = adj[neg_cyclee[neg_cyclee.size() - 1 - bb]][cc].second;
				}
				else {
					original_vector.push_back(adj[neg_cyclee[neg_cyclee.size() - 1 - bb]][cc]);
				}
			}
			adj[neg_cyclee[neg_cyclee.size() - 1 - bb]] = original_vector;
		    adj[neg_cyclee[neg_cyclee.size() - 2 - bb]].push_back(make_pair(neg_cyclee[neg_cyclee.size() - 1 - bb], -weight_of_edge));
		}
		return not_empty_vector;
	}
	return empty_vectorr;	
}
int main(int argc, char const* argv[])
{
	ifstream iss; // For IO operation.
	iss.open(argv[1]);

	ofstream out; // For Output operation.
	out.open(argv[2]);

	int v, u, w;
	int iteration;
	iss >> iteration;
	int matrix_size;
	while (iteration > 0) // Reading input.
	{
		iss >> matrix_size;
		int v = 0;
		int n = matrix_size * 2;
		vector<pair<int, int>>* adj = new vector <pair<int, int>>[n];
		for (int i = 0; i < matrix_size; i++)
		{
			int u = 0;
			for (int k = 0; k < matrix_size; k++)
			{
				iss >> w;
				if (i != k)
				{
					adj[v].emplace_back(u + matrix_size, -w);
				}
				else
				{
					adj[u + matrix_size].emplace_back(v, w);
				}
				u++;
			}
			v++;
		}
		vector <int> a;
		a.push_back(1);
		while (!a.empty()) // Function.
		{
			a = spfa_early_terminate(dis, pre, in_queue, n, adj, on_stack, visited);
		}
		int total_volt = 0;
		for (int e = 0; e < n; e++) // Output operation.
		{
			for (int o = 0; o < adj[e].size(); o++)
			{
				if (adj[e][o].second > 0)
				{
					total_volt += adj[e][o].second;
				}
			}
		}
		out << total_volt << endl;
		iteration--;
	}
	return 0;
}
