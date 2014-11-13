#!/usr/bin/python
# -*- coding: utf-8 -*-
from sys import *
import re, string

if len(argv) < 2:
    print('Add name of the file to filter') # 11203194

paren = re.compile('\(.*\)')
punct = re.compile('[^a-zA-ZąęńśźżółćĄĘŃŚŹŻÓŁĆ]+')
short = re.compile(r'\b[a-zA-Z]{1,2}\b')
exten = re.compile(r'\.\w{2,4}\b')
repea = re.compile(r'(\w)\1\1+')
ready = set()

def proc(filename):
    i = 0
    for line in open(argv[1]):
        i += 1
        if(i % 100000 == 0):
            print('\rReading', filename, ':', i/1000000, 'million lines...', file=stderr, end='')
            stderr.flush()
        if '/' in line: continue
        line = paren.sub('', line)
        if exten.search(line): continue
        line = punct.sub(' ', line)
        line = repea.sub('\\1\\1', line)
        line = short.sub('', line)
        line = ' '.join(line.split())
        line = line.strip().lower()
        if len(line) == 0: continue
        #print(line)
        ready.add(line)

    print('\rReading of', filename, 'done:', i, ' lines', file=stderr)
    stderr.flush()

    out = open(filename + '-filtered', 'w')
    n = len(ready)
    i = 0
    for line in sorted(ready):
        i += 1
        if(i % 100000 == 0):
            print('\rWriting output:', i/1000000, 'million lines... (out of', n, ')', file=stderr, end='')
            stderr.flush()
        print(line, file=out, end=' ')
    out.close()

    print('\rProcessing', filename+'-filtered', ' done:', n, 'lines', file=stderr)
    stderr.flush()

for fname in argv[1:]:
    proc(fname)
