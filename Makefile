build: myBot randomBot

myBot: MyBot.cpp
	g++ -std=c++11 MyBot.cpp -o MyBot.o

randomBot: RandomBot.cpp
	g++ -std=c++11 RandomBot.cpp -o RandomBot.o

run:
	./halite -d "30 30" "./MyBot.o"

runSameBot:
	./halite -d "30 30" "./MyBot.o" "./MyBot.o"

runStage1:
	./halite -d "30 30" -s 42 "./MyBot.o"