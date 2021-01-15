#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QMouseEvent>
#include <QImage>
#include <QPainter>
#include <QMessageBox>
#include <QDir>
#include <QApplication>
#include <QPixmap>

#include <cmath>
#include <chrono>


#include <QDebug>

//==============================//

class Timer
{
private:
    // Псевдонимы типов используются для удобного доступа к вложенным типам
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1> >;

    std::chrono::time_point<clock_t> m_beg;

public:
    Timer() : m_beg(clock_t::now())
    {
    }

    void reset()
    {
        m_beg = clock_t::now();
    }

    double elapsed() const
    {
        return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
    }

};


//==============================//



//----------paintScene----------//
class paintScene : public QGraphicsScene
{
    Q_OBJECT

signals:
    void beginCalculate();

public:
    explicit paintScene(QGraphicsScene *parent = 0);

    ~paintScene();

private:   
    bool isActiveMouse = 0;
    //=============================================================================================================//

    // Структура, що зберігає координати пікселя (або інші параметри пікселя, як ширина чи довжина)
    struct PixelParameters
    {
        int x;
        int y;
    };

    //=============================================================================================================//

    struct structRGB    {
        int r;
        int g;
        int b;
        int alpha;
    } rgb;

    // currentImg - масив, що зберігає номер останього використаного
    // зображення відповідного каталогу (номер каталогу - порядковий номер комірки масиву
    // тобто currentImg[3] = 12 означає, що у каталозі "3" останнє використане зображення під номером 12)
    int *currentImg = nullptr;
    // Зберігає розмір динамічного масиву currentImg
    int sizeCurrentImg;

    // Зберігає цифру, що зображена на картинці
    //int numbOfCurrentImg;

    // Зберігає шлях до папки з картинками
    QString dataDir;
    // Зберігає кількість зображень
    int numbImg;

    // Масив, що зберігає дані про кольори пікселів картинки
    double **pixelsArray;

    //=============================================================================================================//

    /* На даний момент не використовується */

    // Структура, яка використовується при формуванні "розмітки" графічної сцени
    // Визначає ширину і довжину "розтягнутого" пікселя і та його центральний піксель
    /*
    struct CustomPixelMap
    {
        // x - координата x, y - координата y
        PixelParameters centralPixel;
        // x - ліва межа, y - права межа
        PixelParameters width;
        // x - верхня межа, y - нижня межа
        PixelParameters height;

    } *myCustomPixelMap;
    */

    //=============================================================================================================//

    int graphicsViewWidth, graphicsViewHeight;      // Розмір в пікселях віджета QGraphicsView
    int pixelMapWidth, pixelMapHeight;              // Розмір в пікселях картинки, що передається сцені

    //=============================================================================================================//

    QImage *Image = NULL;                           // Змінна зберігає відрендероване зображення з графічної сцени

    QPixmap *pixmapImage = NULL;                    // Використовується у разі, коли необхідно вставити в сцену
                                                    // графічний об'єкт- наприклад рисунок чи фон

    QGraphicsPixmapItem *pixmapItemImage = NULL;    // Слугує графічним об'єктом, що передається сцені
                                                    // Об'єктом може бути як рисунок (цифри) так і фон

    //=============================================================================================================//

    QPointF previousPoint;                          // Координати попередньої точки.
                                                    // Використовується при визначені координат для рисування ліній

    //=============================================================================================================//

    Qt::MouseButtons mouseButtonState;              // Змінна зберігає натиснуту кнопку миші

    //=============================================================================================================//

private:
    //=============================================================================================================//

    // Для рисування використовуємо події миші
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    //=============================================================================================================//

    void getPoints(int x0, int y0, int x1, int y1, PixelParameters* &arr, int &arrSize);  // Даний метод забезечує
                                                // отримання координат лінії. Дані координати, після обробки findPos
                                                // будуть використовуватися для рисування пікселів на сцені

    //=============================================================================================================//

    void setImage(QPixmap pixmap);                      // Перевантажена функція ! Для saveSceneElements !

    //=============================================================================================================//

public:

    //=============================================================================================================//

    void setImage(QString path);                        // Функція для розміщення графічного об'єкту на сцені

    //=============================================================================================================//

    void setImgSize(int width = 28, int height = 28);   // Функція, яка записує розмір картинки, що буде передаватися сцені
    void setGraphicsViewSize(qreal width = 280.0, qreal height = 280.0); // Функція, яка записує розмір віджета GraphicsView

    // Розмір пікселей, що відображаються на сцені, визначаються відповідно до пропорці розміру сцени (точніше GraphicsView)
    // до розміру картинки, що передається, тобто int ширинаРозміраПікселя = graphicsViewWidth/pixelMapWidth
    // Аналогічно визначається і висота - int висотаРозміраПікселя = graphicsViewHeight/pixelMapHeight

    //=============================================================================================================//

    /* На даний момент не використовується */
    /*
    void setMyCustomPixelMap();                 // Створення "розмітки", яка використовується для рисування
                                                // "розтягнутих" пропорційних пікселів до розміру віджета GraphicsView
    */

    //=============================================================================================================//

    void findPos(qreal X, qreal Y, int &out_x, int &out_y); // Даний метод забезпечує визначення координат
                                                // "розтягнутого" пікселя на "розмітці" і повертає координати
                                                // цього пікселя через out_x та out_y

    //=============================================================================================================//

    void saveSceneElements();                   // Функція проводить рендеринг сцени і зберігає зображення сцени
                                                // Зображення зберігається за адресом в QImage *Image
                                                // Надалі функція очищує вміст сцени і завантажує збережене зображення
                                                // на сцену. Таким чином програма зменшує кількість об'єктів на сцені
                                                // і відповідно зменшує витрати оперативної пам'яті

    //=============================================================================================================//

    void smallBrush(int X, int Y);              // Функція, що реалізовує маленький пензлик (2х2 пікселя)
    void bigBrush(int X, int Y);                // Функція, що реалізовує великий пензлик (3х3 пікселя)
    void whiteBrush(int X, int Y);              // Функція, що реалізовує ластик

    //=============================================================================================================//

    void setDataDir(QString path, int pictures);
    int getNumbOfImg();

    void createImgFoldersData(int quantity = 10);
    void createPixelsArray();

    void deleteImgFoldersData();
    void deletePixelsArray();

    QString getImgName(int numbDir);
    void getRGBfromImg(int proportion);

    void saveImage(QString path);

    // ////////////////////////////////////// //
    // На даний момент, методи не використовуються
    void convertImg();
    void setConvertedImage(QString path);
    // ////////////////////////////////////// //

    double** getPixelsArrayPtr();

    void getRGBfromImgForLearning(QString path);

    void setImageFromWindow();

    void setIsActive(bool var);
};
//----------paintScene----------//


#endif // PAINTSCENE_H
