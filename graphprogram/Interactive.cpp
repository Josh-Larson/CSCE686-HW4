#include <iostream>
#include <string>
#include "Interactive.h"

#include "Globals.h"

//------------------------end graph menus
//------------------------begin file input
//*****************************************************************************
//*
//*	Title: 			file_input
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
//*	Function description:	This program takes file input from a source file
//*					and enters it in the program as the graph data.
//*					However, currently, there is no guard against a
//*				     filename that does not exist.  If such a file name
//*					is typed in, there will be no data for the graph.
//*				     Running Kruskal's algorithm seems to be the safest
//*					method to check that there is data, as other
//*					operations may encounter infinite loops, causing
//*					the program to hang.
//*
//*	Calling Modules:		Main program
//*
//****************************************************************************

//*******************************************************************
//*
//*	Title: 			graph_menus
//*	Date:  			July 2000
//*	Version:		1.1
//*
//*	Project:		Development of C++ graph package
//*
//*	Author: 		Matthew N. Johnson
//*	Coordinator: 		Professor Gary Lamont
//*
//*	Operating system:	Unix
//*	Language:  		C++
//*
//*	Function description:	This consists of several menus that appear
//*				throughout the program.  It includes the main
//*				title that is displayed initially, the menu that
//*				asks for type of input, the help menu (if any),
//*				and the menu displaying available functions.
//*
//*	Calling Modules:	main_program
//*
//********************************************************************
//----------------------------- begin Graph_menus
void graph_menus_title() {
	std::cout << std::endl << "***********************************************************************" << std::endl << std::endl;
	std::cout << "\t\tAir Force Institute of Technology" << std::endl;
	std::cout << "\t\t Graduate School of Engineering" << std::endl;
	std::cout << "\t  Department of Electrical and Computer Engineering" << std::endl << std::endl;
	std::cout << "***********************************************************************" << std::endl << std::endl;
	std::cout << "\t\t STANDARD GRAPH ROUTINES IN C++" << std::endl << std::endl;
	std::cout << "\t\tWelcome to the AFIT Graph Program" << std::endl << std::endl;
}

void graph_menus_help() {
	std::cout << std::endl << "***********************************************************************" << std::endl;
	std::cout << "\t\t\tSorry, no help currently" << std::endl;
	std::cout << "***********************************************************************" << std::endl << std::endl;
}

char graph_menus_method() {
	char method = '?';
	do {
		std::cout << "***********************************************************************" << std::endl;
		std::cout << "\tWhat type of graph input method do you desire?" << std::endl;
		std::cout << "\t\t\tYour options are:" << std::endl;
		std::cout << " F) for file Input, I) for interactive input, H) for help, X) to exit" << std::endl;
		std::cout << "***********************************************************************" << std::endl;
		std::cin >> method;
		method = toupper(method);
	} while ((method != 'F') && (method != 'I') && (method != 'H') && (method != 'X'));
	return method;
}

char graph_menus_functions() {
	char operation = '?';
	do {
		std::cout << "***********************************************************************" << std::endl << std::endl;
		std::cout << "\t\tWhat functions are desired?" << std::endl << std::endl;
		std::cout << "\tThe available options for directed/undirected graphs are:" << std::endl << std::endl;
		std::cout << "\t\tS for strong components" << std::endl;
		std::cout << "\t\tD for depth first search" << std::endl;
		std::cout << "\t\tM for maximum independent set/clique" << std::endl;
		std::cout << "\t\tT for the number of spanning trees" << std::endl;
		std::cout << "\t\tK for Kruskal's minimum spanning tree" << std::endl;
		std::cout << "\t\tP for Prim's minimum spanning tree" << std::endl;
		std::cout << "\t\tB for both Kruskal's and Prim's MST" << std::endl;
		std::cout << "\t\tA for all of the above" << std::endl;
		std::cout << "\t\tJ for Dijkstra's algorithm" << std::endl;
		std::cout << "\t\tN for entry of a new graph" << std::endl;
		std::cout << "\t\tX to terminate program" << std::endl << std::endl;
		std::cout << "All outputs are stored in graph.output except MISs which are stored in " << std::endl;
		std::cout << "MIS.data and the information about each step of the MIS calculation which," << std::endl;
		std::cout << "if desired, is stored in MISinfo.data" << std::endl;
		std::cout << "***********************************************************************" << std::endl << std::endl;
		std::cin >> operation;
		operation = toupper(operation);
	} while ((operation != 'S') && (operation != 'D') && (operation != 'M') && (operation != 'T') && (operation != 'K') && (operation != 'P') && (operation != 'B') && (operation != 'A') && (operation != 'J') && (operation != 'N') && (operation != 'X'));
	return operation;
}

char file_input(bool user, std::string inputfile) {
	//string inputfile;	//name of the file data is taken from
	char weighted = '?', directed = '?', file_opened = 'n';//test to see if file was found
	short a, b, current_vtx, out_deg, to_vtx, arc_index;//iterators
	double arc_weight;//allow for non-integer values of weights
	if (user) {
		do {        //make sure a valid file is found
			std::cout << "Please enter the name of graph input file:" << std::endl;
			std::cin >> inputfile;
			std::ifstream infile(inputfile.c_str());
			if (infile.fail()) {
				file_opened = 'n';
				std::cout << "**Error:  Failed to open " << inputfile << "!!" << std::endl << std::endl;
			} else {
				file_opened = 'y';
			}
		} while (file_opened != 'y');    //loop until a good file is found
	}
	std::ifstream infile(inputfile.c_str());    //re-initialize once proper name is taken
	infile >> directed;        // directed?
	//skip reading up to 50 characters and to the next line, to allow comments
	infile.ignore(50, '\n');
	infile >> weighted;        // weighted?
	infile.ignore(50, '\n');
	infile >> nvertices;    // number of vertices
	for (a = 0; a < nvertices; a++) {    //set cost/adjacency and M matrices to 0
		for (b = 0; b < nvertices; b++) {
			adj_cost_matrix[a][b] = M[a][b] = 0;
		}
	}
	for (current_vtx = 0; current_vtx < nvertices; current_vtx++) { // begin for #1,
		infile.ignore(50, '\n');
		infile >> out_deg; //out degree of current vertex
		if (directed != 'Y') {
			M[current_vtx][current_vtx] = out_deg;
		}
		for (arc_index = 1; arc_index <= out_deg; arc_index++) { // begin for #2
			infile.ignore(50, '\n');
			infile >> to_vtx;
			if (directed == 'Y') {
				adj_cost_matrix[current_vtx][to_vtx - 1] = 1;
				M[current_vtx][to_vtx - 1]--;    //decrement for a link between vertices
				M[to_vtx - 1][current_vtx]--;
				M[to_vtx - 1][to_vtx - 1]++;    //increment for indegree
			} else { // not directed
				adj_cost_matrix[current_vtx][to_vtx - 1] = adj_cost_matrix[to_vtx - 1][current_vtx] = 1;
				M[current_vtx][to_vtx - 1]--; //decrement for a link between vertices
			}
			if (weighted == 'Y') {
				infile.ignore(50, '\n');
				infile >> arc_weight;
				if (directed == 'Y') {
					adj_cost_matrix[current_vtx][to_vtx - 1] = arc_weight;
				} else {
					adj_cost_matrix[current_vtx][to_vtx - 1] = adj_cost_matrix[to_vtx - 1][current_vtx] = arc_weight;
				}
			}
		}        //end for #2
	}        //end for #1
	outputfile << "Data for " << inputfile << ":" << std::endl;
	print_adj_matrix();
	return directed;
}

//------------------------end file input
//------------------------begin interactive input
//*****************************************************************************
//*
//*	Title: 			interactive_input
//*	Date:  			July 2000
//*	Version:			1.1
//*
//*	Project:			Development of C++ graph package
//*
//*	Operating system:		Unix
//*	Language:  			C++
//*
//*	Function description:	Obtains graph information via keyboard input from
//*					the user.  This information is used throughout
//*					the rest of the program to run other functions.
//*
//*	Calling Modules:		graph_program
//*
//*****************************************************************************
char interactive_input() {
	std::string savefile;    //will be the name of the file to which data is saved
	char save = '?', correct_data = '?', weighted = '?', directed = '?'; //save data or not, test to see if input was correct
	bool save_graph_data;
	short a, b, current_vtx, out_deg, to_vtx, arc_index = 1;    //iterators
	double tempdata[2 * vertices + 1], arc_weight = 1;
//to vertex and weight to every vertex + overall out-degree=2*vertices+1
	int counter;    //will count the number of useful inputs in tempdata
	do {
		std::cout << "Do you want to save the graph input data? (Y/N)" << std::endl;
		std::cin >> save;
		save = toupper(save);
		if ((save != 'Y') && (save != 'N')) {
			std::cout << "Illegal input, please try again." << std::endl;
		}
	} while ((save != 'Y') && (save != 'N'));
	save_graph_data = (save == 'Y'); //qualifies bool
	if (save_graph_data) {
		std::cout << "What do you want to call this data file?" << std::endl;//ask user for name of file
		std::cin >> savefile;
		std::cout << "You are about to enter graph data in file:  " << savefile << std::endl;
	} else { // avoid trying to initialize a null string as a file name
		savefile = "void.data";
	}
	std::ofstream outFile(savefile.c_str());
	do {        //ask if directed
		std::cout << "Is this a directed graph? (Y/N)" << std::endl;
		std::cin >> directed;
		directed = toupper(directed);
		if ((directed != 'Y') && (directed != 'N')) {
			std::cout << "Illegal input, please try again." << std::endl;
		}
	} while ((directed != 'Y') && (directed != 'N'));
	if (save_graph_data) {
		outFile << directed << "\t//directed" << std::endl;    //write to file and comment
	}
	do {                //find out if the graph is weighted
		if (directed == 'Y') {
			std::cout << "Do the arcs have weights? (Y/N)" << std::endl;
		} else {
			std::cout << "Do the links have weights? (Y/N)" << std::endl;
		}
		std::cin >> weighted;
		weighted = toupper(weighted);
		if ((weighted != 'Y') && (weighted != 'N')) {
			std::cout << "Illegal input, please try again" << std::endl;
		}
	} while ((weighted != 'Y') && (weighted != 'N'));
	if (save_graph_data) {
		outFile << weighted << "\t//weighted" << std::endl;    //write to file and comment
	}
	do {
		std::cout << "How many vertices/nodes does the graph have:" << std::endl;
		std::cin >> nvertices;
		if ((nvertices <= 0) || (nvertices > vertices)) {
			std::cout << "**Error: You must have more than 0 and less than " << vertices + 1 << " vertices" << std::endl;
		}
	} while ((nvertices <= 0) || (nvertices > vertices));//check to see there is an appropriate number of nodes
	//doesn't check to make sure nvertices is an integer (will crash program)
	if (save_graph_data) {
		outFile << nvertices << "\t//number of vertices" << std::endl;
	}
	if (directed == 'Y') {
		std::cout << "Input the arcs coming out of the ";
	} else {
		std::cout << "Input the links coming out of the ";
	}
	std::cout << nvertices << " vertices by their destination number." << std::endl;
	for (a = 0; a < nvertices; a++) {//set cost/adjacency and M matrices to 0
		for (b = 0; b < nvertices; b++) {
			adj_cost_matrix[a][b] = M[a][b] = 0;
		}
	}
	for (current_vtx = 0; current_vtx < nvertices; current_vtx++) { // begin for #1
		do {                                            //do #1
			for (a = 0; a <= 2 * vertices; a++) {
				tempdata[a] = 0;    //clear tempdata
			}
			counter = 0;                //will be the number of entries to be saved
			if (directed == 'Y') {
				std::cout << "Input the outdegree of vertex " << (current_vtx + 1) << " (include loops):" << std::endl;
			} else {
				std::cout << "Input the degree of vertex " << (current_vtx + 1) << " (include loops):" << std::endl;
			}
			std::cin >> out_deg;
			tempdata[counter] = out_deg;
			counter++;                    //increment for out degree
			for (arc_index = 1; arc_index <= out_deg; arc_index++) { // begin for #2
				if (directed == 'Y') {
					std::cout << "arc " << arc_index << " out of vertex " << (current_vtx + 1) << " goes to vertex: (if a loop, put in same vertex)" << std::endl;
				} else {
					std::cout << "link " << arc_index << " out of vertex " << (current_vtx + 1) << " goes to vertex: (if a loop, put in same vertex)" << std::endl;
				}
				std::cin >> to_vtx;
				tempdata[counter] = to_vtx;
				counter++;        //increment for to vertex
				if (weighted == 'Y') {
					std::cout << "and has weight:" << std::endl;
					std::cin >> arc_weight;
					tempdata[counter] = arc_weight;
					counter++;            //increment for weight
				}
			}        //end for #2
			do {                        //do #2
				std::cout << "Was the data for vertex " << current_vtx + 1 << " correct? (Y/N)" << std::endl;
				std::cin >> correct_data;
				correct_data = toupper(correct_data);
				if ((correct_data != 'Y') && (correct_data != 'N')) {
					std::cout << "Illegal input, please try again." << std::endl;
				}
			} while ((correct_data != 'Y') && (correct_data != 'N'));    //end do #2
		} while (correct_data != 'Y');        //end do #1
//now input the data when it is confirmed to be correct
		if (directed != 'Y') {
			M[current_vtx][current_vtx] = tempdata[0];    //the out degree
		}
		if (weighted == 'Y') {
			for (a = 1; a < counter; a += 2) { // take every other value if weighted
				if (directed == 'Y') {
					adj_cost_matrix[current_vtx][int(tempdata[a]) - 1] = tempdata[a + 1];//a+1 is a weight
					M[current_vtx][int(tempdata[a]) - 1]--;
					M[int(tempdata[a]) - 1][current_vtx]--;
					M[int(tempdata[a]) - 1][int(tempdata[a]) - 1]++;
				} else { // not directed
					adj_cost_matrix[current_vtx][int(tempdata[a]) - 1] = adj_cost_matrix[int(tempdata[a]) - 1][current_vtx] = tempdata[a + 1];
					M[current_vtx][int(tempdata[a]) - 1]--;
				}
			}
		} else { // i.e. not weighted
			for (a = 1; a < counter; a++) {
				if (directed == 'Y') {
					adj_cost_matrix[current_vtx][int(tempdata[a]) - 1] = 1;
					M[current_vtx][int(tempdata[a]) - 1]--;
					M[int(tempdata[a]) - 1][current_vtx]--;
					M[int(tempdata[a]) - 1][int(tempdata[a]) - 1]++;
				} else {
					adj_cost_matrix[current_vtx][int(tempdata[a]) - 1] = adj_cost_matrix[int(tempdata[a]) - 1][current_vtx] = 1;
					M[current_vtx][int(tempdata[a]) - 1]--;
				}
			}
		}
		if (save_graph_data) { // saves and comments the graph data
			outFile << tempdata[0] << "\t//(out) degree of vertex " << current_vtx + 1 << std::endl;
			for (a = 1; a < counter; a++) {
				if (weighted == 'Y') {
					if (a % 2 == 0) { // every other input is a weight, starting with tempdata[2]
						outFile << tempdata[a] << "\t//  arc/link weight" << std::endl;
					} else { // the odd-numbered inputs are the to-vertexes, starting with tempdata[1]
						outFile << tempdata[a] << "\t//arc/link to vertex " << tempdata[a] << std::endl;
					}
				} else { // if not weighted, every input is a to-vertex
					outFile << tempdata[a] << "\t//arc/link to vertex " << tempdata[a] << std::endl;
				}
			}
		}
	}        //end for #1
	if (save_graph_data) {
		outputfile << "Data for " << savefile << ":" << std::endl;
	} else {
		outputfile << "Data for your graph:" << std::endl;
	}
	print_adj_matrix();
	return directed;
}

void print_adj_matrix() {
	short a, b;
	outputfile << "adjacency/cost matrix:" << std::endl;
	for (a = 0; a < nvertices; a++) {
		for (b = 0; b < nvertices; b++) {
			outputfile.width(4);
			outputfile << adj_cost_matrix[a][b];
		}
		outputfile << std::endl;
	}
}