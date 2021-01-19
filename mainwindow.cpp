#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->graphicsView->setMouseTracking(true);

    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    std::srand(time(NULL));
    rand();

    quantityOfLayers = 3;
    layersAndPerceptrons = new int[quantityOfLayers];
    layersAndPerceptrons[0] = 784;
    layersAndPerceptrons[1] = 10;
    layersAndPerceptrons[2] = 10;



    //=====================//

    // Ініціалізуємо об'єкт сцени для рисування
    scene = new paintScene();

    scene->setGraphicsViewSize(ui->graphicsView->width(), ui->graphicsView->height());
    scene->setImage(":/backGroundScene/resources/BackGround.png");
    scene->createImgFoldersData(10);
    scene->createPixelsArray();

    // Створюю pixmap для майбутнього об'єкту (растової картинки)
    QPixmap *pixmap_img = new QPixmap();

    // Надав необхідного розміру для pixmap (відповідно до розміру graphicsView)
    *pixmap_img = pixmap_img->scaled(ui->graphicsView->width(),ui->graphicsView->height(),Qt::IgnoreAspectRatio);

    // Устанавлюємо сцену
    ui->graphicsView->setScene(scene);

    // СИГНАЛИ
    // ===========================
    //Ініціалізація таймера
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    // Запускаємо таймер
    timer->start(100);

    //connect(scene, &paintScene::beginCalculate, this, &MainWindow::calculateNeural);
    connect(this, &MainWindow::sendNumb,this,&MainWindow::setValueOfProgBar);
    // ===========================

    PerceptronsLayer* p = nullptr;

    p = new PerceptronsLayer(784, 32);
    perceptronsLayerVector.push_back(p);
    p = new PerceptronsLayer(32, 10);
    perceptronsLayerVector.push_back(p);
    p = new PerceptronsLayer(10, 10);
    perceptronsLayerVector.push_back(p);
    p = nullptr;

    Neural = new NeuralNetwork();
    Neural->createNeuralNetwork(perceptronsLayerVector);

    Neural->setPrimaryPerseptrons();
    Neural->setPrimaryWeights();


    this->setFixedSize(782,480);

    isActiveDir = 0;
    isActiveNeural = 0;

    bitset = 0xffffe03b;
    setACTIVE(bitset);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setACTIVE(std::bitset<32> bits)
{
    scene->setIsActive(!bits[0]);
    ui->pushButton_20->setDisabled(bits[1]);
    ui->tabWidget->setTabEnabled(0, !bits[2]);
    ui->tabWidget->setTabEnabled(1, !bits[3]);
    ui->tabWidget->setTabEnabled(2, !bits[4]);
    ui->tabWidget->setTabEnabled(3, !bits[5]);
    ui->pushButton_4->setDisabled(bits[6]);
    ui->pushButton_5->setDisabled(bits[7]);
    ui->comboBox->setDisabled(bits[8]);
    ui->comboBox_2->setDisabled(bits[9]);
    ui->pushButton_7->setDisabled(bits[10]);
    ui->pushButton_8->setDisabled(bits[11]);
    ui->pushButton_6->setDisabled(bits[12]);
    ui->pushButton_22->setDisabled(bits[13]);
    ui->pushButton_12->setDisabled(bits[14]);
    ui->pushButton_13->setDisabled(bits[15]);
    ui->pushButton_15->setDisabled(bits[16]);
    ui->pushButton_19->setDisabled(bits[17]);
    ui->pushButton_14->setDisabled(bits[18]);
    ui->pushButton_16->setDisabled(bits[19]);
    ui->pushButton_18->setDisabled(bits[20]);
    ui->pushButton_17->setDisabled(bits[21]);
    ui->pushButton_23->setDisabled(bits[22]);
    ui->pushButton_26->setDisabled(bits[23]);
    ui->pushButton_25->setDisabled(bits[24]);
    ui->pushButton->setDisabled(bits[25]);
    ui->pushButton_2->setDisabled(bits[26]);
    ui->pushButton_11->setDisabled(bits[27]);
    ui->pushButton_3->setDisabled(bits[28]);
    ui->pushButton_9->setDisabled(bits[29]);

}

void MainWindow::slotTimer()
{
    // Перевизначаємо розміри для сцени, залежно від зміни вікна
    // Якщо не встановленно this->setFixedSize
    timer->stop();
    scene->setSceneRect(0,0, ui->graphicsView->width() - 3, ui->graphicsView->height() - 3);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    timer->start(100);
    QWidget::resizeEvent(event);
}

void MainWindow::ShowPerc()
{
    ui->label_34->setText(QString::number(Neural->arr[0]));
    ui->label_35->setText(QString::number(Neural->arr[1]));
    ui->label_36->setText(QString::number(Neural->arr[2]));
    ui->label_37->setText(QString::number(Neural->arr[3]));
    ui->label_38->setText(QString::number(Neural->arr[4]));
    ui->label_39->setText(QString::number(Neural->arr[5]));
    ui->label_40->setText(QString::number(Neural->arr[6]));
    ui->label_41->setText(QString::number(Neural->arr[7]));
    ui->label_42->setText(QString::number(Neural->arr[8]));
    ui->label_43->setText(QString::number(Neural->arr[9]));
}

void MainWindow::calculateNeural()
{
    int dirNumb = (ui->lineEdit_2->text()).toInt();
    qDebug() << "dirNumb = " << dirNumb;
    QString path = scene->getImgName(dirNumb);
    scene->setImage(path);

    scene->getRGBfromImg(1);
    Neural->setInputPerceptrons(scene->getPixelsArrayPtr(),28,28);
    //Neural->showInputPerseptrons();

    Neural->calculateNeuralNetwork();
    Neural->setTrueAnswer(dirNumb);
    qDebug() << "Answer";
    for(int i = 0; i < 10; i++)
    {
        qDebug() << i << " - " << Neural->arr[i];
    }

     //Neural->backPropagation();
     qDebug() << "levelOfEducation = " << Neural->levelOfEducation();

     ShowPerc();
}

void MainWindow::calculateNeuralFromWindow()
{
    scene->setImageFromWindow();
    scene->getRGBfromImg(10);
    Neural->setInputPerceptrons(scene->getPixelsArrayPtr(),28,28);
    //Neural->showInputPerseptrons();

    Neural->calculateNeuralNetwork();

    qDebug() << "Answer";
    for(int i = 0; i < 10; i++)
    {
        qDebug() << i << " - " << Neural->arr[i];
    }

     //Neural->backPropagation();
     qDebug() << "levelOfEducation = " << Neural->levelOfEducation();

     ShowPerc();
}

void MainWindow::studyNeural()
{
    if(isLearning!=1)
    {
    std::thread th1([&]()
    {
        isLearning = 1;
        int dirNumb = g_dirNumb;

        for(int numb = g_numbOfPicture; numb < scene->getNumbOfImg(); numb++)
        {

            QString path = scene->getImgName(dirNumb);

            scene->getRGBfromImgForLearning(path);

            Neural->setInputPerceptrons(scene->getPixelsArrayPtr(),28,28);
            Neural->calculateNeuralNetwork();

            Neural->setTrueAnswer(dirNumb);

            Neural->backPropagation();

                qDebug() << "Number = " << numb;
                qDebug() << "\niteration = " << 1+dirNumb << "\nlevel of Education = " << Neural->levelOfEducation();

                ui->label_49->setText(QString::number(numb+1));
                ui->label_50->setText(QString::number(dirNumb));
                ui->label_51->setText(QString::number(Neural->levelOfEducation()));
                ui->label_55->setText(QString::number(scene->getNumbOfImg()));
                emit sendNumb(numb);


            dirNumb++;
            if(dirNumb>9)
                dirNumb = 0;

            if(isLearning==0)
            {
                g_dirNumb = dirNumb;
                g_numbOfPicture = numb+1;
                break;
            }

            if(numb==(scene->getNumbOfImg() - 1))
            {
                emit sendNumb((scene->getNumbOfImg()));
                g_dirNumb = 0;
                g_numbOfPicture = 0;
            }

        }



        isLearning = 0;
        qDebug() << "Done";
    });

    th1.detach();
    }

}

void MainWindow::setValueOfProgBar(int value)
{
    int percent = (static_cast<double>(value)/static_cast<double>(scene->getNumbOfImg()))*100.0;
    ui->progressBar->setValue(percent);
}

void MainWindow::on_pushButton_clicked()
{
    calculateNeural();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->comboBox_2->clear();

    // Визначаємо, який із двох масивів слоїв є менший - старий чи новий
    // ((індекс comboBox + 1) кількість прихованих слоїв
    // добавляємо ще 2 слої (вхідний/вихідний) і отримуємо index+3)
    int size = ((index+3)<=quantityOfLayers)? (index+3) : quantityOfLayers;

    // Створюю буферний масив для збереження старих значень
    int *buffArr;
    if(layersAndPerceptrons!=nullptr)
    {
        buffArr = new int[size];
        //Процес копіювання
        for(int i = 0; i<size-1;i++)
        {
            if(i==0) { buffArr[i] = 784; }
            else { buffArr[i] = layersAndPerceptrons[i]; }
        }
        // Видаляємо старий масив
        delete[] layersAndPerceptrons;
    }

    // Зберігаємо кількість нових слоїв
    quantityOfLayers = index + 3;

    // Виділяємо пам*ять для нового масиву
    layersAndPerceptrons = new int[quantityOfLayers];

    for(int i = 0; i<quantityOfLayers;i++)
    {
        // Добавляю номери слоїв у випадаючий список
        if(i > 0 && i < quantityOfLayers-1)
        {
        ui->comboBox_2->addItem(QString::number(i));
        }

        if(i<size-1){ layersAndPerceptrons[i] = buffArr[i]; }
        else { layersAndPerceptrons[i] = 10; }
    }

    QString buff = QString::number(layersAndPerceptrons[(ui->comboBox_2->currentIndex()+1)]);
    ui->lineEdit_3->setText(buff);
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    QString buff = QString::number(layersAndPerceptrons[index+1]);
    ui->lineEdit_3->setText(buff);
}

void MainWindow::on_lineEdit_3_textEdited(const QString &text)
{
    int numb = text.toInt();
    if(numb < 1 || numb > 1000)
    {
        QMessageBox::information(this, "Увага", "Невірна кількість персептронів.\nВведіть від 1 до 1000.");
        ui->lineEdit_3->setText("10");
        layersAndPerceptrons[ui->comboBox_2->currentIndex()+1] = 10;
    }
    else
    {
        layersAndPerceptrons[ui->comboBox_2->currentIndex()+1] = numb;
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    isActiveNeural=1;
    bitset = 0xfe078003;
    setACTIVE(bitset);

    createLayers();
    createNeural();

    QMessageBox::information(this, "Нейромережа", "Вітаю!\nНейромережа ініціалізована!");
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->listWidget->clear();
}

void MainWindow::on_pushButton_7_clicked()
{
    QString buff;
    for(int layers = 0; layers < quantityOfLayers; layers++)
    {
        if(layers==quantityOfLayers-1)
        {
            buff = "(базовий)Вихідний слой, персептрони - " +
                    QString::number(layersAndPerceptrons[layers]);
        }
        else if(layers == 0)
        {
            buff = "(базовий)Вхідний слой, персептрони - " +
                    QString::number(layersAndPerceptrons[layers]) + ", ваги - " +
                    QString::number(layersAndPerceptrons[layers]) + "*" +
                    QString::number(layersAndPerceptrons[layers+1]) + " = " +
                    QString::number(layersAndPerceptrons[layers]*layersAndPerceptrons[layers+1]) ;;
        }
        else
        {
            buff = "Прихований слой[" + QString::number(layers) + "] , персептрони - " +
                    QString::number(layersAndPerceptrons[layers]) + ", ваги - " +
                    QString::number(layersAndPerceptrons[layers]) + "*" +
                    QString::number(layersAndPerceptrons[layers+1]) + " = " +
                    QString::number(layersAndPerceptrons[layers]*layersAndPerceptrons[layers+1]) ;;
        }

        ui->listWidget->addItem(buff);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    isActiveNeural=1;
    bitset = 0xfe078003;
    setACTIVE(bitset);

    // Отримую шлях до папки з проектом
    QString exePath = QApplication::applicationDirPath();
    // Утворюю повний шлях до папки із базою даних
    QString pathToDir = exePath + "/base/";
    // Створюю об'єкт для роботи з папкою із базою даних
    QDir *directory = new QDir(pathToDir);
    // Отримую дані про файли, що містяться у теці
    QFileInfoList infoListBase = directory->entryInfoList();

    if(checkLoadingFiles(infoListBase, pathToDir))
    {
        createLayers();
        createNeural();

        for(int layer = 0; layer < quantityOfLayers; layer++)
        {
            Neural->loadWeights(pathToDir, layer);
        }

        QMessageBox::information(this, "Нейромережа", "Вітаю!\nНейромережа ініціалізована!");
    }

    // ----- Оновлюємо випадаючі списки -----
    ui->comboBox->setCurrentText(QString::number(quantityOfLayers-2));
    QString buff = QString::number(layersAndPerceptrons[(ui->comboBox_2->currentIndex()+1)]);
    ui->lineEdit_3->setText(buff);
    // --------------------------------------

   delete directory;

}

void MainWindow::on_pushButton_5_clicked()
{
    isActiveNeural=1;
    bitset = 0xfe078003;
    setACTIVE(bitset);

    // Отримую шлях до папки з проектом
    QString exePath = QApplication::applicationDirPath();

    QString pathToDir = QFileDialog::getExistingDirectory(this, "Open Dir", exePath, QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    //QMessageBox::about(this, "Check Directory", pathToDir);

    pathToDir+='/';

    // Створюю об'єкт для роботи з папкою із базою даних
    QDir *directory = new QDir(pathToDir);
    // Отримую дані про файли, що містяться у теці
    QFileInfoList infoListBase = directory->entryInfoList();

    if(checkLoadingFiles(infoListBase, pathToDir))
    {
        createLayers();
        createNeural();

        for(int layer = 0; layer < quantityOfLayers; layer++)
        {
            Neural->loadWeights(pathToDir, layer);
        }

        QMessageBox::information(this, "Нейромережа", "Вітаю!\nНейромережа ініціалізована!");
    }

    // ----- Оновлюємо випадаючі списки -----
    ui->comboBox->setCurrentText(QString::number(quantityOfLayers-2));
    QString buff = QString::number(layersAndPerceptrons[(ui->comboBox_2->currentIndex()+1)]);
    ui->lineEdit_3->setText(buff);
    // --------------------------------------

    delete directory;

}

// Кнопка вибору шляху до користувацької БД з картинками
void MainWindow::on_pushButton_12_clicked()
{
    isActiveDir = 1;
    bitset = 0xfc030003;
    setACTIVE(bitset);

    // Отримую шлях до папки з проектом
    QString exePath = QApplication::applicationDirPath();

    QString pathToDir = QFileDialog::getExistingDirectory(this, "Open Dir", exePath, QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    //QMessageBox::about(this, "Check Directory", pathToDir);

    pathToDir+='/';

    // Створюю об'єкт для роботи з папкою із базою даних
    QDir *directory = new QDir(pathToDir);
    // Отримую дані про файли, що містяться у теці
    QFileInfoList infoListBase = directory->entryInfoList(QDir::AllDirs);

    if(checkFoldersData(infoListBase, pathToDir))
    {
        // Кількість усіх картинок
        int numbOfAllPict = 0;
        for(int i = 2; i < infoListBase.size(); i++)
        {
            QDir *dir = new QDir(pathToDir+infoListBase[i].baseName()+'/');
            //Отримую дані про файли, що містяться у теці
            QFileInfoList infoList = dir->entryInfoList();
            numbOfAllPict += infoList.size()-2;
            delete dir;
        }

        //qDebug() << numbOfAllPict;

        scene->setDataDir(pathToDir, numbOfAllPict);
        scene->createImgFoldersData(10);

        QMessageBox::information(this, "Нейромережа", "Папка із навчальним матеріалом вибрана.");
    }

}

// Кнопка вибору шляху до БД з картинками за замовчуванням
void MainWindow::on_pushButton_22_clicked()
{
    isActiveDir = 1;
    bitset = 0xfc030003;
    setACTIVE(bitset);

    // Отримую шлях до папки з проектом
    QString exePath = QApplication::applicationDirPath();

    // Утворюю повний шлях до папки із базою даних
    QString pathToDir = exePath + "/training/";

    //QMessageBox::about(this, "Check Directory", pathToDir);

    // Створюю об'єкт для роботи з папкою із базою даних
    QDir *directory = new QDir(pathToDir);
    // Отримую дані про файли, що містяться у теці
    QFileInfoList infoListBase = directory->entryInfoList(QDir::AllDirs);


    if(checkFoldersData(infoListBase, pathToDir))
    {
        // Кількість усіх картинок
        int numbOfAllPict = 0;
        for(int i = 2; i < infoListBase.size(); i++)
        {
            QDir *dir = new QDir(pathToDir+infoListBase[i].baseName()+'/');
            //Отримую дані про файли, що містяться у теці
            QFileInfoList infoList = dir->entryInfoList();
            numbOfAllPict += infoList.size()-2;
            delete dir;
        }

        scene->setDataDir(pathToDir, numbOfAllPict);
        scene->createImgFoldersData(10);

        QMessageBox::information(this, "Нейромережа", "Папка із навчальним матеріалом вибрана.");
    }
}

void MainWindow::on_pushButton_15_clicked()
{
    bitset = 0xdff97ff7;
    setACTIVE(bitset);

    isLearning=0;
}

// Скидуємо навчальний процес нейронної мережі
void MainWindow::on_pushButton_19_clicked()
{
    bitset = 0xfc030003;
    setACTIVE(bitset);

    isLearning = 0;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    g_numbOfPicture = 0;
    g_dirNumb = 0;
    scene->createImgFoldersData(10);
    Neural->setPrimaryPerseptrons();
    Neural->setPrimaryWeights();

    ui->label_49->setText("0");
    ui->label_50->setText("0");
    ui->label_51->setText("0.0");
    ui->label_55->setText("0");
    emit sendNumb(0);
}

void MainWindow::on_pushButton_13_clicked()
{
    bitset = 0xdff8fff7;
    setACTIVE(bitset);

    studyNeural();
}

void MainWindow::on_pushButton_14_clicked()
{
    ui->label_49->setText("0");
    ui->label_50->setText("0");
    ui->label_51->setText("0.0");
    ui->label_55->setText("0");
    emit sendNumb(0);
}

void MainWindow::on_pushButton_17_clicked()
{
    ui->listWidget_3->clear();
}

void MainWindow::on_pushButton_16_clicked()
{
    if(ui->lineEdit_4->text().isNull())
    {
        QMessageBox::warning(this, "Збереження", "Введіть коректну назву папки.");
    }
    else
    {
        int check = 0;
        for(int i = 0; i < ui->lineEdit_4->text().size(); i++)
        {
            char a = ui->lineEdit_4->text().toStdString()[i];
            if(((a >= 48 && a <= 57) || (a >= 97 && a <= 122) || (a >= 65 && a <= 90) || a==95)){}
            else{ check = 1;break; }
        }

        if(check)
        {
            QMessageBox::warning(this, "Збереження", "Введіть коректну назву папки.");
        }
        else
        {
            // Отримую шлях до папки з проектом
            QString exePath = QApplication::applicationDirPath();
            exePath += "/saving/";
            // Створюю об'єкт для роботи з папкою із базою даних
            QDir *directory = new QDir(exePath);
            // Отримую дані про файли, що містяться у теці
            QFileInfoList infoList = directory->entryInfoList(QDir::AllDirs);

            int flag = 0;
            if(directory->exists(ui->lineEdit_4->text()))
            {
                QMessageBox::StandardButton reply = QMessageBox::question(this, "Збереження", "Папка вже існує.\nПерезаписати зміст папки?",
                                                                          QMessageBox::Yes | QMessageBox::No);
                if(reply == QMessageBox::No)
                {
                flag = 1;
                }
            }

            if(flag==0)
            {
                qDebug() << exePath+ui->lineEdit_4->text()+"/";

                exePath + ui->lineEdit_4->text();
                directory->mkdir(ui->lineEdit_4->text());
                Neural->saveWeights(exePath+ui->lineEdit_4->text()+"/");

                QMessageBox::about(this, "Збереження", "Збереженння відбулося успішно!");
            }

        }
    }

}

void MainWindow::on_pushButton_18_clicked()
{
    ui->listWidget_3->clear();

    // Отримую шлях до папки з проектом
    QString exePath = QApplication::applicationDirPath();
    exePath+="/saving/";

    // Створюю об'єкт для роботи з папкою із базою даних
    QDir *directory = new QDir(exePath);
    // Отримую дані про файли, що містяться у теці
    QFileInfoList infoList = directory->entryInfoList(QDir::AllDirs);

    for(int i = 2; i < infoList.size(); i++)
    {
        ui->listWidget_3->addItem(infoList[i].baseName());
    }
}

void MainWindow::on_pushButton_23_clicked()
{
    calculateNeuralFromWindow();
}

void MainWindow::on_pushButton_20_clicked()
{
    scene->setImage(":/backGroundScene/resources/BackGround.png");
    scene->saveSceneElements();
}

void MainWindow::on_pushButton_26_clicked()
{
    isActiveDir = 1;
    bitset = 0xfc030000;
    setACTIVE(bitset);

    // Отримую шлях до папки з проектом
    QString exePath = QApplication::applicationDirPath();

    // Утворюю повний шлях до папки із базою даних
    QString pathToDir = exePath + "/testing/";

    //QMessageBox::about(this, "Check Directory", pathToDir);

    // Створюю об'єкт для роботи з папкою із базою даних
    QDir *directory = new QDir(pathToDir);
    // Отримую дані про файли, що містяться у теці
    QFileInfoList infoListBase = directory->entryInfoList(QDir::AllDirs);


    if(checkFoldersData(infoListBase, pathToDir))
    {
        // Кількість усіх картинок
        int numbOfAllPict = 0;
        for(int i = 2; i < infoListBase.size(); i++)
        {
            QDir *dir = new QDir(pathToDir+infoListBase[i].baseName()+'/');
            //Отримую дані про файли, що містяться у теці
            QFileInfoList infoList = dir->entryInfoList();
            numbOfAllPict += infoList.size()-2;
            delete dir;
        }

        scene->setDataDir(pathToDir, numbOfAllPict);
        scene->createImgFoldersData(10);

        QMessageBox::information(this, "Нейромережа", "Папка із навчальним матеріалом вибрана.");
    }

}

void MainWindow::on_pushButton_25_clicked()
{
    isActiveDir = 1;
    bitset = 0xfc030000;
    setACTIVE(bitset);

    // Отримую шлях до папки з проектом
    QString exePath = QApplication::applicationDirPath();

    QString pathToDir = QFileDialog::getExistingDirectory(this, "Open Dir", exePath, QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
    //QMessageBox::about(this, "Check Directory", pathToDir);

    pathToDir+='/';

    // Створюю об'єкт для роботи з папкою із базою даних
    QDir *directory = new QDir(pathToDir);
    // Отримую дані про файли, що містяться у теці
    QFileInfoList infoListBase = directory->entryInfoList(QDir::AllDirs);

    if(checkFoldersData(infoListBase, pathToDir))
    {
        // Кількість усіх картинок
        int numbOfAllPict = 0;
        for(int i = 2; i < infoListBase.size(); i++)
        {
            QDir *dir = new QDir(pathToDir+infoListBase[i].baseName()+'/');
            //Отримую дані про файли, що містяться у теці
            QFileInfoList infoList = dir->entryInfoList();
            numbOfAllPict += infoList.size()-2;
            delete dir;
        }

        qDebug() << numbOfAllPict;

        scene->setDataDir(pathToDir, numbOfAllPict);
        scene->createImgFoldersData(10);

        QMessageBox::information(this, "Нейромережа", "Папка із навчальним матеріалом вибрана.");
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    ui->listWidget_4->clear();

    // Отримую шлях до папки з проектом
    QString exePath = QApplication::applicationDirPath();
    exePath+="/saving_img/";

    // Створюю об'єкт для роботи з папкою із базою даних
    QDir *directory = new QDir(exePath);
    // Отримую дані про файли, що містяться у теці
    QFileInfoList infoList = directory->entryInfoList(QDir::AllDirs);

    for(int i = 2; i < infoList.size(); i++)
    {
        ui->listWidget_4->addItem(infoList[i].baseName());
    }

}

void MainWindow::on_pushButton_3_clicked()
{
    QString fileName;
    fileName = ui->lineEdit->text();
    if(fileName.size()==1 && (fileName[0]>=48 && fileName[0]<=57))
    {
        if(ui->lineEdit_5->text()=="")
        {
            QMessageBox::warning(this, "Збереження картинки", "Введіть коректну назву папки.");
        }
        else
        {
            int check = 0;
            for(int i = 0; i < ui->lineEdit_5->text().size(); i++)
            {
                char a = ui->lineEdit_5->text().toStdString()[i];
                if(((a >= 48 && a <= 57) || (a >= 97 && a <= 122) || (a >= 48 && a <= 57))){}
                else{ check = 1;break; }
            }

            if(check)
            {
                QMessageBox::warning(this, "Збереження картинки", "Введіть коректну назву папки.");
            }
            else
            {
                // Отримую шлях до папки з проектом
                QString exePath = QApplication::applicationDirPath();
                exePath += "/saving_img/";
                // Створюю об'єкт для роботи з папкою із базою даних
                QDir *directory = new QDir(exePath);

                if(!(directory->exists(ui->lineEdit_5->text())))
                {
                    directory->mkdir(ui->lineEdit_5->text());
                    for(int nameDir = 0; nameDir < 10; nameDir++)
                    {
                        directory->mkdir(exePath + ui->lineEdit_5->text() + '/' +QString::number(nameDir));
                    }

                }

                delete directory;

                exePath += ui->lineEdit_5->text() + '/' + ui->lineEdit->text()+'/';

                directory = new QDir(exePath);
                // Отримую дані про файли, що містяться у теці
                QFileInfoList infoList = directory->entryInfoList(QDir::Files);

                qDebug()<< "size: " << infoList.size();

                exePath+=QString::number(infoList.size())+".png";

                scene->saveImage(exePath);

                QMessageBox::about(this, "Збереження картинки", "Збереженння відбулося успішно!");

                delete directory;

            }
        }

    }
    else
    {
        QMessageBox::warning(this, "Збереження картинки", "Введіть коректну мітку (0-9)");
    }

}

void MainWindow::on_pushButton_11_clicked()
{
    ui->listWidget_4->clear();
}

void MainWindow::on_pushButton_21_clicked()
{
    //bitset = 0xffffffff;
    //setACTIVE(bitset);
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    switch(index)
    {
    case 0:
    {
        if(isActiveNeural==1)
        {
        bitset = 0xfc030003;

        setACTIVE(bitset);
        }
        break;
    }
    case 1:
    {
        bitset = 0xfc030003;
        setACTIVE(bitset);
        break;
    }
    case 2:
    {
        bitset = 0xfc030000;
        if(isActiveDir==1) {bitset.reset(25);}
        else {bitset.set(25);}

        setACTIVE(bitset);
        break;
    }
    case 3:
    {
        bitset = 0xe0030000;

        setACTIVE(bitset);
        break;
    }
    }
}

void MainWindow::on_pushButton_9_clicked()
{
        isLearning = 0;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        g_numbOfPicture = 0;
        g_dirNumb = 0;
        //scene->createImgFoldersData(10);

        ui->label_49->setText("0");
        ui->label_50->setText("0");
        ui->label_51->setText("0.0");
        ui->label_55->setText("0");
        emit sendNumb(0);

        ui->tabWidget->setTabEnabled(0, 1);
        ui->tabWidget->setTabEnabled(1, 1);
        ui->tabWidget->setTabEnabled(2, 1);
        ui->tabWidget->setTabEnabled(3, 1);
        ui->pushButton_22->setDisabled(0);
        ui->pushButton_12->setDisabled(0);
        ui->pushButton_16->setDisabled(0);
        ui->pushButton_18->setDisabled(0);
        ui->pushButton_17->setDisabled(0);


        ui->pushButton_15->setDisabled(1);
        ui->pushButton_19->setDisabled(1);

        bitset = 0xffc31fc3;
        setACTIVE(bitset);

}
