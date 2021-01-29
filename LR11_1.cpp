#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <ctime>

using namespace std;

class MyTimer {
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

mutex mtx;
void findMin(vector<int> vect, int begin, int end, int& min) 
{
	min = vect[begin];
	for (; begin < end; begin++)
		if (min > vect[begin])
		{
			min = vect[begin];
		}
		mtx.lock();
		cout << "Индификатор: " << this_thread::get_id() << " Локальный минимальный элемент: " << min << endl;
		mtx.unlock();
}


int main() {
	srand(time(NULL));
	setlocale(0, "");
	int N, numberOfThread, min;
	vector<int> vect, minVect;
	MyTimer* timer = new MyTimer();

	cout << "Введите количество элементов: ";
	cin >> N;
	if (N <= 0) //обработка искл. ситуац.
	{
		cout << "Ошибка" << endl;
		return 1;
	}
	vect.resize(N);
	for (int i = 0; i < N; i++)
	{
		vect[i] = rand() % N - N / 2;
	}
	cout << "Введите количество потоков: ";
	cin >> numberOfThread;

	if (numberOfThread <= 0) 
	{
		findMin(vect, 0, N, min);
		cout << "Минимальный элемент: " << min << endl;
		system("pause");
		return 0;
	}

	//работа с потоками
	minVect.resize(numberOfThread);
	thread* arrThread = new thread[numberOfThread];
	int sizeOfWork = N / numberOfThread;
	float elapsedTime = 0;
	for (int i = 0; i < numberOfThread; i++) 
	{	
		int beg = i * sizeOfWork;
		int end;
		if (i == numberOfThread - 1)
		{
			end = N;
		}
		else
		{
			end = (i + 1) * sizeOfWork;
		}
		timer->Start();
		arrThread[i] = thread(findMin, vect, beg, end, std::ref(minVect[i]));
		elapsedTime = timer->GetElapsedTime() + elapsedTime;
	}
	for (int i = 0; i < numberOfThread; i++)
	{
		arrThread[i].join();
	}
	cout << endl << endl << "                  ----   Потоки закончили свою работу   ----          " << endl << endl;

	timer->Start();
	findMin(minVect, 0, minVect.size(), min);
	cout << "Минимальный элемент: " << min << endl;
	elapsedTime = timer->GetElapsedTime() + elapsedTime;
	cout << "Прошло время для асинхронной работы: " << elapsedTime << endl;
	cout << endl << endl;
	timer->Start();
	findMin(vect, 0, N, min);
	cout << "Минимальный элемент среди экстремумов: " << min << endl;
	timer->End();

	//освобождение памяти:
	delete[] arrThread;
	delete timer;
	return 0;
}
