#include "box3x3widget.h"
#include <QGridLayout>
#include <QDebug>

Box3x3Widget::Box3x3Widget( QWidget* parent ) :
    QFrame( parent )
{
    setFrameStyle( QFrame::Box );
    setFrameShadow( QFrame::Sunken );
    QGridLayout* gridLayout = new QGridLayout();
    setLayout( gridLayout );

    for( unsigned int row = 0; row < 3; ++row )
    {
        for( unsigned int col = 0; col < 3; ++col )
        {
            Box1x1Widget* currentBox = new Box1x1Widget( this );
            boxes[row][col] = currentBox;
            gridLayout->addWidget( currentBox, row, col, 1, 1 );

            connect( currentBox, SIGNAL( SIG_Box1x1Clicked( int, int ) ),
                           parent, SLOT( SLT_Box1x1Clicked( int, int ) ) );
        }
    }
}

Box3x3Widget::~Box3x3Widget()
{

}

void Box3x3Widget::setCoordinates( int boxRow, int boxCol )
{
    for( unsigned int row = 0; row < 3; ++row )
    {
        for( unsigned int col = 0; col < 3; ++col )
        {
            boxes[row][col]->setCoordinates( boxRow * 3 + row, boxCol * 3 + col );
        }
    }
}


QString Box3x3Widget::getValue( int row, int col )
{
    return boxes[row][col]->getValue();
}


Box1x1Widget* Box3x3Widget::getBox1x1( int row, int col )
{
    return boxes[row][col];
}
