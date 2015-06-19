#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QThread>
#include <QApplication>

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    QWidget* centralWidget = new QWidget;
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing( 0 );
    centralWidget->setLayout( gridLayout );
    setCentralWidget( centralWidget );

    for( unsigned int row = 0; row < 3; ++row )
    {
        for( unsigned int col = 0; col < 3; ++col )
        {
            Box3x3Widget* currentBox = new Box3x3Widget( this );
            boxes[row][col] = currentBox;
            currentBox->setCoordinates( row, col );
            gridLayout->addWidget( currentBox, row, col, 1, 1 );
        }
    }

    m_model = new Model();

    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    m_model->init();
    m_modelsToBeTried.clear();
    modelCounter = 0;
    updateView();
}

bool MainWindow::solveAll()
{
    while( m_model->solveOne() && m_model->verification() )
    {
        update();
        qApp->processEvents();
    }

    if( m_model->isCompleted() )
    {
        qDebug() << "Finished!";
        return true;
    }

    // Not completed: SPLIT ---------------------------------------------
    if( m_model->verification() )
    {
        qDebug() << "Model is not completed. - trying out possibilities.";

        //Search for boxes with only 2 (or at least the fewest) possibilities
        int minPossibilities = 9;
        int minRow = 0;
        int minCol = 0;
        for( int row = 0; row < ROWS; ++row )
        {
            for( int col = 0; col < COLS; ++col )
            {
                int currentPossibilitiesAmount = (int) m_model->getPossibilities( row, col ).size();
                int currentValue = m_model->getValue( row, col );
                if( currentValue == 0 && currentPossibilitiesAmount < minPossibilities )
                {
                    minPossibilities = currentPossibilitiesAmount;
                    minRow = row;
                    minCol = col;
                }
            }
        }

        qDebug() << "Trying from box with row:" << minRow << ", col:" << minCol;

        //Copy model to have A and B
        for( int i = 1; i < minPossibilities; ++i )
        {
            //Remember B
            Model* modelB = new Model( m_model ); //modelB->print();
            int currentPossibility = m_model->getPossibilities( minRow, minCol ).at( i );
            modelB->setValue( currentPossibility, minRow, minCol );
            m_modelsToBeTried.push_back( modelB );
            ++modelCounter;
        }

        int value = m_model->getPossibilities( minRow, minCol ).at( 0 );
        qDebug() << "Trying from box with row:" << minRow << ", col:" << minCol << "with value" << value;
        //Start with A (possibility 0)
        m_model->setValue( value, minRow, minCol );
        update();
        qApp->processEvents();

        solveAll();

    }
    else //If completed -> finished (or also try B)
    {
        //Contradiction -> try out other model from m_modelsToBeTried
        if( m_modelsToBeTried.empty() )
        {
            QMessageBox::critical( this, "Model", "No possible model left!");
            return false;
        }
        else
        {
            delete m_model;
            m_model = m_modelsToBeTried.front();
            m_modelsToBeTried.pop_front();
            solveAll();
        }
    }

    return true;
}

void MainWindow::update()
{
    qDebug() << "update";

    //Rerun if changes happened
    if( reducePossibilities() )
    {
        update();
    }

    //Rerun if changes happened
    if( searchUniqueness() )
    {
        update();
    }

    //Redraw the view in the end
    updateView();

    if( m_model->verification() )
    {
        qDebug() << "Verification successful!";
    }
    else
    {
        //QMessageBox::critical( this, "Verification", "Verification was not successful!");
    }

    if( m_model->isCompleted() )
    {
        QMessageBox::information( this, "Information", "Sudoku completed!\n Using " + QString::number( modelCounter ) + " guesses." );
    }
}

void MainWindow::SLT_Box1x1Clicked( int row, int col )
{
    qDebug() << "Mouse clicked at " << row << ", " << col;

    int solution = m_model->getSolution( row, col );
    if( solution != 0 )
    {
        //Found solution is used
        //Rerun the search for possibilities
        m_model->setValue( solution, row, col );

        update();
    }
}

void MainWindow::on_actionLoad_triggered()
{
    QString fileName = QFileDialog::getOpenFileName( this,
        tr( "Open Sudoku File" ), "", tr( "Sudoku Files (*.txt)" ) );

    QFile file( fileName );
    if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        return;
    }

    setWindowTitle( "Sudoku Solver - " + fileName );
    init();


    int row = 0;
    while( !file.atEnd() )
    {
        QString line = file.readLine();
        QStringList list = line.split( "," );

        for( int col = 0; col < list.size(); ++col )
        {
            m_model->setValue( list.at( col ).toInt(), row, col );
        }

        row++;
    }

    update();
}

void MainWindow::on_actionLoad_Image_triggered()
{
    QString fileName = QFileDialog::getOpenFileName( this,
        tr( "Open Image File" ), "", tr( "Image Files (*.jpg)" ) );

    //m_detector->fillModelFromImage( m_model, fileName );
    update();
}

bool MainWindow::reducePossibilities()
{
    bool returnValue = false;

    for( int row = 0; row < ROWS; ++row )
    {
        for( int col = 0; col < COLS; ++col )
        {
            //If value is set ( != 0 )
            int value = m_model->getValue( row, col );
            if( value != 0 )
            {
                returnValue |= m_model->removePossibilityFromRow( value, row );
                returnValue |= m_model->removePossibilityFromCol( value, col );
                returnValue |= m_model->removePossibilityFromBox3x3( value, row, col );
            }
        }
    }

    returnValue |= m_model->findTwoPairsInRows();
    returnValue |= m_model->findTwoPairsInColumns();
    returnValue |= m_model->findTwoPairsInBoxes3x3();

    returnValue |= m_model->findLinesInBoxes3x3();

    return returnValue;
}

void MainWindow::on_actionSolve_All_triggered()
{
    qDebug() << "Solve All";

    solveAll();

    if( !m_model->isCompleted() )
    {
        QMessageBox::critical( this, "Error", "Could not solve all!" );
    }
}



void MainWindow::on_actionSolve_Next_Step_triggered()
{
    qDebug() << "Solve All";

    if( m_model->solveOne() )
    {
        update();
    }
    else
    {
        QMessageBox::critical( this, "Error", "Could not solve next step!" );
    }
}

void MainWindow::updateView()
{
    //Show value page of value is set (!= 0)
    for( int row = 0; row < ROWS; ++row )
    {
        for( int col = 0; col < COLS; ++col )
        {
            int value = m_model->getValue( row, col );
            std::vector<int> possibilities = m_model->getPossibilities( row, col );
            int solution = m_model->getSolution( row, col );

            int boxRow = row / 3;
            int boxCol = col / 3;
            Box1x1Widget* currentBox1x1 = boxes[boxRow][boxCol]->getBox1x1( row % 3, col % 3 );
            if( value != 0 )
            {
                currentBox1x1->setValue( value );
                currentBox1x1->showValuePage();
            }
            else
            {
                currentBox1x1->showPossibilitiesPage();
                currentBox1x1->updatePossibilities( possibilities );
                currentBox1x1->updateSolution( solution );
            }
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName( this, tr( "Save File" ),
                               "", tr( "Sudoku Files (*.txt)" ) );

    QFile file( fileName );
    if( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
    {
        return;
    }

    QTextStream out( &file );
    for( unsigned int row = 0; row < 9; ++row )
    {
        for( unsigned int col = 0; col < 9; ++col )
        {
            int value = m_model->getValue( row, col );
            if( value == 0 )
            {
                out << " ";
            }
            else
            {
                out << value;
            }

            if( col < 8 )
            {
                out << ",";
            }
        }
        if( row < 8 )
        {
            out << "\n";
        }
    }
}

bool MainWindow::searchUniqueness()
{
    bool returnValue = false;

    //Search for uniqueness (display as fat edges)
    // - in a block (3x3)
    returnValue |= m_model->searchUniquenessInBox3x3();
    // - in a row
    returnValue |= m_model->searchUniquenessInRows();
    // - in a column
    returnValue |= m_model->searchUniquenessInCols();

    return returnValue;
}


