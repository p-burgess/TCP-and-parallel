
#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

#include "CityDatabase.cpp"
int main()
{
    CityDatabase Clist;
    int ch;
    string temp;
    float Latitude;
    float Longitude;
    
    while(1)
    {
       cout<<endl;
       cout<<" Doubly Linked List Operations "<<endl;
       cout<<" ------------------------------"<<endl;
       cout<<" 1. Insertion "<<endl;
       cout<<" 2. Deletion "<<endl;
       cout<<" 3. Display "<<endl;
       cout<<" 4. Exit "<<endl;
       cout<<" Enter your choice : ";
       cin>>ch;
       switch(ch)
       {
          case 1: cout<<" Enter Name to be inserted : ";
                  cin>>temp;
                  cout << "Enter Latitude : ";
                  cin >> Latitude;
                  cout << "Enter Longitude : ";
                  cin >> Longitude;
                  Clist.Insert(temp, Latitude, Longitude );
                  break;
          case 2: cout<<" Enter Name to be deleted : ";
                  cin>>temp;
                  Clist.Remove(temp);
                  break;
          case 3: cout<<" The List contains : ";
                  cout<<Clist;
                  break;
          case 4: system("pause");
                  return 0;
                  break;
       }
    }
    }
    
    
    
