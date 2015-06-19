#include "modelbox1x1.h"
#include <QDebug>

ModelBox1x1::ModelBox1x1( QObject* parent ) :
    QObject( parent ),
    m_value( 0 )
{
}

ModelBox1x1::ModelBox1x1( ModelBox1x1* right ):
    m_value( right->m_value ),
    m_solution( right->m_solution ),
    m_row( right->m_row ),
    m_col( right->m_col ),
    m_possibilities( right->m_possibilities )
{
}

ModelBox1x1::~ModelBox1x1()
{
    m_possibilities.clear();
}

void ModelBox1x1::init()
{
    m_value = 0;
    m_solution = 0;

    m_possibilities.clear();
    for( int i = 1; i <= 9; ++i )
    {
        m_possibilities.push_back( i );
    }
}

void ModelBox1x1::setCoordinates( int row, int col )
{
    m_row = row;
    m_col = col;
}

int ModelBox1x1::getRow()
{
    return m_row;
}

int ModelBox1x1::getCol()
{
    return m_col;
}

void ModelBox1x1::setValue( int value )
{
    m_value = value;
    if( value != 0 )
    {
        m_possibilities.clear();
    }
}

void ModelBox1x1::setSolution( int solution )
{
    m_solution = solution;
}

int ModelBox1x1::getValue()
{
    return m_value;
}

int ModelBox1x1::getSolution()
{
    return m_solution;
}

std::vector<int>& ModelBox1x1::getPossibilities()
{
    return m_possibilities;
}

bool ModelBox1x1::removePossibility( int possibility )
{
    bool returnValue = false;

    if( m_value == 0 )
    {
        if( std::find( m_possibilities.begin(), m_possibilities.end(), possibility ) != m_possibilities.end() )
        {
            qDebug() << "Removing possibility " << possibility << " from " << m_row << ", " << m_col;
            m_possibilities.erase( std::remove( m_possibilities.begin(), m_possibilities.end(), possibility ), m_possibilities.end() );

            returnValue = true;

            if( m_possibilities.size() == 1 )
            {
                m_solution = m_possibilities.at( 0 );
            }
            else if( m_possibilities.empty() )
            {
                qDebug() << "Error: zero possibilities in " << m_row << ", " << m_col;
            }
        }
    }

    return returnValue;
}
