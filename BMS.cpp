#include<iostream>
#include<fstream>
#include<map>
#include<iomanip>
using namespace std;
#define MIN_BALANCE 1000

class AccountNotFound{};

class Account{
	private:
		long accountNumber;
		string firstName;
		string lastName;
		int age;
		string address;
		float balance;
		static long nextAccountNumber;
	public:
		Account(){}
		Account(string fname,string lname,float balance,int age,string address){
			nextAccountNumber++;
			accountNumber=nextAccountNumber;
			firstName=fname;
			lastName=lname;
			this->balance=balance;
			this->age=age;
			this->address=address;
		}
		long getAccNo(){
			return accountNumber;
		}
		string getFirstName(){
			return firstName;
		}
		string getLastName(){
			return lastName;
		}
		float getBalance(){
			return balance;
		}
		int getAge(){
			return age;
		}
		string getAddress(){
			return address;
		}
		
		void Deposit(float amount){
			balance+=amount;
		}
		void Withdraw(float amount){
			if(balance-amount<MIN_BALANCE){
				cout<<endl<<"Amount cannot be withdrawn"<<endl;
			}else{
			    cout<<endl<<"Amount is Withdrawn"<<endl;
				balance-=amount;
			}
		}
		static void setLastAccountNumber(long accountNumber){
			nextAccountNumber=accountNumber;
		}
		static long getLastAccountNumber(){
			return nextAccountNumber;
		}
		friend ofstream & operator<<(ofstream &ofs,Account &acc);
		friend ifstream & operator>>(ifstream &ifs,Account &acc);
		friend ostream & operator<<(ostream &os,Account &acc);
};

long Account::nextAccountNumber=0;

ofstream & operator<<(ofstream &ofs,Account &acc){
	ofs<<acc.accountNumber<<endl;
	ofs<<acc.firstName<<endl;
	ofs<<acc.lastName<<endl;
	ofs<<acc.balance<<endl;
	ofs<<acc.age<<endl;
	ofs<<acc.address<<endl;
	return ofs;
}
ifstream & operator>>(ifstream &ifs,Account &acc){
	ifs>>acc.accountNumber;
	ifs>>acc.firstName;
	ifs>>acc.lastName;
	ifs>>acc.balance;
	ifs>>acc.age;
	ifs>>acc.address;
	return ifs;
}
ostream & operator<<(ostream &os,Account &acc){
	os<<"First Name: "<<acc.getFirstName()<<endl;
	os<<"Last Name: "<<acc.getLastName()<<endl;
	os<<"Account Number: "<<acc.getAccNo()<<endl;
	os<<"Balance: "<<acc.getBalance()<<endl;
	os<<"Age: "<<acc.getAge()<<endl;
	os<<"Address: "<<acc.getAddress()<<endl;
	return os;
}

class Bank{
	private:
		map<long,Account> accounts;
	public:
		Bank(){
			Account account;
			ifstream infile;
			infile.open("Bank.data");
			if(!infile){
				return;
			}
			while(!infile.eof()){
				infile>>account;
				accounts.insert(pair<long,Account>(account.getAccNo(),account));
			}
			Account::setLastAccountNumber(account.getAccNo());
			infile.close();
		}
		
		Account OpenAccount(string fname,string lname,float balance,int age,string address){
			ofstream outfile;
			Account account(fname,lname,balance,age,address);
			accounts.insert(pair<long,Account>(account.getAccNo(),account));
			outfile.open("Bank.data",ios::trunc);
			map<long,Account>::iterator itr;
			for(itr=accounts.begin();itr!=accounts.end();itr++){
				outfile<<itr->second;
			}
			outfile.close();
			return account;
		}
		
		Account BalanceEnquiry(long accountNumber){
			map<long,Account>::iterator itr=accounts.find(accountNumber);
			if(accounts.find(accountNumber)!=accounts.end()){
			    return itr->second;
			}else{
			    throw AccountNotFound();
			}
		}
		
		Account Deposit(long accountNumber,float amount){
			map<long,Account>::iterator itr=accounts.find(accountNumber);
			if(accounts.find(accountNumber)!=accounts.end()){
			    itr->second.Deposit(amount);
			    return itr->second;
			}else{
			    throw AccountNotFound();
			}
		}
		
		Account Withdraw(long accountNumber,float amount){
			map<long,Account>::iterator itr=accounts.find(accountNumber);
			if(accounts.find(accountNumber)!=accounts.end()){
			    itr->second.Withdraw(amount);
			    return itr->second;
			}else{
			    throw AccountNotFound();
			}
		}
		
		Account accountDetails(long accountNumber){
			map<long,Account>::iterator itr=accounts.find(accountNumber);
			if(accounts.find(accountNumber)!=accounts.end()){
			    return itr->second;
			}else{
			    throw AccountNotFound();
			}
		}
		
		void closeAccount(long accountNumber){
			map<long,Account>::iterator itr=accounts.find(accountNumber);
			if(accounts.find(accountNumber)!=accounts.end()){
				accounts.erase(accountNumber);
			}else{
				throw AccountNotFound();
			}
		}
		
		void showAllAccounts(){
			map<long,Account>::iterator itr;
			for(itr=accounts.begin();itr!=accounts.end();itr++){
				cout<<"Account no: "<<itr->first<<endl<<itr->second<<endl;
			}
		}
		
		~Bank(){
			ofstream outfile;
			outfile.open("Bank.data", ios::trunc);
			
			map<long,Account>::iterator itr;
			for(itr=accounts.begin();itr!=accounts.end();itr++){
				outfile<<itr->second;
			}
			outfile.close();
		}
};

main(){
	long accountNumber;
	float amount,balance;
	string fname,lname;
	int age;
	string address;
	
	Bank b;
	Account acc;
	int choice;
	
	cout<<setw(70)<<"***BANKING MANAGEMENT SYSTEM***"<<endl<<endl;
	do{
		cout<<setw(60)<<"SELECT ONE OPTION"<<endl;
		cout<<setw(50)<<"1.OPEN AN ACCOUNT"<<setw(24)<<"2.BALANCE ENQUIRY"<<endl; 
		cout<<setw(42)<<"3.DEPOSIT"<<setw(27)<<"4.WITHDRAWAL"<<endl;
		cout<<setw(50)<<"5.ACCOUNT DETAILS"<<setw(25)<<"6.CLOSE AN ACCOUNT"<<endl;
		cout<<setw(52)<<"7.SHOW ALL ACCOUNTS"<<setw(11)<<"8.EXIT"<<endl;
		cout<<"Enter your choice"<<endl;
		cin>>choice;
		switch(choice){
			case 1:
				cout<<"Enter First Name: ";
				cin>>fname;
				cout<<"Enter Last Name: ";
				cin>>lname;
				cout<<"Enter your Age: ";
				cin>>age;
				if(age<18){
				    do{
				        cout<<"Minimum Age should be 18"<<endl;
				        cout<<"Enter age: ";
				        cin>>age;
				    }while(age<18);
				}
				cout<<"Enter your City of residence: ";
				cin>>address;
				cout<<"Enter intial balance: ";
				cin>>balance;
				if(balance<MIN_BALANCE){
				    do{
				        cout<<"Enter a minimum initial balance of 1000 Rs"<<endl;
				        cout<<"Enter balance: ";
				        cin>>balance;
				    }while(balance<MIN_BALANCE);
				}
				acc=b.OpenAccount(fname,lname,balance,age,address);
				cout<<endl<<"Congratulations Account is Created"<<endl;
				cout<<acc;
				break;
			case 2:
				cout<<"Enter Account Number: ";
				cin>>accountNumber;
				acc=b.BalanceEnquiry(accountNumber);
				cout<<endl<<"Your Balance is: "<<acc.getBalance()<<endl;
				break;
			case 3:
				cout<<"Enter Account Number: ";
				cin>>accountNumber;
				cout<<"Enter Balance: ";
				cin>>amount;
				acc=b.Deposit(accountNumber,amount);
				cout<<endl<<"Amount is Deposited"<<endl;
				cout<<acc;
				break;
			case 4:
				cout<<"Enter Account Number: ";
				cin>>accountNumber;
				cout<<"Enter Balance: ";
				cin>>amount;
				acc=b.Withdraw(accountNumber,amount);
				cout<<acc;
				break;
			case 5:
				cout<<"Enter Account Number: ";
				cin>>accountNumber;
				acc=b.accountDetails(accountNumber);
				cout<<acc;
				break;
			case 7:
				b.showAllAccounts();
				break;
			case 6:
				cout<<"Enter Account Number: ";
				cin>>accountNumber;
				b.closeAccount(accountNumber);
				cout<<"Account is Closed"<<endl;
				break;
			case 8:
			    cout<<"Banking Management System is Closed";
			    break;
			default:
				cout<<"Enter correct choice"<<endl;
		}
	}while(choice!=8);
}
