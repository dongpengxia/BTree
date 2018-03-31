# BTree
A spellchecker using a BTree to store the dictionary.

To compile (make sure DictionaryLL.h and DictionaryBTree.h are in the current folder):
g++ main.cpp DictionaryLL.cpp DictionaryBTree.cpp

To run:
./a.out input_file.txt dictionary.txt removeFromDictionary.txt

In the example:
input_file.txt is the name of the file to spellcheck
dictionary.txt is the name of the file with words for the dictionary
removalFromDictionary.txt is the name of the file containing words to remove from the dictionary

Expected Output (replace "Number" with # sign):
LINKED LIST SPELLCHECKER: 
Number of Misspelled Words: 134
Spellchecking runtime: 1.73652
Most frequently accessed word: the
Number of times word was accessed: 129

BTREE SPELLCHECKER: (M = 4)
Number of Misspelled Words: 134
Spellchecking runtime: 0.005124
Most frequently accessed word: the
Number of times word was accessed: 129

BTREE SPELLCHECKER (M = 8):
Number of Misspelled Words: 134
Spellchecking runtime: 0.004633
Most frequently accessed word: the
Number of times word was accessed: 129

LINKED LIST SPELLCHECKER (AFTER WORD REMOVALS):
Number of Misspelled Words: 295
Spellchecking runtime: 2.28061
Most frequently accessed word: the
Number of times word was accessed: 129

BTREE SPELLCHECKER (M = 4, AFTER WORD REMOVALS):
Number of Misspelled Words: 295
Spellchecking runtime: 0.005406
Most frequently accessed word: the
Number of times word was accessed: 129

BTREE SPELLCHECKER (M = 8, AFTER WORD REMOVALS):
Number of Misspelled Words: 295
Spellchecking runtime: 0.004589
Most frequently accessed word: the
Number of times word was accessed: 129
