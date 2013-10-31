/**********************************
DEEPA CHARI
FALL 2012
CS 490 - SENIOR PROJECT
YALE COLLEGE
**********************************/

#include "header.hpp"	// class declarations

int main(int argc, char** argv) {

	using namespace std;
	
	/**************************** read command line **************************/
	
	if (argc != 4) {
		std::cout << "usage: executable numOfClasses trainCorp testCorp\n";
		exit(1);
	}
	int c = atoi(argv[1]);
	ifstream ftrain(argv[2]);
	ifstream ftest(argv[3]);
	
	/**************************** initialize objects **************************/
	
	Corpus *train, *test;
	try {
		train = new Corpus(c, ftrain);
		test = new Corpus(c, ftest, train);
	}
	catch (logic_error& e) {
		cout << e.what();
		exit(1);
	}
	
	Outputs out(*train);
	Algorithm alg(*train, *test, out);
	
	/**************************** run the algorithm **************************/
	
	#if (DEBUG)
		//cout << "-------------------------------\nBEFORE:\n";
		//alg->print_outputs();
	#endif
	
	alg.Run();
	
	/***************************** print the outputs *************************/
	
	#if (DEBUG)
		//cout << "-------------------------------\nAFTER:\n";
		//alg->print_outputs();
	#endif
	
	string result = (NITER != 0) ? "HALTED" : "CONVERGED";
	cout << result << " after " << alg.num_iter() << " iteration(s) with "
			<< "perplexity " << alg.train_perplexity() << endl;
	cout << "Perplexity of test set: " << alg.test_perplexity() << endl;
	
	/***************************** print the analysis ************************/
	//Analysis an(&alg);
	//an.print_grouping();
	//an.generate();
	
}