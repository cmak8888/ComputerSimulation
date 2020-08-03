#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <vector>
#include <string>
#include <map>
using namespace std;

struct Card {
	int value;           //Ace = 1; Jack = 11; Queen = 12; King = 13;
	string suite;		 //Spades, Clubs, Hearts, Diamonds
	bool reveal;
};

//returns a vector of players from 1 to n where n is the number of players in the game.
vector<int> fillPlayers(int playerCount) {
	vector<int> players;
	for(int i = 1; i <= playerCount; i++) {
		players.push_back(i);
	}
	return players;
}

//Define copy vector functions (forgot how to create templates with T)
vector<int> copy(vector<int> &copyVector) {
	vector<int> newVector;
	for(auto i : copyVector) {
		newVector.push_back(i);
	}
	return newVector;
}

vector<Card> copy(vector<Card> &copyVector) {
	vector<Card> newVector;
	for(auto i : copyVector) {
		newVector.push_back(i);
	}
	return newVector;
}

void insert(vector<Card> &firstVector, vector<Card> &insertVector) {
	for(auto i : insertVector) {
		if (firstVector.empty()) {
			firstVector = {i};
		} else {firstVector.push_back(i); }
	}
}

void insert(vector<int> &firstVector, vector<int> &insertVector) {
	for(auto i : insertVector) {
		if (firstVector.empty()) {
			firstVector = {i};
		} else {firstVector.push_back(i); }
	}
}

//Shuffling Algorithm using Fisher-Yates
void shuffle(vector<Card> &CardDeck, ifstream &inputFile, string fileName) {
	string r;
	for(size_t c = 0; c < CardDeck.size() - 1; c++) {
		if(!inputFile.is_open()) {
			inputFile.open(fileName);
		} else {
			if(!inputFile.eof()) {
				getline(inputFile,r);
			} else {
				inputFile.close();
				inputFile.open(fileName);
				getline(inputFile,r);
			}
		}
		int p = floor(double(stod(r)*(CardDeck.size() - c)) + c);
		Card o = CardDeck[c];
		Card q = CardDeck[p];
		CardDeck[p] = o;
		CardDeck[c] = q;
	}
	return;
}

void output(double turns, int winTrans, double lTrans) {
	cout << "OUTPUT trash turns " << int(turns) << " transitions " << winTrans << " last " << lTrans << endl;
	exit(0);
}

//Retrieves the winning player of the turn
/*int getPlayer(map<int, vector<Card>> &pile, vector<int> &players, vector<Card> &currentPile, map<int, vector<Card>> &winningPile, ifstream &inputFile, string filename) {
	if(players.size() == 1) {
		return players[0];
	}
	vector<int> newPlayerQueue = {};
	int maxValue = pile.at(players[0]).at(0).value;
	for(size_t i = 0; i < players.size(); i++) {							//Iterates over all valid players
		if(pile.at(players[i]).empty()) {								//If player's hand is empty, check WinningPile.
			cout << "Empty" << endl;
			if(winningPile.at(players[i]).empty()) {					//If player has no cards, he is out.
				cout << "He is out" << endl;
				continue;
			} else{														//Else, insert winningpile shuffled into the hand.
				shuffle(winningPile.at(players[i]), inputFile, filename);
				insert(pile.at(players[i]), winningPile.at(players[i]));
				winningPile.at(players[i]) = {};
			}
		}
		if (pile.at(players[i]).at(0).value == 1) {						//Tiebreaker
			if(maxValue == pile.at(players[i]).at(0).value) {
				newPlayerQueue.push_back(players[i]);
			} else {
				maxValue = pile.at(players[i]).at(0).value;
				newPlayerQueue = {};
				newPlayerQueue.push_back(players[i]);
			}
		} else if(pile.at(players[i]).at(0).value > maxValue) {
			maxValue = pile.at(players[i]).at(0).value;
			newPlayerQueue = {};
			newPlayerQueue.push_back(players[i]);
		} else if (pile.at(players[i]).at(0).value == maxValue) {
			newPlayerQueue.push_back(players[i]);
		}
		//cout << pile.at(players[i]).at(0).value << endl;
		currentPile.push_back(pile.at(players[i]).at(0));				//Accumulate my winning card pool
		pile.at(players[i]).erase(pile.at(players[i]).begin());			//Remove the card from the hand
	}
	return getPlayer(pile, newPlayerQueue, currentPile, winningPile, inputFile, filename);
}*/

int playHands(vector<Card> &p1,	vector<Card> &p2, vector<Card> &w1,	vector<Card> &w2, vector<Card> &cPile, ifstream &inputFile, string filename) {
	if(p1.empty()) {
		//cout << "empty" << endl;
		if(w1.empty()) {
			if(p2.empty() && w2.empty()) {				//Cannot proceed, out of cards
				return 0;
			} else
				return 2;
		} else {
			shuffle(w1, inputFile, filename);
			insert(p1,w1);
			w1 = {};
		}
	}
	if(p2.empty()){
		//cout << "empty2" << endl;
		if(w2.empty()) {
			if(p1.empty() && w1.empty()) {				//Cannot proceed, out of cards
				return 0;
			} else
			    return 1;
		} else {
			shuffle(w2, inputFile, filename);
			insert(p2,w2);
			w2 = {};
		}
	} 
	//cout << "1 " << p1.size() << endl;
	//cout << "2 " << p2.size() << endl;
	Card playerCard1 = p1[0];
	//cout << "P1 " << playerCard1.value << endl;
	Card playerCard2 = p2[0];
	//cout << "P2 " << playerCard2.value << endl;
	cPile.push_back(playerCard1);
	cPile.push_back(playerCard2);
	p1.erase(p1.begin());
	p2.erase(p2.begin());
	if(playerCard1.value == 1) {
		if(playerCard2.value != 1) {
			return 1;
		} else {
			return playHands(p1,p2,w1,w2,cPile,inputFile,filename);
		}
	} else if(playerCard2.value == 1) {
		if(playerCard1.value != 1) {
			return 2;
		} else {
			return playHands(p1,p2,w1,w2,cPile,inputFile,filename);
		}
	} else {
		if (playerCard1.value > playerCard2.value) {
			return 1;
		} else if(playerCard1.value < playerCard2.value) {
			return 2;
		} else {
			return playHands(p1,p2,w1,w2,cPile,inputFile,filename);
		}
	} 
}
/**
	War
	A game where players are given an even-ish amount of cards.
	Each turn/exchange, players present the topmost card and determine the highest value card.
	Ace is the highest card. The winner gets both cards in their winning Pile.
	If there is a tie, the exchange continues until a winner has been determined or the losing player has no cards to play.
	Winner is when one player has all 52 cards.
*/
void war(string fileName, int playerCount) {
	//Output
	double numTurns = 0.;
	int winTrans = 0;
	double lastTrans = 0.;
	int winningPlayer = 0;								//Current player winning
	
	ifstream inputFile(fileName);						//Reading Input File
	if( !inputFile ) {
		cerr << "Error opening data file." << endl;
		::exit(1);
	}

	vector<Card> deck;
	string suite;
	for(int i = 0; i < 4; i++) {
		switch(i) {
			case 0:
				suite = "Spades";
				break;
			case 1:
				suite = "Clubs";
				break;
			case 2:
				suite = "Hearts";
				break;
			case 3:
				suite = "Diamonds";
				break;
		}
		for (int j = 1; j < 14; j++) {
			Card card = {j,suite, true};
			deck.push_back(card);
		}
	}
	shuffle(deck, inputFile, fileName);					//Create and shuffle deck of cards
	vector<Card> p1 = {};			//PlayerDeck
	vector<Card> p2 = {};
	vector<Card> w1 = {};			//WinningDeck
	vector<Card> w2 = {};
	for(int i = 0; i < 52; i+=2) {
		p1.push_back(deck[i]);
		p2.push_back(deck[i + 1]);
	}
	//Different Piles
	//maps by Player # and the corresponding deck
	//map<int, vector<Card>> handPile;
	//map<int, vector<Card>> winningPile;
	//vector<int> players = fillPlayers(playerCount);     //All existing players still in the exchange
	//Initialize empty vectors for the piles
	/*for(int i = 1; i <= playerCount; i++) {
		handPile[i] = {};
		winningPile[i] = {};
	}*/
	//cout << "INIT" << endl;
	//Populate Handpile
	/*int j = 1;
	for(int i = 0; i < 52; i++) {
		//cout << "Pile" << endl;
		if (handPile[j].empty()) { handPile[j] = {deck[i]};} else {
			handPile[j].push_back(deck[i]);
		}
		//cout << "Piles" << j << endl;
		if (j + 1 <= playerCount) {
			j++;
		} else {j = 1;}
	}*/
	//Play the game
	while (true) {
		numTurns += 1.0;
		//cout << "Turn " << numTurns << endl;
		vector<Card> currentPile = {}; 						//In case of tiebreakers
		
		//cout << numTurns << endl;
		//int player = getPlayer(handPile, players, currentPile, winningPile, inputFile, fileName);		//Winner of the exchange
		//cout << "player" << player << endl;
		//insert(winningPile.at(player),currentPile);
		//cout << "Added to pile" << endl;
		int player = playHands(p1,p2,w1,w2,currentPile, inputFile,fileName);
		//cout << player << endl;
		if(winningPlayer != player) {
			winningPlayer = player;
			lastTrans = numTurns;
		} else {
			winTrans++;
		}

		if(player == 1) {
			if (!currentPile.empty())
				insert(w1,currentPile);
			if(p1.size() + w1.size() == 52) {
				output(numTurns, winTrans, double(lastTrans/numTurns));
				inputFile.close();
				return;
			}
		} else if (player == 2) {
			if(!currentPile.empty())
				insert(w2,currentPile);
			if(p2.size() + w2.size() == 52) {
				output(numTurns, winTrans, double(lastTrans/numTurns));
				inputFile.close();
				return;
			}
		} else if (player == 0) {
			output(numTurns, winTrans, double(lastTrans)/double(numTurns));
			inputFile.close();
			return;
		}
		
	}
}

//Create a new array for the player
void newHand(vector<Card> &deck, vector<Card> &playerDeck) {
	size_t maxSize = playerDeck.size() - 1;
	playerDeck = {};
	for(size_t i = 0; i < maxSize; i++) {
		Card card = deck[0];
		deck.erase(deck.begin());
		card.reveal = false;
		playerDeck.push_back(card);
	}
}

//Draw a card from the deck
Card drawCard(vector<Card> &deck, vector<Card> &discard, ifstream &inputFile, string filename) {
	Card playerCard;
	if(deck.empty()) {													//If there no more cards to draw, shuffle the bottom of the discard pile.
		Card tempCard = discard[discard.size() - 1];				//Retrieve top of the discard pile.
		discard.pop_back();
		shuffle(discard, inputFile, filename);							//Shuffle Remaining discard pile and replace drawPile.
		insert(deck, discard);			
		discard[0] = tempCard;											//Restore top of discard pile.
	}
	playerCard = deck[0];										//Get card from draw pile
	deck.erase(deck.begin());
	playerCard.reveal = true;
	return playerCard;
}

//Check if hand has the card
bool findCard(vector<Card> &hand, int value) {
	if(size_t(value) > hand.size()) {
		return false;
	}
	if(hand[value-1].reveal == true) {
		if(hand[value-1].value == value) {
			return true;
		}
	}
	return false;
}

//Checks if the array is complete
bool checkArrayComp(vector<Card> &hand) {
	//cout << "Err" << endl;
	for(size_t i = 0; i < hand.size(); i++) {
		if (!hand[i].reveal) { return false; 
		} else {
			if(hand[i].value == 11) { continue; } 
			else if(hand[i].value == int(i + 1)) { continue; 
			} else {
				return false;
			}
		}
	}
	//cout << "True" << endl;
	return true;
}

//swap card with card in the hand
Card swap(vector<Card> &hand, size_t index, Card card) {
	Card tempCard = hand[index];
	hand[index] = card;
	return tempCard;
}

void printHand(vector<Card> &hand) {
	for (auto i : hand) {
		if(i.reveal) {
			cout << i.value << " ";
		} else {
			cout << " X "; 
		}
	}
	cout << endl;
}

//determines optimal position to place wildcard
size_t optimalJack(vector<Card> &discard, vector<Card> &hand) {
	vector<int> numVal(hand.size(),0);								//Records number of necessary cards found in discard.
	// << "Jack" << endl;
	size_t it= 0;
	for(size_t i = 0; i < discard.size(); i++) {						//Populate array
		if(size_t(discard[i].value) <= hand.size()) {
			if(discard[i].value != 11 && !hand[discard[i].value-1].reveal) {
				numVal[discard[i].value-1]++;
			}
		}
	}
	int min = 4;													//4 suites, maximum 4 of each card.
	for(size_t i = 0; i < numVal.size(); i++) {
		if(4 - numVal[i] < min) {											//If 3 of the card is in the discard, there is 1 card left in the drawpile.
			//cout << min << endl;
			min = 4-numVal[i];
		}
	}
	//cout << "Get Min" << endl;
	for(size_t i = 0; i < numVal.size(); i++) {						//Return least likely card to be found in drawpile.
		if(4-numVal[i] == min && hand[i].value != 11) {
			if(!hand[i].reveal) {
				//cout << i << endl;
				return i;
			}
		}
	}
	return it;	//Won't be used.
}

void doPlayerTurn(vector<Card> &hand, vector<Card> &discard, vector<Card> &deck, Card card, ifstream &inputFile, string filename) {
	if(checkArrayComp(hand)) {
		if(hand.size() == size_t(1)) {
			return;
		} else{ 
			newHand(deck,hand); 
		}
	}
	//cout << card.value << endl;
	card.reveal = true;
	printHand(hand);
	//cout << card.value << endl;
	if(size_t(card.value) > 11) {
		discard.push_back(card);
		return;
	} else if(size_t(card.value) == 11) {													//Wildcard
		//cout << "Jack" << endl;
		size_t slot = optimalJack(discard, hand);
		card = swap(hand,slot,card);
		doPlayerTurn(hand,discard,deck, card, inputFile, filename);
		//cout << "Clear" << endl;
		return;
	} else {
		if(size_t(card.value) > hand.size() || findCard(hand,card.value)) {
			discard.push_back(card);
			return;
		} else {
			card = swap(hand,size_t(card.value-1),card);
			doPlayerTurn(hand,discard,deck, card, inputFile, filename);
			return;
		}
	}
}

void trash(string filename, int playerCount) {
	//Output
	double numTurns = 0.;
	int winTrans = 0;
	double lastTrans = 0.;
	int winningPlayer = 0;								//Current player winning
	bool win = false;
	bool playerTurn = false;							//0 is player 1, 1 is player 2
	
	ifstream inputFile(filename);						//Reading Input File
	if( !inputFile ) {
		cerr << "Error opening data file." << endl;
		::exit(1);
	}

	vector<Card> deck;
	string suite;
	for(int i = 0; i < 4; i++) {
		switch(i) {
			case 0:
				suite = "Spades";
				break;
			case 1:
				suite = "Clubs";
				break;
			case 2:
				suite = "Hearts";
				break;
			case 3:
				suite = "Diamonds";
				break;
		}
		for (int j = 1; j < 14; j++) {
			Card card = {j,suite, false};
			deck.push_back(card);
		}
	}
	shuffle(deck, inputFile, filename);					//Create and shuffle deck of cards

	//4 collections of cards
	vector<Card> drawPile;						//Top at 0
	vector<Card> discardPile;					//Top at the end
	vector<Card> arrayP1;
	vector<Card> arrayP2;

	//Populate Decks
	for(size_t i = 0; i < 20; i += 2) {
		//cout << deck[i].value;
		arrayP1.push_back(deck[i]);
		//cout << deck[i + 1].value << endl;
		arrayP2.push_back(deck[i+1]);
	}
	discardPile.push_back(deck[20]);
	for(size_t i = 21; i < 52; i++) {
		drawPile.push_back(deck[i]);
	}

	/**
		Necessary Steps:
		Starting out:
			Check discard pile.
			If card is trash, ignore
			Take card
			Store in hand, and do stuff
		Following turns:
			Check discard pile
			If card is trash, ignore
			If the card value is already in hand, ignore
			Determine if card in discard pile can go in the hand.
			Store in hand and do stuffs
		After beginning of the turn:
			Determine if card in hand goes in the discard pile or elsewhere in the hand.

	*/

	while(true) {
		numTurns += 1.;
		Card playerCard = {};
		if(!playerTurn)  {
			//cout << "Discrard " << 	discardPile[discardPile.size() - 1].value << endl;															//Player 1's Turn 
			if(discardPile[discardPile.size() - 1].value > 11) {						//If Queen or King
				playerCard = drawCard(drawPile, discardPile, inputFile, filename);		//Draw from DrawCard
			} else if(discardPile[discardPile.size() - 1].value == 11)	{				//Always love a wildcard.
				discardPile.pop_back();
				size_t slot = optimalJack(discardPile, arrayP1);
				playerCard = swap(arrayP1,slot,playerCard);
			} else {
				playerCard = discardPile[discardPile.size() - 1];
				if(findCard(arrayP1, playerCard.value)) {							//We have this card already
					playerCard = drawCard(drawPile, discardPile, inputFile, filename);		//Draw from DrawCard
				} else {
					if(size_t(playerCard.value) > arrayP1.size()) {
						playerCard = drawCard(drawPile, discardPile, inputFile, filename);		//Draw from DrawCard
					} else {
						discardPile.pop_back();
						playerCard = swap(arrayP1,size_t(playerCard.value-1),playerCard);
					}
				}
			}
			doPlayerTurn(arrayP1, discardPile, drawPile,playerCard, inputFile, filename);
		} else {
			//cout << "Help" << endl;
			if(size_t(discardPile[discardPile.size() - 1].value) > 11) {						//If Queen or King
				playerCard = drawCard(drawPile, discardPile, inputFile, filename);		//Draw from DrawCard
			} else if(size_t(discardPile[discardPile.size() - 1].value) == 11)	{				//Always love a wildcard.
				discardPile.pop_back();
				size_t slot = optimalJack(discardPile, arrayP2);
				playerCard = swap(arrayP2,slot,playerCard);
			} else {
				playerCard = discardPile[discardPile.size() - 1];
				if(findCard(arrayP2, playerCard.value)) {							//We have this card already
					playerCard = drawCard(drawPile, discardPile, inputFile, filename);		//Draw from DrawCard
				} else {
					if(size_t(playerCard.value) > arrayP2.size()) {
						playerCard = drawCard(drawPile, discardPile, inputFile, filename);		//Draw from DrawCard
					} else {
						// cout << "Work" << endl;
						// cout << playerCard.value << endl;
						// cout << arrayP2.size() << endl;
						playerCard = swap(arrayP2,size_t(playerCard.value-1),playerCard);
					}
				}
			}
			doPlayerTurn(arrayP2, discardPile, drawPile,playerCard, inputFile, filename);
		}
		if(arrayP1.size() < arrayP2.size()) {
			if(winningPlayer != 0) {
				winningPlayer = 0;
				winTrans += 1;
				lastTrans = numTurns;
			}
		} else if(arrayP1.size() > arrayP2.size()) {
			if(winningPlayer != 1) {
				winningPlayer = 1;
				winTrans += 1;
				lastTrans = numTurns;
			}
		}
		if(checkArrayComp(arrayP1)) {
			if(arrayP1.size() == 1) {
				win = true;
			} 
		} else if(checkArrayComp(arrayP2)) {
			if(arrayP2.size() == 1) {
				win = true;
			}
		}
		playerTurn = !playerTurn;
		if(win) {
			output(numTurns, winTrans, double(lastTrans)/double(numTurns));
			return;
		}
	}

}

int main(int argc, char** argv) {
	int playerCount = 2;
	if(argc < 2) {
		cout << "Error, not enough arguments. Program now exitting.";
		return 1;
	}
	string filename = ( argc > 2 ? argv[2] : "/dev/null" );

	//if (argc == 4) {								//An optional 3rd argument to specify number of players
	//	playerCount = stoi(argv[3]);
	//}
	string game = argv[1];
	if(game.compare("war") == 0 || game.compare("War") == 0) {
		//cout << "war";
		war(filename, playerCount);
	} else if (game.compare("trash") == 0 || game.compare("Trash") == 0) {
		trash(filename, playerCount);
	}
	return 0;
}