# Makefile for the DoSgen project
NAME = dosgen
VERSION	    = $(call _get_tag,VERSION)
RELEASE	    = $(call _get_tag,RELEASE)
_get_tag    = $(shell rpm -q --queryformat "%{$1}" --specfile $(NAME).spec | head -1)

WORKDIR	    = $(shell pwd)
SOURCEDIR   = $(WORKDIR)/$(RPMBUILD)/SOURCES
SRCRPMDIR   = $(WORKDIR)/$(RPMBUILD)/SRPMS
BUILDDIR    = $(WORKDIR)/$(RPMBUILD)/BUILD
RPMDIR	    = $(WORKDIR)/$(RPMBUILD)/RPMS

RPMBUILD    = rpmbuild
RPMBUILDCMD = $(RPMBUILD) --define "_sourcedir $(SOURCEDIR)" --define "_srcrpmdir $(SRCRPMDIR)" --define "_builddir $(BUILDDIR)" --define "_rpmdir $(RPMDIR)" --define "_build_name_fmt %{NAME}-%{VERSION}-%{RELEASE}.%{ARCH}.rpm"

DISTDIR     = $(NAME)-$(VERSION)
TARBALL     = $(DISTDIR).tar.gz

.PHONY: build install rpm test-rpm test-srpm tar devel release clean


CODE_DIRS   = dosgen # init-script dosgen-web
MAN_DIRS    = man

all :: all-code all-man


%-code : Makefile
	@$(foreach d,$(CODE_DIRS), $(MAKE) -C $(d) $* || exit 1; )

%-man : Makefile
	@$(foreach d,$(MAN_DIRS), $(MAKE) -C $(d) $* || exit 1; )

install :: install-code install-man

$(TARBALL): clean
	mkdir $(DISTDIR)
	cp -r $(CODE_DIRS) configure README.md man Makefile dosgen.spec $(DISTDIR)
	tar -czf $(TARBALL) $(DISTDIR)
	$(RM) -r $(DISTDIR)

tar: $(TARBALL)

test-srpm: $(TARBALL)
	mkdir -p $(SOURCEDIR)
	cp $(TARBALL) $(SOURCEDIR) 
	$(RPMBUILDCMD) --nodeps -ts $(TARBALL)

test-rpm: $(TARBALL)
	mkdir -p $(SOURCEDIR)
	cp $(TARBALL) $(SOURCEDIR)
	$(RPMBUILDCMD) --nodeps -ta $(TARBALL)

rpm: $(TARBALL)
	$(RPMBUILD) -ta $(TARBALL)

release: tar test-srpm

clean :: clean-code clean-man
