#include "HeaderParser.h"
#include "../ParserStruttura/tag.h"
#include <fstream>
#include <libgen.h>

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
  if (!parse(header, 0, header_model, header_tags, header_extractor))
    cout << DEFAULT_HEADER << "<?error\n" << header << "\n?>" << endl;
}

void HeaderParser::parseHeader(istream& in) 
{
  string header,buf;
  while(getline(in, buf))
    header += buf + "\n";
  parseHeader(header.c_str());
}

void HeaderParser::parseFooter(const char * footer, int offset) 
{
  if (parse(footer, offset, footer_model, footer_tags, footer_extractor, false))
    closeTag(conclusione);
  else
    cout << footer << DEFAULT_FOOTER;
}

void HeaderParser::parseFooter(istream& in, int offset) 
{
  string footer,buf;
  while(getline(in, buf))
    footer += buf + "\n";  
  parseFooter(footer.c_str(), offset);
}

bool HeaderParser::parse(const char * buffer, 
			 int offset, 
			 const HMM& model, 
			 const hash_map<int,int>& tags,
			 TextSequenceFeatureExtractor& extractor,
			 bool header) 
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
    openTag(intestazione);

  for(int i = 0; i < sequence.size(); i++){
    hash_map<int,int>::const_iterator statetag = tags.find(states[i]);
    if(statetag == tags.end()){
      cerr << "ERROR: state " << states[i] << " has no matching tag" << endl;
      exit(1);
    }
    if(statetag->second != currtag){
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
    if (i == sequence.size()-1){
      if(currtag != -1){
#ifndef HEADERPARSER
	saveTag(currtag, start, offset + strlen(buffer)-1, strbuffer);
#else
	saveTag(currtag, start-offset, strlen(buffer)-1, strbuffer);
#endif
      }
    }
  }
  return true;
}

void HeaderParser::saveTag(int tagvalue,
			   int start,
			   int end,
			   const string& buffer) const 
{
  if(ignoreTag(tagvalue)){
    cout <<  "<?error\n" << buffer.substr(start,end-start+1) << "\n?>" << endl;
    return;
  }
#ifndef HEADERPARSER
  tag * tagstruct = tagInit(tagTipo(tagvalue), start, end, 0.);
  tagMemorizza(tagstruct);
#else
  cout << "<" << tagName(tagvalue) << ">" << endl
       << buffer.substr(start,end-start+1) 
       << "</" << tagName(tagvalue) << ">" << endl;
  if(tagvalue == titolodoc)
    closeTag(intestazione);
  if(tagvalue == formulafinale)
    openTag(conclusione);
#endif
}


bool HeaderParser::ignoreTag(int tagvalue) const
{
  switch(tagTipo(tagvalue)){
  case varie:
  case pubblicazione: return true;
  default: return false;
  }
}

void HeaderParser::openTag(int tagvalue) const
{
#ifdef HEADERPARSER
  cout << "<" << tagName(tagvalue) << ">" << endl;
#endif
}

void HeaderParser::closeTag(int tagvalue) const
{
#ifdef HEADERPARSER
  cout << "</" << tagName(tagvalue) << ">" << endl;
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
  case conclusione: return "conclusione";
  case intestazione: return "intestazione";
  case annessi: return "annessi";
  case varie: return "varie";
  default: return "";
  }
}

#ifdef HEADERPARSER
int main(int argc, char* argv[]) {

  string workdir = dirname(argv[0]);
  string config_files[] = { "header_model",
			   "footer_model",
			   "header_extractor_model",
			   "header_extractor_config",
			   "footer_extractor_model",
			   "footer_extractor_config",
			   "parser_config"};

  if (argc < 2 || !strcmp(argv[1], "--help")){
    cerr << "Usage: " << argv[0] << " [-header,-footer]"
	 << " [ <workdir> <header_model> <footer_model>"
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
	
       HeaderParser parser((workdir + "/" + config_files[0]).c_str(),
			   (workdir + "/" + config_files[1]).c_str(),
			   (workdir + "/" + config_files[2]).c_str(),
			   (workdir + "/" + config_files[3]).c_str(),
			   (workdir + "/" + config_files[4]).c_str(),
			   (workdir + "/" + config_files[5]).c_str(),
			   (workdir + "/" + config_files[6]).c_str());
			   
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
