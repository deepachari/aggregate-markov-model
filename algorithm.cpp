/**********************************
DEEPA CHARI
FALL 2012
CS 490 - SENIOR PROJECT
YALE COLLEGE
**********************************/

#include <cmath>		// for log
#include <cstring>		// for memcpy
#include <ctime>		// for debugging the algorithm
#include "header.hpp"	// class declarations

using namespace std;

inline double& Algorithm::Array3D::operator() (int i, int j, int m) {
	return arr[i*parent.v*parent.c + j*parent.c + m];
}

void Algorithm::Array3D::print () {

	for (int m=0; m<parent.c; ++m) {
		cout << "m = " << m << ":\n";
		for (int i=0; i<parent.v; ++i) {
			for (int j=0; j<parent.v; ++j) {
				cout << operator()(i, j, m) << ' ';
			}
			cout << endl;
		}
	}
	
}

// e-step
void Algorithm::Run () {
	
	Outputs old(train);
	double old_perplexity, new_perplexity (calc_perplexity(train, outputs));
	clock_t start, end;
	iter = 0;
	#if (DEBUG)
		cout << "Run beginning\n";
		cout << "v = " << v << "; c = " << c << endl;
		cout << "#iter\tPerplexity\tTime\t\tTest Perplexity\n";
	#endif
	
	do {
	
		++iter;
		Array3D tmp(*this);
		old.copyfrom(outputs);
		
		// calculate full table of intermediate values (for speedup)
		calc_h(tmp);
		
		// calculate denominators
		double* fdenoms = new double[c];
		double* gdenoms = new double[v];
		for (int m=0; m<c; ++m) {
			fdenoms[m] = 0;
			for (int j=0; j<v; ++j) {
				for (int k=0; k<v; ++k) {
					fdenoms[m] += tmp(j, k, m);
				}
			}
		}
		for (int i=0; i<v; ++i) {
			gdenoms[i] = 0;
			for (int j=0; j<v; ++j) {
				for (int n=0; n<c; ++n) {
					gdenoms[i] += tmp(i, j, n);
				}
			}
		}
		
		// go through each possible word-class pair
		for (int i=0; i<v; ++i) {
			
			for (int m=0; m<c; ++m) {
			
				string w (train.getword(i));

				// compute f and g for word i and class m
				double fnum(0), gnum(0);
				for (int j=0; j<v; ++j) {
					fnum += tmp(j, i, m);
					gnum += tmp(i, j, m);
				}
				
				// update the arrays
				outputs.f[w][m] = fnum / fdenoms[m];
				outputs.g[w][m] = gnum / gdenoms[i];
				
			}
			
		}
		
		old_perplexity = new_perplexity;
		//new_perplexity = calc_perplexity(train, outputs);
		
		#if (DEBUG)
			end = clock();
			double iter_time ((double)(end-start)/CLOCKS_PER_SEC);
			cout << iter << endl; //<< '\t' << train_perplexity() << '\t' << iter_time << '\t'
			//		<< test_perplexity() << endl;
		
			// print each iteration
			/*cout << "-------------------------------\nIteration " 
					<< iter << endl;
			print_outputs();*/
			start = clock();
		#endif
	
	} while (!isConverged(old_perplexity, new_perplexity));
	
}

// computes the probability that word i belongs to class m given that word i
// precedes word j <-- M-STEP
void Algorithm::calc_h (Array3D& tmp) {

	for (int i=0; i<v; ++i) {
		for (int j=0; j<v; ++j) {
			double ct (train.paircount(i, j));
			vector<double> const& fvec (outputs.f[train.getword(j)]);
			vector<double> const& gvec (outputs.g[train.getword(i)]);
			double denom (inner_product(fvec.begin(), fvec.end(), 
					gvec.begin(), 0.0));
			for (int m=0; m<c; ++m) {
				tmp(i, j, m) = ((fvec[m] * gvec[m]) / denom) * ct;
			}
		}
	}
	
}

// computes the log likelihood of the test corpus
double Algorithm::loglk (Corpus& c, Outputs const& o) {

	double result (0);
	
	for (outmap::const_iterator git (o.g.begin()); git != o.g.end(); ++git) {
		string w1 (git->first);
		vector<double> v1 (git->second);
		for (outmap::const_iterator fit (o.f.begin()); fit != o.f.end(); ++fit) 
		{
			string w2 (fit->first);
			vector<double> v2 (fit->second);
			double tprob (inner_product(v1.begin(), v1.end(), v2.begin(), 0.0));
			result += c.paircount(w1, w2) * log(tprob);
		}
	}
	
	return result;
	
}

// tests for convergence
bool Algorithm::isConverged (double old, double neww, double eps) 
{
	double change ((old - neww) / old);
	return NITER == 0 ? change < eps : iter == NITER;
}