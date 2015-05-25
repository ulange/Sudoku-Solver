#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include "modelbox1x1.h"

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model( QObject *parent = 0 );
    ~Model();

    void init();

    void setValue( int value, int row, int col );
    int getValue( int row, int col );

    int getSolution( int row, int col );

    std::vector<int>& getPossibilities( int row, int col );

    bool removePossibilityFromRow( int possibility, int row );
    bool removePossibilityFromCol( int possibility, int col );
    bool removePossibilityFromBox3x3( int possibility, int row, int col );

    bool findTwoPairsInRows();
    bool findTwoPairsInColumns();
    bool findTwoPairsInBoxes3x3();

    bool findLinesInBoxes3x3();

    bool searchUniquenessInRows();
    bool searchUniquenessInCols();
    bool searchUniquenessInBox3x3();

    std::vector<ModelBox1x1*> getBoxes1x1InBox3x3( int box3x3 );
    std::vector<ModelBox1x1*> getBoxes1x1InRow( int row );
    std::vector<ModelBox1x1*> getBoxes1x1InCol( int col );

    bool verification();

protected:
    std::vector< std::vector<ModelBox1x1*> > m_boxes;

signals:

public slots:
};

#endif // MODEL_H
