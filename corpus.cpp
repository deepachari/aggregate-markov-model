/**********************************
DEEPA CHARI
FALL 2012
CS 490 - SENIOR PROJECT
YALE COLLEGE
**********************************/

#include <string>		// to represent words
#include "header.hpp"	// class declarations

using namespace std;

Corpus::Corpus (int c_, ifstream& fin, Corpus* old, double smooth) 
: words(), counts(), W(), I(), v(), c(c_), vtot()
{

	try {
		getwords(fin, old, smooth);
	}
	catch (logic_error& e) {
		throw;
	}
	
	#if (DEBUG)
		cout << vtot << " words read; corpus initialized\n";
	#endif
	
}

void Corpus::readin(string word) {

	words.push_back(word);
	++counts[word];
	++v;
	
}

string Corpus::process(string word, Corpus* old) {

	if (old != NULL) {
		if (find(old->words.begin(), old->words.end(), word) 
				== old->words.end()) 
		{
			return "UNK";
		}
	}
	
	if (!isalpha((int)word[0]) and word.size() > 1) {
		return "#";
	}
	
	return word;
	
}

// get the list of distinct words and the counts of word pairs
void Corpus::getwords (ifstream& fin, Corpus* oldcorp, double smooth) {

	string w;
	int old(0);
	
	// read in first word
	fin >> w;
	if (!w.empty()) {
		w = process(w, oldcorp);
		readin(w);
	}
	
	// read in rest of words
	while (fin >> w) {
	
		if (w.empty()) continue;
		if (w[0] == '[') continue;
		
		w = process(w, oldcorp);
		
		// if word was already read in, get its index
		stringvec::const_iterator it = find(words.begin(), words.end(), w);
		unsigned int index = it - words.begin();
		
		// update words, counts, and pair counts
		if (index == words.size()) {
			readin(w);
		}
		else {
			++counts[w];
		}
		++paircount(old, index);
		++paircount(words[old], w);
		
		// bookkeeping
		old = index;
		++vtot;
		
	}
	
	stringvec::const_iterator it = find(words.begin(), words.end(), "UNK");
	if (it == words.end()) {
		words.push_back("UNK");
	}
	
	if (words.empty() || counts.empty()) {
		throw logic_error("No words read\n");
	}
	
	// smoothing
	for (int i (0); i < v; ++i) {
		for (int j (0); j < v; ++j) {
			paircount(words[i], words[j]) += smooth;
			paircount(i, j) += smooth;
		}
	}

}

/************* functions called by Algorithms friend class ***************/

void Corpus::print () const {

	cout << endl << "Word:\tCount:\n";
	for (intmap::const_iterator it (counts.begin()); it != counts.end(); ++it) {
		cout << it->first << '\t' << it->second << endl;
	}
	cout << endl;
	cout << "Pair counts:\n";
	for (wordmap::const_iterator i = W.begin(); i != W.end(); ++i) {
		cout << (i->first).first << '\t' << (i->first).second << '\t'
				<< i->second << endl;
	}
}

/*
// unit test - debugging
int main (int argc, char** argv) {

	using namespace std;
	
	int c = atoi(argv[1]);
	ifstream ftrain(argv[2]);
	ifstream ftest(argv[3]);
	Corpus* train, *test;
	
	try {
		train = new Corpus(c, ftrain);
		test = new Corpus(c, ftest);
	}
	catch (logic_error& e) {
		cout << e.what();
		exit(1);
	}
	
	cout << "TRAIN: \n";
	train->print();
	cout << "TEST: \n";
	test->print();
	
}
*/
