# vim: set ft=make:
WWW=
BOOST_CPPFLAGS=@BOOST_CPPFLAGS@
BOOST_LDFLAGS=@BOOST_LDFLAGS@ 
BOOST_FILESYSTEM_LIB=@BOOST_FILESYSTEM_LIB@
BOOST_SYSTEM_LIB=@BOOST_SYSTEM_LIB@
BOOST_PROGRAM_OPTIONS_LIB=@BOOST_PROGRAM_OPTIONS_LIB@
BOOST_PYTHON_LIB=@BOOST_PYTHON_LIB@
PYTHON_CPPFLAGS=@PYTHON_CPPFLAGS@
PYTHON_LDFLAGS=@PYTHON_LDFLAGS@
PREMIA_CPPFLAGS=@PREMIA_CPPFLAGS@
# Clean unused defs
DEFS=

lib_LTLIBRARIES=libpypremia.la libfspremia.la 

libpypremia_la_SOURCES=pypremia.cpp
libpypremia_la_LDFLAGS=-avoid-version $(PYTHON_LDFLAGS) $(BOOST_LDFLAGS) $(BOOST_PYTHON_LIB) $(BOOST_SYSTEM_LIB) $(PREMIA_LDFLAGS) $(AM_LDFLAGS)
libpypremia_la_CXXFLAGS=$(BOOST_CPPFLAGS) $(PYTHON_CPPFLAGS) -I../include $(PREMIA_CPPFLAGS) $(AM_CXXFLAGS)
libpypremia_la_LIBADD=$(libdir)/libpremia.la

libfspremia_la_SOURCES=fspremia.cpp
libfspremia_la_LDFLAGS=-avoid-version  $(AM_LDFLAGS)
libfspremia_la_CPPFLAGS=-DPREMIADLL_API=
libfspremia_la_CXXFLAGS=-I../include $(BOOST_CPPFLAGS) $(PREMIA_CPPFLAGS) $(AM_CXXFLAGS)
libfspremia_la_LIBADD=$(libdir)/libpremia.la

noinst_PROGRAMS=pygen ksgen fsgen check_help

pygen_SOURCES=pygen.cpp
pygen_CXXFLAGS=$(BOOST_CPPFLAGS) -I../include $(PREMIA_CPPFLAGS) $(AM_CXXFLAGS)
pygen_LDFLAGS=$(BOOST_FILESYSTEM_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_SYSTEM_LIB) $(BOOST_LDFLAGS)  $(AM_LDFLAGS)
pygen_LDADD=$(libdir)/libpremia.la

check_help_SOURCES=check_help.cpp
check_help_CXXFLAGS=$(BOOST_CPPFLAGS) -I../include $(PREMIA_CPPFLAGS) $(AM_CXXFLAGS)
check_help_LDFLAGS=$(BOOST_FILESYSTEM_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_SYSTEM_LIB) $(BOOST_LDFLAGS)  $(AM_LDFLAGS)
check_help_LDADD=$(libdir)/libpremia.la


ksgen_SOURCES=ksgen.cpp
ksgen_CXXFLAGS=$(BOOST_CPPFLAGS) -I../include $(PREMIA_CPPFLAGS) $(AM_CXXFLAGS)
ksgen_LDFLAGS=$(BOOST_FILESYSTEM_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_SYSTEM_LIB) $(BOOST_LDFLAGS)  $(AM_LDFLAGS)
ksgen_LDADD=$(libdir)/libpremia.la

fsgen_SOURCES=fsgen.cpp
fsgen_CXXFLAGS=$(BOOST_CPPFLAGS) -I../include $(PREMIA_CPPFLAGS) $(AM_CXXFLAGS)
fsgen_LDFLAGS=$(BOOST_FILESYSTEM_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_SYSTEM_LIB) $(BOOST_LDFLAGS)  $(AM_LDFLAGS)
fsgen_LDADD=$(libdir)/libpremia.la


install-exec-hook: install-python install-karrigell

install-python: pygen libpypremia.la
	./pygen --output-dir=../python/premia --dll-dir=.libs
	$(RM) ../python/premia/libpypremia.so
	$(LN_S) "$(libdir)/libpypremia$(SHEXT)"  ../python/premia/libpypremia.so


install-karrigell: install-python ksgen
	@if test -z $(WWW); then \
		echo "**********************************************************"; \
		echo "* Installation dir is not defined                        *"; \
		echo "* Please call make install-karrigell WWW=path_to_www_dir *"; \
		echo "**********************************************************"; \
		exit 1; \
	else \
		./ksgen --output-dir=$(WWW) ;\
	fi

install-fsharp: fsgen libfspremia.la
	./fsgen --output-dir=../fsharp/premia

