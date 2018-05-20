# Makefile for building DoSgen documentation

MANDIR      = /usr/share/man/man8

INSTALL_MAN = install -m 644
RONN_CMD    = ronn --roff

RONNS   = $(wildcard *.ronn)
MANS    = $(patsubst %.8.ronn,%.8,$(RONNS) )
MANGZS := $(foreach man,$(MANS),$(man).gz)

LC_ALL := en_US.UTF-8
LANG    = $(LC_ALL)
LANGUAGE= $(LANG)
export LC_ALL
export LANG
export LANGUAGE

.PHONY: all clean install

all     :: $(MANS)

%.8 : %.8.ronn
	$(RONN_CMD) $<


install :: $(MANS)
	gzip --force $(MANS)
	mkdir -p $(DESTDIR)/$(MANDIR)
	$(INSTALL_MAN) $(MANGZS) $(DESTDIR)/$(MANDIR)

clean:
	@$(RM) $(MANS)
	@$(RM) *.gz
	@echo Cleaned!
