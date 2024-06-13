#include "main.h"
//int MAXSIZE; //delete
//FINAL

class imp_res : public Restaurant
{
	public:
    class Time {
    public:
        customer* data;
        bool isSittingOnTable;
        Time* next;
        Time* prev;
    public:
        Time(customer* data, bool isSittingOnTable, Time* next = nullptr, Time* prev = nullptr)
            : data(data), isSittingOnTable(isSittingOnTable), next(next), prev(prev) {}
        ~Time() { delete data; }
    };

public:
    customer* Table_Customer;
    customer* WaitList_Customer;
    int Table_CurrentSize;
    int WaitList_CurrentSize;
    Time* Time_HeadCustomer;
    Time* Time_TailCustomer;

public:
    imp_res()
        : Table_Customer(nullptr), WaitList_Customer(nullptr), Table_CurrentSize(0),
          WaitList_CurrentSize(0), Time_HeadCustomer(nullptr), Time_TailCustomer(nullptr) {}
	~imp_res(){
		for (Time* current = Time_HeadCustomer; current != nullptr;) {
			Time* nextCustomer = current->next;
			current->next = nullptr;
			current->prev = nullptr;
			delete current;
			current = nextCustomer;
		}
    	Time_HeadCustomer = nullptr;
	};














//Help Functions
	//nếu RED đổi thì hàm này cũng đổi
	
	void 		ResPrint(){
		return;
		cout << "TableSize = " << Table_CurrentSize << " / WaitlisiSize = " << WaitList_CurrentSize << endl;
		Time* run = Time_HeadCustomer;

		int count = 1;
		while (run){
			cout << "run[" << count << "] = " << run->data->name << "/" << run->data->energy << "  -->  ";
			count ++;
			run = run->next;
		}
		cout << endl << endl;
		return;
	}
	
	int 		Ressize(){
		return Table_CurrentSize + WaitList_CurrentSize;
	}
	
	void 		GENRL_KickOutOfRestaurant(Time* cusTime){
		//nếu ngồi trong bàn
		if (cusTime->isSittingOnTable){
			if 			(Table_CurrentSize == 1){
				Table_Customer = nullptr;
				Table_CurrentSize = 0;
				return;
			}
			else if 	(Table_CurrentSize > 1) {
				//xoá khỏi bàn ăn
				customer* kickedCustomer = cusTime->data;
				kickedCustomer->prev->next = kickedCustomer->next;
				kickedCustomer->next->prev = kickedCustomer->prev;
				//cập nhật TableCustomer
				if (kickedCustomer->energy > 0)		{Table_Customer = kickedCustomer->next;}
				else if (kickedCustomer->energy < 0){Table_Customer = kickedCustomer->prev;}
				Table_CurrentSize --;

				kickedCustomer->next = nullptr;
				kickedCustomer->prev = nullptr;
				delete kickedCustomer;
				return;
			}
		}
		//nếu ngồi trong hàng chờ
		else if (!cusTime->isSittingOnTable){
			if 			(WaitList_CurrentSize == 1){
				WaitList_Customer = nullptr;
				WaitList_CurrentSize = 0;
				return;
			}
			else if 	(WaitList_CurrentSize > 1){
				//xoá khỏi hàng chờ
				customer* kickedCustomer = cusTime->data;
				if (kickedCustomer == WaitList_Customer){
					WaitList_Customer = WaitList_Customer->next;

				}
				kickedCustomer->prev->next = kickedCustomer->next;
				kickedCustomer->next->prev = kickedCustomer->prev;
				WaitList_CurrentSize --;
				kickedCustomer->next = nullptr;
				kickedCustomer->prev = nullptr;
				delete kickedCustomer;
				return;
			}
		}
	}

	void 		GENRL_MoveCustomer_WaitlistToTable() {
		while (Table_CurrentSize < MAXSIZE && WaitList_CurrentSize > 0) {

			customer* movingCustomer = WaitList_Customer;
			Time* findMovingCustomerInTime = Time_HeadCustomer;
	//tìm khách hàng ngồi trong hàng chờ đầu tiên
			while (findMovingCustomerInTime) {
				if (findMovingCustomerInTime->data == WaitList_Customer) {
					findMovingCustomerInTime->isSittingOnTable = true;
					break;
				}
				findMovingCustomerInTime = findMovingCustomerInTime->next;
			}
	//giảm size hàng chờ
			if (WaitList_CurrentSize == 1) {
				WaitList_Customer = nullptr;
			} 
			else {
				WaitList_Customer = movingCustomer->next;
				WaitList_Customer->prev = movingCustomer->prev;
				movingCustomer->prev->next = WaitList_Customer;
			}
			WaitList_CurrentSize--;

		
	//Tuyển khách vào giống RED
			if (Table_CurrentSize == 0) {
					Table_Customer = movingCustomer;
					movingCustomer->next = movingCustomer;
					movingCustomer->prev = movingCustomer;
				} 
			else if (Table_CurrentSize >= MAXSIZE / 2) {
					int max_diff = 0;
					customer* maxDiffCustomer = nullptr;
					customer* temp = Table_Customer;
					int RESwithoutAbsolute = -1;

					
					do {
						int current_diff = abs(movingCustomer->energy - temp->energy);
						if (current_diff > max_diff) {
							max_diff = current_diff;
							maxDiffCustomer = temp;
							RESwithoutAbsolute = movingCustomer->energy - temp->energy;
						}
						temp = temp->next;
					} while (temp != Table_Customer);

					if (RESwithoutAbsolute > 0) {RED_addCustomer_Clockwise(movingCustomer, maxDiffCustomer);} 
					else {RED_addCustomer_counterClockwise(movingCustomer, maxDiffCustomer);
					}
			} 
			else {
					if (movingCustomer->energy >= Table_Customer->energy) {RED_addCustomer_Clockwise(movingCustomer, Table_Customer);} 
					else {RED_addCustomer_counterClockwise(movingCustomer, Table_Customer);}
				}

			Table_CurrentSize++;
			Table_Customer = movingCustomer;
		}
		return;
	}

	bool 		RED_NameIsExistInRestaurant (string name){
		Time* run = Time_HeadCustomer;
		for (int i = 0; i < Ressize(); i++){
			if (run->data->name == name){return 1;}
			run = run->next;
		}
		return 0;
	}
	
	bool 		RED_RestaurantIsFull(){
		if (Ressize() >= MAXSIZE*2){
			return 1;
		}
		return 0;
	}
	
	void 		RED_addCustomer_Clockwise (customer* newCustomer, customer* position){
		newCustomer->next = position->next;
		newCustomer->prev = position;
		position->next->prev = newCustomer;
		position->next = newCustomer;
	}

	void 		RED_addCustomer_counterClockwise (customer* newCustomer, customer* position){
		newCustomer->next = position;
		newCustomer->prev = position->prev;
		position->prev->next = newCustomer;
		position->prev = newCustomer;
	}

	void 		RED_UpdateCustomerTime(customer* newCustomer, bool isSittingInTable) {
		Time* newTimeCustomer = new Time(newCustomer, isSittingInTable);
		Time_TailCustomer->next = newTimeCustomer;
		newTimeCustomer->prev = Time_TailCustomer;
		Time_TailCustomer = Time_TailCustomer->next;
	}

	Time* 		BLUE_FirstTableCustomerInTime() {
		Time* temp = Time_HeadCustomer;
		
		//Tìm khách hàng đầu tiên trong hàng chờ 
		while ((temp) && (!temp->isSittingOnTable)) {
			temp = temp->next;
		}
		

		//cập nhật hàng time
		if (temp) {
			if (Ressize() == 1) {
				Time_HeadCustomer = nullptr;
				Time_TailCustomer = nullptr;
			} 
			else {
				if (temp == Time_HeadCustomer) {
					Time_HeadCustomer = Time_HeadCustomer->next;
					Time_HeadCustomer->prev = nullptr;
				} 
				else if (temp == Time_TailCustomer) {
					Time_TailCustomer = Time_TailCustomer->prev;
					Time_TailCustomer->next = nullptr;
				} 
				else {
					temp->prev->next = temp->next;
					temp->next->prev = temp->prev;
				}
			}
		}
		return temp;
	}

	void 		REVERSAL_swapAddress(customer* cus1, customer* cus2){
		customer* tempCus = new customer();

		tempCus->next = cus1->next;
		tempCus->prev = cus1->prev;
		cus1->next->prev = tempCus;
		cus1->prev->next = tempCus;

		cus1->next = cus2->next;
		cus1->prev = cus2->prev;
		cus2->next->prev = cus1;
		cus2->prev->next = cus1;

		cus2->next = tempCus->next;
		cus2->prev = tempCus->prev;
		tempCus->next->prev = cus2;
		tempCus->prev->next = cus2;
		
		tempCus->prev = nullptr;
		tempCus->next = nullptr;
		delete tempCus;
	}

	void 		DOMAIN_separateCustomers(Time*& MagicianHead, Time*& MagicianTail, Time*& SoulHead, Time*& SoulTail) {
		Time* Time_tempRun = Time_HeadCustomer;
		
		int k = 0;
		while (k < Ressize()) {
			if (Time_tempRun->data->energy > 0) {
				if (!MagicianHead) {
					MagicianHead = Time_tempRun;
					MagicianTail = Time_tempRun;
				}
				else if (MagicianHead) {
					MagicianTail->next = Time_tempRun;
					Time_tempRun->prev = MagicianTail;
					MagicianTail = Time_tempRun;
				}
			}
			else if (Time_tempRun->data->energy < 0) {
				if (!SoulHead) {
					SoulHead = Time_tempRun;
					SoulTail = Time_tempRun;
				}
				else if (SoulHead) {
					SoulTail->next = Time_tempRun;
					Time_tempRun->prev = SoulTail;
					SoulTail = Time_tempRun;
				}
			}
			Time_tempRun = Time_tempRun->next;
			k++;
		}

		// Cập nhật đầu đuôi của danh sách Magician và Soul
		if (SoulHead) {
			SoulHead->prev = nullptr;
			SoulTail->next = nullptr;
		}
		if (MagicianHead) {
			MagicianHead->prev = nullptr;
			MagicianTail->next = nullptr;
		}
	}

	void 		DOMAIN_calculateEnergySums(int& sumOfPositiveEnergy, int& sumOfNegativeEnergy) {
		// Tính tổng năng lượng trong bàn ăn
		customer* temp1az = Table_Customer;
		int i = 0;
		while (i < Table_CurrentSize) {
			if (temp1az->energy > 0) {
				sumOfPositiveEnergy += temp1az->energy;
			}
			else if (temp1az->energy < 0) {
				sumOfNegativeEnergy += temp1az->energy;
			}
			temp1az = temp1az->next;
			i++;
		}

		// Tính tổng năng lượng trong hàng chờ
		customer* temp2az = WaitList_Customer;
		int j = 0;
		while (j < WaitList_CurrentSize) {
			if (temp2az->energy > 0) {
				sumOfPositiveEnergy += temp2az->energy;
			}
			else if (temp2az->energy < 0) {
				sumOfNegativeEnergy += temp2az->energy;
			}
			temp2az = temp2az->next;
			j++;
		}
	}

	void 		UNLIMITED_processSubarray(customer* start, customer* end) {
		customer* min = start;
		customer* temp2 = start->next;
		int distance_StartToMin = 1;
		int distance_StartToEnd = 1;

		while (temp2 != end->next) {
			distance_StartToEnd++;
			if (min->energy > temp2->energy) {
				min = temp2;
				distance_StartToMin = distance_StartToEnd;
			}
			temp2 = temp2->next;
		}

		int k = 0;
		while (k <= distance_StartToEnd - distance_StartToMin) {
			min->print();
			min = min->next;
			k++;
		}

		int l = 0;
		while (l < distance_StartToMin - 1) {
			start->print();
			start = start->next;
			l++;
		}
	}

	void 		UNLIMITED_findOptimalSubarray(customer* tempaz, int size, customer*& start, customer*& end, int& maxCount, int& result) {
		int i = 0;
		while (i < size) {
			int curr_sum = 0;
			customer* tempbz = tempaz;

			int j = 0;
			while (j < size) {
				curr_sum += tempbz->energy;

				if (j >= 3 && (result > curr_sum || result == curr_sum && j + 1 >= maxCount)) {
					start = tempaz;
					end = tempbz;
					maxCount = j + 1;
					result = curr_sum;
				}
				tempbz = tempbz->next;
				j++;
			}
			tempaz = tempaz->next;
			i++;
		}
	}

	bool 		PURPLE_Cus1BeforeCus2(customer* cus1, customer* cus2){
		Time* run = Time_HeadCustomer;
		while (run){
			if (run->data == cus1) {return false;}
			else if (run->data == cus2) {return true;}
			run = run->next;
		}
		return true;
	}
	
	customer* 	PURPLE_CustomerAt(customer* cus, int index) {
		while (index > 0 && cus != NULL) {
			cus = cus->next;
			index--;
		}
		return cus;
	}
	
	int 		PURPLE_inssort2(customer* head, int n, int incr) {
		int swapNumber = 0;
		int i = incr;
		
		do {
			int j = i;

			do {
				customer* cus1 = PURPLE_CustomerAt(head, j);
				customer* cus2 = PURPLE_CustomerAt(head, j - incr);

				if ((abs(cus1->energy) < abs(cus2->energy)) || (abs(cus1->energy) == abs(cus2->energy) && PURPLE_Cus1BeforeCus2(cus1, cus2))) {
					break;
				}

				REVERSAL_swapAddress(cus1, cus2);
				swapNumber++;

				if (cus2 == WaitList_Customer) {
					WaitList_Customer = cus1;
				}
				if (head == cus2) {
					head = cus1;
				}

				j -= incr;
			} while (j >= incr);

			i += incr;
		} while (i < n);

		return swapNumber;
	}

	Time* 		PURPLE_FindMaxEnergyCustomer() {
		Time* temp = Time_TailCustomer;
		Time* timeMax = nullptr;
		int q = 0;
		do {
			if ((!temp->isSittingOnTable && !timeMax) || (!temp->isSittingOnTable && abs(temp->data->energy) > abs(timeMax->data->energy))) {
				timeMax = temp;
			}
			temp = temp->prev;
			q++;
		} while (q < Ressize());

		return timeMax;
	}

	int 		PURPLE_CalculateWaitlistSize(Time* timeMax) {
		customer* FirstWaitlistCustomer = WaitList_Customer;
		int size = 1;
		while (FirstWaitlistCustomer != timeMax->data) {
			FirstWaitlistCustomer = FirstWaitlistCustomer->next;
			size++;
		}
		return size;
	}

	void 		PURPLE_SortWaitlistCustomers(int size, int& timeToBLUE) {
		for (int i = size / 2; i > 2; i /= 2) {
			for (int j = 0; j < i; j++) {
				timeToBLUE += PURPLE_inssort2(PURPLE_CustomerAt(WaitList_Customer, j), size - j, i);
			}
		}
		timeToBLUE += PURPLE_inssort2(WaitList_Customer, size, 1);
	}

	void		LIGHT_PrintWaitlist(){
		//in thông tin khách trong hàng chờ
		if (WaitList_CurrentSize != 0){
			customer* temp = WaitList_Customer;
			for (int i = 0; i < WaitList_CurrentSize; i++){
				temp->print();
				temp = temp->next;
			}
		}
	}

	void 		LIGHT_PrintTablelist_Clockwise(){
		//in thông tin khách trong bàn cùng chiều kim đồng hồ
		customer* temp = Table_Customer;
		for (int i = 0; i < Table_CurrentSize; i++){
			temp->print();
			temp = temp->next;
		}
	}

	void		LIGHT_PrintTablelist_counterClockwise(){
		//in thông tin khách trong bàn ngược chiều kim đồng hồ
		customer* temp = Table_Customer;
		for (int i = 0; i < Table_CurrentSize; i++){
			temp->print();
			temp = temp->prev;
		}
	}





	void RED(string name, int energy) {
		if (energy == 0 || RED_RestaurantIsFull() || RED_NameIsExistInRestaurant(name)) {
			ResPrint();
			return;
		}

		customer* newCustomer = new customer(name, energy, nullptr, nullptr);

		if (Table_CurrentSize == 0) {
			Table_Customer = newCustomer;
			Table_Customer->next = Table_Customer;
			Table_Customer->prev = Table_Customer;

			Table_CurrentSize++;
			Time_HeadCustomer = new Time(Table_Customer, true);
			Time_TailCustomer = Time_HeadCustomer;
		} 

		else if (Table_CurrentSize == MAXSIZE) {
			//nếu hàng chờ chưa có ai
			if (WaitList_CurrentSize == 0) {
				WaitList_Customer = newCustomer;
				WaitList_Customer->next = WaitList_Customer;
				WaitList_Customer->prev = WaitList_Customer;
			} 
			//nếu hàng chờ đã có người
			else { 
				newCustomer->next = WaitList_Customer;
				newCustomer->prev = WaitList_Customer->prev;
				WaitList_Customer->prev->next = newCustomer;
				WaitList_Customer->prev = newCustomer;
			}
			//cập nhật Time và size
			WaitList_CurrentSize++;
			RED_UpdateCustomerTime(newCustomer, 0);
			ResPrint();
		}
		//nếu khách trong bàn >= maxsize/2
		else if (Table_CurrentSize >= MAXSIZE / 2) {
			int max_diff = 0;
			int RESwithoutAbsolute = 0;
			customer* maxDiffCustomer = nullptr;
			customer* temp = Table_Customer;



			//tìm khách hàng có chệnh lệch lớn nhất
			do {
				int current_diff = abs(energy - temp->energy);
				if (current_diff > max_diff) {
					max_diff = current_diff;
					maxDiffCustomer = temp;
					RESwithoutAbsolute = energy - temp->energy;
				}
				temp = temp->next;
			} while (temp != Table_Customer);


			
			//nếu RES > 0 -> thêm cùng chiều kim đồng hồ
			if (RESwithoutAbsolute > 0) 		{RED_addCustomer_Clockwise(newCustomer, maxDiffCustomer);} 
			//nếu RES < 0 -> thêm ngược chiều kim đồng hồ
			else if (RESwithoutAbsolute < 0) 	{RED_addCustomer_counterClockwise(newCustomer, maxDiffCustomer);}

			//cập nhật Time và size
			Table_Customer = newCustomer;
			Table_CurrentSize++;
			RED_UpdateCustomerTime(Table_Customer, 1);
			ResPrint();
		} 
		
		else {
			//nếu năng lượng lớn hơn -> thêm cùng chiều kim đồng hồ
			if (energy >= Table_Customer->energy) 	{RED_addCustomer_Clockwise(newCustomer, Table_Customer);} 
			//nếu năng lượng bé hơn -> thêm ngược chiều kim đồng hồ
			else 									{RED_addCustomer_counterClockwise(newCustomer, Table_Customer);}

			//cập nhật Time và size
			Table_Customer = newCustomer;
			Table_CurrentSize++;
			RED_UpdateCustomerTime(Table_Customer, 1);
			ResPrint();
		}
	}

	void BLUE(int num) {
		if (Table_CurrentSize == 0) {
			ResPrint();
			return;
		}
		
		int number_customersKicked = 0;
		
		while (number_customersKicked < num && Table_CurrentSize > 0) {
			//tìm khách hàng đầu tiên trong bàn
			Time* kickedCustomer = BLUE_FirstTableCustomerInTime();
			//đuổi
			GENRL_KickOutOfRestaurant(kickedCustomer);
			number_customersKicked++;
		}
		//tuyển khách mới
		GENRL_MoveCustomer_WaitlistToTable();
	}

	void PURPLE() {
		if (WaitList_CurrentSize <= 1) {
			ResPrint();
			return;
		}

		int timeToBLUE = 0;
		//Tìm khách hàng có năng lượng lớn nhất
		Time* timeMax = PURPLE_FindMaxEnergyCustomer();
		//Tìm size
		int size = PURPLE_CalculateWaitlistSize(timeMax);
		//Shellsort hàng chờ
		PURPLE_SortWaitlistCustomers(size, timeToBLUE);
		//đuổi khách
		BLUE(timeToBLUE % MAXSIZE);
	}

 	void REVERSAL() {
		if (Table_CurrentSize <= 1) {
			ResPrint();
			return;
		}
		
		customer* start = nullptr;
		customer* end = nullptr;
		customer* StoreAddressCustomer = Table_Customer;

		int i = 0;
		while (i < Table_CurrentSize) {
			if (StoreAddressCustomer->energy > 0) {
				break;
			}
			StoreAddressCustomer = StoreAddressCustomer->prev;
			i++;
		}
		
		start = Table_Customer;
		i = 0;
		while (i < Table_CurrentSize) {
			if (start->energy < 0) {
				break;
			}
			start = start->prev;
			i++;
		}
		
		end = Table_Customer->next;
		i = 0;
		while (i < Table_CurrentSize) {
			if (end->energy < 0) {
				break;
			}
			end = end->next;
			i++;
		}
		
		if (start->energy < 0 && start != end) {
			i = 0;
			while (i < Table_CurrentSize/2) {
				REVERSAL_swapAddress(start, end);
				customer* temp = start;
				start = end;
				end = temp;
				
				start = start->prev;
				int j = 0;
				while (j < Table_CurrentSize) {
					if (start->energy < 0) {
						break;
					}
					start = start->prev;
					j++;
				}
				if (start == end) {
					break;
				}
				
				end = end->next;
				j = 0;
				while (j < Table_CurrentSize) {
					if (end->energy < 0) {
						break;
					}
					end = end->next;
					j++;
				}
				if (start == end) {
					break;
				}
				
				i++;
			}
		}
		
		start = StoreAddressCustomer;
		i = 0;
		while (i < Table_CurrentSize) {
			if (start->energy > 0) {break;}
			start = start->prev;
			i++;
		}
		
		end = StoreAddressCustomer->next;
		i = 0;
		while (i < Table_CurrentSize) {
			if (end->energy > 0) {break;}
			end = end->next;
			i++;
		}
		
		if (start->energy > 0 && start != end) {
			i = 0;
			while (i < Table_CurrentSize / 2) {
				REVERSAL_swapAddress(start, end);
				customer* temp = start;
				start = end;
				end = temp;
				
				start = start->prev;
				int j = 0;
				while (j < Table_CurrentSize) {
					if (start->energy > 0) {break;}
					start = start->prev;
					j++;
				}
				if (start == end) {break;}
				
				end = end->next;
				j = 0;
				while (j < Table_CurrentSize) {
					if (end->energy > 0) {break;}
					end = end->next;
					j++;
				}
				if (start == end) {break;}
				
				i++;
			}
		}
	}

	void UNLIMITED_VOID() {
		if (Table_CurrentSize <= 3) {
			ResPrint();
			return;
		}

		int result = INT32_MAX;
		int maxCount = 0;
		customer* start = nullptr;
		customer* end = nullptr;
		customer* tempaz = Table_Customer;
		//tìm chuỗi con hợp lệ
		UNLIMITED_findOptimalSubarray(tempaz, Table_CurrentSize, start, end, maxCount, result);
		//tìm min, sau đó in ra từ min theo chiều kim đồng hồ
		UNLIMITED_processSubarray(start, end);
	}

	void DOMAIN_EXPANSION() {
		if (Table_CurrentSize == 0){
			ResPrint();
			return;
		}

	// Tính tổng năng lượng trong bàn ăn và hàng chờ
		int sumOfPositiveEnergy = 0;
		int sumOfNegativeEnergy = 0;
		DOMAIN_calculateEnergySums(sumOfPositiveEnergy, sumOfNegativeEnergy);

		// Tách khách hàng thành hai danh sách: Magician và Soul
		Time* MagicianHead = nullptr;
		Time* MagicianTail = nullptr;
		Time* SoulHead = nullptr;
		Time* SoulTail = nullptr;
		Time* Time_tempRun = Time_HeadCustomer;

		//chia danh sách
		DOMAIN_separateCustomers(MagicianHead, MagicianTail, SoulHead, SoulTail);


	// Nếu tổng năng lượng dương lớn hơn hoặc bằng giá trị năng lượng âm tuyệt đối
		if (sumOfPositiveEnergy >= abs(sumOfNegativeEnergy)) {
			// Cập nhật danh sách Time cho khách hàng Magician
			Time_HeadCustomer = MagicianHead;
			Time_TailCustomer = MagicianTail;
			// In các khách hàng Soul theo thứ tự ngược
			Time* printR = SoulTail;
			while (printR) {
				printR->data->print();
				printR = printR->prev;
			}
			// Xóa khách hàng Soul khỏi danh sách
			while (SoulHead) {
				Time* customerTimeDelete = SoulHead;
				SoulHead = SoulHead->next;
				GENRL_KickOutOfRestaurant(customerTimeDelete);
			}
		}
	// Nếu tổng năng lượng dương nhỏ hơn giá trị năng lượng âm tuyệt đối
		else if (sumOfPositiveEnergy < abs(sumOfNegativeEnergy)) {
			// Cập nhật danh sách Time cho khách hàng Soul
			Time_HeadCustomer = SoulHead;
			Time_TailCustomer = SoulTail;
			// In các khách hàng Magician theo thứtự ngược
			Time* printR = MagicianTail;
			while (printR) {
				printR->data->print();
				printR = printR->prev;
			}
			// Xóa khách hàng Magician khỏi danh sách
			while (MagicianHead != nullptr) {
				Time* customerTimeDelete = MagicianHead;
				MagicianHead = MagicianHead->next;
				GENRL_KickOutOfRestaurant(customerTimeDelete);
			}
		}
		// Di chuyển khách hàng từ hàng chờ vào bàn ăn
		GENRL_MoveCustomer_WaitlistToTable();
	}

	void LIGHT(int num){ //OK
			//cout << "light " << num << endl;

			//in thông tin khách trong hàng chờ
			if (num == 0) 			{LIGHT_PrintWaitlist();}
			//in thông tin khách trong bàn cùng chiều kim đồng hồ
			else if (num > 0)		{LIGHT_PrintTablelist_Clockwise();}
			//in thông tin khách trong bàn ngược chiều kim đồng hồ
			else if (num < 0)		{LIGHT_PrintTablelist_counterClockwise();}


			ResPrint();
			return;
	}


};