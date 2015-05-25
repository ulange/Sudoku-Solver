#include "model.h"
#include <QDebug>

Model::Model( QObject* parent ) : QObject( parent )
{
    for( int row = 0; row < ROWS; ++row )
    {
        std::vector<ModelBox1x1*> modelRow;

        for( int col = 0; col < COLS; ++col )
        {
            ModelBox1x1* currentBox = new ModelBox1x1( this );
            currentBox->setCoordinates( row, col );
            modelRow.push_back( currentBox );
        }
        m_boxes.push_back( modelRow );
    }
}

Model::~Model()
{

}

void Model::init()
{
    for( int row = 0; row < ROWS; ++row )
    {
        for( int col = 0; col < COLS; ++col )
        {
            m_boxes.at( row ).at( col )->init();
        }
    }
}

void Model::setValue( int value, int row, int col )
{
    m_boxes.at( row ).at( col )->setValue( value );

}

int Model::getValue( int row, int col )
{
     return m_boxes.at( row ).at( col )->getValue();
}

int Model::getSolution( int row, int col )
{
     return m_boxes.at( row ).at( col )->getSolution();
}

std::vector<int>& Model::getPossibilities( int row, int col )
{
    return m_boxes.at( row ).at( col )->getPossibilities();
}

bool Model::removePossibilityFromRow( int possibility, int row )
{
    bool returnValue = false;

    for( int col = 0; col < COLS; ++col )
    {
        returnValue |= m_boxes.at( row ).at( col )->removePossibility( possibility );
    }

    return returnValue;
}

bool Model::removePossibilityFromCol( int possibility, int col )
{
    bool returnValue = false;

    for( int row = 0; row < ROWS; ++row )
    {
        returnValue |= m_boxes.at( row ).at( col )->removePossibility( possibility );
    }

    return returnValue;
}

bool Model::removePossibilityFromBox3x3( int possibility, int row, int col )
{
    bool returnValue = false;

    int boxRow = row / 3;
    int boxCol = col / 3;

    //qDebug() << "Removing in Box3x3: " << possibility << " from " << row << ", " << col;
    for( int r = boxRow * 3; r < boxRow * 3 + 3; ++r )
    {
        for( int c = boxCol * 3; c < boxCol * 3 + 3; ++c )
        {
            //qDebug() << r << ", " << c;
            //if( !( r == row && c == col ) ) //Do not remove from input value
            {
                returnValue |= m_boxes.at( r ).at( c )->removePossibility( possibility );
            }
        }
    }

    return returnValue;
}

bool Model::searchUniquenessInRows()
{
    bool returnValue = false;

    for( int row = 0; row < 9; ++row )
    {
        //Search for each possible digit (1...9) for uniqueness in the possibilities of each 1x1 box
        for( int digit = 1; digit <= 9; ++digit )
        {
            int count = 0;
            ModelBox1x1* currentBox1x1 = 0;
            for( int col = 0; col < 9; ++col )
            {
                std::vector<int> v = m_boxes.at( row ).at( col )->getPossibilities();
                if( std::find( v.begin(), v.end(), digit ) != v.end() )
                {
                    count++;
                    currentBox1x1 = m_boxes.at( row ).at( col );
                }
            }

            if( count == 1 ) //Exactly one possibility!
            {
                qDebug() << "searchUniquenessInRows " << digit << " in " << currentBox1x1->getRow() << " " << currentBox1x1->getCol();
                currentBox1x1->setSolution( digit );

                //returnValue = true;
            }
        }
    }

    return returnValue;
}

bool Model::searchUniquenessInCols()
{
    bool returnValue = false;

    for( int col = 0; col < 9; ++col )
    {
        //Search for each possible digit (1...9) for uniqueness in the possibilities of each 1x1 box
        for( int digit = 1; digit <= 9; ++digit )
        {
            int count = 0;
            ModelBox1x1* currentBox1x1 = 0;
            for( int row = 0; row < 9; ++row )
            {
                std::vector<int> v = m_boxes.at( row ).at( col )->getPossibilities();
                if( std::find( v.begin(), v.end(), digit ) != v.end() )
                {
                    count++;
                    currentBox1x1 = m_boxes.at( row ).at( col );
                }
            }

            if( count == 1 ) //Exactly one possibility!
            {
                qDebug() << "searchUniquenessInCols " << digit << " in " << currentBox1x1->getRow() << " " << currentBox1x1->getCol();
                currentBox1x1->setSolution( digit );

                //returnValue = true;
            }
        }
    }

    return returnValue;
}



bool Model::searchUniquenessInBox3x3()
{
    bool returnValue = false;

    for( int boxRow = 0; boxRow < 3; ++boxRow )
    {
        for( int boxCol = 0; boxCol < 3; ++boxCol )
        {
            //Search for each possible digit (1...9) for uniqueness in the possibilities of each 1x1 box
            for( int digit = 1; digit <= 9; ++digit )
            {
                int count = 0;
                ModelBox1x1* currentBox1x1 = 0;
                for( int subRow = 0; subRow < 3; ++subRow )
                {
                    for( int subCol = 0; subCol < 3; ++subCol )
                    {
                        std::vector<int> v = m_boxes.at( boxRow * 3 + subRow ).at( boxCol * 3 + subCol )->getPossibilities();
                        if( std::find( v.begin(), v.end(), digit ) != v.end() )
                        {
                            count++;
                            currentBox1x1 = m_boxes[boxRow * 3 + subRow][boxCol * 3 + subCol];
                        }
                    }
                }

                if( count == 1 ) //Exactly one possibility!
                {
                    qDebug() << "searchUniquenessInBox3x3 " << digit << " " << currentBox1x1->getRow() << " " << currentBox1x1->getCol();
                    currentBox1x1->setSolution( digit );

                    //returnValue = true;
                }
            }
        }
    }

    return returnValue;
}



std::vector<ModelBox1x1*> Model::getBoxes1x1InBox3x3( int box3x3 )
{
    std::vector<ModelBox1x1*> result;

    int boxRow = box3x3 / 3;
    int boxCol = box3x3 % 3;

    for( int row = boxRow * 3; row < boxRow * 3 + 3; ++row )
    {
        for( int col = boxCol * 3; col < boxCol * 3 + 3; ++col )
        {
            result.push_back( m_boxes.at( row ).at( col ) );
        }
    }

    return result;
}

std::vector<ModelBox1x1*> Model::getBoxes1x1InRow( int row )
{
    std::vector<ModelBox1x1*> result;

    for( int col = 0; col < COLS; ++col )
    {
        result.push_back( m_boxes.at( row ).at( col ) );
    }

    return result;
}

std::vector<ModelBox1x1*> Model::getBoxes1x1InCol( int col )
{
    std::vector<ModelBox1x1*> result;

    for( int row = 0; row < ROWS; ++row )
    {
        result.push_back( m_boxes.at( row ).at( col ) );
    }

    return result;
}

bool Model::findLinesInBoxes3x3()
{
    bool returnValue = false;

    for( int box3x3 = 0; box3x3 < 9; ++box3x3 )
    {
        std::vector<ModelBox1x1*> currentBoxes = getBoxes1x1InBox3x3( box3x3 );

        //Search for digits that exist only in one column of the current box3x3
        for( int digit = 1; digit <= 9; ++digit )
        {
            bool uniqueCol = true;
            int colFound = -1;
            for( int box1x1 = 0; box1x1 < currentBoxes.size(); ++box1x1 )
            {
                std::vector<int> v = currentBoxes.at( box1x1 )->getPossibilities();
                if( std::find( v.begin(), v.end(), digit ) != v.end() )
                {
                    if( colFound == -1 )
                    {
                        colFound = currentBoxes.at( box1x1 )->getCol();
                    }
                    else if( colFound != currentBoxes.at( box1x1 )->getCol() )
                    {
                         uniqueCol = false;
                    }
                }
            }

            if( uniqueCol && colFound != -1 )
            {
                qDebug() << "Unique digit " << digit << " found in column " << colFound << " by Box3x3 " << box3x3;

                //Eliminate the found digit from the same global column
                for( int row = 0; row < 9; ++row )
                {
                    if( row / 3 != box3x3 / 3 )
                    {
                        returnValue |= m_boxes.at( row ).at( colFound )->removePossibility( digit );
                    }
                }
            }

            bool uniqueRow = true;
            int rowFound = -1;
            for( int box1x1 = 0; box1x1 < currentBoxes.size(); ++box1x1 )
            {
                std::vector<int> v = currentBoxes.at( box1x1 )->getPossibilities();
                if( std::find( v.begin(), v.end(), digit ) != v.end() )
                {
                    if( rowFound == -1 )
                    {
                        rowFound = currentBoxes.at( box1x1 )->getRow();
                    }
                    else if( rowFound != currentBoxes.at( box1x1 )->getRow() )
                    {
                         uniqueRow = false;
                    }
                }
            }

            if( uniqueRow && rowFound != -1 )
            {
                qDebug() << "Unique digit " << digit << " found in row " << rowFound << " by Box3x3 " << box3x3;

                //Eliminate the found digit from the same global row
                for( int col = 0; col < 9; ++col )
                {
                    if( col / 3 != box3x3 % 3 )
                    {
                        returnValue |= m_boxes.at( rowFound ).at( col )->removePossibility( digit );
                    }
                }
            }
        }
    }

    return returnValue;
}

bool Model::verification()
{
    bool result = true;

    for( int row = 0; row < ROWS; ++row )
    {
        int boxRow = row / 3;

        std::vector<ModelBox1x1*> compareBoxesRow = getBoxes1x1InRow( row );

        for( int col = 0; col < COLS; ++col )
        {
            int boxCol = col / 3;

            std::vector<ModelBox1x1*> compareBoxesCol = getBoxes1x1InCol( col );

            std::vector<ModelBox1x1*> compareBoxesBox3x3 = getBoxes1x1InBox3x3( boxRow * 3 + boxCol );

            ModelBox1x1* currentBox1x1 = m_boxes.at( row ).at( col );

            for( int i = 0; i < compareBoxesRow.size(); ++i )
            {
                if( compareBoxesRow.at( i ) != currentBox1x1 )
                {
                    int value = currentBox1x1->getValue();
                    int compareValue = compareBoxesRow.at( i )->getValue();

                    if( value > 0 && compareValue > 0 )
                    {

                        bool contradiction = ( value == compareValue );
                        if( contradiction )
                        {
                            result = false;
                            qDebug() << "Contradiction found in row in " << row << "," << col;
                        }
                    }
                }
            }

            for( int i = 0; i < compareBoxesCol.size(); ++i )
            {
                if( compareBoxesCol.at( i ) != currentBox1x1 )
                {
                    int value = currentBox1x1->getValue();
                    int compareValue = compareBoxesCol.at( i )->getValue();

                    if( value > 0 && compareValue > 0 )
                    {

                        bool contradiction = ( value == compareValue );
                        if( contradiction )
                        {
                            result = false;
                            qDebug() << "Contradiction found in col in " << row << "," << col;
                        }
                    }
                }
            }

            for( int i = 0; i < compareBoxesBox3x3.size(); ++i )
            {
                if( compareBoxesBox3x3.at( i ) != currentBox1x1 )
                {
                    int value = currentBox1x1->getValue();
                    int compareValue = compareBoxesBox3x3.at( i )->getValue();

                    if( value > 0 && compareValue > 0 )
                    {

                        bool contradiction = ( value == compareValue );
                        if( contradiction )
                        {
                            result = false;
                            qDebug() << "Contradiction found in box3x3 in " << row << "," << col;
                        }
                    }
                }
            }
        }
    }

    return result;
}



bool Model::findTwoPairsInBoxes3x3()
{
    //1. Get all boxes (1x1) which fulfill the requirement box3x3(i)
    //2. Search in this set for boxes with only two possibilities
    //3. Search for matches between theses boxes
    //4. Found matches should be distributed in the requirement column(i) (eliminate possibilities of the matches)

    bool foundMatch = false;

//    for( int box3x3 = 0; box3x3 < 9; ++box3x3 )
//    {
//        std::vector<Box1x1Widget*> currentRequirementBoxes = getBoxes1x1InBox3x3( box3x3 );
//        std::vector<Box1x1Widget*> BoxesWithTwoPossibilities;

//        for( unsigned int box = 0; box < 9; ++box )
//        {
//            Box1x1Widget* currentBox = currentRequirementBoxes.at( box );
//            std::vector<int> curPossibilities = currentBox->getPossibilities();
//            if( curPossibilities.size() == 2 )
//            {
//                BoxesWithTwoPossibilities.push_back( currentBox );
//            }
//        }

//        if( BoxesWithTwoPossibilities.size() >= 2 )
//        {
//            qDebug() << "findTwoPairsInBoxes3x3 found at least two boxes with two possiblitites in box3x3 " << box3x3;

//            //Compare each with each
//            for( int a = 0; a < BoxesWithTwoPossibilities.size(); ++a )
//            {
//                for( int b = a + 1; b < BoxesWithTwoPossibilities.size(); ++b )
//                {
//                    if( a != b ) //Do not compare one box with itself (would be always true)
//                    {
//                        std::vector<int> possibilitiesA = BoxesWithTwoPossibilities.at( a )->getPossibilities();
//                        std::vector<int> possibilitiesB = BoxesWithTwoPossibilities.at( b )->getPossibilities();
//                        if( possibilitiesA.size() < 2 )
//                        {
//                            qDebug() << "Possibilities suddenly empty in " << BoxesWithTwoPossibilities.at( a )->getRow() << ", " << BoxesWithTwoPossibilities.at( a )->getCol();
//                            continue;
//                        }
//                        if( possibilitiesB.size() < 2 )
//                        {
//                            qDebug() << "Possibilities suddenly empty in " << BoxesWithTwoPossibilities.at( b )->getRow() << ", " << BoxesWithTwoPossibilities.at( b )->getCol();
//                            continue;
//                        }

//                        if( possibilitiesA.at( 0 ) == possibilitiesB.at( 0 ) && possibilitiesA.at( 1 ) == possibilitiesB.at( 1 ) )
//                        {
//                            qDebug() << "Found a pair match! Elements " << a << " and " << b;

//                            //Now remove these two digits from all others in
//                            for( unsigned int box = 0; box < 9; ++box )
//                            {
//                                Box1x1Widget* currentBox1x1 = currentRequirementBoxes.at( box );
//                                if( currentBox1x1 != BoxesWithTwoPossibilities.at( a ) && currentBox1x1 != BoxesWithTwoPossibilities.at( b ) )
//                                {
//                                    if( currentBox1x1->removeFromPossibilities( BoxesWithTwoPossibilities.at( a )->getPossibilities().at( 0 ) ) )
//                                    {
//                                        //Remember to restart the function
//                                        foundMatch = true;
//                                        qDebug() << "Removed " << BoxesWithTwoPossibilities.at( a )->getPossibilities().at( 0 );
//                                    }
//                                    if( currentBox1x1->removeFromPossibilities( BoxesWithTwoPossibilities.at( a )->getPossibilities().at( 1 ) ) )
//                                    {
//                                        //Remember to restart the function
//                                        foundMatch = true;
//                                        qDebug() << "Removed " << BoxesWithTwoPossibilities.at( a )->getPossibilities().at( 1 );
//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }

    return foundMatch;
}

bool Model::findTwoPairsInColumns()
{
    //1. Get all boxes (1x1) which fulfill the requirement column(i)
    //2. Search in this set for boxes with only two possibilities
    //3. Search for matches between theses boxes
    //4. Found matches should be distributed in the requirement column(i) (eliminate possibilities of the matches)

    bool foundMatch = false;

//    for( int col = 0; col < 9; ++col )
//    {
//        std::vector<ModelBox1x1*> currentRequirementBoxes = getBoxes1x1InColumn( col );
//        std::vector<ModelBox1x1*> BoxesWithTwoPossibilities;

//        for( unsigned int box = 0; box < 9; ++box )
//        {
//            Box1x1Widget* currentBox = currentRequirementBoxes.at( box );
//            std::vector<int> curPossibilities = currentBox->getPossibilities();
//            if( curPossibilities.size() == 2 )
//            {
//                BoxesWithTwoPossibilities.push_back( currentBox );
//            }
//        }

//        if( BoxesWithTwoPossibilities.size() >= 2 )
//        {
//            qDebug() << "findTwoPairsInColumns found at least two boxes with two possiblitites in column " << col;

//            //Compare each with each
//            for( int a = 0; a < BoxesWithTwoPossibilities.size(); ++a )
//            {
//                for( int b = a + 1; b < BoxesWithTwoPossibilities.size(); ++b )
//                {
//                    if( a != b ) //Do not compare one box with itself (would be always true)
//                    {
//                        std::vector<int> possibilitiesA = BoxesWithTwoPossibilities.at( a )->getPossibilities();
//                        std::vector<int> possibilitiesB = BoxesWithTwoPossibilities.at( b )->getPossibilities();
//                        if( possibilitiesA.size() < 2 )
//                        {
//                            qDebug() << "Possibilities suddenly empty in " << BoxesWithTwoPossibilities.at( a )->getRow() << ", " << BoxesWithTwoPossibilities.at( a )->getCol();
//                            continue;
//                        }
//                        if( possibilitiesB.size() < 2 )
//                        {
//                            qDebug() << "Possibilities suddenly empty in " << BoxesWithTwoPossibilities.at( b )->getRow() << ", " << BoxesWithTwoPossibilities.at( b )->getCol();
//                            continue;
//                        }

//                        if( possibilitiesA.at( 0 ) == possibilitiesB.at( 0 ) && possibilitiesA.at( 1 ) == possibilitiesB.at( 1 ) )
//                        {
//                            qDebug() << "Found a pair match! Elements " << a << " and " << b;

//                            //Now remove these two digits from all others in
//                            for( unsigned int box = 0; box < 9; ++box )
//                            {
//                                Box1x1Widget* currentBox1x1 = currentRequirementBoxes.at( box );
//                                if( currentBox1x1 != BoxesWithTwoPossibilities.at( a ) && currentBox1x1 != BoxesWithTwoPossibilities.at( b ) )
//                                {
//                                    if( currentBox1x1->removeFromPossibilities( BoxesWithTwoPossibilities.at( a )->getPossibilities().at( 0 ) ) )
//                                    {
//                                        //Remember to restart the function
//                                        foundMatch = true;
//                                        qDebug() << "Removed " << BoxesWithTwoPossibilities.at( a )->getPossibilities().at( 0 );
//                                    }
//                                    if( currentBox1x1->removeFromPossibilities( BoxesWithTwoPossibilities.at( a )->getPossibilities().at( 1 ) ) )
//                                    {
//                                        //Remember to restart the function
//                                        foundMatch = true;
//                                        qDebug() << "Removed " << BoxesWithTwoPossibilities.at( a )->getPossibilities().at( 1 );
//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }

    return foundMatch;
}

bool Model::findTwoPairsInRows()
{
    //1. Get all boxes (1x1) which fulfill the requirement row(i)
    //2. Search in this set for boxes with only two possibilities
    //3. Search for matches between theses boxes
    //4. Found matches should be distributed in the requirement row(i) (eliminate possibilities of the matches)

    bool foundMatch = false;

//    for( int row = 0; row < 9; ++row )
//    {
//        std::vector<Box1x1Widget*> currentRequirementBoxes = getBoxes1x1InRow( row );
//        std::vector<Box1x1Widget*> BoxesWithTwoPossibilities;

//        for( unsigned int box = 0; box < 9; ++box )
//        {
//            Box1x1Widget* currentBox = currentRequirementBoxes.at( box );
//            std::vector<int> curPossibilities = currentBox->getPossibilities();
//            if( curPossibilities.size() == 2 )
//            {
//                BoxesWithTwoPossibilities.push_back( currentBox );
//            }
//        }

//        if( BoxesWithTwoPossibilities.size() >= 2 )
//        {
//            qDebug() << "findTwoPairsInRows found at least two boxes with two possiblitites in row " << row;

//            //Compare each with each
//            for( int a = 0; a < BoxesWithTwoPossibilities.size(); ++a )
//            {
//                for( int b = a + 1; b < BoxesWithTwoPossibilities.size(); ++b )
//                {
//                    if( a != b ) //Do not compare one box with itself (would be always true)
//                    {
//                        std::vector<int> possibilitiesA = BoxesWithTwoPossibilities.at( a )->getPossibilities();
//                        std::vector<int> possibilitiesB = BoxesWithTwoPossibilities.at( b )->getPossibilities();
//                        if( possibilitiesA.size() < 2 )
//                        {
//                            qDebug() << "Possibilities suddenly empty in " << BoxesWithTwoPossibilities.at( a )->getRow() << ", " << BoxesWithTwoPossibilities.at( a )->getCol();
//                            continue;
//                        }
//                        if( possibilitiesB.size() < 2 )
//                        {
//                            qDebug() << "Possibilities suddenly empty in " << BoxesWithTwoPossibilities.at( b )->getRow() << ", " << BoxesWithTwoPossibilities.at( b )->getCol();
//                            continue;
//                        }

//                        if( possibilitiesA.at( 0 ) == possibilitiesB.at( 0 ) && possibilitiesA.at( 1 ) == possibilitiesB.at( 1 ) )
//                        {
//                            qDebug() << "Found a pair match! Elements " << a << " and " << b;

//                            //Now remove these two digits from all others in
//                            for( unsigned int box = 0; box < 9; ++box )
//                            {
//                                Box1x1Widget* currentBox1x1 = currentRequirementBoxes.at( box );
//                                if( currentBox1x1 != BoxesWithTwoPossibilities.at( a ) && currentBox1x1 != BoxesWithTwoPossibilities.at( b ) )
//                                {
//                                    if( currentBox1x1->removeFromPossibilities( BoxesWithTwoPossibilities.at( a )->getPossibilities().at( 0 ) ) )
//                                    {
//                                        //Remember to restart the function
//                                        foundMatch = true;
//                                        qDebug() << "Removed " << BoxesWithTwoPossibilities.at( a )->getPossibilities().at( 0 );
//                                    }
//                                    if( currentBox1x1->removeFromPossibilities( BoxesWithTwoPossibilities.at( a )->getPossibilities().at( 1 ) ) )
//                                    {
//                                        //Remember to restart the function
//                                        foundMatch = true;
//                                        qDebug() << "Removed " << BoxesWithTwoPossibilities.at( a )->getPossibilities().at( 1 );
//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }

    return foundMatch;
}
