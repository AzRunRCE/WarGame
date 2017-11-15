#include <stdlib.h>
#include "include/ft_item.h"

Item* pushItem(Item* head, Item *new_node)
{
	if (head == NULL) {
		head = new_node;
		return head;
	}
	/* go to the last node */
	Item *cursor = head;
	while (cursor->next != NULL)
		cursor = cursor->next;
	cursor->next = new_node;
	return head;
}

int countItemWall(Item* head)
{
	int total = 0;
	if (head == NULL) {
		return 0;
	}
	/* go to the last node */
	Item *cursor = head;
	while (cursor != NULL)
	{
		total++;
		if (cursor->next == NULL)
			break;
		cursor = cursor->next;
	}
	return total;
}