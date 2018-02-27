#include<iostream>
#include<cstring>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include<iterator>

// Macro when the desired name was not found
#define NOT_FOUND -1

using namespace std;

/*Returns first name from full_name string*/
string ex_F_Name(string full_name){
	string fname;
	int pos = full_name.find(" ");
	fname = full_name.substr(0,pos);
	return fname;
}

/*Returns last name from full_name string*/
string ex_L_Name(string full_name){
	string lname;
	int pos = full_name.find(" ");
	lname = full_name.substr(pos,full_name.length()-1);
	return lname;
}

/*Returns Lastname firstname form 8*/
string lastFirst(string fn){
	return ex_L_Name(fn) + ex_F_Name(fn);
}

/*Class used for each entry in the phone book*/
class Entry{
private:
	string full_name;
	string last_first;
	string phone;

public:
	//Destructor for object
	~Entry(){;}	
	//Default constructor for object
	Entry(){;}
	//Constructor for object when full name and number are given
	Entry(string full_name, string phone){
		this->full_name = full_name;
		this->phone = phone;
		this->last_first = lastFirst(this->full_name);
	}

	//Getter function for full name in format "Firstname Lastname"
	string getFullName(){
		return this->full_name;
	}

	//Getter function for full name in format "LastnameFirstName" for sorting purposes
	string getLastFirst(){
		return this->last_first;
	}

	//Getter function for the phone number string
	string getPhone(){
		return this->phone;
	}

	//Displaying the contents of the entry
	void displayEntry(){
		cout << this->full_name << " " << this->phone << endl;
	}
};

//The phone directory, as a vector with objects of the Entry class defined above
vector<Entry> phonebook;

//Function which splits the array into left and right arrays and merges them according to alphabetical order
void SplitAndMerge(vector<Entry> &phonebook, int start, int mid, int end){
	
	//Size of left vector
	int l_size = mid - start + 1;

	//Size of right vector	
	int r_size = end - mid;

	vector<Entry> left_array(l_size), right_array(r_size);

	//Copy first half elements from the vector which has to be split	
	for(int i=0;i<l_size;i++){

	//start + i to copy the first half
		left_array[i] = phonebook[start+i];	

	} 

	//Copy the next half elements from the vector which has to be split
	for(int j=0;j<r_size;j++){

	//mid+j+1 to copy the next half
		right_array[j] =phonebook[mid + j + 1];	
	
	}

	int i=0,j=0;

	//Start the merge from the first position in the left vector which is denoted by start
	int com_p = start;

	while(i < l_size && j < r_size){

		/*
		If the element pointed in left vector is alphabetically smaller than element pointed in right vector
		then copy the element from left vector and increment pointer, otherwise copy from right and increment pointer
		*/
		if(left_array[i].getLastFirst().compare(right_array[j].getLastFirst()) <= 0){
			phonebook[com_p] = left_array[i];
			i++;
		}
		else{
			phonebook[com_p] = right_array[j];
			j++;
		}
		com_p++;
	}

	/*
	Since the above loop ends as soon as i==l_size or j==r_size, we have to copy the elements which might be left
	in any of these vectors
	*/
	while(i < l_size){
		phonebook[com_p] = left_array[i];
		i++;
		com_p++;
	}

	while(j < r_size){
		phonebook[com_p] = right_array[j];
		j++;
		com_p++;
	}
}

/* Merge sort function that recursively calls itself till we reach one element. 
If start == end meaning only one element is left, and that would be the terminating condition*/
void MergeSort(vector<Entry> &phonebook, int start, int end){
	if(start < end){
		// Calculate mid
		int mid = start + (end - start)/2;

		// Split the array into left and right
		MergeSort(phonebook,start,mid);
		MergeSort(phonebook,mid+1,end);

		// Merge the arrays
		SplitAndMerge(phonebook,start,mid,end);
	}
}

/* Swaps the data in the objects, used for quicksort partitions */
void Swap(Entry &a, Entry &b){
	Entry temp;
	temp = a;
	a = b;
	b = temp;
}

/* Returns the position of the pivot once it has successfully partitioned. 
That is, all the elements to the left of the pivot are smaller than pivot 
and all elements to right of pivot are greater than the pivot. 
The return value is the place where the pivot is placed in the array. 
Also the pivot is chosen at random and is placed at the start of vector for easier computation */
int PlaceRandomPivot(vector<Entry> &phonebook, int start, int end){
	int random = start + rand()%(end - start +1);
	Swap(phonebook[start],phonebook[random]);

	Entry pivot = phonebook[start];
	int pivot_pos = end + 1;

	for(int i= end;i>=start;i--){
		if(phonebook[i].getPhone().compare(pivot.getPhone()) > 0){
			pivot_pos--;
			Swap(phonebook[i],phonebook[pivot_pos]);
		}
	}

	pivot_pos--;
	Swap(phonebook[pivot_pos],phonebook[start]);
	return pivot_pos;
}

/* The function of random quick sort that recursively calls itself */
void RandomQuickSort(vector<Entry> &phonebook, int start, int end){
	if(start < end){
		// Place the pivot after sorting and return the value
		int pivot = PlaceRandomPivot(phonebook,start,end);

		// Recursively call the function on left and right halves
		RandomQuickSort(phonebook,start,pivot-1);
		RandomQuickSort(phonebook,pivot+1,end);
	}
}

/* The function which only performs quick sort on the entries with same names*/
void RQuickSort(){
	// The variables to find the repeated index
	int start = 0,end=0;
	for(int i=0;i<phonebook.size();i++){
		if(phonebook[i].getLastFirst().compare(phonebook[i+1].getLastFirst()) == 0){
			end++;
		}
		//	As soon as a the names differ, quick sort on those entries if there are more than 1
		else{	
			if((end - start) >= 1)
				RandomQuickSort(phonebook,start,end);
			// Move the pointers to the next entry
			start = end + 1;
			end = start;
		}
	}
}

/* Merge sort and then quick sort */
void sortDirectory(){
	MergeSort(phonebook,0,phonebook.size()-1);
	RQuickSort();
}

/* Binary search for searching the index of the name given*/
int BinarySearch(vector<Entry> &phonebook, string name, int low, int high){
	if(low <= high){
		int mid = low + (high - low)/2;
		if(phonebook[mid].getLastFirst().compare(name) == 0)
			return mid;
		else if(phonebook[mid].getLastFirst().compare(name) < 0)
			return BinarySearch(phonebook,name,mid+1,high);
		else
			return BinarySearch(phonebook,name,low,mid-1);
	}

	return NOT_FOUND;
}

/* Function to search a given name and return their data
along with all the entries with the same names */
void searchDirectory(char *name){
	string nm = string(name);	
	nm = lastFirst(nm);
	//	Get the index using binary search
	int index = BinarySearch(phonebook,nm,0,phonebook.size()-1);	
	/*	If the index is the last element, 
	meaning there are no other same elements, so print the details out */
	if(index == (phonebook.size()-1)){
		cout << index << " " << phonebook[index].getFullName() << " " << phonebook[index].getPhone() << endl;
	}
	else if(index == NOT_FOUND)
		cout << "Name not found" << endl;
	else{
		/* If it is not the last element, then if the entries before it or after it
		have same names */
		int r = index;
		if(r>0){
			while(phonebook[r].getFullName().compare(phonebook[r-1].getFullName()) == 0){
				cout << r-1 << " " << phonebook[r-1].getFullName() << " " << phonebook[r-1].getPhone() << endl;
				r--;
			}
		}
		cout << index << " " << phonebook[index].getFullName() << " " << phonebook[index].getPhone() << endl;
		while(phonebook[index].getFullName().compare(phonebook[index+1].getFullName()) == 0){
			index++;
			cout << index << " " << phonebook[index].getFullName() << " " << phonebook[index].getPhone() << endl;
		}
	}
}

/* Function that uses Binary search to get the last entry with the given name */
int getLastLocation(char *name){
	string nm = string(name);	
	nm = lastFirst(nm);
	int index = BinarySearch(phonebook,nm,0,phonebook.size()-1);
	if(index == (phonebook.size()-1))
		return index;
	else if(index == NOT_FOUND)
		return NOT_FOUND;
	else{
		while(phonebook[index].getFullName().compare(phonebook[index+1].getFullName()) == 0)
			index++;
		return index;
	}
}

/* Function that inserts a new entry given a full name and phone number */
void insertEntry(char *full_name, char *phone){
	string fn = string(full_name);
	string ph = string(phone);
	// Create a new entry object
	Entry *latest = new Entry(fn,ph);
	fn = lastFirst(fn);
	int index;
	//	Try to find if the entry with same name exists, if not simply push the entry into phonebook
	if((index = BinarySearch(phonebook,fn,0,phonebook.size()-1)) == NOT_FOUND)
		phonebook.push_back(*latest);
	else{
		/* If there is an entry with the name, then check if the phone number is same,
		if it is then print out the message and return nothing. Else increment the counter to check
		if next entry has same name and number */
		while(phonebook[index].getLastFirst().compare(fn) == 0){
			if(phonebook[index].getPhone().compare(ph) == 0){
				cout << "Entry already exists in the directory\n" << endl;
				return;
			}
			else{
				index++;
			}
		}
		// If not then push the entry object into the phonebook
		phonebook.push_back(*latest);
		delete latest;
	}
}

// Similar to insert entry, but this does not print "entry exists" it skips that entry
void addEntry(char *full_name, char *phone){
	string fn = string(full_name);
	string ph = string(phone);
	// Create a new entry object
	Entry *latest = new Entry(fn,ph);
	fn = lastFirst(fn);
	int index;
	//	Try to find if the entry with same name exists, if not simply push the entry into phonebook
	if((index = BinarySearch(phonebook,fn,0,phonebook.size()-1)) == NOT_FOUND)
		phonebook.push_back(*latest);
	else{
		/* If there is an entry with the name, then check if the phone number is same,
		if it is then return nothing. Else increment the counter to check
		if next entry has same name and number */
		while(phonebook[index].getLastFirst().compare(fn) == 0){
			if(phonebook[index].getPhone().compare(ph) == 0){
				return;
			}
			else{
				index++;
			}
		}
		// If not then push the entry object into the phonebook
		phonebook.push_back(*latest);
		delete latest;
	}
}

/* Function that takes each line from the file and stores it into the phonebook */
void create(string filePath){
	//	Create ifstream object with the filepath
	ifstream input(filePath);

	//	Error if file was not able to open
	if(!input){
		cout << "Error reading the file - " << filePath << endl;
		exit(0);
	}
	string line;
	while(getline(input,line)){
		istringstream ss(line);
		string i, fn, ln, ph;
		//	Stream the data according to the format
		ss >> i >> fn >> ln >> ph;
		ln[ln.length()-1]='\0';

		int size_f = fn.length() + ln.length() + 1;
		char full_name[size_f];
		strcpy(full_name,fn.c_str());
		strcat(full_name," ");
		strcat(full_name,ln.c_str());

		int size_p = ph.length();
		char phone[size_p];
		strcpy(phone,ph.c_str());

		addEntry(full_name,phone);
	}
}

/* Display allthe entries in the phonebook */
void displayDirectory(){
	cout << "Phone Directory - " << endl;
	for(int i=0;i<phonebook.size();i++)
		phonebook[i].displayEntry();
}

/* Delete the last entry with the given name */
void deleteEntry(char *name){
	int i = getLastLocation(name);
	if( i != NOT_FOUND){
		phonebook.erase(phonebook.begin() + i);
		displayDirectory();
	}
	else
		cout << "Entry does not exists" << endl;
}

int main(int argc, char *argv[]){
	if(!argv[1]){
		cout << "Usage: " << argv[0] << " <filename>" << endl;
		exit(1);
	}

	create(argv[1]);
	sortDirectory();
	displayDirectory();
	int option;
	while(true){
		cout << endl;
		cout << "Please provite options:" << endl;
		cout << "Insert         (press 1)" << endl;
		cout << "Search         (press 2)" << endl;
		cout << "Delete         (press 3)" << endl;
		cout << "Show Directory (press 4)" << endl;
		cout << "Exit           (press 5)" << endl;
		cin >> option;
		cout << endl;
		if(option == 1){
			string fn,ln;
			char ph[20];
			cout << "Enter the full name and phone number: ";
			cin >> fn >> ln;
			cin.getline(ph,20);
			// The getline above takes in a whitepsace before, so we take a char pointer and point it to the char after white space
			char *phone= NULL;
			phone = ph +1;
			int size_f = fn.length() + ln.length() + 1;
			char full_name[size_f];
			strcpy(full_name,fn.c_str());
			strcat(full_name," ");
			strcat(full_name,ln.c_str());

			insertEntry(full_name,phone);
			sortDirectory();
			displayDirectory();
		}
		else if(option == 2){
			string fn,ln;
			cout << "Enter the name you want to search: ";
			cin >> fn >> ln ;
			int size_f = fn.length() + ln.length() + 1;
			char full_name[size_f];
			strcpy(full_name,fn.c_str());
			strcat(full_name," ");
			strcat(full_name,ln.c_str());

			searchDirectory(full_name);
		}
		else if(option == 3){
			string fn,ln;
			cout << "Enter the name you want to delete: ";
			cin >> fn >> ln ;
			int size_f = fn.length() + ln.length() + 1;
			char full_name[size_f];
			strcpy(full_name,fn.c_str());
			strcat(full_name," ");
			strcat(full_name,ln.c_str());

			deleteEntry(full_name);
		}
		else if(option == 4){
			displayDirectory();
		}
		else if(option == 5){
			exit(0);
		}
		else{
			cout << "Enter correct option" << endl;
		}
	}
	return 0;
}