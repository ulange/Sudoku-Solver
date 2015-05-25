#ifndef BOX1X1WIDGET_H
#define BOX1X1WIDGET_H

#include <QFrame>

namespace Ui {
class Box;
}
class Box1x1Widget : public QFrame
{
    Q_OBJECT

public:
    explicit Box1x1Widget( QWidget* parent = 0 );
    ~Box1x1Widget();

    void showValuePage();
    void showPossibilitiesPage();

    void updatePossibilities( std::vector<int> possibilities );
    void updateSolution( int solution );

    void setValue( int value );
    QString getValue();

    void setCoordinates( int row, int col );

protected:
    void mousePressEvent( QMouseEvent* event );

private:
    Ui::Box* ui;
    int m_row;
    int m_col;

    QFont m_normalText;
    QFont m_boldText;

signals:
    void SIG_Box1x1Clicked( int row, int col );

public slots:

};

#endif // BOX1X1WIDGET_H
