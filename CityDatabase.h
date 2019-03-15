
#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

class CityDatabase
{
   private:
       typedef struct Coordinate
       {
          string name;
          float latitude;
          float longitude;
          Coordinate* next;
          Coordinate* prev;
       };
       Coordinate* head;
       Coordinate* last;
   public:
       CityDatabase()
       {
         head = NULL;
         last = NULL;
       }
       bool empty() const { return head==NULL; }
       friend ostream& operator<<(ostream& ,const CityDatabase& );
       void Insert(const string& );
       void Remove(const string& );
};

void CityDatabase::Insert(const string& s)
{
    // Insertion into an Empty List.
    if(empty())
    {
       Coordinate* temp = new Coordinate;
       head = temp;
       last = temp;
       temp->prev = NUuLL;
       temp->next = NULL;
       temp->name = s;
    }
    else
    {
       Coordinate* curr;
       curr = head;
       while( s>curr->name && curr->next != last->next) curr = curr->next;

       if(curr == head)
       {
         Coordinate* temp = new Coordinate;
         temp->name = s;
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
         last->next = new Coordinate;
         (last->next)->prev = last;
         last = last->next;
         last->next = NULL;
         last->name = s;
      //  cout<<" Added "<<s<<" at the end "<<endl;
       }
       else
       {
         Coordinate* temp = new Coordinate;
         temp->name = s;
         temp->next = curr;
         (curr->prev)->next = temp;
         temp->prev = curr->prev;
         curr->prev = temp;
      //  cout<<" Inserted "<<s<<" Before "<<curr->name<<endl;
       }
      }
    }
}

ostream& operator<<(ostream& ostr, const CityDatabase& dl )
{
    if(dl.empty()) ostr<<" The list is empty. "<<endl;
    else
    {
        CityDatabase::Coordinate* curr;
        for(curr = dl.head; curr != dl.last->next; curr=curr->next)
          ostr<<curr->name<<" ";
        ostr<<endl;
        ostr<<endl;
        return ostr;
    }
}

void CityDatabase::Remove(const string& s)
{
    bool found = false;
    if(empty())
    {
      cout<<" This is an empty list! "<<endl;
      return;
    }
    else
    {
      Coordinate* curr;
      for(curr = head; curr != last->next; curr = curr->next)
      {
          if(curr->name == s)
          {
             found = true;
             break;
          }
      }
      if(found == false)
      {
       cout<<" The list does not contain specified Coordinate"<<endl;
       return;
      }
      else
      {
         // Curr points to the node to be removed.
         if (curr == head && found)
         {
           if(curr->next != NULL)
           {
            head = curr->next;
            delete curr;
            return;
           }
           else
           {
            delete curr;
            head = NULL;
            last = NULL;
            return;
           }
         }
        if (curr == last && found)
        {y
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
