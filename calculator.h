#include <cstdlib>

class Calculator {

private:
	int _playersCount;
	long long _totalHands;
	long long* _playerWins;
	double* _playerTies;
	int* _bestPlayers;
	int _factorials[52][8];
	
	int _tableCardsCount;
	int _deadCardsCount;

	int (*_playerColorCount)[4];
	int (*_playerColorMasks)[4];
	int* _playerColors;
	long long* _playerMasks3;

	int* _deckCards;
	int _deckCardsCount;
	int _tableCards[5];
	int _usedCards[52];

	int _realToShiftedTable[8192];
	int _highCardsTable[8192];
	int _colorsTable[8192];
	int _lowestOneTable[8192];
	int _highestOneTable[8192];

	void initFactorials();
	void initTables();
	void initArrays();

	void fillRealMasksTable();
	void fillHighestOneTable();
	void fillLowestOneTable();
	void fillHighCardsTable();
	void fillColorsTable();

	int getLexicalOrder5(int c5, int c4, int c3, int c2, int c1);
	int getLexicalOrder3(int c3, int c2, int c1);
	int getLexicalOrder2(int c2, int c1);

	long long getFactorialDivision(int n, int n2);
	long long getFactorial(int n);


	void addCardToPlayer(int player, int card);
	void addCardToPlayers(int deckCard);
	void removeCardFromPlayer(int player, int card);
	void removeCardFromPlayers(int deckCard);
	int max(int x, int y);
	int getPlayerResult(int i);
	void fullTableCalculate(int c3);
	void notFullTableCalculate(int c3);
	void checkBestPlayers();

public:
	Calculator(int playersCount);
	void setCards(int (*playerHands)[2], int tableCards[], int tableCardsCount, int deadCards[], int deadCardsCount);
	void calculate();
	void clear();

	long long* getPlayerWins();
	double* getPlayerTies();
    long long getTotalHands();
};