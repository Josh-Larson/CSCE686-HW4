#pragma once

#include <fstream>

constexpr int vertices = 100;    //change this value to create larger graphs
extern int nvertices;                //will be the actual number of vertices in the graph
extern double adj_cost_matrix[vertices][vertices];//cost and adjacency matrix (combined)
extern double M[vertices][vertices];//matrix for determining number of spanning trees
extern short clique_adj[vertices][vertices];    //will be adjacency matrix of complement graph
extern std::ofstream outputfile;//file to which results are sent
extern std::ofstream MISfile;    //writes the information from each step of the MIS algorithm
extern std::ofstream MISets;        //writes just the MISs and cliques
extern bool user; //true if using c++ compiler false if using java gui
