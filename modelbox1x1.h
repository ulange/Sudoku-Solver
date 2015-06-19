#ifndef MODELBOX1X1_H
#define MODELBOX1X1_H

#include <QObject>

const int ROWS = 9;
const int COLS = 9;

class ModelBox1x1 : public QObject
{
    Q_OBJECT
public:
    explicit ModelBox1x1(QObject *parent = 0);
    ModelBox1x1( ModelBox1x1* right );
    ~ModelBox1x1();

    void init();

    void setCoordinates( int row, int col );
    int getRow();
    int getCol();

    void setValue( int value );
    int getValue();

    void setSolution( int solution );
    int getSolution();

    std::vector<int>& getPossibilities();

    bool removePossibility( int possibility );

protected:
    int m_value;
    int m_solution;

    int m_row;
    int m_col;

    std::vector<int> m_possibilities;

signals:

public slots:
};

#endif // MODELBOX1X1_H
