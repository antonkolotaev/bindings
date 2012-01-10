BOOST_ROOT = ~/boost_1_47_0
PREMIA_ROOT = ../..
KARRIGELL_WWW = /home/kolotaev/Karrigell-3.1.1/www

I_BOOST =  -I $(BOOST_ROOT) \
           -I /usr/include/python2.7

# for the moment i don't know how to tell the library loader to look into this directory
# so all boost libraries should be put also to LD_LIBRARY_PATH (e.g. /usr/lib)           
L_BOOST = -L $(BOOST_ROOT)/stage/lib
           

I_PREMIA = -I $(PREMIA_ROOT)/Src/common \
           -I $(PREMIA_ROOT)/Src \
           -I $(PREMIA_ROOT)/Src/libpnl/src/include \
           -I $(PREMIA_ROOT)/bindings/include

L_PREMIA = -lpremia -lpnl \
           -L $(PREMIA_ROOT)/Src/.libs \
           -L $(PREMIA_ROOT)/Src/libpnl/src/.libs

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

