#!/usr/bin/perl

if(@ARGV < 1){
  $prog = `basename $0`;
  chomp($prog);
  print "\nUsage:\n\t$prog <states_number> < <vocabulary>\n\n";
  exit(1);
}

$states = $ARGV[0];
$symbols = <STDIN>;
chomp($symbols);
$p = 1. / $symbols;
$logp = log($p);

$i = 0;
while(defined($row = <STDIN>)){
  @symbol = split(" ", $row);
  $vocabulary[$i] = $symbol[1];
  $i++;
}

for($s = 0; $s < $states; $s++){
  for($w = 0; $w < $symbols; $w++){
    print "$s $vocabulary[$w] $p $logp\n";
  }
}
