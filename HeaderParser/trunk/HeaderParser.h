#ifndef HEADERPARSER_H
#define HEADERPARSER_H

#include <HMM.h>
#include <TextSequenceFeatureExtractor.h>

#define THRESHOLD -1e10
#define DEFAULT_HEADER "\n<intestazione></intestazione>\n<formulainiziale></formulainiziale>\n"
#define DEFAULT_FOOTER "\n<formulafinale></formulafinale>\n<conclusione></conclusione>\n"
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


class HeaderParser
{
  HMM header_model;
  HMM footer_model;
  TextSequenceFeatureExtractor header_extractor;
  TextSequenceFeatureExtractor footer_extractor;
  hash_map<int,int> header_tags;
  hash_map<int,int> footer_tags;

 public:
  HeaderParser(const char * header_model_file,
		  const char * footer_model_file,
		  const char * header_extractor_model_file,
		  const char * header_extractor_config_file,
		  const char * footer_extractor_model_file,
		  const char * footer_extractor_config_file,
		  const char * parser_config_file);
  HeaderParser(std::string model_dir,
	       std::string header_model_file = "header_model",
	       std::string footer_model_file = "footer_model",
	       std::string header_extractor_model_file = "header_extractor_model",
	       std::string header_extractor_config_file = "header_extractor_config",
	       std::string footer_extractor_model_file = "footer_extractor_model",
	       std::string footer_extractor_config_file = "footer_extractor_config",
	       std::string parser_config_file = "parser_config");
  void parseHeader(const char * header, std::ostream& out);
  void parseFooter(const char * footer, int offset, std::ostream& out); 
  void parseHeader(std::istream& in, std::ostream& out); 
  void parseFooter(std::istream& in, int offset, std::ostream& out); 
  static const char * tagName(int tagvalue);
  static std::string tagAttributes(int tagvalue, const std::string& attributevalue);

 protected:
  void saveTag(int tagvalue,
	       int start,
	       int end,
	       const std::string& buffer,
	       std::ostream& out) const;
  void openTag(int tagvalue, std::ostream& out) const;
  void closeTag(int tagvalue, std::ostream& out) const;
  bool ignoreTag(int tagvalue) const;
  bool errorTag(int tagvalue) const;
  std::string addFormatTags(std::string buf) const;
  bool parse(const char * buffer, 
	     int offset, 
	     const HMM& model, 
	     const hash_map<int,int>& tags,
	     TextSequenceFeatureExtractor& extractor,
	     bool header,
	     std::ostream& out);
  void init(std::istream& in);
  void defaultFooter(std::string footer, std::ostream& out) const;
};

#endif

