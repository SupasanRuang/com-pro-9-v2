#include <iostream>
#include <stdlib.h> 
#include<iomanip>
#include<string>
#include<vector>
#include <ctime>
#include "sqlite3.h"

using namespace std;

// Pointer to SQLite connection 
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

char table[9] = {'1','2','3','4','5','6','7','8','9'};
vector<table_order> table1,table2,table3,table4,table5,table6,table7,table8,table9;
vector<daily_income> daily;
vector<menubook> book;

//prototype
void allmenu();     void showmenu(int);    void addmenu();     void deletemenu();      void edit_menu();
int callback(void *NotUsed, int argc, char **argv, char **azColName);

void number_table_order(int); void loop_order(vector<table_order> &table);

void start();   void select_table();
void select_checkbill();

void push_menubook(vector<menubook> &book);                     void printmenubook(vector<menubook> &book);
void addorder(vector<table_order> &note , int id, int num);
int find_id(vector<menubook> book ,int id);                     int find_name(vector<menubook> book ,string name);
void add_date(vector<daily_income> daily,string date ,int income);
void show_bill(vector<table_order> &note ,vector<menubook> book);
int check_bill(vector<table_order> &note ,vector<menubook> book);
bool cancelorder(vector<table_order> &note ,int id);
void show_table_order(vector<table_order> thetable,vector<menubook> book);
vector<table_order>* which_table(int number);
void table_bill();
string datenow();

int main() 
{ 
    sqlite3_stmt * stmt;    // Save the connection result
    int exit = 0;
    exit = sqlite3_open("database.db", &db); 
  
    // Test if there was an error
    if (exit) { 
        cout << "DB Open Error: " << sqlite3_errmsg(db) << endl; 
    } else {
        cout << "Opened Database Successfully!" << endl; 
    }

    push_menubook(book);
    cout<<"printmenubook\n";  
    printmenubook(book) ;
    select_table();
    table_bill();
    unsigned int k=table9.size();
    cout<<k<<endl;
    sqlite3_close(db);// Close the connection
    return (0); 
} 

void start(){
    int select;
    bool done = true;
    
    while(done){
        cout << "--START--";
        cout << "1.Select table ( press 1 )\n" << "2.Check bill ( press 2 )\n" << "3.Edit menu ( press 3 )\n" << "4.Check daily balance ( press 4 )\n";
        cout << "5.Exit ( press 5 )\n";
        cin >> select;

        switch (select)
        {
            case 1:
                select_table();
                break;
            case 2:
                select_checkbill();
                break;
            case 3:
                edit_menu();
                break;
            case 4:
            
                break;
            case 5:
                done = false;
                break;    
            default:

                break;
        }
        
    }
    
}

void select_table(){
    
    int sl_table,count=0;
    for(int j=0 ; j<9;j++){
        cout << table[j] << " ";
        if(table[j]=='-')
        {
            count++;
        }
    }
    cout << endl;
    if(count==9)
    {
        cout<<"Sorry all table are used.\nPlease wait it empty." << endl;
        return;
    }
    while(true)
    {
        cout << "input table want to select : ";
        cin >> sl_table;
        if (sl_table<1||sl_table>9)
        {
            cout<<"---------------------------------------------------------\n";
            cout<<"Don't have this table\n";
            cout<<"---------------------------------------------------------\n";
            continue;
        }
        
        if(table[sl_table]!='-')
        {
           break; 
        }
        else 
        {
            cout<<"Sorry this table are used. Please select table again." << endl;
        }
    }
    table[sl_table-1] = '-';
    for(int j=0 ; j<9;j++){
        cout << table[j] << " ";
    }
    cout << endl;
    number_table_order(sl_table);
}

void select_checkbill(){
    int sl_table;
    cout << "input num : ";
    cin >> sl_table;

    table[sl_table-1] = '-';
    for(int j=0 ; j<9;j++){
        cout << table[j] << " ";
    }
    cout <<endl;
}

void loop_order(vector<table_order> &thetable){
    allmenu();
    int id,num;
    while(true){
        
        cout << "Input order menu ID (If want to exit input 0) : ";
        cin >> id;
        if(id == 0) break;
        int x=find_id(book,id);
        //cout<<x<<endl;
        if(x==-1)
        {
            cout<<"---------------------------------------------------------\n";
            cout<<"Don't have this ID\n";
            cout<<"---------------------------------------------------------\n";
            continue;
        }
        cout << "Input quantity order : ";
        cin >> num;
        if(num>0)
        {
            addorder(thetable,id,num);
            show_table_order(thetable,book);
        }
        else 
        {
            cout << "Input quantity order Error \n";
            cout<<"---------------------------------------------------------\n";
        }       
    }
}

void show_table_order(vector<table_order> thetable,vector<menubook> book)
{
    cout<<"---------------------------------------------------------\n\n";
    cout <<setw(10)<<right<<"Food ID"<<"\t"<<setw(15)<<left <<"Food Name"<<setw(10)<<right<<"Quantity"<<endl;
    for (unsigned int i = 0; i < thetable.size(); i++)
    {
        cout <<setw(10)<<right<<thetable[i].food_id;
        int k= find_id(book,thetable[i].food_id);
        cout <<"\t"<<setw(15)<<left<<book[k].name; 
        cout <<setw(10)<<right<<thetable[i].order<<endl;
    }
    cout<<"---------------------------------------------------------\n";
}


void number_table_order(int tb_num){
    vector<table_order>* vec_point;
    vec_point=which_table(tb_num);
    if(vec_point==NULL)
    {
        return;
    }
    loop_order(*vec_point);
}
vector<table_order>* which_table(int number)
{
    switch (number)
    {
    case 1:
        return &table1;
        break;
    case 2:
        return &table2;
        break;
    case 3:
        return &table3;
        break;
    case 4:
        return &table4;
        break;
    case 5:
        return &table5;
        break;
    case 6:
        return &table6;
        break;
    case 7:
        return &table7;
        break;
    case 8:
        return &table8;
        break;
    case 9:
        return &table9;
        break;
    default:
        return NULL;
        break;
    }     
}

void allmenu(){

    sqlite3_stmt * stmt;
  
    sqlite3_prepare( db, "SELECT * FROM menu;", -1, &stmt, NULL );//preparing the statement

    const unsigned char* text;
    bool done = false;

    cout<<"---------------------------------------------------------\n\n";
    cout <<setw(10)<<right<<"Food ID"<<"\t"<<setw(15)<<left <<"Food Name"<<setw(10)<<right<<"Price"<<endl;
    while (!done) {
        switch (sqlite3_step(stmt)) {
            case SQLITE_ROW:
            for(int i=0; i<3;i++){
                text = sqlite3_column_text(stmt, i);
                if(i==1)
                {
                    cout <<"\t"<<setw(15)<<left; 
                } 
                else cout <<setw(10)<<right;
                cout <<text ;
            }
                break;
            
            case SQLITE_DONE:
                done = true;
                break;

            default:
                fprintf(stderr, "Failed.\n");
                return;
        }
        cout << endl;
    }
    cout<<"---------------------------------------------------------\n";

    sqlite3_finalize(stmt);

}

void showmenu(int id){
    sqlite3_stmt * stmt;

    string str_id = to_string(id);
    string text2 = "SELECT * FROM menu WHERE food_id = " + str_id + " ;" ;

    sqlite3_prepare( db, text2.c_str(), -1, &stmt, NULL );//preparing the statement
    const unsigned char* text;
    sqlite3_step(stmt);
            for(int i=0;i<3;i++){
                text = sqlite3_column_text(stmt,i);
                cout << text << " ";
            }
    sqlite3_finalize(stmt);
}

void addmenu(){
    sqlite3_stmt * stmt;
    
    string name,price;
    cout << "Input food name : ";
    cin >> name ;
    cout << "Input food price : ";
    cin >> price ;

    string sqlstatement = "INSERT INTO menu (food_name, price) VALUES ('" + name + "','" + price + "');";
    sqlite3_prepare( db, sqlstatement.c_str(), -1, &stmt, NULL );//preparing the statement
    sqlite3_exec(db, sqlstatement.c_str(), callback, 0, NULL);
    
}

void deletemenu(){
    string id;
    cout << "Input food ID : ";
    cin >> id;
    sqlite3_stmt * stmt;
    string sqlstatement = "DELETE FROM menu WHERE food_id = ('" + id + "');";
    sqlite3_prepare( db, sqlstatement.c_str(), -1, &stmt, NULL );//preparing the statement
    sqlite3_exec(db, sqlstatement.c_str(), callback, 0, NULL);
}

void edit_menu(){
    int select;
    cout << "what you want to edit\n"<<"1.Add menu ( press 1 )\n"<<"2.Delete menu ( press 2 )\n"<<"3.Go back ( press 3 )\n"<<"Input your choice : ";
    cin >> select;

    if(select == 1) addmenu();
    if(select == 2) deletemenu();
    if(select == 3) ;

}

int callback(void *NotUsed, int argc, char **argv, char **azColName){

    // int argc: holds the number of results
    // (array) azColName: holds each column returned
    // (array) argv: holds each value

    for(int i = 0; i < argc; i++) {
        
        // Show column name, value, and newline
        cout << azColName[i] << ": " << argv[i] << endl;
    
    }

    // Insert a newline
    cout << endl;

    // Return successful
    return 0;
}

void checkbill(){
    sqlite3_stmt * stmt;
  
    sqlite3_prepare( db, "SELECT * FROM menu;", -1, &stmt, NULL );//preparing the statement
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
            return;
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

void show_bill(vector<table_order> &note ,vector<menubook> book)
{
    double sum=0;

    cout <<"---------------------------------------------------------\n\n";
    cout <<setw(10)<<right<<"Food ID"<<"\t"<<setw(15)<<left <<"Food Name"<<setw(10)<<right<<"Price";
    cout <<setw(10)<<right<<"Quantity"<<setw(10)<<right<<"Total"<<endl;
    for(unsigned int i=0 ;i<note.size();i++)
    {
        int j=find_id(book,note[i].food_id);
        cout <<setw(10)<<right<<book[j].id<<"\t"<<setw(15)<<left <<book[j].name<<setw(10)<<right<<book[j].price;
        cout <<setw(10)<<right<<note[i].order<<setw(10)<<right<<note[i].order*book[j].price<<endl;
        sum+=note[i].order*book[j].price;
    }
    cout <<"---------------------------------------------------------\n";
    cout <<setw(10)<<right<<" "<<"\t"<<setw(15)<<left <<"SUM"<<setw(30)<<right<<sum<<endl;
    cout <<"---------------------------------------------------------\n";
    return;
}

int check_bill (vector<table_order> &note ,vector<menubook> book)
{
    double sum=0;
    for(unsigned int i=0 ;i<note.size();i++)
    {
        int j=find_id(book,note[i].food_id);
        sum+=note[i].order*book[j].price;
    }
    return sum;
}



void table_bill()
{
    int number ,count=0 ;
    vector<table_order> *vec_point; 
    vec_point = which_table(number);
    for(int j=0 ; j<9;j++){
        cout << table[j] << " ";
        if(table[j]!='-')
        {
            count++;
        }
    }
    cout << endl;
    if(count==9)
    {
        cout<<"All table are empty." << endl;
        return;
    }
    while(true)
    {
        cout << "input table want to check bill : ";
        cin >> number ;
        cout<<table[number-1]<<endl;
        if(table[number-1]!='-')
        {
           cout<<"Sorry this table are empty. Please select table again." << endl; 
        }
        else 
        {
            break;
        }
    }
    vec_point = which_table(number);
    show_bill(*vec_point,book);
    int money=check_bill(*vec_point,book);

    (*vec_point).clear();

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
                    basic_string<unsigned char> temp = sqlite3_column_text(stmt, i);
                    string text( temp.begin(), temp.end() );
                    book[book.size()-1].name=text;
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
                cout<<"Push Menubook Successfully!"<<endl;
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
    cout<<"---------------------------------------------------------\n\n";
    cout <<setw(10)<<right<<"Food ID"<<"\t"<<setw(15)<<left <<"Food Name"<<setw(10)<<right<<"Price"<<endl;
    for (unsigned int i = 0; i < book.size(); i++)
    {
        cout <<setw(10)<<right<<book[i].id;
        cout <<"\t"<<setw(15)<<left<<book[i].name; 
        cout <<setw(10)<<right<<book[i].price<<endl;

    }
    cout<<"---------------------------------------------------------\n";
}
//gcc sqlite3.c -c   ทำแบบนี้มันจะไม่ได้ exe แต่จะได้เป็น object file ที่ลิงค์เข้ากับโค้ดให้โปรแกรมเราไปเรียกใช้ตอน compile ครับ
//how to run (safe file first) ; g++ main.cpp sqlite3.o ; ./a.exe