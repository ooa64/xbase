## Process this file with automake to produce Makefile.in
##
#	This file is part of the XBase libraries
#    Copyright (C) 1998 Denis Pershin (dyp@inetlab.com)
#
#    This library is free software; you can redistribute it and/or
#    modify it under the terms of the GNU Lesser General Public
#    License as published by the Free Software Foundation; either
#    version 2.1 of the License, or (at your option) any later version.
#
#    This library is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#    Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public
#    License along with this library; if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#    Contact:
#
#      Mail:
#
#        Technology Associates, Inc.
#        XBase Project
#        1455 Deming Way #11
#        Sparks, NV 89434
#        USA
#
#      Email:
#
#        xbase@techass.com
#
#      See our website at:
#
#        linux.techass.com
#

SUBDIRS = @XSUBDIRS@

MAINTAINERCLEANFILES = Makefile.in configure aclocal.m4 libtool

EXTRA_DIST = \
	makefile.g95

bin_SCRIPTS = xbase-config

rpm : dist
	cp xbase-@VERSION@.tar.gz /usr/src/redhat/SOURCES
	cp xbase.spec /usr/src/redhat/SPECS
	(cd /usr/src/redhat/SPECS ; rpm -ba --clean xbase.spec)

docs :
	(cd docs ; make docs)

.PHONY : docs