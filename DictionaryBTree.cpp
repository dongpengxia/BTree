/*
 *  DictionaryBTree.cpp
 *
 *  Spellchecker
 *
 *  11/14/2016
 *  Dongpeng Xia
 */

#include <string>
#include "DictionaryBTree.h"

using std::string;


/****************************************************************************
 ****************************************************************************
 **                                                                        **
 **                                                                        **
 **                   class BTreeNode implementation                       **
 **                                                                        **
 **                                                                        **
 ****************************************************************************
 ****************************************************************************/

/****************************************************************************
 *               BTreeNode class constructor with parameters                *
 ****************************************************************************/

BTreeNode::BTreeNode(int m)
{
    keywords = new string[m-1]();
    accessCounts = new int[m-1]();
    children = new BTreeNode*[m]();
    numKeys = 0;
    numChildren = 0;
    isLeaf = true;
    
} //END constructor with parameters for BTreeNode class



/****************************************************************************
 *                        BTreeNode class destructor                        *
 ****************************************************************************/

BTreeNode::~BTreeNode()
{
    delete[] keywords;
    delete[] accessCounts;
    delete[] children;
    keywords = NULL;
    accessCounts = NULL;
    children = NULL;
    numKeys = 0;
    numChildren = 0;
    isLeaf = true;
    
} //END destructor for BTreeNode class





/****************************************************************************
 ****************************************************************************
 **                                                                        **
 **                                                                        **
 **                   class DictBTree implementation                       **
 **                                                                        **
 **                                                                        **
 ****************************************************************************
 ****************************************************************************/

/****************************************************************************
 *               DictBTree class constructor with parameters                *
 ****************************************************************************/

DictBTree::DictBTree(int num)
{
    m = num;
    root = new BTreeNode(num);
    
} //END constructor with parameters for DictBTree



/****************************************************************************
 *                       DictBTree class destructor                         *
 ****************************************************************************/

DictBTree::~DictBTree()
{
    clear(root);
    root = NULL;
    
} //END destructor for DictBTree



/****************************************************************************
 *                  DictBTree class member function clear                   *
 ****************************************************************************/

void DictBTree::clear(BTreeNode* &currentRoot)
{
    for(int i = 0; i < currentRoot->numChildren; i++)
    {
        clear(currentRoot->children[i]); //recursive call to delete children first
    }
    
    //delete currentRoot after its children have been deleted
    delete currentRoot;
    currentRoot = NULL;
    
} //END DictBTree member function clear()



/****************************************************************************
 *              DictBTree class member function splitChildBTree             *
 ****************************************************************************/

void DictBTree::splitChildBTree(BTreeNode* parent, BTreeNode* child, int i)
{
    //child is a full child of parent
    //the right half of child will be copied into newNode
    //then the median key of child will be copied into parent
    //so that child will be to the left of parent's median key
    //and newNode will be to the right of parent's median key
    //Thus, child is split into two nodes.
    
    BTreeNode* newNode = new BTreeNode(m); //make new node which will hold the right half of child
    newNode->isLeaf = child->isLeaf;
    
    if(!(child->isLeaf))
    {
        //copy right half of children from child to newNode
        for (int j = 0; j < (m/2); j++)
        {
            if(child->children[j+((m+1)/2)] != NULL)
            {
                newNode->children[j] = child->children[j+((m+1)/2)];
                (newNode->numChildren)++;
                child->children[j+((m+1)/2)] = NULL;
                (child->numChildren)--;
            }
        }
    }
    
    //copy right half of keys from child to newNode
    for(int j = 0; j < ((m/2)-1); j++)
    {
        newNode->keywords[j] = child->keywords[j+((m+1)/2)];
        newNode->accessCounts[j] = child->accessCounts[j+((m+1)/2)];
        (newNode->numKeys)++;
        child->keywords[j+((m+1)/2)] = "";
        child->accessCounts[j+((m+1)/2)] = 0;
        (child->numKeys)--;
    }
    
    //shift children in parent to make room
    for(int j = ((parent->numChildren)-1); j > (i-1); j--)
    {
        parent->children[j+1] = parent->children[j];
    }
    
    parent->children[i] = newNode;
    (parent->numChildren)++;
    
    //shift keys in parent to make room
    for(int j = ((parent->numKeys)-1); j >= (i-1); j--)
    {
        parent->keywords[j+1] = parent->keywords[j];
        parent->accessCounts[j+1] = parent->accessCounts[j];
    }
    
    //copy over median key into parent
    parent->keywords[i-1] = child->keywords[(m-1)/2];
    parent->accessCounts[i-1] = child->accessCounts[(m-1)/2];
    child->keywords[(m-1)/2] = "";
    child->accessCounts[(m-1)/2] = 0;
    (child->numKeys)--;
    (parent->numKeys)++;
    
} //END DictBTree member function splitChildBTree()



/****************************************************************************
 *            DictBTree class member function insertNonFullBTree            *
 ****************************************************************************/

void DictBTree::insertNonFullBTree(BTreeNode* iNode, string str)
{
    bool repeatAfterSplit = false;
    bool isRepeat = false;
    int i = (iNode->numKeys)-1;
    
    //check keywords to make sure word to be inserted is not a repeat
    int j = 0;
    while((j <= i) && !isRepeat)
    {
        if(iNode->keywords[j] == str)
        {
            isRepeat = true;
        }
        j++;
    }
    
    if(!isRepeat)
    {
        if(iNode->isLeaf)   //insert into leaf
        {
            //move keys to make space
            while((i >= 0) && (str < (iNode->keywords[i])))
            {
                iNode->keywords[i+1] = iNode->keywords[i];
                i--;
            }
            //insert key
            iNode->keywords[i+1] = str;
            iNode->accessCounts[i+1] = 0;
            (iNode->numKeys)++;
        }
        else
        {
            //find child to go down
            i = ((iNode->numChildren)-1);
            while((i > 0) && (str < (iNode->keywords[i-1])))
            {
                i--;
            }
            
            //split the child if it is full
            if((iNode->children[i]->numKeys) == (m-1))
            {
                splitChildBTree(iNode, iNode->children[i], (i+1));
                if(str > (iNode->keywords[i]))
                {
                    i++;
                }
                else if (str == iNode->keywords[i])
                {
                    //check for repeats at median after splitting
                    repeatAfterSplit = true;
                }
            }
            if(!repeatAfterSplit)
            {
                //recursive call
                insertNonFullBTree((iNode->children[i]), str);
            }
        }
    }
    
} //END DictBTree member function insertNonFullBTree



/****************************************************************************
 *                   DictBTree class member function insert                 *
 ****************************************************************************/

void DictBTree::insert(string istr)
{
    if((root->numKeys) == (m-1))
    {
        //root is full
        BTreeNode* copyBTreeRoot = root;
        root = new BTreeNode(m);
        root->isLeaf = false;
        root->numChildren = 1;
        root->children[0] = copyBTreeRoot;
        splitChildBTree(root, copyBTreeRoot, 1);
        insertNonFullBTree(root, istr);
    }
    else
    {
        insertNonFullBTree(root, istr);
    }
    
} //END DictBTree member function insert



/****************************************************************************
 *              DictBTree class member function searchBTree                 *
 ****************************************************************************/

bool DictBTree::searchBTree(BTreeNode* currentRoot, string searchStr)
{
    if(!currentRoot)
    {
        return false;
    }
    
    //find correct index
    int i = 0;
    while((i < (currentRoot->numKeys)) && (searchStr > (currentRoot->keywords[i])))
    {
        i++;
    }
    
    //found in current node
    if((i < (currentRoot->numKeys)) && (searchStr == (currentRoot->keywords[i])))
    {
        ((currentRoot->accessCounts)[i])++;
        return true;
    }
    if((currentRoot->isLeaf) || currentRoot->children[i] == NULL)
    {
        //search key is not in the tree
        return false;
    }
    else
    {
        //recursive call to go down child
        return searchBTree(currentRoot->children[i], searchStr);
    }
    
} //END DictBTree member function searchBTree



/****************************************************************************
 *           DictBTree class member function findMostAccessed               *
 ****************************************************************************/

void DictBTree::findMostAccessed(BTreeNode* currentRoot, int &highestCount, string &mostPopularWord)
{
    for(int i = 0; i < currentRoot->numKeys; i++)
    {
        if(currentRoot->accessCounts[i] > highestCount)
        {
            //a more frequent word has been found
            highestCount = currentRoot->accessCounts[i];
            mostPopularWord = currentRoot->keywords[i];
        }
    }
    for(int j = 0; j < currentRoot->numChildren; j++)
    {
        //recursive call for each child
        findMostAccessed(currentRoot->children[j], highestCount, mostPopularWord);
    }
    
} //END DictBTree member function findMostAccessed



/****************************************************************************
 *               DictBTree class member function merge                      *
 ****************************************************************************/

void DictBTree::merge(BTreeNode* parent, int lcindex)
{
    //lcindex stands for "left child index"
    //parent's children at lcindex (left) and lcindex+1 (right) are merged into left child
    
    BTreeNode* left = parent->children[lcindex];
    
    //copy over parent's key between left child and right child
    left->keywords[left->numKeys] = parent->keywords[lcindex];
    left->accessCounts[left->numKeys] = parent->accessCounts[lcindex];
    left->numKeys++;
    
    BTreeNode* right = parent->children[lcindex+1];
    
    //copy over right's children into left (if there are any)
    int i = left->numChildren;
    for(int j = 0; j < right->numChildren; j++)
    {
        left->children[i+j] = right->children[j];
        left->numChildren++;
    }
    
    //copy over right's keys into left
    i = left->numKeys;
    for(int j = 0; j < right->numKeys; j++)
    {
        left->keywords[i+j] = right->keywords[j];
        left->accessCounts[i+j] = right->accessCounts[j];
        left->numKeys++;
    }
    
    //don't need right anymore
    delete right;
    right = NULL;
    
    //shift parent's keys to cover original key between left and right
    i = lcindex;
    for(i = lcindex; i < parent->numKeys-1; i++)
    {
        parent->keywords[i] = parent->keywords[i+1];
        parent->accessCounts[i] = parent->accessCounts[i+1];
    }
    parent->keywords[i] = "";
    parent->accessCounts[i] = 0;
    parent->numKeys--;
    
    //shift parent's children to cover original location of right child
    i = lcindex + 1;
    for(i = lcindex+1; i < (parent->numChildren - 1); i++)
    {
        parent->children[i] = parent->children[i+1];
    }
    parent->children[i] = NULL;
    parent->numChildren--;

} //END DictBTree member function merge



/****************************************************************************
 *              DictBTree class member function remove                      *
 ****************************************************************************/

void DictBTree::remove(BTreeNode* current, string rStr)
{
    if(!current)
    {
        return; //current is NULL
    }
    
    if(root->numKeys == 0 && !(root->isLeaf))
    {
        //root has no keys, change root to one layer below
        BTreeNode* toDelete = root;
        root = root->children[0];
        delete toDelete;
        toDelete = NULL;
    }
    
    //find removal index in current node
    int i = 0;
    while((i < current->numKeys) && rStr > (current->keywords[i]))
    {
        i++;
    }
    
    //if the key to be removed is a key in the current node
    if((i < current->numKeys) && (rStr == current->keywords[i]))
    {
        //found it!
        if(current->isLeaf)
        {
            //case 1
            //current node is a leaf
            //shift keys over for deletion
            int j = i;
            for(j = i; j < current->numKeys-1; j++)
            {
                current->keywords[j] = current->keywords[j+1];
                current->accessCounts[j] = current->accessCounts[j+1];
            }
            current->keywords[j] = "";
            current->accessCounts[j] = 0;
            current->numKeys--;
        }
        else
        {
            //case 2
            //current node is not a leaf
            
            if(current->children[i] && (i < current->numChildren)
               && ((current->children[i]->numKeys) > ((m/2)-1)))
            {
                //left child has more than minimum number of keys
                
                BTreeNode* placeHolder = current;
                
                current = current->children[i];
                while(!(current->isLeaf))
                {
                    current = current->children[current->numChildren-1];
                }
                
                //make copy of biggest key in left child's subtree
                string temp = current->keywords[current->numKeys - 1];
                int tempNum = current->accessCounts[current->numKeys - 1];
                
                //recursive call to delete biggest key in left child's subtree
                remove(root, temp);
                
                //overwrite biggest key into current node for deletion
                placeHolder->keywords[i] = temp;
                placeHolder->accessCounts[i] = tempNum;
            }
            else if(current->children[i+1] && ((i+1) < current->numChildren)
                    && (current->children[i+1]->numKeys > ((m/2)-1)))
            {
                //right child has more than minimum number of keys
                
                BTreeNode* placeHolder = current;
                
                current = current->children[i+1];
                while(!(current->isLeaf))
                {
                    current = current->children[0];
                }
                
                //make copy of smallest key in right child's subtree
                string temp = current->keywords[0];
                int tempNum = current->accessCounts[0];
                
                //recursive call to delete smallest key in right child's subtree
                remove(root, temp);
                
                //overwrite smallest key into current node for deletion
                placeHolder->keywords[i] = temp;
                placeHolder->accessCounts[i] = tempNum;
            }
            else
            {
                //both children[i+1] and children[i] have minimum number of keys
                merge(current, i);
                
                //now the search key is in children[i]
                remove(current->children[i], rStr);
            }
        }
    }
    else if(!(current->isLeaf) && current->children[i] != NULL)
    {
        //did not find in current node, need to keep searching down child
        
        if(current->children[i]->numKeys <= ((m/2)-1))
        {
            //case 3
            //next child to search has only the minimum number of keys
            
            if(((i-1) >= 0) && current->children[i-1]
                && ((current->children[i-1]->numKeys) > ((m/2)-1)))
            {
                //next child's left neighbor has more than minimum number of keys
                
                //shift keys and children so that next child has one more key
                //and left neighbor has one less key
                
                BTreeNode* right = current->children[i];
                BTreeNode* left = current->children[i-1];
                for(int j = right->numKeys - 1; j >= 0; j--)
                {
                    right->keywords[j+1] = right->keywords[j];
                    right->accessCounts[j+1] = right->accessCounts[j];
                }
                right->numKeys++;
                right->keywords[0] = current->keywords[i-1];
                right->accessCounts[0] = current->accessCounts[i-1];
                
                if(!(right->isLeaf))
                {
                    for(int j = right->numChildren-1; j >= 0; j--)
                    {
                        right->children[j+1] = right->children[j];
                    }
                    
                    right->children[0] = left->children[left->numChildren-1];
                    right->numChildren++;
                    left->children[left->numChildren - 1] = NULL;
                    left->numChildren--;
                }
                
                current->keywords[i-1] = left->keywords[left->numKeys-1];
                current->accessCounts[i-1] = left->accessCounts[left->numKeys-1];
                
                left->keywords[left->numKeys-1] = "";
                left->accessCounts[left->numKeys-1] = 0;
                left->numKeys--;
                
            }
            else if(((i+1) < (current->numChildren)) && current->children[i+1]
                    && (current->children[i+1]->numKeys > ((m/2)-1)))
            {
                //next child's right neighbor has more than minimum number of keys
                
                //shift keys and children so that next child has one more key
                //and right neighbor has one less key
                
                BTreeNode* right = current->children[i+1];
                BTreeNode* left = current->children[i];
                
                left->keywords[left->numKeys] = current->keywords[i];
                left->accessCounts[left->numKeys] = current->accessCounts[i];
                left->numKeys++;
                
                if(!(right->isLeaf))
                {
                    left->children[left->numChildren] = right->children[0];
                    left->numChildren++;
                }
                
                current->keywords[i] = right->keywords[0];
                current->accessCounts[i] = right->accessCounts[0];
                
                int j = 0;
                for(j = 0; j < (right->numKeys-1); j++)
                {
                    right->keywords[j] = right->keywords[j+1];
                    right->accessCounts[j] = right->accessCounts[j+1];
                }
                right->keywords[j] = "";
                right->accessCounts[j] = 0;
                right->numKeys--;
                
                if(!(right->isLeaf))
                {
                    j = 0;
                    for(j = 0; j < right->numChildren-1; j++)
                    {
                        right->children[j] = right->children[j+1];
                    }
                    right->children[j] = NULL;
                    right->numChildren--;
                }
            }
            else if((i < (current->numChildren-1)) && current->children[i]
                    && current->children[i+1])
            {
                //merge next child and next child's right neighbor because both
                //have minimum number of keys
                merge(current, i);
            }
            else if((i == (current->numChildren-1)) && current->children[i]
                    && current->children[i-1] && (i-1 >= 0))
            {
                //next child is the last child of current
                //merging has the index set to one before i
                merge(current, i-1);
                i--;
            }
        }
        remove(current->children[i], rStr); //recursive call to next child
    }
    
} //END DictBTree member function remove