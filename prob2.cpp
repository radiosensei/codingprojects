#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

class Person     //base class of Person
{
protected:
	string name;
	string phone;
	string email;

public:
	Person(string name, string phone, string email)
		:name(name), phone(phone), email(email){ }  //Constructor

	virtual string ToString() const    //A virtual string called ToString is created and manually assigned parameters
	{
		return "Person named " + name + "has a phone number " + phone + "and their email is " + email;
	}
	virtual ~Person() = default;    //Virtual destructor that sets Person() back to default values
};

class Student :public Person  //Derived Student class from base class (Person)
{
private:
	string year;

public:
	Student(string name, string phone, string email, string year)    //Constructor for derived class
		:Person(name, phone, email), year(year){ }

	string ToString() const override
	{
		return Person::ToString() + "and is in their" + year + " year";
		
	}
};

class Employee : public Person
{
protected:
	double salary;

public:
	Employee(string name, string phone, string email, double salary)
		:Person(name, phone, email), salary(salary){ }

	string ToString()const override
	{
		stringstream ss;
		ss << fixed << setprecision(2);
		ss << Person::ToString() << " and earns " << salary << " annually";
		return ss.str();
	}
};

class Faculty :public Employee
{
private:
	string title;

public:
	Faculty(string name, string phone, string email, double salary, string title)
		:Employee(name, phone, email, salary), title(title){ }

	string ToString() const override
	{
		return Employee::ToString() + " and holds the title of " + title;
	}
};

class Staff : public Employee
{
private:
	string role;

public:
	Staff(string name, string phone, string email, double salary, string role)
		:Employee(name, phone, email,salary), role(role){ }

	string ToString() const override
	{
		return Employee::ToString() + " and works as " + role;
	}
};

int main()
{
	vector< shared_ptr <Person>> people;


	people.push_back(make_shared<Person>("Tiffany", "243-556-4321", "Tif2@gmail.com"));

	people.push_back(make_shared<Student>("Ryan ", "656-783-8736 ", "tdog5@tttu.com ", " freshman"));

	people.push_back(make_shared<Employee>("John ", "801-890-1273 ", " Jon@giggle.com ",  55777));

	people.push_back(make_shared<Faculty>("Sam ", "332-435-9845 ", "runs9@utu.edu ", 87632 , "Dean"));

	people.push_back(make_shared<Staff>("Jill ", "562-895-7412 ", "J12@bigschool.edu", 52864 , "Assistant"));

	for(const auto& Person:people)
	{
		cout << Person->ToString() << endl<<endl;
	}

}