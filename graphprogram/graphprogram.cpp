//*********************************************************************
//**			copyright (c) 1994/1996/1999/2000					 **
//**																 **
//**		Permission to use, copy and distribute this software	 **
//**		for educational purposes without fee is hereby granted   **
//**		provided that the copyright notice and this permission   **
//**		notice appear on all copies.  Permission to modify the   **
//**		software is granted, but not the right to distribute the **
//**		modified code.  All modifications are to be distributed  **
//**		as changes to released versions by AFIT.				 **
//**																 **
//*********************************************************************
//*********************************************************************
//*
//*	Title: 		Graph Package
//*	Date:  		July 2000
//*	Version:		1.1
//*	Project:		Development of C++ graph package
//*
//*	Authors: 		Professor Gary B. Lamont, James R. Holten 
//*				(1985); Timothy S. Weber (1986); Andy Beard, 
//*				Mark Huson, and Mike Proicou (1990); Andrew E. 
//*				McNear (1992); Matthew N. Johnson(1999,2000)
//*
//*	Coordinator: 	Professor Gary Lamont
//*
//*	Operating system:		Unix
//*	Language:  			C++
//*
//*	Objective:		This is the main program that coordinates the
//*				actions of the graphing program.  It calls all
//*				functions.  
//*
//*	Parameters-	Inputs   File or terminal data describing a graph		
//*			outputs  Those outputs described in the comments that
//*				   precede the main driver routine.
//*
//*History: An original graph program was written in Pascal in 1985 in 
//*	   support of the AFIT courses in data structures and algorithms.
//*	   The Pascal code was converted to Ada in 1986 with new 
//*	   functions added.  This version represented the graph matrix 
//*	   using a 2-dimensional array (adjacency matrix).  In 1989, 
//*	   the capability was added to compute the maximal independent
//*	   subsets as outlined in the Christofides text.  The software to 
//*	   represent the graph using a linked-list, sparse matrix was 
//*	   developed in 1991.  The user interface was expanded and code 
//*	   was written to compute all spannning trees.  In 1992, several 
//*	   routines were changed to improve functionality.  The help 
//*	   screens were moved into separate file, making future editing 
//*	   much simpler.  The screen displays were improved to better 
//*	   clarify data inputs.  The storing of user-entered data into 
//*	   input files was changed to add descriptive meaning to each 
//*	   data value for ease of reconstruction of a pictorial graph 
//*	   from the output data file.  Many of the modules of the 
//*	   previous version were refined in 1994.  In 1999, the Ada 
//*	   version was rewritten and updated in C++. The function to find 
//*	   all spanning trees was changed to give the number of spanning 
//*	   trees to save space and unnecessary calculation.  
//*
//*********************************************************************
#include <string>
#include <iostream>
#include "Interactive.h"
#include "GraphAlgorithms.h"
#include "Globals.h"

using namespace std;

int nvertices;                //will be the actual number of vertices in the graph
double adj_cost_matrix[vertices][vertices];//cost and adjacency matrix (combined)
double M[vertices][vertices];//matrix for determining number of spanning trees
short clique_adj[vertices][vertices];    //will be adjacency matrix of complement graph
std::ofstream outputfile("graph.output");//file to which results are sent
std::ofstream MISfile("MISinfo.data");    //writes the information from each step of the MIS algorithm
std::ofstream MISets("MIS.data");        //writes just the MISs and cliques
bool user = true; //true if using c++ compiler false if using java gui


//*****************************************************************************
//*
//*	Title:		Main driver Program
//*	Date:		July 2000
//*	Version:	1.1 (in C++)
//*	Name:		main
//*	Function:	This is the main driver program for the graph package of 
//*			programs.  It executes the following functions:
//*			1.	Prints the opening title screen. (graph_menus_title)
//*			2.	Displays the help screen describing the program's 
//*				capabilities. (graph_menus_help)
//*			3.	Determines the method the user will use to enter the 
//*				graph data.  (graph_menus_method)
//*			4.	Determines the operation to perform on the graph data. 
//*				(graph_menus_functions)
//*			5.	Opens the input file.  (open_input_file) 
//*			6.	Computes the depth first search of the graph.
//*			7.	Computes the strong components of the graph. 
//*			8.	Computes the basis and contrabasis of the graph.
//*			9.	Computes the maximal independent sets and cliques of 
//*					the graph.
//*			10.	Computes the number of spanning trees of the graph.
//*			11.	Computes the minimum spanning tree of the graph using 
//*					Kruskal's algorithm.
//*			12.	Computes the minimum spanning tree of the graph using 
//*					Prim's algorithm.
//*			13. Computes the minimum path between two vertices using 
//*					Dijkstra's algorithm
//*
//*	Modules called: graph_menus_title, graph_menus_help, graph_menus_method, 
//*			    input_file, interactive_input, strong_component, dfs, 
//*			    MIS, clique, spanning trees, kruskal, prim, dijkstra
//*****************************************************************************
int main(int argc, char * argv[]) {
	graph_menus_title();//display the title
	char choice = '?', function = '?', directed = '?';
	short start = 0, start_vertex = 0, end_vertex = 0;
	char print_MIS = '?';
	string inputfile = "?";
	
	directed = file_input(false, argc > 1 ? argv[1] : "/home/josh/School/AFIT/2020-02/CSCE686/graphprogram/graphs/q1.txt");
	MIS(directed, false, 'N');
	clique(directed);
//	do {
//		do {
//			choice = graph_menus_method();    //obtain input method
//			if ('F' == choice) { // file input method
//				directed = file_input(user, inputfile);
//			}
//			if ('I' == choice) { // interactive input method
//				directed = interactive_input();
//			}
//			if ('H' == choice) { // help menu (if one exists)
//				graph_menus_help();
//			}
//			if ('X' == choice) { // exit
//				return choice;
//			}
//		} while ((choice != 'X') && (choice != 'F') && (choice != 'I'));
//		do {
//			function = graph_menus_functions();    //obtain function to be performed
//			if ('S' == function) { // strong component
//				strong_component(directed);
//			}
//			if ('D' == function) { // depth-first search
//				dfs(user, start);
//			}
//			if ('M' == function) { // MIS and clique
//				MIS(directed, user, print_MIS);
//				clique(directed);
//			}
//			if ('T' == function) { // number of spanning trees
//				spanning_trees(1);
//			}
//			if ('K' == function) { // kruskal's algorithm
//				kruskal(directed);
//			}
//			if ('P' == function) { // prim's algorithm
//				prim(directed);
//			}
//			if ('B' == function) { // both kruskal and prim
//				kruskal(directed);
//				prim(directed);
//			}
//			if ('A' == function) { // all of the above functions
//				strong_component(directed);
//				dfs(user, start);
//				MIS(directed, user, print_MIS);
//				clique(directed);
//				spanning_trees(1);
//				kruskal(directed);
//				prim(directed);
//			}
//			if ('J' == function) { // dijkstra's algorithm
//				dijkstra(user, start_vertex, end_vertex);
//			}
//		} while ((function != 'X') && (choice != 'X') && (function != 'N'));
//		outputfile << endl;
//	} while ((function != 'X') && (choice != 'X') || (function == 'N'));
	return 0;
}
