#ifndef __STRINGTABLE_H
#define __STRINGTABLE_H

#include "Record.h"

class StringTable {
public:
  StringTable(int maxSize, int matchLength);

  bool insert(Record *r);
  
  void remove(Record *r);
  
  Record *find(const char *key);

  void fill();

  void repopulate();
  
  void printTable();

  int toHashValue(int s);

  int m;
  
  Record **table = new Record*[2];

  int intRec = 0;

  double minLoadFactor = .25;

  double loadFactor = 0;

  int powerOfListSize = 1;

private:
  int matchLength;
  int toHashKey(const char *s) const;
  int baseHash(int hashKey) const;
  int stepHash(int hashKey) const;

  // Utility function for fast computation of n mod m
  // NB: Assumes m is a power of 2
  int modPow2(int n, int m) const {
    return (n & (m-1));
  }
  // Constants that may be helpful for implementing your hash functions
  
  // (sqrt(5)-1)/2, or Golden Ratio - 1 (recommended by Knuth)
  static constexpr double KNUTHVAL   = 0.6180339887498948482045868343656;
  static constexpr double PI         = 3.1415926535897932384626433832795;
};

#endif
