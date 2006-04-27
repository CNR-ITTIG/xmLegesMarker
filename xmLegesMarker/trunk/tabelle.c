/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		tabelle.c
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
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

		//Se il flag è 1 allora ci sono degli annessi, quindi è necessaria la conversione del testo in iso
		if (flagAnn!=0)
		{
			testoIso = utilConvTextToIso(buffer);

			//Qui alla variabile globale domTextBuffer verrà assegnato il testo nel formato originario
			domInit(contenitore, nodoContenitore, testoIso); // passo il testo convertito in iso
			tablefound=_tabLexStart(testoIso); 

		}
		else
		{
			domInit(contenitore, nodoContenitore,buffer); // non c'è bisogno di convertire il testo 
			tablefound=_tabLexStart(buffer); // qui perchè è già in iso
		}

		domClose();

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
