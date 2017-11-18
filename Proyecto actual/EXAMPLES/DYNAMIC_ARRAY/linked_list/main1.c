/* ## (c) 2017, Arkadiusz Drabczyk, arkadiusz@drabczyk.org */

#include <stdio.h>
#include <stdlib.h>

struct node {
	int data;
	struct node *next;
};

int init(struct node **head, int data)
{
	*head = malloc(sizeof(struct node));
	if (!*head) {
		fprintf(stderr, "Failed to init a linked list\n");
		return 1;
	}

	(*head)->data = data;
	(*head)->next = NULL;

	return 0;
}

int insert(struct node **head, int data)
{
	struct node *current = *head;
	struct node *tmp;

	do {
		tmp = current;
		current = current->next;
	} while (current);

	/* create a new node after tmp */
	struct node *new = malloc(sizeof(struct node));
	if (!new) {
		fprintf(stderr, "Failed to insert a new element\n");
		return 1;
	}
	new->next = NULL;
	new->data = data;

	tmp->next = new;

	return 0;

}

void delete(struct node **head, int data)
{
	struct node *current = *head;
	struct node *prev = NULL;

	do {
		if (current->data == data) {
			break;
		}
		prev = current;
		current = current->next;
	} while (current);

	/* if the first element */
	if (current == *head) {
		/* reuse prev */
		prev = *head;
		*head = current->next;
		free(prev);
		return;
	}

	/* if the last element */
	if (current->next == NULL) {
		prev->next = NULL;
		free(current);
		return;
	}

	prev->next = current->next;
	free(current);
	return;
}

void print(struct node **head)
{
	struct node *current = *head;
	while (current) {
		printf("current data: %d, address: %p\n", current->data,
		       current);
		current = current->next;
	}
}

void reverse(struct node **head)
{
	struct node *current = *head, *newnext = NULL, *tmp;

	do {
		tmp = current->next;
		current->next = newnext;
		newnext = current;
		current = tmp;
	} while (current);

	*head = newnext;
	return;
}

void deinit(struct node **head)
{
	struct node *node = *head;
	do {
		struct node *tmp;
		tmp = node;
		node = node->next;
		free(tmp);
	} while (node);
}

int main(void)
{
	struct node *head;
	if (init(&head, 25) != 0) {
		fprintf(stderr, "Failed to init a new linked list");
		return EXIT_FAILURE;
	}

	insert(&head, 30);

	insert(&head, 55);
	insert(&head, 210);
	insert(&head, 1000000);

	puts("Print the linked list:");
	print(&head);
	puts("\nDelete first element.");
	delete(&head, 25);
	puts("Print the linked list:");
	print(&head);

	puts("\nDelete last element.");
	delete(&head, 1000000);
	puts("Print the linked list:");
	print(&head);

	puts("\nDelete middle element.");
	delete(&head, 55);
	puts("Print the linked list:");
	print(&head);

	puts("\nAdd more elements.");
	insert(&head, 1);
	insert(&head, 2);
	insert(&head, 3);
	puts("Print the linked list:");
	print(&head);

	puts("\nReverse the linked list");
	reverse(&head);
	puts("Print the linked list:");
	print(&head);
	deinit(&head);

	return EXIT_SUCCESS;
}
