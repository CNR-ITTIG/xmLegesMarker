#include "HeaderParser.h"
#include "../ParserStruttura/tag.h"
#include <fstream>
#include <assert.h>
#include "Default.h"

using namespace std;

Month MonthMapping;

HeaderParser::HeaderParser(std::string modeldir,
			   std::string header_intestazione_model_file,
			   std::string header_pubblicazione_model_file,
			   std::string header_formulainiziale_model_file,
			   std::string footer_formulafinale_model_file,
			   std::string footer_dataeluogo_model_file,
			   std::string footer_sottoscrizioni_model_file,
			   std::string footer_annessi_model_file,
			   std::string header_extractor_model_file,
			   std::string header_extractor_config_file,
			   std::string footer_extractor_model_file,
			   std::string footer_extractor_config_file,
			   std::string parser_config_file)
{
  ifstream in((modeldir + "/" + header_intestazione_model_file).c_str());
  if(in.good()){
    in >> header_intestazione_model;
    in.close();
  }
  else{
    istringstream in_s(header_intestazione_model_default);
    in_s >> header_intestazione_model;
  }

  ifstream in8((modeldir + "/" + header_pubblicazione_model_file).c_str());
  if(in8.good()){
    in8 >> header_pubblicazione_model;
    in8.close();
  }
  else{
    istringstream in_s(header_pubblicazione_model_default);
    in_s >> header_pubblicazione_model;
  }

  ifstream in9((modeldir + "/" + header_formulainiziale_model_file).c_str());
  if(in9.good()){
    in9 >> header_formulainiziale_model;
    in9.close();
  }
  else{
    istringstream in_s(header_formulainiziale_model_default);
    in_s >> header_formulainiziale_model;
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

  ifstream in10((modeldir + "/" + footer_annessi_model_file).c_str());
  if(in10.good()){
    in10 >> footer_annessi_model;
    in10.close();
  }
  else{
    istringstream in_s(footer_annessi_model_default);
    in_s >> footer_annessi_model;
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
  int state,tag,open;
  string buf;
  in >> buf >> m_ted >> ws;
  if(buf != "TED"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  if(!getline(in,buf) || buf != "HEADER"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  if(!getline(in,buf) || buf != "INTESTAZIONE"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  while(getline(in,buf) && buf != ""){
    istringstream is(buf);
    is >> state >> tag >> open >> ws;
    header_intestazione_tags[state] = make_pair(tag,open);
  }
  if(!getline(in,buf) || buf != "PUBBLICAZIONE"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  while(getline(in,buf) && buf != ""){
    istringstream is(buf);
    is >> state >> tag >> open >> ws;
    header_pubblicazione_tags[state] = make_pair(tag,open);
  }
  if(!getline(in,buf) || buf != "FORMULAINIZIALE"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  while(getline(in,buf) && buf != ""){
    istringstream is(buf);
    is >> state >> tag >> open >> ws;
    header_formulainiziale_tags[state] = make_pair(tag,open);
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
  if(!getline(in,buf) || buf != "ANNESSI"){
    cerr << "ERROR in reading parser config file" << endl;
    exit(1);
  }
  while(getline(in,buf) && buf != ""){
    istringstream is(buf);
    is >> state >> tag >> open >> ws;
    footer_annessi_tags[state] = make_pair(tag,open);
  }
}

int HeaderParser::parseHeader(istream& in, ostream& out) 
{
  string header,buf;
  while(getline(in, buf))
    header += buf + "\n";
  return parseHeader(header, out);
}

string HeaderParser::removeTeds(const std::string& header, vector<string>& teds)
{
  if(!m_ted)
    return header;
  string buf = "";
  int end = 0,beg = 0;
  while((beg = header.find("||",end)) != string::npos){
    buf += header.substr(end, beg - end);
    end =  header.find("||",beg+1);
    teds.push_back(header.substr(beg,end-beg+2));
    end+=2;
  }
  buf += header.substr(end);
  return buf;
}

int HeaderParser::parseHeader(const std::string& header, ostream& out) 
{
  vector<string> teds;
  string strbuffer = removeTeds(header, teds);
  SqueezeWords(strbuffer);
  const char * buffer = strbuffer.c_str();

  bool found = false;
  unsigned int last = 0, first = 0, offset = 0;
  int * states = NULL, * pub_states = NULL;
  int notes = 1, curr_ted = 0;

  // extract terms
  vector<int> sequence, offsets;
  if(!header_extractor.buildExample(sequence, offsets, buffer, strlen(buffer), 0)){
    defaultHeader(header, out);	
    return notes;
  }

  // parse intestazione
  if(sequence.size() > 0){
    states = new int[sequence.size()];
    header_intestazione_model.viterbiPath(sequence, states, sequence.size());
    if ((first = getFirstMatchingState(states, sequence.size(), header_intestazione_tags)) < sequence.size()){
      found = true;
      // recover eventual pubblicazione before intestazione
      if(first > 0){
	pub_states = new int[first];
	vector<int> pub_sequence;
	copyElements(sequence, pub_sequence, 0, first-1);
	header_pubblicazione_model.viterbiPath(pub_sequence, pub_states, pub_sequence.size());
	savePubblicazione(strbuffer, pub_states, pub_sequence.size(), offsets, offset, header_pubblicazione_tags);
	if(hasCorrectStates(pub_states, pub_sequence.size()))
	  last = saveTags(strbuffer, teds, &curr_ted, pub_states, pub_sequence.size(), offsets, offset, last, out, header_pubblicazione_tags, &notes);     
	if(last < first-1)
	  saveTag(sconosciuto, offsets[last], offsets[first], strbuffer, teds, &curr_ted, out); 	
	last = first;
	delete[] pub_states;
      }
      openTag(intestazione, out);
      last = saveTags(strbuffer, teds, &curr_ted, states, sequence.size(), offsets, offset, last, out, header_intestazione_tags);      
      removeProcessedElements(sequence, last);
      offset += last + 1;
    }
    delete[] states;
  }

  // parse formulainiziale
  if(sequence.size() > 0){
    last = 0;
    states = new int[sequence.size()];
    header_formulainiziale_model.viterbiPath(sequence, states, sequence.size());
    if ((first = getFirstMatchingState(states, sequence.size(), header_intestazione_tags)) < sequence.size()){
      // recover eventual pubblicazione before formulainiziale
      if(first > 0){
	pub_states = new int[first];
	vector<int> pub_sequence;
	copyElements(sequence, pub_sequence, 0, first-1);
	header_pubblicazione_model.viterbiPath(pub_sequence, pub_states, pub_sequence.size());
	savePubblicazione(strbuffer, pub_states, pub_sequence.size(), offsets, offset, header_pubblicazione_tags);
	last = saveTitle(strbuffer, teds, &curr_ted, pub_states, pub_sequence.size(), offsets, offset, last, out, found, header_pubblicazione_tags, &notes);
	last++;
	delete[] pub_states;
      }
      else{
	if(!found)
	  out << DEFAULT_INTESTAZIONE << endl;
	else{
	  openTag(titolodoc,out);
	  closeTag(titolodoc,out);
	}	  
      }
      openTag(formulainiziale,out);
      last = saveTags(strbuffer, teds, &curr_ted, states, sequence.size(), offsets, offset, last, out, header_formulainiziale_tags);      
      closeTag(formulainiziale,out);
      removeProcessedElements(sequence, last);
      offset += last + 1;
      found = true;
    }
    else{ // search pubblicazione, otherwise put all in titolodoc
      header_pubblicazione_model.viterbiPath(sequence, states, sequence.size());
      savePubblicazione(strbuffer, states, sequence.size(), offsets, offset, header_pubblicazione_tags);
      last = saveTitle(strbuffer, teds, &curr_ted, states, sequence.size(), offsets, offset, last, out, found, header_pubblicazione_tags, &notes);
      openTag(formulainiziale,out);
      closeTag(formulainiziale,out);
      removeProcessedElements(sequence, last);
      offset += last + 1;
      found = true;
    }
    delete[] states;
  }

  // if nothing found print default header
  if(!found){
    defaultHeader(header, out);	
    return notes;
  }
  
  // put remains in error tag
  if (offset < offsets.size())
    saveTag(sconosciuto, offsets[offset], strbuffer.length(), strbuffer, teds, &curr_ted, out); 

  return notes;
}

unsigned int HeaderParser::saveTitle(const string& strbuffer, 
				     vector<string>& teds,
				     int * curr_ted,
				     int * states, 
				     unsigned int statesnumber,
				     const vector<int>& offsets, 
				     unsigned int offset, 
				     unsigned int state,
				     ostream& out,
				     bool found,
				     const hash_map<int,pair<int,int> >& tags,
				     int * id) const
{
  unsigned int first = 0, last = 0;
  // if no pubblicazione found
  if ((first = getFirstMatchingState(states, statesnumber, tags)) == statesnumber){
    if (found){ // if tipo previously found, consider text as title
      saveTag(titolodoc, offsets[offset], offsets[offset+first], strbuffer, teds, curr_ted, out); 	
      closeTag(intestazione,out);
    }
    else{
      out << DEFAULT_INTESTAZIONE << endl;
      saveTag(sconosciuto, offsets[offset], offsets[offset+first], strbuffer, teds, curr_ted, out); 	
    }
    return first-1;
  }
  // else choose as title the longer part either before or after match
  last = getLastMatchingState(states, statesnumber, tags);
  if((statesnumber - last) > first){
    saveTags(strbuffer, teds, curr_ted, states, statesnumber, offsets, offset, state, out, tags, id);    
    saveTag(titolodoc,offsets[offset+last+1], offsets[offset+statesnumber], strbuffer, teds, curr_ted, out); 
    closeTag(intestazione,out);
    return statesnumber-1;
  }
  else{
    saveTag(titolodoc,offsets[offset], offsets[offset+first], strbuffer, teds, curr_ted, out); 
    closeTag(intestazione,out);
    return saveTags(strbuffer, teds, curr_ted, states, statesnumber, offsets, offset, first, out, tags, id);    
  }
}

void HeaderParser::savePubblicazione(const string& strbuffer, 
				     int * states, 
				     unsigned int statesnumber,
				     const vector<int>& offsets, 
				     unsigned int offset, 
				     const hash_map<int,pair<int,int> >& tags)

{
  if(!hasCorrectStates(states, statesnumber))
    return;

  int found = 0, maxfound = 3;
  unsigned int trimmed;
  string data, num;
  for (unsigned int i = 0; i < statesnumber && found < maxfound; i++){
    hash_map<int,pair<int,int> >::const_iterator k = tags.find(states[i]);
    assert(k != tags.end());
    if((k->second).first == datapubbl){
      data = normalizeDate(strbuffer.substr(offsets[offset+i],offsets[offset+i+3]-offsets[offset+i]));
      i+=2;
      found++;
    }
    else if((k->second).first == numpubbl){
      num = trimEnd(strbuffer.substr(offsets[offset+i],offsets[offset+i+1]-offsets[offset+i]), &trimmed);
      found++;      
    }
    else if((k->second).first == sopubbl){
      num += "/" + trimEnd(strbuffer.substr(offsets[offset+i],offsets[offset+i+1]-offsets[offset+i]), &trimmed);
      found++;
    }
  }
  //cout << "<pubblicazione tipo=\"GU\" num=\"" << num << "\" norm=\"" << data << "\"/>" << endl;
}

void copyElements(const vector<int>& src, 
		  vector<int>& dst, 
		  unsigned int start, 
		  unsigned int end)
{
  assert(end >= start && end < src.size());
  for(int i = start; i <= end; i++)
    dst.push_back(src[i]);
}

int HeaderParser::parseFooter(istream& in, ostream& out, int notes) 
{
  string footer,buf;
  while(getline(in, buf))
    footer += buf + "\n";  
  return parseFooter(footer.c_str(), out, notes);
}

int HeaderParser::parseFooter(const char * footer, ostream& out, int notes) 
{
  vector<string> teds;
  string strbuffer = removeTeds((string)footer, teds);
  SqueezeWords(strbuffer);
  const char * buffer = strbuffer.c_str();

  bool found = false;
  unsigned int last = 0, offset = 0;
  int * states = NULL, curr_ted = 0;

  // extract terms
  vector<int> sequence, offsets, header_sequence, header_offsets;
  if(!footer_extractor.buildExample(sequence, offsets, buffer, strlen(buffer), 0)){
    defaultFooter(footer, out);	
    return notes;
  }
  header_extractor.buildExample(header_sequence, header_offsets, buffer, strlen(buffer), 0);

  // parse formulafinale
  if(sequence.size() > 0){
    states = new int[sequence.size()];
    footer_formulafinale_model.viterbiPath(sequence, states, sequence.size());
    if (hasCorrectStates(states, sequence.size())){
      if (!found){
	last = saveLastComma(strbuffer, teds, &curr_ted, states, sequence.size(), offsets, offset, out, footer_formulafinale_tags, 
			     header_sequence, header_offsets, &notes);
	found = true;
      }
      last = saveTags(strbuffer, teds, &curr_ted, states, sequence.size(), offsets, offset, last, out, footer_formulafinale_tags);
      removeProcessedElements(sequence, last);
      removeProcessedElements(header_sequence, last);
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
	last = saveLastComma(strbuffer, teds, &curr_ted, states, sequence.size(), offsets, offset, out, footer_dataeluogo_tags,
			     header_sequence, header_offsets, &notes);
	found = true;
	openTag(formulafinale, out);
	closeTag(formulafinale, out);
	openTag(conclusione, out);
      }
      else{ // search for pubblicazione
	last = getFirstMatchingState(states, sequence.size(), footer_dataeluogo_tags); 
	findPubblicazione(strbuffer, teds, &curr_ted, header_sequence, last, header_offsets, offset, out, &notes);
      }
      last = saveTags(strbuffer, teds, &curr_ted, states, sequence.size(), offsets, offset, last, out, footer_dataeluogo_tags);
      removeProcessedElements(sequence, last);
      removeProcessedElements(header_sequence, last);
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
	last = saveLastComma(strbuffer, teds, &curr_ted, states, sequence.size(), offsets, offset, out, footer_sottoscrizioni_tags,
			     header_sequence, header_offsets, &notes);
	found = true;
	openTag(formulafinale, out);
	closeTag(formulafinale, out);
	openTag(conclusione, out);
      }      
      else{ // search for pubblicazione
	last = getFirstMatchingState(states, sequence.size(), footer_sottoscrizioni_tags); 
	findPubblicazione(strbuffer, teds, &curr_ted, header_sequence, last, header_offsets, offset, out, &notes);
      }
      openTag(sottoscrizioni, out);
      last = saveTags(strbuffer, teds, &curr_ted, states, sequence.size(), offsets, offset, last, out, footer_sottoscrizioni_tags);
      found = true;
      if (!hasVisto(states, sequence.size())){
	openTag(visto, out);
	closeTag(visto, out);
      }
      removeProcessedElements(sequence, last);
      removeProcessedElements(header_sequence, last);
      offset += last + 1;
      closeTag(sottoscrizioni, out);
    }
    delete[] states;
  }
  
  // end conclusione
  if(found)
    closeTag(conclusione, out);

  // parse annessi
  if(sequence.size() > 0){
    states = new int[sequence.size()];
    footer_annessi_model.viterbiPath(sequence, states, sequence.size());
    if (hasCorrectStates(states, sequence.size())){
      last = 0;
      if (!found){
	last = saveLastComma(strbuffer, teds, &curr_ted, states, sequence.size(), offsets, offset, out, footer_annessi_tags,
			     header_sequence, header_offsets, &notes);
	found = true;
	out << DEFAULT_FOOTER << endl;
      }
      else{ // search for pubblicazione
	last = getFirstMatchingState(states, sequence.size(), footer_annessi_tags); 
	findPubblicazione(strbuffer, teds, &curr_ted, header_sequence, last, header_offsets, offset, out, &notes);
      }
      last = saveTags(strbuffer, teds, &curr_ted, states, sequence.size(), offsets, offset, last, out, footer_annessi_tags, &notes);
      found = true;
      removeProcessedElements(sequence, last);
      removeProcessedElements(header_sequence, last);
      offset += last + 1;
    }
    delete[] states;
  }

  // if nothing found print default footer
  if(!found){
    defaultFooter(footer, out);	
    return notes;
  }
  
  // put remains (annessi) in error tag
  if (offset < offsets.size())
    saveTag(sconosciuto, offsets[offset], strbuffer.length(), strbuffer, teds, &curr_ted, out); 
  
  return notes;
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
    hash_map<int,pair<int,int> >::const_iterator k = footer_sottoscrizioni_tags.find(states[i]);
    assert(k != footer_sottoscrizioni_tags.end());
    if((k->second).first == visto)
      return true;
  }
  return false;
}

unsigned int HeaderParser::getFirstMatchingState(int * states, 
						 unsigned int statesnumber,
						 const hash_map<int,pair<int,int> >& tags) const
{
  unsigned int state = 0;
  while(state < statesnumber){
    hash_map<int,pair<int,int> >::const_iterator k = tags.find(states[state]);
    assert(k != tags.end());
    if(!errorTag((k->second).first))
      break;
    state++;
  }
  return state;
}

int HeaderParser::getLastMatchingState(int * states, 
				       unsigned int statesnumber,
				       const hash_map<int,pair<int,int> >& tags) const
{
  int state = statesnumber-1;
  while(state >= 0){
    hash_map<int,pair<int,int> >::const_iterator k = tags.find(states[state]);
    assert(k != tags.end());
    if(!errorTag((k->second).first))
      break;
    state--;
  }
  return state;
}

void HeaderParser::findPubblicazione(const string& strbuffer, 
				     vector<string>& teds,
				     int * curr_ted,
				     const vector<int>& sequence, 
				     int first,
				     const vector<int>& offsets, 
				     unsigned int offset, 
				     ostream& out,
				     int * notes)
{
  if (first == 0)
    return;

  int last = 0;
  int * pub_states = new int[first];
  vector<int> pub_sequence;
  copyElements(sequence, pub_sequence, 0, first-1);
  header_pubblicazione_model.viterbiPath(pub_sequence, pub_states, pub_sequence.size());
  savePubblicazione(strbuffer, pub_states, pub_sequence.size(), offsets, offset, header_pubblicazione_tags);
  if(hasCorrectStates(pub_states, pub_sequence.size()))
    last = saveTags(strbuffer, teds, curr_ted, pub_states, pub_sequence.size(), offsets, offset, 0, out, header_pubblicazione_tags, notes);     
  if(last < first-1)
    saveTag(sconosciuto, offsets[offset+last], offsets[offset+first], strbuffer, teds, curr_ted, out); 	
  delete[] pub_states;
}   

unsigned int HeaderParser::saveLastComma(const string& strbuffer, 
					 vector<string>& teds,
					 int * curr_ted,
					 int * states, 
					 unsigned int statesnumber,
					 const vector<int>& offsets, 
					 unsigned int offset, 
					 ostream& out,
					 const hash_map<int,pair<int,int> >& tags,
					 const vector<int>& header_sequence, 
					 const vector<int>& header_offsets, 
					 int * notes) const
{
  int state = getFirstMatchingState(states, statesnumber, tags);
  if(state==0){
    out << "<comma>\n</comma>\n";
    return state;
  }
  // find eventual pubblicazione
  int * pub_states = new int[state];
  vector<int> pub_sequence;
  copyElements(header_sequence, pub_sequence, 0, state-1);
  header_pubblicazione_model.viterbiPath(pub_sequence, pub_states, pub_sequence.size());
  int first = getFirstMatchingState(pub_states, pub_sequence.size(), header_pubblicazione_tags);
  if (first == pub_sequence.size())
    out <<  "<comma>" << addTeds(strbuffer.substr(offsets[offset], offsets[offset+state]-offsets[offset]), teds, curr_ted) << "\n</comma>\n";
  else{
    out <<  "<comma>" << addTeds(strbuffer.substr(offsets[offset], offsets[offset+first]-offsets[offset]), teds, curr_ted) << "\n</comma>\n";
    int last = saveTags(strbuffer, teds, curr_ted, pub_states, pub_sequence.size(), header_offsets, offset, first, out, header_pubblicazione_tags, notes);     
    if(last < state-1)
      saveTag(sconosciuto, offsets[offset+last], offsets[offset+state], strbuffer, teds, curr_ted, out); 	
  }
  delete[] pub_states;
  return state;
}

void HeaderParser::removeProcessedElements(vector<int>& sequence, int last)
{
  for(unsigned int i = last+1; i < sequence.size(); i++)
    sequence[i-last-1] = sequence[i];
  sequence.resize(sequence.size()-last-1);
}

unsigned int HeaderParser::saveTags(const string& strbuffer, 
				    vector<string>& teds,
				    int * curr_ted,
				    int * states, 
				    unsigned int statesnumber,
				    const vector<int>& offsets, 
				    unsigned int offset, 
				    unsigned int initstate,
				    ostream& out,
				    const hash_map<int,pair<int,int> >& tags,
				    int * id) const
{
  hash_map<int,pair<int,int> >::const_iterator statetag,currstatetag;

  // get last matched state
  int last;
  for(last = statesnumber-1; last >= 0; last--){
    statetag = tags.find(states[last]);
    assert(statetag != tags.end());
    if(!errorTag((statetag->second).first))
      break;
  }
 
  // save tags for matching states
  int currtag = -1, start = -1;
  for(int state=initstate; state <= last; state++){
    statetag = tags.find(states[state]);
    assert(statetag != tags.end());
    if((state == initstate || states[state] != states[state-1]) && 
	((statetag->second).second > -2) &&
	((statetag->second).second > 0 || (statetag->second).first != currtag)){
      if(currtag != -1)
	saveTag(currtag, start, offsets[state+offset], strbuffer, teds, curr_ted, out, ((currstatetag->second).second > -1), id);
      currstatetag = statetag;
      currtag = (statetag->second).first;
      start = offsets[state+offset];
    }
    if (state == last){
      if(currtag != -1){
	//if(last < statesnumber-1)
	  saveTag(currtag, start, offsets[state+offset+1], strbuffer, teds, curr_ted, out, ((currstatetag->second).second != -1), id);
	  //else
	  //saveTag(currtag, start, strbuffer.length(), strbuffer, out, ((currstatetag->second).second > -1));
      }
    }
  }
  return last;
}

void  HeaderParser::defaultFooter(std::string footer, ostream& out) const
{
  unsigned int dot = footer.find('.'), ted = 0;
  if(dot != string::npos){  
    if(m_ted)
      dot = footer.find("\n",dot);
    out <<  "<comma>" << footer.substr(0, dot+1) << "\n</comma>\n";
    if (footer.substr(dot+1).find_first_not_of(" \n\t\r") != string::npos)
      out << "<?error\n" << footer.substr(dot+1) << "\n?>\n"; 
  }
  else{
    out << "<?error\n" << footer << "\n?>\n"; 
  }
  out << DEFAULT_FOOTER << endl;
}

void  HeaderParser::defaultHeader(std::string header, ostream& out) const
{
  if (header.find_first_not_of(" \n\t\r") != string::npos)
    out << "<?error\n" << header << "\n?>\n"; 
  out << DEFAULT_INTESTAZIONE << "\n" << DEFAULT_FORMULAINIZIALE << endl;
}

void HeaderParser::saveTag(int tagvalue,
			   int start,
			   int end,
			   const string& buffer,
			   vector<string>& teds,
			   int * curr_ted,
			   ostream& out,
			   bool withtags,
			   int * id) const 
{
  if(errorTag(tagvalue) || noteTag(tagvalue)){
    out <<  "<?error\n" << addTeds(buffer.substr(start,end-start), teds, curr_ted) << "\n?>" << endl;
    return;
  }
  if(ignoreTag(tagvalue)){
    out << addTeds(buffer.substr(start,end-start), teds, curr_ted) << endl;
    return;
  }
  if(withtags)
    openTag(tagvalue, out, buffer.substr(start,end-start), id);
  if(formatTag(tagvalue)){
    if(tagvalue == preambolo)
      out << addSemicolumnFormatTags(buffer.substr(start,end-start), teds, curr_ted);
    else
      out << addFormatTags(buffer.substr(start,end-start), teds, curr_ted);
  }
  else if(tagvalue == formulafinale){
    int last = buffer.find_last_not_of(" \r\n\t", end-1);
    out << "<h:p> " << addTeds(buffer.substr(start,last-start+1), teds, curr_ted) << " </h:p>";
    //out << endl;
    out << addTeds(buffer.substr(last+1, end-last-1), teds, curr_ted);
  }
  else if(trimmedTag(tagvalue) && withtags){
    unsigned int trimmed = 0;
    out << trimEnd(buffer.substr(start,end-start), &trimmed);
    closeTag(tagvalue, out);
    if (trimmed < end-start)
      out << addTeds(buffer.substr(start+trimmed,end-start-trimmed), teds, curr_ted);
    return;
  }
  else
    out << addTeds(buffer.substr(start,end-start), teds, curr_ted);
  if(withtags)
    closeTag(tagvalue, out);
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
  string month = buffer.substr(beg, end - beg);
  if(month.find_first_not_of("0123456789") != string::npos)
    month = MonthMapping[lowercase(month)];
  else if(month.length() == 1)
    month = "0" + month;
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

string trimEnd(const string& buf, unsigned int * trimmed)
{
  *trimmed = buf.find_last_not_of("`';:,.?!\{}[]\\|/<>-_°() \r\t\n") + 1;
  return buf.substr(0,*trimmed);
}

string HeaderParser::addSemicolumnFormatTags(string text, vector<string>& teds, int * curr_ted) const
{
  istringstream in(text);
  string line, out, buf;
  unsigned int end = 0;

  while(getline(in, line)){
    unsigned int semicolumn = line.find(";");
    if(semicolumn != string::npos){
      out += "<h:p> " + buf + line.substr(0,semicolumn+1) + " </h:p>";
      if(m_ted && *curr_ted < teds.size())
	out += teds[(*curr_ted)++];
      out += "\n";
      if(semicolumn < line.length()-1)
	buf = line.substr(semicolumn+1);
      else
	buf = "";
      continue;
    }
    if (line.find_first_not_of(" \n\t\r") != string::npos)
      buf += line;
    if(m_ted && *curr_ted < teds.size())
      (*curr_ted)++;
  }
  if ((end = buf.find_last_not_of(" \n\t\r")) != string::npos)
    out += "<h:p> " + buf.substr(0,end+1) + " </h:p>\n";
  return out;
}

string HeaderParser::addFormatTags(string buf, vector<string>& teds, int * curr_ted) const
{
  istringstream in(buf);
  string line, out;

  while(getline(in, line))
    if (line.find_first_not_of(" \n\t\r") != string::npos){
      out += "<h:p> " + line + " </h:p>";
      if(m_ted && *curr_ted < teds.size())
	out += teds[(*curr_ted)++];
      out += "\n";
    }
  return out;
}

string HeaderParser::addTeds(string buf, vector<string>& teds, int * curr_ted) const
{
  if (!m_ted || teds.size() == 0 )
    return buf;
  
  int beg = 0;//buf.find_first_not_of(" \n\t\r");
  while((beg = buf.find('\n', beg)) != string::npos && *curr_ted < teds.size()){
    string ted = teds[(*curr_ted)++];
    buf.insert(beg, ted);
    beg += ted.size() + 1;
  } 
  return buf;
}


void SqueezeWords(string& buf)
{
  static int numwords = 15;
  static string words[] = {"d i s p o n e", "d e c r e t a", "a d o t t a", "e m a n a", "p r o m u l g a",
			   "D i s p o n e", "D e c r e t a", "A d o t t a", "E m a n a", "P r o m u l g a",
			   "D I S P O N E", "D E C R E T A", "A D O T T A", "E M A N A", "P R O M U L G A"};
  static string squeezed[] = {"dispone", "decreta", "adotta", "emana", "promulga",
			      "Dispone", "Decreta", "Adotta", "Emana", "Promulga",
			      "DISPONE", "DECRETA", "ADOTTA", "EMANA", "PROMULGA"};
  for (int i = 0; i < numwords; i++){
    int beg = 0;
    while((beg = buf.find(words[i],beg)) != string::npos)
      buf.replace(beg,words[i].length(),squeezed[i]);
  }
}

bool HeaderParser::errorTag(int tagvalue) const
{
  switch(tagTipo(tagvalue)){
  case sconosciuto: return true;
  default: return false;
  }
}

bool HeaderParser::trimmedTag(int tagvalue) const
{
  switch(tagTipo(tagvalue)){
  case numdoc:
  case datadoc: return true;
  default: return false;
  }
}

bool HeaderParser::noteTag(int tagvalue) const
{
  switch(tagTipo(tagvalue)){
  case pubblicazione: 
  case nota:
  case registrazione:
  case lavoripreparatori:
  case annessi: return true;
  default: return false;
  }
}

bool HeaderParser::ignoreTag(int tagvalue) const
{
  switch(tagTipo(tagvalue)){
  case varie: return true;
  default: return false;
  }
}

bool HeaderParser::formatTag(int tagvalue) const
{
  switch(tagTipo(tagvalue)){
  case nota:
  case registrazione:
  case lavoripreparatori:
  case annessi:
  case pubblicazione: 
  case formulainiziale:
  case preambolo: return true;
  default: return false;
  }
}

void HeaderParser::openTag(int tagvalue, 
			   ostream& out,
			   const std::string& tagcontent,
			   int * id) const
{
  openContextTags(tagvalue, out);
  out << "<" << tagName(tagvalue) << tagAttributes(tagvalue, tagcontent, id) << ">" << endl;
}

void HeaderParser::closeTag(int tagvalue, ostream& out) const
{
  out << "</" << tagName(tagvalue) << ">" << endl;
  closeContextTags(tagvalue, out);
}

void HeaderParser::openContextTags(int tagvalue, ostream& out) const
{
  switch(tagTipo(tagvalue)){
  case pubblicazione: 
  case nota:
  case registrazione:
  case lavoripreparatori:
  case annessi:
    out << "<inlinemeta><redazionale>" << endl;
    break;
  default:
    return;
  }
}

void HeaderParser::closeContextTags(int tagvalue, ostream& out) const
{
  switch(tagTipo(tagvalue)){
  case pubblicazione: 
  case nota:
  case registrazione:
  case lavoripreparatori:
  case annessi:
    out << "</redazionale></inlinemeta>" << endl;
    break;
  default:
    return;
  }
}

std::string HeaderParser::tagAttributes(int tagvalue, 
					const std::string& tagcontent,
					int * id)
{
  switch(tagTipo(tagvalue)){
  case datadoc:
    return (string)" norm=\"" +  normalizeDate(tagcontent) + "\"";
  case dataeluogo:
    return (string)" norm=\"" +  normalizeDate(tagcontent) + "\" codice=\"00100\"";
  case pubblicazione:
  case nota:
  case registrazione:
  case lavoripreparatori:
  case annessi:{
    ostringstream attr;
    attr << " id=\"n" << (*id)++ << "\"";
    return attr.str();
  }
  default:
    return "";
  }
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
  case pubblicazione: return "nota";
  case emanante: return "emanante";
  case conclusione: return "conclusione";
  case intestazione: return "intestazione";
  case annessi: return "nota";
  case nota: return "nota";
  case registrazione: return "nota";
  case lavoripreparatori: return "nota";
  case varie: return "varie";
  case sottoscrivente: return "sottoscrivente";
  case visto: return "visto";
  case sconosciuto: return "";
  case preambolo: return "preambolo";
  default: return "";
  }
}

#ifdef HEADERPARSER
int main(int argc, char* argv[]) {

  char *r = rindex(argv[0], '/');
  if (r) *r = 0; else r = rindex(argv[0], '\\');
  if (r) *r = 0;
  string workdir = (string)argv[0] + "/Models";
  string config_files[] = { "header_intestazione_model",
			    "header_pubblicazione_model",
			    "header_formulainiziale_model",
			    "footer_formulafinale_model",
			    "footer_dataeluogo_model",
			    "footer_sottoscrizioni_model",
			    "footer_annessi_model",
			    "header_extractor_model",
			    "header_extractor_config",
			    "footer_extractor_model",
			    "footer_extractor_config",
			    "parser_config"};

  if (argc < 2 || !strcmp(argv[1], "--help")){
    cerr << "Usage: " << argv[0] << " [-header,-footer]"
	 << " [ <workdir> <header_intestazione_model> <header_pubblicazione_model>"
	 << " <header_formulainiziale_model> <footer_formulafinale_model>"
	 << " <footer_dataeluogo_model> <footer_sottoscrizioni_model> <footer_annessi_model>"
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
			   config_files[0],
			   config_files[1],
			   config_files[2],
			   config_files[3],
			   config_files[4],
			   config_files[5],
			   config_files[6],
			   config_files[7],
			   config_files[8],
			   config_files[9],
			   config_files[10]);

       if (!strcmp(command, "-header"))
	 parser.parseHeader(cin, cout);
       else if (!strcmp(command, "-footer"))
	 parser.parseFooter(cin, cout, 1);
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





















