boggle: boggle.o score.o boggle.h
	gcc boggle.o score.o -o boggle

boggle.o: boggle.cpp boggle.h
	gcc -g -c boggle.cpp

score.o: score.cpp boggle.h
	gcc -g -c score.cpp

