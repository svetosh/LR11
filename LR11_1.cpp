#include <chrono>
#include <thread>
#include <iostream>

class MyTimer 
{
public:
	void Start();
	void End();
	float GetElapsedTime();

private:
	std::chrono::time_point<std::chrono::steady_clock> start, finish;
	std::chrono::duration<float> duration;
};

void MyTimer::Start() 
{
	start = std::chrono::high_resolution_clock::now();
}

void MyTimer::End() 
{
	finish = std::chrono::high_resolution_clock::now();
	duration = finish - start;
	float result = duration.count();
	std::cout << "Прошло время: " << result << " секунд " << std::endl;
}

float MyTimer::GetElapsedTime() 
{
	finish = std::chrono::high_resolution_clock::now();
	duration = finish - start;
	float result = duration.count();
	return result;
}

#include <iostream>
#include<thread>
#include<mutex>
#include <vector>
#include<ctime>

using namespace std;
mutex mtx;
void findMin(vector<int>& vect, int begin, int end, float& threadTime, int& min, MyTimer* timer) 
{
	timer->Start();
	min = vect[begin];
	for (; begin < end; begin++)
		if (min > vect[begin]) 
		{
			min = vect[begin];
		}
	threadTime += timer->GetElapsedTime();

	mtx.lock();
	cout << "Индификатор: " << this_thread::get_id() << " Локальный минимальный элемент: " << min << endl;
	mtx.unlock();
}

int main() {
	srand(time(nullptr));
	setlocale(0, "ru");
	int N, numberOfThread, min;
	vector<int> vect, minVect;
	MyTimer* timer = new MyTimer();
	float elapsedTime = 0;
	cout << "Введите количество элементов: "; cin >> N;
	if (N <= 0) 
	{
		cout << "Ошибка" << endl;
		return 1;
	}
	vect.resize(N);
	for (int i = 0; i < N; i++)
		vect[i] = rand() % N - N / 2;
	cout << "Введите количество потоков: "; cin >> numberOfThread;

	if (numberOfThread <= 0) 
	{
		findMin(vect, 0, N, elapsedTime, min, timer);
		cout << "Минимальный элемент: " << min << endl;
		system("pause");
		return 0;
	}
	minVect.resize(numberOfThread);
	thread* arrThread = new thread[numberOfThread];
	int sizeOfWork = N / numberOfThread;
	for (int i = 0; i < numberOfThread; i++) 
	{	
		int beg = i * sizeOfWork;
		int end;
		if (i == numberOfThread - 1)
			end = N;
		else
			end = (i + 1) * sizeOfWork;
		arrThread[i] = thread(findMin, std::ref(vect), beg, end, std::ref(elapsedTime),
			std::ref(minVect[i]), timer);
	}
	for (int i = 0; i < numberOfThread; i++)
		arrThread[i].join();
	cout << endl << endl << "                  ----   Потоки закончили свою работу   ----          " << endl << endl;
	findMin(minVect, 0, minVect.size(), elapsedTime, min, timer);
	cout << "Минимальный элемент: " << min << endl;
	cout << "Прошло время для асинхронной работы: " << fixed << elapsedTime << endl;

	elapsedTime = 0;
	cout << endl << endl;
	findMin(vect, 0, N, elapsedTime, min, timer);
	cout << "Минимальный элемент среди экстремумов: " << min << endl;
	cout << "Прошло время: " << fixed << elapsedTime << " секунд " << std::endl;
	delete[] arrThread;
	delete timer;
	system("pause");

	return 0;
}
