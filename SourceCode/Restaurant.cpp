#include "main.h"

class imp_res : public Restaurant
{
	
	public:	
		imp_res() {};
		bool empty_res = 1;
		int cus_num = 0, wait_num = 0;
		customer* cursor;
		customer* head;
		customer* tail;
		customer* waitlineHead;

		//Connecting Tail with Head
		void tailToHead() {
			tail->next = head;
			head->prev = tail;
		}

		//Adding customer to the left side of the current table
		void addCustomerLeft(customer* cus) {
			if (cursor == head) {
				cursor->prev = cus;
				cus->next = cursor;
				cursor = head = cus;
			} else {
				customer* temp = cursor->prev;
				cursor->prev = cus;
				cus->next = cursor;
				cus->prev = temp;
				temp->next = cus;
			}
		}

		//Adding customer to the right side of the current table
		void addCustomerRight(customer* cus) {
			if (cursor == tail) {
				cursor->next = cus;
				cus->prev = cursor;
				cursor = tail = cus;
			} else {
				customer* temp = cursor->next;
				cursor->next = cus;
				cus->prev = cursor;
				cus->next = temp;
				temp->prev = cus;
			}
		}

		//Adding customer
		void addCustomer(customer* cus) {
			if(empty_res) {
					head = tail = cursor = cus;
					empty_res = 0;
					cus_num++;
				} else {
					if (cus->energy >= cursor->energy) {
						addCustomerRight(cus);
					} else {
						addCustomerLeft(cus);
					}
					cursor = cus;
					tailToHead;
					cus_num++;
				}
		}

		//Checking for duplicate name in restaurant and waiting line
		bool nameCheck(string name) {
			customer* ptr = head;
			do {
				if (ptr->name == name) return 1;
				ptr = ptr->next;
			} while (ptr != head);
			if (!waitlineHead) {
				ptr = waitlineHead;
				do {
					if (ptr->name == name) return 1;
					ptr = ptr->next;
				} while (ptr != nullptr);
			}
			return 0;
		}
		
		//Adding customer to the waiting line
		void addWaitingCustomer(customer* cus) {
			if (!waitlineHead) waitlineHead = cus;
			else {
				customer* ptr = waitlineHead;
				while (ptr->next != nullptr) ptr = ptr->next;
				ptr->next = cus;
				cus->prev = ptr;
			}
			wait_num++;
		}
		
		//Remove one customer from the waiting line in FIFO order
		void removeWaitingCustomer() {
			customer* temp = waitlineHead;
			waitlineHead = waitlineHead->next;
			waitlineHead->prev = nullptr;
			wait_num--;
			delete temp;
		}
		
		//Filling the restaurant until it reach the maximum capacity or the waiting line clears
		void fillRestaurant() {
			if (!waitlineHead) return;
			if (cus_num == MAXSIZE) return;
			customer* cus = waitlineHead;
			while (cus_num < MAXSIZE || wait_num > 0) {
				addCustomer(cus);
				removeWaitingCustomer();
				cus_num++;
				wait_num--;
			}
			if (wait_num == 0) waitlineHead = nullptr;
		}
		
		void RED(string name, int energy)
		{
			if (energy == 0 || wait_num > MAXSIZE) return;
			cout << name << " " << energy << endl;
			customer *cus = new customer (name, energy, nullptr, nullptr);
			fillRestaurant();
			if (nameCheck(name)) return;
			if (cus_num == MAXSIZE) {
				addWaitingCustomer(cus);
			}
			else if (cus_num >= (MAXSIZE / 2)) {
				int res;
				customer* ptr = head;
				customer* hold;
				do {
					if (abs(cus->energy - ptr->energy) > abs(res)) {
						res = cus->energy - ptr->energy;
						hold = ptr;
					}
					ptr = ptr->next;
				} while (ptr != head);
				cursor = hold;
				if (res > 0) addCustomerLeft(cus);
				else addCustomerRight(cus);
			} else {
				addCustomer(cus);
			}

		}

		void BLUE(int num)
		{
			cout << "blue "<< num << endl;
		}
		void PURPLE()
		{
			cout << "purple"<< endl;
		}
		void REVERSAL()
		{
			cout << "reversal" << endl;
		}
		void UNLIMITED_VOID()
		{
			cout << "unlimited_void" << endl;
		}
		void DOMAIN_EXPANSION()
		{
			cout << "domain_expansion" << endl;
		}
		void LIGHT(int num)
		{
			cout << "light " << num << endl;
		}
};