/* */

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>


int comparison_function_for_sorting(gpointer first, gpointer second) {
	int first_int = *((int *) first);
	int second_int = *((int *) second);
	return first_int - second_int;
}


int main(int argc, char** argv) {
	/* initialize the empty hash table and some pointers */
    GHashTable *hash_table = g_hash_table_new(g_str_hash, g_str_equal);
    gchar *word;
    gint count;

    /* read the text file -- used example in https://lzone.de/examples/Glib */
    gsize length; 
    gchar *text, *filename = "pride_and_prejudice.txt";
    if (g_file_get_contents (filename, &text, &length, NULL)) {
	    /* split the text into a list of words */
		/* setting the max_tokens field to 0 means the text will be completely
	    split up (as opposed to some max number of splits) */
		gint max_tokens = -1;
	    gchar ** array_of_words = g_strsplit(text, " ", max_tokens);
	    gchar ** word_ptr;
 
	    /* iterate through each word and update its count in the hash table */
	    for (word_ptr = array_of_words; *word_ptr; word_ptr++) {
	    	word = *word_ptr;
	    	gint word_length = strlen(word);

	    	/* iterate through the letters to try to strip the low-hanging
	    	punctuation fruit */
	    	for (gint i = 0; i < word_length; i++) {
	    		gchar letter = word[i];
	    		if (letter == '.' || letter == ',' || letter == ';' || letter == ':' || letter == "'" || letter == ' ' || letter == '!' || letter == '?') {
	    			word[i] = '\0';
	    		}
			}

			/* update the word's count or add it to the hash map if it's new */
			count = g_hash_table_lookup(hash_table, word);

			if (count == 0) {
				g_hash_table_insert(hash_table, word, 1);
			} else {
				g_hash_table_replace(hash_table, word, count + 1);
			}
	    }

	 	/* NOTE: I DID NOT ACTUALLY GET SORTING WORKING BECAUSE I
	 	OVERLOOKED THE FACT THAT YOU SORT KEYS BY THEIR VALUES BUT
	 	THE STUFF BELOW IS WHAT I HAD WHEN I REALIZED THAT*/

	    /* initialize objects needed to iterate through the hash table */
	    GHashTableIter table_iter;
        gint table_size = g_hash_table_size(hash_table);
        g_hash_table_iter_init (&table_iter, hash_table);

        /* initialize the list we'll add the keys to and sort */
        GArray* array_of_keys = g_array_new(FALSE, FALSE, sizeof(char *));

        /* add the hash table keys to the list (we're reusing earlier 
        pointers because it's climate action semester) */
        while (g_hash_table_iter_next (&table_iter, (gpointer) &word, (gpointer) &count))
        {
            g_array_append_val(array_of_keys, word);
        }

        /* sort the list */
        g_array_sort(array_of_keys, comparison_function_for_sorting);

        /* iterate through the sorted list to print the sorted 
        key-value pairs */
        for (gint i = 0; i < table_size; i++) {
        	gchar * word = g_array_index(array_of_keys, gchar *, i);
        	gint count = g_hash_table_lookup(hash_table, word);
        	printf("%s: %d\n", word, count);
        }

	    /* free the data structures we used */
	    g_strfreev(array_of_words);
    	g_free (text);
    	g_hash_table_destroy(hash_table);
    	g_array_free(array_of_keys, TRUE);
    }

    return 0;
}


// int main(int argc, char∗∗ argv) {
// 	GHashTable∗ hash = g_hash_table_new(g_str_hash, g_str_equal);
// 	g_hash_table_insert(hash, "Virginia", "Richmond");
// 	g_hash_table_insert(hash, "Texas", "Austin");
// 	g_hash_table_insert(hash, "Ohio", "Columbus");
// 	printf("There are %d keys in the hash\n", g_hash_table_size(hash));
// 	printf("The capital of Texas is %s\n", g_hash_table_lookup(hash, "Texas"));
// 	gboolean found = g_hash_table_remove(hash, "Virginia");
// 	printf("The value 'Virginia' was %sfound and removed\n", found ? "" : "not ");
// 	g_hash_table_destroy(hash);
// 	return 0;
// }
