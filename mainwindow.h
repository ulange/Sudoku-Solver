#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "box3x3widget.h"
#include "model.h"
#include "sudokuinimagedetector.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = 0 );
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    Box3x3Widget* boxes[3][3];

    Model* m_model;
    std::list<Model*> m_modelsToBeTried;
    SudokuInImageDetector* m_detector;
    int modelCounter;

    void updateView();

    void init();

    void removeFromRow( int value, int row );
    void removeFromCol( int value, int col );

    bool reducePossibilities();
    bool searchUniqueness();

    void update();

    bool solveAll();

public slots:
    void SLT_Box1x1Clicked( int row, int col );

protected slots:
    void on_actionLoad_triggered();
    void on_actionSave_triggered();
    void on_actionLoad_Image_triggered();
    void on_actionSolve_All_triggered();
    void on_actionSolve_Next_Step_triggered();
};

#endif // MAINWINDOW_H
