#!/bin/bash

# I want to continue evaluating the differences between these files:
#
# ./clean_wordlist.sh
# diff --side-by-side wordlist.final words.final | grep -v q > results
# vi results

cat wordlist.original | \
  grep -v '[^a-z]' | \
  grep -v '[èéâóû]' | \
  grep -v '^[a-z]$' > wordlist.temp

cat << EndOfMessage >> wordlist.temp
abbe
abbes
adios
attache
attaches
blase
boutonniere
boutonnieres
cafe
cafes
canape
canapes
chateau
chateaux
chatelaine
chatelaines
cliche
cliched
cliches
clientele
clienteles
confrere
confreres
consomme
cortege
corteges
creche
creches
crouton
croutons
crudites
debutante
debutantes
decollete
derailleur
derailleurs
derriere
derrieres
detente
divorcee
divorcees
eclair
eclairs
eclat
elan
emigre
emigres
entree
entrees
epee
epees
etude
etudes
fiance
fiancee
fiances
fiancees
EndOfMessage

cat wordlist.temp | grep -v '^ *$' | sort | uniq > wordlist.final
rm -f wordlist.temp
