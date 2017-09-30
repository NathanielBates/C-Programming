/*
Nathaniel Bates
PID: 3957579
Course: COP3223C, Section: 2
Casino Assignment
3/13/16
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define ChipCost	11			/*Defined constants for the cost of a chip and the amount the casino buys back a chip.*/
#define BuyBack		10

int option, money = 1000, x, point, BetAmount, TotalChips;		/*Global variables used in the different functions.*/
char roll;														/*Variable used for scanf function when user rolls dice.*/

void CasinoOptions()										/*Function containing the options for the casino.*/				
{
	printf("Welcome to the Casino. Here are your choices:\n");
	printf("1) Buy Chips\n");
	printf("2) Sell Chips\n");
	printf("3) Play Craps\n");
	printf("4) Play Arup's Game of Dice\n");
	printf("5) Status Report\n");
	printf("6) Quit\n\n");
	printf("Enter your choice: ");
	scanf("%d", &option);

	while (option < 1 || option > 6)						/*Input validation so user enters a valid option.*/
	{
		printf("\nPlease enter a value from 1 to 6!\nTry Again: ");
		scanf("%d", &option);
	}

}

int pairofdice()						/*Function that generates two random number from 1 to 6, returns the sum of the roll, and displays what the user rolled.*/
{
	int d1, d2, k;
	srand(GetTickCount());
	d1 = (rand() % 6) + 1;
	d2 = (rand() % 6) + 1;
	k = d1 + d2;
	printf("You rolled a %d\n", k);

	return k;
}

int craps()															/*Function for the craps game*/
{
	printf("Press 'r' and hit enter for your first roll.\n");		/*Prompt for user to roll the dice, pair of dice function ins't called until r is entered.*/
	scanf("%s", &roll);
	point = pairofdice();											/*K is returned from dice function and stored in the point variable.*/

	if (point == 7 || point == 11)
	{
		TotalChips += (BetAmount * 2);								/*If a 7 or 11 is rolled, user wins double what they bet. 1 is returned.*/
		return 1;
	}

	else if (point == 2 || point == 3 || point == 12)				/*If a 2, 3, or 12 is rolled, user loses and a 0 is returned.*/
	{
		return 0;
	}

	else
	{
		while (x != 7 || x != point)								/*Loop continues to prompt user to roll until a 7 or the orginal point value is returned.*/
		{
			printf("Press 'r' and hit enter for your next roll.\n");
			scanf("%s", &roll);
			x = pairofdice();										/*x variable captures the return value from the dice function, used to compare previous roll.*/

			if (x == 7)
			{
				return 0;											/*If 7 rolled user loses, 0 is returned.*/
			}

			else if (x == point)
			{
				TotalChips += (BetAmount * 2);						/*If point is rolled, user wins double what they bet. 1 is returned.*/
				return 1;
			}
		}
	}
}

int arupsdice()														/*Function for Arup's Game of Dice.*/
{

	printf("Press 'r' and hit enter for your first roll.\n");		/*Prompt for the user to roll.*/
	scanf("%s", &roll);

	point = pairofdice();											/*K is returned from dice function and stored in the point variable.*/

	if (point == 12 || point == 11)
	{
		TotalChips += (BetAmount * 2);								/*If an 11 or 12 is rolled, user wins double what they bet and a 1 is returned.*/
		return 1;
	}

	else if (point == 2)											/*User loses if a 2 is rolled, 0 is returned.*/
	{
		return 0;
	}

	else
	{
		printf("Press 'r' and hit enter for your next roll.\n");	/*User prompted to roll one more time.*/
		scanf("%s", &roll);

		x = pairofdice();											/*Function returns second value, stored in x variable.*/

		if (x > point)												/*If second value is greater than the point then user wins.*/
		{
			TotalChips += (BetAmount * 2);							/*User wins double what they bet and a 1 is returned.*/
			return 1;
		}

		else
		{
			return 0;												/*Second value is less then point, user loses and 0 is returned.*/
		}
	}

}

int buychips(int *cash)												/*Function for user to purchase chips*/
{
	int AmountSpent, chips, BuyChips;

	printf("\nHow much cash do you want to spend for chips?\n");		/*Prompts user to enter the amount of money to spend.*/
	scanf("%d", &BuyChips);

	if (BuyChips < ChipCost)										/*Ensures user enters a value greater than or equal to chip cost.*/
	{
		printf("Chips cost $11 each. Please enter that amount or greater.\n");
		chips = 0;													/*If invalid entry, number of chips is 0 when returned.*/
	}

	else if (BuyChips > money)										/*Ensures user enters a value less than their total amount of money.*/
	{
		printf("Sorry you don't have that much money. No chips bought.\n");
		chips = 0;													/*If invalid entry, number of chips is 0 when returned.*/
	}

	else
	{																/*If amount of money is valid, pointer cash is set equal to the amount of cash spent (BuyChips) memory address.*/
		cash = &BuyChips;											/*So, *cash is equal to the amount spent.*/
		chips = *cash / ChipCost;									/*Number of chips that can be bought is determined.*/
		money = money - *cash;										/*Money left without the change received.*/
		AmountSpent = chips * ChipCost;								/*Amount that the chips costs.*/
		*cash = BuyChips - AmountSpent;								/*cash is set to the change the user will receive, as per instructions in assignment.*/
		money += *cash;												/*Change added to user's total amount of money.*/
	}
	printf("\n");
	return chips;													/*Number of chips purchased is returned.*/
}

int sellchips(int numchips)											/*Function for user to sell chips.*/
{
	int CashReceived;												/*Variable holding the amount of cash received for selling chips.*/

	printf("\nHow many chips would you like to sell back?\n");		/*Prompts user to enter the number of chips to sell.*/
	scanf("%d", &numchips);

	if (numchips <= 0)												/*Ensure user enters a value greater than 0.*/
	{
		printf("Please enter a value greater than 0.\n");			
		CashReceived = 0;											/*If invalid entry, CashReceived is 0 when returned.*/
	}

	else if (numchips > TotalChips)									/*Ensures user enters a number of chips less than or equal to their total chip count.*/
	{
		if (TotalChips == 0)
		{
			printf("You have no chips to sell.\n");					/*Informs user they are out of chips, and 0 is returned.*/
		}

		else
		{
			printf("Your chip count is %d. Please enter that value or less.\n", TotalChips);		/*Message displays tell the user their chip count, and 0 is returned.*/
		}
		
		CashReceived = 0;											
	}

	else
	{
		CashReceived = (numchips * BuyBack);						/*Calculates the amount of money paid for the chips. That value is returned.*/
		TotalChips -= numchips;										/*Number of chips sold subtracted from total chip count.*/						
	}

	printf("\n");
	return CashReceived;
}

void statusreport(int numchips, int cash)							/*Function displays the total number of chips and money user has.*/
{
	printf("\nYou currently have $%d left and %d chips.\n\n", cash, numchips);
}

int ChipsToBet()												/*Function that handles the bet for each game.*/
{	
	
	printf("\nHow many chips would you like to bet?\n");			/*Prompts user for the amount to bet.*/
	scanf("%d", &BetAmount);

	if (BetAmount <= 0)											/*Ensures user enters a value greater than 0.*/
	{
		printf("Sorry, that is not allowed. No game played.\n\n");
		BetAmount = 0;
		return 0;												/*If invalid entry, 0 is returned and BetAmount reset to 0.*/
	}

	else if (BetAmount > TotalChips)							/*Ensures user enters a value less than the total chip count. */
	{
		printf("You don't have enough chips! Your current chip count is %d.\n\n", TotalChips);
		BetAmount = 0;
		return 0;												/*If invalid entry, 0 is returned and BetAmount reset to 0.*/
	}

	else
	{
		TotalChips -= BetAmount;								/*User's entry is valid and chip count is decreased by that amount. */
		return 1;
	}
}

int main()
{
	int play, WinOrLoss;						/*If play variable = 1, then the game can be played.*/
	do
	{											/*If WinOrLoss = 0, user lost; If WinOrLoss = 1, user wins.*/
		CasinoOptions();						/*Function for the casino options called.*/
		if (option == 1)						/*Buy Chips*/
		{
			TotalChips += buychips(0);			/*Number of chips bought returned and added to total.*/
		}

		else if (option == 2)					/*Sell Chips*/
		{
			money += sellchips(0);				/*Money received from sale of chips is added to the total.*/
		}

		else if (option == 3)					/*Craps Game*/
		{
			play = ChipsToBet();				/*The betting function called, if 1 is returned then game is played.*/
			if (play == 1)
			{
				WinOrLoss = craps();			/*Function called and a 1 or 0 is returned.*/

				if (WinOrLoss == 0)
				{
					printf("Sorry you have lost.\n\n");		/*Output if the user loses.*/
				}

				else
				{
					printf("You Win!\n\n");					/*Output if user wins.*/
				}
			}

			BetAmount = 0;						/*BetAmount reset to 0 for future games.*/
		}

		else if (option == 4)					/*Arup's Game of Dice*/
		{
			play = ChipsToBet();				/*The betting function called, if 1 is returned then game is played.*/
			if (play == 1)
			{
				WinOrLoss = arupsdice();		/*Function called and a 1 or 0 is returned.*/

				if (WinOrLoss == 0)
				{
					printf("Sorry you have lost.\n\n");		/*Output if the user loses.*/
				}

				else
				{
					printf("You Win!\n\n");					/*Output if user wins.*/
				}
			}

			BetAmount = 0;						/*BetAmount reset to 0 for future games.*/			
		}

		else if (option == 5)					/*Status Report*/
		{
			statusreport(TotalChips, money);	/*Status report function called, total amount of chips and money passed into function.*/
		}

		else									/*User Quits Game*/
		{
			money += (TotalChips * BuyBack);								/*All chips are sold back, money received added to total.*/
			printf("After selling your chips, you have $%d.\n", money);		/*Display user's total amount of money and thanks them for playing.*/
			printf("Thanks for playing!\n\n");
		}

	} while (option == 0 || option < 6);		/*Do while loop used to continually display casino options and games until user selects option 6 to quit.*/

	system("pause");
	return 0;
}