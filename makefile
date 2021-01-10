restarts = 10
instancia = SJC500-3.txt

all:IA.cpp
	g++ IA.cpp -o IA
	
clean:
	rm -rf *.o
	
run:
	./IA DRP-INSTANCIAS/SJC-DRP/SJC324-3.txt > results.txt
	./IA DRP-INSTANCIAS/SJC-DRP/SJC402-3.txt >> results.txt
	./IA DRP-INSTANCIAS/SJC-DRP/SJC500-3.txt >> results.txt
	./IA DRP-INSTANCIAS/SJC-DRP/SJC500-7.txt >> results.txt
	./IA DRP-INSTANCIAS/SJC-DRP/SJC708-5.txt >> results.txt
	./IA DRP-INSTANCIAS/SJC-DRP/SJC708-7.txt >> results.txt
	./IA DRP-INSTANCIAS/SJC-DRP/SJC708-11.txt >> results.txt
	./IA DRP-INSTANCIAS/SJC-DRP-AEDs/SJC324-3.txt >> results.txt
	./IA DRP-INSTANCIAS/SJC-DRP-AEDs/SJC402-3.txt >> results.txt
	./IA DRP-INSTANCIAS/SJC-DRP-AEDs/SJC500-3.txt >> results.txt
	./IA DRP-INSTANCIAS/SJC-DRP-AEDs/SJC500-7.txt >> results.txt
	./IA DRP-INSTANCIAS/SJC-DRP-AEDs/SJC708-5.txt >> results.txt
	./IA DRP-INSTANCIAS/SJC-DRP-AEDs/SJC708-7.txt >> results.txt
	./IA DRP-INSTANCIAS/SJC-DRP-AEDs/SJC708-11.txt >> results.txt
	
test:
	mkdir testings
	cd testings
	echo "" > testing$(instancia)
	for i in {1..$(restarts)}; do ./IA $(instancia) >> testing$(instancia); done
	cd ..
