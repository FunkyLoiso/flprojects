#include <stdarg.h>

#include "Field.h"
#include "Exception/OutOfFieldException.h"

using namespace AutoBot;


AutoBot::Field::Field( QVector<unsigned int> dimSizes ) : m_dimSizes(dimSizes)
{
}

AutoBot::Field::~Field( void )
{
}

void Field::DoMove(const Move& move)
{
	checkMoveInBounds(move);

	m_moves.append(move);
	m_field[getMoveShift(move)] = &*(m_moves.end()-1);
}

Move Field::GetMove(Move::CoordType x, ...) const
{
	Move::MoveCoords coords;
	coords.append(x);

	{
		va_list args;
		va_start(args, x);

		for(unsigned int i = 1; i < getDimsCount(); ++i)
		{
			coords.append(va_arg(args, Move::CoordType));
		}
	
		va_end(args);
	}

	return GetMove(coords);
}


Move Field::GetMove(const Move::MoveCoords& coords) const
{
	unsigned int shift = getMoveShift(coords);

	if(m_field.contains(shift)) return *m_field[shift];
	else return Move();
}


unsigned int Field::getDimsCount() const
{
	return m_dimSizes.size();
}

unsigned int Field::getDimSize( unsigned int dimm ) const
{
	if(dimm < getDimsCount()) return m_dimSizes[dimm];
	else return 0;
}

unsigned long Field::getMoveShift(const Move& move) const
{
	return getMoveShift(move.Coordiantes());
}

unsigned long Field::getMoveShift(const Move::MoveCoords& coords) const
{
	unsigned long result = 0;
	for(size_t i = 0; i < coords.size(); ++i)
	{
		unsigned long multiplier = 1;
		for(unsigned int mi = 0; mi < i; ++mi) multiplier *= getDimSize(mi);
		result += multiplier * coords[i];
	}
	return result;
}

QString Field::PrintDimensions() const
{
	QString result("(");
	for each(unsigned int dimmSize in m_dimSizes)
	{
		result.append(QString::number(dimmSize) + ", ");
	}
	result.chop(2);
	return result + ")";
}

void Field::checkMoveInBounds(const Move& move) const throw(OutOfFieldException)
{
	if( getMoveShift(move.Coordiantes()) > getMaxShift() ) throw OutOfFieldException(move, *this);
}

Field::ShiftType Field::getMaxShift() const
{
	Move::MoveCoords coords;
	for each(DimSizeType dimSize in m_dimSizes)
	{
		coords.append(dimSize-1);
	}

	return getMoveShift(coords);
}

