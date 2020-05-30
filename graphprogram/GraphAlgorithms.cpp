#include "Interactive.h"
#include <iostream>
#include <list>
#include "GraphAlgorithms.h"

#include "Globals.h"

//--------------------------------------begin strong component
//************************************************************************
//*
//*	Title: 			strongComponent
//*	Date:  			July 2000
//*	Version:			1.1
//*
//*	Project:			Development of C++ graph package
//*
//*	Author: 			Matthew N. Johnson
//*	Coordinator: 		Professor Gary Lamont
//*
//*	Operating system:		Unix
//*	Language:  			C++
//*
//*	Function description:	Finds the strong components of a graph along with
//*					the basis and contra basis of the graph.  For
//*					nondirected graphs, it returns all vertices for
//*					strong component, basis and contrabasis.
//*
//*	Algorithm:			Strong components are determined from the Q and R
//*					matrices.  If a vertex can reach another vertex
//*					and be reached from that vertex, the two vertices
//*					are in the same strong component.  This is
//*					determined by comparing a particular spot in the
//*					matrices, if Q[i][j]=1 and R[i][j]=1, xi and xj
//*					are in the same strong component.  Basis is found
//*					with the Reaching matrix.  Rows of R are compared
//*					to find the vertex that can reach the most other
//*				      vertices.  If a vertex can reach all the vertices,
//*					it is the basis.  Otherwise, other vertices are
//*					added until all the vertices can be reached.  The
//*					contrabasis is found in the same manner, except
//*					that the Q (reachability) matrix is used instead
//*					of the R matrix.
//*
//*	Order of analysis:	O(nvertices^3)
//*
//*	Globals:			adjacency matrix
//*
//*	Local Variables:		Reaching and reachability matrices
//*
//*	Calling Modules:		graph_program
//*
//************************************************************************
void basis_contra(int Q_R[vertices][vertices], int SC_mat[vertices][vertices], int type) { // type = 1 for basis, 2 for contrabasis
	short a, b, c, current_vtx = 0, number = 0, number_reached = 0, most_reached = 0, contra_base = 0, complement = 0, complementor = 0;
	std::__cxx11::list<int> contra_basis;//stores a list of the vertices currently in the (contra)basis (actual numbers)
	std::__cxx11::list<int>::iterator c_b;
	std::__cxx11::list<int> not_reached;//stores a list of vertices not reached(reachable) (indexes to vertices)
	std::__cxx11::list<int>::iterator nr;
	for (a = 0; a < nvertices; a++) {
		number_reached = 0;
		for (b = 0; b < nvertices; b++) {
			number_reached += Q_R[a][b];//will equal 1 if a can reach b, 0 if not
		}
		if (number_reached > most_reached) {
			most_reached = number_reached;
			contra_base = (a + 1);    //gives the row (i.e. vertex) reaching(reached from) the most
			//vertices, must be part of (contra)basis
		}
	}
	contra_basis.push_back(contra_base); //begins a list of the vertices in the (contra)basis
	for (a = 0; a < nvertices; a++) {
		if (Q_R[contra_base - 1][a] == 0) {
			not_reached.push_back(a);    //list of vertices not reached(reachable) by vertices
		}                                //in the (contra)basis currently
	}
	if (not_reached.size() > 0) { // i.e. one vertex/strong component is not the sole (contra)basis //if #1
		do {
			for (current_vtx = 0; current_vtx < nvertices; current_vtx++) { // for #1
				for (nr = not_reached.begin(); nr != not_reached.end(); nr++) { // for #2
					//find a vertex that does reach a not-reached(reachable) vertex
					if (Q_R[current_vtx][(*nr)] == 1) {
						number = 0;
						for (c_b = contra_basis.begin(); c_b != contra_basis.end(); c_b++) {
							if ((*c_b) != current_vtx + 1) { // current vertex not that particular element of the basis
								number++;
							}
						}
					}    //if
					//find the vertex that is not in the (contra)basis and that reaches(is reached by) the most
					//vertices not yet found by vertices in the current (contra)basis
					if (number == contra_basis.size()) { // current vertex is not in the (contra)basis
						b = 0;
						for (c = 0; c < nvertices; c++) {
							for (c_b = contra_basis.begin(); c_b != contra_basis.end(); c_b++) {
								if ((Q_R[current_vtx][c] == 1) && (Q_R[(*c_b) - 1][c] == 0)) { // current vertex reaches(reachable from) c and the members of the (contra)basis do(are) not
									b++;
								}
							}
						}
						if (b > complement) {
							complement = b;            //highest total from above
							complementor = current_vtx;    //add this to the (contra)basis
						}
					}
				}            //end for #2
			}                // end for #1
			//once the best new candidate is found, add it and proceed
			contra_basis.push_back(complementor + 1);    //store actual value
			for (a = 0; a < nvertices; a++) {
				if (Q_R[complementor][a] == 1) {
					not_reached.remove(a);//take out vertices that are now reached(reachable)
				}
			}
			complement = 0;//reset for another pass
		} while (not_reached.size() > 0); //untill all the vertices are reached(reachable)
		if (type == 1) { // means calculating the basis, so indicate this in print
			outputfile << "  basis: (choose one from each []):" << std::endl;
			std::cout << "basis: (choose one from each [])" << std::endl;
		} else { // i.e. calculating contrabasis
			outputfile << "  contrabasis: (choose one from each []):" << std::endl;
			std::cout << "contrabasis: (choose one from each [])" << std::endl;
		}
		for (c_b = contra_basis.begin(); c_b != contra_basis.end(); c_b++) {//output the (contra)basis
			std::cout << "[";
			outputfile << "[";
			for (a = 0; a < nvertices; a++) {
				if (SC_mat[(*c_b) - 1][a] != 0) {
					std::cout.width(3);
					std::cout << SC_mat[(*c_b) - 1][a];
					outputfile.width(3);
					outputfile << SC_mat[(*c_b) - 1][a];
				}
			}
			std::cout << "]";
			outputfile << "]";
		}
		std::cout << std::endl;
		outputfile << std::endl;
	}            //end if #1
	else { // one vertex/strong component is the (contra)basis
		
		if (type == 1) { // calculating the basis
			outputfile << "  basis: (choose one from within the []):" << std::endl << "[";
			std::cout << "basis: (choose one from within [])" << std::endl << "[";
		} else { // calculating the contrabasis
			outputfile << "  contrabasis: (choose one from within the []):" << std::endl << "[";
			std::cout << "contrabasis: (choose one from within the [])" << std::endl << "[";
		}
		for (a = 0; a < nvertices; a++) {
			if (SC_mat[(contra_base) - 1][a] != 0) {
				std::cout.width(3);
				std::cout << SC_mat[(contra_base) - 1][a];
				outputfile.width(3);
				outputfile << SC_mat[(contra_base) - 1][a];
			}
		}
		std::cout << "]" << std::endl;
		outputfile << "]" << std::endl;
	}
}

void strong_component(char directed) {
	int a, b, c, no_of_elements = 0, similarities = 0;    //iterators
	int R[vertices][vertices] = {0}, R2[vertices][vertices], Q[vertices][vertices], SC_mat[vertices][vertices] = {0};
	for (a = 0; a < nvertices; a++) {
		R[a][a] = 1;    //a vertex can reach itself obviously
	}
	for (a = 0; a < nvertices; a++) { // find correspondence up to degree 2
		for (b = 0; b < nvertices; b++) {
			if (adj_cost_matrix[a][b] != 0) { // vertex a and b are connected...
				R[a][b] = 1;
				for (c = 0; c < nvertices; c++) {
					if (adj_cost_matrix[b][c] != 0) { // b and c are connected...
						R[a][c] = 1;//therefore, a can reach c
					}
				}
			}
		}
	}
	do {                    //find up to correspondence^n
		for (a = 0; a < nvertices; a++) {
			for (b = 0; b < nvertices; b++) {
				R2[a][b] = R[a][b];
			}
		}
		for (a = 0; a < nvertices; a++) {
			for (b = 0; b < nvertices; b++) {
				if (R[a][b] == 1) { // a and b are connected
					for (c = 0; c < nvertices; c++) {
						if (R[b][c] == 1) { // b and c are connected
							R[a][c] = 1;    //so a can reach c
						}    //if
					}        //for
				}            //if
			}                //for
		}                    //for
		similarities = 0;
		//similarities counts number of changes from one iteration to the next,
		// if equal to nvertices^2, all correspondences are found
		for (a = 0; a < nvertices; a++) {
			for (b = 0; b < nvertices; b++) {
				if (R2[a][b] == R[a][b]) {
					similarities++;
				}
			}
		}
	} while (similarities < (nvertices * nvertices));
	//display R matrix and 	create reachability matrix(transpose of R matrix)
	outputfile << "R matrix" << std::endl;
	for (a = 0; a < nvertices; a++) {
		for (b = 0; b < nvertices; b++) {
			outputfile.width(3);
			outputfile << R[a][b];
			Q[b][a] = R[a][b];
		}
		outputfile << std::endl;
	}
	//display Q matrix
	outputfile << "Q matrix" << std::endl;
	for (a = 0; a < nvertices; a++) {
		for (b = 0; b < nvertices; b++) {
			outputfile.width(3);
			outputfile << Q[a][b];
		}
		outputfile << std::endl;
	}
	//now find Strong Component from Q and R matrices, to be output later
	for (a = 0; a < nvertices; a++) {
		for (b = 0; b < nvertices; b++) {
			if ((R[a][b] == 1) && (Q[a][b] == 1)) { // compare a row of R and Q//a can reach b, and a can reached from b, so in the same SC
				SC_mat[a][b] = b + 1;
			}
		}
	}
	basis_contra(R, SC_mat, 1);                //find the basis
	basis_contra(Q, SC_mat, 2);                //find the contrabasis
	//if a and b are in the same strong component, their rows in SC_mat will be the same
	//so set one to all zeros, so that the same strong component isn't printed more than once
	for (a = 0; a < nvertices; a++) {
		for (b = a + 1; b < nvertices; b++) {
			if ((SC_mat[b][a] == SC_mat[a][a]) && (SC_mat[a][a] != 0)) {//a and b are in the same strong component
				for (c = 0; c < nvertices; c++) {
					SC_mat[b][c] = 0;//discount correspondence of b, because
				}                    //they are already counted by a
			}
		}
	}
	std::cout << "strong components:" << std::endl;            //output the strong components
	outputfile << "strong components:" << std::endl;
	for (a = 0; a < nvertices; a++) {
		no_of_elements = 0;
		for (b = 0; b < nvertices; b++) {
			no_of_elements += SC_mat[a][b];
		}
		if (no_of_elements != 0) { // already eliminated double counting above, so it gives unique bases
			std::cout << "[";
			outputfile << "[";
			for (c = 0; c < nvertices; c++) {
				if (SC_mat[a][c] != 0) {
					outputfile.width(3);
					outputfile << SC_mat[a][c];
					std::cout.width(3);
					std::cout << SC_mat[a][c];
				}
			}
			std::cout << "]" << std::endl;
			outputfile << "]" << std::endl;
		}
	}
}

//---------------------begin dfs
//************************************************************************
//*
//*	Title: 			dfs
//*
//*	Date:  			July 2000
//*	Version:			1.1
//*
//*	Project:			Development of C++ graph package
//*
//*	Author: 			Matthew N. Johnson
//*	Coordinator: 		Professor Gary Lamont
//*
//*	Operating system:		Unix
//*	Language:  			C++
//*
//*	Function description:	performs a depth first search on the vertices of
//*				     the graph.  In the case of an undirected graph, it
//*					will usually return the vertices in only one or a
//*					few branches.  Directed graphs will return a new
//*					list on each line that starts with the lowest
//*					numbered vertex that has not yet been reached and
//*					begins a search from that vertex.
//*
//*	Order of analysis:	at most O(n^2)
//*
//*	Globals:			the adjacency matrix
//*
//*	Local Variables:		a list of discovered vertices
//*
//*	Calling Modules:		main program
//*
//************************************************************************
int find_new_tree(std::__cxx11::list<int> discovered) { // new tree altogether
	short possible_start = 1, new_tree_start = -1;
	std::__cxx11::list<int>::iterator d;
	discovered.push_front(nvertices + 1);//in case all the elements from 1 to n are present
	discovered.sort();//gives an ordered list of the discovered vertices
	//finds the lowest numbered vertex not yet in the tree
	do {
		for (d = discovered.begin(); d != discovered.end(); d++) {
			if ((possible_start != (*d)) && (new_tree_start == -1)) { // second half assures it is the first one
				new_tree_start = possible_start;
			}
			possible_start++;
		}
	} while (new_tree_start == -1);    //assures taking the lowest one
	std::cout << "new tree:" << std::endl << new_tree_start;
	outputfile << "new tree:" << std::endl << new_tree_start;
	return new_tree_start;
}

int find_new_start(std::__cxx11::list<int> discovered) { // new start in same tree
	short a, correspondences_discovered = 0, fully_used_vertices = 0, total_correspondences = 0, new_start = 0;
	std::__cxx11::list<int>::iterator d;
	std::__cxx11::list<int>::iterator dd;
	do {
		for (d = discovered.begin(); d != discovered.end(); d++) {
			correspondences_discovered = total_correspondences = 0;//reset values
			for (a = 0; a < nvertices; a++) {
				if (adj_cost_matrix[(*d) - 1][a] != 0) {
					total_correspondences++;    //increments for # of correspondence
					for (dd = discovered.begin(); dd != discovered.end(); dd++) {
						if ((a + 1) == (*dd)) {
							correspondences_discovered++;//increments if correspondence already
						}        //discovered
					}
				}
			}
			if (correspondences_discovered == total_correspondences) { // all correspondences have been discovered
				fully_used_vertices++;
			} else { // vertex has some correspondence not yet discovered
				new_start = (*d);
			}
		}
	} while ((new_start == 0) && (fully_used_vertices < discovered.size()));
	if (fully_used_vertices == discovered.size()) { // all correspondences used, find a new tree
		new_start = -1;
	}
	return new_start;
}

void dfs(bool user, short start) {
	short a, b, number_of_corr, tempstart, dfs_mat[vertices][vertices] = {0};
	char found = '?';
	std::__cxx11::list<int> discovered;
	std::__cxx11::list<int>::iterator d;
	double ave = 0.0, sum;
	for (a = 0; a < nvertices; a++) {
		for (b = 0; b < nvertices; b++) {
			if (adj_cost_matrix[a][b] != 0) {
				dfs_mat[a][b] = 1;    //create simple, 1,0 matrix that will be changed
			}
		}
	}
	if (user) {
		do {
			std::cout << "At which vertex would you like to start the depth-first search?" << std::endl;
			std::cin >> start;
			if ((start <= 0) || (start > nvertices)) {
				std::cout << "**Error: The start must be between 1 and " << nvertices << std::endl;
			}
		} while ((start <= 0) || (start > nvertices));
	}
	discovered.push_front(start);//indicates a vertex has been found in the search
	std::cout << "Depth first search starting with vertex " << start << ":" << std::endl << start;
	outputfile << "Depth first search starting with vertex " << start << ":" << std::endl << start;
	do {            //do #1
		do {    //do #2
			tempstart = start;        //save tempstart in case changed later
			for (a = 0; a < nvertices; a++) {
				if (adj_cost_matrix[start - 1][a] != 0) { // a is a correspondence to the start
					found = '?';
					for (d = discovered.begin(); d != discovered.end(); d++) {
						if ((a + 1) == (*d)) {
							found = 'y';//vertex a has been discovered
						}
					}
					if (found != 'y') { // hadn't yet been found
						dfs_mat[start - 1][a] = dfs_mat[a][start - 1] = -1;
						//-1 indicates a vertex is now found, and won't be considered again
						std::cout << " --> " << a + 1;
						outputfile << " --> " << a + 1;
						discovered.push_front(a + 1);//indicate the vertex is discovered
						start = a + 1;//start from the new vertex just found
					} else {
						dfs_mat[start - 1][a] = dfs_mat[a][start - 1] = -1;
						//already have that vertex, so don't consider again
					}
				}//if
				number_of_corr = sum = ave = 0;
				if (tempstart != start) { // there is a correspondence to the new start
					for (b = 0; b < nvertices; b++) {
						sum += dfs_mat[start - 1][b];
						//finds the sum of the values for the correspondences of the start
						if (dfs_mat[start - 1][b] != 0) {
							number_of_corr++;//finds the number of elements in the sum
						}
					}
					if (number_of_corr > 0) {
						ave = sum / number_of_corr;
					}
				}//if
			}    //for
		} while ((ave > -1) && (number_of_corr != 0));//ave=-1 when all correspondences of a vertex are found
		//number_of_corr==0 when there are no correspondences to the start (end while #1)
		if ((ave == -1) || (number_of_corr == 0) && (discovered.size() < nvertices)) { // either all correspondences are used(ave==-1) or there are none(number_of_corr==0)
			std::cout << std::endl;
			outputfile << std::endl;
			start = find_new_start(discovered);
			if (start != -1) { // start=-1 if all correspondences of all the discovered vertices have been used, if so, start a new tree
				std::cout << start;
				outputfile << start;
			} else { // i.e. start==-1, so start a new tree
				if (discovered.size() < nvertices) {
					start = find_new_tree(discovered);//find a new tree, send the list of vertices already discovered
					discovered.push_front(start);    //add the new start to the list of discovered vertices
				}
			}
		}
	} while (discovered.size() < nvertices);        //end while #2
	std::cout << std::endl;
	outputfile << std::endl;
}


//----------------------------begin calculation of number of spanning trees
//*****************************************************************************
//*	Title: 			spanning_trees
//*
//*	Date:  			July 2000
//*	Version:			1.1
//*
//*	Project:			Development of C++ graph package
//*
//*	Operating system:		Unix
//*	Language:  			C++
//*
//*	Function description:	This function determines the number of spanning
//*					trees in a given graph.  It does so by taking the
//*					determinant of the matrix M, which is determined
//*					from graph input.  M is defined as follows:  a
//*					diagonal entry, mii, is the degree of vertex xi, if not
//*					directed, and the in degree if directed, and an entry mij
//*					is minus the number of parallel links between xi and xj.
//*					It contains only n-1 rows/columns, where n is the number
//*					of vertices.
//*
//*	Algorithm:		Uses Gauss-Jordan elimination to find an upper
//*					triangular matrix and then multiplies the
//*					pivots to find the determinant.  It is assumed
//*					that no vertex has degree(in and out together) of
//*					zero.  This makes this abbreviated method viable
//*					for the matrix.
//*
//*	Order of analysis:
//*
//*	Calling Modules:		Main program, kruskal, prim
//*
//*****************************************************************************
double spanning_trees(int print_option)//print if option==1, don't if option==0
//option is 0 when called from kruskal() or prim()
{
	short a, b, h;    //all iterators
	double multiplier;    //factor for multiplying and eliminating columns
	double determinant = 1.0;//determinant of the matrix
	for (a = 0; a < nvertices - 1; a++) {
		for (h = 1; h < nvertices - 1 - a; h++) {
			multiplier = 1.0;
			if (M[a + h][a] != 0) { // an element below a pivot is not zero
				multiplier = ((-(M[a + h][a])) / (M[a][a]));
				for (b = a; b < nvertices - 1; b++) {
					M[a + h][b] += ((M[a][b]) * multiplier);//eliminate a ro
				}
			}
		}
	}
	for (a = 0; a < nvertices - 1; a++) {
		determinant *= M[a][a];
	}
	if (determinant < 1)
		determinant = 0;
	if (print_option == 1) { // when called from main, not kruskal or prim or SC
		std::cout << "number of spanning trees = " << determinant << std::endl;
		outputfile << "number of spanning trees = " << determinant << std::endl;
	}
	return determinant;
}

//--------------------------end calculation of number of spanning trees
//*******************************
//* Title:		remove_loops
//* Date:		May 2000
//* Version:	1.0
//*
//* Function description:   It rules out one side of a loop(that is two links between
//* two vertices) If weighted, get rid of heavier side, else arbitrarily from
//* the higher numbered one to the lower numbered one. This is to ease in the
//* calculation of the minimum spanning trees.
//*
//* Calling modules: kruskal(), prim()
//********************************
double remove_loops(char directed, double tempcost_matrix[vertices][vertices]) {
	short a, b;//iterators
	if (directed == 'Y') {
		for (a = 0; a < nvertices; a++) {
			for (b = nvertices - 1; b >= a; b--) { // only consider the lower triangle of the matrix
				if ((tempcost_matrix[a][b] != 0) && (tempcost_matrix[b][a] != 0)) {
					if (tempcost_matrix[a][b] > tempcost_matrix[b][a]) { // if unweighted, are equal
						tempcost_matrix[a][b] = 999999;
					} else {
						tempcost_matrix[b][a] = 999999;//defaults here if unweighted
					}
				}
			}
		}
	}
	return tempcost_matrix[vertices][vertices];
}

//--------------------------begin kruskal and prim algorithms
//*****************************************************************************
//*
//*	Title: 			Prim
//*	Date:  			July 2000
//*	Version:			1.1
//*
//*	Project:			Development of C++ graph package
//*
//*	Operating system:		Unix
//*	Language:  			C++
//*
//*	Function description:	Determines the minimum spanning tree using Prim's
//*					algorithm as found in Christofides, p. 139-139.
//*					A problem could arise in a situation where
//*					an undirected, weighted graph had an area where
//*					there were two parallel links between two
//*					vertices with different weights.  In this case,
//*					if the lower cost is entered first, it would be
//*					erased and the second, higher cost would
//*					overwrite it.  To avoid this, simply input the
//*					lower cost second.
//*
//*	Order of analysis:	O(nvertices^2)
//*
//*	Globals:			number of vertices (nvertices) the adjacency
//*					matrix (adj_matrix)
//*
//*	Local Variables:		a temporary adjacency matrix (tempadj_matrix)
//*
//*	Calling Modules:		graph program
//*
//************************************************************************
void prim(char directed) {
	double possible;
	possible = spanning_trees(0);//test to see if there are even spanning trees(undirected only)
	if ((possible == 0) && (directed != 'Y')) {
		std::cout << "Prim: The graph is disconnected, so there are no spanning trees" << std::endl;
		outputfile << "Prim: The graph is disconnected, so there are no spanning trees" << std::endl;
		return;
	}
	std::__cxx11::list<int> visited;//a list of the vertices that are visited
	std::__cxx11::list<int>::iterator v;
	short low_column = 0, low_row = 0;//will store the vertices to be used
	short a, b, f, g, h, nlinks = 0;    //iterators and counters
	double total_cost = 0, lowest_cost = 999998;    //total cost so far and current lowest cost under consideration
	double beta_matrix[vertices][vertices];    //will store the costs of links under consideration
	double tempcost_matrix[vertices][vertices];//stores costs and will be changed
	for (a = 0; a < nvertices; a++) {
		for (b = 0; b < nvertices; b++) {
			tempcost_matrix[a][b] = adj_cost_matrix[a][b]; //set a temporary matrix to be changed
			beta_matrix[a][b] = 1000000;    //equivalent to infinity in Prim's description
		}
	}
	tempcost_matrix[vertices][vertices] = remove_loops(directed, tempcost_matrix);
	if (directed == 'Y') {
		std::cout << "Prim - include arcs:" << std::endl;
		outputfile << "Prim - include arcs:" << std::endl;
	} else {
		std::cout << "Prim - include links:" << std::endl;
		outputfile << "Prim - include links:" << std::endl;
	}
	for (a = 0; a < nvertices; a++) {
		for (b = 0; b < nvertices; b++) {
			if ((adj_cost_matrix[a][b] != 0) && (adj_cost_matrix[a][b] < lowest_cost)) {//arbitrarily start with the cheapest link
				lowest_cost = adj_cost_matrix[a][b];
				low_row = a;    //a and b are the two ends of the cheapest link
				low_column = b;
			}
		}
	}
	total_cost += adj_cost_matrix[low_row][low_column];
	std::cout << "(" << low_row + 1 << " --> " << low_column + 1 << ")";
	outputfile << "(" << low_row + 1 << " --> " << low_column + 1 << ") ";
	visited.push_back(low_row);//add the two endpoints of the cheapest link to
	visited.push_back(low_column);//the list of visited vertices
	nlinks++;
	//find all the correspondences and costs of the start vertex (low_row)
	for (a = 0; a < nvertices; a++) {
		if (adj_cost_matrix[low_row][a] != 0) { // new possibilities for links
			beta_matrix[low_row][a] = adj_cost_matrix[low_row][a];
		}
		if (directed == 'Y') {
			if (adj_cost_matrix[a][low_row] != 0) {
				beta_matrix[a][low_row] = adj_cost_matrix[a][low_row];
			}
		}
	}
	//indicate that the link is used
	tempcost_matrix[low_row][low_column] = tempcost_matrix[low_column][low_row] = -1;
	beta_matrix[low_row][low_column] = beta_matrix[low_column][low_row] = 999999;
	do {
		lowest_cost = 999998;
		for (f = 0; f < nvertices; f++) {
			//find the correspondences of the two endpoints of a used link
			if (tempcost_matrix[low_column][f] > 0) { // greater than zero if a link exists and hasn't been used
				beta_matrix[low_column][f] = adj_cost_matrix[low_column][f];
			}
			if (tempcost_matrix[low_row][f] > 0) {
				beta_matrix[low_row][f] = adj_cost_matrix[low_row][f];
			}
			if (directed == 'Y') { // check correspondences to and from
				if (tempcost_matrix[f][low_column] > 0) {
					beta_matrix[f][low_column] = adj_cost_matrix[f][low_column];
				}
				if (tempcost_matrix[f][low_row] > 0) {
					beta_matrix[f][low_row] = adj_cost_matrix[f][low_row];
				}
			}
		}
		//find the next cheapest link off the subtree, add it to the tree
		for (g = 0; g < nvertices; g++) {
			for (h = 0; h < nvertices; h++) {
				if (beta_matrix[g][h] < lowest_cost) {
					lowest_cost = beta_matrix[g][h];
					low_row = g;        //the two ends of the cheapest link
					low_column = h;
				}
			}
		}
		nlinks++;    //add the link
		a = 0;
		b = 0;
		// check to see if the endpoints have been visited to avoid circuits
		for (v = visited.begin(); v != visited.end(); v++) {
			if (low_row != (*v)) {
				a++;
			}
			if (low_column != (*v)) {
				b++;
			}
		}
		if ((a == visited.size()) || (b == visited.size())) { // at most, one of the above can be true because Prim's algorithm grows from a single subtree
			if (a == visited.size()) { // a (the low_row) has not been visited
				visited.push_back(low_row);
			} else { // b (the low_column) has not been visited
				visited.push_back(low_column);
			}
			//add the link to the tree, add its cost to the total cost, indicate
			//that the link is used with -1
			total_cost += tempcost_matrix[low_row][low_column];
			std::cout << "(" << low_row + 1 << " --> " << low_column + 1 << ")";
			outputfile << "(" << low_row + 1 << " --> " << low_column + 1 << ") ";
			tempcost_matrix[low_row][low_column] = tempcost_matrix[low_column][low_row] = -1;
			beta_matrix[low_row][low_column] = beta_matrix[low_column][low_row] = 999999;
		} else { // i.e. both endpoints of the next cheapest link have already been visited
			nlinks--;//indicate that the link should not be added, but was considered
			tempcost_matrix[low_row][low_column] = tempcost_matrix[low_column][low_row] = -1;
			beta_matrix[low_row][low_column] = beta_matrix[low_column][low_row] = 999999;
		}
	} while (nlinks < nvertices - 1);
	std::cout << std::endl << "total cost is: " << total_cost << std::endl;
	outputfile << std::endl << "total cost is: " << total_cost << std::endl;
}

//************************************************************************
//*	Title: 			kruskal
//*	Date:  			July 2000
//*	Version:			1.1
//*
//*	Project:			Development of C++ graph package
//*
//*	Author: 			Matthew N. Johnson
//*	Coordinator: 		Professor Gary Lamont
//*
//*	Operating system:		Unix
//*	Language:  			C++
//*
//*	Function description:	Determines the minimum spanning tree using
//*					Kruskal's algorithm. It uses a system that
//*					assigns values to each vertex according to the
//*					root of the subtree it is a part of.  It uses
//*					this information to decide if a link can be
//*					added.  A link can be added, if either: 1.  One
//*					or both of the vertices are not yet included in
//*					any subtree, or 2. The two vertices are part of
//*					different subtrees.
//*
//*	Order of analysis:	O(nlinks*nvertices^2)
//*
//*	Globals:			number of vertices (nvertices), adjacency matrix
//* 					(adj_matrix)
//*
//*	Local Variables:		a temporary adjacency matrix (tempadj_matrix)
//*
//*	Calling Modules:		main program
//************************************************************************
void kruskal(char directed) {
	double possible;
	possible = spanning_trees(0);//check to see if the graph is connected(undirected only)
	if ((possible == 0) && (directed != 'Y')) {
		std::cout << "Kruskal: The graph is disconnected, so there are no spanning trees" << std::endl;
		outputfile << "Kruskal: The graph is disconnected, so there are no spanning trees" << std::endl;
		return;
	}
	double tempcost_matrix[vertices][vertices], total_cost = 0.0;//will store the cost values
	short a, b, c, d, c_visited = 0, d_visited = 0, old_root = 0, nlinks = 0;//mostly iterators
	std::__cxx11::list<int> visited;//a list of the vertices that have been used/considered
	std::__cxx11::list<int>::iterator vi;
	std::__cxx11::list<double> costList;//will be an ordered list of the costs, so that they can be used lowest to highest
	std::__cxx11::list<double>::iterator cl;
	int roots[vertices];//indicates the root of the subtree to which each vertex belongs
	for (a = 0; a < nvertices; a++) {
		for (b = 0; b < nvertices; b++) {
			tempcost_matrix[a][b] = adj_cost_matrix[a][b]; //initialize all costs
		}
		roots[a] = -1;//-1 indicates a root has not yet been assigned
	}
	tempcost_matrix[vertices][vertices] = remove_loops(directed, tempcost_matrix);
	if (directed != 'Y') {    //get lower triangular matrix so costs are only counted once (undirected only)
		for (a = 0; a < nvertices; a++) {
			for (b = 0; b <= a; b++) {
				tempcost_matrix[a][b] = 0;
			}
		}
	}
	for (a = 0; a < nvertices; a++) {
		for (b = 0; b < nvertices; b++) {
			if (tempcost_matrix[a][b] != 0) {
				costList.push_back(tempcost_matrix[a][b]);//make a list of all the costs
			}
		}
	}
	costList.sort();//put all the costs in order to be used in sequence
	if (directed == 'Y') {
		std::cout << "Kruskal - include arcs:" << std::endl;
		outputfile << "Kruskal - include arcs:" << std::endl;
	} else {
		std::cout << "Kruskal - include links:" << std::endl;
		outputfile << "Kruskal - include links:" << std::endl;
	}
	do {
		for (cl = costList.begin(); cl != costList.end(); cl++) { // for #1
			for (c = 0; c < nvertices; c++) { // for #2
				for (d = 0; d < nvertices; d++) { // for #3
					if ((tempcost_matrix[c][d] == (*cl)) && (nlinks < nvertices - 1))//if #1
//the connection from c to d is the cheapest left and there aren't too many links
					{
						nlinks++;//increment the number of links
						if (nlinks == 1) { // if the first link, it must be counted
							roots[c] = roots[d] = c;// call c the root of this tree
							visited.push_back(c);//indicate these vertices have now been visited
							visited.push_back(d);
							total_cost += tempcost_matrix[c][d];//add the cost of this link to the total
							std::cout << "(" << c + 1 << " --> " << d + 1 << ")";
							outputfile << "(" << c + 1 << " --> " << d + 1 << ") ";
							tempcost_matrix[c][d] = tempcost_matrix[c][d] = -1;//indicate that a link has been used
						} else { // there is more than one link			//else #1
							c_visited = 0;
							d_visited = 0;
							for (vi = visited.begin(); vi != visited.end(); vi++) {
								if (c != (*vi)) {
									c_visited++;//increment if c isn't the value on the visited list
								}
								if (d != (*vi)) {
									d_visited++;//increment if d isn't the value on the visited list
								}
							}
							if ((c_visited == visited.size()) || (d_visited == visited.size())) { // if #2//either c or d (or both) has (have) not been visited
								if ((c_visited == visited.size()) && (d_visited == visited.size())) {//starting a new subtree(neither c nor d have been visited)
									roots[c] = roots[d] = c;//make c the root of this new tree
									visited.push_back(c);    //add the vertices to the list of visited
									visited.push_back(d);
								} else if (c_visited == visited.size()) {        //c has not been visited, but d has
									roots[c] = roots[d];//add d-c link to the subtree of d
									visited.push_back(c);
								} else if (d_visited == visited.size()) {        //d has not been visited, but c has
									roots[d] = roots[c];//add c-d link to the subtree of c
									visited.push_back(d);
								}
								total_cost += tempcost_matrix[c][d];//add the cost
								std::cout << "(" << c + 1 << " --> " << d + 1 << ")";
								outputfile << "(" << c + 1 << " --> " << d + 1 << ") ";
								tempcost_matrix[c][d] = tempcost_matrix[c][d] = -1;//indicate the link is used
							}                                        //end if #2
							else if (roots[c] != roots[d]) //i.e. c and d have both been visited
								//two possibilities:1.joining two subtrees(allowable). . .
							{
								old_root = roots[d];
								for (b = 0; b < nvertices; b++) {
									if (roots[b] == old_root) { // i.e. d is considered the root of the                            //subtree to which b belongs
										roots[b] = roots[c];//make c the root of the combined subtrees
									}
								}
								total_cost += tempcost_matrix[c][d];//add the cost of the link
								tempcost_matrix[c][d] = tempcost_matrix[d][c] = -1;//indicate that the link is used
								std::cout << "(" << c + 1 << " --> " << d + 1 << ")";
								outputfile << "(" << c + 1 << " --> " << d + 1 << ") ";
							} else { // 2. Are in the same tree, i.e. creates a circuit (not allowed)
								tempcost_matrix[c][d] = tempcost_matrix[d][c] = -1; //indicate the link is used
								nlinks--;        //remove link
							}        //end else
						}    //end else #1
					}                    //end if #1
				}                        //end for #3
			}                            //end for #2
		}                                //end for #1
		
	} while (nlinks < nvertices - 1);
	std::cout << std::endl << "total cost is: " << total_cost << std::endl;
	outputfile << std::endl << "total cost is: " << total_cost << std::endl;
}

//--------------------------end kruskal and prim
//--------------------------begin dijkstra
//************************************************************************
//*	Title: 				dijkstra
//*	Date:  				June 2000
//*	Version:			1.0
//*
//*	Project:			Development of C++ graph package
//*
//*	Author: 			Matthew N. Johnson
//*	Coordinator: 		Professor Gary Lamont
//*
//*	Operating system:	Unix
//*	Language:  			C++
//*
//*	Function description:	Determines the minimum path between two vertices
//*							using Dijkstra's algorithm. It labels vertices
//*							according to the minimum of their current cost (initially
//*							set to infinity for all except the start) and the
//*							cost to add their link to the current tree.  Once the pre-
//*							determined end vertex is labelled, its cost is the minimum
//*							path from the start vertex to the end vertex.
//*
//*	Order of analysis:	O(nlinks*nvertices)
//*
//*	Globals:			number of vertices (nvertices), adjacency/cost matrix
//* 					(adj_cost_matrix)
//*
//*	Local Variables:	a temporarily labelled list (temporary)
//*
//*	Calling Modules:	main program
//************************************************************************
//steps two and three in Dijkstra's algorithm
int dijkstra2_3(std::__cxx11::list<int> temporary, int p_vertex, double labels[vertices], double newlabels[vertices]) {
	short a, x_star = -1;    //iterator and x_star value (to be returned)
	double min_label = 10000000;        //keeps track of minimum label
	std::__cxx11::list<int>::iterator temp;
	for (temp = temporary.begin(); temp != temporary.end(); temp++) { // go through all temporarily labeled vertices
		if (adj_cost_matrix[p_vertex][(*temp)] != 0) { // a temporarily labeled vertex is an element of                                            //the correspondence of the p_vertex
			if (labels[(*temp)] > (labels[p_vertex] + adj_cost_matrix[p_vertex][(*temp)])) {
				labels[(*temp)] = labels[p_vertex] + adj_cost_matrix[p_vertex][(*temp)];
			}//update its label, if necessary
		}
		if (labels[(*temp)] < min_label) {
			min_label = labels[(*temp)];    //keep track of the cheapest label
			x_star = (*temp);                    //and the vertex with that label
		}
	}
	for (a = 0; a < nvertices; a++) {
		newlabels[a] = labels[a];        //the returned values
	}
	return x_star;
}

void dijkstra(bool user, short start_vertex, short end_vertex) { // steps one, four and five in Dijkstra's algorithm
	short a, x_star, p_vertex;
	std::__cxx11::list<int> temporary;
	double labels[vertices];
	if (user) {
		do {
			std::cout << "What is the start vertex for Dijkstra's algorithm?" << std::endl;
			std::cin >> start_vertex;
			if (start_vertex < 1 || start_vertex > nvertices) {
				std::cout << "**Error: start vertex must be between 1 and " << nvertices << ".  Please try again." << std::endl;
			}
		} while (start_vertex < 1 || start_vertex > nvertices);
		do {
			std::cout << "What is the end vertex?" << std::endl;
			std::cin >> end_vertex;
			if (end_vertex < 1 || end_vertex > nvertices) {
				std::cout << "**Error: end vertex must be between 1 and " << nvertices << ".  Please try again." << std::endl;
			}
		} while (end_vertex < 1 || end_vertex > nvertices);
	}
	if (start_vertex == end_vertex) {
		std::cout << "Dijkstra - Minimum cost from vertex " << start_vertex << " to vertex " << end_vertex << " is 0" << std::endl;
		outputfile << "Dijkstra - Minimum cost from vertex " << start_vertex << " to vertex " << end_vertex << " is 0" << std::endl;
		return;
	}
	start_vertex--;
	end_vertex--;//to avoid having to subtract one for all referencing
	for (a = 0; a < nvertices; a++) {
		labels[a] = 10000000;    //equivalent to infinity in algorithm
		temporary.push_back(a);
	}
	labels[start_vertex] = 0;        //start vertex's label is 0
	temporary.remove(start_vertex);    //equivalent to setting the label to permanent
	p_vertex = start_vertex;
	do {
		x_star = dijkstra2_3(temporary, p_vertex, labels, labels);
		if (x_star == -1) { // no correspondences on the list
			std::cout << "A path does not exist between the two vertices" << std::endl;
			outputfile << "A path does not exist between the two vertices" << std::endl;
			return;
		}
		temporary.remove(x_star);    //set x_star to permanent
		p_vertex = x_star;            //make x_star the new p
	} while (x_star != end_vertex);
	std::cout << "Dijkstra - Minimum cost from vertex " << start_vertex + 1 << " to vertex " << end_vertex + 1 << " is " << labels[end_vertex] << std::endl;
	outputfile << "Dijkstra - Minimum cost from vertex " << start_vertex + 1 << " to vertex " << end_vertex + 1 << " is " << labels[end_vertex] << std::endl;
}
