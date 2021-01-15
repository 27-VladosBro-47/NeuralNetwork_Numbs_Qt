#include "paintscene.h"

//----------paintScene----------//
paintScene::paintScene(QGraphicsScene *parent) : QGraphicsScene(parent)
{

}

paintScene::~paintScene()
{

}

// Для рисування використовуємо події миші

void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if(isActiveMouse==1)
    {

    if(mouseButtonState==Qt::NoButton)
    {
        mouseButtonState =  event->button();
    }
    else
    {
        mouseButtonState=Qt::NoButton;
    }

    if(mouseButtonState==Qt::LeftButton)
    {
        int X, Y;

        findPos(event->scenePos().x(), event->scenePos().y(), X, Y);
        saveSceneElements();
        smallBrush(X, Y);

        // Зберігаємо координати точки, де була натиснута клавіша миші
        previousPoint = event->scenePos();
    }

    if(mouseButtonState==Qt::RightButton)
    {
        int X, Y;

        findPos(event->scenePos().x(), event->scenePos().y(), X, Y);
        saveSceneElements();
        whiteBrush(X, Y);

        // Зберігаємо координати точки, де була натиснута клавіша миші
        previousPoint = event->scenePos();
    }

    saveSceneElements();

    }
}

void paintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(isActiveMouse==1)
    {
        mouseButtonState =  Qt::NoButton;

        saveSceneElements();
    }
}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(isActiveMouse==1)
    {

        int currentX, currentY;
        currentX = event->scenePos().x();
        currentY = event->scenePos().y();

        PixelParameters *lineArr; // В даний динамічний масив будуть зберігатися
                                  // координати точок лінії, котру необхідно зафарбувати

        int arrSize;

        // Ініціалізація lineArr проходить всередині функції getPoints
        getPoints(previousPoint.x(), previousPoint.y(), currentX, currentY, lineArr, arrSize);


        int X, Y;
        if(mouseButtonState==Qt::LeftButton)
        {
            for(int i = 0; i < arrSize; i++)
            {
                findPos(lineArr[i].x, lineArr[i].y, X, Y);
                saveSceneElements();
                bigBrush(X,Y);
            }
            //addLine(previousPoint.x(),previousPoint.y(),currentX,currentY,QPen(Qt::yellow,2,Qt::SolidLine,Qt::RoundCap));
        }

        if(mouseButtonState==Qt::RightButton)
        {
            for(int i = 0; i < arrSize; i++)
            {
                findPos(lineArr[i].x, lineArr[i].y, X, Y);
                saveSceneElements();
                whiteBrush(X, Y);
            }
        }

        // Зберігаємо координати точки, де була натиснута клавіша миші
        //previousPoint = event->scenePos();
        previousPoint.setX(lineArr[arrSize-1].x);
        previousPoint.setY(lineArr[arrSize-1].y);

        saveSceneElements();

        delete [] lineArr;
    }
}

// Функція для розміщення графічного об'єкту на сцені
// Також встановлює значення для pixelMapWidth та pixelMapHeight
void paintScene::setImage(QString path)
{
    delete Image;
    delete pixmapImage;
    delete pixmapItemImage;

    pixmapImage =  new QPixmap();
    if(!pixmapImage->load(path))
    {
        QMessageBox::critical(0, "setImage", "Картинка не була завантажена");
    }
    pixelMapWidth = pixmapImage->size().width();
    pixelMapHeight = pixmapImage->size().height();

    Image = new QImage(graphicsViewWidth, graphicsViewHeight, QImage::Format_RGB32);
    *Image = pixmapImage->toImage();


    //delete pixmapImage;
    //pixmapImage =  new QPixmap();
    *pixmapImage = QPixmap::fromImage(*Image);

    *pixmapImage = pixmapImage->scaled(graphicsViewWidth, graphicsViewHeight, Qt::IgnoreAspectRatio);

    pixmapItemImage = new QGraphicsPixmapItem(*pixmapImage);

    addItem(pixmapItemImage);

}

// Перевантажена функція ! Для saveSceneElements !
void paintScene::setImage(QPixmap pixmap)
{
    // у даному методі не відбувається перезаписування Image
    // через те, що це відбувається в saveSceneElements()


    delete pixmapImage;
    // Не звільняємо пам'ять, через те, що в saveSceneElements
    // пам'ять вже була звільнена
    //delete pixmapItemImage;

    pixmapImage =  new QPixmap();
    *pixmapImage = pixmap;
    *pixmapImage = pixmapImage->scaled(graphicsViewWidth, graphicsViewHeight, Qt::IgnoreAspectRatio);

    pixmapItemImage = new QGraphicsPixmapItem(*pixmapImage);
    addItem(pixmapItemImage);
}

void paintScene::setImageFromWindow()
{
    delete Image;
    Image = new QImage(graphicsViewWidth, graphicsViewHeight, QImage::Format_RGB32);
    QPainter painter(Image);
    // Рендер сцени
    painter.setRenderHint(QPainter::Antialiasing);
    render(&painter, Image->rect(),QRectF(0,0,graphicsViewWidth,graphicsViewHeight), Qt::IgnoreAspectRatio);
    painter.end();
    // Завершення рендерування
    paintScene::clear();

    setImage(QPixmap::fromImage(*Image));


    /*
    delete Image;
    delete pixmapImage;
    delete pixmapItemImage;

    pixmapImage =  new QPixmap();
    if(!pixmapImage->load(path))
    {
        QMessageBox::critical(0, "Backgroung", "Картинка заднього фону не була завантажена");
    }
    pixelMapWidth = pixmapImage->size().width();
    pixelMapHeight = pixmapImage->size().height();

    Image = new QImage(graphicsViewWidth, graphicsViewHeight, QImage::Format_RGB32);
    *Image = pixmapImage->toImage();


    //delete pixmapImage;
    //pixmapImage =  new QPixmap();
    *pixmapImage = QPixmap::fromImage(*Image);

    *pixmapImage = pixmapImage->scaled(graphicsViewWidth, graphicsViewHeight, Qt::IgnoreAspectRatio);

    pixmapItemImage = new QGraphicsPixmapItem(*pixmapImage);

    addItem(pixmapItemImage);
    */

}


// Функція, яка записує розмір картинки, що буде передаватися сцені
void paintScene::setImgSize(int width, int height)
{
    pixelMapWidth = width;
    pixelMapHeight = height;
}

// Функція, яка записує розмір віджета GraphicsView
void paintScene::setGraphicsViewSize(qreal width, qreal height)
{
    graphicsViewWidth = static_cast<int>(width);
    graphicsViewHeight = static_cast<int>(height);
}

// Даний метод забезпечує визначення координат
// "розтягнутого" пікселя на "розмітці" і повертає координати
// цього пікселя через out_x та out_y
void paintScene::findPos(qreal X, qreal Y, int &out_x, int &out_y)
{
    int x = static_cast<int>(X);
    int y = static_cast<int>(Y);

    int sizePixelW = graphicsViewWidth / pixelMapWidth;
    int sizePixelH = graphicsViewHeight / pixelMapHeight;

    out_x = out_y = 0;

    out_x = x/sizePixelW;
    out_y = y/sizePixelH;
}

// Даний метод забезечує
// отримання координат лінії. Дані координати, після обробки findPos
// будуть використовуватися для рисування пікселів на сцені
void paintScene::getPoints(int x0, int y0, int x1, int y1, PixelParameters* &lineArr, int &arrSize)
{
    //int x1, y1;
    //x1 = static_cast<int>(X1);
    //y1 = static_cast<int>(Y1);

    double dx, dy;
    dx = fabs(x1 - x0);     // Відстань між точками х0 та х2, яка спроектована на вісь 0X
    dy = fabs(y1 - y0);     // Відстань між точками х0 та х2, яка спроектована на вісь 0Y


    // Надалі алгоритм визначає, яка сама пряма була передана у функцію:
    // точка, пряма пропорційна одній із осей, чи звичайна пряма
    // У разі, якщо це звичайна пряма
    // Також визначається тангенс кута між прямою і віссю координат 0X.
    // Якщо tg<=1, обчислення координат лінії відбувається відносно осі 0X (dx)
    // Якщо tg>1, обчислення координат лінії відбувається відносно осі 0Y (dy)
    // Визначається у якій чверті координат лежить пряма відносно початку прямої.
    if(dx==0 || dy==0)
    {
        if(dx == 0 && dy == 0)      // Якщо лінія - це точка
        {
            lineArr = new PixelParameters[1];
            arrSize = 1;

            lineArr[0].x = x0;
            lineArr[0].y = y0;
        }
        else
        {
            if(dx == 0)             // Якщо лінія - це пряма, що паралельна осі 0Y
            {
                lineArr = new PixelParameters[(int)dy];
                arrSize = dy;

                if( (y0 - y1) > 0 ) // Лінія направлена вверх
                {
                    for(int i = 0; i < arrSize; i++)
                    {
                        lineArr[i].x = x0;
                        lineArr[i].y = y0 - i;
                    }
                }
                else                // Лінія направлена вниз
                {
                    for(int i = 0; i < arrSize; i++)
                    {
                        lineArr[i].x = x0;
                        lineArr[i].y = y0 + i;
                    }
                }

            }
            if(dy == 0)             // Якщо лінія - це пряма, що паралельна осі 0X
            {
                lineArr = new PixelParameters[(int)dx];
                arrSize = dx;

                if( (x0 - x1) > 0 ) // Лінія направлена ліворуч
                {
                    for(int i = 0; i < arrSize; i++)
                    {
                        lineArr[i].x = x0 - i;
                        lineArr[i].y = y0;
                    }
                }
                else                // Лінія направлена праворуч
                {
                    for(int i = 0; i < arrSize; i++)
                    {
                        lineArr[i].x = x0 + i;
                        lineArr[i].y = y0;
                    }
                }
            }
        }
    }
    else                    // Довільна лінія
    {
        // В старому варіанті функції ці значення обраховувалися кожен раз по новій
        int x0_x1, y0_y1;
        x0_x1 = x0-x1;
        y0_y1 = y0-y1;

        double dy_dx, dx_dy;
        dy_dx = dy/dx;
        dx_dy = dx/dy;


        if( (dy_dx <= 1) )  // tg<=1
        {
            lineArr = new PixelParameters[(int)dx];
            arrSize = dx;

            // I чверть
            if(x0_x1 < 0 && y0_y1 > 0)
            {
                for(int i = 0; i < arrSize; i++)
                {
                    lineArr[i].x = x0 + i;
                    lineArr[i].y = round(y0 - (dy_dx*i));
                }
            }
            // II чверть
            if(x0_x1 > 0 && y0_y1 > 0)
            {
                for(int i = 0; i < arrSize; i++)
                {
                    lineArr[i].x = x0 - i;
                    lineArr[i].y = round(y0 - (dy_dx*i));
                }
            }
            // III чверть
            if(x0_x1 > 0 && y0_y1 < 0)
            {
                for(int i = 0; i < arrSize; i++)
                {
                    lineArr[i].x = x0 - i;
                    lineArr[i].y = round(y0 + (dy_dx*i));
                }
            }
            // VI чверть
            if(x0_x1 < 0 && y0_y1 < 0)
            {
                for(int i = 0; i < arrSize; i++)
                {
                    lineArr[i].x = x0 + i;
                    lineArr[i].y = round(y0 + (dy_dx*i));
                }
            }
        }
        else                // tg>1
        {
            lineArr = new PixelParameters[(int)dy];
            arrSize = dy;


            // I чверть
            if(x0_x1 < 0 && y0_y1 > 0)
            {
                for(int i = 0; i < arrSize; i++)
                {
                    lineArr[i].y = y0 - i;
                    lineArr[i].x = round(x0 + (dx_dy*i));
                }
            }
            // II чверть
            if(x0_x1 > 0 && y0_y1 > 0)
            {
                for(int i = 0; i < arrSize; i++)
                {
                    lineArr[i].y = y0 - i;
                    lineArr[i].x = round(x0 - (dx_dy*i));
                }
            }
            // III чверть
            if(x0_x1 > 0 && y0_y1 < 0)
            {
                for(int i = 0; i < arrSize; i++)
                {
                    lineArr[i].y = y0 + i;
                    lineArr[i].x = round(x0 - (dx_dy*i));
                }
            }
            // VI чверть
            if(x0_x1 < 0 && y0_y1 < 0)
            {
                for(int i = 0; i < arrSize; i++)
                {
                    lineArr[i].y = y0 + i;
                    lineArr[i].x = round(x0 + (dx_dy*i));
                }
            }
        }
    }
}

// Функція проводить рендеринг сцени і зберігає зображення сцени
// Зображення зберігається за адресом в QImage *Image
// Надалі функція очищує вміст сцени і завантажує збережене зображення
// на сцену. Таким чином програма зменшує кількість об'єктів на сцені
// і відповідно зменшує витрати оперативної пам'яті

void paintScene::saveSceneElements()
{

    delete Image;
    Image = new QImage(graphicsViewWidth, graphicsViewHeight, QImage::Format_RGB32);
    QPainter painter(Image);
    // Рендер сцени
    painter.setRenderHint(QPainter::Antialiasing);
    render(&painter, Image->rect(),QRectF(0,0,graphicsViewWidth,graphicsViewHeight), Qt::IgnoreAspectRatio);
    painter.end();
    // Завершення рендерування
    paintScene::clear();

    //getRGBfromImg();

    setImage(QPixmap::fromImage(*Image));

    //out_image->save("test_img.png");

    /*
    Image = new QImage(graphicsViewWidth, graphicsViewHeight, QImage::Format_RGB32);
    QPainter painter(&(*Image));
    painter.setRenderHint(QPainter::Antialiasing);
    render(&painter);
    Image = painter.to
    */

}

// Функція, що реалізовує великий пензлик (3х3 пікселя)
void paintScene::bigBrush(int X, int Y)
{
    int buffX, buffY;

    int r = 0;
    int g = 0;
    int b = 0;
    int alpha = 0;

    int pixelSizeWidth = graphicsViewWidth/pixelMapWidth;
    int pixelSizeHeight = graphicsViewHeight/pixelMapHeight;

    //------------------------------//
    // Центральний піксель
    {
    buffX = X*pixelSizeWidth;
    buffY = Y*pixelSizeHeight;
    addRect(buffX,
           buffY,
          pixelSizeWidth,
          pixelSizeHeight,
          QPen(Qt::NoPen), QBrush(Qt::white));
    }
    //------------------------------//


    //------------------------------//
    // Лівий піксель
    {
    buffX = (X-1)*pixelSizeWidth;
    buffY = Y*pixelSizeHeight;
    Image->pixelColor(buffX,buffY).getRgb(&r, &g, &b, &alpha);
    if(r>0)
    {
        if(r<245)
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(QColor(r + 5,r + 5,r + 5)));
        }
        else
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(Qt::white));
        }
    }
    else
    {
        addRect(buffX,
              buffY,
              pixelSizeWidth,
              pixelSizeHeight,
              //QPen(Qt::NoPen), QBrush(QColor(130,130,130)));
                QPen(Qt::NoPen), QBrush(QColor(70,70,70)));
    }
    }
    //------------------------------//

    /*
    //------------------------------//
    // Лівий верхній піксель
    {
    buffX = (X-1)*pixelSizeWidth;
    buffY = (Y-1)*pixelSizeHeight;
    Image->pixelColor(buffX,buffY).getRgb(&r, &g, &b, &alpha);
    if(r>0)
    {
        if(r<245)
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(QColor(r + 5,r + 5,r + 5)));
        }
        else
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(Qt::white));
        }
    }
    else
    {
        addRect(buffX,
              buffY,
              pixelSizeWidth,
              pixelSizeHeight,
              QPen(Qt::NoPen), QBrush(QColor(145,145,145)));
    }
    }
    //------------------------------//
    */

    //------------------------------//
    // Верхній піксель
    {
    buffX = X*pixelSizeWidth;
    buffY = (Y-1)*pixelSizeHeight;
    Image->pixelColor(buffX,buffY).getRgb(&r, &g, &b, &alpha);
    if(r>0)
    {
        if(r<245)
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(QColor(r + 5,r + 5,r + 5)));
        }
        else
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(Qt::white));
        }
    }
    else
    {
        addRect(buffX,
              buffY,
              pixelSizeWidth,
              pixelSizeHeight,
              //QPen(Qt::NoPen), QBrush(QColor(130,130,130)));
                QPen(Qt::NoPen), QBrush(QColor(90,90,90)));

    }
    }
    //------------------------------//

    /*
    //------------------------------//
    // Правий верхній піксель
    {
    buffX = (X+1)*pixelSizeWidth;
    buffY = (Y-1)*pixelSizeHeight;
    Image->pixelColor(buffX,buffY).getRgb(&r, &g, &b, &alpha);
    if(r>0)
    {
        if(r<245)
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(QColor(r + 5,r + 5,r + 5)));
        }
        else
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(Qt::white));
        }
    }
    else
    {
        addRect(buffX,
              buffY,
              pixelSizeWidth,
              pixelSizeHeight,
              QPen(Qt::NoPen), QBrush(QColor(135,135,135)));
    }
    }
    //------------------------------//
    */

    //------------------------------//
    // Правий піксель
    {
    buffX = (X+1)*pixelSizeWidth;
    buffY = Y*pixelSizeHeight;
    Image->pixelColor(buffX,buffY).getRgb(&r, &g, &b, &alpha);
    if(r>0)
    {
        if(r<245)
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(QColor(r + 5,r + 5,r + 5)));
        }
        else
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(Qt::white));
        }
    }
    else
    {
        addRect(buffX,
              buffY,
              pixelSizeWidth,
              pixelSizeHeight,
              //QPen(Qt::NoPen), QBrush(QColor(115,115,115)));
                QPen(Qt::NoPen), QBrush(QColor(100,100,100)));
    }
    }
    //------------------------------//

    /*
    //------------------------------//
    // Правий нижній піксель
    {
    buffX = (X+1)*pixelSizeWidth;
    buffY = (Y+1)*pixelSizeHeight;
    Image->pixelColor(buffX,buffY).getRgb(&r, &g, &b, &alpha);
    if(r>0)
    {
        if(r<245)
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(QColor(r + 5,r + 5,r + 5)));
        }
        else
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(Qt::white));
        }
    }
    else
    {
        addRect(buffX,
              buffY,
              pixelSizeWidth,
              pixelSizeHeight,
              QPen(Qt::NoPen), QBrush(QColor(125,125,125)));
    }
    }
    //------------------------------//
    */

    //------------------------------//
    // Нижній піксель
    {
    buffX = X*pixelSizeWidth;
    buffY = (Y+1)*pixelSizeHeight;
    Image->pixelColor(buffX,buffY).getRgb(&r, &g, &b, &alpha);
    if(r>0)
    {
        if(r<245)
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(QColor(r + 5,r + 5,r + 5)));
        }
        else
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(Qt::white));
        }
    }
    else
    {
        addRect(buffX,
              buffY,
              pixelSizeWidth,
              pixelSizeHeight,
              //QPen(Qt::NoPen), QBrush(QColor(110,110,110)));
                QPen(Qt::NoPen), QBrush(QColor(85,85,85)));
    }
    }
    //------------------------------//

    /*
    //------------------------------//
    // Лівий нижній піксель
    {
    buffX = (X-1)*pixelSizeWidth;
    buffY = (Y+1)*pixelSizeHeight;
    Image->pixelColor(buffX,buffY).getRgb(&r, &g, &b, &alpha);
    if(r>0)
    {
        if(r<245)
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(QColor(r + 5,r + 5,r + 5)));
        }
        else
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(Qt::white));
        }
    }
    else
    {
        addRect(buffX,
              buffY,
              pixelSizeWidth,
              pixelSizeHeight,
              QPen(Qt::NoPen), QBrush(QColor(140,140,140)));
    }
    }
    //------------------------------//
    */
}

// Функція, що реалізовує маленький пензлик (2х2 пікселя)
void paintScene::smallBrush(int X, int Y)
{
    int buffX, buffY;

    int r = 0;
    int g = 0;
    int b = 0;
    int alpha = 0;

    int pixelSizeWidth = graphicsViewWidth/pixelMapWidth;
    int pixelSizeHeight = graphicsViewHeight/pixelMapHeight;

    //------------------------------//
    // Центральний піксель
    {
    buffX = X*pixelSizeWidth;
    buffY = Y*pixelSizeHeight;
    addRect(buffX,
           buffY,
          pixelSizeWidth,
          pixelSizeHeight,
          QPen(Qt::NoPen), QBrush(Qt::white));
    }
    //------------------------------//


    //------------------------------//
    // Правий піксель
    {
    buffX = (X+1)*pixelSizeWidth;
    buffY = Y*pixelSizeHeight;
    Image->pixelColor(buffX,buffY).getRgb(&r, &g, &b, &alpha);
    if(r>0)
    {
        if(r<245)
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(QColor(r + 5,r + 5,r + 5)));
        }
        else
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(Qt::white));
        }
    }
    else
    {
        addRect(buffX,
              buffY,
              pixelSizeWidth,
              pixelSizeHeight,
              QPen(Qt::NoPen), QBrush(QColor(220,220,220)));
    }
    }
    //------------------------------//


    //------------------------------//
    // Правий нижній піксель
    {
    buffX = (X+1)*pixelSizeWidth;
    buffY = (Y+1)*pixelSizeHeight;
    Image->pixelColor(buffX,buffY).getRgb(&r, &g, &b, &alpha);
    if(r>0)
    {
        if(r<245)
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(QColor(r + 5,r + 5,r + 5)));
        }
        else
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(Qt::white));
        }
    }
    else
    {
        addRect(buffX,
              buffY,
              pixelSizeWidth,
              pixelSizeHeight,
              QPen(Qt::NoPen), QBrush(QColor(175,175,175)));
    }

    }
    //------------------------------//


    //------------------------------//
    // Нижній піксель
    {
    buffX = X*pixelSizeWidth;
    buffY = (Y+1)*pixelSizeHeight;
    Image->pixelColor(buffX,buffY).getRgb(&r, &g, &b, &alpha);
    if(r>0)
    {
        if(r<245)
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(QColor(r + 5,r + 5,r + 5)));
        }
        else
        {
            addRect(buffX,
                  buffY,
                  pixelSizeWidth,
                  pixelSizeHeight,
                  QPen(Qt::NoPen), QBrush(Qt::white));
        }
    }
    else
    {
        addRect(buffX,
              buffY,
              pixelSizeWidth,
              pixelSizeHeight,
              QPen(Qt::NoPen), QBrush(QColor(200,200,200)));
    }
    }
    //------------------------------//
}

// Функція, що реалізовує ластик
void paintScene::whiteBrush(int X, int Y)
{
    int buffX, buffY;

    int pixelSizeWidth = graphicsViewWidth/pixelMapWidth;
    int pixelSizeHeight = graphicsViewHeight/pixelMapHeight;

    buffX = X*pixelSizeWidth;
    buffY = Y*pixelSizeHeight;

    addRect(buffX,buffY,pixelSizeWidth,pixelSizeHeight,QPen(Qt::NoPen), QBrush(Qt::black));
    addRect(buffX-pixelSizeWidth,buffY,pixelSizeWidth,pixelSizeHeight,QPen(Qt::NoPen), QBrush(Qt::black));
    addRect(buffX,buffY-pixelSizeHeight,pixelSizeWidth,pixelSizeHeight,QPen(Qt::NoPen), QBrush(Qt::black));
    addRect(buffX+pixelSizeWidth,buffY,pixelSizeWidth,pixelSizeHeight,QPen(Qt::NoPen), QBrush(Qt::black));
    addRect(buffX,buffY+pixelSizeHeight,pixelSizeWidth,pixelSizeHeight,QPen(Qt::NoPen), QBrush(Qt::black));

}

void paintScene::createImgFoldersData(int quantity)
{
    if(currentImg!=nullptr)
    {
        delete [] currentImg;
    }

    sizeCurrentImg = quantity;
    currentImg = new int[quantity];
    for(int i = 0; i < quantity; i++)
    {
        // ПРИМІТКА: ініціалізація масива значенням "2" обґрунтовано тим, що при використанні
        // функції QFileInfoList, перші два значення - це посилання на поточну і попередню папки
        currentImg[i] = 2;
    }
}

void paintScene::deleteImgFoldersData()
{
    delete [] currentImg;
    sizeCurrentImg = 0;
}

void paintScene::createPixelsArray()
{
    pixelsArray = new double*[pixelMapHeight];
    for(int i = 0; i < pixelMapWidth; i++)
    {
        pixelsArray[i] = new double[pixelMapWidth];
    }
}

void paintScene::deletePixelsArray()
{
    for(int i = 0; i < pixelMapWidth; i++)
    {
        delete [] pixelsArray[i];
    }

    delete [] pixelsArray;
}

QString paintScene::getImgName(int numbDir)
{
    //numbOfCurrentImg = numbDir;

    if(numbDir>sizeCurrentImg)
    {
        QMessageBox::critical(0,"paintScene::getImgName", "Не коректний номер папки із зображеннями\n(numb_dir>size_current_img)");
    }

    // Утворюю повний шлях до папки з зображеннями
    QString str = dataDir + '/' + QString::number(numbDir);
    //qDebug() << str;

    // Створюю об'єкт для роботи з папкою із зображеннями
    QDir *directory = new QDir(str);
    // Отримую дані про файли, що містяться у теці
    QFileInfoList infoListImg = directory->entryInfoList();

    // Якщо були використані всі зображення - використовую перше зображення
    if(currentImg[numbDir] >= infoListImg.size()-1)
    {
        currentImg[numbDir] = 2;
    }

    // Отримую повний шлях до файла
    str+='/'+infoListImg[currentImg[numbDir]].fileName();
    // Змінюю номер останього використаного зображення
    ++currentImg[numbDir];
    //infoListImg.size();       // кількість файлів в директорії

    delete directory;
    return str;
}

void paintScene::getRGBfromImg(int proportion)
{
    //int pixelWidth = graphicsViewWidth/pixelMapWidth;
    //int pixelHeight= graphicsViewHeight/pixelMapHeight;

    //qDebug() << "======================================";
    for(int i = 0; i < pixelMapHeight; i++)
    {
        //QDebug aaa = qDebug();
        for(int j = 0; j < pixelMapWidth; j++)
        {
            Image->pixelColor((j*proportion),(i*proportion)).getRgb(&rgb.r, &rgb.g, &rgb.b, &rgb.alpha);
            pixelsArray[i][j] = static_cast<double>(rgb.r)/255.0;
            //aaa << "p[" << i << "][" << j << "]=" << pixelsArray[i][j] << " ";
        }
        //qDebug() = aaa;
        //qDebug() << "-----------------------------------";
    }
    //qDebug() << "======================================";
}

void paintScene::getRGBfromImgForLearning(QString path)
{
    QPixmap *pixmapImageBuff =  new QPixmap();
    if(!pixmapImageBuff->load(path))
    {
        QMessageBox::critical(0, "Backgroung", "Картинка заднього фону не була завантажена");
    }
    int pixelMapWidthBuff = pixmapImageBuff->size().width();
    int pixelMapHeightBuff = pixmapImageBuff->size().height();

    QImage *ImageBuff = new QImage(pixelMapWidthBuff, pixelMapHeightBuff, QImage::Format_RGB32);
    *ImageBuff = pixmapImageBuff->toImage();

    for(int i = 0; i < pixelMapHeightBuff; i++)
    {
        for(int j = 0; j < pixelMapWidthBuff; j++)
        {
            ImageBuff->pixelColor(j,i).getRgb(&rgb.r, &rgb.g, &rgb.b, &rgb.alpha);
            pixelsArray[i][j] = static_cast<double>(rgb.r)/255.0;
        }
    }

}

// Встановлюю шлях до папок з картинками
void paintScene::setDataDir(QString path, int pictures)
{
    numbImg = pictures;
    dataDir = path;
}

// Встановлюємо загальну кількість картинок із навчальних тек
int paintScene::getNumbOfImg()
{
    return numbImg;
}

void paintScene::saveImage(QString path)
{
    saveSceneElements();
    setImageFromWindow();
    getRGBfromImg(10);
    double** arr = getPixelsArrayPtr();

    QImage *img = new QImage(pixelMapWidth, pixelMapHeight, QImage::Format_RGB32);

    for(int i = 0; i < pixelMapHeight; i++)
    {
        for(int j = 0; j < pixelMapWidth; j++)
        {
            img->setPixelColor(j,i,QColor(255*arr[i][j],255*arr[i][j],255*arr[i][j]));

        }
    }

    qDebug() << path;
    img->save(path);

    delete img;
}

// ////////////////////////////////////// //
// На даний момент, методи не використовуються
void paintScene::convertImg()
{
    for(int i = 0; i < pixelMapHeight; i++)
    {
        for(int j = 0; j < pixelMapWidth; j++)
        {
            Image->pixelColor(j,i).getRgb(&rgb.r, &rgb.g, &rgb.b, &rgb.alpha);
            Image->setPixel(j,i, qRgb(255 - rgb.r, 255 - rgb.g, 255 - rgb.b));
        }
    }
}
void paintScene::setConvertedImage(QString path)
{
    delete Image;
    delete pixmapImage;
    delete pixmapItemImage;

    pixmapImage =  new QPixmap();
    if(!pixmapImage->load(path))
    {
        QMessageBox::critical(0, "paintScene::setConvertedImage", "Картинка не була завантажена");
    }

    pixelMapWidth = pixmapImage->size().width();
    pixelMapHeight = pixmapImage->size().height();

    Image = new QImage(graphicsViewWidth, graphicsViewHeight, QImage::Format_RGB32);
    *Image = pixmapImage->toImage();

    convertImg();

    //delete pixmapImage;
    //pixmapImage =  new QPixmap();
    *pixmapImage = QPixmap::fromImage(*Image);

    *pixmapImage = pixmapImage->scaled(graphicsViewWidth, graphicsViewHeight, Qt::IgnoreAspectRatio);

    pixmapItemImage = new QGraphicsPixmapItem(*pixmapImage);
    addItem(pixmapItemImage);

}
// ////////////////////////////////////// //

double** paintScene::getPixelsArrayPtr()
{
    return pixelsArray;
}

void paintScene::setIsActive(bool var)
{
    isActiveMouse = var;
}

//----------paintScene----------//
