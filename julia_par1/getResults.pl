#!/usr/bin/perl
use strict;
use warnings;
use 5.010;

my @lines=<>;
my $lines_str = join('',@lines);
my $i = 0;
my $tiempo_medio = 0;

while($lines_str =~ m/Tiempo\s*Conjunto\s*Julia:\s*(\d+(\.\d+)?)\s*milisegundos/gix){
    say "Tiempo $i : $1";
    $tiempo_medio += $1; 
    $i++;
}
$tiempo_medio/=$i if $i > 0;

say "Tiempo medio: $tiempo_medio";
