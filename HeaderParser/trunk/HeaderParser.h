#ifndef HEADERPARSER_H
#define HEADERPARSER_H

#include <HMM.h>
#include <TextSequenceFeatureExtractor.h>

using namespace std;

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
  void parseHeader(const char * header);
  void parseFooter(const char * footer, int offset); 
  void parseHeader(std::istream& in); 
  void parseFooter(std::istream& in, int offset); 
  static const char * tagName(int tagvalue);

 protected:
  void saveTag(int tagvalue,
	       int start,
	       int end,
	       const std::string& buffer) const;
  void parse(const char * buffer, 
	     int offset, 
	     const HMM& model, 
	     const hash_map<int,int>& tags,
	     TextSequenceFeatureExtractor& extractor);
  void init(std::istream& in);

};

#endif
