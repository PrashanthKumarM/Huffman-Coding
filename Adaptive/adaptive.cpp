#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <fstream>
#include <queue>
#include <ctime>
#include "adaptive.h"
#include "vit.cpp"
using namespace std;

int main() {	

  char infilename[32], outfilename[32];
  char option;
  clock_t start, finish;

  cout << "[E]ncode or [D]ecode?\n";
  cin >> option;

  if (option == 'E') {

    cout << "Enter the name of input file: ";
    cin >> infilename;
    sscanf(infilename, "%[^.]", outfilename);
    strcat(outfilename, ".bin");

    ifstream instream;
	  instream.open(infilename, ios::in | ios::binary);
	
	  ofstream outstream;
	  outstream.open(outfilename, ios::out | ios::binary);
    
    start = clock();		
    compress(&instream, &outstream);
    finish = clock();
    cout << "\nExecution time: " << ((double)(finish - start))/CLOCKS_PER_SEC << "s\n";

  } else if (option == 'D') {
  	cout << "Enter the name of input file: ";
    cin >> infilename;
    sscanf(infilename, "%[^.]", outfilename);
    strcat(outfilename, ".out");

    ifstream instream;
	  instream.open(infilename, ios::in | ios::binary);
	
	  ofstream outstream;
	  outstream.open(outfilename, ios::out | ios::binary);
    
    start = clock();	
	  outstream.open(outfilename, ios::out | ios::binary);
    		
    decompress(&instream, &outstream);
    finish = clock();
    cout << "\nExecution time: " << ((double)(finish - start))/CLOCKS_PER_SEC << "s\n";
    
  } else {
    cout << "Invalid Option!!";
  }
}