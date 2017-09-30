/*
Nathaniel Bates
COP 3223C
BlackJack Assignment
*/

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define CLUBS 0						/*Constants used for suit determination.*/
#define DIAMONDS 1
#define HEARTS 2
#define SPADES 3

enum{ TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };	/*Initialized ranks for different cards in the deck.*/

int Deck[52];						/*Global variables and arrays used in multiple functions.*/
int CurrentDealCard;
int Dealer[20], Player[20];
int DealerCards, PlayerCards;
int DealerScore, PlayerScore;

int money = 1000;					/*Player starts off with 1000 chips.*/

void DoShuffle()
{
	int i, nextcard;
	int Used[52];

	/* Here we clear out Used array to zeros, which indicates no
	values have been used. */
	for (i = 0; i < 52; i++) Used[i] = 0;

	/* Loop through the deck of cards, there are 52 values */
	for (i = 0; i < 52; i++)
	{
		/* Here we used a do-while. If the card has already been used,
		we need to keep generating random numbers until we find a card
		that has not been used. */
		do
		{
			nextcard = rand() % 52; /* Value from 0 to 51 */

		} while (Used[nextcard] == 1); /* This is our check */


		/* Here we set to 1 so that we remember that this card has been used */
		Used[nextcard] = 1;

		/* Finally, put the card in the deck. */
		Deck[i] = nextcard;
	}
}

void DrawCard(int rank, int suit)	/*Function prints out the card that is dealt.*/
{

	switch (rank)
	{
	case TWO:
		printf("Two ");
		break;
	case THREE:
		printf("Three ");
		break;
	case FOUR:
		printf("Four ");
		break;
	case FIVE:
		printf("Five ");
		break;
	case SIX:
		printf("Six ");
		break;
	case SEVEN:
		printf("Seven ");
		break;
	case EIGHT:
		printf("Eight ");
		break;
	case NINE:
		printf("Nine ");
		break;
	case TEN:
		printf("Ten ");
		break;
	case JACK:
		printf("Jack ");
		break;
	case QUEEN:
		printf("Queen ");
		break;
	case KING:
		printf("King ");
		break;
	case ACE:
		printf("Ace ");
		break;
	}

	switch (suit)
	{
	case CLUBS:
		printf("Clubs");
		break;
	case DIAMONDS:
		printf("Diamonds");
		break;
	case HEARTS:
		printf("Hearts");
		break;
	case SPADES:
		printf("Spades");
		break;
	}

}

void DealCards()	/*Function that deals out the first two cards for each player.*/
{
	PlayerCards = DealerCards = CurrentDealCard = 0;		/*Intitialized to 0.*/

	Player[PlayerCards++] = Deck[CurrentDealCard++];		/*Players first card equals first card in shuffled deck.*/
	Dealer[DealerCards++] = Deck[CurrentDealCard++];		/*Dealers first card equals second card in shuffled deck.*/
	Player[PlayerCards++] = Deck[CurrentDealCard++];		/*Players second card equals third card in shuffled deck.*/
	Dealer[DealerCards++] = Deck[CurrentDealCard++];		/*Dealers second card equals fourth card in shuffled deck.*/

}

void DisplayDealtCards()
{
	int i;

	printf("\nDealer:\n");
	for (i = 0; i < DealerCards - 1; i++) DrawCard(Dealer[i] % 13, Dealer[i] / 13), printf("\n");	/*Terminating condition set to DealerCards - 1, so only one card is shown.*/
	printf("\nPlayer:\n");
	for (i = 0; i < PlayerCards; i++) DrawCard(Player[i] % 13, Player[i] / 13), printf("    ");		/*Both of the players cards are shown.*/
	printf("Player Score is %d\n\n", PlayerScore);													/*Player's total score so far is shown.*/

}

void DisplayDealtCards2()		/*Second Function created for when it is the dealers turn to draw to 17.*/
{
	int i;

	printf("\nDealer:\n");
	for (i = 0; i < DealerCards; i++) DrawCard(Dealer[i] % 13, Dealer[i] / 13), printf("    ");		/*All of the dealers cards are now shown.*/
	printf("Dealer Score is %d\n\nPlayer:\n", DealerScore);											/*Dealers total score is now shown.*/
	for (i = 0; i < PlayerCards; i++) DrawCard(Player[i] % 13, Player[i] / 13), printf("    ");		/*All of the player's cards are shown.*/
	printf("Player Score is %d\n\n", PlayerScore);													/*Player's total score is shown.*/

}
void DisplayChoice()					/*Display option includes the choice to double down.*/
{
	printf("What would you like to do?\n");
	printf("1. Hit\n2. Stand\n3. Double Down\n");
}

void DisplayChoice2()					/*Display option doesn't include double down, since it wasn't chosen when it was allowed.*/
{
	printf("What would you like to do?\n");
	printf("1. Hit\n2. Stand\n");
}

int GetChoice(int min, int max)			/*Function to get user choice, and the choice is validated.*/
{
	int choice;

	do
	{
		printf("Make a choice from %d to %d:", min, max);
		scanf("%d", &choice);
	} while (choice < min || choice > max);

	return(choice);
}


void CalculateScores()					/*Function to calculate scores.*/
{
	int i, rank, suit;

	PlayerScore = 0;					/*Player score initialized to 0.*/
	for (i = 0; i < PlayerCards; i++)	/*Loops through all of the player's cards.*/
	{
		rank = Player[i] % 13;			/*Suit and rank determined for each card.*/
		suit = Player[i] / 13;

		if (rank >= TWO && rank <= TEN) PlayerScore += (rank + 2);			/*If the rank is between 2 and 10, appropriate score is added.*/
		
		else if (rank >= JACK && rank <= KING) PlayerScore += 10;			/*If the rank is a face card, 10 is added.*/
		
		else PlayerScore += 11;												/*If the card is an Ace, 11 is added.*/
	
	}

	for (i = 0; i < PlayerCards; i++)						/*Loops through each of the player's cards.*/
	{
		if ((Player[i] % 13 == ACE) && PlayerScore > 21)	/*If there is an Ace and the total score is over 21, 10 is deducted to give an Ace a value of 1.*/
		{
			PlayerScore -= 10;
		}
	}

	DealerScore = 0;								/*Dealer score initialized to 0.*/
	for (i = 0; i < DealerCards; i++)				/*Loops through all of the dealer's cards.*/
	{
		rank = Dealer[i] % 13;						/*Suit and rank determined for each card.*/
		suit = Dealer[i] / 13;

		if (rank >= TWO && rank <= TEN) DealerScore += (rank + 2);	/*If the rank is between 2 and 10, appropriate score is added.*/

		else if (rank >= JACK && rank <= KING) DealerScore += 10;	/*If the rank is a face card, 10 is added.*/

		else DealerScore += 11;						/*If the card is an Ace, 11 is added.*/
		
	}
	
	for (i = 0; i < DealerCards; i++)						/*Loops through each of the dealer's cards.*/
	{
		if ((Dealer[i] % 13 == ACE) && DealerScore > 21)	/*If there is an Ace and the total score is over 21, 10 is deducted to give an Ace a value of 1.*/
		{
			DealerScore -= 10;
		}
	}

}

void DisplayWinner(int chips)										/*Function displays the winner and tells if either player went over 21. If player Wins, money won added to total.*/
{
	if (DealerScore > PlayerScore && DealerScore <= 21 )
	{
		printf("Dealer Wins!!!\nChip count is now %d!\n\n", money);
	}
	
	else if (PlayerScore > DealerScore && PlayerScore <= 21)
	{
		money += (2 * chips);
		printf("Player Wins!!!\nChip count is now %d!\n\n", money);
	}
	
	else if (PlayerScore == DealerScore)
	{
		money += chips;
		printf("It's a Push!!!\nPlayer gets their chips back!\n\n");
	}

	else if (PlayerScore > 21)
	{
		printf("Player Busts, Dealer Wins!!!\nChip count is now %d!\n\n", money);
	}

	else if (DealerScore > 21)
	{
		money += (2 * chips);
		printf("Dealer Busts, Player Wins!!!\nChip count is now %d!\n\n", money);
	}
}

void Display()		/*Welcome Menu for the Blackjack game.  Lets player know their chip count.*/
{
	
		printf("************************************************\n");
		printf("*   Welcome to Blackjack Knights!!!            *\n");
		printf("*   1. Deal cards                              *\n");
		printf("*   2. Quit this program                       *\n");
		printf("************************************************\n");
		printf("You have %d chips to play with!\n\n", money);
}

int Betting()		/*Betting function. Verifies player has enough chips to bet. Returns the number of chips.*/
{
	int chips;
	
	do
	{
		printf("How many chips would you like to bet?\n");
		scanf("%d", &chips);

		if (chips > money)
		{
			printf("You don't have enough chips! Chip count is %d.\n", money);
		}
		

	} while (chips > money || chips < 1);

	return chips;
}

int main()
{
	int option, choice, chips;						/*Choice and quit variables used when player is selecting an option.*/
	
	do
	{												/*Do while loop, game continues until player chooses to quit.*/
		Display();
		option = GetChoice(1, 2);

		if (option == 1)
		{
			chips = Betting();
			money -= chips;
			srand(GetTickCount());				/*Random number generator is seeded.*/

			DoShuffle();						/*Cards in the deck are shuffled and randomized differently each time program runs.*/

			DealCards();						/*Two cards are dealt to the player and the dealer.*/
			CalculateScores();					/*Scores calculated*/
			DisplayDealtCards();				/*Player's cards and score are shown, Only one of the dealer's cards are shown and the score is hidden.*/

			DisplayChoice();					/*Display options for the user including the option to double down.*/
			choice = GetChoice(1, 3);

			if (choice == 1)					/*Hit is selected.*/
			{
				Player[PlayerCards++] = Deck[CurrentDealCard++];			/*Player recieves another card.*/
				CalculateScores();											/*New score is calculated.*/
				DisplayDealtCards();										/*All of the player's cards are shown as well as the score.*/

				while (PlayerScore <= 21)					/*Loops until the player's score is 21 or higher.*/
				{
					DisplayChoice2();						/*Asks player if they want to hit or stand.*/
					choice = GetChoice(1, 2);

					if (choice == 1)						/*Player hits and another card is dealt, score is calculated, score and cards shown.*/
					{
						Player[PlayerCards++] = Deck[CurrentDealCard++];
						CalculateScores();
						DisplayDealtCards();
					}

					else break;								/*Player Stands, breaks out of loop, choice now equals 2 and the dealer is dealt a card until the score is 17.*/

				}

			}

			if (choice == 2)								/*Player stands.*/
			{
				if (DealerScore < 17)						/*If dealer's score is less than 17, cards are dealt until the score is at least that high.*/
				{
					while (DealerScore < 17)
					{
						Dealer[DealerCards++] = Deck[CurrentDealCard++];
						CalculateScores();
						DisplayDealtCards2();
					}
				}

				else
				{
					DisplayDealtCards2();					/*If the dealer already has a score of 17 or higher, the cards and the score is shown.*/
				}
			}

			if (choice == 3)								/*Player chooses to double down.*/
			{
				if (money >= chips)
				{
					money -= chips;							/*Money and chip counts adjusted to reflect double the intitial bet was placed.*/
					chips += chips;

					Player[PlayerCards++] = Deck[CurrentDealCard++];			/*Since player doubled down, only one card is dealt.*/
					CalculateScores();
					DisplayDealtCards2();

					if (PlayerScore <= 21)										/*If player doesn't bust or hit 21, dealer is dealt cards until it's score is 17.*/
					{
						while (DealerScore < 17)
						{
							Dealer[DealerCards++] = Deck[CurrentDealCard++];
							CalculateScores();
							DisplayDealtCards2();
						}
					}
				}
				
				else if (money < chips)
				{
					
					printf("You don't have enough to double your initial bet.\n");		/*Player doesn't have enough to double, hit or stand option displayed.*/
						while (PlayerScore <= 21)										/*Loops until the player's score is 21 or higher.*/
						{
							DisplayChoice2();											/*Asks player if they want to hit or stand.*/
							choice = GetChoice(1, 2);

							if (choice == 1)											/*Player hits and another card is dealt, score is calculated, score and cards shown.*/
							{
								Player[PlayerCards++] = Deck[CurrentDealCard++];
								CalculateScores();
								DisplayDealtCards();
							}

							else break;													/*Player Stands, breaks out of loop, choice now equals 2 and the dealer is dealt a card until the score is 17.*/

						}

						if (DealerScore < 17)						/*If dealer's score is less than 17, cards are dealt until the score is at least that high.*/
						{
							while (DealerScore < 17)
							{
								Dealer[DealerCards++] = Deck[CurrentDealCard++];
								CalculateScores();
								DisplayDealtCards2();
							}
						}

						else
						{
							DisplayDealtCards2();					/*If the dealer already has a score of 17 or higher, the cards and the score is shown.*/
						}
				}
			}

			DisplayWinner(chips);											/*Winner is displayed. Chips passed in to update player's chip total (money).*/
			
		}

		if (money < 1)
		{
			printf("You are out of chips! Thanks for playing!\n\n");		/*If player has zero chips, program exits.*/
			option = 2;
		}

	} while (option != 2);
	
	system("pause");
	return 0;
}

