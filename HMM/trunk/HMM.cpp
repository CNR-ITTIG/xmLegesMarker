#include "HMM.h"
#include <cassert>
#include <cmath>
#include <fstream>

using namespace std;

void HMM::train(vector<int> ** data, int numdata, double tolerance)
{
  int iteration = 0;
  while(EM(data, numdata, tolerance))
    cout << "EM iteration " << iteration++ << endl;
  cout << "Finished training" << endl; 
  computeLogs(A);
  computeLogs(B);
  computeLogs(S);
  
}

void HMM::test(vector<int> ** data, int numdata) const
{ 
  for(int i = 0; i < numdata; i++){
    int states[data[i]->size()];
    cout << exp(viterbiPath(*(data[i]), states, data[i]->size())) << " = ";
    for(int t = 0; t < data[i]->size(); t++)
      cout << states[t] << " ";
    cout << endl;
  }
}

bool HMM::EM(vector<int> ** data, int numdata, double tolerance)
{

  //DEBUG
  cout << "A\n";
  A.Write(cout);
  cout << "B\n";
  B.Write(cout);
  cout << "S\n";  
  S.Write(cout);
  cout << "-------\n";
  //DEBUG

  SparseMatrix<Element> numA,numB,denA,newS;
  SparseMatrix<double> alpha,beta;
  for(int k = 0; k < numdata; k++){
    SparseMatrix<Element> currAnum,currBnum,currAden;
    double norm[data[k]->size()-1];
    for(int t = data[k]->size()-2; t >= 0; t--){
      alpha = computeAlpha(t, *(data[k]), norm);
      beta = computeBeta(t+1, *(data[k]), norm, data[k]->size());
      //DEBUG
      cout << "k=" << k << " t=" << t << " alpha" << endl;
      alpha.Write(cout);
      cout << "k=" << k << " t=" << t << " beta" << endl;
      beta.Write(cout);
      //DEBUG
      for(SparseMatrix<Element>::SparseMapConstIterator h = A.elements_map_.begin();  h != A.elements_map_.end(); h++){
	currAnum((h->first).r, (h->first).c) += alpha((h->first).r, 0) * h->second * B((h->first).c, (*(data[k]))[t+1]) * 
	  (t == data[k]->size()-2) ? 1 : beta((h->first).c, 0);
	double den = 0.;
	for(SparseMatrix<double>::SparseMapConstIterator j = beta.elements_map_.begin(); j != beta.elements_map_.end(); j++)
	  den += alpha((h->first).r, 0) * A((h->first).r, (j->first).r) * B((j->first).r, (*(data[k]))[t+1]) * 
	    (t == data[k]->size()-2) ? 1 : beta(j->first);
	currAden((h->first).r, (h->first).c) += den;
	currBnum((h->first).r,(*(data[k]))[t]) += den;
      }
      // update initial state probabilities
      if(t==0){
	SparseMatrix<Element> currS;
	for(SparseMatrix<Element>::SparseMapConstIterator h = S.elements_map_.begin();  h != S.elements_map_.end(); h++)
	  for(SparseMatrix<double>::SparseMapConstIterator j = beta.elements_map_.begin(); j != beta.elements_map_.end(); j++)
	    currS((h->first).r, 0) += alpha((h->first).r, 0) * A((h->first).r, (j->first).r) * B((j->first).r, (*(data[k]))[t+1]) * beta(j->first);
	newS += currS.Norm();
      }
    }
    double Pk;
    numA += currAnum.Norm(&Pk);
     numB += currBnum / Pk;
    denA += currAden / Pk;
  }

  numA = numA / denA;
  numB = numB / denA;
  newS = newS.Norm();

  //DEBUG
  cout << "A\n";
  A.Write(cout);
  cout << "B\n";
  B.Write(cout);
  cout << "S\n";  
  S.Write(cout);
  cout << "-------\n";

  cout << "numA\n";
  numA.Write(cout);
  cout << "numB\n";
  numB.Write(cout);
  cout << "newS\n";  
  newS.Write(cout);
  cout << "-------\n";
  //DEBUG
  
  double difference = (float)((A - numA).absSum()) / (float)A.size() + (float)((B - numB).absSum()) / (float)B.size() 
    + (float)((S - newS).absSum()) / (float)S.size();
  A = numA;
  B = numB;
  S = newS;

  //DEBUG
  cout << "A\n";
  A.Write(cout);
  cout << "B\n";
  B.Write(cout);
  cout << "S\n";  
  S.Write(cout);
  cout << "-------\n";
  cout << "difference=" << difference << endl;
  //DEBUG

  if (difference > tolerance)
    return true;
  return false;
}

void HMM::computeLogs(SparseMatrix<Element>& matrix)
{  
  for(SparseMatrix<Element>::SparseMapIterator k = matrix.elements_map_.begin();  k != matrix.elements_map_.end(); k++)
    (k->second).setLog();
}

SparseMatrix<double> HMM::computeAlpha(int t, int * sequence, double * norm) const{

  SparseMatrix<double> alpha;

  // initialization
  if (t == 0){
    for(SparseMatrix<Element>::SparseMapConstIterator k = S.elements_map_.begin();  k != S.elements_map_.end(); k++)
      alpha((k->first).r,0) = (k->second).value * B((k->first).r,sequence[t]).value;
  }
  // induction
  else{
    hash_map<int,Element> col = B.getCol(sequence[t]);
    SparseMatrix<double> old_alpha = computeAlpha(t-1, sequence, norm);
    for(hash_map<int,Element>::iterator h = col.begin(); h != col.end(); h++){    
      double past = 0.;
      hash_map<int,Element> col2 = A.getCol(h->first);
      for(hash_map<int,Element>::const_iterator k = col2.begin(); k != col2.end(); k++)
	past += (k->second).value * old_alpha(k->first,0);
      alpha(h->first,0) = (h->second).value * past;
    }  
  }
  // normalization
  return alpha.Norm(&norm[t]);
}

SparseMatrix<double> HMM::computeBeta(int t, int * sequence, double * norm, int T) const{

  SparseMatrix<double> beta;

  // initialization (all ones, will be checked later)
  if (t == T-1) 
    return beta;

  // second step
  if (t == T-2){
    for(SparseMatrix<Element>::SparseMapConstIterator k = A.elements_map_.begin();  k != A.elements_map_.end(); k++)
      beta((k->first).r,0) += (k->second).value * (B((k->first).c,sequence[t+1])).value;
  }   
  // induction
  else{
    SparseMatrix<double> old_beta = computeBeta(t+1, sequence, norm, T);
    for(SparseMatrix<Element>::SparseMapConstIterator k = A.elements_map_.begin();  k != A.elements_map_.end(); k++)
      beta((k->first).r,0) += (k->second).value * (B((k->first).c,sequence[t+1])).value * old_beta((k->first).c,0);
  }
  // normalization
  for(SparseMatrix<double>::SparseMapIterator k = beta.elements_map_.begin();  k != beta.elements_map_.end(); k++)
    k->second *= norm[t];

  return beta;
}

SparseMatrix<double> HMM::computeAlpha(int t, const vector<int>& sequence, double * norm) const{

  SparseMatrix<double> alpha;

  // initialization
  if (t == 0){
    for(SparseMatrix<Element>::SparseMapConstIterator k = S.elements_map_.begin();  k != S.elements_map_.end(); k++)
      alpha((k->first).r,0) = (k->second).value * B((k->first).r,sequence[t]).value;
  }
  // induction
  else{
    hash_map<int,Element> col = B.getCol(sequence[t]);
    SparseMatrix<double> old_alpha = computeAlpha(t-1, sequence, norm);
    for(hash_map<int,Element>::iterator h = col.begin(); h != col.end(); h++){    
      double past = 0.;
      hash_map<int,Element> col2 = A.getCol(h->first);
      for(hash_map<int,Element>::const_iterator k = col2.begin(); k != col2.end(); k++)
	past += (k->second).value * old_alpha(k->first,0);
      alpha(h->first,0) = (h->second).value * past;
    }  
  }
  // normalization
  return alpha.Norm(&norm[t]);
}

SparseMatrix<double> HMM::computeBeta(int t, const vector<int>& sequence, double * norm, int T) const{

  SparseMatrix<double> beta;

  // initialization (all ones, will be checked later)
  if (t == T-1) 
    return beta;

  // second step
  if (t == T-2){
    for(SparseMatrix<Element>::SparseMapConstIterator k = A.elements_map_.begin();  k != A.elements_map_.end(); k++){
      beta((k->first).r,0) += (k->second).value * (B((k->first).c,sequence[t+1])).value;
      //DEBUG
      cout << "A(" << k->first.r << "," << k->first.c << ")=" << (k->second).value << "\n"
	   << "B(" << k->first.c << "," << sequence[t+1] << ")=" << (B((k->first).c,sequence[t+1])).value << "\n";
      cout << "beta(" << k->first.r << ",0)=" << beta((k->first).r,0) << endl; 
      //DEBUG
    }
  }   
  // induction
  else{
    SparseMatrix<double> old_beta = computeBeta(t+1, sequence, norm, T);
    for(SparseMatrix<Element>::SparseMapConstIterator k = A.elements_map_.begin();  k != A.elements_map_.end(); k++)
      beta((k->first).r,0) += (k->second).value * (B((k->first).c,sequence[t+1])).value * old_beta((k->first).c,0);
  }
  // normalization
  for(SparseMatrix<double>::SparseMapIterator k = beta.elements_map_.begin();  k != beta.elements_map_.end(); k++){
    k->second *= norm[t];
    //DEBUG
    cout << "norm[" << t << "]=" << norm[t] << "\n"
	 << "beta(" << k->first.r << ",0)=" << beta((k->first).r,0) << endl; 
    //DEBUG
  }

  //DEBUG
  cout << "t=" << t << " beta\n";
  beta.Write(cout);
  cout << "---\n";
  //DEBUG
  return beta;
}

double HMM::sequenceLogLikelihood(int * sequence, int T) const
{
  double logLikelihood = 0.;
  double norm[T];
  SparseMatrix<double> alpha = computeAlpha(T-1, sequence, norm);
  for(int t = 0; t < T; t++)
    logLikelihood -= log(norm[t]);
  return logLikelihood;
}

double HMM::sequenceLogLikelihood(const vector<int>& sequence, int T) const
{
  double logLikelihood = 0.;
  double norm[T];
  SparseMatrix<double> alpha = computeAlpha(T-1, sequence, norm);
  for(int t = 0; t < T; t++)
    logLikelihood -= log(norm[t]);
  return logLikelihood;
}

double HMM::viterbiPath(const vector<int>& sequence, int * states, int T) const
{
  assert(T>=1);
  // initialization
  hash_map<int,float> delta;
  SparseMatrix<int> psi;
  for(SparseMatrix<Element>::SparseMapConstIterator k = S.elements_map_.begin();  k != S.elements_map_.end(); k++){
    Element b = B((k->first).r,sequence[0]);
    if(b.value != 0)
      delta.insert(make_pair((k->first).r,(k->second).logvalue+b.logvalue));
  }
  // iteration
  for(int t = 1; t < T; t++)
    viterbiPath(sequence[t], psi, delta, t);
  // termination
  float max = -MAXFLOAT;
  if(isLeftRightModel){
    hash_map<int,float>::iterator k = delta.find(endState);
    if(k != delta.end())
      max = k->second;
    states[T-1] = endState;
  }
  else{
    for(hash_map<int, float>::iterator k = delta.begin(); k != delta.end(); k++)
      if(k->second > max){
	max = k->second;
	states[T-1] = k->first;
      }
  }
  for (int t = T - 2; t >= 0; t--)
    states[t] = psi(t+1,states[t+1]);
  return max;
}

double HMM::viterbiPath(int* sequence, int * states, int T) const
{
  assert(T>=1);
  // initialization
  hash_map<int,float> delta;
  SparseMatrix<int> psi;
  for(SparseMatrix<Element>::SparseMapConstIterator k = S.elements_map_.begin();  k != S.elements_map_.end(); k++){
    Element b = B((k->first).r,sequence[0]);
    if(b.value != 0)
      delta.insert(make_pair((k->first).r,(k->second).logvalue+b.logvalue));
  }
  // iteration
  for(int t = 1; t < T; t++)
    viterbiPath(sequence[t], psi, delta, t);
  // termination
  float max = -MAXFLOAT;
  if(isLeftRightModel){
    hash_map<int,float>::iterator k = delta.find(endState);
    if(k != delta.end())
      max = k->second;
    states[T-1] = endState;
  }
  else{
    for(hash_map<int, float>::iterator k = delta.begin(); k != delta.end(); k++)
      if(k->second > max){
	max = k->second;
	states[T-1] = k->first;
      }
  }
  for (int t = T - 2; t >= 0; t--)
    states[t] = psi(t+1,states[t+1]);
  return max;
}

ostream & operator<<(ostream &out, const HMM& hmm)
{
    hmm.A.Write(out);
    hmm.B.Write(out);
    hmm.S.Write(out);
    out << hmm.isLeftRightModel << " " << hmm.endState;
    return out;
}

istream & operator>>(std::istream &in, HMM& hmm)
{
  hmm.A.Read(in);
  hmm.B.Read(in);
  hmm.S.Read(in);
  in >> hmm.isLeftRightModel >> hmm.endState;
  return in;
}

void HMM::viterbiPath(int symbol, SparseMatrix<int>& psi, hash_map<int,float>& delta, int t) const
{
  hash_map<int,float> delta_new;
  hash_map<int,Element> col = B.getCol(symbol);
  for(hash_map<int,Element>::iterator h = col.begin(); h != col.end(); h++){    
    float max = -MAXFLOAT;
    for(hash_map<int, float>::iterator k = delta.begin(); k != delta.end(); k++){
      Element a = A(k->first,h->first);
      if((a.value != 0) && ((k->second+a.logvalue) > max)){
	max = k->second+a.logvalue;
	psi(t, h->first) = k->first;
      }
    }
    if(max != -MAXFLOAT)
      delta_new.insert(make_pair(h->first, max+(h->second).logvalue));
  }

  delta = delta_new;  
}

std::vector<int> ** HMM::loadData(std::istream& in, int & numdata) const
{
  numdata = 0;
  std::string buf;
  while(getline(in, buf))
    numdata++;
  in.seekg(0);
  in.clear();
  std::vector<int> ** data = new (vector<int>*)[numdata];

  int currdata = 0;
  while(getline(in, buf)){
    data[currdata] = new vector<int>;
    int offset = -1;
    while(true){
      int end = buf.find_first_of(" ",offset+1);
      if(end == string::npos){
	long value = atol(buf.substr(offset+1).c_str());
	data[currdata]->push_back(value);
	break;
      }
      long value = atol(buf.substr(offset+1,end-offset-1).c_str());
      offset = end;
      data[currdata]->push_back(value);
    }
    currdata++;
  }
  return data;
}


#ifdef STANDALONEHMM
const option CommandLineOption::optionNames[] =
{ 
  { "train",      required_argument,  0, 't'},
  { "test",      required_argument,  0, 'T'},
  { "model",     required_argument,  0, 'm'},
  { "epsilon", required_argument,  0, 'e'},
  { "help",       no_argument,        0, '?'},
  { 0,            0,                  0, 0  }
};
  


CommandLineOption::CommandLineOption(int argc, char *argv[]) :
  trainfile(""),
  testfile(""),
  modelfile(""),
  epsilon(1e-3)
{
  int c;
  int* opti = 0;
  
  // you must put ':' only after parameters that require an argument
  // so after 'a' and '?' we don't put ':'

  while ((c = getopt_long(argc, argv, "t:T:m:e:?", optionNames, opti)) != -1) {
    switch(c) {
    case 't':
      trainfile = optarg;
      break;
    case 'T':
      testfile = optarg;
      break;
    case 'm':
      modelfile = optarg;
      break;
    case 'e':
      epsilon = atof(optarg);
      break;     
    case '?':
    default:
      usage(*argv);      
    }
  }
}

void CommandLineOption::usage(char* progname) {
  cerr << "usage: " << progname << "..." << endl;
  cerr << "\t-t --train: trainfile" << endl;
  cerr << "\t-T --test: testfile" << endl;
  cerr << "\t-m --model: modelfile (REQUIRED)" << endl;
  cerr << "\t-e --epsilon: tolerance (default=1e-3)" << endl;
  cerr << "\t-?, --help: this message" << endl;
  exit(0);
}


int main(int argc, char ** argv){
  
  CommandLineOption options(argc,argv);

  if( options.modelfile == ""){
    cerr << "Missing required arguments\n";
    options.usage(argv[0]);
    return 1;
  }
  if( options.trainfile == "" && options.testfile == ""){
    cerr << "Specify at least a train/test file\n";
    options.usage(argv[0]);
    return 1;
  }

  if(options.trainfile != ""){
    HMM hmm;
    ifstream in_model(options.modelfile);
    ifstream in_data(options.trainfile);
    assert(in_model.good() && in_data.good());
    in_model >> hmm;
    in_model.close(); 
    int numdata;
    vector<int> ** data = hmm.loadData(in_data, numdata); 
    hmm.train(data, numdata, options.epsilon);
    ofstream out_model(options.modelfile);
    assert(out_model.good());
    out_model << hmm;
    delete[] data;
  }
   
  if(options.testfile != ""){
    HMM hmm;
    ifstream in_model(options.modelfile);
    ifstream in_data(options.testfile);
    assert(in_model.good() && in_data.good());
    in_model >> hmm;
    in_model.close();
    int numdata;
    vector<int> ** data = hmm.loadData(in_data, numdata);
    hmm.test(data, numdata);
    delete[] data;
  }
  
  return 0;
}
#endif


