#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
#include <unordered_map>
#include <cassert>
using namespace std;

class PhoneRecord
{
private:
    string name;
    string organisation;
    vector<string> phoneNumbers;

public:
    // Constructor
    PhoneRecord(const string &n, const string &org, const vector<string> &numbers)
        : name(n), organisation(org), phoneNumbers(numbers) {}

    // Getter methods
    string getName() const
    {
        return name;
    }

    string getOrganisation() const
    {
        return organisation;
    }

    vector<string> getPhoneNumbers() const
    {
        return phoneNumbers;
    }
};

class HashTableRecord
{
private:
    int key;
    PhoneRecord *element; // Pointer to PhoneRecord
    HashTableRecord *next;

public:
    // Constructor
    HashTableRecord(int k, PhoneRecord *rec)
        : key(k), element(rec), next(NULL) {}

    // Getter methods
    int getKey() const
    {
        return key;
    }

    PhoneRecord *getRecord() const
    {
        return element;
    }

    HashTableRecord *getNext() const
    {
        return next;
    }

    void setNext(HashTableRecord *nxt)
    {
        next = nxt;
    }
};

class PhoneBook
{
private:
    static const int HASH_TABLE_SIZE = 263;
    HashTableRecord *hashTable[HASH_TABLE_SIZE];

public:
    // Constructor
    PhoneBook()
    {
        for (int i = 0; i < HASH_TABLE_SIZE; ++i)
        {
            hashTable[i] = NULL;
        }
    }

    // Add your own implementation for hashing
    long long computeHash(const string &str)
    {
        int m = 263;
        int p = 1000000007;
        int x = 263;
        long long hash = 0;
        for (int i = 0; i < str.size(); i++)
        {
            long long power = pow(x, i);
            int s = str[i];
            hash += (s * power % p);
        }
        return hash % m;
    }

    // Add your own implementation for adding a contact
    void addContact(PhoneRecord *record)
    {
       // cout << "Adding contact" << endl;
        string p = record->getName();
        assert(record!=NULL  && "record is NULL");
        stringstream ss(p);
        string word;
        // int key=0;
        long long Hash = 0;
        while (ss >> word)
        {
            //cout << "Word [" << word << ']' << endl;
            Hash = computeHash(word);
            while (Hash >= HASH_TABLE_SIZE)
            {
                Hash = Hash % HASH_TABLE_SIZE;
            }
            int key = Hash;
           // cout << "Location " << key << endl;
            // int key = computeHash(word);
            HashTableRecord *record_in_table = new HashTableRecord(key, record);
            if (hashTable[key] == NULL)
            {
                hashTable[key] = record_in_table;
                // hashTable[key]->setNext(record_in_table);
            }
            else
            {
                HashTableRecord *head = hashTable[key];
                while (head->getNext() != NULL)
                {
                    head = head->getNext();
                }
                head->setNext(record_in_table);
            }
        }
        // cout<<"printlist  "<<endl;

        //cout << "Added successfully" << endl;
    }

    void printlist()
    {
        // cout << "************************************" << endl;
        for (int i = 0; i < HASH_TABLE_SIZE; i++)
        {
            HashTableRecord *current = hashTable[i];
            while (current != NULL)
            {
                PhoneRecord *record = current->getRecord();
                cout << "Name " << record->getName() << endl;
                cout << "Organisation " << record->getOrganisation() << endl;

                vector<string> phoneNumbers = record->getPhoneNumbers();
                cout << "Phone Numbers: ";
                for (const string &number : phoneNumbers)
                {
                    cout << number << " ";
                }
                cout << endl;
                current = current->getNext();
            }
        }
    }

    int countRecordsPointingTo(const PhoneRecord *record)
    {
        int count = 0;
        for (int i = 0; i < HASH_TABLE_SIZE; i++)
        {
            HashTableRecord *head = hashTable[i];
            while (head != NULL)
            {
                if (head->getRecord() == record)
                {
                    count++;
                }
                head = head->getNext();
            }
        }
        return count;
        // what if it is just the total number of words in string
    }

    // Add your own implementation for deleting a contactbool deleteContact(const string *searchName)

     // Add your own implementation for fetching contacts
        
bool deleteContact(const string *searchName)
{
    vector<PhoneRecord *> contacts = fetchContacts(searchName);
    if (contacts.empty())
    {
        return false; // Contact not found, return false
    }

    //If there are multiple PhoneRecords retrieved, the first one in the list will be chosen for deletion. Let’s call this one as the selected PhoneRecord object.
    PhoneRecord *recordToDelete = contacts[0];
    bool contactDeleted = false; // Flag to track if at least one contact was deleted

    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        HashTableRecord *head = hashTable[i];

        //  the deletion of the first node 
        if (head != nullptr && head->getRecord() == recordToDelete)
        {
            hashTable[i] = head->getNext();
            delete head;
            contactDeleted = true;
        }
        
        // Check and delete subsequent nodes
        while (head != nullptr)
        {
            HashTableRecord *temp = head->getNext();
            if (temp != nullptr && temp->getRecord() == recordToDelete)
            {
                head->setNext(temp->getNext());
                delete temp;
                contactDeleted = true;
            }
            else
            {
                //head = temp;
                head= head->getNext();
            }
        }
    }
    return contactDeleted;
}

        vector<PhoneRecord *> fetchContacts(const string *query)
        {
            unordered_map<PhoneRecord *, int> recordCount; // To count shared records
            vector<PhoneRecord *> ContactsDirectory;
            stringstream ss(*query);
            string word;
            long long Hash = 0;
            // int count=0;
            while (ss >> word)
            {
                //int key = computeHash(word);
                Hash = computeHash(word);
                while (Hash >= HASH_TABLE_SIZE)
                {
                    Hash = Hash % HASH_TABLE_SIZE;
                }
                int key = Hash;
                HashTableRecord *p = hashTable[key];
                while (p != NULL)
                {
                    PhoneRecord *m = p->getRecord();
                    bool isthere = false;
                    for (int i = 0; i < ContactsDirectory.size(); i++)
                    {
                        if (ContactsDirectory[i] == p->getRecord())
                        {
                            isthere = true;
                            break;
                        }
                    }
                    if (isthere == false)
                    {
                        ContactsDirectory.push_back(p->getRecord());
                        recordCount[m] = countRecordsPointingTo(m);
                    }
                    // countRecordsPointingTo(p->getRecord());
                    p = p->getNext();
                }
            }

            cout<<ContactsDirectory.size()<<endl;
            sort(ContactsDirectory.begin(), ContactsDirectory.end(), [&](PhoneRecord *a, PhoneRecord *b)
                 { return recordCount[a] > recordCount[b]; });

            return ContactsDirectory;
        }

        // Add your own implementation for counting records pointing to a contact

        // Add your own implementation for reading records from a file

        void readRecordsFromFile(const string &filename)
        {
            ifstream input_file(filename);

            if (!input_file.is_open())
            {
                cerr << "Error: Could not open the input file." << endl;
                return;
            }

            // start
            // PhoneBook * contact_details= new PhoneBook();
            string line;
            int i = 0;
            while (true)
            {
                getline(input_file, line);
                // cout<<"The line is : "<<line<<endl;
                if (input_file.peek() == EOF)
                {
                    // cout<<"EOF"<<endl;
                    break;
                }
                // cout<<line<<endl;
                stringstream ss(line);
                string name;
                getline(ss, name, ',');
                // cout << "Name " << name << endl;
                string remaining;
                vector<string> phonenumbers;
                string organisation;
                while (getline(ss, remaining, ','))
                {
                    int p = remaining[0] - 48;
                    if (p >= 0 && p <= 9)
                    {
                        phonenumbers.push_back(remaining);
                        // cout << "Phonenumber " << remaining << endl;
                    }
                    else
                    {
                        organisation = remaining;
                        // cout << "Organisation " << organisation << endl;
                    }
                }
                // cout << "------------------------------------" << endl;
                PhoneRecord *contact = new PhoneRecord(name, organisation, phonenumbers);
                // cout << "Name " << contact->getName() << endl;
                // cout << "Phonenumber " << contact->getPhoneNumbers() << endl;
                // cout << "Organisation " << contact->getOrganisation()<< endl;
                addContact(contact);
            }
            // cout<<"Printing list of records"<<endl;
            input_file.close();
        }

        // Destructor
        ~PhoneBook()
        {
            for (int i = 0; i < HASH_TABLE_SIZE; ++i)
            {
                HashTableRecord *current = hashTable[i];
                while (current != NULL)
                {
                    HashTableRecord *temp = current;
                    current = current->getNext();
                    delete temp;
                }
            }
        }
    };

    /*
    int main()

    {   PhoneBook pari;

        pari.readRecordsFromFile("Details.txt");
        // Your test cases and program logic can go here.
        // read_input_file("Details.txt");
        pari.printlist();
        return 0;
    }
    */
