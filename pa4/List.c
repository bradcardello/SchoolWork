// Brad Cardello
// bcardell
// pa4

#include<stdio.h>
#include<stdlib.h>
#include"List.h"

// structs --------------------------------------------------------------------

// private NodeObj type
typedef struct NodeObj{
   int data;
   struct NodeObj* prev;
   struct NodeObj* next;
} NodeObj;

// private Node type
typedef NodeObj* Node;

// private ListObj type
typedef struct ListObj{
   Node front;
   Node back;
   Node cursor;
   int length;
   int cursorIndex;
} ListObj;

// Constructors-Destructors ---------------------------------------------------

// newNode()
// Returns reference to new Node object. Initializes prev, next, and data fields.
// Private.
Node newNode(int data){
   Node N = malloc(sizeof(NodeObj));
   N->data = data;
   N->prev = NULL;
   N->next = NULL;
   return(N);
}

// freeNode()
// Frees heap memory pointed to by *pN, sets *pN to NULL.
// Private.
void freeNode(Node* pN){
   if(pN != NULL && *pN != NULL){
      free(*pN);
      *pN = NULL;
   }
}

// Returns reference to new empty List object.
List newList(void){
   List L;
   L = malloc(sizeof(ListObj));
   L->front = L->back = L->cursor = NULL;
   L->length = 0;
   L->cursorIndex = -1;
   return(L);
}

// Frees all heap memory associated with List *pS, and sets *pS to NULL.
void freeList(List* pL){
   if(pL != NULL && *pL != NULL) { 
      while(length(*pL) != 0) { 
         deleteFront(*pL); 
      }
      free(*pL);
      *pL = NULL;
   }
}


// Access functions -----------------------------------------------------------

// Returns number of elements in this list.
int length(List L){
   return L->length;
}

// Returns the index of the cursor element in this list, or
// returns -1 if the cursor element is undefined.
int getIndex(List L){
   L->cursorIndex = 0;
   if (L->cursor == NULL){
      L->cursorIndex = -1;
      return L->cursorIndex;
   }
   else{
      for (Node N = L->front; N != NULL; N = N->next){
         if (N == L->cursor) return L->cursorIndex;
         L->cursorIndex++;
      }
   }
   return L->cursorIndex;
}

// Returns front element in this list
// Pre: length()>0
int front(List L){
   if (length(L) <= 0){
      printf("List Error: front() called on empty List\n");
      exit(1);
   }
   return L->front->data;
}

// Returns back element in this List
// Pre: length()>0
int back(List L){
   if (length(L) <= 0){
      printf("List Error: back() called on empty List\n");
      exit(1);
   }
   return L->back->data;
}

// Returns cursor element in this list.
// Pre: length()>0, getIndex()>=0
int getElement(List L){
   if (length(L) <= 0){
      printf ("List Error: getElement() called on empty List\n");
      exit(1);
   }
   if (getIndex(L) < 0){
      printf ("List Error: getElement() called on undefined cursor\n");
      exit(1);
   }
   return L->cursor->data;
}

// Returns 1 if this List and L are the same integer
// sequence. Returns 0 in any other case.  The cursor
// is ignored in both lists.
int equals(List A, List B){
   int flag = 1;
   Node N = NULL;
   Node M = NULL;

   if( A == NULL || B == NULL ){
      printf("List Error: calling equals() on NULL List reference\n");
      exit(1);
   }
   N = A->front;
   M = B->front;
   if(length(A) != length(B)) return 0;
   while(flag && N != NULL){
      flag = (N->data == M->data);
      N = N->next;
      M = M->next;
   }
   return flag;
}


// Manipulation procedures ----------------------------------------------------

// Re-sets this List to the empty state.
void clear(List L){
   L->front = L->back = L->cursor = NULL;
   L->length = 0;
   L->cursorIndex = -1;
}

// If 0<=i<=length()-1, moves the cursor to the element
// at index i in list L, otherwise the cursor becomes 
// undefined.
void moveTo(List L, int i){
   L->cursorIndex = 0;
   if (i >= 0 && i <= length(L) - 1){
      for (Node N = L->front; L->cursorIndex <= i; N = N->next){
         if (L->cursorIndex == i){
            L->cursor = N;
            break;
         }
         else L->cursorIndex++;
      }
   }
   else{
      L->cursor = NULL;
      L->cursorIndex = -1;
   }
}

// If 0<getIndex()<=length()-1, moves the cursor one step toward the
// front of the list. If getIndex()==0, cursor becomes undefined.
// If getIndex()==-1, cursor remains undefined. This operation is
// equivalent to moveTo(getIndex()-1).
void movePrev(List L){
   if (0 < getIndex(L) && getIndex(L) <= length(L) - 1)
      moveTo(L, getIndex(L) - 1);
   else if (getIndex(L) == 0){
      L->cursorIndex = -1;
      L->cursor = NULL;
   }
}

// If 0<=getIndex()<length()-1, moves the cursor one step toward the
// back of the list. If getIndex()==length()-1, cursor becomes
// undefined. If index==-1, cursor remains undefined. This
// operation is equivalent to moveTo(getIndex()+1).
void moveNext(List L){
   if (0 <= getIndex(L) && getIndex(L) < length(L) - 1)
      moveTo(L, getIndex(L) + 1);
   else if (getIndex(L) == length(L) - 1 || L->cursorIndex == -1){
      L->cursorIndex = -1;
      L->cursor = NULL;
   }
}

// Inserts new element before front element in this List.
void prepend(List L, int data){
   L->length++;
   Node N = newNode(data);
   N->next = L->front;
   N->prev = NULL;
   if (L->front != NULL) {
      L->front->prev = N;
   }
   else{
      L->back = N;
   }
   L->front = N;
   if (L->cursorIndex != -1) L->cursorIndex++;
}

// Inserts new element after back element in this List.
void append(List L, int data){
   L->length++;
   Node temp = newNode(data);
   temp->prev = L->back;
   temp->next = NULL;
   if (L->back != NULL) {
      L->back->next = temp;
   }
   else{
      L->front = temp;
   }
   L->back = temp;
}

// Inserts new element before cursor element in this
// List. Pre: length()>0, getIndex()>=0
void insertBefore(List L, int data){
   if (length(L) <= 0){
      printf("List Error: insertBefore() called on empty List\n");
      exit(1);
   }
   if (getIndex(L) < 0){
      printf("List Error: insertBefore() called on undefined cursor\n");
      exit(1);
   }
   if (L->cursor == L->front){
      prepend(L, data);
      return;
   }
   if (!(length(L) >0 && getIndex(L) >=0)) return;
   Node temp = newNode(data);
   temp->prev = L->cursor->prev;
   temp->next = L->cursor;
   if (L->cursor->prev != NULL) {
      L->cursor->prev->next = temp;
   }
   else{
      L->front = temp;
   }
   L->cursor->prev = temp;
   L->length++;
   
}

// Inserts new element after cursor element in this
// List. Pre: length()>0, getIndex()>=0
void insertAfter(List L, int data){
   if (length(L) <= 0){
      printf("List Error: insertAfter() called on empty List\n");
      exit(1);
   }
   if (getIndex(L) < 0){
      printf("List Error: insertAfter() called on undefined cursor\n");
      exit(1);
   }
   if (L->cursor == L->back){
      append(L, data);
      return;
   }
   Node N = newNode(data);
   N->prev = L->cursor;
   N->next = L->cursor->next;
   if (L->cursor->next != NULL)
      L->cursor->next->prev = N;
   L->cursor->next = N;
   L->length++;
}

// Deletes the front element in this List. Pre: length()>0
void deleteFront(List L){
   Node N = NULL;
   if(length(L) <= 0){
      printf("List Error: deleteFront() called on empty List\n");
      exit(1);
   }
   N = L->front;
   if(length(L) > 1) L->front = L->front->next;
   else L->front = L->back = NULL;
   L->length--;
   freeNode(&N);
}

// Deletes the back element in this List. Pre: length()>0
void deleteBack(List L){
   Node N = NULL;
   if(length(L) <= 0){
      printf("List Error: deleteBack() called on empty List\n");
      exit(1);
   }
   N = L->back;
   L->back = L->back->prev;
   L->back->next = NULL;
   L->length--;
   freeNode(&N);
}

// Deletes cursor element in this List. Cursor is undefined after this
// operation. Pre: length()>0, getIndex()>=0
void delete(List L){
   if (length(L) <= 0){
      printf("List Error: delete() called on empty List");
      exit(1);
   }
   if (getIndex(L) < 0){
      printf("List Error: delete() called on undefined cursor");
      exit(1);
   }
   
   if (L->cursor == L->front){
      deleteFront(L);
      return;
   }
   else if (L->cursor == L->back){
      deleteBack(L);
      return;
   }
   L->cursor->prev->next = L->cursor->next;
   L->cursor->next->prev = L->cursor->prev;
   L->cursor = NULL;
   L->length--;
   L->cursorIndex = -1;
}


// Other operations -----------------------------------------------------------

// Prints data elements in L on a single line to stdout.
void printList(FILE* out, List L){
   Node N = NULL;

   if( L == NULL ){
      printf("List Error: calling printList() on NULL List reference\n");
      exit(1);
   }
   for(N = L->front; N != NULL; N = N->next){
      fprintf(out, "%d ", N->data);
   }
}

// Returns a new list representing the same integer sequence as this
// list. The cursor in the new list is undefined, regardless of the
// state of the cursor in this List. This List is unchanged.
List copyList(List L){
   List new = newList();
   for (Node curr = L->front; curr != NULL; curr = curr->next){
      append(new, curr->data);
   }
   return new;
}