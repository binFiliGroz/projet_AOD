CC=cc
LATEXC=pdflatex
DOCC=doxygen
CFLAGS=-g -Wall 

REFDIR=.
SRCDIR=$(REFDIR)/src
BINDIR=$(REFDIR)/bin
DOCDIR=$(REFDIR)/doc
REPORTDIR=$(REFDIR)/rapport

LATEXSOURCE=$(wildcard $(REPORTDIR)/*.tex)
CSOURCE=$(wildcard $(SRCDIR)/compileBST.c)
PDF=$(LATEXSOURCE:.tex=.pdf)


all: binary report doc 


$(BINDIR)/compileBST: $(CSOURCE)
	$(CC) $(CFLAGS)  $^ -o $@

%.pdf: $(LATEXSOURCE)
	$(LATEXC) -output-directory $(REPORTDIR) $^ 

$(DOCDIR)/index.html: $(SRCDIR)/Doxyfile $(CSOURCE) 
	$(DOCC) $(SRCDIR)/Doxyfile

binary: $(BINDIR)/compileBST

report: $(PDF) 

doc: $(DOCDIR)/index.html

clean:
	rm -rf $(DOCDIR) $(BINDIR)/* $(REPORTDIR)/*.aux $(REPORTDIR)/*.log  $(REPORTDIR)/rapport.pdf 


.PHONY: all doc binary report 
