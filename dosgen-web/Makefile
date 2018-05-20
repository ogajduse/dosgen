# Makefile for dosgen-web

SYSTEMD_EXIST       := $(shell [[ `systemctl` =~ -\.mount ]] && echo systemd)
SYSVINIT_EXIST      := $(shell [[ -f /etc/init.d/cron && ! -h /etc/init.d/cron ]] && echo sysvinit )

SYSTEMD_UNIT_DIR     = /usr/lib/systemd/system
SERVICE_FILES        = dosgen-web.service

INITV_DIR            = /etc/init.d
INIT_FILES           = dosgen-web.init

ENV_DIR              = /etc/sysconfig
ENV_FILES            = dosgen-web.env

INSTALL_SYSTEMD_UNIT = install -m 0644
INSTALL_INITV_SCRIPT = install -m 0755

.PHONY: install install-env install-npm update clean

all :
	@echo Specify target. Targets available: install install-env install-npm update clean

install-npm :
	npm install

install-env : $(ENV_FILES)
	mkdir -p $(DESTDIR)/$(ENV_DIR)
	install -m 0644 $(ENV_FILES) $(DESTDIR)/$(ENV_DIR)

ifneq ($(SYSTEMD_EXIST),) # systemd
install : install-env 
	mkdir -p $(DESTDIR)/$(SYSTEMD_UNIT_DIR) 
	$(INSTALL_SYSTEMD_UNIT) $(SERVICE_FILES) $(DESTDIR)/$(SYSTEMD_UNIT_DIR)
else # SysVinit
install : install-env
	mkdir -p $(DESTDIR)/$(INITV_DIR)
	$(INSTALL_INITV_SCRIPT) $(INIT_FILES) $(DESTDIR)/$(INITV_DIR)
endif

update : install

clean :
	@rm -rf node_modules
