/*
 *  DictionaryLL.cpp
 *
 *  Spellchecker
 *
 *  11/14/2016
 *  Dongpeng Xia
 */

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "DictionaryLL.h"

using std::ostream;
using std::istream;
using std::bad_alloc;
using std::string;
using std::setw;



/****************************************************************************
 ****************************************************************************
 **                                                                        **
 **                                                                        **
 **                     class Node implementation                          **
 **                                                                        **
 **                                                                        **
 ****************************************************************************
 ****************************************************************************/

/****************************************************************************
 *        Node class default constructor/constructor with parameters        *
 ****************************************************************************/

Node::Node(string w, unsigned int c, Node* nextNode): word(w), findCount(c), next(nextNode)
{
    
} //END constructor with parameters for Node class





/****************************************************************************
 ****************************************************************************
 **                                                                        **
 **                                                                        **
 **                  class DictLL implementation                           **
 **                                                                        **
 **                                                                        **
 ****************************************************************************
 ****************************************************************************/

/****************************************************************************
 *         DictLL class overloaded stream insertion operator                *
 ****************************************************************************/

ostream& operator<<(ostream &os, const DictLL &linkList)
{
    Node *current = linkList.head;
    
    while(current != NULL)
    {
        os << setw(50) << current->word << "\t" << current->findCount << "\n";
        current = current->next;
    }
    
    return os;
    
} //END overloaded stream insertion operator for DictLL class



/****************************************************************************
 *             DictLL class overloaded stream extraction operator           *
 ****************************************************************************/

istream& operator>>(istream &is, DictLL &d1)
{
    string str;
    
    while(is >> str)
    {
        d1.push_back(str, 0);
    }
    
    return is;
    
} //END overloaded stream extraction operator for DictLL class



/****************************************************************************
 *                      DictLL class copyList function                      *
 ****************************************************************************/

void DictLL::copyList(const DictLL &listToCopy)
{
    try
    {
        //if listToCopy is nonempty
        if(listToCopy.count > 0)
        {
            Node *current = listToCopy.head;
            head = new Node(current->word, current->findCount, NULL);
            tail = head;
            
            while(current->next != NULL)
            {
                this->push_back(current->next->word, current->next->findCount);
                current = current->next;
            }
            
            count = listToCopy.size();
        }
        //if listToCopy is empty
        else
        {
            head = NULL;
            tail = NULL;
            count = 0;
        }
        
    }//end try block
    
    catch(bad_alloc ba)
    {
        throw;
        
    }//end catch block
    catch(...)
    {
        throw;
        
    }//end catch block
    
} //END DictLL class private function copyList



/****************************************************************************
 *                   DictLL class default constructor                       *
 ****************************************************************************/

DictLL::DictLL(): count(0), head(NULL), tail(NULL)
{
    //see member initialization list above
    
} //END default constructor for DictLL class



/****************************************************************************
 *                      DictLL class destructor                             *
 ****************************************************************************/

DictLL::~DictLL()
{
    //call the clear function to remove all objects and update count to 0
    this->clear();
    
} //END destructor for DictLL class



/****************************************************************************
 *                      DictLL class copy constructor                       *
 ****************************************************************************/

DictLL::DictLL(const DictLL &otherDictLL)
{
    try
    {
        this->copyList(otherDictLL);
    }//end try block
    
    catch(bad_alloc ba)
    {
        throw;
        
    }//end catch block
    catch(...)
    {
        throw;
        
    }//end catch block
    
} //END copy constructor for DictLL class



/****************************************************************************
 *               DictLL class overloaded assignment operator                *
 ****************************************************************************/

DictLL DictLL::operator=(const DictLL &rhsObj)
{
    try
    {
        //check against self assignment
        if(this != &rhsObj)
        {
            //destroy current linked list
            this->clear();
            this->copyList(rhsObj);
        }
        
        return rhsObj;
    }//end try block
    
    catch(bad_alloc ba)
    {
        throw;
        
    }//end catch block
    catch(...)
    {
        throw;
        
    }//end catch block
    
} //END overloaded assignment operator for DictLL class



/****************************************************************************
 *                   DictLL class push_back member function                 *
 ****************************************************************************/

void DictLL::push_back(string cWord, unsigned int cfindCount)
{
    try
    {
        if(head == NULL)
        {
            //linked list is empty, add one Node to beginning
            head = new Node(cWord, cfindCount, NULL);
            tail = head;
        }
        
        else
        {
            tail->next = new Node(cWord, cfindCount, NULL);
            tail = tail->next;
        }
        
        //increment count
        count++;
        
    }//end try block
    catch(bad_alloc ba)
    {
        throw;
        
    }//end catch block
    catch(...)
    {
        throw;
        
    }//end catch block
    
} //END DictLL class member function push_back



/****************************************************************************
 *                      DictLL class clear member function                  *
 ****************************************************************************/

void DictLL::clear()
{
    Node *current = head;
    
    while(head != NULL)
    {
        head = head->next;
        delete current;
        current = head;
    }
    
    current = head = tail = NULL;
    count = 0;
    
} //END DictLL class member function clear



/****************************************************************************
 *                     DictLL class remove member function                  *
 ****************************************************************************/

bool DictLL::remove(string removeStr)
{
    bool found = false;
    
    if(count == 1)
    {
        if(head->word == removeStr)
        {
            found = true;
            delete head;
            head = tail = NULL;
            count = 0;
        }
    }
    else if (count > 1)
    {
        //at least two items in linked list
        Node *deleteThis;
        Node *current;
        
        if(head->word == removeStr)
        {
            found = true;
            current = head;
            head = head->next;
            delete current;
            current = NULL;
            count--;
        }
        else if (tail->word == removeStr)
        {
            found = true;
            current = head;
            while(current->next != tail)
            {
                current = current->next;
            }
            delete tail;
            tail = current;
            tail->next = NULL;
            count--;
        }
        else
        {
            //neither head nor tail contain removeStr
            //can only delete from middle of the list
            
            current = head;
            
            while(current->next != tail && !found)
            {
                if(current->next->word == removeStr)
                {
                    found = true;
                    deleteThis = current->next;
                    current->next = current->next->next;
                    delete deleteThis;
                    deleteThis = NULL;
                    count--;
                }
                current = current->next;
            }
        }
    }

    return found;
    
} //END DictLL class member function removeStr



/****************************************************************************
 *                  DictLL class seqSearch member function                  *
 ****************************************************************************/

bool DictLL::seqSearch(string searchStr)
{
    bool found = false;
    
    if(count == 1)
    {
        if(head->word == searchStr)
        {
            found = true;
            (head->findCount)++;
        }
    }
    else if (count > 1)
    {
        //at least two items in linked list
        Node *current;
        Node *temp;
        
        if(head->word == searchStr)
        {
            found = true;
            (head->findCount)++;
        }
        else if (tail->word == searchStr)
        {
            found = true;
            (tail->findCount)++;
            
            current = head;
            while(current->next != tail)
            {
                current = current->next;
            }
            
            //move tail to the front
            tail->next=head;
            head = tail;
            tail = current;
            tail->next = NULL;
        }
        else
        {
            //neither head nor tail contain searchStr
            //can only be in middle of the list
            
            current = head;
            
            while(current->next != tail && !found)
            {
                if(current->next->word == searchStr)
                {
                    found = true;
                    (current->next->findCount)++;
    
                    //move to front
                    temp = current->next;
                    current->next = current->next->next;
                    temp->next = head;
                    head = temp;
                }
                current = current->next;
            }
        }
    }
    return found;
    
} //END DictLL class member function seqSearch



/****************************************************************************
 *               DictLL class mostPopularWord member function               *
 ****************************************************************************/

Node* DictLL::mostPopularWord()
{
    Node* topWord = NULL;
    unsigned int highestFindCount = 0;
    
    Node* current = head;
            
    //short circuit evaluation prevents illegal access
    while(current && ((current->findCount) > 0))
    {
        if((current->findCount) > highestFindCount)
        {
            highestFindCount = current->findCount;
            topWord = current;
        }
        current = current->next;
    }
    
    return topWord;
    
} //END DictLL class member function mostPopularWord