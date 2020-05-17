#ifndef MYLINKEDLIST_H
#define MYLINKEDLIST_H
#include "BaseGameEntity.h"

// look into making into class template later

/////////////////////////////////////////////////////////////////////////
//	This is a QUICK LinkedList to replace the array system used by the 
//	AI_Manager Class - Will iron out any issues in the future when time permits.
//  Though it is capable, I don't use the bi-directional nature of the list.
//	LinkedList: head in ctor is a dummy node and will never be changed;
//				PushFront - insert data to the front of the linked list
//				PushBack -	insert data to the back of the linked list
//				PopFront -	remove the node at the front - list integrity is maintained
//				PopBack -	remove the node at the back - list integrity is maintained
//				PopEntity - remove node that entity occupies by ID (deletes node and entity)
//							list integrity is maintained.
//				ListCount - retrieve the number of items in the list
//				GetIndex -	will retrieve index of an object if found;
//							will retrun 0 if there are no nodes;
//							will return -1 if index is not found;
//							I recommend using GetIndex to verify if
//							an object exists before using Access()
//				Update -	For AI_Manager.  Cycle through list and update entities.
//				Access -	Access a BaseGameEntity by using its ID
//							Use GetIndex to verify whether an entity exists
//							if you are unsure of an ID.
//							DO NOT USE IF YOU HAVE NOT CREATED ANY ENTITIES!
//							WILL RETURN NULL POINTER IF FAILED!!!!!
/////////////////////////////////////////////////////////////////////////

class LinkedList
{
	private:

		struct ListNode 
		{	BaseGameEntity* data;
			ListNode* next;
			ListNode* prev;};

		ListNode* head;
		ListNode* tail;
		ListNode* temp;
		ListNode* iterator;

	public:
		
		LinkedList()
		{	head = new ListNode;
			head->data = NULL;
			head->next = NULL;
			head->prev = NULL;
			tail = head;
			temp = head;
			iterator = head;}

		virtual ~LinkedList()
		{	iterator = head->next;
			while( iterator->next != NULL )
			{	temp = iterator;
				iterator = iterator->next;
				if(temp->data) delete temp->data;
			}
		//	if(iterator->data) delete iterator->data;		// check!*

		}

		bool PushFront( BaseGameEntity* entity )
		{	if( head->next != NULL )			// if there is a next node
			{	temp = head->next;				// remember location of next node
				head->next = new ListNode;		// create new node
				iterator = head->next;			// move to new node
				iterator->data = entity;		// update data in new node
				iterator->next = temp;			// copy location of remembered node
				iterator->prev = head;			// prev position is head
				temp->prev = iterator;			// update prev in old node
			}
			else								// if there is no next node
			{	head->next = new ListNode;		// create new node at head->next
				iterator = head->next;			// move iterator to new node
				iterator->data = entity;		// update data
				iterator->next = NULL;			// there is no next node
				iterator->prev = head;			// update prev
				tail = iterator;				// this node is now tail
			}
			return true;
		}

		bool PushBack( BaseGameEntity* entity )
		{	temp = tail;						// remember position of old tail
			tail->next = new ListNode;			// create new node at tail->next
			iterator = tail->next;				// move to tail->next
			iterator->data = entity;			// update data
			iterator->next = NULL;				// last node has NULL for next
			iterator->prev = temp;				// assign old tail to prev
			tail = iterator;					// new node is now tail
			return true;
		}

		bool PopFront( void )
		{	if( head->next == NULL )			// skip if there is no other nodes
				return true;
			
			iterator = head->next;				// move to next position
			temp = iterator->next;				// remember next node
			delete iterator;					// delete current node
			head->next = temp;					// assign head's next to remembered node
			temp->prev = head;					// update head in node
			return true;
		}

		bool PopBack( void )
		{	if( tail == head )					// skip if tail is head
				return true;

			iterator = tail->prev;				// get position before tail
			delete tail;						// delete tail
			tail = iterator;					// update tail
			tail->next = NULL;					// update next 
			return true;
		}

		bool PopEntity( int ID )
		{	if( head->next == NULL )					// if there are no other nodes
				return true;

			ListNode* temp2;
			iterator = head->next;	
			while( iterator->next != NULL )				
			{	
				if( iterator->data->ID() == ID )		// if entity is found
				{	
					if( iterator->next == NULL )		// if node is last
					{	
						temp = iterator->prev;			// remember prev position
						delete iterator;				// delete node
						tail = temp;					// assign tail to prev position
						temp->next = NULL;				// update next to NULL in prev node
						return true;
					}
						temp = iterator;				// remember node position
						temp2 = iterator->prev;			// remember prev position
						iterator = iterator->next;		// remember next position
						delete temp;					// delete node
						temp2->next = iterator;			// link prev position to next position
						iterator->prev = temp2;			// update prev in next position
						return true;
				}
				iterator = iterator->next;
			}
			return false;
		}

		int ListCount( void )
		{	if( head->next == NULL )
				return 0;

			int count = 1;
			iterator = head->next;
			while( iterator->next != NULL )
			{	iterator = iterator->next;
				count++;
			}
			return count;
		}

		int GetIndex( int ID )									// retrieve index of an enity if found
		{	iterator = head;						
				for( int i = 1 ; iterator->next != NULL ; i++ )	// index does not start at 0
				{	iterator = iterator->next;
						if( iterator->data->ID() == ID ) 
								return i;						// return index of entity if found
				}
			return -1;											// -1 indicates ID was not found
		}

		bool Update( void )
		{	if( head->next == NULL )							// if there are no other nodes
			return true;

			iterator = head->next;
			while( iterator->next != NULL )
			{	iterator->data->update();
				iterator = iterator->next;
			}
			iterator->data->update();
			return true;
		}

		BaseGameEntity* Access( int ID )
		{	iterator = head;
		int count = 0;
			while( iterator->next != NULL )
			{	iterator = iterator->next;
				if( iterator->data->ID() == ID )
				return iterator->data; count++;	}
			return NULL;						// return NULL warning!
		}
		
};

#endif