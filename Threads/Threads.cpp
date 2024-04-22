#include <iostream>
#include <thread>
using namespace std;
using namespace std::chrono_literals;

bool finish = false;

void Plus() {
    while (!finish) {
        cout << "+ ";
        std::this_thread::sleep_for(1s);
    }
}
void Minus() {
    while (!finish) {
        cout << "- ";
        std::this_thread::sleep_for(1s);
    }
}

int main()
{
    thread plus_thread(Plus);    
    thread minus_thread(Minus);  

    cin.get();
    finish = true;

    if (minus_thread.joinable())minus_thread.join();
    if (plus_thread.joinable())plus_thread.join();
    
}