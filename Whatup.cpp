#include <iostream>
#include <stdlib.h> 
#include<iomanip>
#include<string>
#include<vector>
#include <ctime>
#include "sqlite3.h"

using namespace std;
sqlite3* db; 

struct menubook
{
    int id ;
    string name;
    int price;

};

struct  table_order
{
    int food_id;
    int order;
    
};

struct daily_income
{
    string date;
    int income;
};

void push_menubook(vector<menubook> &book);
void printmenubook(vector<menubook> &book);
void addorder(vector<table_order> &note , int id, int num);
int find_id(vector<menubook> book ,int id);
int find_name(vector<menubook> book ,string name);
void add_date(vector<daily_income> daily,string date ,int income);
int bill_check(vector<table_order> &note ,vector<menubook> book ,vector<daily_income> daily);
string datenow();

int main()
{
    vector<table_order> table1,table2;
    vector<daily_income> daily;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    if(table1.size()==0)
    cout<<"using";
    else cout<<"no using";
    return 0;

}

void addorder(vector<table_order> &note , int id, int num)
{
    bool having=true;
    for(unsigned int i=0 ;i<note.size();i++)
    {
        if(note[i].food_id==id)
        {
            having=false;
            note[i].order+=num;
        }
    }
    if(having)
    {
        table_order temp={id,num};
        note.push_back(temp);
    }
}

int find_id(vector<menubook> book ,int id)
{
    for(unsigned int i=0 ;i<book.size();i++)
    {
        if(book[i].id==id)
        {
            return i;    
        }
    }
    return -1;

}

int find_name(vector<menubook> book ,string name)
{
    for(unsigned int i=0 ;i<book.size();i++)
    {
        if(book[i].name==name)
        {
            return i;    
        }
    }
    return -1;
}

string datenow()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    string year=to_string(1900 + ltm->tm_year);
    string month=to_string(1 + ltm->tm_mon);
    string day=to_string(ltm->tm_mday);
    string date=year+"-"+month+"-"+day;
    return date;

}

void add_date(vector<daily_income> daily,string date ,int income)
{ 
    for(unsigned int i=0 ;i<daily.size();i++)
    {
        if(daily[i].date==date)
        {
           daily[i].income+=income;
           return ;
        }
    }
        daily_income temp={date,income};
        daily.push_back(temp);
}


int bill_check(vector<table_order> &note ,vector<menubook> book ,vector<daily_income> daily)
{
    double sum=0;
    cout<<"---------------------------------------------------------\n\n";
    cout <<setw(10)<<right<<"Food ID"<<"\t"<<setw(15)<<left <<"Food Name"<<setw(10)<<right<<"Price";
    cout <<setw(10)<<right<<"Order"<<setw(10)<<right<<"Total"<<endl;
    for(unsigned int i=0 ;i<note.size();i++)
    {
        int j=find_id(book,note[i].food_id);
        cout <<setw(10)<<right<<book[j].id<<"\t"<<setw(15)<<left <<book[j].name<<setw(10)<<right<<book[j].price;
        cout <<setw(10)<<right<<note[i].order<<setw(10)<<right<<note[i].order*book[j].price<<endl;
        sum+=note[i].order*book[j].price;
    }
    cout<<"---------------------------------------------------------\n";
    cout<<setw(10)<<right<<" "<<"\t"<<setw(15)<<left <<"SUM"<<setw(30)<<sum;
    cout<<"---------------------------------------------------------\n";
    

    note.clear();
    return sum;
}





bool cancelorder(vector<table_order> &note ,int id)
{
    bool having=true;
    for(unsigned int i=0 ;i<note.size();i++)
    {
        if(note[i].food_id==id)
        {
            
            note.erase(note.begin()+i);
            return true;
        }
    }
    return false;

}





void push_menubook(vector<menubook> &book){

    sqlite3_stmt * stmt;
  
    sqlite3_prepare( db, "SELECT * FROM menu;", -1, &stmt, NULL );
    const unsigned char* text;
    menubook tempbook;
    int num;
    bool done = false;
    while (!done) {
        switch (sqlite3_step(stmt)) {
            case SQLITE_ROW:
            for(int i=0; i<3;i++){
                if(i==0)
                {
                     tempbook.id = sqlite3_column_int(stmt,i);
                     book.push_back(tempbook);
                }
                else if(i==1)
                {
                text = sqlite3_column_text(stmt, i);
                book[book.size()-1].name=*text;
                }
                else if(i==2)
                {
                    num =sqlite3_column_int(stmt,i);
                    book[book.size()-1].price=num;

                }
            }
                break;
            
            case SQLITE_DONE:
                done = true;
                break;

            default:
                fprintf(stderr, "Failed.\n");
                return;
        }
        
    }

    sqlite3_finalize(stmt);

}

void printmenubook(vector<menubook> &book)
{
    for (unsigned int i = 0; i < book.size()-1; i++)
    {
        cout<<book[i].id<<"\t"<<book[i].name<<"\t"<<book[i].price<<endl;

    }
}
