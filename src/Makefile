BOOST_ROOT = /opt/local/include/
PREMIA_ROOT = ../..
KARRIGELL_WWW = /usr/local/src/Karrigell-3.1.1/www

PYTHON_DIR=`python2.7 -c 'import sys; print sys.prefix'`
PYTHON_INCLUDE=$(PYTHON_DIR)/include/python2.7
PYTHON_LIB=$(PYTHON_DIR)/lib



I_BOOST =  -I $(BOOST_ROOT) \
           -I $(PYTHON_INCLUDE)

# for the moment i don't know how to tell the library loader to look into this directory
# so all boost libraries should be put also to LD_LIBRARY_PATH (e.g. /usr/lib)           
L_BOOST = -L/opt/local/lib \
		-L$(PYTHON_LIB) -lpython2.7 -lstdc++ -lboost_system


I_PREMIA = -I $(PREMIA_ROOT)/include \
           -I $(PREMIA_ROOT)/bindings/include

L_PREMIA = -lpremia -lpnl \
           -L$(PREMIA_ROOT)/lib

all: python karrigell fsharp
install-all: install-python install-karrigell install-fsharp

python: pypremia.so pygen
karrigell: python ksgen
fsharp: fspremia.so fsgen

pypremia.o: pypremia.cpp
	gcc -O -fPIC -c $(I_BOOST) $(I_PREMIA)   pypremia.cpp

pypremia.so: pypremia.o
	gcc -shared  -lboost_python $(L_PREMIA) $(L_BOOST) -o pypremia.so pypremia.o 

pygen: pygen.cpp
	gcc -O -o pygen -lboost_filesystem -lboost_program_options $(L_BOOST) $(I_BOOST) $(I_PREMIA) $(L_PREMIA) pygen.cpp

ksgen: ksgen.cpp
	gcc -O -o ksgen -lboost_filesystem -lboost_program_options $(L_BOOST) $(I_BOOST) $(I_PREMIA) $(L_PREMIA) ksgen.cpp

fspremia.o: fspremia.cpp
	gcc -O -fPIC -c $(I_BOOST) $(I_PREMIA) -DPREMIADLL_API=  fspremia.cpp

fspremia.so: fspremia.o
	gcc -shared  $(L_PREMIA) $(L_BOOST) -o fspremia.so fspremia.o 

fsgen: fsgen.cpp
	gcc -O -o fsgen -lboost_filesystem -lboost_program_options $(L_BOOST) $(I_BOOST) $(I_PREMIA) $(L_PREMIA) fsgen.cpp

install-python: python
	./pygen --output-dir=../python/premia

install-karrigell: install-python ksgen 
	./ksgen --output-dir=$(KARRIGELL_WWW)

install-fsharp: fsharp
	./fsgen --output-dir=../fsharp/premia

clean: 
	rm *.o
	rm *.so
	rm pygen
	rm ksgen
	rm fsgen 

