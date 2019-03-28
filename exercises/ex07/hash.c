/* Example code for Exercises in C.

Copyright 2016 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// VALUE: represents a value in a key-value pair

/* Here's one way of making a polymorphic object in C */

typedef struct {
    enum Type {INT, STRING} type;
    union {
        int i;
        char *s;
    };
} Value;


/* Makes a Value object that contains an int.
*
* i: value to store.
*
* returns: pointer to a new Value
*/
Value *make_int_value(int i)
{
    Value *value = (Value *) malloc (sizeof (Value));
    value->type = INT;
    value->i = i;
    return value;
}


/* Makes a Value object that contains a string.
*
* s: value to store.
*
* returns: pointer to a new Value
*/
Value *make_string_value(char *s)
{
    Value *value = (Value *) malloc (sizeof (Value));
    value->type = STRING;
    value->s = s;
    return value;
}


/* Prints a value object.
*
* value: pointer to Value
*
*/
void print_value (Value *value)
{
    if (value == NULL) {
        printf ("%p", value);
        return;
    }
    switch (value->type) {
        case INT:
        printf ("%d", value->i);
        break;
        case STRING:
        printf ("%s", value->s);
        break;
    }
}

// HASHABLE: Represents a key in a key-value pair.

/* Here's another way to make a polymorphic object.

The key can be any pointer type.  It's stored as a (void *), so
when you extract it, you have to cast it back to whatever it is.

`hash` is a pointer to a function that knows how to hash the key.
`equal` is a pointer to a function that knows how to compare keys.

*/

typedef struct {
    void *key;
    int (*hash) (void *);
    int (*equal) (void *, void *);
} Hashable;


/* Makes a Hashable object.
*
* key: pointer to anything
* hash: function that can hash keys
* equal: function that compares keys
*
* returns: pointer to Hashable
*
*/
Hashable *make_hashable(void *key, void* class)
{
    Hashable *hashable = (Hashable *) malloc (sizeof (Hashable));
    hashable->key = key;
    hashable->hash = class->hash;
    hashable->equal = class->hash;
    return hashable;
}


/* Prints a Hashable object.
*
* hashable: pointer to hashable
*/
void print_hashable(Hashable *hashable)
{
    printf ("key %p\n", hashable->key);
    printf ("hash %p\n", hashable->hash);
}


/* Hashes an integer.
*
* p: pointer to integer
*
* returns: integer hash value
*/
int hash_int(void *p)
{
    return *(int *)p;
}


/* Hashes a string.
*
* p: pointer to first char of a string
*
* returns: integer hash value
*/
int hash_string(void *p)
{
    char *s = (char *) p;
    int total = 0;
    int i = 0;

    while (s[i] != 0) {
        total += s[i];
        i++;
    }
    return total;
}


/* Hashes any Hashable.
*
* hashable: Hashable object
*
* returns: int hash value
*/
int hash_hashable(Hashable *hashable)
{
    return hashable->hash (hashable->key);
}


/* Compares integers.
*
* ip: pointer to int
* jp: pointer to int
*
* returns: 1 if equal, 0 otherwise
*/
int equal_int (void *ip, void *jp)
{
    // FILL THIS IN!
    if (* (int*) ip == * (int*) jp) {
        return 1;
    }

    return 0;
}


/* Compares strings.
*
* s1: pointer to first char of a string
* s2: pointer to first char of a string
*
* returns: 1 if equal, 0 otherwise
*/
int equal_string (void *s1, void *s2)
{
    // FILL THIS IN!

    /* I don't want to keep casting, so we'll just do that once */
    char * string1 = (char *) s1;
    char * string2 = (char *) s2;

    while (*string1 == *string2) {
        
        if (*string1 == '\0') { /* this means they're both finished */
            return 1;
        }

        /* increment the pointers of both strings to point to their 
        next char */
        string1++;
        string2++;
    }

    return 0;
}


/* Compares Hashables.
*
* h1: Hashable
* h2: Hashable of the same type
*
* returns: 1 if equal, 0 otherwise
*
*/
int equal_hashable(Hashable *h1, Hashable *h2)
{
    // FILL THIS IN!
    
    /* this uses the appropriate equal_X function on the keys */
    return h1->equal (h1->key, h2->key);
}



/* we save space by moving function pointers to class objects -- 
this class is for integer keys... */
typedef struct {
    int (*hash_int) (int *);
    int (*equal_int) (int *, int *);
} IntClass;

/* and this class is for string keys -- they both have the relevant
function pointers for their type */
typedef struct {
    int (*hash_string) (char *);
    int (*equal_string) (char *, char *);
} StringClass;


/* Makes a Hashable int.
*
* Allocates space and copies the int.
*
* x: integer to store
*
* returns: Hashable
*/
Hashable *make_hashable_int (int x)
{
    static IntClass int_class;
    int *p = (int *) malloc (sizeof (int));
    *p = x;
    return make_hashable((void *) p, int_class);
}


/* Makes a Hashable int.
*
* Stores a reference to the string (not a copy).
*
* s: string to store
*
* returns: Hashable
*/
Hashable *make_hashable_string (char *s)
{
    return make_hashable((void *) s, string_class->hash_string, string_class->equal_string);
}


// NODE: a node in a list of key-value pairs

typedef struct node {
    Hashable *key;
    Value *value;
    struct node *next;
} Node;


/* Makes a Node. */
Node *make_node(Hashable *key, Value *value, Node *next)
{
    Node *node = (Node *) malloc (sizeof (Node));
    node->key = key;
    node->value = value;
    node->next = next;
    return node;
}


/* Prints a Node. */
void print_node(Node *node)
{
    print_hashable(node->key);
    printf ("value %p\n", node->value);
    printf ("next %p\n", node->next);
}


/* Prints all the Nodes in a list. */
void print_list(Node *node)
{
    if (node == NULL) {
        return;
    }
    print_hashable(node->key);
    printf ("value %p\n", node->value);
    print_list(node->next);
}


/* Prepends a new key-value pair onto a list.

This is actually a synonym for make_node.
*/
Node *prepend(Hashable *key, Value *value, Node *rest)
{
    return make_node(key, value, rest);
}


/* Looks up a key and returns the corresponding value, or NULL */
Value *list_lookup(Node *list, Hashable *key)
{
    // FILL THIS IN!

    Node *temp = list;

    while (temp != NULL) {

        /* check if we've found the right key (and therefore value) */
        if (temp->key->key == key->key) {
            return temp->value;
        }

        temp = temp->next;
    }

    return NULL;
}


// MAP: a map is an array of lists of key-value pairs

typedef struct map {
    int n;
    Node **lists;
} Map;


/* Makes a Map with n lists. */
Map *make_map(int n)
{
    int i;

    Map *map = (Map *) malloc (sizeof (Map));
    map->n = n;
    map->lists = (Node **) malloc (sizeof (Node *) * n);
    for (i=0; i<n; i++) {
        map->lists[i] = NULL;
    }
    return map;
}


/* Prints a Map. */
void print_map(Map *map)
{
    int i;

    for (i=0; i<map->n; i++) {
        if (map->lists[i] != NULL) {
            printf ("%d\n", i);
            print_list (map->lists[i]);
        }
    }
}


/* Adds a key-value pair to a map. */
void map_add(Map *map, Hashable *key, Value *value)
{
    // FILL THIS IN!

    /* find the proper list of nodes to insert the pair in */
    int hash = hash_hashable(key) % map->n;
    Node *matching_list = (map->lists)[hash];

    /* insert the key-value before the first node in the list and
    update the list pointer in the map */
    Node *inserted_node = prepend(key, value, matching_list);
    (map->lists)[hash] = inserted_node;
}


/* Looks up a key and returns the corresponding value, or NULL. */
Value *map_lookup(Map *map, Hashable *key)
{
    // FILL THIS IN!

    /* find the proper list of nodes to lookup the key in */
    int hash = hash_hashable(key) % map->n;
    Node *matching_node = (map->lists)[hash];

    /* iterate through the list to find the corresponding key */
    return list_lookup(matching_node, key);
}


/* Prints the results of a test lookup. */
void print_lookup(Value *value)
{
    printf ("Lookup returned ");
    print_value (value);
    printf ("\n");
}


int main ()
{
    Hashable *hashable1 = make_hashable_int (1);
    Hashable *hashable2 = make_hashable_string ("Apple");
    Hashable *hashable3 = make_hashable_int (2);

    // make a list by hand
    Value *value1 = make_int_value (17);
    Node *node1 = make_node(hashable1, value1, NULL);
    print_node (node1);

    Value *value2 = make_string_value ("Orange");
    Node *list = prepend(hashable2, value2, node1);
    print_list (list);

    // run some test lookups
    Value *value = list_lookup (list, hashable1);
    print_lookup(value);

    value = list_lookup (list, hashable2);
    print_lookup(value);

    value = list_lookup (list, hashable3);
    print_lookup(value);

    // make a map
    Map *map = make_map(10);
    map_add(map, hashable1, value1);
    map_add(map, hashable2, value2);

    printf ("Map\n");
    print_map(map);

    // run some test lookups
    value = map_lookup(map, hashable1);
    print_lookup(value);

    value = map_lookup(map, hashable2);
    print_lookup(value);

    value = map_lookup(map, hashable3);
    print_lookup(value);

    return 0;
}
