#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QWidget>
#include <QTimer>
#include <QResizeEvent>
#include <QMessageBox>
#include <vector>
#include <bitset>

#include <functional>
#include <thread>
#include <chrono>

#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QDir>
#include <QString>
#include <QDebug>
#include <QCursor>
#include <QFileDialog>
#include <QRegExp>

#include "paintscene.h"
#include "myqgraphicsview.h"
#include "neuralnetwork.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // Оголошуємо таймер для підготовки актуальних розмірів графічної сцени
    QTimer *timer;

    // Оголошуємо сцену
    paintScene *scene;

    // Оголошуємо і ініціалізуємо слої нейромережі
    //PerceptronsLayer *obj0 = new PerceptronsLayer(784, 32);
    //PerceptronsLayer *obj1 = new PerceptronsLayer(32, 10);
    //PerceptronsLayer *obj2 = new PerceptronsLayer(10, 10);
    //PerceptronsLayer *obj3 = new PerceptronsLayer(10, 10);

    // Оголошуємо вектор вказівників на слої нейромережі
    std::vector<PerceptronsLayer*> perceptronsLayerVector;
    // Оголошуємо вказівник на нейромережу
    NeuralNetwork* Neural = nullptr;

    int quantityOfLayers = 0;
    int *layersAndPerceptrons = nullptr;

    // Якщо 1 - процес навчання йде, 0 - навчання призупинено
    bool isLearning = 0;
    // Показує, чи була відкрита папка із навчальними/тестувальними даними
    bool isActiveDir;
    // Показує, чи була створенна нейромережа
    bool isActiveNeural;

    int g_numbOfPicture = 0;;
    int g_dirNumb = 0;

    // Перевизначаємо подію зміни розміру вікна
    // для перерахунку розмірів графічної сцени
    void resizeEvent(QResizeEvent * event);

signals:
    void sendNumb(int value);

private slots:
    void slotTimer();

    void setValueOfProgBar(int value);

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void on_comboBox_2_currentIndexChanged(int index);
    void on_lineEdit_3_textEdited(const QString &text);
    void on_pushButton_6_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_4_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_21_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_pushButton_9_clicked();

public:

    std::function<void()> createLayers = [&]()
    {
        perceptronsLayerVector.size();
        for(int i = 0; i < perceptronsLayerVector.size(); i++)
        {
            delete perceptronsLayerVector[i];
        }

        perceptronsLayerVector.clear();

        PerceptronsLayer* p = nullptr;


        perceptronsLayerVector.resize(quantityOfLayers);


        for(int layer = 0; layer < quantityOfLayers; layer++)
        {
            if(layer==quantityOfLayers-1)
            {
                perceptronsLayerVector[layer] = new PerceptronsLayer(10, 10);
            }
            else if(layer==quantityOfLayers-2)
            {
                perceptronsLayerVector[layer] = new PerceptronsLayer(layersAndPerceptrons[layer], 10);

            }
            else if(layer == 0)
            {
                perceptronsLayerVector[layer] = new PerceptronsLayer(784, layersAndPerceptrons[layer+1]);

            }
            else
            {
                perceptronsLayerVector[layer] = new PerceptronsLayer(layersAndPerceptrons[layer], layersAndPerceptrons[layer+1]);

            }
        }


    };
    std::function<void()> createNeural = [&]()
    {
        if(Neural)
        {
            delete Neural;
        }

        Neural = new NeuralNetwork();
        Neural->createNeuralNetwork(perceptronsLayerVector);


        Neural->setPrimaryPerseptrons();
        Neural->setPrimaryWeights();
    };

    bool checkLoadingFiles(QFileInfoList &infoListBase, QString path)
    {
        QString fileName = "";
        QString textMessage;
        int result = 0;

        // Перевірка наявності "parameters.json"
        if(!infoListBase.contains(path+"parameters.json"))
        {
            result = -1;
        }
        else
        {
            using json = nlohmann::json;
            std::ifstream in_file;
            json j;

            in_file.open((path+"parameters.json").toStdString());

            // Перевірка, чи файл "parameters.json" був відкритий
            if(!in_file.is_open())
            {
                result = -2;
            }
            else
            {
                //j = json::parse(in_file);
                in_file >> j;
                in_file.close();

                // Отримую кількість слоїв
                quantityOfLayers = j["Size"];

                if(layersAndPerceptrons)
                {
                    delete[] layersAndPerceptrons;
                }

                layersAndPerceptrons = new int[quantityOfLayers];

                // Отримую кількість персептронів у слою
                for(int layer = 0; layer<quantityOfLayers; layer++)
                {
                    layersAndPerceptrons[layer] = j["perceprons"][layer];
                }
                j.clear();



                // Перевірка файлів слоїв
                for(int layer = 0; layer<quantityOfLayers; layer++)
                {
                    if(layer==quantityOfLayers-1)
                    {
                        fileName = "outputLayer_perceptronsWeights.json";
                        if(!infoListBase.contains(path+fileName))
                        {
                            result = -3;
                            break;
                        }

                    }
                    else if(layer == 0)
                    {
                        fileName = "inputLayer_perceptronsWeights.json";
                        if(!infoListBase.contains(path+fileName))
                        {
                            result = -3;
                            break;
                        }

                    }
                    else
                    {
                        fileName = "hiddenLayer_perceptronsWeights_" + QString::number(layer) + ".json";
                        if(!infoListBase.contains(path+fileName))
                        {
                            result = -3;
                            break;
                        }

                    }



                    in_file.open((path+fileName).toStdString());

                    // Перевірка, чи файл був відкритий
                    if(!in_file.is_open())
                    {
                        result = -4;
                        break;
                    }
                    else
                    {
                        //j = json::parse(in_file);
                        in_file >> j;
                        in_file.close();



                        // j["Layers"][layer]["Perceptrons"][perceptron]["Weights"][weight]
                        int perceptronsFromFile = 0;
                        int weightsFromFile = 0;

                        perceptronsFromFile = j["Layers"][layer]["Perceptrons"].size();
                        if(perceptronsFromFile!=layersAndPerceptrons[layer])
                        {
                            textMessage = "Невідповідна кількість персептронів у " + fileName + "\n";
                            result = -5;
                            break;
                        }
                        else
                        {



                            for(int i = 0; i < layersAndPerceptrons[layer]; i++)
                            {
                                weightsFromFile = j["Layers"][layer]["Perceptrons"][i]["Weights"].size();
                                if(layer!=quantityOfLayers-1)
                                {
                                    if(weightsFromFile != layersAndPerceptrons[layer+1])
                                    {
                                        textMessage = "Невідповідна кількість ваг у " + fileName + "\n";
                                        result = -5;
                                        break;
                                    }
                                }
                            }

                            perceptronsFromFile = j["Layers"][layer]["Biases"].size();
                            if(perceptronsFromFile!=layersAndPerceptrons[layer])
                            {
                                textMessage = "Невідповідна кількість ваг зміщень(bias) у " + fileName + "\n";
                                result = -5;
                                break;
                            }


                        }


                    }




                }

            }


        }


        // Результат перевірки
        switch(result)
        {
        case -1:
        {
            QMessageBox::warning(this, "Load File", "Файл " + path+"parameters.json" + " не було знайдено.\nМережа не була створена.");

            return 0;
            break;
        }
        case -2:
        {
            QMessageBox::warning(this, "Load File", "Файл " + path+"parameters.json" + " не був відкритий.\nМережа не була створена.");

            return 0;
            break;
        }
        case -3:
        {
            QMessageBox::warning(this, "Load File", "Файл " + path+fileName + " не було знайдено.\nМережа не була створена.");

            return 0;
            break;
        }
        case -4:
        {
            QMessageBox::warning(this, "Load File", "Файл " + path+fileName + " не був відкритий.\nМережа не була створена.");

            return 0;
            break;
        }
        case -5:
        {
            QMessageBox::warning(this, "Load File", "Невідповідна кількість параметрів:\n" + textMessage + "Мережа не була створена.");

            return 0;
            break;
        }
        default:
        {

            return 1;
        }
        }

    }

    bool checkFoldersData(QFileInfoList &infoListBase, QString path)
    {
        int result = 0;
        QString buff;

        // Перевіряємо кількість папок
        if((infoListBase.size()-2)!=10)
        {
            result = -1;
        }
        else
        {
            // Починаю цикл з індекса 2, через те, що перші два значення в infoListBase
            // це вказівники на поточну і попередню папки
            bool flag = 0;
            for(int i = 2; i < (infoListBase.size()); i++)
            {
                if(infoListBase[i].baseName()!= QString::number(i-2))
                {

                    result = -2;
                    break;
                }
                else
                {
                    QDir *directory = new QDir(path+QString::number(i-2)+'/');
                    // Отримую дані про файли, що містяться у теці
                    QFileInfoList infoList = directory->entryInfoList(QDir::Files);
                    //qDebug() << "Size: " << infoList.size();

                    // Перевіряю, чи у теках 0-9 є хоча б один файл
                    if(infoList.size()<1)
                    {
                        result = -3;
                        break;
                    }
                    else
                    {
                        //Якщо достатня кількість файлів, то перевіряю їх тип
                        //Всі файли мають мати розширення .png

                        QRegExp rx("*.png");
                        rx.setPatternSyntax(QRegExp::Wildcard);

                        // Вмикаю цикл на перевірку всіх файлів у теці
                        for(int j = 0; j < infoList.size();j++)
                        {
                        if(!(rx.exactMatch(infoList[j].fileName())))
                        {
                            buff = infoList[j].path() + '/' + infoList[j].fileName() ;
                            result = -4;
                            flag=1;
                            break;
                        }
                        }
                        // (Для того, щоб вийти із зовнішнього циклу)
                        if(flag == 1)
                        {
                            break;
                        }

                    }
                    delete directory;
                }
            }
        }

        // Результат перевірки
        switch(result)
        {
        case -1:
        {
            QMessageBox::warning(this, "Load DataBase", "Невідповідна кількість тек із файлами.");

            return 0;
            break;
        }
        case -2:
        {
            QMessageBox::warning(this, "Load DataBase", "Невірна назва папок\nз навчальним матеріалом.");

            return 0;
            break;
        }
        case -3:
        {
            QMessageBox::warning(this, "Load DataBase", "Має бути хоча б одна картинка в теках 0-9.");

            return 0;
            break;
        }
        case -4:
        {
            QMessageBox::warning(this, "Load DataBase", "Серед картинок наявний файл невідповідного формату.\nШлях: " + buff);

            return 0;
            break;
        }
        default:
        {

            return 1;
        }
        }
    }

    void ShowPerc();

    void calculateNeural();
    void calculateNeuralFromWindow();
    void studyNeural();

    // Встановлюю доступність/недоступність елементів інтерфейсу для користувача
    void setACTIVE(std::bitset<32> bits);


    // Бітсет, який використовується для "вмикання/вимикання" елементів інтерфейсу
    std::bitset<32> bitset;

};
#endif // MAINWINDOW_H
