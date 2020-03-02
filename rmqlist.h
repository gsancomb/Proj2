// CMSC 341 - Spring 2020 - Project 2
#ifndef _RMQLIST_H
#define _RMQLIST_H

#include <iostream>
#include <stdexcept>
#include <cmath>

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
  int _listSize;

  // **********************************
  // Private variables for RMQ go here!
  // **********************************

  // *******************************************
  // Declarations for private functions go here!
  // *******************************************
};


template <class K, class V>
RMQList<K,V>::RMQList() {
}

template <class K, class V>
RMQList<K,V>::~RMQList() {
}

template <class K, class V>
RMQList<K,V>::RMQList(const RMQList<K,V> &rhs) {
}

template <class K, class V>
const RMQList<K,V>& RMQList<K,V>::operator=(const RMQList<K,V> &rhs) {
}

template <class K, class V>
bool RMQList<K,V>::insert(const K& key, const V& value) {
}

template <class K, class V>
bool RMQList<K,V>::remove(const K& key) {
}

template <class K, class V>
bool RMQList<K,V>::update(const K& key, const V& value) {
}

template <class K, class V>
V RMQList<K,V>::query(const K& k1, const K& k2) {
}

template <class K, class V>
void RMQList<K,V>::dumpList() const {
}

template <class K, class V>
void RMQList<K,V>::dumpTable() const {
}

template <class K, class V>
void RMQList<K,V>::clear() {
}

#endif
