#include "HeaderParser.h"
#include "../ParserStruttura/tag.h"
#include <fstream>

using namespace std;

HeaderParser::HeaderParser(const char * header_model_file,
				 const char * footer_model_file,
				 const char * header_extractor_model_file,
				 const char * header_extractor_config_file,
				 const char * footer_extractor_model_file,
				 const char * footer_extractor_config_file,
				 const char * parser_config_file)
{
  ifstream in(header_model_file);
  assert(in.good());
  in >> header_model;
  in.close();
  in.open(footer_model_file);
  assert(in.good());
  in >> footer_model;
  in.close();
  in.open(header_extractor_model_file);
  assert(in.good());
  ifstream in_config(header_extractor_config_file);
  assert(in_config.good());
  header_extractor(in_config, in);
  in.close();
  in_config.close();
  in.open(footer_extractor_model_file);
  assert(in.good());
  in_config.open(footer_extractor_config_file);
  assert(in_config.good());
  footer_extractor(in_config, in);
  in.close();
  in_config.close();
  in_config.open(parser_config_file);
  assert(in_config.good());
  init(in_config);
  in_config.close();
}

void HeaderParser::init(istream& in)
{
  int state,tag;
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
  while(getline(in,buf) && buf != ""){
    istringstream is(buf);
    is >> state >> tag >> ws;
    footer_tags[state] = tag;
  }
}

void HeaderParser::parseHeader(const char * header) 
{
  parse(header, 0, header_model, header_tags, header_extractor);
}

void HeaderParser::parseHeader(istream& in) 
{
  string header,buf;
  while(getline(in, buf))
    header += buf + "\n";  
  parse(header.c_str(), 0, header_model, header_tags, header_extractor);
}

void HeaderParser::parseFooter(const char * footer, int offset) 
{
  parse(footer, offset, footer_model, footer_tags, footer_extractor); 
}

void HeaderParser::parseFooter(istream& in, int offset) 
{
  string footer,buf;
  while(getline(in, buf))
    footer += buf + "\n";  
  parse(footer.c_str(), offset, footer_model, footer_tags, footer_extractor);
}

void HeaderParser::parse(const char * buffer, 
			    int offset, 
			    const HMM& model, 
			    const hash_map<int,int>& tags,
			    TextSequenceFeatureExtractor& extractor) 
{
  string strbuffer = buffer;
  vector<int> sequence, offsets;
  if(!extractor.buildExample(sequence, offsets, buffer, strlen(buffer), 0)){
    cerr << "ERROR: couldn't parse sequence\n" << buffer << endl;
    exit(1);
  }
  int states[sequence.size()];
  model.viterbiPath(sequence, states, sequence.size());
  int currtag = -1, start = -1;

  for(int i = 0; i < sequence.size(); i++){
    hash_map<int,int>::const_iterator statetag = tags.find(states[i]);
    if(statetag == tags.end()){
      cerr << "ERROR: state " << states[i] << " has no matching tag" << endl;
      exit(1);
    }
    if((statetag->second != currtag) || (i == sequence.size()-1)){
      if(currtag != -1){
#ifndef HEADERPARSER
	saveTag(currtag, start, offset + offsets[i]-1, strbuffer);
#else
	saveTag(currtag, start-offset, offsets[i]-1, strbuffer);
#endif
      }
      currtag = statetag->second;
      start = offset + offsets[i];
    }
  }
}

void HeaderParser::saveTag(int tagvalue,
			   int start,
			   int end,
			   const string& buffer) const 
{
#ifndef HEADERPARSER
  tag * tagstruct = tagInit(tagTipo(tagvalue), start, end, 0.);
  tagMemorizza(tagstruct);
#else
  cout << "<" << tagName(tagvalue) << ">" 
       << buffer.substr(start,end-start+1) 
       << "</" << tagName(tagvalue) << ">" << endl;
#endif
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
  case varie:
  default: return "";
  }
}

#ifdef HEADERPARSER
int main(int argc, char* argv[]) {

  if (argc < 2){
    cerr << "Usage: " << argv[0] << " [-header,-footer]" << endl;
    exit(0);
  }
  for (int arg = 1; arg < argc; arg++){
    
    if (*argv[arg] == '-'){
      
       const char * command = argv[arg];
       if ((argc - arg - 1) < 7){
	 cerr << "Too few arguments, need:"
	      << " <header_model> <footer_model>"
	      << " <header_extractor_model> <header_extractor_config>"
	      << " <footer_extractor_model> <footer_extractor_config>"
	      << " <parser_config>" << endl;
	 exit(0);
       }
	const char* header_model = argv[++arg];
	const char* footer_model = argv[++arg];
	const char* header_extractor_model = argv[++arg];
	const char* header_extractor_config = argv[++arg];
	const char* footer_extractor_model = argv[++arg];
	const char* footer_extractor_config = argv[++arg];
	const char* parser_config = argv[++arg];

	HeaderParser parser(header_model, 
			    footer_model, 
			    header_extractor_model, 
			    header_extractor_config, 
			    footer_extractor_model, 
			    footer_extractor_config, 
			    parser_config);
	
	if (!strcmp(command, "-header"))
	  parser.parseHeader(cin);
	else if (!strcmp(command, "-footer"))
	  parser.parseFooter(cin, 0);
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
