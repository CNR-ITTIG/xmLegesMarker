#include "HeaderParser.h"
#include "../ParserStruttura/tag.h"
#include <fstream>
#include <assert.h>

using namespace std;

HeaderParser::HeaderParser(std::string modeldir,
			   std::string header_model_file,
			   std::string footer_model_file,
			   std::string header_extractor_model_file,
			   std::string header_extractor_config_file,
			   std::string footer_extractor_model_file,
			   std::string footer_extractor_config_file,
			   std::string parser_config_file)
{
  ifstream in((modeldir + "/" + header_model_file).c_str());
  assert(in.good());
  in >> header_model;
  in.close();

  ifstream in2((modeldir + "/" + footer_model_file).c_str());
  assert(in2.good());
  in2 >> footer_model;
  in2.close();

  ifstream in3((modeldir + "/" + header_extractor_model_file).c_str());
  assert(in3.good());
  ifstream in3_config((modeldir + "/" + header_extractor_config_file).c_str());
  assert(in3_config.good());
  header_extractor(in3_config, in3);
  in3.close();
  in3_config.close();

  ifstream in4((modeldir + "/" + footer_extractor_model_file).c_str());
  assert(in4.good());
  ifstream in4_config((modeldir + "/" + footer_extractor_config_file).c_str());
  assert(in4_config.good());
  footer_extractor(in4_config, in4);
  in4.close();
  in4_config.close();

  ifstream in5_config((modeldir + "/" + parser_config_file).c_str());
  assert(in5_config.good());
  init(in5_config);
  in5_config.close();
}

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

  ifstream in2(footer_model_file);
  assert(in2.good());
  in2 >> footer_model;
  in2.close();

  ifstream in3(header_extractor_model_file);
  assert(in3.good());
  ifstream in3_config(header_extractor_config_file);
  assert(in3_config.good());
  header_extractor(in3_config, in3);
  in3.close();
  in3_config.close();

  ifstream in4(footer_extractor_model_file);
  assert(in4.good());
  ifstream in4_config(footer_extractor_config_file);
  assert(in4_config.good());
  footer_extractor(in4_config, in4);
  in4.close();
  in4_config.close();

  ifstream in5_config(parser_config_file);
  assert(in5_config.good());
  init(in5_config);
  in5_config.close();
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

void HeaderParser::parseFooter(const char * footer, int offset, ostream& out) 
{
  if (parse(footer, offset, footer_model, footer_tags, footer_extractor, false, out))
    closeTag(conclusione, out);
  else	
    defaultFooter(footer, out);	  
}

void  HeaderParser::defaultFooter(std::string footer, ostream& out) const
{
  unsigned int dot = footer.find('.');
  if(dot != string::npos){  
    out <<  "<comma>" << footer.substr(0, dot+1) << "\n</comma>\n";
    if (footer.substr(dot+1).find_first_not_of(" \n\t") != string::npos)
      out << "<?error\n" << footer.substr(dot+1) << "\n?>\n"; 
    out << DEFAULT_FOOTER;
  }
}

void HeaderParser::parseFooter(istream& in, int offset, ostream& out) 
{
  string footer,buf;
  while(getline(in, buf))
    footer += buf + "\n";  
  parseFooter(footer.c_str(), offset, out);
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
	saveTag(currtag, start-offset, offsets[i]-1, strbuffer, out);
      currtag = statetag->second;
      start = offset + offsets[i];
    }
    if (i == sequence.size()-1){
      if(currtag != -1)
	saveTag(currtag, start-offset, strlen(buffer)-1, strbuffer, out);
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
    out <<  "<?error\n" << buffer.substr(start,end-start+1) << "\n?>" << endl;
    return;
  }
  if(ignoreTag(tagvalue)){
    out << buffer.substr(start,end-start+1) << endl;
    return;
  }
  out << "<" << tagName(tagvalue) << ">" << endl;
  if(tagvalue == formulainiziale)
    out << addFormatTags(buffer.substr(start,end-start+1));
  else
    out << buffer.substr(start,end-start+1);
  out << "</" << tagName(tagvalue) << ">" << endl;
  if(tagvalue == titolodoc)
    closeTag(intestazione, out);
  if(tagvalue == formulafinale)
    openTag(conclusione, out);
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
  case varie:
  case pubblicazione: return true;
  default: return false;
  }
}

bool HeaderParser::ignoreTag(int tagvalue) const
{
  switch(tagTipo(tagvalue)){
  case sottoscrizioni: return true;
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
  default: return "";
  }
}

#ifdef HEADERPARSER
int main(int argc, char* argv[]) {

  char *r = rindex(argv[0], '/');
  if (r) *r = 0; else r = rindex(argv[0], '\\');
  if (r) *r = 0;
  string workdir = argv[0];
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
	 parser.parseHeader(cin, cout);
       else if (!strcmp(command, "-footer"))
	 parser.parseFooter(cin, 0, cout);
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
