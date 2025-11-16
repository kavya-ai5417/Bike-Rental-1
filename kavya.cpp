#include <iostream>
#include <fstream>
using namespace std;

struct Customer {
    string name;
    int age;
};

inline void line() { cout << "---------------------------------------\n"; }

class Vehicle {
public:
    virtual void show() = 0;
};

class Engine : virtual public Vehicle {
public:
    void show() override { cout << "Engine Component\n"; }
};

class Body : virtual public Vehicle {
public:
    void show() override { cout << "Body Component\n"; }
};

class Bike : public Engine, public Body {
protected:
    string model;
    float hourlyRate;

public:
    const int wheels = 2;
    static int bikeCount;

    Bike(string m = "Unknown", float r = 0.0)
        : model(m), hourlyRate(r) {
        bikeCount++;
    }

    virtual void display() {
        cout << model << " | Rate/hr: " << hourlyRate << " Rs\n";
    }

    void show() override { cout << "Bike Vehicle\n"; }

    virtual ~Bike() {}
};

int Bike::bikeCount = 0;

class SportsBike : public Bike {
    int topSpeed;
public:
    SportsBike(string m, float r, int s)
        : Bike(m, r), topSpeed(s) {}

    void display() override {
        cout << model << " | Rate/hr: " << hourlyRate
             << " Rs | Top Speed: " << topSpeed << " km/h\n";
    }
};

class PremiumBike : public SportsBike {
public:
    PremiumBike(string m, float r, int s)
        : SportsBike(m, r, s) {}

    void display() override { SportsBike::display(); }
};

class Rent {
public:
    int hours;
    Rent(int h = 0) : hours(h) {}
    Rent operator+(Rent &r) { return Rent(hours + r.hours); }
};

class Bill {
    float amount;
public:
    Bill(float a = 0) : amount(a) {}
    friend void showBill(const Bill &);
    friend class Manager;
};

void showBill(const Bill &b) { cout << "Total Bill: " << b.amount << " Rs\n"; }

class Manager {
public:
    void modifyBill(Bill &b, float newAmount) { b.amount = newAmount; }
};

float calculate(float rate, int hrs) { return rate * hrs; }
float calculate(float rate, int hrs, float disc) { return (rate * hrs) - disc; }

template <typename T>
T maxValue(T a, T b) { return (a > b ? a : b); }

void saveRecord(string name, float amount) {
    ofstream f("bike_records.txt", ios::app);
    f << "Customer: " << name << " | Bill: " << amount << endl;
    f.close();
}

void readRecords() {
    ifstream f("bike_records.txt");
    string line;
    while (getline(f, line)) cout << line << endl;
    f.close();
}

int main() {
    try {
        line();
        cout << "           BIKE RENTAL SYSTEM\n";
        line();

        Bike* bikes[6];
        bikes[0] = new Bike("Splendor", 70);
        bikes[1] = new Bike("Pulsar", 80);
        bikes[2] = new Bike("Royal Enfield", 150);
        bikes[3] = new Bike("Apache", 80);
        bikes[4] = new SportsBike("Yamaha MT15", 180, 130);
        bikes[5] = new PremiumBike("Yamaha R15", 200, 150);

        cout << "\nAvailable Bikes:\n";
        for (int i = 0; i < 6; i++)
            bikes[i]->display();

        Customer c;
        cout << "\nEnter Name: ";
        cin >> c.name;
        cout << "Enter Age: ";
        cin >> c.age;

        int select;
        cout << "\nSelect bike number (0–5): ";
        cin >> select;

        if (select < 0 || select > 5)
            throw runtime_error("Invalid bike option!");

        int hrs;
        cout << "Enter hours to rent: ";
        cin >> hrs;

        if (hrs <= 0)
            throw runtime_error("Invalid hours!");

        Rent r1(hrs), r2(1);
        Rent total = r1 + r2;

        float billAmount = calculate( bikes[select]->wheels * 25, total.hours );

        Bill bill(billAmount);
        showBill(bill);

        saveRecord(c.name, billAmount);

        cout << "\nSaved Records:\n";
        readRecords();

        for (int i = 0; i < 6; i++)
            delete bikes[i];

        line();
    }
    catch (exception &e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
