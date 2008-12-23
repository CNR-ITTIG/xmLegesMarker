/******************************************************************************
* Project:	xmLeges
* Module:	Marker
* File:		normalizza.lex
* Copyright:	ITTIG/CNR - Firenze - Italy (http://www.ittig.cnr.it)
* Licence:	GNU/GPL (http://www.gnu.org/licenses/gpl.html)
* Authors:	Mirco Taddei (m.taddei@ittig.cnr.it)
******************************************************************************/
%{
#include <stdio.h>
#include <string.h>
#include "config.h"
#include <IttigLogger.h>
#include "normalizza.h"

int norwrap() {
	return 1;
}

%}

LEFT		(<)
RIGHT		(>)

%%

[\t\r]			norAppendChars(1, ' ');

.|\n			norAppendString(strdup(nortext));

%%
