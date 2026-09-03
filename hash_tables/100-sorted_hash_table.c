#include "hash_tables.h"

/**
 * add_to_sorted - Inserts a node into the sorted linked list.
 * @ht: Pointer to the sorted hash table
 * @node: Pointer to the node to insert
 */
void add_to_sorted(shash_table_t *ht, shash_node_t *node)
{
	shash_node_t *tmp;

	if (!ht->shead)
	{
		ht->shead = ht->stail = node;
		node->sprev = node->snext = NULL;
		return;
	}
	if (strcmp(node->key, ht->shead->key) < 0)
	{
		node->snext = ht->shead;
		node->sprev = NULL;
		ht->shead->sprev = node;
		ht->shead = node;
		return;
	}
	tmp = ht->shead;
	while (tmp->snext && strcmp(node->key, tmp->snext->key) > 0)
		tmp = tmp->snext;
	if (!tmp->snext)
	{
		tmp->snext = node;
		node->sprev = tmp;
		node->snext = NULL;
		ht->stail = node;
	}
	else
	{
		node->snext = tmp->snext;
		node->sprev = tmp;
		tmp->snext->sprev = node;
		tmp->snext = node;
	}
}

/**
 * shash_table_create - Creates a sorted hash table.
 * @size: Size of the array
 * Return: Pointer to the table, or NULL on failure
 */
shash_table_t *shash_table_create(unsigned long int size)
{
	shash_table_t *ht = malloc(sizeof(shash_table_t));
	unsigned long int i;

	if (!ht)
		return (NULL);
	ht->size = size;
	ht->array = malloc(sizeof(shash_node_t *) * size);
	if (!ht->array)
	{
		free(ht);
		return (NULL);
	}
	for (i = 0; i < size; i++)
		ht->array[i] = NULL;
	ht->shead = ht->stail = NULL;
	return (ht);
}

/**
 * shash_table_set - Adds an element to the sorted hash table.
 * @ht: Pointer to the table
 * @key: The key, string
 * @value: The value associated with the key
 * Return: 1 if successful, 0 otherwise
 */
int shash_table_set(shash_table_t *ht, const char *key, const char *value)
{
	unsigned long int idx;
	shash_node_t *node, *tmp;
	char *val_dup;

	if (!ht || !key || !*key || !value)
		return (0);
	idx = key_index((const unsigned char *)key, ht->size);
	tmp = ht->array[idx];
	while (tmp)
	{
		if (!strcmp(tmp->key, key))
		{
			val_dup = strdup(value);
			if (!val_dup)
				return (0);
			free(tmp->value);
			tmp->value = val_dup;
			return (1);
		}
		tmp = tmp->next;
	}
	node = malloc(sizeof(shash_node_t));
	if (!node)
		return (0);
	node->key = strdup(key);
	node->value = strdup(value);
	if (!node->key || !node->value)
	{
		free(node->key);
		free(node->value);
		free(node);
		return (0);
	}
	node->next = ht->array[idx];
	ht->array[idx] = node;
	add_to_sorted(ht, node);
	return (1);
}

/**
 * shash_table_get - Retrieves a value associated with a key.
 * @ht: Pointer to the table
 * @key: The key to look for
 * Return: The value, or NULL if not found
 */
char *shash_table_get(const shash_table_t *ht, const char *key)
{
	unsigned long int idx;
	shash_node_t *node;

	if (!ht || !key || !*key)
		return (NULL);
	idx = key_index((const unsigned char *)key, ht->size);
	node = ht->array[idx];
	while (node)
	{
		if (!strcmp(node->key, key))
			return (node->value);
		node = node->next;
	}
	return (NULL);
}

/**
 * shash_table_print - Prints a sorted hash table.
 * @ht: Pointer to the table
 */
void shash_table_print(const shash_table_t *ht)
{
	shash_node_t *n;
	int f = 0;

	if (!ht)
		return;
	printf("{");
	for (n = ht->shead; n; n = n->snext)
	{
		printf("%s'%s': '%s'", f ? ", " : "", n->key, n->value);
		f = 1;
	}
	printf("}\n");
}

/**
 * shash_table_print_rev - Prints a sorted hash table in reverse.
 * @ht: Pointer to the table
 */
void shash_table_print_rev(const shash_table_t *ht)
{
	shash_node_t *n;
	int f = 0;

	if (!ht)
		return;
	printf("{");
	for (n = ht->stail; n; n = n->sprev)
	{
		printf("%s'%s': '%s'", f ? ", " : "", n->key, n->value);
		f = 1;
	}
	printf("}\n");
}

/**
 * shash_table_delete - Deletes a sorted hash table.
 * @ht: Pointer to the table
 */
void shash_table_delete(shash_table_t *ht)
{
	shash_node_t *n, *tmp;

	if (!ht)
		return;
	for (n = ht->shead; n; n = tmp)
	{
		tmp = n->snext;
		free(n->key);
		free(n->value);
		free(n);
	}
	free(ht->array);
	free(ht);
}
