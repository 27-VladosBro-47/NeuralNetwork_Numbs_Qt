#ifndef NEURAL_NETWORK
#define NEURAL_NETWORK

#include <cstdarg>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include "json-develop/single_include/nlohmann/json.hpp"
#include "perceptronslayer.h"

#include <iostream>
#include <QDebug>

class NeuralNetwork
{
private:
    // Зберігає кількість слоїв у нейромережі
    int countLayers;
    // Масив вказівників на слої нейромережі
    PerceptronsLayer **layers;

public:

    double arr[10];

    // Конструктор нейромережі
    // Перший параметр - кількість слоїв персептронів, що передаються у нейромережу
    // Наступні параметри - вказівники на об*єкти класа PerseptronsLayer (Клас слою нейромережі)
    NeuralNetwork();

    void createNeuralNetwork(std::vector<PerceptronsLayer*> &perceptronsLayerVector);

    void deleteNeuralNetwork();

    // Функція, що зберігає дані про колір пікселей картинки
    void setInputPerceptrons(double **pixelsArray, int Width, int Height);

    // Функція виставляє початкові значення для ваг персептронів
    void setPrimaryWeights();

    // Функція виставляє початкові значення для значень персептронів
    void setPrimaryPerseptrons();

    // Сигмоїда
    double sigm(double x);

    // Похідна сигмоїди
    double d_sigm(double x);

    // Метод обраховує персептрони наступного слою   ---   Застосовані потоки
    // begin i end - вказують діапазон персептронів, які необхідно обрахувати
    // layer - номер слою, у якому обраховуються персептрони
    void calculatePerceptrons(int begin, int end, int layer);

    // Метод обраховує значення нейромережі
    void calculateNeuralNetwork();

    // Функція виставляє в останьому слої нейромережі в змінних d_x "правильні значення" персептронів
    // Потім відбувається обрахування d_x = x - d_x
    void setTrueAnswer(int numbPerceptron);

    // --- з.р.п. - Зворотнє розповсюдження помилки ---

    // Обчислення помилки для здвигу за методом з.р.п.
    void backPropagationBias(int layer);

    // Обчислення помилки для ваг за методом з.р.п.
    void backPropagationWeights(int layer);

    // Обчислення помилки для персептронів за методом з.р.п.
    void backPropagationPerceptrons(int layer);

    // Реалізація методу з.р.п.
    void backPropagation();

    void weightsDifference();

    // Функція визначає рівень навчиності нейромережі
    // Чим значення double, що повертає функція, ближче до нуля, тим більше навчина нейромережа
    double levelOfEducation();

    bool saveWeights(QString path);

    bool loadWeights(QString path, int numbOfLayer);




    void checkWeights(int numbOfLayer);
    void showInputPerseptrons();




    ~NeuralNetwork();
};

#endif // !NEURAL_NETWORK
