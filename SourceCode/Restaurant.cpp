#include "main.h"

class imp_res : public Restaurant
{
	public:	
		imp_res() {
			head = nullptr;
			tail = nullptr;
			cus_num = 0;
		};
		customer* head;
		customer* tail;
		int cus_num;
		customer* cursor;
		bool empty_res = 1;
		int wait_num = 0;

		customer* waitlineHead;
		customer* activityLog;

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
					tailToHead();
					cus_num++;
				}
		}

		//Checking for duplicate name in restaurant and waiting line
		bool nameCheck(string name) {
			if (head) {
				customer* ptr = head;
			do {
				if (ptr->name == name) return 1;
				ptr = ptr->next;
			} while (ptr != head);
			}
			if (waitlineHead) {
				customer* ptr = waitlineHead;
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
			freeTable(temp);
			free(temp);
		}
		
		//Filling the restaurant until it reach the maximum capacity or the waiting line clears
		void fillRestaurant() {
			if (!waitlineHead) return;
			if (cus_num == MAXSIZE) return;
			customer* cus = waitlineHead;
			while (cus_num < MAXSIZE && wait_num > 0) {
				addCustomer(cus);
				removeWaitingCustomer();
			}
			if (wait_num == 0) waitlineHead = nullptr;
		}

		//Reseting table values
		void freeTable(customer* cus) {
			cus->energy = 0;
			cus->name = '\0';
			cus->prev = nullptr;
			cus->next = nullptr;
		}

		//Noting customer that successfully has a table to activity log
		void addToActivityLog(customer* cus) {
			if (!activityLog) activityLog = cus;
			else {
				customer* ptr = activityLog;
				while (ptr->next != nullptr) ptr = ptr->next;
				ptr->next = cus;
				cus->prev = ptr;
			}
		}

		//Deleting 1 customer out of activity log in FIFO order
		void removeActivityLog()  {
			customer* ptr = activityLog;
			activityLog = activityLog->next;
			freeTable(ptr);
			free(ptr);
		}

		//Deleting 1 customer out of activity log by choice
		void removeOneActivityLog(customer* cus)  {
			customer* ptr = activityLog;
			while (ptr->name != cus->name) ptr = ptr->next;
			
			if (ptr == activityLog) {
				activityLog = ptr->next;
				freeTable(ptr);
				free(ptr);
			} else if (ptr->next == nullptr) {
				ptr->prev->next = nullptr;
				freeTable(ptr);
				free(ptr);
			} else {
				ptr->next->prev = ptr->prev;
				ptr->prev->next = ptr->next;
				freeTable(ptr);
				free(ptr);
			}
		}

		//Remove one customer from the waiting line by choice
		void removeOneTable(customer* ptr) {
			ptr->next->prev = ptr->prev;
			ptr->prev->next = ptr->next;
			if (ptr == head) {
				head = ptr->next;
			} else if (ptr == tail) {
				tail = ptr->prev;
			}
			if (ptr->energy > 0) cursor = ptr->next;
			else cursor = ptr->prev;
			freeTable(ptr);
			free(ptr);
		}

		//Remove a number of customer out of the restaurant
		void removeNumFromRestaurant(int num) {
			int i = 0;
			customer* ptr = head;
			while (i < num) {
				while (ptr->name != activityLog->name) ptr = ptr->next;
				if (ptr->energy < 0) cursor = ptr->prev;
				else cursor = ptr->next;
				removeOneTable(ptr);
				tailToHead();
				removeActivityLog();
				cus_num;
				i++;
			}
		}

		//Remove all customer out of the restaurant
		void removeAllFromRestaurant() {
			while (head != tail) {
				customer* ptr1 = head;
				customer* ptr2 = activityLog;
				head = head->next;
				tailToHead();
				freeTable(ptr1);
				free(ptr1);
				activityLog = activityLog->next;
				freeTable(ptr2);
				free(ptr2);
			}
			freeTable(head);
			freeTable(activityLog);
			head = nullptr;
			tail = nullptr;
			cursor = nullptr;
			activityLog = nullptr;
			cus_num = 0;
			empty_res = true;
		}

		void swap(customer* cus1, customer* cus2) {
			// int temp_energy;
			// string temp_name;
			// temp_energy = cus1->energy;
			// temp_name = cus1->name;
			// cus1->energy = cus2->energy;
			// cus1->name = cus2->name;
			// cus2->energy = temp_energy;
			// cus2->name = temp_name;

			customer* temp_prev = cus2->prev;
			customer* temp_next = cus2->next;
			customer* temp = cus1;

			cus2->prev = cus1->prev;
			cus2->next = cus1->prev;
			cus1->next->prev = cus2;
			cus1->prev->next = cus2;

			cus1->prev = temp_prev;
			cus1->next = temp_next;
			temp_prev->next = cus1;
			temp_next->prev = cus1;
		}
		
		//Print out customer info
		void print(customer* cus) {
			cout << cus->name << "-" << cus->energy << endl;
		}

		void RED(string name, int energy)
		{
			cout << name << " " << energy << endl;
			if (energy == 0 || wait_num > MAXSIZE) return;
			customer *cus = new customer (name, energy, nullptr, nullptr);
			if (nameCheck(name)) return;
			if (wait_num == MAXSIZE) {
				addWaitingCustomer(cus);
			}
			else if (cus_num >= (MAXSIZE / 2)) {
				int res = 0;
				customer* ptr = head;
				customer* hold = ptr;
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
				cursor = cus;
				addToActivityLog(cus);
			} else {
				addCustomer(cus);
				addToActivityLog(cus);
			}

		}

		void BLUE(int num)
		{
			cout << "blue "<< num << endl;
			if (num <= 0) return;
			if (cus_num <= 0) return;
			if (num < cus_num) {
				removeNumFromRestaurant(num);
			} else {
				removeAllFromRestaurant();
			}
			fillRestaurant();
		}

		void PURPLE()
		{
			cout << "purple"<< endl;
			customer* ptr = waitlineHead;
			customer* temp;
			int max = 0;
			while (ptr->next != nullptr) {
				if (max >= abs(ptr->energy)) {
					temp = ptr;
					max = ptr->energy;
				}
				ptr = ptr->next;
			}
		}
		void REVERSAL()
		{
			cout << "reversal" << endl;
			if (empty_res) return;
			if (cus_num == 1) {
				print(head);
				return;
			}
			customer* ptr1 = cursor;
			customer* ptr2 = cursor->prev;
			bool foundEachother = 0;	

			while (ptr1 != ptr2) {
				while (ptr1 < 0) {
					ptr1 = ptr1->next;
					if (ptr1 == ptr2) {
						foundEachother = 1;
						break;
					}
				}
				while (ptr2 < 0) {
					ptr2 = ptr2->next;
					if (ptr1 == ptr2) {
						foundEachother = 1;
						break;
					}
				}
				if (foundEachother) break;
				else swap(ptr1, ptr2);
				ptr1 = ptr1->next;
				if (ptr1 == ptr2) break;
				ptr2 = ptr2->next;
			}

			while (ptr1 != ptr2) {
				while (ptr1 > 0) {
					ptr1 = ptr1->next;
					if (ptr1 == ptr2) {
						foundEachother = 1;
						break;
					}
				}
				while (ptr2 > 0) {
					ptr2 = ptr2->next;
					if (ptr1 == ptr2) {
						foundEachother = 1;
						break;
					}
				}
				if (foundEachother) break;
				else swap(ptr1, ptr2);
				ptr1 = ptr1->next;
				if (ptr1 == ptr2) break;
				ptr2 = ptr2->next;
			}
		}
		void UNLIMITED_VOID()
		{
			cout << "unlimited_void" << endl;
			if (cus_num < 4) return;
			customer* ptr1 = cursor;
			customer* ptr2 = cursor->next;
			customer* min_ptr;
			int final_min = 0, count = 1, final_count;
			do
			{
				int min = ptr1->energy;
				for (int i = 0; i < 3; i++) {
					min += ptr2->energy;
					count++;
					ptr2 = ptr2->next;
				}
				while (ptr2->energy < 0) {
					min += ptr2->energy;
					count++;
					ptr2 = ptr2->next;
				}
				if (min <= final_min) {
					min_ptr = ptr1;
					final_min = min;
					final_count = count;
				}
				count = 1;
				ptr1 = ptr1->next;
			} while (ptr1 != cursor);
			count = 0;
			ptr1 = min_ptr;
			int min = min_ptr->energy;
			int count_ptr;
			for (count; count < final_count; count++) {
				if (min < ptr1->energy) {
					count_ptr = count;
					ptr2 = ptr1;
				}
				ptr1 = ptr1->next;
			}
			for (count = count_ptr; count <= final_count; count++) {
				print(ptr2);
				ptr2 = ptr2->next;
			}
			ptr2 = min_ptr;
			for (count = 0; count < count_ptr; count++) {
				print(ptr2);
				ptr2 = ptr2->next;
			}
		}
		
		void DOMAIN_EXPANSION()
		{
			cout << "domain_expansion" << endl;
			if (!head) return;
			customer* ptr = head;
			int sum = 0;
			while (ptr->next != head) {
				sum += ptr->energy;
				ptr = ptr->next;
			}
			ptr = waitlineHead;
			while (ptr->next != nullptr) {
				sum += ptr->energy;
				ptr = ptr->next;
			}
			if (sum < 0) {
				ptr = head;
				customer* temp;
				while (ptr->next != head) {
					temp = ptr->next;
					if (ptr->energy < 0){
						removeOneTable(ptr);
						removeOneActivityLog(ptr);
						tailToHead();
						print(ptr);
					}
					ptr = temp;
				}
				cursor = temp->next;
				ptr = waitlineHead;
				while (ptr->next != nullptr) {
					temp = ptr->next;
					if (ptr->energy < 0) {
						removeOneTable(ptr);
						removeOneActivityLog(ptr);
						tailToHead();
						print(ptr);
					}
					ptr = temp;
				}
			} else {
				ptr = head;
				customer* temp;
				while (ptr->next != head) {
					temp = ptr->next;
					if (ptr->energy > -1){
						removeOneTable(ptr);
						removeOneActivityLog(ptr);
						tailToHead();
						print(ptr);
					}
					ptr = temp;
				}
				cursor = temp->prev;
				ptr = waitlineHead;
				while (ptr->next != nullptr) {
					temp = ptr->next;
					if (ptr->energy > -1) {
						removeOneTable(ptr);
						removeOneActivityLog(ptr);
						tailToHead();
						print(ptr);
					}
					ptr = temp;
				}
			}
		}

		void LIGHT(int num)
		{
			cout << "light " << num << endl;
			if (num > 0) {
				customer* ptr = cursor;
				do
				{
					print(ptr);
					ptr = ptr->next;
				} while (ptr != cursor);
				
			} else if(num < 0) {
				customer* ptr = cursor;
				do
				{
					print(ptr);
					ptr = ptr->prev;
				} while (ptr != cursor);
			} else {
				customer* ptr = waitlineHead;
				do
				{
					print(ptr);
					ptr = ptr->next;
				} while (ptr->next != nullptr);
			}
		}
};