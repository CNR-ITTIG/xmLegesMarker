#ifndef HEADERPARSER_H
#define HEADERPARSER_H

#include <HMM.h>
#include <TextSequenceFeatureExtractor.h>

#define THRESHOLD -1e15
#define DEFAULT_INTESTAZIONE "\n<intestazione>\n<tipoDoc></tipoDoc><dataDoc norm=\"\"></dataDoc><numDoc></numDoc>\n<titoloDoc></titoloDoc>\n</intestazione>\n"
#define DEFAULT_FORMULAINIZIALE "\n<formulainiziale></formulainiziale>\n"
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
void copyElements(const vector<int>& src, 
		  vector<int>& dst, 
		  unsigned int start, 
		  unsigned int end);
void SqueezeWords(string& buf);
std::string trimEnd(const std::string& buf, unsigned int * trimmed);

class HeaderParser
{
  bool m_ted;
  HMM header_intestazione_model;
  HMM header_pubblicazione_model;
  HMM header_formulainiziale_model;
  HMM footer_formulafinale_model;
  HMM footer_dataeluogo_model;
  HMM footer_sottoscrizioni_model;
  HMM footer_annessi_model;
  TextSequenceFeatureExtractor header_extractor;
  TextSequenceFeatureExtractor footer_extractor;
  hash_map<int,pair<int,int> > header_intestazione_tags;
  hash_map<int,pair<int,int> > header_pubblicazione_tags;
  hash_map<int,pair<int,int> > header_formulainiziale_tags;
  hash_map<int,pair<int,int> > footer_formulafinale_tags;
  hash_map<int,pair<int,int> > footer_dataeluogo_tags;
  hash_map<int,pair<int,int> > footer_sottoscrizioni_tags;
  hash_map<int,pair<int,int> > footer_annessi_tags;

 public:
  HeaderParser(std::string model_dir,
	       std::string header_intestazione_model_file = "header_intestazione_model",
	       std::string header_pubblicazione_model_file = "header_pubblicazione_model",
	       std::string header_formulainiziale_model_file = "header_formulainiziale_model",
	       std::string footer_formulafinale_model_file = "footer_formulafinale_model",
	       std::string footer_dataeluogo_model_file = "footer_dataeluogo_model",
	       std::string footer_sottoscrizioni_model_file = "footer_sottoscrizioni_model",
	       std::string footer_annessi_model_file = "footer_annessi_model",
	       std::string header_extractor_model_file = "header_extractor_model",
	       std::string header_extractor_config_file = "header_extractor_config",
	       std::string footer_extractor_model_file = "footer_extractor_model",
	       std::string footer_extractor_config_file = "footer_extractor_config",
	       std::string parser_config_file = "parser_config");
  int parseHeader(const std::string& header, std::ostream& out);
  int parseFooter(const char * footer, std::ostream& out, int notes); 
  int parseHeader(std::istream& in, std::ostream& out); 
  int parseFooter(std::istream& in, std::ostream& out, int notes); 
  static const char * tagName(int tagvalue);
  static std::string tagAttributes(int tagvalue, const std::string& tagcontent, int * id);
  static bool hasCorrectStates(int * states, int statesnumber);
  static void removeProcessedElements(std::vector<int>& sequence, int last);
    
 protected:
  unsigned int saveTitle(const string& strbuffer, 
			 vector<string>& teds,
			 int * curr_ted,
			 int * states, 
			 unsigned int statesnumber,
			 const vector<int>& offsets, 
			 unsigned int offset, 
			 unsigned int state,
			 std::ostream& out,
			 bool found,
			 const hash_map<int,pair<int,int> >& tags,
			 int * id = 0) const;
  void saveTag(int tagvalue,
	       int start,
	       int end,
	       const std::string& buffer,
	       vector<string>& teds,
	       int * curr_ted,
	       std::ostream& out,
	       bool withtags = true, 
	       int * id = 0) const;
  void openTag(int tagvalue, 
	       std::ostream& out, 			   
	       const std::string& tagcontent = "",
	       int * id = 0) const;
  void closeTag(int tagvalue, std::ostream& out) const;
  bool ignoreTag(int tagvalue) const;
  bool trimmedTag(int tagvalue) const;
  bool errorTag(int tagvalue) const;
  bool noteTag(int tagvalue) const;
  bool formatTag(int tagvalue) const;
  void openContextTags(int tagvalue, ostream& out) const;
  void closeContextTags(int tagvalue, ostream& out) const;
  std::string addFormatTags(std::string buf,
			    vector<string>& teds,
			    int * curr_ted) const;
  std::string addSemicolumnFormatTags(std::string buf,
				      vector<string>& teds,
				      int * curr_ted) const;
  void init(std::istream& in);
  void defaultFooter(std::string footer, std::ostream& out) const;
  void defaultHeader(std::string header, ostream& out) const;
  unsigned int saveLastComma(const std::string& strbuffer, 
			     vector<string>& teds,
			     int * curr_ted,			  
			     int * states, 
			     unsigned int statesnumber,
			     const std::vector<int>& offsets, 
			     unsigned int offset, 
			     std::ostream& out,
			     const hash_map<int,pair<int,int> >& tags,
			     const vector<int>& header_sequence, 
			     const vector<int>& header_offsets, 
			     int * notes) const;
  unsigned int saveTags(const std::string& strbuffer, 
			vector<string>& teds,
			int * curr_ted,
			int * states, 
			unsigned int statesnumber,
			const std::vector<int>& offsets, 
			unsigned int offset, 
			unsigned int state,
			std::ostream& out,
			const hash_map<int,std::pair<int,int> >& tags,
			int * id = 0) const;
  bool hasVisto(int * states, int statesnumber) const;
  unsigned int getFirstMatchingState(int * states, 
				     unsigned int statesnumber,
				     const hash_map<int,pair<int,int> >& tags) const;
  int getLastMatchingState(int * states, 
			   unsigned int statesnumber,
			   const hash_map<int,pair<int,int> >& tags) const;
  void findPubblicazione(const string& strbuffer, 
			 vector<string>& teds,
			 int * curr_ted,
			 const vector<int>& sequence, 
			 int first,
			 const vector<int>& offsets, 
			 unsigned int offset, 
			 ostream& out,
			 int * notes);
  void savePubblicazione(const string& strbuffer, 
			 int * states, 
			 unsigned int statesnumber,
			 const vector<int>& offsets, 
			 unsigned int offset, 
			 const hash_map<int,pair<int,int> >& tags);
  string removeTeds(const std::string& header, vector<string>& teds);
  string addTeds(string buf, vector<string>& teds, int * curr_ted) const;
};

#endif


