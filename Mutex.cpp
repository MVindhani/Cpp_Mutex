#include <iostream>
#include <thread>
#include <unistd.h>
#include <mutex>

using namespace std;

int a = 1;
int b = 1;
int c = 1;
mutex myMutex;
recursive_mutex Recursive_myMutex;
timed_mutex Timed_myMutex;

void func(int id)
{
    cout <<"Thread called ID :"<< id << endl;
    {
        lock_guard<mutex> lock(myMutex);
        cout << "Enter Critical Section : a = " << a << endl;
        a++;
        cout << "Exit Critical Section : a = " << a << endl;
    }
    
    cout<<"Thread exit"<< endl;
}


void r_func(int id)
{
    cout << "Recursive thread called : "<< id << endl;
    {
        lock_guard<recursive_mutex> lock(Recursive_myMutex);
        cout << b << ": Recursive " << endl;
        b++;
        if(id > 0)
        {
            r_func(id - 1);
        }
    }
    
}

void t_func (int id)
{
    cout <<" Timed thread called : " << id << endl;
    {
        if(Timed_myMutex.try_lock_for(chrono::seconds(2)))
        {
            cout << c << ": Timed " << endl;
            c++;
            cout << c << ": Timed " << endl;
            sleep(3);
            Timed_myMutex.unlock();
        }
        else
        {
            cout << "Failed to get the lock for thread : "<< id << endl;
        }
    }
}

int main()
{
    thread thread1(func,1);
    thread thread2(func,2);
    thread thread3(r_func,3);
    thread thread4(t_func,4);
    
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();

    return 0;
}