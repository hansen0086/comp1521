// ADT for a FIFO queue
// COMP1521 17s2 Week01 Lab Exercise
// Written by John Shepherd, July 2017
// Modified by ...

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Queue.h"

typedef struct QueueNode {
   int jobid;  // unique job ID
   int size;   // size/duration of job
   struct QueueNode *next;
} QueueNode;

struct QueueRep {
   int nitems;      // # of nodes
   QueueNode *head; // first node
   QueueNode *tail; // last node
};



// TODO:
// remove the #if 0 and #endif
// once you've added code to use this function

// create a new node for a Queue
static
QueueNode *makeQueueNode(int id, int size)
{
   QueueNode *new;
   new = malloc(sizeof(struct QueueNode));
   assert(new != NULL);
   new->jobid = id;
   new->size = size;
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
   // TODO
   assert(q != NULL);
   struct QueueNode *CurrNode=q->head;
   while(CurrNode!=NULL)
   {
        struct QueueNode *temp=CurrNode->next;
        assert(CurrNode);
        free(CurrNode);
        CurrNode = temp;
   }
   assert(q->head==NULL);
   free(q);
   
}

// add a new item to tail of Queue
void  enterQueue(Queue q, int id, int size)
{
   assert(q != NULL);
   // TODO
  struct QueueNode *new_node = makeQueueNode(id, size);
  if(q->nitems==0)
  {
        q->head=new_node;
        q->tail=new_node;
  }
  else
  {
        assert(q->tail->next==NULL);
        q->tail->next=new_node;
        q->tail=new_node;
  }
  q->nitems++;
  
}

// remove item on head of Queue
int   leaveQueue(Queue q)
{
   assert(q != NULL);
   // TODO
   if(q->head==NULL)
   {
        return 0;
   }
   else
   {
   int old_head_id = q->head->jobid;
   struct QueueNode *old_head = q->head;
   struct QueueNode *new_head = q->head->next;
   free(old_head);
   q->head = new_head;
   if(new_head==NULL)
   {
        q->tail=new_head;
   }
   q->nitems--;
   return old_head_id; // replace this statement
   }
}

// count # items in Queue
int   lengthQueue(Queue q)
{
   assert(q != NULL);
   return q->nitems;
}

// return total size in all Queue items
int   volumeQueue(Queue q)
{
   assert(q != NULL);
   // TODO
   int size = 0;
   struct QueueNode *CurrNode=q -> head;
   while(CurrNode!=NULL)
   {
        size+=CurrNode->size;
        CurrNode=CurrNode->next;
   }
   return size; // replace this statement
}

// return size/duration of first job in Queue
int   nextDurationQueue(Queue q)
{
   assert(q != NULL);
   // TODO
   if(q->head==NULL)
   {
        return 0; // replace this statement
   }
   else
   {
        assert(q->head);
        assert(q->head->size);
        return q->head->size;
   }
}


// display jobid's in Queue
void showQueue(Queue q)
{
   QueueNode *curr;
   curr = q->head;
   while (curr != NULL) {
      printf(" (%d,%d)", curr->jobid, curr->size);
      curr = curr->next;
   }
}
