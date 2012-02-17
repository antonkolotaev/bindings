WWW=
BOOST_CPPFLAGS=@BOOST_CPPFLAGS@
BOOST_LDFLAGS=@BOOST_LDFLAGS@

PYTHON_DIR=`python2.7 -c 'import sys; print sys.prefix'`
PYTHON_INCLUDE=$(PYTHON_DIR)/include/python2.7
PYTHON_LIB=$(PYTHON_DIR)/lib

include $(top_srcdir)/Make.incl
AM_CPPFLAGS=$(PNL_INCLUDES) $(PREMIA_MINIMUM_INCLUDES)

lib_LTLIBRARIES=libpypremia.la libfspremia.la 

libpypremia_la_SOURCES=pypremia.cpp
libpypremia_la_LDFLAGS=-avoid-version -L$(PYTHON_LIB) $(BOOST_LDFLAGS) -lboost_python -lpython2.7 -lboost_system $(AM_LDFLAGS)
libpypremia_la_CXXFLAGS=-I$(includedir) $(BOOST_CPPFLAGS) -I$(PYTHON_INCLUDE) -I../include $(AM_CXXFLAGS)
libpypremia_la_LIBADD=$(libdir)/libpremia.la $(libdir)/libpnl.la

libfspremia_la_SOURCES=fspremia.cpp
libfspremia_la_LDFLAGS=-avoid-version  $(AM_LDFLAGS)
libfspremia_la_CPPFLAGS=-DPREMIADLL_API=
libfspremia_la_CXXFLAGS=-I$(includedir) -I../include $(BOOST_CPPFLAGS) $(AM_CXXFLAGS)
libfspremia_la_LIBADD=$(libdir)/libpremia.la $(libdir)/libpnl.la

noinst_PROGRAMS=pygen ksgen fsgen

pygen_SOURCES=pygen.cpp
pygen_CXXFLAGS=$(BOOST_CPPFLAGS) -I../include $(AM_CXXFLAGS)
pygen_LDFLAGS=-lboost_filesystem -lboost_program_options -lboost_system $(BOOST_LDFLAGS)  $(AM_LDFLAGS)
pygen_LDADD=$(libdir)/libpremia.la $(libdir)/libpnl.la

ksgen_SOURCES=ksgen.cpp
ksgen_CXXFLAGS=$(BOOST_CPPFLAGS) -I../include $(AM_CXXFLAGS)
ksgen_LDFLAGS=-lboost_filesystem -lboost_program_options -lboost_system $(BOOST_LDFLAGS)  $(AM_LDFLAGS)
ksgen_LDADD=$(libdir)/libpremia.la $(libdir)/libpnl.la

fsgen_SOURCES=fsgen.cpp
fsgen_CXXFLAGS=$(BOOST_CPPFLAGS) -I../include $(AM_CXXFLAGS)
fsgen_LDFLAGS=-lboost_filesystem -lboost_program_options -lboost_system $(BOOST_LDFLAGS)  $(AM_LDFLAGS)
fsgen_LDADD=$(libdir)/libpremia.la $(libdir)/libpnl.la

all-install: install install-python install-karrigell

install-python: pygen libpypremia.la
	./pygen --output-dir=../python/premia --dll-dir=.libs
	$(RM) ../python/premia/libpypremia.so
	$(LN_S) "$(libdir)/libpypremia$(SHEXT)"  ../python/premia/libpypremia.so


install-karrigell: install-python ksgen
	if test -z $(WWW); then \
		echo "Installation dir is not defined"; \
		echo "Please call make install-karrigell WWW=path_to_www_dir"; \
	else \
		./ksgen --output-dir=$(WWW) ;\
	fi

install-fsharp: fsgen libfspremia.la
	./fsgen --output-dir=../fsharp/premia

