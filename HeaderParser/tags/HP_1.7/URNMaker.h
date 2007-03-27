#ifndef URNMAKER_H
#define URNMAKER_H

#include <Lexer/Lexer.h>
#include "Utility.h"

using namespace std;
typedef map<string, string> Conversion;
typedef Conversion::iterator ConversionIterator;
typedef Conversion::const_iterator ConversionConstIterator;

class URNMaker{

  Lexer m_lexer;
  Conversion m_abbreviations; 
  Conversion m_provvedimento2autorita; 
  Conversion m_provvedimento2provvedimento;   
  Conversion m_autorita2autorita; 
  
 public:
  void operator()(istream& in_conversions,
		  istream& in_config,
		  istream& in_stopwords);
  string makeURN(xmlNodePtr root_node);

 protected:
  void loadConversions(istream& in);
  string extractAutorita(const string& provvedimento) const;
  string tokenize(const string& buf);
  string normalizeProvvedimento(const string& provvedimento) const;
  string normalizeAutorita(const string& autorita) const;
  string getNodeContent(xmlNodePtr node) const;
  //string getNodeInitialTextContent(xmlNodePtr node) const;
  string getNodePropContent(xmlNodePtr node, const char * prop) const;
};

#endif

