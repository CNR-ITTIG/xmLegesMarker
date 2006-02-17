#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <libxml/tree.h>

#include <IttigUtil.h>
#include <IttigLogger.h>
#include "config.h"
#include "normalizza.h"
#include "pre.h"
#include "prehtml.h"

#include "annessi.h"	
#include "tag.h"

const char *versione = "1.2";
int visErrore = 0;
char *bufferEnd;
char *namebin = "";
//char _DocNomeStringa[MAXSTRINGA+1] = "Esistente";

//int flagAnnessi = 1;

xmlDocPtr buildEmptyDoc(void);
xmlNodePtr buildEmptyDocMeta(void);
//char *estraiPrimaDiCoda(const char * coda);

void help(void);
void unknown_init(void);

int main(int argc, char *argv[]) 
{
	register int i;
	int c;
	int lo = 0;
	namebin=argv[0];
	//static int flagAnnessi = 1;
	//tipoUscita uscita = doc;
	//tipoDTD dtd = completo;
	
	tipoTesto mtesto = legge;
	configSetDocTesto(mtesto);

	char *file = NULL;
	FILE *fp = NULL;
	char *fileout = NULL;
	FILE *fo = NULL;
	char  *buffer = NULL;
	size_t bufferSize = 1000000;
	int bufferInc = 1000000;
	char *bufferNor = NULL;
	char *bufferPre = NULL;
	char *bufferToPre = NULL;
	//char *encoding = NULL;
	int virgoletteSoppresse = 0;

	opterr = 0;
	while ((c = getopt (argc, argv, "c:d:e:f:hi:l:m:no:p:r:s:t:v:C:H:L:M:R:T:V")) != -1)
		switch (c)
		{			
			case 'm': 
				{ 
					char *tmp=strdup(optarg);					
					if (strstr(tmp,"t"))		configSetDisable(mod_tabelle);
					if (strstr(tmp,"a"))		configSetDisable(mod_annessi);
					free(tmp);
				}
				break;
			case 'p': 
				{ 
				//char *tmp=strdup(optarg);
				}
				break;
			case 'l':	/* ---------------------------------- Visualizza l'intermezzo! */
				{ 
					char *tmp=strdup(optarg);
					if (strstr(tmp,"long")) lo = 1;
					free(tmp);
				}
				break;
			case 'n':	/* ---------------------------------- node count */
					configSetNodeCount(1);
				break;
			case 'c':	/* ---------------------------------- file di configurazione */
				{ 
				char *fc = (char *)strdup(optarg);
				if (!configLeggi(fc))
					fprintf(stderr, "Errore file di configurazione: %s\nUtilizzati parametri di default.\n", fc);
				}
				break;
			case 'd':	/* ---------------------------------- dtd */
				{
				char *tmp = (char *)strdup(optarg);
					if		(!strcmp(tmp, "base"))				configSetDTDTipo(base);
					else if (!strcmp(tmp, "completo"))		configSetDTDTipo(completo);
					else if (!strcmp(tmp, "flessibile"))	configSetDTDTipo(flessibile);
					else if (!strcmp(tmp, "dl"))	configSetDTDTipo(dl);
					else if (!strcmp(tmp, "cnr"))	configSetDTDTipo(cnr);					
					else 
					{
						fprintf(stderr, "Errore tipo di dtd: %s\n", tmp);
						help();
						exit(1);
					}
					free(tmp);
				}
				break;
			case 'e':	/* ---------------------------------- encoding */
				{
					char *tmp = (char *)strdup(optarg);
					if (!strcmp(tmp,"iso"))				configSetEncoding("iso-8859-15");
					else if (!strcmp(tmp,"win"))		configSetEncoding("windows-1252");
					else if (!strcmp(tmp,"utf"))
					{
						configSetEncoding("UTF-8");
						SetFlagUTF8(1);
					}
					else	
						configSetEncoding(tmp);
					free(tmp);
				}
				break;
			case 'f':	/* ---------------------------------- file da analizzare */
				file = (char *)strdup(optarg);
				if (!(fp = fopen(file, "r")))
				{
					fprintf(stderr, "Errore apertura file di ingresso: %s\n", file);
					exit(1);
				}
				break;
			case 'o':	/* ---------------------------------- file di uscita */
				fileout = (char *)strdup(optarg);
				if (!(fo = fopen(fileout, "w")))
				{
					fprintf(stderr, "Errore apertura file di uscita: %s\n", fileout);
					exit(1);
				}
				break;
			case 'h':	/* ---------------------------------- help */
				help();
				exit(0);
			case 'i':	/* ---------------------------------- formato di input */
				{
				char *tmp = (char *)strdup(optarg);
					if (!strcmp(tmp, "txt"))		configSetTipoInput(txt);
					else if (!strcmp(tmp, "html"))		configSetTipoInput(html);
					else if (!strcmp(tmp, "doc"))		configSetTipoInput(doc);
					else if (!strcmp(tmp, "ted"))		configSetTipoInput(ted);
					else 
					{
						fprintf(stderr, "Errore tipo di input: %s\n", tmp);
						help();
						exit(1);
					}
					free(tmp);
				}
				break;
			case 't':	/* ---------------------------------- tipo documento */
				{
				char *tmp = (char *)strdup(optarg);
				/* ---------------------------------------------------------------- documenti con tag specifici */
				if (!strcmp(tmp, "l"))			configSetDocTesto(legge);
				else if (!strcmp(tmp, "lcost"))		configSetDocTesto(leggecostituzionale);
				else if (!strcmp(tmp, "dl"))		configSetDocTesto(decretolegge);
				else if (!strcmp(tmp, "dlgs"))		configSetDocTesto(decretolegislativo);
				else if (!strcmp(tmp, "rd"))		configSetDocTesto(regiodecreto);
				else if (!strcmp(tmp, "dpr"))		configSetDocTesto(decretopresidenterepubblica);
				else if (!strcmp(tmp, "dprNN"))		configSetDocTesto(decretopresidenterepubblicaNN);
				else if (!strcmp(tmp, "dpcm"))		configSetDocTesto(decretopresedenteconsiglioministri);
				else if (!strcmp(tmp, "dpcmNN"))	configSetDocTesto(decretopresedenteconsiglioministriNN);
				else if (!strcmp(tmp, "dm"))		configSetDocTesto(decretoministeriale);
				else if (!strcmp(tmp, "dmNN"))		configSetDocTesto(decretoministerialeNN);
				else if (!strcmp(tmp, "lr"))		configSetDocTesto(leggeregionale);
				else if (!strcmp(tmp, "aut"))		configSetDocTesto(attoauthority);
				else if (!strcmp(tmp, "com"))		configSetDocTesto(comunicato);		// vers. 2
				else if (!strcmp(tmp, "ddl"))		configSetDocTesto(disegnolegge);	// vers. 2
				else if (!strcmp(tmp, "nir"))		configSetDocTesto(documentoNIR);
				else if (!strcmp(tmp, "cnr"))		{ configSetDocTesto(provCNR); configSetDocNome("Provvedimento"); }
				/* ------------------------------------------------------ nomi di documenti senza tag specifici */
				/* ------------------------------------------------ marcatura: <DocumentoNIR nome=Regolamento>  */
				else if (!strcmp(tmp, "reg"))	{configSetDocTesto(documentoNIR); configSetDocNome("Regolamento"); }
				else if (!strcmp(tmp, "circ"))	{configSetDocTesto(documentoNIR); configSetDocNome("Circolare"); }
				else if (!strcmp(tmp, "prov"))	{configSetDocTesto(documentoNIR); configSetDocNome("Provvedimento"); }
				else if (!strcmp(tmp, "rreg"))	{configSetDocTesto(documentoNIR); configSetDocNome("Regolamento Regionale"); }
				// Parametro per individuare automaticamente il tipo di documento:
				else if(!strcmp(tmp,"unknown")) { unknown_init(); configSetDocTesto(unknown); }
				//
				else	{
						fprintf(stderr, "Errore tipo di documento: %s\n", tmp);
						help();
						exit(1);
					}
				free(tmp);
				}
				break;
			case 'T':	/* ---------------------------------- nome documento per documentoNIR */
				{
					char *tmp = (char *)strdup(optarg);
					configSetDocTesto(documentoNIR);
					configSetDocNome(tmp);
					free(tmp);
				}
				break;
			case 'v':	/* ---------------------------------- livello di debug */
				if (!loggerInit(optarg))	help();
				break;
			case 's':	/* ---------------------------------- controllo sequenza */
				{ 
					int l = atoi(optarg);
					if (l == 0 || l == 1)
						configSetSequenzaCheck(l);
					else 
					{
						help();
						exit(1);
					}
				}
				break;
			case 'C':	/* ---------------------------------- tipo di commi (numerati o non) */
				{ 
					int l = atoi(optarg);
					switch (l) 
					{
						case 0: configSetTipoCommi(commiNumerati); break;
						case 1: configSetTipoCommi(commiNNLineeVuote); break;
						case 2: configSetTipoCommi(commiNNSenzaLinea); break;
						default: help(); exit(1);
					}
				}
				break;
			case 'H':
				configSetHeaderParser((char *)strdup(optarg));
				break;
			case 'M':
				configSetHeaderParserModels((char *)strdup(optarg));
				break;
			case 'r':	/* ---------------------------------- rubriche */
				{
				char *tmp = (char *)strdup(optarg);
				if (!strcmp(tmp, "0"))
				{
					configSetRubriche(1);
					configSetRubricaTipo(acapo);
				}
				else if (!strcmp(tmp, "1"))
				{
					configSetRubriche(1);
					configSetRubricaTipo(adiacente); //adiacente con "Art." (sempre)
				}
				else if (!strcmp(tmp, "2"))
				{
					configSetRubriche(1);
					configSetRubricaTipo(deagostini); //adiacente senza "Art."
				}
				else if (!strcmp(tmp, "9"))  // Non c'è rubrica (usato solo nel caso di commi nn)
					configSetRubriche(0);
				
				else 
				{
					help();
					exit(1);
				}
				free(tmp);
				}
				break;
			//case 'R': 
			//	{ /* presenza o meno della rubrica */
			//		char *t = (char *)strdup(optarg);
			//		if (!strcmp(t, "s"))
			//			configSetRubriche(1);
			//		else if (!strcmp(t, "n"))  //Non c'è rubrica
			//			configSetRubriche(0);
			//		else 
			//		{
			//			help();
			//			exit(1);
			//		}
			//		free(t);
			//	}
			//	break;
			case 'V':	/*  ---------------------------------- testo con virgolette soppresse */
				virgoletteSoppresse = 1;
				break;
			case 'L':	//Disattiva output
				if (!loggerSetOutputEnabled(optarg))	help();
				break;
			case '?':
				if (isprint (optopt))
					fprintf (stderr, "Opzione sconosciuta `-%c'.\n", optopt);
				else
					fprintf (stderr, "Opzione sconosciuta `\\x%x'.\n", optopt);
				help();
				exit(1);

			default:
				abort ();
		}		//chiude lo switch iniziale


	if (!file)
		fp = stdin;

	loggerInfo("----------------------------------------------------------");
	configConfigInLog();

	if (configGetDTDTipo() == base && configGetDocStruttura() == docsemiarticolato && configGetDocTestoTipo() != documentoNIR)
	{
		fprintf(stderr, "Conflitto fra DTD (%s) e tipo Documento (%s).\nElaborazione interrotta.\n", configGetDTDTipoStringa(), configGetDocTestoString());
		exit(1);
	}
		
	if (configTipoInput() == doc)		/* ---------------------------------- conversione da DOC a TXT */
	{
		char *awcommand;
		awcommand = (char *) malloc(sizeof(char) * 256);
		if (!file)
			file = "-";
		sprintf(awcommand, "antiword %s > antiword.out", file);
		system(awcommand);
		free(awcommand);
		
		file = (char *)strdup("antiword.out");
			if (!(fp = fopen(file, "r")))
				{
					fprintf(stderr, "Errore apertura file convertito da doc: %s\n", file);
					exit(1);
				}
		configSetTipoInput(txt);
	}
	
	if (file)
		loggerInfo(utilConcatena(2, "ANALISI FILE ", file));
	else
		loggerInfo("ANALISI STDIN");


	buffer = (char *) malloc(sizeof(char) * bufferSize);	/* --------------------------------- lettura input */
	for (i = 0; !feof(fp); i++)
	{
		if (i == bufferSize) 
		{
			bufferSize += bufferInc;
			buffer = (char *) realloc(buffer, sizeof(char) * bufferSize);
		}
		buffer[i] = fgetc(fp);
		if (buffer[i] == EOF)
			break;
	}
	buffer[i] = 0;
	fclose(fp);
	bufferSize = i;

	bufferNor = normalizza(buffer);		/* ---------------------------------- tratta: \n, \r, &, paragrafo */
	
	if (configTipoInput() == html)		/* ---------------------------------- input html */
	{
		bufferToPre = prehtmlAnalizza(bufferNor);
		
		if (lo == 1)
		{
			puts("------------------------------ INIZIO bufferToPre -----------------------------------\n");
			puts(bufferToPre); 
			puts("------------------------------ FINE bufferToPre -------------------------------------\n");
		}
	}
	else	
		bufferToPre = bufferNor;

	//bufferNor = normalizza(bufferToNorm);   
	
	if (configTipoInput() == ted)		/* ---------------------------------- input ted */
		bufferPre = preAnalizza(bufferToPre);
	else 
		bufferPre = bufferToPre;
	
//	bufferPre = preAnalizza(bufferToPre);
	
	if (virgoletteSoppresse)
	{
		puts(bufferPre);
		return 0;
	}

	utilPercCurrSet(5); //Imposta la percentuale di esecuzione al 5%

//--------------------------------------------------------
//					CONVERSIONE FILE

	bufferEnd = bufferPre;

	xmlDocPtr	doc = NULL;
	xmlNodePtr	root = NULL;
	
	doc = xmlNewDoc(BAD_CAST "1.0");

	xmlCreateIntSubset(doc, BAD_CAST "NIR", NULL, BAD_CAST configGetDTDTipoStringa());
	
	root = xmlNewNode(NULL, BAD_CAST "NIR");

	/* Imposta tutti gli attributi del tag <NIR> */
	int tdoc = 0; //variabile che tiene conto del tipo di documento
	if(configGetDocTestoTipo() == disegnolegge)
		tdoc=1;
	if(tdoc==0)
		xmlNewProp(root, BAD_CAST "tipo", BAD_CAST "originale");
//	xmlNewProp(root, BAD_CAST "xmlns", BAD_CAST "http://www.normeinrete.it/nir/1.1");
	xmlNewProp(root, BAD_CAST "xmlns:h", BAD_CAST "http://www.w3.org/HTML/1998/html4");
	xmlNewProp(root, BAD_CAST "xmlns:xlink", BAD_CAST "http://www.w3.org/1999/xlink");

	xmlDocSetRootElement(doc, root);

	AnnessiAnalizza(bufferEnd, root, nir);

	//PostProcessing (Inserimento Attributo ID, conversione TagErrore in PI)
	xxxAttributeIDUpdate(root,NULL,NULL);
	utilErrore2ProcessingInstruction(root);

	if(configGetNodeCount())	utilNodeCount(root);

	if (!fileout) fileout = "-";
	xmlSaveFormatFileEnc(fileout, doc, (const char*)configEncoding(), 1);

	/*
	//Si può salvare il doc anche tramite xmlDocDump:
	int msize = 4096;
	xmlChar *mem;
	xmlDocDumpFormatMemoryEnc(doc, &mem, &msize, (const char*)configEncoding(), 1);
	if (fo == NULL)
		fprintf(stdout,"%s",mem);
	else
		fprintf(fo,"%s",mem);
	*/
		
	utilPercCurrSet(100);
	
	xmlFreeDoc(doc);

	return 0;
}

/******************************************************************************/
/******************************************************************** HELP ****/
/******************************************************************************/

void help(void) 
{
	printf("\n%s [opzioni] -f file\n", namebin);
	puts("");
	puts("Software sviluppato dall'ITTIG/CNR per conto del progetto \"Norme in Rete\".");
	puts("");
	puts("Opzioni:");
	puts("-c file      : file di configurazione");
	puts("-d dtd       : tipo di dtd (base,completo,flessibile,dl,cnr)");
	puts("-e encoding  : set di caratteri del file di origine");
	puts("               alcune abbreviazioni:");
	puts("               'iso'=iso-8859-15, 'utf'=UTF-8, 'win'=windows-1252 ");
	puts("-f file      : file da analizzare");
	puts("-o file      : file di uscita");
	puts("-i input     : formato del file di input");
	puts("               'txt'=testo piatto, 'html'=testo formattato in html");
	puts("               'ted'=formato proprietario TED");
	puts("-n           : attiva il conteggio dei nodi ");
	puts("-s seq       : controllo sequenza (0/1)");
	puts("-C n         : struttura del comma");
	puts("               0=numerati");
	puts("               1=non numerati con almeno una linea vuota separatrice");
	puts("               2=non numerati senza linea vuota separatrice");
	puts("-r n         : tipo rubriche (0/1/9)");
	puts("               0=rubrica a capo");
	puts("               1=rubrica adiacente (con separatore : [ -(])");
	puts("               2=rubrica adiacente senza \"Art.\" (con separatore : [ -(])");	
	puts("               9=testo senza rubrica (nel caso di commi non numerati)");
	puts("-p []        : disattiva i patterns indicati");
	puts("-m [at]      : disattiva i moduli indicati (a = annessi, t =tabella)");
//	puts("-R s/n       : testo con rubriche (se commi non numerati)");
	puts("-t tipo      : tipo di documento analizzato");
	puts("               l=legge                     lcost=legge costituzionale");
	puts("               dl=decreto legge            dlgs=decreto legislativo");
	puts("               lr=legge regionale          rd=regio decreto");
	puts("               dm=decreto ministeriale     dmNN=decreto ministeriale - non numerato");
	puts("               dpr=decreto del Presidente della Repubblica");
	puts("               dprNN=decreto del Presidente della Repubblica - non numerato");
	puts("               dpcm=decreto del Presidente del Consiglio dei Ministri");
	puts("               dpcmNN=decreto del Presidente del Consiglio dei Ministri - non numerato");
	puts("               aut=atto di Authority       com=comunicato");
	puts("               ddl=disegno di legge        nir=documento NIR");
	puts("               reg=regolamento             circ=circolare");
	puts("               rreg=regolamento regionale  prov=provvedimento");
	puts("               cnr=provvedimento CNR");
	puts("-T nomeTipo  : nome tipo di atto da analizzare; valido solo per -t nir");
	puts("-M <dir>     : directory dei modelli per scansione testa e coda");
	puts("-v           : livello di log: error, warn, info, debug");
	puts("-L stderr|   : attiva i log indicati(ripetibile)");
	puts("   file=nome   Esempio1: -L stderr");
	puts("               Esempio2: -L stderr -L file=ParserStruttura.log");
	puts("-V           : visualizzazione del testo con le virgolette soppresse (nessuna conversione)");
	//puts("-l long      : visualizza intermezzo html");
	puts("");
	printf("Versione %s \n",versione);
	exit(1);
}

//Inizializza (crea o cancella il contenuto) il file temporaneo contenente
//il tipoDoc per la funzione 'Importa' di xmLeges.
void unknown_init(void) {
	FILE *fo;
	if(!(fo = fopen("temp/unknown_type.tmp", "w"))) {
		fprintf(stderr,"Errore apertura file \"temp/unknown_type.tmp\"\n");
		exit(1);
	}
	fclose(fo);	
}