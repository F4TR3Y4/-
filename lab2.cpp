#include <iostream>
#include <thread>
#include <mutex>
#include <ctime>
#include <string>
#include <chrono>

using namespace std;

mutex mtx;

int main()
{
    int balls_in_storage = 10000;
    int balls_in_basket = 0;

    thread seller[2];
    thread customers[3];

    for (int i = 0; i < 2; i++) {
        seller[i] = thread([&balls_in_storage, &balls_in_basket](int i) {

            int sum = 0;

            while (balls_in_storage != 0) {
                int n = rand() % 101;

                mtx.lock();

                if (n > balls_in_storage) {
                    n = balls_in_storage;
                }

                balls_in_storage -= n;
                balls_in_basket += n;
                sum += n;

                mtx.unlock();

                this_thread::sleep_for(1us);
            }

            mtx.lock();
            cout << "the seller  " << i << " took " << sum << std::endl;
            mtx.unlock();

            }, i);

    }

    for (int i = 0; i < 3; i++) {
        customers[i] = thread([&balls_in_storage, &balls_in_basket](int i) {
            int sum = 0;

            while (balls_in_storage + balls_in_basket != 0) {

                mtx.lock();

                if (balls_in_basket > 0) {
                    sum++;
                    balls_in_basket--;
                }

                mtx.unlock();

                this_thread::sleep_for(1us);
            }

            mtx.lock();
            cout << "the buyer took " << i << " took " << sum << std::endl;
            mtx.unlock();

            }, i);
    }
    for (int i = 0; i < 2; i++)
    {
        seller[i].join();
    }
    for (int i = 0; i < 3; i++)
    {
        customers[i].join();
    }
}

