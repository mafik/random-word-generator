Random Word Generator
=====================

This program will generate random words using a set of samples that you can supply.

## Examples

### Using English Wikipedia titles

    Delviratita Forevelas Fritishop Acticula Lesultan Scinodon
    Cyclectical Elbertoni Righaznin Microrgo Lixfocatimated
    Sirenikitika Victorican Jtobosox Ktimelin Galactoringing Apropali
    Crostenkingofgo Ralpakistaniste Colesandarabled Hampingers
    Zapotecture Nortery Micharli Jmulda Alexandereko Ilybobwith
    Catalawice Financil Kiplica Yprepan Sexpering Albertyphodroma
    
### Using Polish Wikipedia titles

    Jeziorał Normer Inspecja Kronhigh Aechmolew Bechizotokój Siedliwoł
    Kazachowicza Oddziajscy Seegrindust Mirchberg Osteocentrid
    Ngaoliny Szczypospoli Igrzyszkaplica Austrith Farmalezja
    Osielisabeleto Sezzecchiopp Vogelgijscy Sylville Lutoloni Pierrell
    Kooperominx Paramek Przerwijk Dialeksanthagri Gruzyzn Gajdina
    Aromisao Nijmegharbon Tygrysól Grzegu Przewuskich Adriejewie

Usage
-----

1. Write or download file with samples.

        wget http://dumps.wikimedia.org/enwiki/latest/enwiki-latest-all-titles.gz
        gunzip enwiki-latest-all-titles.gz
        
2. Filter it to lowercase and strip any unnessecary interpunction (including new lines).

        python3 filter.py enwiki-latest-all-titles
        
3. Compile `gen.cpp`...

        g++ gen.cpp -std=c++11 -O2

4. ... and run in with sample file as a parameter:

        ./a.out enwiki-latest-all-titles-filtered

Technique
------

This program generates names by sequentially appending characters. When chosing next character it finds longest suffix of already generated name which exists in sample file and has at least two different successors. It then choses the successor randomly.

Algorithm
----

In order to speed up the generation, it sorts all suffixes of the sample file (this might take some time, depending on the size of the input). Chosing next successor is then accomplished using binary search. O(log n) where n is length of sample file.
