/*
 *  functions.h
 *  
 *  Description: Header file to go with functions.cpp
 *
 *
 *  Created by Añdy Eschbacher on 9/11/11.
 *
 */

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

#include "Flashcard.h"
#include "WordData.h"

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::stringstream;
using std::vector;

bool pass(int, int, float, float);
bool compareAll(vector<string> &, string);
bool isInvalidAnswer(string, vector<string> &);
string hintOptions(int);
string hint(int, bool, int, string, int);
void input(vector<Flashcard> &, char *);
void insertWords(string, Flashcard &,int);
bool isnew(vector<Flashcard> &, string, long unsigned int &);
string ordinal(int num);
string help(char *);
int randIndex(int);
double reaction(double, int);
int weightedIndex(WordData *, int);
string whitespace(int length);
float howWrongIsIt(string,string);
string goodbye(void);
#endif
