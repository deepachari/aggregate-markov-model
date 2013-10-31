/**********************************
DEEPA CHARI
FALL 2012
CS 490 - SENIOR PROJECT
YALE COLLEGE
**********************************/

#include "header.hpp"	// class declarations
using namespace std;

void Analysis::print_grouping (unsigned int groupsz) {
	
	outmap::const_iterator it (alg->outputs.f.begin());
	int c (it->second.size());
	vector<pair<string, double> >* grouping = 
			new vector<pair<string, double> >[c];
	
	for (outmap::const_iterator it (alg->outputs.g.begin()); 
			it != alg->outputs.g.end();
			++it)
	{
		int index (0);
		double max (0);
		for (vector<double>::const_iterator vit (it->second.begin());
				vit != it->second.end();
				++vit)
		{
			if (*vit > max) {
				max = *vit;
				index = vit - it->second.begin();
			}
		}
		grouping[index].push_back(pair<string, double>(it->first, max));
	}
	
	Sortbyfreq sortbyfreq(*this);
	Sortbyprob sortbyprob(*this);
	for (int i (0); i < c; ++i) {
		sort(grouping[i].begin(), grouping[i].end(), sortbyfreq);
		vector<pair<string, double> >::iterator start;
		if (groupsz < grouping[i].size()) {
			start = grouping[i].begin() + groupsz;
		}
		else {
			start = grouping[i].end();
		}
		grouping[i].erase(start, grouping[i].end());
		sort(grouping[i].begin(), grouping[i].end(), sortbyprob);
	}
	
	for (int i (0); i < c; ++i) {
		cout << "Class " << i << ": ";
		for (vector<pair<string, double> >::const_iterator it 
				(grouping[i].begin());
				it != grouping[i].end(); 
				++it) 
		{
			cout << it->first << "(" << alg->train.counts[it->first] << ") ";
		}
		cout << endl;
	}
	
}

// nwords is defaulted to NWORDS (defined in header file)
void Analysis::generate (int nwords, int seed) const {
	
	cout << nwords << " words generated from model: ";
	srand(SEED);
	
	// randomly pick the first word
	int w (rand() % alg->outputs.f.size());
	cout << alg->train.words[w] << ' ';
	
	for (int niter(0); niter < nwords; ++niter) {
		// randomly select a class for word w (weighted by probabilities)
		// and the next word based on that class (weighted)
		int m = select_weighted(alg->outputs.g[alg->train.getword(w)]);
		vector<double> fvec;
		for (outmap::const_iterator it (alg->outputs.f.begin());
				it != alg->outputs.f.end();
				++it)
		{
			fvec.push_back(it->second[m]);
		}
		w = select_weighted(fvec);
		cout << alg->train.getword(w) << ' ';
	}
	cout << endl;
	
}

// declared static in header file
int Analysis::select_weighted (vector<double> vec) {

	double r = ((double)rand()) / RAND_MAX;
	double cumsum = 0;
	vector<double>::const_iterator it (vec.begin());
	for ( ; it != vec.end(); ++it)
	{
		cumsum += *it;
		if (cumsum >= r) {
			break;
		}
	}
	
	return (it == vec.end() ? it - vec.begin() - 1 : it - vec.begin());
	
}
