#include <stdio.h>
#include "tabelle.h"

//extern int flagAnnessi;


xmlNodePtr TabelleAnalizza(  char *buffer)
{
	xmlNodePtr nodoContenitore=NULL;
	char *testoIso = NULL;
	//extern int flagAnnessi;
	int flagAnn = 0;
	int tablefound;


	if (configIsEnabled(mod_tabelle)){
		loggerInfo("INIZIO Tabelle");

		nodoContenitore=xmlNewNode(NULL, BAD_CAST tagTipoToNome(contenitore));

		flagAnn = GetFlagAnnessi();

		//Se il flag � 1 allora ci sono degli annessi, quindi � necessaria la conversione del testo in iso
		if (flagAnn!=0)
		{
			testoIso = utilConvTextToIso(buffer);

			//Qui alla variabile globale xxxTextBuffer verr� assegnato il testo nel formato originario
			xxxInit(contenitore, nodoContenitore, testoIso); // passo il testo convertito in iso
			tablefound=_tabLexStart(testoIso); 

		}
		else
		{
			xxxInit(contenitore, nodoContenitore,buffer); // non c'� bisogno di convertire il testo 
			tablefound=_tabLexStart(buffer); // qui perch� � gi� in iso
		}

		xxxClose();

		if (!tablefound){
			loggerDebug("Nessuna tabella individuata");
			//nessuna tabella individuata
			xmlFree(nodoContenitore);
			nodoContenitore=NULL;
		}
		
		loggerInfo("FINE Tabelle");
	}else
	{
		loggerDebug("Modulo Tabelle Disattivato");
	}

	return nodoContenitore;
}
