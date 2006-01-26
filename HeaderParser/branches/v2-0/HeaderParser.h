#ifndef HEADERPARSER_H
#define HEADERPARSER_H

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <HMM.h>
#include <TextSequenceFeatureExtractor.h>

#define THRESHOLD -1e15
using namespace std;

typedef enum HeaderParser_tagTipo {
	hp_libro=0,
	hp_parte=1,
	hp_titolo=2,
	hp_capo=3,
	hp_sezione=4,
	hp_articolo=5,
	hp_comma=6,
	hp_lettera=7,
	hp_numero=8,
	hp_num=9,
	hp_corpo=10,
	hp_alinea=11,
	hp_rubrica=12,
	hp_tipodoc=13,
	hp_datadoc=14,
	hp_numdoc=15,
	hp_titolodoc=16,
	hp_formulainiziale=17,
	hp_formulafinale=18,
	hp_dataeluogo=19,
	hp_sottoscrizioni=20,
	hp_pubblicazione=21,
	hp_emanante=22,
	hp_intestazione=23,
	hp_conclusione=24,
	hp_annessi=25,
	hp_varie=26,
	hp_sottoscrivente=27,
	hp_visto=28,
	hp_sconosciuto=29,
	hp_preambolo=30,
	hp_registrazione=31,
	hp_nota=32,
	hp_lavoripreparatori=33,
	hp_mod=34,
	hp_virgolette=35,
	hp_datapubbl=36,
	hp_numpubbl=37,
	hp_sopubbl=38,
	hp_legislatura=39,
	hp_relazione=40,
	hp_div=41,
	hp_nothing=42} HP_tagTipo;

//#define TAGTIPODIM 39

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

class HeaderParser
{
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
  xmlNodePtr root_node;

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
  int parseHeader(std::string& header, 
		  xmlNodePtr meta,
		  xmlNodePtr descrittori,
		  xmlNodePtr intestazione,
		  xmlNodePtr formulainiziale,
		  int tdoc,
		  int notes);
  int parseFooter(xmlNodePtr lastcomma, 			      
		  xmlNodePtr meta,
		  xmlNodePtr descrittori,
		  xmlNodePtr formulafinale,
		  xmlNodePtr conclusione,
		  int tdoc,
		  int notes); 
  static const char * tagName(int tagvalue);
  static const char * tagIdName(int tagvalue);

  static bool hasCorrectStates(int * states, int statesnumber);
  static void removeProcessedElements(std::vector<int>& sequence, int last);
  void setRootNode(xmlNodePtr node) { root_node = node;};

 protected:
  void addTagAttributes(int tagvalue, 
			const std::string& tagcontent,
			xmlNodePtr tagnode, 
			int * id) const;
  unsigned int saveTitle(const string& strbuffer, 
			 int * states, 
			 unsigned int statesnumber,
			 const vector<int>& offsets, 
			 unsigned int offset, 
			 unsigned int state,
			 xmlNodePtr descrittori,
			 xmlNodePtr intestazione,
			 xmlNodePtr meta,
			 bool found,
			 const hash_map<int,pair<int,int> >& tags,
			 int * id = 0) const;
  xmlNodePtr openTag(int tagvalue,
		     xmlNodePtr startnode, 
		     const std::string& tagcontent = "",
		     int * id = 0) const;
  xmlNodePtr openContextTags(int tagvalue, xmlNodePtr startnode) const ;
  bool ignoreTag(int tagvalue) const;
  bool nothingTag(int tagvalue) const;
  bool trimmedTag(int tagvalue) const;
  bool trimmedTagDDL(int tagvalue) const; //Aggiunta
  bool errorTag(int tagvalue) const;
  bool noteTag(int tagvalue) const;
  bool formatTag(int tagvalue) const;
  void addSemicolumnFormatTags(string text, xmlNodePtr startnode) const;  
  void addFormatTags(string buf, xmlNodePtr startnode) const;
  void addFormatTagsDiv(string buf, xmlNodePtr startnode) const;
  void init(std::istream& in);
  void defaultFooter(std::string footer, xmlNodePtr lastcomma) const;
  void defaultHeader(xmlNodePtr descrittori, xmlNodePtr intestazione) const;				  
  xmlNodePtr saveTag(int tagvalue,
		     int start,
		     int end,
		     const string& buffer,
		     xmlNodePtr startnode,
		     int tdoc,
		     xmlNodePtr prev_node = NULL,
		     xmlNodePtr subs_node = NULL,
		     bool withtags = true,
		     int * id = 0) const;
  unsigned int saveLastComma(const string& strbuffer, 
			     int * states, 
			     unsigned int statesnumber,
			     const vector<int>& offsets, 
			     unsigned int offset, 
			     xmlNodePtr lastcomma,
			     const hash_map<int,pair<int,int> >& tags,
			     const vector<int>& header_sequence, 
			     const vector<int>& header_offsets, 
			     int * notes) const;
  unsigned int saveTags(const string& strbuffer, 
			int * states, 
			unsigned int statesnumber,
			const vector<int>& offsets, 
			unsigned int offset, 
			unsigned int initstate,
			xmlNodePtr startnode,
			const hash_map<int,pair<int,int> >& tags,
			int tdoc,
			int * id = 0,
			xmlNodePtr prev_node = NULL,
			xmlNodePtr subs_node = NULL) const;
  unsigned int getFirstMatchingState(int * states, 
				     unsigned int statesnumber,
				     const hash_map<int,pair<int,int> >& tags) const;
  int getLastMatchingState(int * states, 
			   unsigned int statesnumber,
			   const hash_map<int,pair<int,int> >& tags) const;
  void findPubblicazione(const string& strbuffer, 
			 const vector<int>& sequence, 
			 int first,
			 const vector<int>& offsets, 
			 unsigned int offset, 
			 xmlNodePtr meta,
			 xmlNodePtr currnode,
			 int * notes) const;
  bool savePubblicazione(const string& strbuffer, 
			 int * states, 
			 unsigned int statesnumber,
			 const vector<int>& offsets, 
			 unsigned int offset, 
			 xmlNodePtr descrittori,
			 const hash_map<int,pair<int,int> >& tags) const;
  xmlNodePtr addChildIfMissing(const char * nodename, 
			       bool * added,
			       xmlNodePtr startnode = NULL,
			       const char * content = "") const;
  xmlNodePtr findChild(const char * nodename, xmlNodePtr startnode = NULL) const;
  void addMissingMeta(xmlNodePtr meta) const;
  void addMissingHeader(xmlNodePtr meta,xmlNodePtr descrittori,xmlNodePtr intestazione,
			      xmlNodePtr formulainiziale, int tdoc) const;
  void HeaderParser::addMissingFooter(xmlNodePtr meta,xmlNodePtr descrittori,
  			xmlNodePtr formulafinale,xmlNodePtr conclusione,int tdoc) const;
};

#endif


