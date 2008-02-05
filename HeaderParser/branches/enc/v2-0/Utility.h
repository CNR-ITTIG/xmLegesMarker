#ifndef UTILITY_H
#define UTILITY_H

#include <map>
#include <string>
#include <vector>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

using namespace std;

class Month{
  
  map<std::string, std::string> mapping;
  
 public:
  Month(){
    mapping["gennaio"] = "01";
    mapping["febbraio"] = "02";
    mapping["marzo"] = "03";
    mapping["aprile"] = "04";
    mapping["maggio"] = "05";
    mapping["giugno"] = "06";    
    mapping["luglio"] = "07";
    mapping["agosto"] = "08";
    mapping["settembre"] = "09";
    mapping["ottobre"] = "10";
    mapping["novembre"] = "11";
    mapping["dicembre"] = "12";
  }
  std::string operator[](std::string buf) const {
    map<std::string, std::string>::const_iterator k = mapping.find(buf);
    return (k == mapping.end()) ? "" : k->second;
  }
};

std::string normalizeDate(const std::string& buffer);
std::string lowercase(const std::string& word);
void copyElements(const vector<int>& src, 
		  vector<int>& dst, 
		  unsigned int start, 
		  unsigned int end);
void SqueezeWords(string& buf);
std::string trimEnd(const std::string& buf, unsigned int * trimmed);
std::string extractURN(std::string& strbuffer);
bool isEmptyString(const string& buf);

xmlNodePtr findChild(const char * nodename, xmlNodePtr startnode);

#endif
