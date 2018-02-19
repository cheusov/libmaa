# Makefile -- Makefile for libmaa documentation
# Created: Sun Jan  8 21:44:38 1995 by faith@cs.unc.edu
# Revised: Wed May 21 00:56:52 1997 by faith@acm.org
#          Tue Nov 14 22:28:59 2017 by vle@gmx.net
# Copyright 1995, 1996, 1997 Rickard E. Faith (faith@acm.org)
# Copyright 2017 Aleksey Cheusov (vle@gmx.net)
# 
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
# 
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

.SUFFIXES: .bib .bbl

LATEX ?=	latex
PERL ?=		perl
FIG2DEV ?=	fig2dev
DVIPS ?=	dvips
REFBIBTEX ?=	refbibtex
BIBTEX ?=	bibtex

libmaa_sources = \
	maa.h maa.c arg.c base26.c base64.c \
	bit.c debug.c error.c flags.c hash.c list.c \
	memory.c parse-concrete.c pr.c prime.c \
	set.c sl.c source.c stack.c string.c text.c \
	timer.c
LIBMAA_SOURCES =	${libmaa_sources:S|^|../maa/|g}

FIGS =
SLIDEFIGS =

TEMPORARIES =	general.tex

libmaa.dvi: libmaa.tex ${TEMPORARIES} ${FIGS}

all: libmaa.dvi

libmaa.bib:
	if ${REFBIBTEX} -j libmaa.tex ${TEMPORARIES} > $@.tmp; then \
		touch $@; \
	fi
	cmp -s $@ $@.tmp || mv $@.tmp $@
	-rm -f $@.tmp

general.tex: ${LIBMAA_SOURCES}
	${PERL} extract.pl ${LIBMAA_SOURCES} > general.tex

.dvi.ps:
	if fgrep 'LANDSCAPE MODE' ${<:H}.log > /dev/null; then \
		${DVIPS} -t landscape -o $@ ${<:H}; \
	else \
		${DVIPS} -o $@ ${<:H}; \
	fi

.tex.stamp:
	test -f $*.stamp || cp $*.aux $*.stamp || touch $*.stamp
	${LATEX} $*
	cmp -s $*.stamp $*.aux || cp $*.aux $*.stamp

libmaa.dvi: libmaa.bbl libmaa.tex
	if fgrep 'LaTeX Warning: Label' libmaa.log>/dev/null; then ${LATEX} libmaa.tex; fi
	if fgrep 'LaTeX Warning: Label' libmaa.log>/dev/null; then ${LATEX} libmaa.tex; fi
	if fgrep 'LaTeX Warning: Label' libmaa.log>/dev/null; then ${LATEX} libmaa.tex; fi

.bib.bbl:
	${BIBTEX} $<
