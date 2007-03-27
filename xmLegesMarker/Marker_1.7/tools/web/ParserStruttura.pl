#!/usr/bin/perl

use CGI;
use LWP::Simple;	# per download documento da rete
use IPC::Open2;		# per interscambio dati input/output con parser

# --- configurazione
$parser		= "/home/mirco/Progetti/ParserStruttura/ParserStruttura";
$parserConf	= "/home/mirco/Progetti/ParserStruttura/ParserStruttura/conf/ParserStruttura.conf";
$logFile	= "/home/mirco/ParserStruttura-cgi.log";
$tmpDir		= "/tmp";


# --- acquisizione parametri da form
$cgi = new CGI;
$dtd = $cgi->param(dtd);
$tipoDoc = $cgi->param(tipodoc);

$formato	= $cgi->param(formato);
$marcatura	= $cgi->param(marcatura);
$uscita		= $cgi->param(uscita);
$inputTipo	= $cgi->param(inputTipo);
$destinazione	= $cgi->param(destinazione);


# --- gestione tipo di input (tastiera, upload, rete) e lettura dati

if ($inputTipo eq 'tastiera') {
	$text = $cgi->param(inputTastiera);
	$name = 'testo';
} elsif ($inputTipo eq 'upload') {
	$text = '';
	$fn = $cgi->upload('inputFile');
	while (<$fn>) {
		$text .= $_;
	}
	close $fn;
	$name = $cgi->param(inputFile);
} elsif ($inputTipo eq 'rete') {
	$text = '';
	$url = $cgi->param(inputURL);
	$text = get($url);
	$name = $url;
}

if ($name){
	$name =~ s/.*[\/\\]//;		# tolgo fino all'ultima /
	$name =~ s/\..*?$//;		# tolgo dall'ultimo punto alla fine
}

# --- gestione errori
open (LOG, ">>$logFile") or $erroreLog="$!";

if (not $logFile or not $parser or $erroreLog) {
	print $cgi->header;
	print $cgi->start_html("Errore configurazione cgi");
	print $cgi->p("Errore: variabile del CGI parser non impostata.") if not $parser;
	print $cgi->p("Errore: variabile del CGI logFile non impostata.") if not $logFile;
	print $cgi->p("Errore: apertura file di log: $erroreLog.") if $erroreLog;
	print $cgi->end_html;
	exit;
}

# --- gestione file di log
#$urlPartenza=$cgi->referer();
print LOG "#" x 60 . "\n";
print LOG "###INIZIO\n";
print LOG "###CITAZIONE\n$text\n";


# --- preparazione chiamata parser
$fileTmp = "$tmpDir/ParserStruttura-cgi.tmp";
open (TMP, ">$fileTmp") or die ("Errore scrittura file \"$fileTmp\" in  \"$tmpDir\": $!");
print TMP "\n$text";
close (TMP);

# --- chiamata parser
$parserExec = "$parser -c $parserConf -f $fileTmp -d $dtd -t $tipoDoc";
$pid = open(RIS, "$parserExec|");
while (<RIS>) {				# output del parser
	$ris .= $_;
}
close RIS;

unlink $fileTmp;

print LOG "###RISULTATO\n$ris\n";
print LOG "###FINE\n";
close (LOG);

# --- gestione output: browser o disco

if ($destinazione eq 'disco') {
	print "Content-Type: text/nir;\n";
	print "Content-Disposition: attachment; filename=\"$name.xml\"\n\n";
} else {
	print $cgi->header("text/xml");
}

if ($destinazione eq 'browser') {
	print $ris;
} else {
	print $ris;
}
