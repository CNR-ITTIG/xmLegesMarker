%{
#include <stdio.h>
#include <string.h>

#include "config.h"
#include <IttigLogger.h>
#include "pre.h"

int prewrap() {
	return 1;
}

int nvir2 = 1;

// TED	(\|{2}([0-9]\|){4}[0-9]{3}\|[0-9]{3}\|{2})
// TED  es. ||2|0|0|0|100|100||

%}

TED	    (\|{2}([0-9]+\|){6}\|)
CANC	([[/])

VIRGO	(["])
				/* virgolette sx: win e utf-8 */
VIRG2A	([«\x93]|\xe0\x80\x9c)
				/* virgolette dx: win e utf-8 */
VIRG2C	([»\x94]|\xe0\x80\x9d)

%x virgo virg2

%%
{TED}           {if (configTipoInput() != ted) REJECT; 
                preAppendChars(preleng, ' ');}

[a-z0-9]+		preAppendString(strdup(pretext));
.|\n			preAppendString(strdup(pretext));

%%

//{CANC}+			preAppendChars(preleng, ' ');	non si capisce il perchè

//{VIRG2A}		nvir2=1; preAppendString(strdup(pretext)); BEGIN(virg2);
//<virg2>{VIRG2A}		nvir2++; preAppendChars(preleng, '#');
//<virg2>{VIRG2C}		{	nvir2--; 
//				if (nvir2==0)	{preAppendString(strdup(pretext)); BEGIN(0);}
//				else 		{preAppendChars(preleng, '#');}
//			}
//
//{VIRGO}			preAppendString(strdup(pretext)); BEGIN(virgo);
//<virgo>{VIRGO}		preAppendString(strdup(pretext)); BEGIN(0);
//
//<virgo,virg2>.|\n	preAppendChars(preleng, '#');
