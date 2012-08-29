# Overridden during automated builds
COMP_NAME    := hp-ams
COMP_VER     := 1.3.0
COMP_PKG_REV := 666

NAME         := $(COMP_NAME)
VERSION      := $(COMP_VER)
BUILD_NUMBER := $(COMP_PKG_REV)

ARCH ?= $(shell uname -m)
MAKE := make ARCH=${ARCH}
#
RPMSOURCES=$(shell rpm --eval %{_sourcedir})
RPMRPMS=$(shell rpm --eval %{_rpmdir})
#
INSTALL=install
DIRINSTALL=install -d
ifeq "$(RPM_BUILD_ROOT)" ""
PREFIX ?= /usr/local
else
PREFIX ?= /
DESTDIR = $(RPM_BUILD_ROOT)
endif
SBINDIR=$(DESTDIR)$(PREFIX)/sbin
ETCDIR=$(DESTDIR)$(PREFIX)/etc
INITDIR=$(ETCDIR)/init.d
MANDIR:=$(shell if [ -d $(PREFIX)/share/man ] ; then \
                    echo $(DESTDIR)/$(PREFIX)/share/man ; \
                elif [ -d $(PREFIX)/usr/share/man ] ; then \
                    echo $(DESTDIR)/$(PREFIX)/usr/share/man ; \
                else \
                    echo $(DESTDIR)/$(PREFIX)/usr/man; fi )
DISTROLIBS ?= $(shell if [ -f /etc/redhat-release ] ; then \
                     echo "-lrpm"; \
                  elif [ -f /etc/SuSE-release ] ; then \
                     echo "-lrpm"; \
                  elif [ -f /etc/lsb-release ] ; then \
                     echo "-lrt"; \
                  elif [ -f /etc/debian_version ] ; then \
                     echo "-lrt"; \
                  else \
                     echo ""; fi )

#
export SHELL=/bin/bash

NETSNMP_VERSION ?= 5.7.1
NETSNMP = net-snmp-$(NETSNMP_VERSION)
NETSNMP_TARBALL=$(NETSNMP).tar.gz
DOWNLOAD=http://downloads.sourceforge.net/project/net-snmp/net-snmp/$(NETSNMP_VERSION)/$(NETSNMP_TARBALL)

OS = linux
NETSNMPCONFIG = $(NETSNMP)/net-snmp-config
export OS NETSNMP VERSION
NETSNMPVERSIONMIN = $(shell echo $(NETSNMP)| cut -f2 -d\.  )
ifeq "$(NETSNMPVERSIONMIN)" "7"
MIBS="host agentx/subagent mibII"
NOTMIBS="mibII/vacm_vars mibII/vacm_conf target agent_mibs cd_snmp notification notification-log-mib disman/event disman/schedule snmpv3mibs mibII/vacm utilities/execute" 
OPTIONS=--enable-read-only --disable-set-support --disable-agent --disable-privacy --without-openssl
else
MIBS="hardware/fsys"
NOTMIBS="target agent_mibs cd_snmp notification notification-log-mib disman/event disman/schedule snmpv3mibs" 
OPTIONS=""
endif

NETSNMPTAR = $(NETSNMP).tar.gz
CC = gcc
OPT ?= -O2

CFLAGS = -fno-strict-aliasing $(OPT) -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -mtune=generic -DNETSNMP_NO_INLINE -Ulinux -Dlinux=linux 
export CFLAGS

PWD = `pwd`
CPPFLAGS = -I. -I ./include  $(shell  if [ -f $(NETSNMPCONFIG) ] ; then $(NETSNMPCONFIG) --build-includes $(NETSNMP); fi) -I$(NETSNMP)/agent/mibgroup/mibII
BUILDAGENTLIBS = $(shell if [ -f $(NETSNMPCONFIG) ] ; then  $(NETSNMPCONFIG) --agent-libs ; fi)   
BUILDNETSNMPDEPS = $(shell if [ -f $(NETSNMPCONFIG) ] ; then  $(NETSNMPCONFIG) --build-lib-deps $(NETSNMP) ; fi)
BUILDNETSNMPCMD =  $(shell if [ -f $(NETSNMPCONFIG) ] ; then  $(NETSNMPCONFIG) --build-command ;fi)
BUILDLIBS = $(BUILDNETSNMPDEPS)  -lm -lresolv -lcrypt -lnl $(DISTROLIBS)

TARFILE=$(NAME)-$(VERSION)-$(COMP_PKG_REV).tar.gz

OBJS=hpHelper.o 

TARGETS=hpHelper

AMSDIRS = cpqHost cpqNic cpqSe cpqScsi cpqIde common recorder

SUBDIRS = $(NETSNMP) $(AMSDIRS)
 
OBJS2=$(shell for d in $(AMSDIRS); do ${MAKE} -C $$d provides OS=$(OS) DIR=$$d/|grep -v ^make; done)

all: subdirs hpHelper 

net-snmp-patch: net-snmp-patch-stamp
net-snmp-patch-stamp: net-snmp-untar-stamp
	cd $(NETSNMP) ; for i in ../patches/$(NETSNMP)-*.patch ; do \
            if [ -f $$i ] ; then \
	    	echo "patching net-snmp with $$i"; \
	    	patch -p1 < $$i ; \
	    fi; \
	    done  
	cd $(NETSNMP) ; for i in ../patches/$(OS)/$(NETSNMP)-*.patch ; do \
            if [ -f $$i ] ; then \
	    	echo "patching net-snmp with $$i"; \
	    	patch -p1 < $$i ;  \
	    fi \
	    done  
	touch $@

net-snmp-tarball: net-snmp-tarball-stamp
net-snmp-tarball-stamp: 
	wget $(DOWNLOAD)
	touch $@

net-snmp-untar: net-snmp-untar-stamp
net-snmp-untar-stamp: net-snmp-tarball-stamp
	tar xovfz $(NETSNMPTAR) 
	touch $@

net-snmp-configure: net-snmp-configure-stamp
net-snmp-configure-stamp: net-snmp-patch-stamp
	cd $(NETSNMP) ; ./configure \
            --with-cflags="$(OPT) -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4 -mtune=generic -DNETSNMP_NO_INLINE" \
            --with-sys-location=Unknown \
            --with-sys-contact=root@localhost \
            --with-logfile=/var/log/snmpd.log \
            --enable-ipv6 \
            --with-nl \
            --with-persistent-directory=/var/net-snmp \
            --enable-ucd-snmp-compatibility \
            --enable-static \
            --disable-shared \
            --with-pic \
            --disable-embedded-perl \
            --without-perl-modules \
            --with-default-snmp-version=2 \
            --enable-mfd-rewrites \
            --with-out-mib-modules=$(NOTMIBS) \
	    --with-mib-modules=$(MIBS) \
            --with-out-transports="TCPIPv6 UDPIPv6 SSH TCP Alias" \
            --with-transports="HPILO UDP Unix" \
	    --disable-manuals \
	    --disable-applications \
	    --disable-md5 \
	    --disable-scripts \
	    --disable-mib-loading $(OPTIONS)
	touch $@

subdirs: $(SUBDIRS) net-snmp-configure-stamp
	@if test "$(SUBDIRS)" != ""; then \
		it="$(SUBDIRS)" ; \
		for i in $$it ; do \
			echo "making all in `pwd`/$$i"; \
			( cd $$i ; $(MAKE)  OS=$(OS)) ; \
			if test $$? != 0 ; then \
				exit 1 ; \
			fi  \
		done \
	fi

test: hpHelper $(SUBDIRS) net-snmp-configure-stamp
	for d in $(AMSDIRS); do echo $$d ; if [ -d $$d ] ; then ${MAKE} -C $$d test DISTROLIBS=$(DISTROLIBS) NETSNMP=$(NETSNMP) OS=$(OS);fi; done

testHelper: testHelper.o $(BUILDNETSNMPDEPS)
	(CC) -o testHelper testHelper.o $(BUILDLIBS)

hpHelper: $(OBJS) $(OBJS2) $(BUILDNETSNMPDEPS)
	$(CC) -o hpHelper $(OBJS) $(OBJS2) $(BUILDLIBS)

clean:
	rm -f $(TARGETS) $(OBJS) 
	for d in $(SUBDIRS); do echo $$d ; if [ -d $$d ] ; then ${MAKE} -C $$d clean;fi; done
	rm -f  debian/changelog
	echo "Finished cleaning"

distclean: clean
	rm -rf tmp
	rm -rf $(TARFILE)
	rm -rf $(NAME).spec
	rm -rf *-stamp
	rm -rf $(NETSNMP)
	rm -rf debian/build
	rm -rf debian/hp-ams
	echo $(OBJS2)

$(NAME).spec:   $(NAME).spec.in
	sed "s/\%VERSION\%/$(VERSION)/g" < $< > $<.tmp
	sed -i "s/\%RELEASE\%/$(subst %,\%,$(BUILD_NUMBER))/g" $<.tmp
	mv $<.tmp $@

debian/changelog: debian/changelog.in
	sed "s/\%VERSION\%/$(VERSION)/g" < $< > $<.tmp
	sed -i "s/\%RELEASE\%/$(subst %,\%,$(BUILD_NUMBER))/g" $<.tmp
	sed -i "s/\%DATE\%/`date -R`/g" $<.tmp
	mv $<.tmp $@

install: all
	$(DIRINSTALL) -m 755 $(SBINDIR) $(INITDIR)
	$(DIRINSTALL) -m 755 $(MANDIR)/man8
	$(INSTALL) -m 755 ./hpHelper $(SBINDIR)
	$(INSTALL) -m 755 ./hp-ams.sh $(INITDIR)/hp-ams
	$(INSTALL) -m 644 ./doc/hpHelper.8 $(MANDIR)/man8
	gzip -f $(MANDIR)/man8/hpHelper.8


source_tarball: $(TARFILE)
$(TARFILE): debian/changelog $(NAME).spec
	mkdir -p tmp/$(NAME)-$(VERSION)
	tar -c * --exclude '*/.svn' \
	         --exclude tmp \
                 --exclude '*~' \
                 --exclude '#*#' \
                 --exclude $(NAME).spec \
                 --exclude '.*.swp' | (cd tmp/$(NAME)-$(VERSION) && tar x)
	$(MAKE) -C tmp/$(NAME)-$(VERSION) distclean
	$(MAKE) -C tmp/$(NAME)-$(VERSION) \
          COMP_NAME=$(COMP_NAME) COMP_VER=$(COMP_VER) \
          COMP_PKG_REV=$(COMP_PKG_REV) $(NAME).spec
	tar -C tmp -cz $(NAME)-$(VERSION) > $@


provides:
	@echo $(OBJS2)

.PHONY: subdirs $(SUBDIRS) all clean install source_tarball
