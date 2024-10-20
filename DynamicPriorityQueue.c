#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define NODE_LIMIT 50 // max amount of node in queue
#define DATA_LIMIT 1000 // Max DataSize

/* Structure Definitions */

struct QueueNode
{
	void* data;
	int priority;
	size_t DataSize;
	struct QueueNode* next;
	struct QueueNode* prev;
};

struct QueueManager
{
	char QueueID[30];
	int QueueNodeCount;
	struct QueueNode* tail;
	struct QueueNode* head;
};


/* Function Prototypes */

struct QueueManager* CreateQueue(const char* QueueID);
struct QueueNode* InsertQueueNode(struct QueueManager*, void* data, size_t DataSize);
struct QueueNode* GrabQueueNode(struct QueueManager*);
bool DeleteQueue(struct QueueManager*);


/* Functions */

struct QueueManager* CreateQueue(const char* QueueID)
{
	// creating an instance of QueueManager called Manager
	struct QueueManager* manager = calloc(1, sizeof(struct QueueManager));	// allocating memory for our queue manager on the heap

	// error checking QueueManager
	if (manager == NULL)
	{
		fprintf(stderr, "\nFailed to allocate memory for QueueManager\n");
		return NULL;
	}

	if (sizeof(QueueID) > sizeof(manager->QueueID) - 1)
	{
		fprintf(stderr, "\nQueueID exceeds max char limit\n");
		return NULL;
	}

	strncpy(manager->QueueID, QueueID, sizeof(manager->QueueID) - 1);

	manager->QueueNodeCount = 0;

	return manager;
}

struct QueueNode* InsertQueueNode(struct QueueManager* QueueManager, void* data, size_t DataSize)
{
	// error checking
	if (QueueManager == NULL)
	{
		fprintf(stderr, "\nFailed to allocate memory for QueueManager\n");
		return NULL;
	}

	if (data == NULL)
	{
		fprintf(stderr, "\ndata in InsertQueueNode is null\n");
		return NULL;
	}

	if (DataSize >= DATA_LIMIT)
	{
		fprintf(stderr, "\nInvalid DataSize\n");
		return NULL;
	}

	int QueueNodeCount = QueueManager->QueueNodeCount;

	if (QueueNodeCount >= NODE_LIMIT)
	{
		fprintf(stderr, "\nInvalid QueueNodeCount\n");
		return NULL;
	}

	// ennd of main error checking

	if (QueueNodeCount > 0 && QueueNodeCount < NODE_LIMIT)
	{
		struct QueueNode* node = calloc(1, sizeof(struct QueueNode));

		if (node == NULL)
		{
			fprintf(stderr, "\nnode in InsertQueueNode is null\n");
			return NULL;
		}


		node->DataSize = DataSize;
		node->data = data;
		node->next = QueueManager->head;
		node->priority = QueueManager->head->priority + 1;
		QueueManager->QueueNodeCount += 1;
		QueueManager->head->prev = node;
		QueueManager->head = node;

	}
	else if (QueueNodeCount == 0) // creating the head node
	{
		struct QueueNode* node = calloc(1, sizeof(struct QueueNode));

		if (node == NULL)
		{
			fprintf(stderr, "\nnode in InsertQueueNode is null\n");
			return NULL;
		}

		node->priority = 1; // 1 is the highest priority
		node->DataSize = DataSize;
		node->data = data;
		QueueManager->QueueNodeCount = 1;
		QueueManager->tail = node;
		QueueManager->head = node;
	}

	else
	{
		fprintf(stderr, "\nInvalid QueueNodeCount\n");
		return NULL;
	}

	return QueueManager->head;

}

struct QueueNode* GrabQueueNode(struct QueueManager* QueueManager)
{
	// error checking
	if (QueueManager == NULL)
	{
		fprintf(stderr, "\nFailed to allocate memory for QueueManager\n");
		return NULL;
	}

	if (QueueManager->tail->data == NULL)
	{
		fprintf(stderr, "\nNo data exists for requested node. Node has still been popped\n");
	}

	struct QueueNode* OldTail = QueueManager->tail;
	void* DataReturn = calloc(1, OldTail->DataSize);

	if (DataReturn == NULL) // error handling for calloc
    {
    	fprintf(stderr, "\nMemory allocation failed\n");
        return NULL;
    }

    memcpy(DataReturn, OldTail->data, OldTail->DataSize);

    if (QueueManager->QueueNodeCount > 1)
    {
    	QueueManager->tail = QueueManager->tail->prev;
    	QueueManager->tail->next = NULL;
    	free(OldTail);
    	QueueManager->QueueNodeCount -= 1;

    	return DataReturn;
    }
    else
    {
    	free(OldTail);
    	QueueManager->QueueNodeCount -= 1;
    	QueueManager->tail = NULL;
    	QueueManager->head = NULL;
    	return DataReturn; // this must be freed on the users end
	}

}

bool DeleteQueue(struct QueueManager* QueueManager)
{
	// error checking
	if (QueueManager == NULL)
	{
		fprintf(stderr, "\nFailed to allocate memory for QueueManager\n");
		return false;
	}

	while (QueueManager->tail != NULL)
	{
		struct QueueNode* OldTail = QueueManager->tail;
		QueueManager->tail = QueueManager->tail->prev;
		free(OldTail);
	}

	if (QueueManager->tail == NULL)
	{
		free(QueueManager);
	}
	else
	{
		return false;
	}


	return true;

}

int main()
{

	char mytext[] = "testowesto";
	int meh = 234;
	int welo = 9699;
	struct QueueManager* MyQueue = CreateQueue("lelo");
	InsertQueueNode(MyQueue, mytext, sizeof(mytext));
	InsertQueueNode(MyQueue, &meh, sizeof(meh));
	InsertQueueNode(MyQueue, &welo, sizeof(meh));
	printf("%s\n", (char*)MyQueue->tail->data);
	printf("%s\n", MyQueue->QueueID);
	printf("%d\n", MyQueue->QueueNodeCount);
	printf("%p\n", MyQueue->tail);
	printf("%p\n", MyQueue->tail->prev);
	printf("%p\n", MyQueue->tail->prev->prev);
	printf("%p\n", MyQueue->tail->prev->prev->prev);
	printf("%p\n", MyQueue->head);
	printf("%p\n", MyQueue->head->next);
	printf("%p\n", MyQueue->head->next->next);

	void* DataReturn = GrabQueueNode(MyQueue);
	printf("%s\n", (char*)DataReturn);
	free(DataReturn);
	DataReturn = GrabQueueNode(MyQueue);
	printf("%d\n", *(int*)DataReturn);
	free(DataReturn);
	DataReturn = GrabQueueNode(MyQueue);
	printf("%d\n", *(int*)DataReturn);
	free(DataReturn);

	bool success = DeleteQueue(MyQueue);
	printf("\n%d\n", success);

	return 0;
}