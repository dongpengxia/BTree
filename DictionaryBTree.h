/*
 *  DictionaryBTree.h
 *
 *  Spellchecker
 *
 *  11/14/2016
 *  Dongpeng Xia
 */

#ifndef DictionaryBTree_h
#define DictionaryBTree_h

using std::string;





/****************************************************************************
 ****************************************************************************
 **                                                                        **
 **                                                                        **
 **                       class BTreeNode declaration                      **
 **                                                                        **
 **                                                                        **
 ****************************************************************************
 ****************************************************************************/

class BTreeNode
{
public:
    string* keywords;        //keys
    int* accessCounts;       //keys
    BTreeNode** children;    //children
    int numKeys;             //# of keys
    int numChildren;         //# of children
    bool isLeaf;
    BTreeNode(int);
    ~BTreeNode();
    
}; //END class BTreeNode declaration





/****************************************************************************
 ****************************************************************************
 **                                                                        **
 **                                                                        **
 **                   class DictBTree declaration                          **
 **                                                                        **
 **                                                                        **
 ****************************************************************************
 ****************************************************************************/

class DictBTree
{
private:
    BTreeNode* root;
    int m;
public:
    DictBTree(int);
    ~DictBTree();
    void clear(BTreeNode*&);
    BTreeNode* getRoot() { return root; }
    bool searchBTree(BTreeNode*, string);
    void splitChildBTree(BTreeNode*, BTreeNode*, int);
    void insertNonFullBTree(BTreeNode*, string);
    void insert(string);
    void findMostAccessed(BTreeNode*, int&, string&);
    void merge(BTreeNode*, int);
    void remove(BTreeNode*, string);
    
}; //END class DictBTree declaration

#endif