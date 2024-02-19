#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

using namespace std;

class Bank
{
private:
    int total;
    int minimumBalance;
    mutex mtx;

public:
    Bank() : total(100), minimumBalance(30) {}

    void withdraw(string name, int withdrawal)
    {
        mtx.lock();
        if (total - withdrawal >= minimumBalance)
        {
            cout << name << " withdrawn " << withdrawal << endl;
            total -= withdrawal;
            cout << "Balance after withdrawal: " << total << endl;
        }
        else
        {
            cout << name << " you cannot withdraw " << withdrawal << endl;
            cout << "Your balance is: " << total << endl;
        }
        mtx.unlock();
        this_thread::sleep_for(chrono::seconds(1));
    }

    void deposit(string name, int deposit)
    {
        mtx.lock();
        cout << name << " deposited " << deposit << endl;
        total += deposit;
        cout << "Balance after deposit: " << total << endl;
        mtx.unlock();
        this_thread::sleep_for(chrono::seconds(1));
    }

    void transfer(string from, string to, int amount)
    {
        mtx.lock();
        if (total - amount >= minimumBalance)
        {
            cout << "Transferring " << amount << " from " << from << " to " << to << endl;
            withdraw(from, amount);
            deposit(to, amount);
        }
        else
        {
            cout << "Insufficient funds for transfer" << endl;
        }
        mtx.unlock();
    }

    int getBalance()
    {
        mtx.lock();
        int balance = total;
        mtx.unlock();
        return balance;
    }
};

int main()
{
    Bank obj;
    vector<thread> threads;

    threads.emplace_back([&obj]()
                         { obj.withdraw("Arnab", 20); });
    threads.emplace_back([&obj]()
                         { obj.deposit("Mukta", 35); });
    threads.emplace_back([&obj]()
                         { obj.withdraw("Rinkel", 80); });
    threads.emplace_back([&obj]()
                         { obj.deposit("Monodwip", 50); }); // Depositing to ensure balance for transfer
    threads.emplace_back([&obj]()
                         { obj.transfer("Arnab", "Monodwip", 10); }); // Transfer between Arnab and Monodwip
    threads.emplace_back([&obj]()
                         { obj.withdraw("Shubham", 40); });
    threads.emplace_back([&obj]()
                         { obj.deposit("Monodwip", 20); }); // Additional deposit to Monodwip's account
    threads.emplace_back([&obj]()
                         { obj.transfer("Monodwip", "Mukta", 15); }); // Transfer from Monodwip to Mukta
    threads.emplace_back([&obj]()
                         { obj.transfer("Shubham", "Rinkel", 25); }); // Transfer from Shubham to Rinkel
    threads.emplace_back([&obj]()
                         { obj.deposit("Shubham", 100); }); // Additional deposit to Shubham's account

    for (auto &t : threads)
    {
        t.join();
    }

    cout << "Total balance of the bank: " << obj.getBalance() << endl;

    return 0;
}
