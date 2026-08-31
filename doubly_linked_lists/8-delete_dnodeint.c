#include <stdlib.h>
#include "lists.h"

/**
 * delete_dnodeint_at_index - Deletes the node at index of a dlistint_t list.
 * @head: Double pointer to the head of the doubly linked list.
 * @index: Index of the node that should be deleted (starts at 0).
 *
 * Return: 1 if it succeeded, -1 if it failed.
 */
int delete_dnodeint_at_index(dlistint_t **head, unsigned int index)
{
	dlistint_t *current;
	unsigned int i;

	if (head == NULL || *head == NULL)
		return (-1);

	current = *head;

	/* Deleting the head node (index 0) */
	if (index == 0)
	{
		*head = current->next;
		if (*head != NULL)
			(*head)->prev = NULL;
		free(current);
		return (1);
	}

	/* Traverse to the node at index */
	for (i = 0; current != NULL && i < index; i++)
		current = current->next;

	/* Index is out of bounds */
	if (current == NULL)
		return (-1);

	/* Update pointers of surrounding nodes */
	if (current->next != NULL)
		current->next->prev = current->prev;

	current->prev->next = current->next;

	free(current);
	return (1);
}
