BOOST_ROOT = /usr/include/
PREMIA_ROOT = ../..
KARRIGELL_WWW = ../../../Karrigell-3.1.1/www

PYTHON_DIR=`python2.6 -c 'import sys; print sys.prefix'`
PYTHON_INCLUDE=$(PYTHON_DIR)/include/python2.6
PYTHON_LIB=$(PYTHON_DIR)/lib



I_BOOST =  -I $(BOOST_ROOT) \
           -I $(PYTHON_INCLUDE)

# for the moment i don't know how to tell the library loader to look into this directory
# so all boost libraries should be put also to LD_LIBRARY_PATH (e.g. /usr/lib)           
L_BOOST = -L/opt/local/lib \
		-L$(PYTHON_LIB) -lpython2.6 -lstdc++ -lboost_system


I_PREMIA = -I $(PREMIA_ROOT)/include \
           -I $(PREMIA_ROOT)/bindings/include

L_PREMIA = -lpremia -lpnl \
           -L$(PREMIA_ROOT)/lib

all: python karrigell fsharp
install-all: install-python install-karrigell install-fsharp

python: pypremia.so pygen
karrigell: python ksgen
fsharp: fspremia.so fsgen

pypremia.lo: pypremia.cpp
	libtool --mode=compile gcc -g -O -c $(I_PREMIA) -I$(PYTHON_INCLUDE) pypremia.cpp

pypremia.so: pypremia.lo
	libtool --mode=link gcc --shared -module -lboost_python -lpython2.6 -o pypremia.so pypremia.lo ../../Src/libpremia.la

pygen.lo: pygen.cpp
	libtool --mode=compile gcc -g -O -c $(I_PREMIA) pygen.cpp

pygen: pygen.lo
	libtool --mode=link gcc -O -o pygen pygen.lo -lboost_filesystem -lboost_program_options ../../Src/libpremia.la
	
ksgen.lo: ksgen.cpp
	libtool --mode=compile gcc -g -O -c $(I_PREMIA) ksgen.cpp

ksgen: ksgen.lo
	libtool --mode=link gcc -O -o ksgen ksgen.lo -lboost_filesystem -lboost_program_options ../../Src/libpremia.la

fspremia.lo: fspremia.cpp
	libtool --mode=compile gcc -g -O -c $(I_PREMIA) -DPREMIADLL_API= fspremia.cpp

fspremia.so: fspremia.lo
	libtool --mode=link gcc --shared -module -o fspremia.so fspremia.lo ../../Src/libpremia.la

fsgen.lo: fsgen.cpp
	libtool --mode=compile gcc -g -O -c $(I_PREMIA) fsgen.cpp

fsgen: fsgen.lo
	libtool --mode=link gcc -O -o fsgen fsgen.lo -lboost_filesystem -lboost_program_options ../../Src/libpremia.la

install-python: python
	./pygen --output-dir=../python/premia --dll-dir=.libs

install-karrigell: install-python ksgen 
	./ksgen --output-dir=$(KARRIGELL_WWW)

install-fsharp: fsharp
	./fsgen --output-dir=../fsharp/premia

clean-karrigell:
	rm -f ksgen.lo

re-karrigell: clean-karrigell ksgen
	./ksgen --output-dir=$(KARRIGELL_WWW)
	
clean: 
	rm -f *.o
	rm -f *.so
	rm -f *.lo
	rm -f pygen
	rm -f ksgen
	rm -f fsgen 

