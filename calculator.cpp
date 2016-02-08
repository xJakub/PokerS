#include <algorithm>
#include "calculator.h"

#define PAIR_MIN_VALUE  1287
#define TWO_PAIR_MIN_VALUE  PAIR_MIN_VALUE + 14 * 286
#define THREE_MIN_VALUE  TWO_PAIR_MIN_VALUE + 13 * 78 + 12
#define STRAIGHT_MIN_VALUE  THREE_MIN_VALUE + 13 * 78
#define COLOR_MIN_VALUE  STRAIGHT_MIN_VALUE + 10
#define FULL_HOUSE_MIN_VALUE  COLOR_MIN_VALUE + 1287
#define POKER_MIN_VALUE  FULL_HOUSE_MIN_VALUE + 13 * 13
#define STRAIGHT_FLUSH_MIN_VALUE  POKER_MIN_VALUE + 13 * 13

long long Calculator::getFactorial(int n) {
	long long result = n;
	while (--n >= 2) {
		result *= n;
	}
	return result;
}

long long Calculator::getFactorialDivision(int n, int n2) {
	long long result = n;
	while (--n > n2) {
		result *= n;
	}
	return result;
}

void Calculator::initFactorials() {
	for (int i=0; i<7; i++) {
		for (int upper=0; upper<52; upper++) {
			int lower = i+1;
			_factorials[upper][lower] = (int) (getFactorialDivision(upper, upper-lower) / getFactorial(lower));
		}
	}
}

void Calculator::fillRealMasksTable() {
	for (int shiftedMask=0; shiftedMask<8192; shiftedMask++) {
		int realMask = 0;

		for (int i = 0; i < 13; i++) {
			int bitPos = (2 + i * 3) % 13;
			realMask |= ((shiftedMask >> bitPos) & 1) << i;
		}
		_realToShiftedTable[realMask] = shiftedMask;
	}
}

void Calculator::fillLowestOneTable() {
	for (int c1 = 0; c1 < 13; c1++) {
		_lowestOneTable[_realToShiftedTable[1 << c1]] = _realToShiftedTable[1 << c1];

		for (int c2 = c1 + 1; c2 < 13; c2++) {
			int mask2 = 0;
			mask2 |= 1 << c1;
			mask2 |= 1 << c2;
			_lowestOneTable[_realToShiftedTable[mask2]] = _realToShiftedTable[1 << c1];

			for (int c3 = c2 + 1; c3 < 13; c3++) {
				int mask3 = 0;
				mask3 |= 1 << c1;
				mask3 |= 1 << c2;
				mask3 |= 1 << c3;

				_lowestOneTable[_realToShiftedTable[mask3]] = _realToShiftedTable[1 << c1];
			}
		}
	}
}

void Calculator::fillColorsTable() {

	for (int c1 = 0; c1 < 13; c1++) {
		for (int c2 = c1 + 1; c2 < 13; c2++) {
			for (int c3 = c2 + 1; c3 < 13; c3++) {
				for (int c4 = c3 + 1; c4 < 13; c4++) {
					for (int c5 = c4 + 1; c5 < 13; c5++) {

						int mask5 = 0;
						mask5 |= 1 << c1;
						mask5 |= 1 << c2;
						mask5 |= 1 << c3;
						mask5 |= 1 << c4;
						mask5 |= 1 << c5;
						_colorsTable[mask5] = COLOR_MIN_VALUE + getLexicalOrder5(c5, c4, c3, c2, c1);

						for (int c6 = c5 + 1; c6 < 13; c6++) {
							int mask6_5 = 0;
							mask6_5 |= 1 << c1;
							mask6_5 |= 1 << c2;
							mask6_5 |= 1 << c3;
							mask6_5 |= 1 << c4;
							mask6_5 |= 1 << c5;
							mask6_5 |= 1 << c6;
							_colorsTable[mask6_5] = COLOR_MIN_VALUE + getLexicalOrder5(c6, c5, c4, c3, c2);

							for (int c7 = c6 + 1; c7 < 13; c7++) {
								int mask7_5 = 0;
								mask7_5 |= 1 << c1;
								mask7_5 |= 1 << c2;
								mask7_5 |= 1 << c3;
								mask7_5 |= 1 << c4;
								mask7_5 |= 1 << c5;
								mask7_5 |= 1 << c6;
								mask7_5 |= 1 << c7;
								_colorsTable[mask7_5] = COLOR_MIN_VALUE + getLexicalOrder5(c7, c6, c5, c4, c3);
							}
						}
					}
				}

			}
		}
	}

	for (int i = 3; i < 13; i++) {
		// escaleras, empezamos por 3 (5 / A2345) y terminamos por 12 (A / TKQJA)
		int mask = 0;
		mask |= (1 << ((i - 4 + 13) % 13));
		mask |= (1 << (i - 3));
		mask |= (1 << (i - 2));
		mask |= (1 << (i - 1));
		mask |= (1 << (i));

		for (int c6 = 0; c6 < 13; c6++) {
			// if ((mask & (1 << c6)) != 0) continue;

			for (int c7 = c6; c7 < 13; c7++) {
				// if ((mask & (1 << c7)) != 0) continue;

				int mask7 = mask | (1 << c7) | (1 << c6);
				_colorsTable[mask7] = STRAIGHT_FLUSH_MIN_VALUE + (i - 3);
			}
		}
	}

}

void Calculator::fillHighestOneTable() {

	for (int c1 = 0; c1 < 13; c1++) {
		_highestOneTable[_realToShiftedTable[1 << c1]] = _realToShiftedTable[1 << c1];

		for (int c2 = c1 + 1; c2 < 13; c2++) {
			int mask2 = 0;
			mask2 |= 1 << c1;
			mask2 |= 1 << c2;
			_highestOneTable[_realToShiftedTable[mask2]] = _realToShiftedTable[1 << c2];

			for (int c3 = c2 + 1; c3 < 13; c3++) {
				int mask3 = 0;
				mask3 |= 1 << c1;
				mask3 |= 1 << c2;
				mask3 |= 1 << c3;

				_highestOneTable[_realToShiftedTable[mask3]] = _realToShiftedTable[1 << c3];
			}
		}
	}
}

void Calculator::fillHighCardsTable() {

	for (int c1 = 0; c1 < 13; c1++) {

		int mask1 = 0;
		mask1 |= 1 << c1;
		_highCardsTable[_realToShiftedTable[mask1]] = c1;

		for (int c2 = c1 + 1; c2 < 13; c2++) {
			int mask2 = 0;
			mask2 |= 1 << c1;
			mask2 |= 1 << c2;
			_highCardsTable[_realToShiftedTable[mask2]] = getLexicalOrder2(c2, c1);

			for (int c3 = c2 + 1; c3 < 13; c3++) {
				int mask3_1 = 0;
				mask3_1 |= 1 << c1;
				mask3_1 |= 1 << c2;
				mask3_1 |= 1 << c3;
				_highCardsTable[_realToShiftedTable[mask3_1]] = c3;

				for (int c4 = c3 + 1; c4 < 13; c4++) {
					int mask4_2 = 0;
					mask4_2 |= 1 << c1;
					mask4_2 |= 1 << c2;
					mask4_2 |= 1 << c3;
					mask4_2 |= 1 << c4;
					_highCardsTable[_realToShiftedTable[mask4_2]] = getLexicalOrder2(c4, c3);

					for (int c5 = c4 + 1; c5 < 13; c5++) {

						int mask5_3 = 0;
						mask5_3 |= 1 << c1;
						mask5_3 |= 1 << c2;
						mask5_3 |= 1 << c3;
						mask5_3 |= 1 << c4;
						mask5_3 |= 1 << c5;
						_highCardsTable[_realToShiftedTable[mask5_3]] = getLexicalOrder3(c5, c4, c3);

						for (int c6 = c5 + 1; c6 < 13; c6++) {
							for (int c7 = c6 + 1; c7 < 13; c7++) {
								int mask7_5 = 0;
								mask7_5 |= 1 << c1;
								mask7_5 |= 1 << c2;
								mask7_5 |= 1 << c3;
								mask7_5 |= 1 << c4;
								mask7_5 |= 1 << c5;
								mask7_5 |= 1 << c6;
								mask7_5 |= 1 << c7;
								_highCardsTable[_realToShiftedTable[mask7_5]] = getLexicalOrder5(c7, c6, c5, c4, c3);
							}
						}
					}
				}

			}
		}
	}

	for (int i = 3; i < 13; i++) {
		// escaleras, empezamos por 3 (5 / A2345) y terminamos por 12 (A / TKQJA)
		int mask = 0;
		mask |= (1 << ((i - 4 + 13) % 13));
		mask |= (1 << (i - 3));
		mask |= (1 << (i - 2));
		mask |= (1 << (i - 1));
		mask |= (1 << (i));

		for (int c6 = 0; c6 < 13; c6++) {
			// if ((mask & (1 << c6)) != 0) continue;

			for (int c7 = c6; c7 < 13; c7++) {
				// if ((mask & (1 << c7)) != 0) continue;

				int mask7 = mask | (1 << c7) | (1 << c6);
				_highCardsTable[_realToShiftedTable[mask7]] = STRAIGHT_MIN_VALUE + (i - 3);
			}
		}
	}

}

int Calculator::getLexicalOrder5(int c5, int c4, int c3, int c2, int c1) {
	int result = 0;
	result += getFactorialDivision(c5, c5 - 5) / getFactorial(5);
	result += getFactorialDivision(c4, c4 - 4) / getFactorial(4);
	result += getFactorialDivision(c3, c3 - 3) / getFactorial(3);
	result += getFactorialDivision(c2, c2 - 2) / getFactorial(2);
	result += getFactorialDivision(c1, c1 - 1) / getFactorial(1);
	return result;
}

int Calculator::getLexicalOrder3(int c3, int c2, int c1) {
	int result = 0;
	result += getFactorialDivision(c3, c3 - 3) / getFactorial(3);
	result += getFactorialDivision(c2, c2 - 2) / getFactorial(2);
	result += getFactorialDivision(c1, c1 - 1) / getFactorial(1);
	return result;
}

int Calculator::getLexicalOrder2(int c2, int c1) {
	int result = 0;
	result += getFactorialDivision(c2, c2 - 2) / getFactorial(2);
	result += getFactorialDivision(c1, c1 - 1) / getFactorial(1);
	return result;
}

#define ZEROFILL(x) std::fill_n(x, 8192, 0)

void Calculator::initTables() {
	ZEROFILL(_realToShiftedTable);
	ZEROFILL(_highCardsTable);
	ZEROFILL(_highestOneTable);
	ZEROFILL(_lowestOneTable);
	ZEROFILL(_colorsTable);

	fillRealMasksTable();
	fillHighestOneTable();
	fillLowestOneTable();
	fillHighCardsTable();
	fillColorsTable();
}

void Calculator::initArrays() {
	_totalHands = 0;

	_bestPlayers = new int[_playersCount];
	_playerMasks3 = new long long[_playersCount];
	_playerColorMasks = new int[_playersCount][4];
	_playerColorCount = new int[_playersCount][4];

	_playerWins = new long long[_playersCount];
	_playerTies = new double[_playersCount];

	_playerColors = new int[_playersCount];
	for (int i=0; i<_playersCount; i++) {
		_playerColors[i] = -1;
	}

	_deckCards = new int[52 - _playersCount*2];
}

void Calculator::setCards(int (*playerHands)[2], int tableCards[], int tableCardsCount, int deadCards[], int deadCardsCount) {
	std::fill_n(_usedCards, 52, 0);
	std::fill_n(_playerMasks3, _playersCount, 0);
	std::fill_n(_playerColors, _playersCount, -1);

	for (int i=0; i<tableCardsCount; i++) {
		_tableCards[i] = tableCards[i];
	}

	_tableCardsCount = tableCardsCount;
	_deadCardsCount = deadCardsCount;

	for (int i = 0; i < tableCardsCount; i++) {
		_usedCards[tableCards[i]]++;
	}

	for (int i = 0; i < tableCardsCount; i++) {
		_usedCards[deadCards[i]]++;
	}

	for (int i = 0; i < _playersCount; i++) {
		std::fill_n(_playerColorMasks[i], 4, 0);
		std::fill_n(_playerColorCount[i], 4, 0);
		_usedCards[playerHands[i][0]]++;
		_usedCards[playerHands[i][1]]++;
		addCardToPlayer(i, playerHands[i][0]);
		addCardToPlayer(i, playerHands[i][1]);
	}

	int pos = 0;
	for (int i = 0; i < 52; i++) {
		if (_usedCards[i] == 0) {
			_deckCards[pos++] = i;
		}
	}
	_deckCardsCount = pos;
}


void Calculator::addCardToPlayers(int deckCard) {
	for (int i = 0; i < _playersCount; i++) {
		addCardToPlayer(i, deckCard);
	}
}

void Calculator::addCardToPlayer(int player, int card) {
	int number = card >> 2;
	int color = card & 3;

	_playerMasks3[player] += (1ull << (number * 3));

	_playerColorMasks[player][color] |= (1 << number);

	if (++_playerColorCount[player][color] == 5) {
		_playerColors[player] = color;
	}
}


void Calculator::removeCardFromPlayer(int player, int card) {
	int number = card >> 2;
	int color = card & 3;

	_playerMasks3[player] -= (1ull << (number * 3));

	_playerColorMasks[player][color] -= (1 << number);

	if (--_playerColorCount[player][color] == 4) {
		_playerColors[player] = -1;
	}

}

void Calculator::removeCardFromPlayers(int deckCard) {
	for (int i = 0; i < _playersCount; i++) {
		removeCardFromPlayer(i, deckCard);
	}
}

int Calculator::max(int x, int y) {
	//return Math.max(x, y);
	return x ^ ((x ^ y) & ((x - y) >> 31));
}


int Calculator::getPlayerResult(int i) {
	int result = 0;

	if (_playerColors[i] != -1) {
		result = _colorsTable[_playerColorMasks[i][_playerColors[i]]];
	}

	long long tmp = _playerMasks3[i] & 0x4924924924ull;
	int pokers = (int) ((tmp & 0x924) + ((tmp >> 13) & 0x492) + (tmp >> 26));

	tmp = (_playerMasks3[i] & 0x2492492492ull) << 1;
	int pairs = (int) ((tmp & 0x924) + ((tmp >> 13) & 0x492) + (tmp >> 26));

	tmp = (_playerMasks3[i] & 0x1249249249ull) << 2;
	int kickers = (int) ((tmp & 0x924) + ((tmp >> 13) & 0x492) + (tmp >> 26));

	if (pokers == 0) {
		// carta alta o escalera
		result = max(result, _highCardsTable[kickers | pairs]);

		int threes = pairs & kickers;
		pairs -= threes;
		kickers -= threes;

		if (threes != 0) {
			// AAABBBC -> AAABBC
			pairs |= threes - _highestOneTable[threes];
			threes = _highestOneTable[threes];

			if (pairs != 0) {
				// full house
				result = max(result, FULL_HOUSE_MIN_VALUE + 13 * _highCardsTable[threes] + _highCardsTable[_highestOneTable[pairs]]);

			} else if (result < STRAIGHT_MIN_VALUE) {
				// trio
				result = max(result, THREE_MIN_VALUE + 78 * _highCardsTable[threes] + _highCardsTable[kickers]);
			}

		} else if (pairs != 0 && result < STRAIGHT_MIN_VALUE) {
			if ((pairs & (pairs - 1)) != 0) {
				// doble par

				if ((kickers & (kickers - 1)) == 0) {
					// solo hay un kicker, asi que hay 3 pares
					// AABBCCD -> AABBCD
					kickers += _lowestOneTable[pairs];
					pairs -= _lowestOneTable[pairs];
				}
				result = max(result, TWO_PAIR_MIN_VALUE + 13 * _highCardsTable[pairs] + _highCardsTable[_highestOneTable[kickers]]);
			} else {
				result = max(result, PAIR_MIN_VALUE + 286 * _highCardsTable[pairs] + _highCardsTable[kickers]);
			}
		}

	} else {
		// poker
		result = max(result, POKER_MIN_VALUE + 13 * _highCardsTable[pokers] + _highCardsTable[_highestOneTable[kickers | pairs]]);
	}

	return result;
}

__inline void Calculator::checkBestPlayers() {
	int bestPlayersCount = 1;
	_bestPlayers[0] = 0;
	int bestResult = getPlayerResult(0);

	for (int i = 1; i < _playersCount; i++) {
		int result = getPlayerResult(i);
		if (result > bestResult) {
			bestPlayersCount = 1;
			_bestPlayers[0] = i;
			bestResult = result;
		} else if (result == bestResult) {
			_bestPlayers[bestPlayersCount++] = i;
		}
	}

	if (bestPlayersCount == 1) {
		_playerWins[_bestPlayers[0]] += 1;
	} else {
		for (int i = 0; i < bestPlayersCount; i++) {
			_playerTies[_bestPlayers[i]] += 1.0 / bestPlayersCount;
		}
	}

	_totalHands++;
}

// esto da como un 2-3% más de velocidad. nos gustan los porcentajes, así que es bienvenido
void Calculator::notFullTableCalculate(int c3)  {
	for (int c4 = c3 + 1; c4 < _deckCardsCount - 1; c4 += 1) {
		if (_tableCardsCount >= 4) {
			addCardToPlayers(_tableCards[3]);
			c4 = -1;
		} else {
			addCardToPlayers(_deckCards[c4]);
		}

		for (int c5 = c4 + 1; c5 < _deckCardsCount; c5 += 1) {
			addCardToPlayers(_deckCards[c5]);

			int bestPlayersCount = 1;
			_bestPlayers[0] = 0;
			int bestResult = getPlayerResult(0);

			for (int i = 1; i < _playersCount; i++) {
				int result = getPlayerResult(i);
				if (result > bestResult) {
					bestPlayersCount = 1;
					_bestPlayers[0] = i;
					bestResult = result;
				} else if (result == bestResult) {
					_bestPlayers[bestPlayersCount++] = i;
				}
			}

			if (bestPlayersCount == 1) {
				_playerWins[_bestPlayers[0]] += 1;
			} else {
				for (int i = 0; i < bestPlayersCount; i++) {
					_playerTies[_bestPlayers[i]] += 1.0 / bestPlayersCount;
				}
			}

			_totalHands++;

			removeCardFromPlayers(_deckCards[c5]);
		}
		if (_tableCardsCount >= 4) {
			removeCardFromPlayers(_tableCards[3]);
			break;
		} else {
			removeCardFromPlayers(_deckCards[c4]);
		}
	}
}

void Calculator::fullTableCalculate(int c3)  {
	addCardToPlayers(_tableCards[3]);
	addCardToPlayers(_tableCards[4]);
	checkBestPlayers();
	removeCardFromPlayers(_tableCards[4]);
	removeCardFromPlayers(_tableCards[3]);
}


void Calculator::calculate()  {

	for (int c1 = 0; c1 < _deckCardsCount - 4; c1 += 1) {
		if (_tableCardsCount >= 1) {
			// Si hay cartas en la mesa, no recorremos las posibilidades
			// En su lugar, añadimos la carta fija
			addCardToPlayers(_tableCards[0]);
			// c1=-1 para que c2 empiece en 0
			c1 = -1;
		} else {
			addCardToPlayers(_deckCards[c1]);
		}

		for (int c2 = c1 + 1; c2 < _deckCardsCount - 3; c2 += 1) {
			if (_tableCardsCount >= 2) {
				addCardToPlayers(_tableCards[1]);
				c2 = -1;
			} else {
				addCardToPlayers(_deckCards[c2]);
			}

			for (int c3 = c2 + 1; c3 < _deckCardsCount - 2; c3 += 1) {
				if (_tableCardsCount >= 3) {
					addCardToPlayers(_tableCards[2]);
					c3 = -1;
				} else {
					addCardToPlayers(_deckCards[c3]);
				}

				if (_tableCardsCount < 5) {
					notFullTableCalculate(c3);
				}
				else {
					fullTableCalculate(c3);
				}

				if (_tableCardsCount >= 3) {
					removeCardFromPlayers(_tableCards[2]);
					break;
				} else {
					removeCardFromPlayers(_deckCards[c3]);
				}
			}
			if (_tableCardsCount >= 2) {
				removeCardFromPlayers(_tableCards[1]);
				break;
			} else {
				removeCardFromPlayers(_deckCards[c2]);
			}
		}
		if (_tableCardsCount >= 1) {
			removeCardFromPlayers(_tableCards[0]);
			break;
		} else {
			removeCardFromPlayers(_deckCards[c1]);
		}
	}
}

long long* Calculator::getPlayerWins() {
	return _playerWins;
}

double* Calculator::getPlayerTies() {
	return _playerTies;
}

long long Calculator::getTotalHands() {
	return _totalHands;
}

void Calculator::clear() {
	for (int i=0; i<_playersCount; i++) {
		_playerWins[i] = 0;
		_playerTies[i] = 0;
	}
	_totalHands = 0;
}


Calculator::Calculator(int playersCount) {
	_playersCount = playersCount;

	initFactorials();
	initTables();
	initArrays();
}