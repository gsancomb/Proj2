// CMSC 341 - Spring 2020 - Project 2
#ifndef _RMQLIST_H
#define _RMQLIST_H

#include <iostream>
#include <stdexcept>
#include <cmath>

using namespace std;
using std::swap;
using std::ostream;
using std::cout;
using std::endl;
using std::sqrt;
using std::range_error;
using std::invalid_argument;

// Macro for a two-argument min function
#define MIN(a, b)  ((a) < (b) ? (a) : (b))


// forward declarations
template <class K, class V> class RMQList;
template <class K, class V> class Node;
template <class K, class V> ostream& operator<<(ostream &sout, const Node<K,V> &x);

// *********************************************
// Node - node class for the RMQList linked list
//   key and value are templated (types K and V)
// *********************************************

//class KeyVal {
//public:
//    KeyVal() { _data = pair<int,int>(0,0); }
//    KeyVal(int key, int val) { _data = pair<int,int>(key, val); }
//    int getKey() const { return _data.first; }
//    int getVal() const { return _data.second; }
//    void setKey(int){}
//private:
//    pair<int,int> _data;
//};

// Function prototypes
//bool operator<(const KeyVal& kv1, const KeyVal& kv2) { return kv1.getKey() < kv2.getKey(); }
//int minimum(const vector<KeyVal> &data, int first, int last);
//int bsearch(const vector<KeyVal> &A, int key);

template <class K, class V>
class Node {
  friend RMQList<K, V>;
public:
  Node(K key = K(), V value = V(), Node<K, V> *next = nullptr) {
    _key = key;
    _value = value;
    _next = next;
  }

  friend ostream& operator<< <K,V>(ostream &sout, const Node<K,V> &x);
private:
  K _key;
  V _value;
  Node *_next;
};

// Overloaded insertion operator for Node
template <class K, class V>
ostream& operator<<(ostream &sout, const Node<K,V> &x) {
  sout << "Key: " << x._key << ", Value: " << x._value;
  return sout;
}

// *******************************************************
// RMQList - list container (linked list) with RMQ support
//   key and value are templated (types K and V)
// *******************************************************

template <class K, class V>
class RMQList {
public:
  // Create an empty RMQList object
  RMQList();
  
  // Destructor, Copy Constructor, Assignment Operator
  ~RMQList();
  RMQList(const RMQList<K,V> &rhs);
  const RMQList<K,V>& operator=(const RMQList<K,V> &rhs);

  // In-line function. Returns the size (number of elements).
  int size() const { return _listSize; }

  // In-line function. Returns true if the list is empty; false
  // otherwise.
  bool empty() const { return _head == nullptr; }
  
  // Insert an element into the list; list must be kept in increasing
  // order by key; duplicate keys are not allowed, so return false if
  // there is already an entry with the specified key, true otherwise.
  // Should check if key > largest current key and, if so, append to
  // the end of the list without iteration.
  bool insert(const K& key, const V& value);

  // Remove an element from the list; return false if no element
  // exists with the specified key value, true otherwise
  bool remove(const K& key);

  // Update value for the element with given key; return false if
  // there is no element with the given key, true otherwise
  bool update(const K& key, const V& value);
  
  // RMQ Query for k1 to k2.  Throws exceptions (see documentation).
  V query(const K& k1, const K& k2);

  // Dump the list entries
  void dumpList() const;
  
  // Dump the RMQ info and table.  What gets dumped depends on which
  // RMQ method is used.
  void dumpTable() const;

  // Clear the data data strucures
  void clear();



 private:
  Node<K,V> *_head;
  Node<K,V> *_tail;
  int _listSize{};

  // **********************************
  // Private variables for RMQ go here!
  // **********************************
    int rmqNumBlocks = 0;
    int rmqBlockSize = 0;
    V *rmqBlockMin;
    pair<K,V> *rmqBlockKey;
  // *******************************************
  // Declarations for private functions go here!
  // *******************************************
    void blockBuild ();
//    V minimum(const pair<K,V> *, K, K);
    int bsearch(const pair<K,V> *, K );
};

template <class K, class V>
void RMQList<K,V>::blockBuild(){
    Node<K,V> *currNode = _head;
    rmqBlockSize = (int) sqrt((float) _listSize);
    rmqNumBlocks = _listSize / rmqBlockSize;
    rmqBlockKey = new  pair<K,V> [_listSize];
    rmqBlockMin = new  V [rmqNumBlocks];

    int minindex = 0;
    int minValNum = 0;
    K minKey = currNode->_key;
    int minKeyIndex = 0;
    for (int i = 0; i < _listSize; i++){
        auto *keyPair = new pair<K,V>(currNode->_key, currNode->_value);
        rmqBlockKey[i] = *keyPair;
        currNode = currNode ->_next;


        if (minindex == 0 ){
            minKeyIndex = i;
            rmqBlockMin[minValNum] = rmqBlockKey[i].second;
            minindex++;
        }
        else {
            if (minindex >= (rmqBlockSize-1)){
                if (rmqBlockKey[minKeyIndex].second > rmqBlockKey[i].second ) {
                    rmqBlockMin[minValNum] = rmqBlockKey[i].second;
                    minKeyIndex = i;
                }
                minindex = 0;
                minValNum++;
            }
            else {
                if (rmqBlockKey[minKeyIndex].second > rmqBlockKey[i].second) {
                    rmqBlockMin[minValNum] = rmqBlockKey[i].second;
                    minKeyIndex = i;
                }
                minindex++;
            }
        }
    }
}

template <class K, class V>
RMQList<K,V>::RMQList() {
    _head = nullptr;
    _tail = _head;
    _listSize = 0;
}

template <class K, class V>
RMQList<K,V>::~RMQList() {
   clear();

}

template <class K, class V>
RMQList<K,V>::RMQList(const RMQList<K,V> &rhs) {
    for (Node<K, V> *currNode = rhs._head; currNode != nullptr; currNode = currNode->_next) {
        insert(currNode->getKey(), currNode->getValue());
    }
}

template <class K, class V>
const RMQList<K,V>& RMQList<K,V>::operator=(const RMQList<K,V> &rhs) {
    for (Node<K, V> *currNode = rhs._head; currNode != nullptr; currNode = currNode->_next) {
        insert(currNode->_key, currNode->_value);
    }
}

template <class K, class V>
bool RMQList<K,V>::insert(const K& key, const V& value) {
//    cout << "insert function \n";
    Node<K,V> *lastNode = _head;
    if (_listSize == 0){
//        cout << "Add to front \n";
        auto *newNode = new Node<K,V>(key, value);
        _head = newNode;
        _tail = newNode;
        _listSize++;
    }
    else {
        for (Node<K, V> *currNode = _head; currNode != nullptr; currNode = currNode->_next) {
//        cout << "loop-y-loop \n";
            //ends function if key exists in the list
            if (key == currNode->_key) {
//                cout << "not Added copy of key in list\n";
                return false;
            }
                //if the key is less then the node then it inserts the node to the list
            else if (key < currNode->_key) {
                //inserts the node to the start if last and current are the same meaning its currNode is at _head
                if (_head == currNode) {
//                    cout << "Add to front \n";
                    auto *newNode = new Node<K, V>(key, value);
                    _head = newNode;
                    newNode->_next = currNode;
                    _listSize++;
                    return true;
                }
                    //add the the node to the middle of the list
                else {
//                    cout << "Add to middle \n";
                    auto *newNode = new Node<K, V>(key, value);
                    lastNode->_next = newNode;
                    newNode->_next = currNode;
                    _listSize++;
                    return true;

                }
            }
                //if the iteration makes it to the end then it inserts the the node to the end of the list.
            else if (currNode == _tail) {
//                cout << "Add to back \n";
                auto *newNode = new Node<K, V>(key, value);
                _tail = newNode;
                currNode->_next = newNode;
                newNode->_next = nullptr;
                _listSize++;
                return true;
            }
            lastNode = currNode;
        }
    }
    return false;
}

template <class K, class V>
bool RMQList<K,V>::remove(const K& key) {
//    cout << "remove \n";
    Node<K,V> *lastNode = _head;
    if (_listSize != 0) {
        for (Node<K, V> *currNode = _head; currNode != nullptr; currNode = currNode->_next) {
//            cout << "loopy-loop\n";
//            cout << currNode->_key << "  " << key << endl;
            if (currNode->_key == key) {
//                cout << "remove if state\n";

                //remove start
                if (_head == currNode) {
//                    cout << "remove start\n";
                    _head = currNode->_next;
                    delete currNode;
                    currNode = nullptr;
                    _listSize--;
                    return true;
                }
                //remove end
                else if (currNode->_next == nullptr) {
//                    cout << "remove end\n";
                    _tail = lastNode;
                    lastNode->_next = nullptr;
                    delete currNode;
                    currNode = nullptr;
                    _listSize--;
                    return true;
                }
                    //remove middle
                else {
//                    cout << "remove middle\n";
                    lastNode->_next = currNode->_next;
                    delete currNode;
                    currNode = nullptr;
                    _listSize--;
                    return true;
                }
            }
            lastNode = currNode;
        }
    }
    return false;
}

template <class K, class V>
bool RMQList<K,V>::update(const K& key, const V& value) {
    for (Node<K, V> *currNode = _head; currNode != nullptr; currNode = currNode->_next) {
        if (currNode->_key == key){
            currNode->_value = value;
        }
    }
    return false;
}
//todo query
template <class K, class V>
V RMQList<K,V>::query(const K& k1, const K& k2) {
    if (_listSize == 0) {
        throw range_error("The List is empty");
    } else {
        blockBuild();

        int index_K1 = bsearch(rmqBlockKey, k1);
        if (index_K1 < 0)
            throw invalid_argument("Key 1 not in list");

        int index_K2 = bsearch(rmqBlockKey, k2);
        if (index_K2 < 0)
            throw invalid_argument("Key 2 not in list");

        int indexDif = abs(index_K1 - index_K2);
        int lowestIndex = 0;

        if (index_K1 < index_K2) { lowestIndex = index_K1; }
        else { lowestIndex = index_K2; }

        V minValue = rmqBlockKey[lowestIndex].second;
        for (int i = lowestIndex; i <= lowestIndex + indexDif; i++) {
            if (i % rmqBlockSize == 0 and (i + rmqBlockSize) < (lowestIndex + indexDif)) {
                if (minValue > rmqBlockMin[i / rmqBlockSize]) {
                    minValue = rmqBlockMin[i / rmqBlockSize];
                }
                i += rmqBlockSize;
            } else {
                if (minValue > rmqBlockKey[i].second) {
                    minValue = rmqBlockKey[i].second;
                }
            }
        }
        return minValue;
    }
}


template <class K, class V>
void RMQList<K,V>::dumpList() const {
//    cout << "dumplist\n";
    Node<K,V> *currNode = _head;
    for (int i = 0; i < _listSize; i++){
//        cout << "loopy-loop\n";
        cout << "Key: " << currNode->_key << " Value: " << currNode->_value << endl;
        currNode = currNode->_next;
    }
}
template <class K, class V>
void RMQList<K,V>::dumpTable() const {
    cout << "Num blocks: " << rmqNumBlocks <<endl;
    cout << "Block size: " << rmqBlockSize<<endl;
    cout << "Block mins:\n";
    for (int i = 0; i < rmqNumBlocks; i ++){
        cout << rmqBlockMin[i]<<"  " ;

    }
    cout << endl;
//    8  1  0  2
}

template <class K, class V>
void RMQList<K,V>::clear() {
    for (int i = 0; i < _listSize; i ++) {
        remove(_head->_next->_key);
    }
    _head = nullptr;
    _tail = nullptr;
    delete [] rmqBlockKey;
    delete [] rmqBlockMin;
    _listSize = 0;
    rmqNumBlocks = 0;
    rmqBlockSize = 0;
}

//template <class K, class V>
//V RMQList<K, V>::minimum(const pair<K,V> * data, K key1, K key2) {
//    int indx1 = bsearch(data, key1);
//    int indx2 = bsearch(data, key2);
//    if (indx1 > indx2) swap(indx1, indx2);
//
//    V minVal = data[indx1].second;
//    for (int i = indx1+1; i <= indx2; i++) {
//        minVal = ( data[i].second < minVal ? data[i].second : minVal );
//    }
//    return minVal;
//}

template<class K, class V>
int RMQList<K, V>::bsearch(const pair<K,V> *A, K key) {
    int n = _listSize;
    int p = 0, r = n - 1;
    int q;

    while (p != r) {
        q = (p + r) / 2;
        if (key <= A[q].first) {
            r = q;
        } else {
            p = q + 1;
        }
    }

    if (key == A[p].first) {
        return p;
    } else {
        return -p;
    }
}
//// dont use for any thing other then testing RMQ
//template <class K, class V>
//int RMQList<K,V>::bruteForce() {
//    K minValue = _head->_key;
//    Node<K,V> *currNode = _head;
//    for (int i = 0; i < _listSize; i ++){
//        if (minValue > currNode->_key){
//            minValue = currNode->_key;
//        }
//        currNode = currNode->_next;
//    }
//    return minValue;
//}



//V RMQList<K,V>::minimum( r data, K key1, K key2) {
//    int indx1 = bsearch(data, key1);
//    int indx2 = bsearch(data, key2);
//    if (indx1 > indx2) swap(indx1, indx2);
//
//    int minVal = data[indx1].getVal();
//    for (int i = indx1+1; i <= indx2; i++) {
//        minVal = ( data[i].getVal() < minVal ? data[i].getVal() : minVal );
//    }
//    return minVal;
//}

//int RMQList<K,V>::bsearch(const pair<K,V>&  A, K key) {
//    int n = A.size();
//    int p = 0, r = n - 1;
//    int q;
//
//    while (p != r) {
//        q = (p+r)/2;
//        if (key <= A[q].getKey()) {
//            r = q;
//        } else {
//            p = q+1;
//        }
//    }
//
//    if ( key == A[p].getKey() ) {
//        return p;
//    } else {
//        return -p;
//    }
//}
#endif