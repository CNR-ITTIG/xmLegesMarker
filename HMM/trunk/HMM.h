
#ifndef HMM_H
#define HMM_H

#include <hash_map.h>
#include <iostream>
#include <vector>
#include "SparseMatrix.h"
#include <getopt.h>

class HMM{

  struct Element{
    float value;
    float logvalue;
    Element(float value_ = 0.){
      value = value_;
    }
    Element(const HMM::Element& el){
      value = el.value;
      logvalue = el.logvalue;
    }
    friend std::istream & operator>>(std::istream &in, Element& el){
      in >> el.value >> el.logvalue;
      return in;
    }
    friend std::ostream & operator<<(std::ostream &out, const Element& el){
      out << el.value << " " << el.logvalue;
      return out;
    }
    Element& operator +=(const HMM::Element& op2){
      Element res(*this);
      res.value += op2.value;
    }
    Element& operator /(const HMM::Element& op2){
      Element res(*this);
      res.value /= op2.value;
    }
    operator float() const{
      return this->value;
    }
    void setLog(){
      logvalue = log(value);
    }
  };

  SparseMatrix<Element> A,B,S;
  bool isLeftRightModel;
  int endState;

 public:

  double viterbiPath(int * sequence, int * states, int T) const;
  double viterbiPath(const vector<int>& sequence, int * states, int T) const;
  double sequenceLogLikelihood(int * sequence, int T) const;
  double sequenceLogLikelihood(const vector<int>& sequence, int T) const;
  void train(std::vector<int> ** data, int numdata, double tolerance = 1e-3);
  void test(std::vector<int> ** data, int numdata) const;
  friend std::ostream & operator<<(std::ostream &out, const HMM& hmm);
  friend std::istream & operator>>(std::istream &in, HMM& hmm);
  std::vector<int> ** loadData(std::istream& in, int& numdata) const;

 protected:

  void viterbiPath(int symbol, SparseMatrix<int>& psi, hash_map<int,float>& delta, int t) const;
  SparseMatrix<double> computeAlpha(int t, int * sequence, double * norm) const;
  SparseMatrix<double> computeBeta(int t, int * sequence, double * norm, int T) const;
  SparseMatrix<double> computeAlpha(int t, const vector<int>& sequence, double * norm) const;
  SparseMatrix<double> computeBeta(int t, const vector<int>& sequence, double * norm, int T) const;
  bool EM(std::vector<int> ** data, int numdata, double tolerance = 1e-3);
  void computeLogs(SparseMatrix<Element>& matrix);
};

#ifdef STANDALONEHMM
class CommandLineOption{

private:
  static const option optionNames[];

 public:
  const char * trainfile;
  const char * testfile;
  const char * modelfile;
  double epsilon;

  CommandLineOption(int argc = 0, char *argv[]=NULL);
  void usage(char* progname);
};
#endif

#endif

