#include "HeaderParser.h"
#include "../ParserStruttura/tag.h"
#include <fstream>
#include <assert.h>
#include "Default.h"

using namespace std;

Month MonthMapping;

HeaderParser::HeaderParser(std::string modeldir,
			   std::string header_model_file,
			   std::string footer_formulafinale_model_file,
			   std::string footer_dataeluogo_model_file,
			   std::string footer_sottoscrizioni_model_file,
			   std::string header_extractor_model_file,
			   std::string header_extractor_config_file,
			   std::string footer_extractor_model_file,
			   std::string footer_extractor_config_file,
			   std::string parser_config_file)
{
  ifstream in((modeldir + "/" + header_model_file).c_str());
  if(in.good()){
    in >> header_model;
    in.close();
  }
  else{
    istringstream in_s(header_model_default);
    in_s >> header_model;
  }

  ifstream in2((modeldir + "/" + footer_formulafinale_model_file).c_str());
  if(in2.good()){
    in2 >> footer_formulafinale_model;
    in2.close();
  }
  else{
    istringstream in_s(footer_formulafinale_model_default);
    in_s >> footer_formulafinale_model;
  }

  ifstream in6((modeldir + "/" + footer_dataeluogo_model_file).c_str());
  if(in6.good()){
    in6 >> footer_dataeluogo_model;
    in6.close();
  }
  else{
    istringstream in_s(footer_dataeluogo_model_default);
    in_s >> footer_dataeluogo_model;
  }

  ifstream in7((modeldir + "/" + footer_sottoscrizioni_model_file).c_str());
  if(in7.good()){
    in7 >> footer_sottoscrizioni_model;
    in7.close();
  }
  else{
    istringstream in_s(footer_sottoscrizioni_model_default);
    in_s >> footer_sottoscrizioni_model;
  }

  ifstream in3((modeldir + "/" + header_extractor_model_file).c_str());
  ifstream in3_config((modeldir + "/" + header_extractor_config_file).c_str());
  if(in3.good() && in3_config.good()){
    header_extractor(in3_config, in3);
    in3.close();
    in3_config.close();
  }
  else{
    istringstream in_s(header_extractor_model_default);
    istringstream in_sc(header_extractor_config_default);
    header_extractor(in_sc, in_s);
  }
  
  ifstream in4((modeldir + "/" + footer_extractor_model_file).c_str());
  ifstream in4_config((modeldir + "/" + footer_extractor_config_file).c_str());
  if(in4.good() && in4_config.good()){
    footer_extractor(in4_config, in4);
    in4.close();
    in4_config.close();
  }
  else{
    istringstream in_s(footer_extractor_model_default);
    istringstream in_sc(footer_extractor_config_default);
    footer_extractor(in_sc, in_s);
  }

  ifstream in5_config((modeldir + "/" + parser_config_file).c_str());
  if(in5_config.good()){
    init(in5_config);
    in5_config.close();
  }
  else{
    istringstream in_sc(parser_config_default);
    init(in_sc);
  }
}

void HeaderParser::init(istream& in)
{
  int state,tag;
  bool open;
  string buf;
  if(!getline(in,buf) || buf != "HEADER"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  while(getline(in,buf) && buf != ""){
    istringstream is(buf);
    is >> state >> tag >> ws;
    header_tags[state] = tag;
  }
  if(!getline(in,buf) || buf != "FOOTER"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  if(!getline(in,buf) || buf != "FORMULAFINALE"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  while(getline(in,buf) && buf != ""){
    istringstream is(buf);
    is >> state >> tag >> open >> ws;
    footer_formulafinale_tags[state] = make_pair(tag,open);
  }
  if(!getline(in,buf) || buf != "DATAELUOGO"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  while(getline(in,buf) && buf != ""){
    istringstream is(buf);
    is >> state >> tag >> open >> ws;
    footer_dataeluogo_tags[state] = make_pair(tag,open);
  }
  if(!getline(in,buf) || buf != "SOTTOSCRIZIONI"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  while(getline(in,buf) && buf != ""){
    istringstream is(buf);
    is >> state >> tag >> open >> ws;
    footer_sottoscrizioni_tags[state] = make_pair(tag,open);
  }
}

void HeaderParser::parseHeader(const char * header, ostream& out) 
{
  if (!parse(header, 0, header_model, header_tags, header_extractor, true, out)){
    out << DEFAULT_HEADER; 
    if(((string)header).find_first_not_of(" \n\t") != string::npos)
      out << "<?error\n" << header << "\n?>";
    out << endl;
  }
}

void HeaderParser::parseHeader(istream& in, ostream& out) 
{
  string header,buf;
  while(getline(in, buf))
    header += buf + "\n";
  parseHeader(header.c_str(), out);
}

void HeaderParser::parseFooter(const char * buffer, ostream& out) 
{
  bool found = false;
  unsigned int last = 0, offset = 0;
  int * states = NULL;

  // extract terms
  string strbuffer = buffer;
  vector<int> sequence, offsets;
  if(!footer_extractor.buildExample(sequence, offsets, buffer, strlen(buffer), 0)){
    defaultFooter(buffer, out);	
    return;
  }

  // parse formulafinale
  if(sequence.size() > 0){
    states = new int[sequence.size()];
    footer_formulafinale_model.viterbiPath(sequence, states, sequence.size());
    if (hasCorrectStates(states, sequence.size())){
      if (!found){
	last = saveLastComma(strbuffer, states, sequence.size(), offsets, offset, out, footer_formulafinale_tags);
	found = true;
      }
      last = saveTags(strbuffer, states, sequence.size(), offsets, offset, last, out, footer_formulafinale_tags);
      removeProcessedElements(sequence, last);
      offset += last + 1;
      openTag(conclusione, out);
    }
    delete[] states;
  }

  // parse dataeluogo
  if(sequence.size() > 0){
    states = new int[sequence.size()];
    footer_dataeluogo_model.viterbiPath(sequence, states, sequence.size());
    if (hasCorrectStates(states, sequence.size())){
      last = 0;
      if (!found){
	last = saveLastComma(strbuffer, states, sequence.size(), offsets, offset, out, footer_dataeluogo_tags);
	found = true;
	openTag(formulafinale, out);
	closeTag(formulafinale, out);
	openTag(conclusione, out);
      }
      last = saveTags(strbuffer, states, sequence.size(), offsets, offset, last, out, footer_dataeluogo_tags);
      removeProcessedElements(sequence, last);
      offset += last + 1;
    }
    delete[] states;
  }

  // parse sottoscrizioni
  if(sequence.size() > 0){
    states = new int[sequence.size()];
    footer_sottoscrizioni_model.viterbiPath(sequence, states, sequence.size());
    if (hasCorrectStates(states, sequence.size())){
      last = 0;
      if (!found){
	last = saveLastComma(strbuffer, states, sequence.size(), offsets, offset, out, footer_sottoscrizioni_tags);
	found = true;
	openTag(formulafinale, out);
	closeTag(formulafinale, out);
	openTag(conclusione, out);
      }      
      openTag(sottoscrizioni, out);
      last = saveTags(strbuffer, states, sequence.size(), offsets, offset, last, out, footer_sottoscrizioni_tags);
      found = true;
      if (!hasVisto(states, sequence.size())){
	openTag(visto, out);
	closeTag(visto, out);
      }
      removeProcessedElements(sequence, last);
      offset += last + 1;
      closeTag(sottoscrizioni, out);
    }
    delete[] states;
  }

  // if nothing found print default footer
  if(!found){
    defaultFooter(buffer, out);	
    return;
  }
  
  // put remains (annessi) in error tag
  closeTag(conclusione, out);
  if (offset < offsets.size())
    saveTag(annessi, offsets[offset], strbuffer.length(), strbuffer, out); 
}

bool HeaderParser::hasCorrectStates(int * states, int statesnumber)
{
  for(int i = 0; i < statesnumber; i++)
    if(states[i] > 0)
      return true;
  return false;
}

bool HeaderParser::hasVisto(int * states, int statesnumber) const
{
  for(int i = 0; i < statesnumber; i++){
    hash_map<int,pair<int,bool> >::const_iterator k = footer_sottoscrizioni_tags.find(states[i]);
    assert(k != footer_sottoscrizioni_tags.end());
    if((k->second).first == visto)
      return true;
  }
  return false;
}

unsigned int HeaderParser::saveLastComma(const string& strbuffer, 
					 int * states, 
					 unsigned int statesnumber,
					 const vector<int>& offsets, 
					 unsigned int offset, 
					 ostream& out,
					 const hash_map<int,pair<int,bool> >& tags) const
{
  unsigned int state = 0;
  while(state < statesnumber){
    hash_map<int,pair<int,bool> >::const_iterator k = tags.find(states[state]);
    assert(k != tags.end());
    if(!errorTag((k->second).first))
      break;
    state++;
  }
  if(state==0)
    out << "<comma>\n</comma>\n";
  else
    out <<  "<comma>" << strbuffer.substr(offsets[offset], offsets[offset+state]-offsets[offset]) << "\n</comma>\n";
  return state;
}

void HeaderParser::removeProcessedElements(vector<int>& sequence, int last)
{
  for(unsigned int i = last+1; i < sequence.size(); i++)
    sequence[i-last-1] = sequence[i];
  sequence.resize(sequence.size()-last-1);
}

unsigned int HeaderParser::saveTags(const string& strbuffer, 
				    int * states, 
				    unsigned int statesnumber,
				    const vector<int>& offsets, 
				    unsigned int offset, 
				    unsigned int state,
				    ostream& out,
				    const hash_map<int,pair<int,bool> >& tags) const
{
  hash_map<int,pair<int,bool> >::const_iterator statetag;

  // get last matched state
  unsigned int last;
  for(last = statesnumber-1; last >= 0; last--){
    statetag = tags.find(states[last]);
    assert(statetag != tags.end());
    if(!errorTag((statetag->second).first))
      break;
  }
 
  // save tags for matching states
  int currtag = -1, start = -1;
  for(; state <= last; state++){
    statetag = tags.find(states[state]);
    assert(statetag != tags.end());
    if((state == 0 || states[state] != states[state-1]) && ((statetag->second).second || (statetag->second).first != currtag)){
      if(currtag != -1)
	saveTag(currtag, start, offsets[state+offset], strbuffer, out);
      currtag = (statetag->second).first;
      start = offsets[state+offset];
    }
    if (state == last){
      if(currtag != -1){
	if(last < statesnumber-1)
	  saveTag(currtag, start, offsets[state+offset+1], strbuffer, out);
	else
	  saveTag(currtag, start, strbuffer.length(), strbuffer, out);
      }
    }
  }
  return last;
}

void  HeaderParser::defaultFooter(std::string footer, ostream& out) const
{
  unsigned int dot = footer.find('.');
  if(dot != string::npos){  
    out <<  "<comma>" << footer.substr(0, dot+1) << "\n</comma>\n";
    if (footer.substr(dot+1).find_first_not_of(" \n\t") != string::npos)
      out << "<?error\n" << footer.substr(dot+1) << "\n?>\n"; 
  }
  else{
    out << "<?error\n" << footer << "\n?>\n"; 
  }
  out << DEFAULT_FOOTER << endl;
}

void HeaderParser::parseFooter(istream& in, ostream& out) 
{
  string footer,buf;
  while(getline(in, buf))
    footer += buf + "\n";  
  parseFooter(footer.c_str(), out);
}

bool HeaderParser::parse(const char * buffer, 
			 int offset, 
			 const HMM& model, 
			 const hash_map<int,int>& tags,
			 TextSequenceFeatureExtractor& extractor,
			 bool header,
			 ostream& out) 
{
  string strbuffer = buffer;
  vector<int> sequence, offsets;
  if(!extractor.buildExample(sequence, offsets, buffer, strlen(buffer), 0))
    return false;
  int states[sequence.size()];
  if (model.viterbiPath(sequence, states, sequence.size()) < THRESHOLD)
    return false;
  int currtag = -1, start = -1;

  if(header)
    openTag(intestazione, out);

  for(unsigned int i = 0; i < sequence.size(); i++){
    hash_map<int,int>::const_iterator statetag = tags.find(states[i]);
    if(statetag == tags.end()){
      cerr << "ERROR: state " << states[i] << " has no matching tag" << endl;
      exit(1);
    }
    if(statetag->second != currtag){
      if(currtag != -1)
	saveTag(currtag, start-offset, offsets[i], strbuffer, out);
      currtag = statetag->second;
      start = offset + offsets[i];
    }
    if (i == sequence.size()-1){
      if(currtag != -1)
	saveTag(currtag, start-offset, strlen(buffer), strbuffer, out);
    }
  }
  return true;
}

void HeaderParser::saveTag(int tagvalue,
			   int start,
			   int end,
			   const string& buffer,
			   ostream& out) const 
{
  if(errorTag(tagvalue)){
    out <<  "<?error\n" << buffer.substr(start,end-start) << "\n?>" << endl;
    return;
  }
  if(ignoreTag(tagvalue)){
    out << buffer.substr(start,end-start) << endl;
    return;
  }
  out << "<" << tagName(tagvalue) << tagAttributes(tagvalue, buffer.substr(start,end-start)) << ">" << endl;
  if(tagvalue == formulainiziale)
    out << addFormatTags(buffer.substr(start,end-start));
  else if(tagvalue == formulafinale)
    out << "<h:p> " << buffer.substr(start,end-start) << " </h:p>\n";
  else
    out << buffer.substr(start,end-start);
  out << "</" << tagName(tagvalue) << ">" << endl;
  if(tagvalue == titolodoc)
    closeTag(intestazione, out);
}

string normalizeDate(const string& buffer)
{
  unsigned int beg = buffer.find_first_of("0123456789");
  if (beg == string::npos) return "";
  unsigned int end = buffer.find_first_not_of("0123456789", beg);
  if (end == string::npos) return "";
  string day = buffer.substr(beg, end - beg);
  if(day.length() == 1)
    day = "0" + day;
  beg = buffer.find_first_not_of(" \r\t\n", end);
  if (beg == string::npos) return "";
  end = buffer.find_first_of(" \r\t\n", beg);
  if (end == string::npos) return "";
  string month = MonthMapping[lowercase(buffer.substr(beg, end - beg))];
  if (month == "") return "";
  beg = buffer.find_first_of("0123456789", end);
  if (beg == string::npos) return "";
  end = buffer.find_first_not_of("0123456789", beg);
  if (end == string::npos) end = buffer.length();
  string year = buffer.substr(beg, end - beg);
  if(year.length() != 4)
    return "";
  return year + month + day;
}

string lowercase(const string& word)
{

  string lower;
  for(unsigned int i=0; i<word.length(); i++)
    lower += tolower(word[i]);
  return lower;
}


string HeaderParser::addFormatTags(string buf) const
{
  istringstream in(buf);
  string line, out;

  while(getline(in, line))
    out += "<h:p> " + line + " </h:p>\n";
  return out;
}

bool HeaderParser::errorTag(int tagvalue) const
{
  switch(tagTipo(tagvalue)){
  case sconosciuto:
  case varie:
  case annessi:
  case pubblicazione: return true;
  default: return false;
  }
}

bool HeaderParser::ignoreTag(int tagvalue) const
{
  switch(tagTipo(tagvalue)){
  default: return false;
  }
}

void HeaderParser::openTag(int tagvalue, ostream& out) const
{
  out << "<" << tagName(tagvalue) << ">" << endl;
}

void HeaderParser::closeTag(int tagvalue, ostream& out) const
{
  out << "</" << tagName(tagvalue) << ">" << endl;
}


const char * HeaderParser::tagName(int tagvalue)
{
  switch(tagTipo(tagvalue)){
  case libro:	return "libro";
  case parte:	return "parte";
  case titolo:	return "titolo";
  case capo:	return "capo";
  case sezione:	return "sezione";
  case articolo:	return "articolo";
  case comma:	return "comma";
  case lettera:	return "el";
  case numero:	return "en";
  case num:	return "num";
  case corpo:	return "corpo";
  case alinea:	return "alinea";
  case rubrica:	return "rubrica";
  case tipodoc: return "tipoDoc";
  case datadoc: return "dataDoc";
  case numdoc:  return "numDoc";
  case titolodoc: return "titoloDoc";
  case formulainiziale: return "formulainiziale";
  case formulafinale: return "formulafinale";
  case dataeluogo: return "dataeluogo";
  case sottoscrizioni: return "sottoscrizioni";
  case pubblicazione: return "pubblicazione";
  case emanante: return "emanante";
  case conclusione: return "conclusione";
  case intestazione: return "intestazione";
  case annessi: return "annessi";
  case varie: return "varie";
  case sottoscrivente: return "sottoscrivente";
  case visto: return "visto";
  case sconosciuto: return "";
  default: return "";
  }
}

std::string HeaderParser::tagAttributes(int tagvalue, const std::string& attributevalue)
{
  switch(tagTipo(tagvalue)){
  case datadoc:
  case dataeluogo:
    return (string)" norm=\"" +  normalizeDate(attributevalue) + "\"";
  default:
    return "";
  }
}


#ifdef HEADERPARSER
int main(int argc, char* argv[]) {

  char *r = rindex(argv[0], '/');
  if (r) *r = 0; else r = rindex(argv[0], '\\');
  if (r) *r = 0;
  string workdir = (string)argv[0] + "/Models";
  string config_files[] = { "header_model",
			    "footer_formulafinale_model",
			    "footer_dataeluogo_model",
			    "footer_sottoscrizioni_model",
			    "header_extractor_model",
			    "header_extractor_config",
			    "footer_extractor_model",
			    "footer_extractor_config",
			    "parser_config"};

  if (argc < 2 || !strcmp(argv[1], "--help")){
    cerr << "Usage: " << argv[0] << " [-header,-footer]"
	 << " [ <workdir> <header_model> <footer_formulafinale_model>"
	 << " <footer_dataeluogo_model> <footer_sottoscrizioni_model>"
	 << " <header_extractor_model> <header_extractor_config>"
	 << " <footer_extractor_model> <footer_extractor_config>"
	 << " <parser_config> ]" << endl;
    exit(0);
  }
  for (int arg = 1; arg < argc; arg++){

    if (*argv[arg] == '-'){

       const char * command = argv[arg];
       if(argc - arg - 1 > 0){
	 workdir = argv[++arg];
	 int i = 0;
	 while(++arg < argc)
	   config_files[i++] = argv[arg];
       }
       
       HeaderParser parser(workdir,
			   config_files[1],
			   config_files[2],
			   config_files[3],
			   config_files[4],
			   config_files[5],
			   config_files[6],
			   config_files[7],
			   config_files[8]);

       if (!strcmp(command, "-header"))
	 parser.parseHeader(cin, cout);
       else if (!strcmp(command, "-footer"))
	 parser.parseFooter(cin, cout);
       else{
	 cerr << "ERROR: unknown command " << argv[arg] << endl;
	 return -1;
       }
    }
    else{
      cerr << "ERROR: missing command " << endl;
      return -1;
    }
  }
}
#endif





















