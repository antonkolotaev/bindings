BOOST_ROOT = /usr/include/
PREMIA_ROOT = ../..
KARRIGELL_WWW = /usr/local/src/Karrigell-3.1.1/www

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
	libtool --mode=link gcc --shared -lboost_python -lpython2.6 -o pypremia.so pypremia.lo ../../Src/libpremia.la

pygen.lo: pygen.cpp
	libtool --mode=compile gcc -g -O -c $(I_PREMIA) pygen.cpp

pygen: pygen.lo
	libtool --mode=link gcc -O -o pygen pygen.lo -lboost_filesystem -lboost_program_options ../../Src/libpremia.la
	
ksgen: ksgen.cpp
	gcc -O -o ksgen -lboost_filesystem -lboost_program_options $(L_BOOST) $(I_BOOST) $(I_PREMIA) $(L_PREMIA) ksgen.cpp

fspremia.o: fspremia.cpp
	gcc -O -fPIC -c $(I_BOOST) $(I_PREMIA) -DPREMIADLL_API=  fspremia.cpp

fspremia.so: fspremia.o
	gcc -shared  $(L_PREMIA) $(L_BOOST) -o fspremia.so fspremia.o 

fsgen: fsgen.cpp
	gcc -O -o fsgen -lboost_filesystem -lboost_program_options $(L_BOOST) $(I_BOOST) $(I_PREMIA) $(L_PREMIA) fsgen.cpp

install-python: python
	./pygen --output-dir=../python/premia --dll-dir=.libs

install-karrigell: install-python ksgen 
	./ksgen --output-dir=$(KARRIGELL_WWW)

install-fsharp: fsharp
	./fsgen --output-dir=../fsharp/premia

clean: 
	rm *.o
	rm *.so
	rm *.lo
	rm pygen
	rm ksgen
	rm fsgen 

