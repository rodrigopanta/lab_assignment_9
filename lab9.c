#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 37

// RecordType
struct RecordType {
  int id;
  char name;
  int order;
};

// Node for linked list
struct Node {
  struct RecordType record;
  struct Node *next;
};

// Hash table structure
struct HashTable {
  struct Node *table[TABLE_SIZE];
};

// Compute the hash function
int hash(int x) { return x % TABLE_SIZE; }

// Function to create a new node
struct Node *createNode(struct RecordType record) {
  struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
  if (newNode == NULL) {
    printf("Memory allocation failed!\n");
    exit(1);
  }
  newNode->record = record;
  newNode->next = NULL;
  return newNode;
}

// Function to insert a record into the hash table
void insert(struct HashTable *ht, struct RecordType record) {
  int index = hash(record.id);
  struct Node *newNode = createNode(record);
  if (ht->table[index] == NULL) {
    ht->table[index] = newNode;
  } else {
    // Collision occurred, add to the chain
    struct Node *temp = ht->table[index];
    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = newNode;
  }
}

// display records in the hash structure
void displayRecordsInHash(struct HashTable *pHashArray, int hashSz) {
  int i;

  for (i = 0; i < hashSz; ++i) {
    if (pHashArray->table[i] != NULL) {
      printf("Index %d -> ", i);
      struct Node *current = pHashArray->table[i];
      while (current != NULL) {
        printf("%d %c %d -> ", current->record.id, current->record.name,
               current->record.order);
        current = current->next;
      }
      printf("\n");
    }
  }
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData) {
  FILE *inFile = fopen(inputFileName, "r");
  int dataSz = 0;
  int i, n;
  char c;
  struct RecordType *pRecord;
  *ppData = NULL;

  if (inFile) {
    fscanf(inFile, "%d\n", &dataSz);
    *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
    if (*ppData == NULL) {
      printf("Cannot allocate memory\n");
      exit(-1);
    }
    for (i = 0; i < dataSz; ++i) {
      pRecord = *ppData + i;
      fscanf(inFile, "%d ", &n);
      pRecord->id = n;
      fscanf(inFile, "%c ", &c);
      pRecord->name = c;
      fscanf(inFile, "%d ", &n);
      pRecord->order = n;
    }
    fclose(inFile);
  }

  return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz) {
  int i;
  printf("\nRecords:\n");
  for (i = 0; i < dataSz; ++i) {
    printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
  }
  printf("\n\n");
}

int main(void) {
  struct RecordType *pRecords;
  int recordSz = 0;

  recordSz = parseData("input.txt", &pRecords);
  printRecords(pRecords, recordSz);

  // Create a hash table
  struct HashTable ht;

  // Initialize all elements of the table to NULL
  for (int i = 0; i < TABLE_SIZE; i++) {
    ht.table[i] = NULL;
  }

  // Insert records into the hash table
  for (int i = 0; i < recordSz; i++) {
    insert(&ht, pRecords[i]);
  }

  // Display records in the hash table
  displayRecordsInHash(&ht, TABLE_SIZE);

  return 0;
}
