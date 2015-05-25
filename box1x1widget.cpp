#include "box1x1widget.h"
#include "ui_box.h"
#include <QDebug>

Box1x1Widget::Box1x1Widget( QWidget* parent ) :
    QFrame( parent ),
    ui( new Ui::Box )
{
    ui->setupUi( this );
    setFrameStyle( QFrame::Box );

    m_boldText = QFont( "Times", 8, QFont::Bold );
    m_normalText = QFont( "Times", 8, QFont::Light );
}

Box1x1Widget::~Box1x1Widget()
{

}

void Box1x1Widget::setValue( int value )
{
    ui->labelValue->setText( QString::number( value ) );
}

void Box1x1Widget::showValuePage()
{
    ui->stackedWidget->setCurrentIndex( 1 );
}

void Box1x1Widget::showPossibilitiesPage()
{
    ui->stackedWidget->setCurrentIndex( 0 );
}

void Box1x1Widget::updatePossibilities( std::vector<int> p )
{
    //qDebug() << "updatePossibilities at " << m_row << ", " << m_col;
    std::find( p.begin(), p.end(), 1 ) != p.end() ? ui->label1->setText( "1" ) : ui->label1->setText( "" );
    std::find( p.begin(), p.end(), 2 ) != p.end() ? ui->label2->setText( "2" ) : ui->label2->setText( "" );
    std::find( p.begin(), p.end(), 3 ) != p.end() ? ui->label3->setText( "3" ) : ui->label3->setText( "" );
    std::find( p.begin(), p.end(), 4 ) != p.end() ? ui->label4->setText( "4" ) : ui->label4->setText( "" );
    std::find( p.begin(), p.end(), 5 ) != p.end() ? ui->label5->setText( "5" ) : ui->label5->setText( "" );
    std::find( p.begin(), p.end(), 6 ) != p.end() ? ui->label6->setText( "6" ) : ui->label6->setText( "" );
    std::find( p.begin(), p.end(), 7 ) != p.end() ? ui->label7->setText( "7" ) : ui->label7->setText( "" );
    std::find( p.begin(), p.end(), 8 ) != p.end() ? ui->label8->setText( "8" ) : ui->label8->setText( "" );
    std::find( p.begin(), p.end(), 9 ) != p.end() ? ui->label9->setText( "9" ) : ui->label9->setText( "" );
}

void Box1x1Widget::updateSolution( int solution )
{
    if( solution != 0 )
    {
        setLineWidth( 3 );
        if( solution == 1 ) ui->label1->setFont( m_boldText );
        else if( solution == 2 ) ui->label2->setFont( m_boldText );
        else if( solution == 3 ) ui->label3->setFont( m_boldText );
        else if( solution == 4 ) ui->label4->setFont( m_boldText );
        else if( solution == 5 ) ui->label5->setFont( m_boldText );
        else if( solution == 6 ) ui->label6->setFont( m_boldText );
        else if( solution == 7 ) ui->label7->setFont( m_boldText );
        else if( solution == 8 ) ui->label8->setFont( m_boldText );
        else if( solution == 9 ) ui->label9->setFont( m_boldText );
    }
    else
    {
        setLineWidth( 1 );
        ui->label1->setFont( m_normalText );
        ui->label2->setFont( m_normalText );
        ui->label3->setFont( m_normalText );
        ui->label4->setFont( m_normalText );
        ui->label5->setFont( m_normalText );
        ui->label6->setFont( m_normalText );
        ui->label7->setFont( m_normalText );
        ui->label8->setFont( m_normalText );
        ui->label9->setFont( m_normalText );
    }
}

void Box1x1Widget::setCoordinates( int row, int col )
{
    m_row = row;
    m_col = col;
}

QString Box1x1Widget::getValue()
{
    return ui->labelValue->text();
}

void Box1x1Widget::mousePressEvent( QMouseEvent* event )
{
    //Send signal to main window with coordinates
    emit SIG_Box1x1Clicked( m_row, m_col );
}

