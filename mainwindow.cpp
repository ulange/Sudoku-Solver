#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>

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

    updateView();
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


