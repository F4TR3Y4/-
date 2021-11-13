#include <iostream>
#include <thread>
#include <vector>
#include <cmath>

int main()
{
	int n;
	unsigned int available_threads = std::thread::hardware_concurrency();

	std::cout << "Enter the number of rows / columns for the matrix\n";
	std::cin >> n;

	std::cout << available_threads << " available streams.\n";
	std::cout << "How many threads to use?\n";
	std::cin >> available_threads;

	int** Alfa = new int* [n];
	int** Beta = new int* [n];
	int** Gamma = new int* [n];

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			Alfa[i] = new int[n];
			Beta[i] = new int[n];
			Gamma[i] = new int[n];
		}
	}

	// fill 
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			Alfa[i][j] = rand() % 10;
			Beta[i][j] = rand() % 10;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << Alfa[i][j] << " ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << Beta[i][j] << " ";
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;

	std::vector<std::thread> thread_pool(available_threads);
	auto start_time = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < available_threads; i++)
	{
		thread_pool[i] = std::thread([](int** a, int** b, int** c, int n, int available_threads, int thread_number) {

			int N = (n / available_threads) * (thread_number + 1);

			if (thread_number == available_threads - 1) N += n % available_threads;

			for (int i = (n / available_threads) * thread_number; i < N; i++)
			{
				for (int j = 0; j < n; j++)
				{
					for (int k = 0; k < n; k++)
					{
						c[i][j] += a[i][k] * b[k][j];
					}
				}

			}

			}, Alfa, Beta, Gamma, n, available_threads, i);
	}

	std::cout << std::endl;

	for (int i = 0; i < available_threads; i++)
	{
		thread_pool[i].join();
		std::cout << "stream" << i << " finished work" << std::endl;
	}

	auto stop_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
	std::cout << "\nSpent time: " << duration.count() << " microseconds" << std::endl;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << Gamma[i][j] << " ";
		}
		std::cout << std::endl;
	}

	return 0;
}
