// PageTable.c ... implementation of Page Table operations
// COMP1521 17s2 Assignment 2
// Written by Hansen Liu, October 2017
//FIFO implement strategy: Using a Queue structure to store the pages
//                         enterqueue when a page is loaded
//                         leavequeue when a new frame is required
//LRU implement strategy: Using a double linked list to store the pages
//                        insert at head when a newpage is loaded
//                        move the node into the front of the list when a page is acccessed
//                        the tail is the page requred
//                        after the requesst, that victim page(node)is deleted
#include <stdlib.h>
#include <stdio.h>
#include "Memory.h"
#include "Stats.h"
#include "PageTable.h"
#include <assert.h>

// Symbolic constants

#define NOT_USED 0
#define IN_MEMORY 1
#define ON_DISK 2

// PTE = Page Table Entry

typedef struct QueueRep *Queue;
typedef struct DDList *DDL;
// make a new empty Queue
Queue makeQueue();
// release space used by Queue
void  freeQueue(Queue);
// add a new item to tail of Queue
void  enterQueue(Queue, int);
// remove item on head of Queue
int   leaveQueue(Queue);

void InsertAtHead(int,DDL);
void deleteTail(DDL);
void deleteItem(int,DDL);

typedef struct {
   char status;      // NOT_USED, IN_MEMORY, ON_DISK
   char modified;    // boolean: changed since loaded
   int  frame;       // memory frame holding this page
   int  accessTime;  // clock tick for last access
   int  loadTime;    // clock tick for last time loaded
   int  nPeeks;      // total number times this page read
   int  nPokes;      // total number times this page modified
   int  page;
   // TODO: add more fields here, if needed ...
} PTE;

struct Node  {
	int data;
	struct Node*  next;
	struct Node*  prev;
};//double linklist structure

struct DDList{
  struct Node * head;
  struct Node * tail;
};//double linklist node


// The virtual address space of the process is managed
//  by an array of Page Table Entries (PTEs)
// The Page Table is not directly accessible outside
//  this file (hence the static declaration)

static PTE *PageTable;      // array of page table entries
static int  nPages;         // # entries in page table
static int  replacePolicy;  // how to do page replacement
static int  fifoList;       // index of first PTE in FIFO list
static int  fifoLast;       // index of last PTE in FIFO list

static Queue fifo_list;
static DDL lru_ddlist;

//static struct QueueNode* head;
//static struct QueueNode * tail;
// Forward refs for private functions

static int findVictim(int);

// initPageTable: create/initialise Page Table data structures

void initPageTable(int policy, int np)
{
   PageTable = malloc(np * sizeof(PTE));
   if (PageTable == NULL) {
      fprintf(stderr, "Can't initialise Memory\n");
      exit(EXIT_FAILURE);
   }
   replacePolicy = policy;
   nPages = np;
   fifoList = 0;
   fifoLast = nPages-1;
   for (int i = 0; i < nPages; i++) {
      PTE *p = &PageTable[i];
      p->status = NOT_USED;
      p->modified = 0;
      p->frame = NONE;
      p->accessTime = NONE;
      p->loadTime = NONE;
      p->nPeeks = p->nPokes = 0;
   }
   fifo_list = makeQueue();//initialize the queue for fifo algorithm

   lru_ddlist = malloc(sizeof(struct DDList));
   lru_ddlist->tail = NULL;
   lru_ddlist->head = NULL;
}

// requestPage: request access to page pno in mode
// returns memory frame holding this page
// page may have to be loaded
// PTE(status,modified,frame,accessTime,nextPage,nPeeks,nWrites)

int requestPage(int pno, char mode, int time)
{
   if (pno < 0 || pno >= nPages) {
      fprintf(stderr,"Invalid page reference\n");
      exit(EXIT_FAILURE);
   }
   PTE *p = &PageTable[pno];
   int fno;
   switch (p->status) {
   case NOT_USED:
   case ON_DISK:
      // TODO: add stats collection
      countPageFault();
      fno = findFreeFrame();
      if (fno == NONE) {
         int vno = findVictim(time);
#ifdef DBUG
         printf("Evict page %d\n",vno);
#endif

        PTE *pv = &PageTable[vno];
        if(pv -> modified!=0) saveFrame(fno);

        pv -> status = ON_DISK;
        pv -> modified = 0;
        pv -> accessTime = NONE;
        pv -> loadTime = NONE;
        fno = pv -> frame;
        pv -> frame = NONE;

        deleteItem(vno,lru_ddlist);//delete the vitim page from the double link list
         // TODO:
         // if victim page modified, save its frame
         // collect frame# (fno) for victim page
         // update PTE for victim page
         // - new status
         // - no longer modified
         // - no frame mapping
         // - not accessed, not loaded
      }
      p -> frame = fno;
      printf("Page %d given frame %d\n",pno,fno);

      p -> page = pno;
      loadFrame(fno,pno,time);
      p -> status = IN_MEMORY;
      p -> modified = 0;
      p -> accessTime = time;
      p -> loadTime = time;
      // TODO:
      // load page pno into frame fno
      // update PTE for page
      // - new status
      // - not yet modified
      // - associated with frame fno
      // - just loaded

      enterQueue(fifo_list,pno);//when loaded, enter the page into queue


      break;
   case IN_MEMORY:
      // TODO: add stats collection
      countPageHit();
      break;
   default:
      fprintf(stderr,"Invalid page status\n");
      exit(EXIT_FAILURE);
   }
   if (mode == 'r')
      p->nPeeks++;
   else if (mode == 'w') {
      p->nPokes++;
      p->modified = 1;
   }

   p->accessTime = time;
   deleteItem(pno,lru_ddlist); 
   InsertAtHead(pno,lru_ddlist);
   //the delete and insert implements moving the page to the end of the list
   return p->frame;
}

// findVictim: find a page to be replaced
// uses the configured replacement policy

static int findVictim(int time)
{
   int victim = NONE;
   switch (replacePolicy) {
   case REPL_LRU:
      // TODO: implement LRU strategy
      victim = lru_ddlist->tail->data;
      break;
   case REPL_FIFO:
      // TODO: implement FIFO strategy
      victim = leaveQueue(fifo_list);
      break;
   case REPL_CLOCK:
      return 0;
   }
   return victim;
}
// showPageTableStatus: dump page table
// PTE(status,modified,frame,accessTime,nextPage,nPeeks,nWrites)

void showPageTableStatus(void)
{
   char *s;
   printf("%4s %6s %4s %6s %7s %7s %7s %7s\n",
          "Page","Status","Mod?","Frame","Acc(t)","Load(t)","#Peeks","#Pokes");
   for (int i = 0; i < nPages; i++) {
      PTE *p = &PageTable[i];
      printf("[%02d]", i);
      switch (p->status) {
      case NOT_USED:  s = "-"; break;
      case IN_MEMORY: s = "mem"; break;
      case ON_DISK:   s = "disk"; break;
      }
      printf(" %6s", s);
      printf(" %4s", p->modified ? "yes" : "no");
      if (p->frame == NONE)
         printf(" %6s", "-");
      else
         printf(" %6d", p->frame);
      if (p->accessTime == NONE)
         printf(" %7s", "-");
      else
         printf(" %7d", p->accessTime);
      if (p->loadTime == NONE)
         printf(" %7s", "-");
      else
         printf(" %7d", p->loadTime);
      printf(" %7d", p->nPeeks);
      printf(" %7d", p->nPokes);
      printf("\n");
   }
}

//***************queue structure*********************************

typedef struct QueueNode {
   int num;   // page id for the victim
   struct QueueNode *next;
} QueueNode;

struct QueueRep {
   int nitems;      // # of nodes
   QueueNode *head; // first node
   QueueNode *tail; // last node
};

// create a new node for a Queue
static
QueueNode* makeQueueNode(int number)
{
   QueueNode* new;
   new = malloc(sizeof(struct QueueNode));
   assert(new != NULL);
   new-> num = number;
   new->next = NULL;
   return new;
}

// make a new empty Queue
Queue makeQueue()
{
   Queue new;
   new = malloc(sizeof(struct QueueRep));
   assert(new != NULL);
   new->nitems = 0; new->head = new->tail = NULL;
   return new;
}

// release space used by Queue
void  freeQueue(Queue q)
{
   assert(q != NULL);
   QueueNode *curr = q->head;
   while(curr != NULL){
      q->head = curr->next;
      assert(curr!=NULL);
      free(curr);
      curr = q->head;
   }
   q->nitems = 0;
   assert(q!=NULL);
   free(q);
}

// add a new item to tail of Queue
void  enterQueue(Queue q, int number)
{
   assert(q != NULL);
   QueueNode *new = makeQueueNode(number);
   if(q->nitems == 0){
      q->head = new;
      q->tail = new;
   }else{
      q->tail->next = new;
      q->tail = new;
   }
   q->nitems++;
}

// remove item on head of Queue
int   leaveQueue(Queue q)
{
   assert(q != NULL);
   // TODO
   if(q->head != NULL){
      int temp = q->head->num;
      QueueNode *curr = q->head;
      q->head = q->head->next;
      q->nitems--;
      assert(curr!=NULL);
      free(curr);
      return temp;
   }

   return 0; // replace this statement
}
//***************DoubleLinkedList structure*********************************

//Creates a new Node and returns pointer to it.
struct Node* newNode(int item) {
	struct Node* newNode  = (struct Node* )malloc(sizeof(struct Node));
	newNode->data = item;
	newNode->prev = NULL;
	newNode->next = NULL;
	return newNode;
}

//Inserts a Node at head of doubly linked list
void InsertAtHead(int item,DDL ddl) {
	struct Node* new = newNode(item);
	if(ddl->head == NULL) {
      ddl->head = new;
      ddl->tail = new;
		return;
	}
	ddl->head->prev = new;
	new->next = ddl->head;
	ddl->head = new;
}

void deleteTail(DDL ddl){
    struct Node*  toDelete;
    assert(ddl->tail!=NULL);
      toDelete = ddl->tail;
      ddl->tail = ddl->tail->prev; // Move last pointer to 2nd last node
      ddl->tail->next = NULL; // Remove link to of 2nd last node with last node
      assert(toDelete!=NULL);
      free(toDelete);       // Delete the last node

}

void deleteItem(int item,DDL ddl) {
   struct Node*  curr;
   struct Node*  tmp;
   for (curr = ddl->head; curr != NULL; curr = curr->next) {
      if (curr->data == item) {
         if (curr->prev == NULL) { /* Remove from beginning */
            ddl->head = curr->next;
         } else if(curr->next == NULL) { /* Remove from end */
            deleteTail(ddl);
         } else { /* Remove from middle */
            tmp = curr->prev;
            tmp->next = curr->next;

            tmp = curr->next;
            tmp->prev = curr->prev;
         }
      }
  }
}
