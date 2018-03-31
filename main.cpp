/*
 *  main.cpp
 *
 *  Spellchecker
 *
 *  11/14/2016
 *  Dongpeng Xia
 */


#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <stdexcept>
#include "DictionaryLL.h"
#include "DictionaryBTree.h"

using std::string;
using std::stringstream;
using std::fstream;
using std::cout;
using std::endl;

//exception classes
class fileOpenError{};
class tooFewArguments{};

//function lowerAlphaOnly accepts a string and returns it with only lowercase alphabet letters
string lowerAlphaOnly(string);

//loadDictionaryLL reads input dictionary file into a dictionary linked list
void loadDictionaryLL(string, DictLL&);

//function spellCheckInputFileLL opens an input file and spellchecks it, returns misspelled count
unsigned long spellCheckInputFileLL(string, DictLL&);

//function removeFileItemsFromDictLL removes items from removal file from dictionaryLL
void removeFileItemsFromDictLL(string, DictLL&);

//loadDictionaryBTree reads input dictionary file into a dictionary B Tree
void loadDictionaryBTree(string, DictBTree&);

//function spellCheckInputFileBTree opens an input file and spellchecks it, returns misspelled count
unsigned long spellCheckInputFileBTree(string, DictBTree&);

//function removeFileItemsFromDictBTree removes items from removal file from dictionary BTree
void removeFileItemsFromDictBTree(string, DictBTree&);



int main(int argc, const char * argv[])
{
    try
    {
        if(argc > 3)
        {
            string inputFile = string(argv[1]);        //inputFile passed in
            string dictFile = string(argv[2]);         //dictFile passed in
            string removalFile = string(argv[3]);      //removalFile passed in
        
        
            unsigned long numMispelled = 0;
            clock_t timeBegin, timeEnd;
            double spellCheckTime = 0.0;

        
            
            //Linked List Spell Checking
            DictLL lexicon1, lexicon2;
            Node* mostFrequentAccess;
            loadDictionaryLL(dictFile, lexicon1);
            lexicon2 = lexicon1; //copy lexicon1 to lexicon2
            
            timeBegin = clock();
            numMispelled = spellCheckInputFileLL(inputFile, lexicon1);
            timeEnd = clock();
            spellCheckTime = (double)(timeEnd - timeBegin)/CLOCKS_PER_SEC;
            
            mostFrequentAccess = lexicon1.mostPopularWord();
            
            cout << "LINKED LIST SPELLCHECKER:" << endl;
            cout << "# of Misspelled Words: " << numMispelled << endl;
            cout << "Spellchecking runtime: " << spellCheckTime << endl;
            
            if(mostFrequentAccess)
            {
                cout << "Most frequently accessed word: " << mostFrequentAccess->word << endl;
                cout << "# of times word was accessed: " << mostFrequentAccess->findCount << endl;
            }
            else
            {
                cout << "No words in the dictionary were accessed." << endl;
            }
            cout << endl;
            
            
            
            //BTree Spell Checking (M = 4)
            string mostFrequentWord = "";
            int highestFrequency = 0;
            DictBTree* firstTree = new DictBTree(4);
            loadDictionaryBTree(dictFile, *firstTree);
            
            timeBegin = clock();
            numMispelled = spellCheckInputFileBTree(inputFile, *firstTree);
            timeEnd = clock();
            spellCheckTime = (double)(timeEnd - timeBegin)/CLOCKS_PER_SEC;
            
            firstTree->findMostAccessed(firstTree->getRoot(), highestFrequency, mostFrequentWord);
            
            cout << "BTREE SPELLCHECKER: (M = 4)" << endl;
            cout << "# of Misspelled Words: " << numMispelled << endl;
            cout << "Spellchecking runtime: " << spellCheckTime << endl;
            cout << "Most frequently accessed word: " << mostFrequentWord << endl;
            cout << "# of times word was accessed: " << highestFrequency << endl;
            cout << endl;
            
            
            
            //BTree Spell Checking (M = 8)
            mostFrequentWord = "";
            highestFrequency = 0;
            DictBTree* secondTree = new DictBTree(8);
            loadDictionaryBTree(dictFile, *secondTree);
            
            timeBegin = clock();
            numMispelled = spellCheckInputFileBTree(inputFile, *secondTree);
            timeEnd = clock();
            spellCheckTime = (double)(timeEnd - timeBegin)/CLOCKS_PER_SEC;
            
            secondTree->findMostAccessed(secondTree->getRoot(), highestFrequency, mostFrequentWord);
            
            cout << "BTREE SPELLCHECKER (M = 8):" << endl;
            cout << "# of Misspelled Words: " << numMispelled << endl;
            cout << "Spellchecking runtime: " << spellCheckTime << endl;
            cout << "Most frequently accessed word: " << mostFrequentWord << endl;
            cout << "# of times word was accessed: " << highestFrequency << endl;
            cout << endl;
            
            
            
            //Linked List Removal Then Spell Checking
            removeFileItemsFromDictLL(removalFile, lexicon2);
            
            timeBegin = clock();
            numMispelled = spellCheckInputFileLL(inputFile, lexicon2);
            timeEnd = clock();
            spellCheckTime = (double)(timeEnd - timeBegin)/CLOCKS_PER_SEC;
            
            mostFrequentAccess = lexicon2.mostPopularWord();
            
            cout << "LINKED LIST SPELLCHECKER (AFTER WORD REMOVALS):" << endl;
            cout << "# of Misspelled Words: " << numMispelled << endl;
            cout << "Spellchecking runtime: " << spellCheckTime << endl;
            
            if(mostFrequentAccess)
            {
                cout << "Most frequently accessed word: " << mostFrequentAccess->word << endl;
                cout << "# of times word was accessed: " << mostFrequentAccess->findCount << endl;
            }
            else
            {
                cout << "No words in the dictionary were accessed." << endl;
            }
            cout << endl;
            
            
        
            //BTree Removal Then Spell Checking (M = 4)
            mostFrequentWord = "";
            highestFrequency = 0;
            DictBTree* thirdTree = new DictBTree(4);
            loadDictionaryBTree(dictFile, *thirdTree);
            removeFileItemsFromDictBTree(removalFile, *thirdTree);
            
            timeBegin = clock();
            numMispelled = spellCheckInputFileBTree(inputFile, *thirdTree);
            timeEnd = clock();
            spellCheckTime = (double)(timeEnd - timeBegin)/CLOCKS_PER_SEC;
            
            thirdTree->findMostAccessed(thirdTree->getRoot(), highestFrequency, mostFrequentWord);
            
            cout << "BTREE SPELLCHECKER (M = 4, AFTER WORD REMOVALS):" << endl;
            cout << "# of Misspelled Words: " << numMispelled << endl;
            cout << "Spellchecking runtime: " << spellCheckTime << endl;
            cout << "Most frequently accessed word: " << mostFrequentWord << endl;
            cout << "# of times word was accessed: " << highestFrequency << endl;
            cout << endl;
            
            
            
            //BTree Removal Then Spell Checking (M = 8)
            mostFrequentWord = "";
            highestFrequency = 0;
            DictBTree* fourthTree = new DictBTree(8);
            loadDictionaryBTree(dictFile, *fourthTree);
            removeFileItemsFromDictBTree(removalFile, *fourthTree);
            
            timeBegin = clock();
            numMispelled = spellCheckInputFileBTree(inputFile, *fourthTree);
            timeEnd = clock();
            spellCheckTime = (double)(timeEnd - timeBegin)/CLOCKS_PER_SEC;
            
            fourthTree->findMostAccessed(fourthTree->getRoot(), highestFrequency, mostFrequentWord);
            
            cout << "BTREE SPELLCHECKER (M = 8, AFTER WORD REMOVALS):" << endl;
            cout << "# of Misspelled Words: " << numMispelled << endl;
            cout << "Spellchecking runtime: " << spellCheckTime << endl;
            cout << "Most frequently accessed word: " << mostFrequentWord << endl;
            cout << "# of times word was accessed: " << highestFrequency << endl;
            cout << endl;
        
        }
        else
        {
            //error: not enough arguments
            tooFewArguments error;
            throw error;
        }
        
        
    }//END try block
    catch(tooFewArguments error)
    {
        cout << "ERROR: Not enough arguments. Need file names and paths." << endl;
    }
    catch(fileOpenError error)
    {
        cout << "ERROR: File failed to open." << endl;
    }
    catch(bad_alloc error)
    {
        cout << "ERROR: Failed to allocate dynamic memory" << endl;
    }
    catch( ... )
    {
        cout << "ERROR: UNKNOWN ERROR CAUGHT IN MAIN" << endl;
        
    } //END catch all block
    
    return 0;
    
}//END function main()



//function lowerAlphaOnly accepts a string and returns it with only lowercase alphabet letters
string lowerAlphaOnly(string str)
{
    string result = "";
    char c;
    
    for(int index = 0; index < str.length(); index++)
    {
        c = str[index];
        if(isalpha(c))
        {
            result = result + char(tolower(c));
        }
    }
    
    return result;
    
} //end lowerAlphaOnly function



//function loadDictionaryLL opens fileNameAndPath and loads dictionary linked list from input data
void loadDictionaryLL(string fileNameAndPath, DictLL& dictionary)
{
    try
    {
        fstream inFileStream;
        
        //Open file name and path
        inFileStream.open(fileNameAndPath.c_str());
        
        //If the input data file failed to open
        if(!inFileStream)
        {
            fileOpenError fileError;
            throw fileError;
        }
        else
        {
            //fileNameAndPath opened correctly
            string str;
            stringstream stream;
            
            while(inFileStream >> str)
            {
                stream << lowerAlphaOnly(str) << " ";
            }
            stream >> dictionary;
        }
        
        inFileStream.close();
    }
    catch(fileOpenError error)
    {
        throw;
    }
    catch( ... )
    {
        throw;
    }
    
} //end loadDictionaryLL function



//function spellCheckInputFileLL opens fileNameAndPath and spellchecks it, returns # misspelled
unsigned long spellCheckInputFileLL(string fileNameAndPath, DictLL& dictionary)
{
    try
    {
        unsigned long misspelledCount = 0;
        
        fstream inFileStream;
        
        //Open file name and path
        inFileStream.open(fileNameAndPath.c_str());
        
        //If the input data file failed to open
        if(!inFileStream)
        {
            fileOpenError fileError;
            throw fileError;
        }
        else
        {
            //fileNameAndPath opened correctly
            string str;
            
            while(inFileStream >> str)
            {
                str = lowerAlphaOnly(str);
                if(str != "")
                {
                    if(dictionary.seqSearch(str))
                    {
                        //str is in the dictionary
                    }
                    else
                    {
                        //str is not in the dictionary
                        misspelledCount++;
                    }
                }
            }
        }
        inFileStream.close();
        
        return misspelledCount;
    }
    catch(fileOpenError error)
    {
        throw;
    }
    catch( ... )
    {
        throw;
    }

} //end spellCheckInputFileLL



//function removeFileItemsFromDictLL removes items from removal file from dictionaryLL
void removeFileItemsFromDictLL(string fileNameAndPath, DictLL& dictionary)
{
    try
    {
        fstream inFileStream;
        
        //Open file name and path
        inFileStream.open(fileNameAndPath.c_str());
        
        //If the input data file failed to open
        if(!inFileStream)
        {
            fileOpenError fileError;
            throw fileError;
        }
        else
        {
            //fileNameAndPath opened correctly
            string str;
            
            while(inFileStream >> str)
            {
                str = lowerAlphaOnly(str);
                if(str != "")
                {
                    dictionary.remove(str);
                }
            }
        }
        inFileStream.close();
    }
    catch(fileOpenError error)
    {
        throw;
    }
    catch( ... )
    {
        throw;
    }
    
} //end removeFileItemsFromDictLL



//loadDictionaryBTree reads input dictionary file into a dictionary B Tree
void loadDictionaryBTree(string fileNameAndPath, DictBTree& dictionary)
{
    try
    {
        fstream inFileStream;
        
        //Open file name and path
        inFileStream.open(fileNameAndPath.c_str());
        
        //If the input data file failed to open
        if(!inFileStream)
        {
            fileOpenError fileError;
            throw fileError;
        }
        else
        {
            //fileNameAndPath opened correctly
            string str;
            
            while(inFileStream >> str)
            {
                str = lowerAlphaOnly(str);
                if(str != "")
                {
                    dictionary.insert(str);
                }
            }
        }
        
        inFileStream.close();
    }
    catch(fileOpenError error)
    {
        throw;
    }
    catch( ... )
    {
        throw;
    }
    
} //end loadDictionaryBTree function



//function spellCheckInputFileBTree opens an input file and spellchecks it, returns # misspelled
unsigned long spellCheckInputFileBTree(string fileNameAndPath, DictBTree& dictionary)
{
    try
    {
        unsigned long misspelledCount = 0;
        
        fstream inFileStream;
        
        //Open file name and path
        inFileStream.open(fileNameAndPath.c_str());
        
        //If the input data file failed to open
        if(!inFileStream)
        {
            fileOpenError fileError;
            throw fileError;
        }
        else
        {
            //fileNameAndPath opened correctly
            string str;
            
            while(inFileStream >> str)
            {
                str = lowerAlphaOnly(str);
                if(str != "")
                {
                    if(dictionary.searchBTree(dictionary.getRoot(),str))
                    {
                        //str is in the dictionary
                    }
                    else
                    {
                        //str is not in the dictionary
                        misspelledCount++;
                    }
                }
            }
        }
        inFileStream.close();
        
        return misspelledCount;
    }
    catch(fileOpenError error)
    {
        throw;
    }
    catch( ... )
    {
        throw;
    }
    
} //end spellCheckInputFileBTree



//function removeFileItemsFromDictBTree removes items from removal file from dictionary BTree
void removeFileItemsFromDictBTree(string fileNameAndPath, DictBTree& dictionary)
{
    try
    {
        fstream inFileStream;
        
        //Open file name and path
        inFileStream.open(fileNameAndPath.c_str());
        
        //If the input data file failed to open
        if(!inFileStream)
        {
            fileOpenError fileError;
            throw fileError;
        }
        else
        {
            //fileNameAndPath opened correctly
            string str;
            
            while(inFileStream >> str)
            {
                str = lowerAlphaOnly(str);
                if(str != "")
                {
                    dictionary.remove(dictionary.getRoot(),str);
                }
            }
        }
        inFileStream.close();
    }
    catch(fileOpenError error)
    {
        throw;
    }
    catch( ... )
    {
        throw;
    }
    
} //end removeFileItemsFromDictBTree