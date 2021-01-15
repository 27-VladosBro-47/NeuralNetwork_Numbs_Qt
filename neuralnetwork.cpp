#include "neuralnetwork.h"
#include <QDebug>

// Конструктор нейромережі
// Перший параметр - кількість слоїв персептронів, що передаються у нейромережу
// Наступні параметри - вказівники на об*єкти класа PerseptronsLayer (Клас слою нейромережі)
NeuralNetwork::NeuralNetwork()
{
}

void NeuralNetwork::createNeuralNetwork(std::vector<PerceptronsLayer*> &perceptronsLayerVector)
{
    countLayers = perceptronsLayerVector.size();
    layers = new PerceptronsLayer*[countLayers];
    for (int i = 0; i < countLayers; ++i)
    {
        layers[i] = perceptronsLayerVector[i];
    }
}

void NeuralNetwork::deleteNeuralNetwork()
{
    delete[] layers;
}

// Функція, що зберігає дані про колір пікселей картинки
void NeuralNetwork::setInputPerceptrons(double **pixelsArray, int Width, int Height)
{
    int numb = 0;
    //if (layers[0]->getQuantityOfPerceptrons() == Width * Height)
    //{
        for (int i = 0; i < Height; ++i)
        {
            for (int j = 0; j < Width; ++j)
            {
                layers[0]->getPerceptronsArray()[numb].x = pixelsArray[i][j];

                //if(pixelsArray[i][j] >= 0.0 && pixelsArray[i][j] <= 0.05)
                //    layers[0]->getPerceptronsArray()[numb].x = 0.03;
                //else
                //    layers[0]->getPerceptronsArray()[numb].x = pixelsArray[i][j];
                ++numb;
            }

        }
    //}
    //else
    //{
    //    qDebug() << "Something wrong\nNeuralNetwork::setInputPerceptrons";
    //}
}

// Функція виставляє початкові значення для ваг персептронів
void NeuralNetwork::setPrimaryWeights()
{
    srand(time(NULL));
    rand();

    for (int layer = 0; layer < countLayers; layer++)
    {
        for (int perceptron = 0; perceptron < layers[layer]->getQuantityOfPerceptrons(); perceptron++)
        {
            for (int weight = 0; weight < layers[layer]->getQuantityOfWeights(); weight++)
            {
                //layers[layer]->getPerceptronsArray()[perseptron].weights[weight] = (rand() % 11) - 5 + 0.05;
                layers[layer]->getPerceptronsArray()[perceptron].weights[weight] = (double)(rand()) / RAND_MAX * (1.0 - 0.01) + 0.01 - 0.5;
                //layers[layer]->getPerceptronsArray()[perseptron].weights[weight] = 1.0;
                layers[layer]->getPerceptronsArray()[perceptron].d_weights[weight] = 0.0;
                //qDebug() << "Layer " << layer  <<  " perceptron[" << perseptron << "] " << "weight[" << weight << "] = " << layers[layer]->getPerceptronsArray()[perseptron].weights[weight];
            }

            //layers[layer]->getBias().weights[perseptron] = (rand() % 11) - 5 + 0.05;
            layers[layer]->getBias().weights[perceptron] = (double)(rand()) / RAND_MAX * (1.0 - 0.01) + 0.01 - 0.5;
            //qDebug() << "Layer " << layer  <<  " bias[" << perseptron << "] = " << layers[layer]->getBias().weights[perseptron];
        }

    }
}

// Функція виставляє початкові значення для значень персептронів
void NeuralNetwork::setPrimaryPerseptrons()
{
    srand(time(NULL));
    rand();

    for (int layer = 0; layer < countLayers; layer++)
    {

        for (int perseptron = 0; perseptron < layers[layer]->getQuantityOfPerceptrons(); perseptron++)
        {
            layers[layer]->getPerceptronsArray()[perseptron].x = (double)(rand()) / RAND_MAX * (1.0 - 0.01) + 0.01;
            //qDebug() << "Layer " << layer  << " perceptron[" << perseptron << "] = " << layers[layer]->getPerceptronsArray()[perseptron].x;
        }

    }
}

// Сигмоїда
double NeuralNetwork::sigm(double x)
{
    return (1.0 / (1.0 + exp(-x)));
}

// Похідна сигмоїди
double NeuralNetwork::d_sigm(double x)
{
    return (sigm(x) * (1 - sigm(x)))+0.00001;
}

// Метод обраховує персептрони наступного слою   ---   Застосовані потоки
// begin i end - вказують діапазон персептронів, які необхідно обрахувати
// layer - номер слою, у якому обраховуються персептрони
void NeuralNetwork::calculatePerceptrons(int begin, int end, int layer)
{
    //qDebug() << "New layer " << layer;
    for (int perceptron = begin; perceptron < end; perceptron++)
    {
        layers[layer]->getPerceptronsArray()[perceptron].z = 0;

        for (int perceptron1 = 0; perceptron1 < layers[layer - 1]->getQuantityOfPerceptrons(); perceptron1++)
        {
            layers[layer]->getPerceptronsArray()[perceptron].z +=
                layers[layer - 1]->getPerceptronsArray()[perceptron1].x *
                layers[layer - 1]->getPerceptronsArray()[perceptron1].weights[perceptron];
        }

        layers[layer]->getPerceptronsArray()[perceptron].z += layers[layer]->getBias().weights[perceptron];

        layers[layer]->getPerceptronsArray()[perceptron].x = sigm(layers[layer]->getPerceptronsArray()[perceptron].z);
        //qDebug() << layers[layer]->getPerceptronsArray()[perceptron].x;
    }
}

// Метод обраховує значення нейромережі
void NeuralNetwork::calculateNeuralNetwork()
{
    for (int layer = 0; layer < (countLayers - 1); layer++)
    {
        // Отримую кількість персептронів, які необхідно обрахувати на наступному слої нейромережі
        int quantityPerceptrons = layers[layer + 1]->getQuantityOfPerceptrons();

        std::thread th1([&]()
        {
            calculatePerceptrons(0, quantityPerceptrons / 2, layer + 1);
        });

        calculatePerceptrons(quantityPerceptrons / 2, quantityPerceptrons, layer + 1);

        th1.join();
    }

    for(int i = 0; i < 10; i++)
    {
        arr[i] = layers[countLayers - 1]->getPerceptronsArray()[i].x;
    }
}

// Функція виставляє в останьому слої нейромережі в змінних d_x "правильні значення" персептронів
// Потім відбувається обрахування d_x = x - d_x
void NeuralNetwork::setTrueAnswer(int numbPerceptron)
{

    for (int perceptron = 0; perceptron < layers[countLayers - 1]->getQuantityOfPerceptrons(); perceptron++)
    {
        if (perceptron == numbPerceptron)
        {
            layers[countLayers - 1]->getPerceptronsArray()[perceptron].d_x = layers[countLayers - 1]->getPerceptronsArray()[perceptron].x - 1.0;

            //qDebug() << "setTrueAnswer TRUE " << perceptron << " = " << layers[countLayers - 1]->getPerceptronsArray()[perceptron].d_x;
        }
        else
        {
            layers[countLayers - 1]->getPerceptronsArray()[perceptron].d_x = layers[countLayers - 1]->getPerceptronsArray()[perceptron].x - 0.0;

            //qDebug() << "setTrueAnswer " << perceptron << " = " << layers[countLayers - 1]->getPerceptronsArray()[perceptron].d_x;
        }

        //layers[countLayers - 1]->getPerceptronsArray()[perceptron].d_x = layers[countLayers - 1]->getPerceptronsArray()[perceptron].x -
        //    layers[countLayers - 1]->getPerceptronsArray()[perceptron].d_x;
    }

    /*
    layers[countLayers - 1]->getPerceptronsArray()[0].d_x = layers[countLayers - 1]->getPerceptronsArray()[0].x;
    layers[countLayers - 1]->getPerceptronsArray()[1].d_x = layers[countLayers - 1]->getPerceptronsArray()[1].x;
    layers[countLayers - 1]->getPerceptronsArray()[2].d_x = layers[countLayers - 1]->getPerceptronsArray()[2].x;
    layers[countLayers - 1]->getPerceptronsArray()[3].d_x = layers[countLayers - 1]->getPerceptronsArray()[3].x;
    layers[countLayers - 1]->getPerceptronsArray()[4].d_x = layers[countLayers - 1]->getPerceptronsArray()[4].x;
    layers[countLayers - 1]->getPerceptronsArray()[5].d_x = layers[countLayers - 1]->getPerceptronsArray()[5].x;
    layers[countLayers - 1]->getPerceptronsArray()[6].d_x = layers[countLayers - 1]->getPerceptronsArray()[6].x;
    layers[countLayers - 1]->getPerceptronsArray()[7].d_x = layers[countLayers - 1]->getPerceptronsArray()[7].x;
    layers[countLayers - 1]->getPerceptronsArray()[8].d_x = layers[countLayers - 1]->getPerceptronsArray()[8].x;
    layers[countLayers - 1]->getPerceptronsArray()[9].d_x = layers[countLayers - 1]->getPerceptronsArray()[9].x;

    layers[countLayers - 1]->getPerceptronsArray()[numbPerceptron].d_x = layers[countLayers - 1]->getPerceptronsArray()[numbPerceptron].x - 1.0;
    */

}

// --- з.р.п. - Зворотнє розповсюдження помилки ---

// Обчислення помилки для здвигу за методом з.р.п.
void NeuralNetwork::backPropagationBias(int layer)
{
    for (int bias = 0; bias < layers[layer]->getQuantityOfPerceptrons(); bias++)
    {
        double d_weight = 0;

        //double x = layers[layer]->getPerceptronsArray()[bias].x;
        double d_x = layers[layer]->getPerceptronsArray()[bias].d_x;
        double z = layers[layer]->getPerceptronsArray()[bias].z;

        d_weight = 2*(d_x)*d_sigm(z);

        layers[layer]->getBias().d_weights[bias] = d_weight;

        //layers[layer]->getBias().weights[bias] -= layers[layer]->getBias().d_weights[bias];
    }
}

// Обчислення помилки для ваг за методом з.р.п.
void NeuralNetwork::backPropagationWeights(int layer)
{
    for (int perceptron = 0; perceptron < layers[layer - 1]->getQuantityOfPerceptrons(); perceptron++)
    {
        for (int weight = 0; weight < layers[layer - 1]->getQuantityOfWeights(); weight++)
        {
            double d_weight = 0.0;

            //double x = layers[layer]->getPerceptronsArray()[weight].x;
            double d_x = layers[layer]->getPerceptronsArray()[weight].d_x;
            double z = layers[layer]->getPerceptronsArray()[weight].z;
            double xx = layers[layer - 1]->getPerceptronsArray()[perceptron].x;

            d_weight = 2*(d_x)*d_sigm(z)*xx;

            layers[layer - 1]->getPerceptronsArray()[perceptron].d_weights[weight] = d_weight;

            //layers[layer - 1]->getPerceptronsArray()[perceptron].weights[weight] -=
                //layers[layer - 1]->getPerceptronsArray()[perceptron].d_weights[weight];

            //if(layers[layer - 1]->getPerceptronsArray()[perceptron].d_weights[weight]==0)
            //{
            //    qDebug() << "w " << layer-1 << " " << perceptron << " " << weight << " = " << layers[layer - 1]->getPerceptronsArray()[perceptron].d_weights[weight];

            //    qDebug() << "d_x " << d_x;
            //    qDebug() << "z " << z;
            //    qDebug() << "xx " << xx;
            //    qDebug() << "===============";
            //}
        }

    }

    backPropagationBias(layer);
}

void NeuralNetwork::weightsDifference()
{
    for (int layer = countLayers - 1; layer > 0; --layer)
    {
        for (int perceptron = 0; perceptron < layers[layer - 1]->getQuantityOfPerceptrons(); perceptron++)
        {
            for (int weight = 0; weight < layers[layer - 1]->getQuantityOfWeights(); weight++)
            {

                layers[layer - 1]->getPerceptronsArray()[perceptron].weights[weight] -=
                    0.02*layers[layer - 1]->getPerceptronsArray()[perceptron].d_weights[weight];
                layers[layer - 1]->getPerceptronsArray()[perceptron].d_weights[weight] = 0.0;

            }
        }

        for (int bias = 0; bias < layers[layer]->getQuantityOfPerceptrons(); bias++)
        {
            layers[layer]->getBias().weights[bias] -= 0.02*layers[layer]->getBias().d_weights[bias];
            layers[layer]->getBias().d_weights[bias]=0.0;
        }
    }
}

// Обчислення помилки для персептронів за методом з.р.п.
void NeuralNetwork::backPropagationPerceptrons(int layer)
{
    for (int perceptron = 0; perceptron < layers[layer - 1]->getQuantityOfPerceptrons(); perceptron++)
    {
        double d_perceptron = 0;
        for (int weight = 0; weight < layers[layer - 1]->getQuantityOfWeights(); weight++)
        {
            //double x = layers[layer]->getPerceptronsArray()[weight].x;
            double d_x = layers[layer]->getPerceptronsArray()[weight].d_x;
            double z = layers[layer]->getPerceptronsArray()[weight].z;
            double ww = layers[layer - 1]->getPerceptronsArray()[perceptron].weights[weight];

            d_perceptron += 2*(d_x)*d_sigm(z)*ww;
        }

        layers[layer - 1]->getPerceptronsArray()[perceptron].d_x = d_perceptron;
        //() << "layer = " << (layer - 1) <<" perceptron = " << perceptron << " d_x = " << d_perceptron;
        layers[layer - 1]->getPerceptronsArray()[perceptron].x -= layers[layer - 1]->getPerceptronsArray()[perceptron].d_x;

    }

}

// Реалізація методу з.р.п.
void NeuralNetwork::backPropagation()
{
    for (int layer = countLayers - 1; layer > 0; --layer)
    {
        backPropagationWeights(layer);
        backPropagationPerceptrons(layer);
    }
    weightsDifference();
}

// Функція визначає рівень навчиності нейромережі
// Чим значення double, що повертає функція, ближче до нуля, тим більше навчина нейромережа
double NeuralNetwork::levelOfEducation()
{
    double result = 0.0;
    for (int perceptron = 0; perceptron < layers[countLayers - 1]->getQuantityOfPerceptrons(); perceptron++)
    {
        //double x = layers[countLayers - 1]->getPerceptronsArray()[perceptron].x;
        double d_x = layers[countLayers - 1]->getPerceptronsArray()[perceptron].d_x;

        result += pow((d_x), 2);
    }
    return result;
}

bool NeuralNetwork::saveWeights(QString path)
{
    using json = nlohmann::json;

    std::ifstream in_file;
    std::ofstream out_file;
    json j;

    auto openFile = [&in_file](QString fileName)
    {
    in_file.open(fileName.toStdString());

        if(!in_file.is_open())
        {
            in_file.close();

            std::fstream f (fileName.toStdString(), std::ios::in | std::ios::out | std::ios::app);
            f << '{' << std::endl << std::endl << '}';
            f.close();

            in_file.open(fileName.toStdString());
        }


    };

    auto saveAndCloseFile = [&](QString fileName)
    {
        in_file.close();
        out_file.open(fileName.toStdString());

        out_file << std::setw(4) << j << std::endl;
        out_file.close();
    };

    QString fileName;

    fileName = path + "parameters.json";
    openFile(fileName);

    j = json::parse(in_file);

    j["Size"] = countLayers;
    for(int layer = 0; layer<countLayers; layer++)
    {
        j["perceprons"][layer] = layers[layer]->getQuantityOfPerceptrons();
    }

    saveAndCloseFile(fileName);
    j.clear();

        for(int layer = 0; layer<countLayers; layer++)
        {

            // Визначаю назву файла
            if(layer == countLayers-1)  { fileName = path +  "outputLayer_perceptronsWeights.json"; }         // Вихідний слой
            else
            {   if(layer == 0){ fileName = path +  "inputLayer_perceptronsWeights.json"; }                    // Вхідний слой
                else{ fileName = path +  "hiddenLayer_perceptronsWeights_"+QString::number(layer) + ".json";} // Приховані слої
            }

            openFile(fileName);

            j = json::parse(in_file);

            for(int perceptron = 0; perceptron < layers[layer]->getQuantityOfPerceptrons(); perceptron++)
            {
                for(int weight = 0; weight < layers[layer]->getQuantityOfWeights(); weight++)
                {

                    j["Layers"][layer]["Perceptrons"][perceptron]["Weights"][weight] =
                    {{"weight", layers[layer]->getPerceptronsArray()[perceptron].weights[weight]}};

                }

            }

            for(int bias = 0; bias < layers[layer]->getQuantityOfPerceptrons(); bias++)
            {
                j["Layers"][layer]["Biases"][bias] = {{"weight", layers[layer]->getBias().weights[bias]}};

            }

            saveAndCloseFile(fileName);
            j.clear();

        }

        return 1;
}

bool NeuralNetwork::loadWeights(QString path, int numbOfLayer)
{
    QString fileName;
    // Визначаю назву файла
    if(numbOfLayer == countLayers-1)
    { fileName = path +  "outputLayer_perceptronsWeights.json"; }         // Вихідний слой
    else
    {   if(numbOfLayer == 0){ fileName = path +  "inputLayer_perceptronsWeights.json"; }                    // Вхідний слой
        else{ fileName = path +  "hiddenLayer_perceptronsWeights_"+QString::number(numbOfLayer) + ".json";} // Приховані слої
    }


    using json = nlohmann::json;

    std::ifstream file;
    json j;

    file.open(fileName.toStdString());

    //if(!file.is_open())
    //{
    //    return 0;
    //}

    //qDebug() << fileName;

    j = json::parse(file);

    file.close();

    for(int perceptron = 0; perceptron < layers[numbOfLayer]->getQuantityOfPerceptrons(); perceptron++)
    {
        for(int weight = 0; weight < layers[numbOfLayer]->getQuantityOfWeights(); weight++)
        {
            layers[numbOfLayer]->getPerceptronsArray()[perceptron].weights[weight] =
                    j["Layers"][numbOfLayer]["Perceptrons"][perceptron]["Weights"][weight]["weight"];
            //qDebug() << "L: " << numbOfLayer << " P: " << perceptron << " W: " << weight << " = " << layers[numbOfLayer]->getPerceptronsArray()[perceptron].weights[weight];
        }
        layers[numbOfLayer]->getBias().weights[perceptron] = j["Layers"][numbOfLayer]["Biases"][perceptron]["weight"];
    }

    j.clear();

    return 1;
}

void NeuralNetwork::checkWeights(int numbOfLayer)
{
    for(int perceptron = 0; perceptron < layers[numbOfLayer]->getQuantityOfPerceptrons(); perceptron++)
    {
        for(int weight = 0; weight < layers[numbOfLayer]->getQuantityOfWeights(); weight++)
        {
            qDebug() << "perceptron[" << perceptron << "] weight[" << weight << "] = " << layers[numbOfLayer]->getPerceptronsArray()[perceptron].weights[weight];
        }
        qDebug() << " ";
    }
}

void NeuralNetwork::showInputPerseptrons()
{

        for (int i = 0; i < 28; ++i)
        {
            QDebug aaa = qDebug();
            for (int j = 0; j < 28; ++j)
            {

                aaa << layers[0]->getPerceptronsArray()[i*28+j].x << " ";
            }
            qDebug() = aaa;
        }

}



NeuralNetwork::~NeuralNetwork()
{

}
