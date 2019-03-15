
#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

		typedef struct Data
		{
			float la; 
			float lo; 
		} Data;
		
class Dllist
{
	private:
		typedef struct Node
		{
			string name;
			float latitude;
			float longitude;
			Node* next;
			Node* prev;
		} Node;
		Node* head;
		Node* last;

		
	public:
		Dllist()
		{
			head = NULL;
			last = NULL;
		}
		bool empty() const { return head==NULL; }
		Node *find(const Dllist&, string&);
		Data get_Coordinates(string& s);
		friend ostream& operator<<(ostream& ,const Dllist& );
		void Insert(const string&, float, float );
		void Remove(const string& );
};

void Dllist::Insert(const string& s, float La, float Lo)
{
    // Insertion into an Empty List.
    if(empty())
    {
       Node* temp = new Node;
       head = temp;
       last = temp;
       temp->prev = NULL;
       temp->next = NULL;
       temp->name = s;
       temp->latitude = La;
       temp->longitude = Lo;
    }
    else
    {
       Node* curr;
       curr = head;
       while( s>curr->name && curr->next != last->next) curr = curr->next;
       if(curr == head)
       {
         Node* temp = new Node;
         temp->name = s;
         temp->latitude = La;
         temp->longitude = Lo;
         temp->prev = curr;
         temp->next = NULL;
         head->next = temp;
         last = temp;
      //  cout<<" Inserted "<<s<<" After "<<curr->name<<endl;
       }
       else
       {
       if(curr == last && s>last->name)
       {
         last->next = new Node;
         (last->next)->prev = last;
         last = last->next;
         last->next = NULL;
         last->name = s;
         last->latitude = La;
         last->longitude = Lo;
      //  cout<<" Added "<<s<<" at the end "<<endl;
       }
       else
       {
         Node* temp = new Node;
         temp->name = s;
         temp->latitude = La;
         temp->longitude = Lo;
         temp->next = curr;
         (curr->prev)->next = temp;
         temp->prev = curr->prev;
         curr->prev = temp;
      //  cout<<" Inserted "<<s<<" Before "<<curr->name<<endl;
       }
      }
    }
}

ostream& operator<<(ostream& ostr, const Dllist& dl )
{
    if(dl.empty()) ostr<<" The list is empty. "<<endl;
    else
    {
        Dllist::Node* curr;
        for(curr = dl.head; curr != dl.last->next; curr=curr->next) {
          ostr << curr->name <<" ";
          ostr << curr->latitude << " ";
          ostr << curr->longitude << " ";}
        ostr<<endl;
   //     ostr<<endl;
        return ostr;
    }
}

Data Dllist::get_Coordinates(string& s)
{
	bool found = false;
	Data* result = new Data;
	result->la = 0.0;
	result->lo = 0.0;

	if(empty())
	{
	  cout<<" This is an empty list! "<<endl;
	  return (result);
	} else {
		Node* curr;
		for(curr = head; curr != last->next; curr = curr->next) {
			if(curr->name == s) {
				 found = true;
				 break;
			}
		}
		if(found == false) {
			//cout<<" The list does not contain specified Node"<<endl;
			return (result);
		} else {
			// Curr points to the node to be returned.
			result->la = curr->latitude;
			result->lo = curr->longitude;
			return (result);
		}
	}
}	


void Dllist::Remove(const string& s)
{
	bool found = false;
	if(empty())
	{
		cout<<" This is an empty list! "<<endl;
		return;
	} else {
		Node* curr;
		for(curr = head; curr != last->next; curr = curr->next) {
			if(curr->name == s) {
			 found = true;
			 break;
			}
		}
		if(found == false) {
			cout<<" The list does not contain specified Node"<<endl;
			return;
		} else {
			 // Curr points to the node to be removed.
			 if (curr == head && found) {
				if(curr->next != NULL) {
					head = curr->next;
					delete curr;
					return;
				} else {
					delete curr;
					head = NULL;
					last = NULL;
					return;
				}
			}
			if (curr == last && found) {
				last = curr->prev;
				delete curr;
				return;
			}
			(curr->prev)->next = curr->next;
			(curr->next)->prev = curr->prev;
			delete curr;
		}
	}
}
