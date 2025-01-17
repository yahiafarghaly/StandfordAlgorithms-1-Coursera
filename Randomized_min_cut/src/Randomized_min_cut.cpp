//============================================================================
// Name        : Randomized_min_cut.cpp
// Author      : YF
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
using namespace std;

typedef vector< pair<int,vector<int> >>	vertices_list; // each vertex points to edges incident on it
typedef vector< pair<int,int> > edges_list; //	each edge points to its endpoints

void read_graph_data(string file_path,
					vertices_list& n_list,
					edges_list& m_list);

int RKarger(vertices_list& n_list,
			edges_list& m_list,
			int& v1,int& v2);

void init_randomGenerator()
{
	  /* initialize random seed: */
	  srand (time(NULL));
}

int getRandomNumber(int start,int end)
{
	return rand() % (end+1) + start; // (end + 1) to get an actually range between end and start.
}

//#define DEBUG

int main() {

	vertices_list n_list,n_lst;
	edges_list m_list,m_lst;
	int v1,v2,min_cut = 100000; // final two nodes with min cuts
	int old_min_cut = min_cut;
	//read_graph_data("./src/input_graphs/sample_graph.txt",n_list,m_list);
	//read_graph_data("./src/input_graphs/6.txt",n_list,m_list);
	//read_graph_data("./src/input_graphs/self_loop.txt",n_list,m_list);
	read_graph_data("./src/input_graphs/quiz_input.txt",n_list,m_list);
	std::cout << "#n = " << n_list.size() << ", #m = " << m_list.size() << "\n";
	std::cout << "-----------------------------------\n";
	do
	{
		n_lst.clear();
		m_lst.clear();
		std::copy(n_list.begin(), n_list.end(), std::back_inserter(n_lst));
		std::copy(m_list.begin(), m_list.end(), std::back_inserter(m_lst));

		min_cut = RKarger(n_lst,m_lst,v1,v2);
		if(min_cut < old_min_cut)
		{
			cout << "Min Cut = " << min_cut << endl;
			old_min_cut = min_cut;
		}
	}while(min_cut >= 15);

	return 0;
}

void read_graph_data(string file_path,
					vertices_list & n_list,
					edges_list& m_list)
{
	std::fstream in_file(file_path, std::ios_base::in);

	string line;
	vector<int> vertices;
	while(getline(in_file,line))
	{
		if(line.size() > 0)
		{
			stringstream stream(line);
			int num;
			vertices.clear();
			while (1)
			{
				stream >> num;
				if (!stream) break;
				vertices.push_back(num);
			}

			n_list.push_back(make_pair(vertices[0],
					std::vector<int>(vertices.begin() + 1,vertices.end())));
		}
	}

	for(unsigned int i = 0; i < n_list.size(); ++i)
		for(unsigned int j = 0; j < n_list[i].second.size(); j++ )
			m_list.push_back(make_pair(n_list[i].first,n_list[i].second[j]));

	// remove redundancy edges
	for(unsigned int i = 0; i < m_list.size();++i)
	{
		for(unsigned int j = i + 1; j < m_list.size(); j++)
		{
			if(m_list[i].first == m_list[j].second && m_list[i].second == m_list[j].first)
				m_list.erase(m_list.begin() + j);

		}
	}

	in_file.close();
}


int RKarger(vertices_list& n_list,
			edges_list& m_list,
			int& v1,int& v2)
{

	if(n_list.size() == 1)
	{
		v1 = v2 = n_list[0].first;
		return 0;
	}
	if(m_list.size() == 1)
	{
		v1 = m_list[0].first;
		v2 = m_list[0].second;
		return 1;
	}

	init_randomGenerator();

	int edge_idx;
#ifdef DEBUG
	unsigned long iter_cnt = 0;
#endif
	while(n_list.size() > 2)
	{
		edge_idx = getRandomNumber(0,m_list.size() - 1);
		v1 = m_list[edge_idx].first;
		v2 = m_list[edge_idx].second;
#ifdef DEBUG
		std::cout << "Iter# " << ++iter_cnt << std::endl;
		std::cout << "> Select: (v1,v2) = " << "( " << v1 << ", " << v2 << " )" << std::endl;
#endif
		// remove the randomly selected edge
		m_list.erase(m_list.begin() + edge_idx);
		// remove one vertex of the merged vertex.
		for(int k = 0; k < n_list.size();++k)
		{
			if(n_list[k].first == v2)
			{
				n_list.erase(n_list.begin() + k);
				break;
			}
		}
#ifdef DEBUG
		printf("> Merging (%d,%d) ==> %d\n",v1,v2,v1);
		printf("----> (%d) Vertices:\n",n_list.size());
		for_each(n_list.begin(),n_list.end(),[](pair<int,vector<int>>& v){
			std::cout << v.first << " ";
		});
		printf("\n");
#endif
		// update all edges with the new connection.
		for_each(m_list.begin(), m_list.end(),
				[&]( pair<int,int>& e)
				{
					if(e.first == v2)
						e.first = v1;
					if(e.second == v2)
						e.second = v1;
				});

		// Remove self loops
		for(auto edge = m_list.begin(); edge != m_list.end() ; ++edge)
		{
			if(edge->first == edge->second)
			{
#ifdef DEBUG
				printf("----> Removing self-loop(%d,%d)\n",edge->first,edge->second);
#endif
				m_list.erase(edge);
				/*
				 * Go back two positions of the deleted position, since
				 * edge is incremented by one at the end of the loop.
				 * so, it will miss the second element that was after
				 * the deleted element.
				 * */
				edge = edge - 2;
			}
		}

#ifdef DEBUG
		printf("----> (%d) Edges Connections:\n",m_list.size());
		for_each(m_list.begin(), m_list.end(),
				[&]( pair<int,int>& e)
				{
					printf("(%d,%d)\n",e.first,e.second);
				});
		std::cout << "\n# # # # # # # # # # # # # # # # \n";
#endif
	}

	v1 = m_list[0].first;
	v2 = m_list[0].second;
	return m_list.size(); // last cut result.
}
