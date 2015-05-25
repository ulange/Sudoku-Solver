#ifndef BOX3X3WIDGET_H
#define BOX3X3WIDGET_H

#include <QFrame>
#include "box1x1widget.h"

class Box3x3Widget : public QFrame
{
    Q_OBJECT
public:
    explicit Box3x3Widget( QWidget *parent = 0 );
    ~Box3x3Widget();

    QString getValue( int row, int col );

//    void searchUniquenessInBox1x1();
//    void searchUniquenessInBox3x3();
    void setCoordinates( int row, int col );
    Box1x1Widget* getBox1x1( int row, int col );

protected:
    Box1x1Widget* boxes[3][3];

signals:

public slots:
};

#endif // BOX3X3WIDGET_H
