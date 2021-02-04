#include <chrono>
#include <thread>
#include <iostream>

class MyTimer{
public:
	void Start();
	void End();
	float GetElapsedTime();

private:
	std::chrono::time_point<std::chrono::steady_clock> start, finish;
	std::chrono::duration<float> duration;
};

void MyTimer::Start() {
	start = std::chrono::high_resolution_clock::now();
}

void MyTimer::End() {
	finish = std::chrono::high_resolution_clock::now();
	duration = finish - start;
	float result = duration.count();
	std::cout << "Прошло время: " << result << " секунд " << std::endl;
}

float MyTimer::GetElapsedTime() {
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

void findMinInPart(const vector<int> &vect, const int &begin, const int &end, int &min) {
	min = vect[begin];
	for (int i = begin + 1; i < end; ++i) {
		if (min > vect[i]) {
			min = vect[i];
		}
	}

	mtx.lock();
	cout << "Индификатор: " << this_thread::get_id() << " Локальный минимальный элемент: " << min << endl;
	mtx.unlock();
}
int findMinAsync(vector<int> &vect, int threadsNum)
{
	int *minElements = new int[threadsNum];
	thread *arrThread = new thread[threadsNum];
	int partSize = vect.size() / threadsNum;

	for (int i = 0; i < threadsNum; ++i) {
		arrThread[i] = thread(findMinInPart, std::ref(vect), partSize * i, partSize * (i + 1), std::ref(minElements[i]));
	}

	for (int i = 0; i < threadsNum; ++i)
		arrThread[i].join();

	int minElement = minElements[0];
	for (int i = 1; i < threadsNum; ++i) {
		if (minElement > minElements[i]) {
			minElement = minElements[i];
		}
	}

	mtx.lock();
	cout << "Индификатор: " << this_thread::get_id() << " Локальный минимальный элемент: " << minElement << endl;
	mtx.unlock();

	return minElement;
}

int findMinSync(const vector<int> &vect, const int begin, const int end) {
	int min = vect[begin];
	for (int i = begin + 1; i < end; ++i) {
		if (min > vect[i]) {
			min = vect[i];
		}
	}

	return min;
}

int main() {
	srand(time(NULL));
	setlocale(0, "");
	int N, numberOfThreads, min;
	vector<int> vect, minVect;
	MyTimer* timer = new MyTimer();
	float elapsedTime = 0.0;
	cout << "Введите количество элементов: "; cin >> N;
	if (N <= 0) {
		cout << "Ошибка" << endl;
		return 1;
	}
	vect.resize(N);
	for (int i = 0; i < N; i++)
		vect[i] = rand() % N - N / 2;
	for (int i = 0; i < N; i++)
		cout << vect[i] << " ";
	cout << endl;

	cout << "Введите количество потоков: "; cin >> numberOfThreads;

	timer->Start();
	min = findMinSync(vect, 0, N);
	elapsedTime = timer->GetElapsedTime();

	cout << endl << "Минимальный элемент(синхр): " << min << endl;
	cout << "Прошло время для синхронной работы: " << fixed << elapsedTime << endl << endl;

	timer->Start();
	min = findMinAsync(vect, numberOfThreads);
	elapsedTime = timer->GetElapsedTime();

	cout << endl << "Минимальный элемент (асинхр): " << min << endl;
	cout << "Прошло время для асинхронной работы: " << fixed << elapsedTime << endl << endl;


	delete timer;
	system("pause");

	return 0;
}

