boggle: boggle.o score.o boggle.h
	gcc boggle.o score.o -o boggle

boggle.o: boggle.cpp boggle.h
	gcc -c boggle.cpp

score.o: score.cpp boggle.h
	gcc -c score.cpp

