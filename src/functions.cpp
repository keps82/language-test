/*
 *  functions.cpp
 *  
 *  Description: Various non-class functions for LanguageTest.cpp
 *
 *
 *  Created by Añdy Eschbacher on 9/11/11.
 *
 */

#include "functions.h"

extern bool debug;

//  To be used to verify if a given test has been passed
//  This is only a prototype
bool pass(int numOfHints, int numEntries, float totalAvgTime, float totalAvgPercent)
{
    bool passVar = ((numOfHints < (numEntries / 30)) && \
                   (totalAvgTime < 2.0) && \
                   (totalAvgPercent > 0.9));
    return passVar;
}

float howWrongIsIt(string test, string compare)
{
    /* 
     * This function will tell the user how wrong their word is if they so
     * choose to know...  There are several ways to say how wrong they are, so
     * I don't know how to start... AE
     * Ideas:
     *  - percentage of correct letters? - but in correct order?
     *  - choosing similiar consonant?  b instead of p, etc.
     *  - chose correct gender
     *  - etc.
     *
     *  I think there must be a good algorithm out there we can use
     */
    return 0.0;
}

vector<string> stripParentheses(vector<string> words)
{ 
  vector<string> strippedWords = words;
  string paren (" (");
  size_t it;

  for (int i=0;i<words.size();i++)
     {
       it=strippedWords[i].find(paren);
       if(it!=string::npos)
	 {
	 strippedWords[i].erase(strippedWords[i].begin() 
			     + strippedWords[i].find(paren), 
		   strippedWords[i].end());
	 }
    }
  return strippedWords;
}


// Mimics string compare -- returns 1 if there is no match
// I don't like to do all this extra work for each case, but for now it works.
bool isInvalidAnswer(string answer, vector<string> & ws)
{
    vector<string> strippedws = stripParentheses(ws);
    bool isWrong = true;
    vector<string>::iterator it;

    for (it = ws.begin(); it != ws.end(); it++)
        if ( !answer.compare(*it) )
            isWrong = false;

    for (it = strippedws.begin(); it !=strippedws.end(); it++)
        if ( !answer.compare(*it) )
            isWrong = false;

    return isWrong;
}


string hintOptions(int leftmargin)
{
    stringstream hint;
    hint << '\a' << endl; // Ring system bell
    hint << whitespace(leftmargin) << "Want a letter?  Type '-l'." << endl;
    hint << whitespace(leftmargin) << "Want more than one letter? Type '-l#', where # = a number 1 - 9." << endl;
    hint << whitespace(leftmargin) << "Want the answer?  Type '-a'." << endl;
    hint << whitespace(leftmargin) << "Want the number of letters?  Type '-n'." << endl;
    hint << whitespace(leftmargin) << "Want to skip this word?  Type '-s'." << endl;
    hint << whitespace(leftmargin) << "Want help?  Type '-h'." << endl;
    hint << whitespace(leftmargin) << "Want out of here?  Type CTRL-D (End of file -- EOF)." << endl;
    hint << whitespace(leftmargin) << "Want to turn these messages off?  Type '-d' to disable." << endl;
    return hint.str();
}

               
               
void input(vector<Flashcard> & ws, char * inFile)
{
    // Do some error-checking to make sure there are the proper number of
    //   columns, proper encoding(? not binary), etc.
    string temp1, temp2;
    size_t found = string::npos;
    ifstream infile(inFile,ifstream::in);
    Flashcard tempset;
    int lineNum = 1, delimWidth = 1;
    string delimiters[] = {"\t","    ","   ","  "};

    while ( !infile.is_open() )
    {
        if (debug) cout << "in input()" << endl;
        cout << "File '" << inFile << "' does not exist as specified." << endl;
        cout << "Enter another filename (or 'exit' to exit): ";
        cin >> inFile;

        if (cin.eof() || !strcmp(inFile,"exit") || !strcmp(inFile,"'exit'"))
        {
            cout << endl;
            exit(0);
        }
        else
            infile.open(inFile,ifstream::in);
    }

    cout << "Inputting vocabulary from '" << inFile << "'" << endl;
    while ( !infile.eof() )
    {
        getline(infile, temp1);             // Read in line from file

        if (temp1 == "")                    // Skip empty lines
            continue;

        for (int ii = 0; ii < 4 && found == string::npos; ii++)
        {
            found = temp1.find(delimiters[ii]);
            delimWidth = delimiters[ii].size();
        }

        if (found == string::npos) // If none of the delims are found, go to next word
        {
            cout << "Skipping: '" << temp1 << "'" << endl;
            temp1.clear();
            continue;
        }

        temp2 = temp1;                      // Make a copy of the line read in

        try // This my be redundant with previous if statement
        {
            temp1.erase(0,found + delimWidth);
            temp2.erase(found,temp2.size() - found);
        }
        catch (std::out_of_range &e)
        {
            std::cerr << "Caught: '" << e.what() << "' on line " << lineNum << endl;
            std::cerr << "Type: " << typeid(e).name() << endl;
            std::cerr << "Not inputting '" << temp1 << "'" << endl;
            continue;
        }
        catch ( std::exception &e )
        {
            std::cerr << "Caught: " << e.what() << endl;
            std::cerr << "Type: " << typeid(e).name() << endl;
            continue;
        }

        // Insert words into tempset
        insertWords(temp1, tempset, 1);
        insertWords(temp2, tempset, 2);

        // Put tempset into Flashcard vector
        ws.push_back(tempset);

        // Clear variables for next time through
        tempset.clearWS();
        temp1.clear();
        temp2.clear();
        found = string::npos;

        lineNum++;
    }
    cout << endl;

    infile.close();
}



void insertWords(string words, Flashcard & tempset, int step)
{
    size_t found;
    if (debug) cout << "words are: '" << words << "'" << endl;

    while ( words.find(",") != string::npos )   // while comma is found
    {
        found = words.rfind(",");
        string tempWord = words.substr(found+1);
        if (debug) cout << "New word: " << tempWord << endl;
        switch (step)
        {
            case 1:
                if (debug) cout << "case : " << step << endl;
                tempset.sideA.push_back(tempWord);
                break;
            case 2:
                if (debug) cout << "case : " << step << endl;
                tempset.sideB.push_back(tempWord);
                break;
            default:
                break; // What should we do for this case?
        }
        words.erase(found);
    }

    if (debug) cout << "New word: " << words << endl;

    switch (step)   // Otherwise catch non-comma case
    {
        case 1:
            if (debug) cout << "case : " << step << endl;
            tempset.sideA.push_back(words);
            break;
        case 2:
            if (debug) cout << "case : " << step << endl;
            tempset.sideB.push_back(words);
            break;
        default:
            break;  // What should we do for this case?
    }
}

// isnew() is obsolete
bool isnew(vector<Flashcard> & ws, string test, long unsigned int & index)
{   // Returns true if 'test' is not already in the vector ws (i.e., if its new)
    // Also sets the value where a non-new word occurs
    bool isNew = true;

    if (ws.size() == 0)
        isNew = true;
    else
        for (int i = 0; i < ws.size(); i++)
            if ( !test.compare(ws[i].sideB[0]) )
            {
                isNew = false;
                index = i;
                break;
            }

    return isNew;
}


string ordinal(int num)
{
    string ords[10] = { "th", "st", "nd", "rd", "th", \
                        "th", "th", "th", "th", "th"};
    stringstream ord;
    ord << num << ords[num % 10];
    return ord.str();
}

string help(char * prog)
{
    stringstream help;
    help << "Commandline language learner. Version 1.0 Beta" << endl;
    help << "Kandy Software. Always wary." << endl;
    help << endl;
    help << "usage:" << endl;
    help << "  " << prog << " [options]" << endl;
    help << endl;
    help << "options:" << endl;
    help << "    -i <string>    input file name" << endl;
    help << "    -v             turn off verbose output" << endl;
    help << "    -l             list available dictionaries" << endl;
    help << "    -h             print out this help menu" << endl;
    help << "    -d             print debugging information to troubleshoot" << endl;
    help << endl;
    return help.str();
}

int randIndex(int num)
{
    return (rand() % num);
}

double reaction(double time, int numLttrs)
{
    // 0.28 = seconds per letter if wpm = 100 and avg word is 6 letters long
    double reactionTime = time - 0.28 * numLttrs;
    if (reactionTime < 0.0)
        reactionTime = 0.0;
    if (debug) cout << "reactionTime = " << reactionTime << endl;

    return reactionTime;
}

// int weightedIndex(WordData * data, int numEntries)
// {
//     static int lastIndex;
//     int currIndex;
//     extern boost::mt19937 gen;
//     double prob[numEntries];
//         // Copy probabilities to simple array so partial_sum() can use it.
//         // It's possible that this step isn't necessary but I cannot figure out
//         // a way to use consecutive pointers in the partial_sum() function for
//         // the structure data[ii].probability
//     for (int ii = 0; ii < numEntries; ii++)
//         prob[ii] = data[ii].probability;
//     do
//     {
//         vector<double> cumulative;
//         std::partial_sum(&prob[0], &prob[0] + numEntries, \
//                          std::back_inserter(cumulative));
//         if (debug) cout << "partial_sum() calculated" << endl;
//         boost::uniform_real<> dist(0.0, cumulative.back());
//         boost::variate_generator<boost::mt19937&, boost::uniform_real<> > prob(gen, dist);
//         currIndex = std::lower_bound(cumulative.begin(), cumulative.end(), prob()) - cumulative.begin();
//     } while (currIndex == lastIndex);

//     lastIndex = currIndex;

//     return currIndex;
// }


string whitespace(int length)
{
    string whitespace;
    for (int k = 0; k < length + 2; k++)
        whitespace += " ";
    return whitespace;
}

string goodbye(void)
{
    string goodbyes[] = {"Goodbye", "Hej då", "Sayonara", "¡Adiós",
                         "Adieu", "Ciao", "Tchüss", "Au revoir",
                         "Namaste"};
        // What do you think?  Should those be in a file instead? Nice! Yeah, we could generate a goodbye file with loads and loads of languages.  It'd be a nice touch :) Maybe a welcome message at the start too.

    return goodbyes[randIndex(9)] + "!";
}


