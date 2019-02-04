/*  This program takes user-inputted integers, one per line, until the
	user hits Control-D.

	Written for SoftSys Exercise 2 by Mary Keenan. */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*  Runs the input loop until the user hits Control-D 
*/
void main() {

	int array_size = 100;
	int buffer_length = 10;
	int inputted_number_string[buffer_length]; /* numbers < 10^buffer_length can be entered */
	int inputted_numbers[array_size]; /* array_size numbers can be entered */
	int index = 0;

	while (!feof(stdin)) {

		if (index > array_size) {
			printf("The max array size of %i has been exceeded\n", array_size);
			return;
		}

		scanf("%11s", inputted_number_string);

		if (strlen(inputted_number_string) > buffer_length) {
			printf("The input exceeds the buffer length of %i\n", buffer_length);
			return;
		}

		inputted_numbers[index] = atoi(inputted_number_string);

		check_for_conversion_error(inputted_numbers[index], inputted_number_string, 
			strlen(inputted_number_string));

		index++;
	}

	/* index - 1 bc an extra input was added when ctrl-d was entered */
	count(inputted_numbers, index - 1);
}


/*  Checks if the user-inputted string can be converted to an integer
	and displays error message if not.

	converted_value: int value of the string after being converted with atoi()
	input_string: character pointer for the array that stores the inputted string
	input_string_len: int value of the length of the inputted string
*/
void check_for_conversion_error(int converted_value, char * input_string, 
	int input_string_len) {

	/* atoi() returns a 0 if it can't convert the inputted string */
	if (converted_value == 0) {
		/* we'll run through the individual string characters to make sure 
		there was an error and the user didn't just input 0 */
		for (int input_index = 0; input_index < input_string_len; 
			input_index++) {
			if (input_string[input_index] != "0") {
				printf("input could not be converted to an integer\n");
				break;
			}
		}
	}
}


/*  Sums all of the user-inputted values.

	inputted_numbers: int pointer for the array of converted-to-integer values
	length: int value of the length of the inputted string
*/
void count(int * inputted_numbers, int length) {
	
	int sum = 0;

	for (int n = 0; n < length; n++) {
		sum += inputted_numbers[n];
	}

	printf("%i\n", sum);
}


