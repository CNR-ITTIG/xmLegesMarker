#ifndef __SPARSEMATRIX_H
#define __SPARSEMATRIX_H
#include <values.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <hash_map.h>
#include <cmath>

using namespace std;

template<class DATATYPE>
class SparseMatrix
{
 
 public:
    
  // hash functions
  struct Key{
    int r;
    int c;
    Key(int ro = 0 , int co = 0){
      r = ro;
      c = co;
    }
  };
  
  struct HashFunc
  {
    hash<int> H;
    size_t operator () (const Key &key) const
    {
      return H(H(key.r) - key.c);
    }
  };
  struct EqualKey
  {
    bool operator () (const Key &op1, const Key &op2) const
    {
      return (op1.r == op2.r) && (op1.c == op2.c); 
    }
  };
  typedef hash_map<Key,DATATYPE,HashFunc,EqualKey> SparseMap;
  typedef SparseMap::const_iterator SparseMapConstIterator;
  typedef SparseMap::iterator SparseMapIterator;
  SparseMap elements_map_;

 public:

  // constructors

  SparseMatrix();
  SparseMatrix(const SparseMatrix<DATATYPE> &src);
  SparseMatrix(istream& is);
  virtual ~SparseMatrix();

  // initialization
  void Clear();
  SparseMatrix<DATATYPE>& operator = (const SparseMatrix<DATATYPE> &src);
		
  // Get/Set
	
  operator DATATYPE() const; // cast to scalar
  DATATYPE operator () (int r, int c) const;
  DATATYPE& operator () (int r, int c);
  DATATYPE operator () (Key k) const;
  DATATYPE& operator () (Key k);
  hash_map<int,DATATYPE> getRow(int r) const;
  hash_map<int,DATATYPE> getCol(int c) const;
  unsigned int size() const {return elements_map_.size();};

  // normalization

  SparseMatrix<DATATYPE> NormR(SparseMatrix<DATATYPE>& norm) const;
  SparseMatrix<DATATYPE> NormC(SparseMatrix<DATATYPE>& norm) const;
  SparseMatrix<DATATYPE> NormR() const;
  SparseMatrix<DATATYPE> NormC() const;
  SparseMatrix<DATATYPE> Norm(double* norm) const;
  SparseMatrix<DATATYPE> Norm() const;
  SparseMatrix<DATATYPE> getNormR() const;
  SparseMatrix<DATATYPE> getNormC() const;

  // arithmetic operations
  
  SparseMatrix<DATATYPE>& operator +=(const SparseMatrix<DATATYPE>& op2); // WARNING: only *this decides nonzero values  
  SparseMatrix<DATATYPE> operator /(const SparseMatrix<DATATYPE>& op2) const;
  SparseMatrix<DATATYPE> operator *(const SparseMatrix<DATATYPE>& op2) const;
  SparseMatrix<DATATYPE> operator +(const SparseMatrix<DATATYPE>& op2) const;
  SparseMatrix<DATATYPE> operator -(const SparseMatrix<DATATYPE>& op2) const;
  SparseMatrix<DATATYPE> operator /(const DATATYPE& op2) const;
  DATATYPE absSum() const;
  

  // serialization
  
  void Read(istream& is=cin);
  void Write(ostream& os=cout) const;

};

template<class DATATYPE>
SparseMatrix<DATATYPE>::SparseMatrix()
{
}

template<class DATATYPE>
SparseMatrix<DATATYPE>::SparseMatrix(const SparseMatrix<DATATYPE> &src)
{
  *this = src;
}

template<class DATATYPE>
SparseMatrix<DATATYPE>::SparseMatrix(istream& is)
{
  Read(is);
}

template<class DATATYPE>
SparseMatrix<DATATYPE>::~SparseMatrix()
{
}

template<class DATATYPE>
SparseMatrix<DATATYPE>& SparseMatrix<DATATYPE>::operator=(const SparseMatrix<DATATYPE> &src)
{
  elements_map_ = src.elements_map_;
  return *this;
}


template<class DATATYPE>
DATATYPE SparseMatrix<DATATYPE>::operator()(int r, int c) const
{
  SparseMap::const_iterator i = elements_map_.find(Key(r,c));
  if (i == elements_map_.end())
    return 0;
  return i->second;
}

template<class DATATYPE>
DATATYPE SparseMatrix<DATATYPE>::operator()(Key k) const
{
  SparseMap::const_iterator i = elements_map_.find(k);
  if (i == elements_map_.end())
    return 0;
  return i->second;
}

template<class DATATYPE>
DATATYPE& SparseMatrix<DATATYPE>::operator()(int r, int c)
{
  Key k(r,c);
  SparseMap::iterator i = elements_map_.find(k);
  if (i == elements_map_.end())
    return ((elements_map_.insert(make_pair(k,0))).first)->second;
  return i->second;
}

template<class DATATYPE>
DATATYPE& SparseMatrix<DATATYPE>::operator()(Key k)
{
  SparseMap::iterator i = elements_map_.find(k);
  if (i == elements_map_.end())
    return ((elements_map_.insert(make_pair(k,0))).first)->second;
  return i->second;
}


template<class DATATYPE>
SparseMatrix<DATATYPE>::operator DATATYPE() const 
{ 
  return (*this)(Key(0,0)); 
}

template<class DATATYPE>
void SparseMatrix<DATATYPE>::Clear()
{
  elements_map_.clear();
}


template<class DATATYPE>
void SparseMatrix<DATATYPE>::Read(istream& is)
{
  // clear matrix
  Clear();
  // get elements until EOF or empty line
  string line;
  int lines=0;
  while(getline(is,line) && line != ""){
    int r,c;
    DATATYPE value;
    istringstream iss(line);
    iss >> r;
    iss >> c;
    iss >> value;
    lines++;
    (*this)(r,c) = value;
  }
}

template<class DATATYPE>
void SparseMatrix<DATATYPE>::Write(ostream &os) const {
  for(SparseMap::const_iterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    os << (k->first).r << " " << (k->first).c << " " << k->second << "\n";
  os << endl;
}

template<class DATATYPE>
hash_map<int,DATATYPE> SparseMatrix<DATATYPE>::getRow(int r) const
{
  hash_map<int,DATATYPE> row;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    if((k->first).r == r)
      row.insert(make_pair((k->first).c, k->second));
  return row;
}

template<class DATATYPE>
hash_map<int,DATATYPE> SparseMatrix<DATATYPE>::getCol(int c) const
{
  hash_map<int,DATATYPE> col;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    if((k->first).c == c)
      col.insert(make_pair((k->first).r, k->second));
  return col;
}

template<class DATATYPE>
SparseMatrix<DATATYPE> SparseMatrix<DATATYPE>::NormR() const
{
  SparseMatrix<DATATYPE> normed;
  hash_map<int,DATATYPE> norm;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    norm[(k->first).r] += k->second;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    normed(k->first) = k->second / norm[(k->first).r];
  return normed;
}

template<class DATATYPE>
SparseMatrix<DATATYPE> SparseMatrix<DATATYPE>::NormC() const
{
  SparseMatrix<DATATYPE> normed;
  hash_map<int,DATATYPE> norm;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    norm[(k->first).c] += k->second;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    normed(k->first) = k->second / norm[(k->first).c];
  return normed;
}

template<class DATATYPE>
SparseMatrix<DATATYPE> SparseMatrix<DATATYPE>::NormR(SparseMatrix<DATATYPE>& norm) const
{
  SparseMatrix<DATATYPE> normed;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    norm((k->first).r,0) += k->second;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    normed(k->first) = k->second / norm((k->first).r,0);
  return normed;
}

template<class DATATYPE>
SparseMatrix<DATATYPE> SparseMatrix<DATATYPE>::NormC(SparseMatrix<DATATYPE>& norm) const
{
  SparseMatrix<DATATYPE> normed;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    norm(0,(k->first).c) += k->second;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    normed(k->first) = k->second / norm(0,(k->first).c);
  return normed;
}

template<class DATATYPE>
SparseMatrix<DATATYPE> SparseMatrix<DATATYPE>::Norm(double* norm) const
{
  *norm = 0.;
  SparseMatrix<DATATYPE> normed;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    *norm += k->second;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    normed(k->first) = k->second / *norm;
  return normed;
}

template<class DATATYPE>
SparseMatrix<DATATYPE> SparseMatrix<DATATYPE>::Norm() const
{
  double norm = 0;
  SparseMatrix<DATATYPE> normed;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    norm += k->second;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    normed(k->first) = k->second / norm;
  return normed;
}

template<class DATATYPE>
SparseMatrix<DATATYPE> SparseMatrix<DATATYPE>::getNormR() const
{
  SparseMatrix<DATATYPE> norm;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    norm((k->first).r,0) += k->second;
  return norm;
}

template<class DATATYPE>
SparseMatrix<DATATYPE> SparseMatrix<DATATYPE>::getNormC() const
{
  SparseMatrix<DATATYPE> norm;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    norm(0,(k->first).c) += k->second;
  return norm;
}

template<class DATATYPE>
SparseMatrix<DATATYPE>& SparseMatrix<DATATYPE>::operator +=(const SparseMatrix<DATATYPE>& op2)
{
  for(SparseMapIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    k->second += op2(k->first);
  return (*this);
}

template<class DATATYPE>
SparseMatrix<DATATYPE> SparseMatrix<DATATYPE>::operator /(const SparseMatrix<DATATYPE>& op2) const
{
  SparseMatrix<DATATYPE> res;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    res(k->first) = k->second / op2(k->first);
  return res;
}

template<class DATATYPE>
SparseMatrix<DATATYPE> SparseMatrix<DATATYPE>::operator -(const SparseMatrix<DATATYPE>& op2) const
{
  SparseMatrix<DATATYPE> res;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    res(k->first) = k->second - op2(k->first);
  return res;
}

template<class DATATYPE>
SparseMatrix<DATATYPE> SparseMatrix<DATATYPE>::operator +(const SparseMatrix<DATATYPE>& op2) const
{
  SparseMatrix<DATATYPE> res;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    res(k->first) = k->second + op2(k->first);
  return res;
}

template<class DATATYPE>
SparseMatrix<DATATYPE> SparseMatrix<DATATYPE>::operator *(const SparseMatrix<DATATYPE>& op2) const
{
  SparseMatrix<DATATYPE> res;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    res(k->first) = k->second * op2(k->first);
  return res;
}


template<class DATATYPE>
SparseMatrix<DATATYPE> SparseMatrix<DATATYPE>::operator /(const DATATYPE& op2) const
{
  SparseMatrix<DATATYPE> res;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    res(k->first) = k->second / op2;
  return res;
}

template<class DATATYPE>
DATATYPE SparseMatrix<DATATYPE>::absSum() const
{
  DATATYPE absSum;
  for(SparseMapConstIterator k = elements_map_.begin(); k != elements_map_.end(); k++)
    absSum += fabs(k->second);
  return absSum;
}

#endif



