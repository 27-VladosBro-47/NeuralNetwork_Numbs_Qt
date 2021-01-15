#ifndef PERCEPTRONS_LAYER
#define PERCEPTRONS_LAYER

class PerceptronsLayer
{
private:
    struct Perceptron
    {
        double x = 0.0;
        double d_x = 0.0;
        double *weights;
        double *d_weights;
        double z;

    };

    Perceptron *perceptronsArray;
    Perceptron bias;

    int quantityOfPerceptrons;
    int quantityOfWeights;

public:
    PerceptronsLayer(int Perceptrons, int Weights);

    // Отримання масиву персептронів, що належать одному слою
    Perceptron* getPerceptronsArray();
    // Отримання персептрону здвига
    Perceptron getBias();
    // Отримання кількості персептронів у слої
    int getQuantityOfPerceptrons();
    // Отримання кількості ваг, що виходять від кожного персептрона в слою
    int getQuantityOfWeights();

    ~PerceptronsLayer();

};

#endif // !PERCEPTRONS_LAYER
