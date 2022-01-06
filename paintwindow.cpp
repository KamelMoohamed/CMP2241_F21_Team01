#include <QColorDialog>
#include <QFileDialog>
#include <QPropertyAnimation>
#include <QDebug>
#include <QSequentialAnimationGroup>
#include <QFontDatabase>
#include "paintwindow.h"
#include "ui_paintwindow.h"
#include "paintscene.h"
#include "json_utilities.h"
#include "painttable.h"



PaintWindow::PaintWindow(QWidget *parent) :
    QMainWindow(parent, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint),
    ui(new Ui::PaintWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    scene = new PaintScene(ui->graphicsView,ui->DataTable);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &PaintWindow::slotTimer);
    timer->start(100);

    QFontDatabase::addApplicationFont(":/fonts/resources/IROicons.otf");
    QFontDatabase::addApplicationFont(":/fonts/resources/Montserrat-Regular.ttf");




    // Styling of the upper buttons
    QString upperBtnStyleSheet="QPushButton {"
                   "background-color: #f9f9f9;"
                   "color: black;"
                   "font-family: Montserrat;"
                   "border-radius: 20px;"
                   "}"
                   "QPushButton:hover{"
                   "background-color: #e0e0e0;"
                   "}"
                   "QPushButton:focus{"
                   "background-color: black;"
                   "color: white;"
                   "}";


    // Styling of the side buttons
    QString sideBtnStyleSheet="QPushButton {"
                   "color: #a5aeaf;"
                   "border-radius: 10px;"
                   "text-align: left;"
                   "}"
                   "QPushButton:hover{"
                   "color: #6f6f6f;"
                   "}"
                   "QPushButton:pressed{"
                   "color: #000;"
                   ""
                   "}";

    QString tableBtnStyleSheet="QPushButton {"
                   "background-color: #f9f9f9;"
                   "color: black;"
                   "font-family: Montserrat;"
                   "border-radius: 20px;"
                   "}"
                   "QPushButton:hover{"
                   "background-color: #e0e0e0;"
                   "}"
                   "QPushButton:pressed{"
                   "background-color: black;"
                   "color: white;"
                   "}";




    // Assigning the stylesheet to the upper buttons
    ui->rectangleBtn->setStyleSheet(upperBtnStyleSheet);
    ui->lineBtn->setStyleSheet(upperBtnStyleSheet);
    ui->triangleBtn->setStyleSheet(upperBtnStyleSheet);
    ui->circleBtn->setStyleSheet(upperBtnStyleSheet);


    // Assigning the stylesheet to the side buttons
    ui->colorBtn->setStyleSheet(sideBtnStyleSheet);
    ui->redoBtn->setStyleSheet(sideBtnStyleSheet);
    ui->undoBtn->setStyleSheet(sideBtnStyleSheet);
    ui->saveBtn->setStyleSheet(sideBtnStyleSheet);
    ui->tableBtn->setStyleSheet(sideBtnStyleSheet);
    ui->ersr_Btn->setStyleSheet(sideBtnStyleSheet);

    // Assigning the stylesheet to the container table buttons
    ui->SortASBtn->setStyleSheet(tableBtnStyleSheet);
    ui->SortDSBtn->setStyleSheet(tableBtnStyleSheet);
    ui->menuNew->setStyleSheet(tableBtnStyleSheet);
    ui->menuOpen->setStyleSheet(tableBtnStyleSheet);
    ui->menuSave->setStyleSheet(tableBtnStyleSheet);




    // sets placeholder text to the search bar
    ui->searchBar->setPlaceholderText("Search");

    //assign embedded font to GUI
    ui->colorBtn->setFont(QFont("IROicons", 20));
    ui->redoBtn->setFont(QFont("IROicons", 20));
    ui->undoBtn->setFont(QFont("IROicons", 18));
    ui->saveBtn->setFont(QFont("IROicons", 19));
    ui->tableBtn->setFont(QFont("IROicons", 28));
    ui->ersr_Btn->setFont(QFont("IROicons", 22));
    ui->searchIcon->setFont(QFont("IROicons", 20));
    ui->closeBtn->setFont(QFont("IROicons", 21));
    ui->miniBtn->setFont(QFont("IROicons", 21));

    //assign embedded font to the GUI
    ui->redVal->setFont(QFont("Montserrat", 9));
    ui->blueVal->setFont(QFont("Montserrat", 9));
    ui->greenVal->setFont(QFont("Montserrat", 9));
    ui->hexVal->setFont(QFont("Montserrat", 9));

}


PaintWindow::~PaintWindow()
{
    delete ui;
}

void PaintWindow::slotTimer()
{
    timer->stop();
    scene->setSceneRect(0,0, ui->graphicsView->width() - 20, ui->graphicsView->height() - 20);
}

/*
 * Paint Window Buttons Clicked Functions
 */
void PaintWindow::on_triangleBtn_clicked()
{
    scene->setTypeFigure(PaintScene::TriangleType);
}


void PaintWindow::on_rectangleBtn_clicked()
{
    scene->setTypeFigure(PaintScene::RectangleType);
}


void PaintWindow::on_lineBtn_clicked()
{
    scene->setTypeFigure(PaintScene::LineType);
}


void PaintWindow::on_circleBtn_clicked()
{
    scene->setTypeFigure(PaintScene::CircleType);
}


void PaintWindow::on_undoBtn_clicked()
{
    if(!scene->undoStack->isClean()){
        if(scene->savingCounter == 0){
            scene->savingCounter++;
        }else{
            scene->savingCounter--;
        }
    }
    scene->undoStack->undo();

}


void PaintWindow::on_redoBtn_clicked()
{
    scene->undoStack->redo();
    if(!scene->undoStack->isClean())
        scene->savingCounter++;
}


void PaintWindow::on_colorBtn_clicked()
{
    QColor newColor = QColorDialog::getColor();
    if (newColor.isValid()){
        scene->setColor(newColor);
        ui->redVal->setText("R: " + QString::number(newColor.red()));
        ui->greenVal->setText("R: " + QString::number(newColor.green()));
        ui->blueVal->setText("R: " + QString::number(newColor.blue()));
        ui->hexVal->setText("HEX: " + newColor.name());
    }


}


void PaintWindow::on_saveBtn_clicked()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Save As"), "",
                tr("JSON (*.json);;PNG (*.png );; All Files (*)"),&selectedFilter);

    if (selectedFilter == "JSON (*.json)") {
        json_utilities::save(scene, fileName);
    }
    else if (selectedFilter == "PNG (*.png )") {
        json_utilities::savePNG(scene, fileName);
    }
}


void PaintWindow::on_verticalSlider_sliderMoved(int position)
{
    scene->setWeight(position);
    ui->lineWVal->setText("Line Weight: " + QString::number(position));

}


void PaintWindow::on_tableBtn_clicked()
{

    QPropertyAnimation *animCont = new QPropertyAnimation(ui->tableCont, "geometry");
    animCont->setDuration(400);
    animCont->setStartValue(ui->tableCont->geometry());

    if(ui->tableCont->geometry() == QRect(1220,149,321,571)){ // Check table Pos
        // if table is viewed out the view

        animCont->setEndValue(QRect(886,149,321,571));
    }
    else{
        // if table is viewed over the view
        animCont->setEndValue(QRect(1220,149,321,571));

    }
  animCont->start();

}

//Method for eraser button, the main idea is that it deletes the shape you're clicking.
void PaintWindow::on_ersr_Btn_clicked()
{
    scene->setTypeFigure(PaintScene::EraserType);
}


// Function for when the user clicks enter while on the search bar.
void PaintWindow::on_searchBar_returnPressed()
{
    //get the text from the search bar and place it into a string.
    QString srchTxt = ui->searchBar->text();

    //then we copy the Figure vector into a temporary vector,
    QVector<Figure*> *TempVec = scene->ItemsVec;

    /*
     * Then we search that vector trying to find a match for (srchTxt), hopefully we can!
     * complextity of find_if= N applications where N = std::distance(first, last).
    */
    auto it = std::find_if(TempVec->begin(), TempVec->end(), [=] (Figure* const& element) {
        return (element->name == srchTxt);
    });

    bool found = (it != TempVec->end());

    if(found==true){
        int index = std::distance(TempVec->begin(), it);

        // Get index of element from iterator
        ui->DataTable->selectRow(index);

        Figure* item= (*scene->ItemsVec)[index];
        PaintTable::UpdateInfoTable(ui->InfoTable,item);
        if  (ui->tableCont->geometry() == QRect(1220,149,321,571)) // Check tables visibiity
                             on_tableBtn_clicked();

    }
    else
        ui->DataTable->clearSelection();

}



void PaintWindow::open(QString path)
{
    json_utilities::open(scene,ui->DataTable,path);
}


void PaintWindow::on_DataTable_cellChanged(int row, int column)
{
    if (column!= 0)
        return;

    (*scene->ItemsVec)[row]->name = ui->DataTable->item(row,column)->text();

}


void PaintWindow::on_SortASBtn_clicked()
{
    PaintTable::SortAVec(scene->ItemsVec);
    PaintTable::UpdateTable(ui->DataTable,*scene->ItemsVec);
}


void PaintWindow::on_SortDSBtn_clicked()
{
    PaintTable::SortDVec(scene->ItemsVec);
    PaintTable::UpdateTable(ui->DataTable,*scene->ItemsVec);
}


void PaintWindow::mouseMoveEvent(QMouseEvent *event)
{
    if( event->buttons().testFlag(Qt::LeftButton) && mMoving){
        this->move(this->pos() + (event->pos() - mLastMousePosition));
    }
}


void PaintWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        mMoving = true;
        mLastMousePosition = event->pos();
    }
}


void PaintWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        mMoving = false;
    }
}


void PaintWindow::on_DataTable_cellClicked(int row, int column)
{
    Figure* item= (*scene->ItemsVec)[row];
    PaintTable::UpdateInfoTable(ui->InfoTable,item);
    Q_UNUSED(column)
}





void PaintWindow::on_logoBtn_clicked()
{
    QPropertyAnimation *animCont = new QPropertyAnimation(ui->menu, "geometry");
    animCont->setDuration(5);
    animCont->setStartValue(ui->tableCont->geometry());

    if(ui->menu->geometry() == QRect(50,69,121,111)){ // Check table Pos
        // if table is viewed out the view

        animCont->setEndValue(QRect(50,-115,121,111));
    }
    else{
        // if table is viewed over the view
        animCont->setEndValue(QRect(50,69,121,111));

    }
  animCont->start();
}


void PaintWindow::on_menuNew_clicked()
{
    messageDialog *s = new messageDialog();
    if(scene->savingCounter > 0){
        s->show();
        s->actionType = 2;
        s->exec();
    }

    if(s->exitCheck){
        delete this->scene->ItemsVec;
        delete this->scene->undoStack;
        scene->clear();
        this->scene->undoStack =new QUndoStack();
        this->scene->ItemsVec = new QVector<Figure*>();
        PaintTable::UpdateTable(scene->table, *scene->ItemsVec);
        Figure::countZero();
        scene->savingCounter = 0;
    }
}


void PaintWindow::on_menuOpen_clicked()
{
    messageDialog *s = new messageDialog();
    if(scene->savingCounter > 0){
        s->show();
        s->actionType = 1;
        s->exec();
    }

    if(s->exitCheck){
        delete this->scene->ItemsVec;
        delete this->scene->undoStack;
        scene->clear();
        this->scene->undoStack =new QUndoStack();
        this->scene->ItemsVec = new QVector<Figure*>();
        PaintTable::UpdateTable(scene->table, *scene->ItemsVec);
        Figure::countZero();
        scene->savingCounter = 0;

        QString path = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        "/c://",
                                                        tr("JSON (*.json)"));
        this->open(path);
    }

}


void PaintWindow::on_menuSave_clicked()
{
    on_saveBtn_clicked();
}

void PaintWindow:: on_menuSave(QString path){
    json_utilities::save(scene, path);
}

