#include "MIS.h"

#include "Globals.h"

#include <optional>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iostream>
#include <chrono>


#define PIVOTING 1
#define ORDERING 1


bool range_all_of(size_t count, const std::function<bool(size_t)>& predicate) {
	for (size_t i = 0; i < count; i++) {
		if (!predicate(i))
			return false;
	}
	return true;
}

template<typename InputIterator, typename T>
bool all_of(InputIterator first, InputIterator last, const T & val) {
	while (first != last) {
		if (*first != val)
			return false;
		++first;
	}
	return true;
}

template<typename InputIterator, typename T>
size_t count_if(InputIterator first, InputIterator last, const std::function<bool(size_t index, const T &)> & predicate) {
	size_t count = 0;
	size_t index = 0;
	while (first != last) {
		if (predicate(index, *first)) {
			count++;
		}
		++first;
		index++;
	}
	return count;
}

template<typename InputIterator, typename T>
std::optional<T> min_if(InputIterator first, InputIterator last, const int degeneracyOrdering[vertices], const std::function<bool(size_t index, const T &)> & predicate) {
	std::optional<T> minimum_element;
	size_t index = 0;
	auto it = first;
	while (it != last) {
		if (predicate(index, first[degeneracyOrdering[index]]) && (!minimum_element ||  *minimum_element > first[degeneracyOrdering[index]])) {
			minimum_element = first[degeneracyOrdering[index]];
		}
		++it;
		index++;
	}
	return minimum_element;
}


template<typename InputIterator, typename T, typename U>
std::optional<T> min_if(InputIterator first, InputIterator last, const int degeneracyOrdering[vertices], const std::function<bool(size_t index, const T &)> & predicate, const std::function<U(size_t index, const T &)> & translation) {
	std::optional<U> minimum_element, mimimum_value;
	size_t index = 0;
	auto it = first;
	while (it != last) {
		const auto element = first[degeneracyOrdering[index]];
		if (predicate(index, element)) {
			auto val = translation(index, element);
			if (!mimimum_value || *mimimum_value > val) {
				minimum_element = element;
				mimimum_value = val;
			}
		}
		++it;
		index++;
	}
	return minimum_element;
}


//---------------------begin MIS/clique
//************************************************************************
//*
//*	Title: 			MIS
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
//*	Function description:	This function uses the algorithm as described in
//*					Christofides, page 35, to find the maximal
//*					independent sets.  In addition to the output that
//*					is displayed on the screen, the state of the
//*					information at each successive step and iteration
//*					is written to the file 'MISinfo.data'. However,
//*					note that this file can be rather large. For
//*				     example, for a 13-vertex, directed graph, the file
//*					was 23 pages in length when sent to Microsoft
//*					Word.  This size grows almost exponentially, as a
//*					20-vertex, directed graph created a 400+ page
//*					file.  If you wish to view only the maximal
//*					independent sets, they are printed to the file
//*				     called 'MIS.data'.  This may be more convenient if
//*					your only concern is the final answer.
//*
//*	Algorithm:			As described in Christofides, p. 35.
//*
//*	Order of analysis:	O((3^(n/3))*nvertices^2)
//*
//*	Globals:			adjacency matrix, number of vertices
//*
//*	Local Variables:		The sets of Q+, Q-, S, and X
//*
//*	Calling Modules:		Main program
//*
//************************************************************************
//this function is common to both MIS and clique calculation
char equation_3_8(short k, char print_MIS, char MIS_or_clique, int Qminus[vertices][vertices], int Qplus[vertices][vertices]) {
	bool eq38 = false;
	const auto qminus_empty = all_of(Qminus[k], Qminus[k+nvertices], 0);
	if (!qminus_empty) { // Q- is not empty, check eq38
		const auto qplus_empty = all_of(Qplus[k], Qplus[k+nvertices], 0);
		if (qplus_empty) {
			eq38 = true; // 3.8 satisfied if Q+ is empty
		} else { // Q+ is not empty	else #2
			eq38 = true; // Assuming it's true, find a counterexample
			for (auto a = 0; a < nvertices; a++) {
				if (Qminus[k][a] != 0)
					continue; // i.e. l+1 element of Q-
				if (MIS_or_clique == 'c')
					eq38 = eq38 && range_all_of(nvertices, [&](auto b) { return (clique_adj[a][b] == 0) || (Qplus[k][b] == 0); });
				else
					eq38 = eq38 && range_all_of(nvertices, [&](auto b) { return (adj_cost_matrix[a][b] == 0) || (Qplus[k][b] == 0); });
			}        //end for #1
		}            //end else #2
	}                //end else #1
	if (print_MIS == 'Y') {
		MISfile << "step 3:";
		if (eq38) {
			MISfile << "  equation 3.8 satisfied, go to step 5" << std::endl;
		} else {
			MISfile << "  equation 3.8 not satisfied, go to step 4" << std::endl;
		}
	}
	return eq38 ? 'S' : 'U';
}



//this function is used by MIS and clique calculation
short step2(short k, char eq38, char& neweq38, char print_MIS, char directed, char MIS_or_clique, int degeneracyOrdering[vertices], int Qplus[vertices][vertices], int Qminus[vertices][vertices], int S[vertices][vertices], int X[vertices], int newQplus[vertices][vertices], int newQminus[vertices][vertices], int newS[vertices][vertices], int newX[vertices]) {
	short a;
#if PIVOTING
	// Pivoting
	auto xk = min_if<int*, int, int>(Qminus[k], Qminus[k]+nvertices, degeneracyOrdering, [](const auto index, const auto & val) { return val != 0; }, [&](const auto index, const auto & val) {
		if (MIS_or_clique == 'c')
			return count_if<short*, short>(clique_adj[val], clique_adj[val]+nvertices, [&](const auto index, const auto & v) { return v != 0 && Qplus[k][index] != 0; });
		return count_if<double*, double>(adj_cost_matrix[val], adj_cost_matrix[val]+nvertices, [&](const auto index, const auto & v) { return v != 0 && Qplus[k][index] != 0; });
	}).value_or(nvertices);
	if (xk != nvertices) {
		if (MIS_or_clique == 'c')
			xk = min_if<int*, int>(Qplus[k], Qplus[k] + nvertices, degeneracyOrdering, [&](const auto index, const auto & val) { return val != 0 && clique_adj[xk][index] != 0; }).value_or(nvertices);
		else
			xk = min_if<int*, int>(Qplus[k], Qplus[k] + nvertices, degeneracyOrdering, [&](const auto index, const auto & val) { return val != 0 && adj_cost_matrix[xk][index] != 0; }).value_or(nvertices);
	}
	if (xk == nvertices)
		xk = min_if<int*, int>(Qplus[k], Qplus[k] + nvertices, degeneracyOrdering, [](const auto index, const auto & val) { return val != 0; }).value_or(nvertices);
#else
	// Find the lowest numbered vertex in Q+
	auto xk = min_if<int*, int>(Qplus[k], Qplus[k] + nvertices, degeneracyOrdering, [](const auto index, const auto & val) { return val != 0; }).value_or(nvertices);
#endif
	
	std::copy(Qplus[k], Qplus[k]+nvertices, Qplus[k+1]);
	std::copy(Qminus[k], Qminus[k]+nvertices, Qminus[k+1]);
	std::copy(S[k], S[k]+nvertices, S[k+1]);
	X[k] = S[k + 1][xk - 1] = xk;//add xk to the set X of vertices and to S
	for (a = 0; a < nvertices; a++) {
		if (MIS_or_clique == 'c') {
			if (directed == 'Y') {
				if (clique_adj[a][(X[k]) - 1] == 1) {
					Qminus[k + 1][a] = Qplus[k + 1][a] = 0;    //remove correspondences of xk
				}
			}
			if (clique_adj[(X[k]) - 1][a] == 1) {
				Qminus[k + 1][a] = Qplus[k + 1][a] = 0;    //remove correspondences of xk
			}
		} else {
			if (directed == 'Y') {
				if (adj_cost_matrix[a][(X[k] - 1)] != 0) {
					Qminus[k + 1][a] = Qplus[k + 1][a] = 0;//remove correspondences of xk(to and from)
				}
			}
			if (adj_cost_matrix[(X[k]) - 1][a] != 0) {
				Qminus[k + 1][a] = Qplus[k + 1][a] = 0;        //remove correspondences
			}
		}
	}
	Qplus[k + 1][(X[k]) - 1] = 0; //remove xk from Q+(k+1)
	k++;                        //increment k
	if (print_MIS == 'Y') { // print the values of the sets, if requested
		MISfile << "step 2:" << std::endl << "  xi" << k << " = " << xk << std::endl << "  Q+" << k << ":";
		for (a = 0; a < nvertices; a++) {
			if (Qplus[k][a] != 0) {
				MISfile.width(3);
				MISfile << Qplus[k][a];        //the Q+ set
			}
		}
		MISfile << std::endl << "  Q-" << k << ":";
		for (a = 0; a < nvertices; a++) {
			if (Qminus[k][a] != 0) {
				MISfile.width(3);
				MISfile << Qminus[k][a];        //the Q- set
			}
		}
		MISfile << std::endl << "  S " << k << ":";
		for (a = 0; a < nvertices; a++) {
			if (S[k][a] != 0) {
				MISfile.width(3);
				MISfile << S[k][a];            //the S set
			}
		}
		MISfile << std::endl << "  k = " << k << std::endl;
	}
	eq38 = equation_3_8(k, 'N', MIS_or_clique, Qminus, Qplus);
	neweq38 = eq38;
	return k;
}

//MIS and clique
short step5(short k, char eq38, char& neweq38, char print_MIS, char done, char& newdone, char MIS_or_clique, int degeneracyOrdering[vertices], int Qplus[vertices][vertices], int Qminus[vertices][vertices], int S[vertices][vertices], int X[vertices], int newQplus[vertices][vertices], int newQminus[vertices][vertices], int newS[vertices][vertices], int newX[vertices]) {
	short a;
	k--;                        //decrement k
	if (print_MIS == 'Y') {
		MISfile << "step 5:" << std::endl << "  k = " << k << std::endl;
	}
	if (k >= 0) {
		Qplus[k][(X[k]) - 1] = 0;            //remove xk from Q+ and...
		Qminus[k][(X[k]) - 1] = X[k];    //add it to Q-
		for (a = 0; a < nvertices; a++) {
			S[k][a] = S[k + 1][a];                //set S(k+1) to S(k)
		}
		S[k][(X[k]) - 1] = 0;                //remove xk from S
		done = 'N';
		if (print_MIS == 'Y') {
			MISfile << "  Q+" << k << ":";
			for (a = 0; a < nvertices; a++) {
				if (Qplus[k][a] != 0) {
					MISfile.width(3);
					MISfile << Qplus[k][a];        //print Q+
				}
			}
			MISfile << std::endl << "  Q-" << k << ":";
			for (a = 0; a < nvertices; a++) {
				if (Qminus[k][a] != 0) {
					MISfile.width(3);
					MISfile << Qminus[k][a];        //print Q-
				}
			}
			MISfile << std::endl << "  S " << k << ":";
			for (a = 0; a < nvertices; a++) {
				if (S[k][a] != 0) {
					MISfile.width(3);
					MISfile << S[k][a];            //print S
				}
			}
			MISfile << std::endl;
		}
		eq38 = equation_3_8(k, 'N', MIS_or_clique, Qminus, Qplus);
	} else { // k<0, so algorithm is done
		done = 'Y';
		if (print_MIS == 'Y') {
			MISfile << "k<=0 and Q+ is empty, stop" << std::endl;
		}
	}
	newdone = done;
	neweq38 = eq38;
	return k;
}

//MIS and clique
short step4(short k, char eq38, char& neweq38, char print_MIS, short max_ind, short& newmax_ind, char done, char& newdone, char directed, char MIS_or_clique, int degeneracyOrdering[vertices], int Qplus[vertices][vertices], int Qminus[vertices][vertices], int S[vertices][vertices], int X[vertices], int newQplus[vertices][vertices], int newQminus[vertices][vertices], int newS[vertices][vertices], int newX[vertices]) {
	short a, sSize = 0, Qplus_size = 0, Qminus_size = 0;
	for (a = 0; a < nvertices; a++) {
		Qplus_size += Qplus[k][a];    //count to see if the sets are empty
		Qminus_size += Qminus[k][a];
	}
	if (Qplus_size != 0) { // i.e. Q+ is not empty
		if (print_MIS == 'Y') {
			MISfile << "step 4:  Q+ is not empty, go to step 2" << std::endl;
		}
		k = step2(k, eq38, eq38, print_MIS, directed, MIS_or_clique, degeneracyOrdering, Qplus, Qminus, S, X, Qplus, Qminus, S, X);
	} else if (Qminus_size == 0) { // Q+ and Q- are empty
		if (print_MIS == 'Y') {
			MISfile << "step 4:  Q+ and Q- are empty" << std::endl << "  Maximal independent set:  [";
		}
		if (MIS_or_clique == 'm') {
			MISets << "Maximal independent set:  [";
			std::cout << "Maximal independent set:  [";
		} else {
			MISets << "clique:  [";
			std::cout << "clique:  [";
		}
		for (a = 0; a < nvertices; a++) {
			if (S[k][a] != 0) { // print the mis
				std::cout.width(3);
				std::cout << S[k][a];
				if (print_MIS == 'Y') {
					MISfile.width(3);
					MISfile << S[k][a];
				}
				MISets.width(3);
				MISets << S[k][a];
				sSize++;
			}
		}
		std::cout << "]" << std::endl;
		MISets << "]" << std::endl;
		if (print_MIS == 'Y') {
			MISfile << "]" << "  Go to step 5" << std::endl;
		}
		if (sSize > max_ind) {
			max_ind = sSize;
		}
		k = step5(k, eq38, eq38, print_MIS, done, done, MIS_or_clique, degeneracyOrdering, Qplus, Qminus, S, X, Qplus, Qminus, S, X);
	} else { // Q+ empty, but Q- not
		if (print_MIS == 'Y') {
			MISfile << "step 4:  Q+ empty, but Q- not empty, go to step 5" << std::endl;
		}
		k = step5(k, eq38, eq38, print_MIS, done, done, MIS_or_clique, degeneracyOrdering, Qplus, Qminus, S, X, Qplus, Qminus, S, X);
	}
	newdone = done;        //set the values that are returned
	newmax_ind = max_ind;
	neweq38 = eq38;
	return k;
}

void getDegeneracyOrdering(int degeneracyOrdering[vertices], char MIS_or_clique) {
	for (int i = 0; i < vertices; i++)
		degeneracyOrdering[i] = i;
#if ORDERING
	int startIndex = 1;
	auto getRemainingDegree = [&](int i) {
		int degree = 0;
		for (int j = startIndex; j < vertices; j++) {
			if (MIS_or_clique == 'c') {
				if (clique_adj[i][j] != 0)
					degree++;
			} else {
				if (adj_cost_matrix[i][j] != 0)
					degree++;
			}
		}
		return degree;
	};
	
	while (startIndex <= nvertices) {
		auto m = *std::min_element(degeneracyOrdering+startIndex-1, degeneracyOrdering+nvertices, [&](int left, int right) { return getRemainingDegree(left) < getRemainingDegree(right); });
		degeneracyOrdering[startIndex-1] = m;
		degeneracyOrdering[m] = startIndex-1;
		startIndex++;
	}
#endif
}

//MIS only
int MIS(char directed, bool user, char print_MIS) {
	int Qminus[vertices][vertices] = {0}, Qplus[vertices][vertices] = {0}, S[vertices][vertices] = {0}, X[vertices] = {0};//Q-, Q+, S, and X for MIS calculation
	int degeneracyOrdering[vertices] = {0};
	getDegeneracyOrdering(degeneracyOrdering, 'm');
	short a, k = 0, max_ind = 0;//set state of k and max_ind(the maximum size of an MIS)
	char eq38 = '?';        //says whether or not equation 3.8 is satisfied
	char done = 'N';
	if (user) {
		do {    //ask whether or not to print the information from each step
			std::cout << "Do you want to print the intermediate results of the MIS calculation? (Y/N)" << std::endl;
			std::cin >> print_MIS;
			print_MIS = toupper(print_MIS);
			if ((print_MIS != 'N') && (print_MIS != 'Y')) {
				std::cout << "Illegal input, please try again" << std::endl;
			}
		} while ((print_MIS != 'N') && (print_MIS != 'Y'));
	}
	const auto start = std::chrono::steady_clock::now();
	for (a = 0; a < nvertices; a++) {
		Qplus[0][a] = (a + 1);//set Q+0 to all vertices
	}
	if (print_MIS == 'Y') {
		MISfile << "step 1:" << std::endl << "  k = 0" << std::endl << "  Q+0:";
		for (a = 0; a < nvertices; a++) {
			MISfile.width(3);
			MISfile << Qplus[k][a];    //print Q+
		}
		MISfile << std::endl << "  Q-0:" << std::endl << "  S 0:" << std::endl;//Q- and S always empty initially
	}
	k = step2(k, eq38, eq38, print_MIS, directed, 'm', degeneracyOrdering, Qplus, Qminus, S, X, Qplus, Qminus, S, X);
	do {
		if (eq38 == 'S') { // equation 3.8 is satisfied, go to step 5
			k = step5(k, eq38, eq38, print_MIS, done, done, 'm', degeneracyOrdering, Qplus, Qminus, S, X, Qplus, Qminus, S, X);
		} else { // equation 3.8 is not satisfied, go to step 4
			k = step4(k, eq38, eq38, print_MIS, max_ind, max_ind, done, done, directed, 'm', degeneracyOrdering, Qplus, Qminus, S, X, Qplus, Qminus, S, X);
		}
	} while (done != 'Y');
	const auto end = std::chrono::steady_clock::now();
	const auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	fprintf(stdout, "module=mis,time=%ld\n", time);
	std::cout << "independence number = " << max_ind << std::endl;
	if (print_MIS == 'Y') {
		MISfile << "independence number = " << max_ind << std::endl;
	}
	MISets << "  independence number = " << max_ind << std::endl << "MIS calculation took " << time << "us." << std::endl;
	return 0;
}

//*****************************************************************************
//*
//*	Title: 			clique
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
//*	Function description:	finds the cliques of a graph by considering the
//*					complementary graph to the one entered in the
//*					data.
//*
//*	Algorithm:			Uses the same algorithm as MIS (Christofides, p.
//*					35) but uses the information of the complementary
//*					graph.
//*
//*	Order of analysis:	O((3^(n/3))*nvertices^2)
//*
//*	Globals:			adjacency matrix, number of vertices
//*
//*	Local Variables:		cliquenumber
//*
//*	Calling Modules:		main program
//*
//*****************************************************************************
//uses equation_3_8, step2, step4 and step5 from above
int clique(char directed) {
	int cQminus[vertices][vertices] = {0}, cQplus[vertices][vertices] = {0}, cS[vertices][vertices] = {0}, cX[vertices] = {0};//Q-, Q+, S and X for clique calculation
	int degeneracyOrdering[vertices] = {0};
	getDegeneracyOrdering(degeneracyOrdering, 'c');
	char eq38 = '?';        //says whether or not equation 3.8 is satisfied
	short a, b, ck = 0, cliquenumber = 0;
	char cdone = 'N';
	for (a = 0; a < nvertices; a++) { // make clique_adj the adjacency matrix for the complement graph
		for (b = 0; b < nvertices; b++) {
			if (adj_cost_matrix[a][b] != 0) {
				clique_adj[a][b] = 0;
			} else {
				clique_adj[a][b] = 1;
			}
		}
		clique_adj[a][a] = 1;        //a node is adjacent to itself
		cQplus[0][a] = (a + 1);        //Qplus equals all vertice
	}
	ck = step2(ck, eq38, eq38, 'N', directed, 'c', degeneracyOrdering, cQplus, cQminus, cS, cX, cQplus, cQminus, cS, cX);
	do {
		if (eq38 == 'S') {
			ck = step5(ck, eq38, eq38, 'N', cdone, cdone, 'c', degeneracyOrdering, cQplus, cQminus, cS, cX, cQplus, cQminus, cS, cX);
		} else {
			ck = step4(ck, eq38, eq38, 'N', cliquenumber, cliquenumber, cdone, cdone, directed, 'c', degeneracyOrdering, cQplus, cQminus, cS, cX, cQplus, cQminus, cS, cX);
		}
	} while (cdone != 'Y');
	std::cout << "clique number = " << cliquenumber << std::endl;
	MISets << "  clique number = " << cliquenumber << std::endl;
	return 0;
}

//----------------------------end MIS/clique