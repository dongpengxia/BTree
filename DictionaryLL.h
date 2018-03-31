/*
 *  DictionaryLL.h
 *
 *  Spellchecker
 *
 *  11/14/2016
 *  Dongpeng Xia
 */

#ifndef DictionaryLL_h
#define DictionaryLL_h

using std::ostream;
using std::istream;
using std::bad_alloc;
using std::string;





/****************************************************************************
 ****************************************************************************
 **                                                                        **
 **                                                                        **
 **                     class Node declaration                             **
 **                                                                        **
 **                                                                        **
 ****************************************************************************
 ****************************************************************************/

class Node
{
public:
    string word;
    unsigned int findCount; //number of times we had to find this word
    Node *next;
    Node(string = "", unsigned int = 0, Node* = NULL);
    
};//END class Node declaration





/****************************************************************************
 ****************************************************************************
 **                                                                        **
 **                                                                        **
 **                      class DictLL declaration                          **
 **                                                                        **
 **                                                                        **
 ****************************************************************************
 ****************************************************************************/

class DictLL
{
    friend ostream& operator<<(ostream&, const DictLL&);
    //Postcondition: The dictionary linked list is output to the stream
    
    friend istream& operator>>(istream&, DictLL&);
    //Postcondition: Extracts values for each data member from the input stream.
    
private:
    unsigned long count;
    Node* head;
    Node* tail;
    
    void copyList(const DictLL &listToCopy);
    //copyList is a private function called by both copy constructor and overloaded = operator
    //must ensure deep copy
    
public:
    DictLL();
    
    ~DictLL();
    //destructor, must ensure memory is deallocated
    
    DictLL(const DictLL&);
    //copy constructor
    
    DictLL operator=(const DictLL&);
    //overloaded = operator
    
    void push_back(string, unsigned int);
    //member function push_back results in a Node object being added to the end of the linked
    //list dictionary, requires dynamic allocation of memory
    
    void clear();
    //member function clear removes all objects from the list and deallocates memory
    
    bool remove(string);
    //member function remove deletes the node with the argument string in it
    //returns false if string not in linked list
    
    bool seqSearch(string);
    //member function seqSearch runs a sequential search looking for a string
    //if the string is found, the node is incremented and moved to the front of the linked list
    //if the string is not found, the function returns false
    
    Node* mostPopularWord();
    //member function mostPopularWord finds the Node with the highest findCount member
    
    unsigned long size() const { return count; }
    //member function size returns number of objects in the dictionary linked list
    
    
};//END class DictLL declaration

#endif