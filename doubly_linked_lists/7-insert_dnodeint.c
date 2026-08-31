#include <stdlib.h>
#include "lists.h"

/**
 * insert_dnodeint_at_index - Inserts a new node at a given position.
 * @h: Double pointer to the head of the doubly linked list.
 * @idx: Index of the list where the new node should be added (starts at 0).
 * @n: Integer value for the new node.
 *
 * Return: Address of the new node, or NULL if it failed or if it's not
 *         possible to add the new node at index idx.
 */
dlistint_t *insert_dnodeint_at_index(dlistint_t **h, unsigned int idx, int n)
{
	dlistint_t *new_node, *current;
	unsigned int i;

	if (h == NULL)
		return (NULL);

	/* Inserting at index 0 is equivalent to adding at head */
	if (idx == 0)
		return (add_dnodeint(h, n));

	current = *h;

	/* Traverse the list to reach the node just before index idx */
	for (i = 0; current != NULL && i < idx - 1; i++)
		current = current->next;

	/* Index is out of bounds */
	if (current == NULL)
		return (NULL);

	/* If inserting at the end of the list */
	if (current->next == NULL)
		return (add_dnodeint_end(h, n));

	/* Inserting in the middle */
	new_node = malloc(sizeof(dlistint_t));
	if (new_node == NULL)
		return (NULL);

	new_node->n = n;
	new_node->next = current->next;
	new_node->prev = current;
	current->next->prev = new_node;
	current->next = new_node;

	return (new_node);
}
