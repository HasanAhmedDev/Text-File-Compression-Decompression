#include<iostream>
#include<string>
#include<fstream>
#include<list>
using namespace std;
int nextprime(int number)
{
	int prime = number;
	bool check = true;
	while (check)
	{
		bool primecheck = true;
		for (int i = 2; i < prime; i++)
		{                                              //Returns the very next prime number of given number
			if (prime % i == 0)
			{
				primecheck = false;
				break;
			}
		}
		if (primecheck)
			check = false;
		else
			prime++;
	}
	return prime;
}

template<class T>
class BT;
class charandfreq;
template<class T>
class bnode
{
	T data;
	bnode* leftchild;                    //Implementation of node class for Binary Tree.
	bnode* rightchild;
public:
	bnode()
	{
		leftchild = nullptr;
		rightchild = nullptr;
	}
	void Display()
	{
		cout << data << endl;
	}
	void insertdata(T _data)
	{
		data = _data;
	}
	bnode(T _data, bnode<T>* l = nullptr, bnode<T>* r = nullptr)
	{
		data = _data;
		leftchild = l;
		rightchild = r;
	}
	bool operator<(const bnode<T>& temp)
	{
		return(data < temp.data);
	}
	bool operator>(const bnode<T>& temp)
	{
		return(data > temp.data);
	}
	bnode<T>& operator=(bnode<T>& second)
	{
		data = second.data;
		leftchild = second.leftchild;
		rightchild = second.rightchild;
		return *this;
	}
	friend class BT<T>;
	friend class MinHeap;
	friend void decompressing(bnode<charandfreq> huffnode, int count); //Making decompressing function friend as it have to traverse the tree made by huffman algo.
};

class charandfreq
{
	char character;                  // Class for storing the charcter and its frequency.
	int frequency;
public:
	charandfreq(char _character = 0, int freq = 0)
	{
		character = _character;
		frequency = freq;
	}
	void increasefreq()
	{
		frequency++;
	}
	friend class HashTable1;
	friend ostream& operator<<(ostream& out, charandfreq& a)
	{
		if (a.character == '\n')
			out << "New Line" << "  " << a.frequency;
		else
			out << a.character << "  " << a.frequency;
		return out;
	}
	bool operator<(const charandfreq& temp)
	{
		return (frequency < temp.frequency);
	}
	bool operator>=(const charandfreq& temp)
	{
		return(frequency >= temp.frequency);
	}
	bool operator>(const charandfreq& temp)
	{
		return(frequency > temp.frequency);
	}
	char getchar()
	{
		return character;
	}
	friend class MinHeap;
};

class charandcode
{                                //Class for storing the character and its huffman code.
	char character;
	string code;
public:
	charandcode(char _character = 0, string _code = "")
	{
		character = _character;
		code = _code;
	}
	friend ostream& operator<<(ostream& out, charandcode& a)
	{
		out << a.character << "  " << a.code;
		return out;
	}
	friend class HashTable2;
};

class HashTable1
{
	int maxsize;
	int sizeused;                      //A hastable to store the character and its frequency in file
	list<charandfreq>* table;
public:
	HashTable1(int size)
	{
		maxsize = size;
		sizeused = 0;
		table = new list<charandfreq>[size];
	}
	int Hashfunction(int number)
	{
		return (number % maxsize);
	}
	void rehashing()
	{
		//if the loadfactor surpasses the 0.75 then make the new hashtable of size of double of previous size and its next prime number then perform the hashfunction on old data using new size.
		int number = maxsize * 2;
		int primenumber = nextprime(number);
		int oldsize;
		charandfreq* olddata = returndata(oldsize);
		list<charandfreq>* temp = new list <charandfreq>[primenumber];
		table = temp;
		sizeused = 0;
		maxsize = primenumber;
		for (int i = 0; i < oldsize; i++)
		{
			bool check = false;
			int number = (int)olddata[i].character;
			int key = Hashfunction(number);
			list<charandfreq>::iterator it;
			for (it = table[key].begin(); it != table[key].end(); it++)
			{
				if (it->character == olddata[i].character)
				{
					it->increasefreq();
					check = true;
					break;
				}
			}
			if (!check)
			{
				table[key].push_back(olddata[i]);
				sizeused++;
			}
		}
		delete[]olddata;
	}
	void insert(charandfreq data)
	{
		// Checks if the data is previously inserted. If so, then only increase frequency, otherwise insert in table.
		float size = float(maxsize);
		float used = float(sizeused);
		float loadfactor = used / size;
		if (loadfactor > 0.75)
		{
			rehashing();
		}
		bool check = false;
		int number = (int)data.character;
		int key = Hashfunction(number);
		list<charandfreq>::iterator it;
		for (it = table[key].begin(); it != table[key].end(); it++)
		{
			if (it->character == data.character)
			{
				it->increasefreq();
				check = true;
				break;
			}
		}
		if (!check)
		{
			table[key].push_back(data);
			sizeused++;
		}
	}
	void DisplayHash()
	{
		// Displays all the data of Hash Table.
		for (int i = 0; i < maxsize; i++)
		{
			list<charandfreq>::iterator it;
			for (it = table[i].begin(); it != table[i].end(); it++)
			{
				cout << *it;
				cout << endl;
			}
		}
	}
	charandfreq* returndata(int& size)
	{
		// Returns all the data of HashTable in an array.
		charandfreq* temp = new charandfreq[sizeused];
		int index = 0;
		for (int i = 0; i < maxsize; i++)
		{
			list<charandfreq>::iterator it;
			for (it = table[i].begin(); it != table[i].end(); it++)
			{
				temp[index] = *it;
				index++;
			}
		}
		size = sizeused;
		return temp;
	}
};

class HashTable2
{
	int maxsize;            //Same as above Hash Table wit the replacement of frequency with code of character.
	int sizeused;
	list<charandcode>* table;
public:
	HashTable2(int size)
	{
		maxsize = size;
		sizeused = 0;
		table = new list<charandcode>[size];
	}
	int Hashfunction(int number)
	{
		return (number % maxsize);
	}
	void rehashing()
	{
		int number = maxsize * 2;
		int primenumber = nextprime(number);
		int oldsize;
		charandcode* olddata = getdata(oldsize);
		list<charandcode>* temp = new list <charandcode>[primenumber];
		table = temp;
		sizeused = 0;
		maxsize = primenumber;
		for (int i = 0; i < oldsize; i++)
		{
			bool check = false;
			int number = (int)olddata[i].character;
			int key = Hashfunction(number);
			list<charandcode>::iterator it;
			for (it = table[key].begin(); it != table[key].end(); it++)
			{
				if (it->character == olddata[i].character)
				{
					check = true;
					break;
				}
			}
			if (!check)
			{
				table[key].push_back(olddata[i]);
				sizeused++;
			}
		}
		delete[]olddata;
	}
	void insert(charandcode data)
	{
		float size = float(maxsize);
		float used = float(sizeused);
		float loadfactor = used / size;
		//cout << loadfactor << endl;
		if (loadfactor > 0.75)
		{
			rehashing();
		}
		bool check = false;
		int number = (int)data.character;
		int key = Hashfunction(number);
		list<charandcode>::iterator it;
		for (it = table[key].begin(); it != table[key].end(); it++)
		{
			if (it->character == data.character)
			{
				check = true;
				break;
			}
		}
		if (!check)
		{
			table[key].push_back(data);
			sizeused++;
		}
	}
	void displayhash()
	{
		for (int i = 0; i < maxsize; i++)
		{
			list<charandcode>::iterator it;
			for (it = table[i].begin(); it != table[i].end(); it++)
			{
				cout << *it;
				cout << endl;
			}
		}
	}
	string GETCODE(char c)
	{
		int num = (int)c;
		int hashindex = Hashfunction(num);
		list<charandcode>::iterator it;
		for (it = table[hashindex].begin(); it != table[hashindex].end(); it++)
		{
			if (it->character == c)
				return it->code;
		}
	}
	charandcode* getdata(int& size)
	{
		charandcode* temp = new charandcode[sizeused];
		int index = 0;
		for (int i = 0; i < maxsize; i++)
		{
			list<charandcode>::iterator it;
			for (it = table[i].begin(); it != table[i].end(); it++)
			{
				temp[index] = *it;
				index++;
			}
		}
		size = sizeused;
		return temp;
	}

};

class MinHeap
{
	int heapsize;                       //Min Heap class for computing the huffman codes.
	int maxsize;
	bnode <charandfreq>* h;
public:
	MinHeap(int size = 10)
	{
		maxsize = size;
		h = new bnode<charandfreq>[maxsize];
		heapsize = 0;
	}
	void insert(bnode<charandfreq> data)
	{
		heapsize++;                //Inserts the node in minheap
		h[heapsize] = data;
		int i = heapsize;
		while (i > 1)
		{
			if (h[i] < h[i / 2])
			{
				bnode<charandfreq> temp = h[i];
				h[i] = h[i / 2];
				h[i / 2] = temp;
				i = i / 2;
			}
			else
				break;
		}

	}
	void MinHeapify(int i)
	{
		int left = 2 * i;                 //Balances the heap by checking all the heap properties.
		int right = 2 * i + 1;
		int largest = i;
		if (left <= heapsize && h[left] < h[largest])
			largest = left;
		if (right <= heapsize && h[right] < h[largest])
			largest = right;
		if (i != largest)
		{
			bnode<charandfreq> temp = h[i];
			h[i] = h[largest];
			h[largest] = temp;
			MinHeapify(largest);

		}
	}
	bnode<charandfreq> Extractmin()
	{
		bnode<charandfreq> data = h[1];  //Extracts the top most node and heapify.
		h[1] = h[heapsize];
		heapsize--;
		MinHeapify(1);
		return data;
	}
	void printcodes(bnode<charandfreq>* root, string code)
	{
		if (root == nullptr)
		{                                        //This function is used to print all the codes of all characters.
			return;
		}

		code.push_back('0');
		printcodes(root->leftchild, code);
		code.pop_back();

		if (root->data.character != 0)
		{
			cout << root->data.character << "\t" << code << endl;
		}

		code.push_back('1');
		printcodes(root->rightchild, code);
		code.pop_back();
	}
	void huffman()
	{
		//Implementaion of huffman Algorithm.
		while (heapsize > 1)
		{
			bnode<charandfreq>* temp = new bnode<charandfreq>(Extractmin());
			bnode<charandfreq>* temp1 = new bnode<charandfreq>(Extractmin());
			bnode<charandfreq>* in = new bnode<charandfreq>(charandfreq(0, temp->data.frequency + temp1->data.frequency), temp, temp1);
			insert(*in);
		}

	}
	void getcode(bnode<charandfreq>* root, char a, string code, string& code1)
	{
		if (root == nullptr)              //This function is used to get code of any character in passed string.
		{
			return;
		}

		code.push_back('0');
		getcode(root->leftchild, a, code, code1);
		code.pop_back();

		if (root->data.character == a)
		{
			code1 = code;

		}

		code.push_back('1');
		getcode(root->rightchild, a, code, code1);
		code.pop_back();
	}
};
void compressing(int& getshiftcount, bnode<charandfreq>& _mainnode)
{
	string content;
	ifstream fin;
	fin.open("Data.txt");
	if (fin.is_open())
	{

		content.assign((istreambuf_iterator<char>(fin)), (istreambuf_iterator<char>()));
		fin.close();
		HashTable1 a(27);
		for (int i = 0; i < content.size(); i++)
		{                                                    //Inserting all the characters in Hashtable A to so we can get frequency of each char.
			charandfreq temp(content[i], 1);
			a.insert(temp);
		}
		charandfreq* DATA;
		int sizeused;
		DATA = a.returndata(sizeused);                      //Getting data from Hash Table A.
		bnode<charandfreq>* trees = new bnode<charandfreq>[sizeused];
		for (int i = 0; i < sizeused; i++)
		{
			trees[i].insertdata(DATA[i]);                  //Creating each node for each character.
		}
		MinHeap c(sizeused + 1);
		for (int i = 0; i < sizeused; i++)
		{
			c.insert(trees[i]);
		}
		c.huffman();
		bnode<charandfreq> mainnode = c.Extractmin();   //After huffman, extracting the only node so we can get codes.
		HashTable2 b(27);
		for (int i = 0; i < sizeused; i++)
		{
			string codeforchar;
			c.getcode(&mainnode, DATA[i].getchar(), "", codeforchar);     //HashTable 2 for inserting code with characters.
			charandcode temp(DATA[i].getchar(), codeforchar);
			b.insert(temp);
		}
		cout << "Displaying Characters and their Frequencies....." << endl << endl;
		a.DisplayHash();
		cout << endl << "Displaying Characters and their Codes....." << endl << endl;
		c.printcodes(&mainnode, "");
		ofstream fout;
		fout.open("Compressed.txt", ios::binary);
		int shiftcount = 0;
		if (fout.is_open())
		{
			fout.clear();
			string codeforchar;
			bool putt = false;
			unsigned char A = 0;
			unsigned char B = 128;
			int start = 1;
			cout << endl << endl << "NOW COMPRESSING FILE....." << endl << endl;
			for (int i = 0; i < content.size(); i++)                                  //Putting data using codes created and making packs of 8 bytes into compressed file.
			{
				codeforchar = b.GETCODE(content[i]);
				for (int j = 0; j < codeforchar.size(); j++)
				{
					if (start != 1)
						B >>= 1;
					start = 0;
					if (codeforchar[j] == '1')
					{
						A = A | B;
					}
					shiftcount++;
					if (shiftcount == 8)
					{
						fout.put(A);
						A = 0;
						B = 128;
						shiftcount = 0;
						start = 1;
					}
				}
			}
			if (shiftcount != 0)
			{
				fout.put(A);
			}
			fout.close();
			cout << "FILE COMPRESSED SUCCESSFULLY....." << endl;
		}
		else
		{
			cout << "File Not Opened....." << endl;
		}
		getshiftcount = shiftcount;          //Returning number of bits left in last byte and the node of codes to be used in decompressing.
		_mainnode = mainnode;
	}
	else
	{
		cout << "File Not Opened....." << endl;
	}

}
void decompressing(bnode<charandfreq> huffnode, int count)
{
	string content;
	ifstream fin;
	fin.open("Compressed.txt", ios::binary);
	if (fin.is_open())
	{

		content.assign((istreambuf_iterator<char>(fin)), (istreambuf_iterator<char>()));
		fin.close();

		unsigned char A = 0;
		unsigned char B = 128;                            //Again using shifting of bytes and using them in huffman node to get character and put in decompressed file.
		unsigned char C = 128;
		int shiftcount = 0;
		int start;
		ofstream fout;
		fout.open("Decompressed.txt");
		if (fout.is_open())
		{
			cout << endl << "NOW DECOMPRESSING FILE....." << endl << endl;
			fout.clear();
			bnode<charandfreq>* temp = &huffnode;
			for (int i = 0; i < content.size(); i++)
			{

				shiftcount = 0;
				B = 128;
				C = 128;
				start = 1;
				A = content[i];
				while (shiftcount != 8)
				{
					shiftcount++;
					if (start == 0)
						B >>= 1;
					C = B;
					C = C & A;
					if (C != 0)
					{
						temp = temp->rightchild;
						if (temp->data.getchar() != 0)
						{

							fout.put(temp->data.getchar());
							temp = &huffnode;

						}
					}
					else
					{
						temp = temp->leftchild;
						if (temp->data.getchar() != 0)
						{

							fout.put(temp->data.getchar());
							temp = &huffnode;

						}
					}
					start = 0;
					if (i == content.size() - 1 && shiftcount == count)
						break;
				}
			}
			cout << "FILE DECOMPRESSED SUCCESFULLY....." << endl << endl;
			fout.close();
		}
		else
		{
			cout << "File Not Opened....." << endl;
		}
	}
	else
	{
		cout << "File Not Opened....." << endl;
	}

}

int main()
{
	//Driver
	bnode<charandfreq> huffmannode;
	int shiftcount;
	compressing(shiftcount, huffmannode);
	decompressing(huffmannode, shiftcount);
	system("pause");
}