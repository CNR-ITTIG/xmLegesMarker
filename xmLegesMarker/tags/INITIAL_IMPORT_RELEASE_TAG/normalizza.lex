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



%%
[\t\r]			norAppendChars(1, ' ');
\xB6			norAppendChars(1, '\n');		// segno di paragrafo

[a-z0-9]+		norAppendString(strdup(nortext));
.|\n			norAppendString(strdup(nortext));

%%

//&			norAppendString("&#38;");	// entità numerica di &: spostato in html
//&quot;		norAppendChars(1, '"');		spostato in html
//&nbsp;		norAppendChars(1, ' ');		spostato in html

//&[a-z]+;		norAppendString(strdup(nortext));	// entità simboliche - spostato in html
//&#[0-9]+;		norAppendString(strdup(nortext));	// entità numeriche - spostato in html
