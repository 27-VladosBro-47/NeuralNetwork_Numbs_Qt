#include "perceptronslayer.h"

PerceptronsLayer::PerceptronsLayer(int Perceptrons, int Weights)
{
    quantityOfPerceptrons = Perceptrons;
    quantityOfWeights = Weights;

    perceptronsArray = new Perceptron[Perceptrons];
    for (int i = 0; i < Perceptrons; i++)
    {
        perceptronsArray[i].weights = new double[Weights];
        perceptronsArray[i].d_weights = new double[Weights];
    }

    bias.x = bias.d_x = bias.z = 1.0;
    bias.weights = new double[Perceptrons];
    bias.d_weights = new double[Perceptrons];
}

// Отримання масиву персептронів, що належать одному слою
PerceptronsLayer::Perceptron* PerceptronsLayer::getPerceptronsArray()
{
    return perceptronsArray;
}

// Отримання персептрону здвига
PerceptronsLayer::Perceptron PerceptronsLayer::getBias()
{
    return bias;
}

// Отримання кількості персептронів у слої
int PerceptronsLayer::getQuantityOfPerceptrons()
{
    return quantityOfPerceptrons;
}

// Отримання кількості ваг, що виходять від кожного персептрона в слою
int PerceptronsLayer::getQuantityOfWeights()
{
    return quantityOfWeights;
}

PerceptronsLayer::~PerceptronsLayer()
{
    for (int i = 0; i < quantityOfPerceptrons; i++)
    {
        delete[] perceptronsArray[i].weights;;
        delete[] perceptronsArray[i].d_weights;;
    }
    delete[] perceptronsArray;

    delete bias.weights;
    delete bias.d_weights;
}
