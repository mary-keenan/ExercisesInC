/*  This program counts cards for a game of Blackjack.

	Adapted from pg 37 of Head First C by Mary Keenan. */


#include <stdio.h>
#include <stdlib.h>

/* declare functions */
void get_card_name();
void get_card_value(char * card_name);
void update_count(int val);

/* declare global variable */
char card_name[3];


/*  Runs the card-counting loop until the user inputs an X. 
*/
int main() {

	while (card_name[0] != 'X') 
	{
		get_card_name();
		get_card_value(card_name);
	}

	return 0;
}

/*  Prompts the user to enter a card name.

	User input is truncated to the first two characters.
 */
void get_card_name() {

	puts("Enter the card_name: ");
	scanf("%2s", card_name);
}


/*  Converts card names to card values.

	card_name: character array that stores the inputted card name
*/
void get_card_value(char * card_name) {

	int val = 0;

	switch(card_name[0]) {
		case 'K':
		case 'Q':
		case 'J':
			val = 10;
			break;
		case 'A': 
		/* A is a little weird to handle since it's value is 11 (>10 in check below), 
		but since it doesn't affect the count, we'll just skip that part */
		case 'X':
			return;
		default:
			val = atoi(card_name);
	}
	
	update_count(val);
}

/*  Increments or decrements the count based on valid card values. 

	val: integer value of the inputted card name
*/
void update_count(int val) {
	static int count = 0;

	if ((val < 2) || (val > 10)) {
		puts("I don't understand that value!");
	} else if ((val > 2) && (val < 7)) {
		count++;
	} else if (val == 10) {
		count--;
	}

	printf("Current count: %i\n", count);
}







