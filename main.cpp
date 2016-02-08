#include <stdio.h>
#include <time.h>
#include "calculator.h"

using namespace std;

int main() {
	Calculator calc(2);

	float startTime = (float)clock()/CLOCKS_PER_SEC;

	int cards[][2] = {{0,1}, {50,51}};
	calc.clear();
	calc.setCards(cards, NULL, 0, NULL, 0);

	for (int i=0; i<10; i++) {
		calc.calculate();
	}

	float endTime = (float)clock()/CLOCKS_PER_SEC;

	float elapsedTime = endTime - startTime;

	long long* playerWins = calc.getPlayerWins();
	double* playerTies = calc.getPlayerTies();

	printf("Total hands: %lld\n", calc.getTotalHands());

	for (int i=0; i<2; i++) {
		float playerEquity = (playerTies[i] + playerWins[i]) / calc.getTotalHands() * 100;

		printf("Player %d: %d wins, %.2f ties (%.3f%%)\n", i+1, playerWins[i]);
		printf("%.2f ties (%.3f%%)\n", playerTies[i], playerEquity);
	}

	printf("Speed: %.0f games/s\n", (calc.getTotalHands()/elapsedTime));
	printf("Elapsed %f s\n", elapsedTime);

	getchar();
}