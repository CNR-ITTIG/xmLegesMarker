#include "URNMaker.h"
#include <fstream>
#include <sstream>
#include <assert.h>

using namespace std;

void URNMaker::operator()(istream& in_conversions,
			  istream& in_config,
			  istream& in_stopwords)
{
  loadConversions(in_conversions);
  m_lexer(in_config, in_stopwords);
}

void URNMaker::loadConversions(istream& in)
{
  string buf,src,dst;
  if(!Lexer::getLine(in,buf) || buf != "ABBREVIATIONS"){
    cerr << "ERROR in reading URNMaker conversions file" << endl;
    exit(1);
  }
  while(Lexer::getLine(in,buf) && buf != ""){
    istringstream is(buf);
    is >> src >> dst >> ws;
    m_abbreviations[src] = dst;
  }
  if(!Lexer::getLine(in,buf) || buf != "PROVVEDIMENTO2AUTORITA"){
    cerr << "ERROR in reading URNMaker conversions file" << endl;
    exit(1);
  }
  while(Lexer::getLine(in,buf) && buf != ""){
    istringstream is(buf);
    is >> src >> dst >> ws;
    m_provvedimento2autorita[src] = dst;
  }
  if(!Lexer::getLine(in,buf) || buf != "PROVVEDIMENTO2PROVVEDIMENTO"){
    cerr << "ERROR in reading URNMaker conversions file" << endl;
    exit(1);
  }
  while(Lexer::getLine(in,buf) && buf != ""){
    istringstream is(buf);
    is >> src >> dst >> ws;
    m_provvedimento2provvedimento[src] = dst;
  }
  if(!Lexer::getLine(in,buf) || buf != "AUTORITA2AUTORITA"){
    cerr << "ERROR in reading URNMaker conversions file" << endl;
    exit(1);
  }
  while(Lexer::getLine(in,buf) && buf != ""){
    istringstream is(buf);
    is >> src >> dst >> ws;
    m_autorita2autorita[src] = dst;
  }
}

string URNMaker::makeURN(xmlNodePtr root_node)
{
  xmlNodePtr intestazione_node = findChild("intestazione", root_node);
  if(intestazione_node == NULL)
    return "";
  xmlNodePtr emanante_node = findChild("emanante", intestazione_node);
  xmlNodePtr tipodoc_node = findChild("tipoDoc", intestazione_node);
  xmlNodePtr datadoc_node = findChild("dataDoc", intestazione_node);
  xmlNodePtr numdoc_node = findChild("numDoc", intestazione_node);
  xmlNodePtr formulainiziale_node = findChild("formulainiziale", root_node);
  string urn = "urn:nir:", buf = "";
  string provvedimento_content, autorita_content;
  
  if(tipodoc_node == NULL)
    return "";
  buf = getNodeContent(tipodoc_node);
  provvedimento_content = tokenize(buf);
  autorita_content = extractAutorita(provvedimento_content); 
  if(autorita_content.size() > 0)
    urn += normalizeAutorita(autorita_content) + ":";
  else if(emanante_node != NULL){
    buf = getNodeContent(emanante_node);
    autorita_content = tokenize(buf);
    urn += normalizeAutorita(autorita_content) + ":";
  }
  else if(formulainiziale_node != NULL){
    buf = getNodeContent(formulainiziale_node);
    //buf = getNodeInitialTextContent(formulainiziale_node);
    autorita_content = tokenize(buf);
    urn += normalizeAutorita(autorita_content) + ":";
  }
  else
    urn += "stato:"; // default a caso
  urn += normalizeProvvedimento(provvedimento_content) + ":";

  if(datadoc_node != NULL){
    buf = getNodePropContent(datadoc_node, "norm");
    urn += buf.substr(0,4) + "-" + buf.substr(4,2) + "-" + buf.substr(6,2) + ";";
  }
  if(numdoc_node != NULL)
    urn += getNodeContent(numdoc_node);
  else
    urn += "nir-1";
  return urn;
}

string URNMaker::extractAutorita(const string& provvedimento) const 
{
  ConversionConstIterator k = m_provvedimento2autorita.find(provvedimento);
  if(k != m_provvedimento2autorita.end())
    return k->second;
  return "";
}

string URNMaker::tokenize(const string& buf)
{
  string tokenized = "";
  m_lexer.lexBuffer(buf.c_str(), buf.length());
  Sequence words;
  m_lexer.getSequence(words);
  for(int i = 0; i < words.size(); i++){
    ConversionConstIterator k = m_abbreviations.find(words[i]);
    if(k != m_abbreviations.end())
      tokenized += k->second + ".";
    else
      tokenized += words[i] + ".";
  }
  if(tokenized.length() > 0)
    tokenized.erase(tokenized.length()-1);
  return tokenized;
}

string URNMaker::normalizeProvvedimento(const string& provvedimento) const
{
  ConversionConstIterator k = m_provvedimento2provvedimento.find(provvedimento);
  if(k != m_provvedimento2provvedimento.end())
    return k->second;
  return provvedimento;
}

string URNMaker::normalizeAutorita(const string& autorita) const
{
  string normautorita = autorita;
  ConversionConstIterator k = m_autorita2autorita.find(normautorita);
  if(k != m_autorita2autorita.end())
    return k->second;
  unsigned int end = normautorita.find(".concerto.");
  if(end != string::npos)
    normautorita.replace(end, end+strlen(".concerto."), "+");
  end = normautorita.find(".vist");
  if(end != string::npos)
    return normautorita.substr(0,end);
  return normautorita;
}

//string URNMaker::getNodeInitialTextContent(xmlNodePtr node) const
//{
//   xmlNodePtr child = NULL;
//   string content = "";
//   while((child = node->xmlChildrenNode) != NULL){
//     if (xmlNodeIsText(child)){
//       content = getNodeContent(child);
//       if (!isEmptyString(content))
// 	return content;
//     }
//     child = child->next;
//   }
//   return "";
//}



string URNMaker::getNodeContent(xmlNodePtr node) const
{
  xmlChar * buf = xmlNodeGetContent(node);
  string data = (char *) buf;
  xmlFree(buf);
  return data;
}

string URNMaker::getNodePropContent(xmlNodePtr node, const char * prop) const
{
  xmlChar * buf = xmlGetProp(node, BAD_CAST prop);
  string data = (char *) buf;
  xmlFree(buf);
  return data;
}

