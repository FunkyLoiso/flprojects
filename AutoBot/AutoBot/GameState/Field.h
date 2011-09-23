#ifndef Field_h__
#define Field_h__

#include <QVector>
#include <QMap>

#include "Move.h"

namespace AutoBot
{
class OutOfFieldException;

/**	��������� �� ����� */
class Field
{
public:
	typedef unsigned int DimSizeType;
	typedef QVector<DimSizeType> FieldDims;

	Field(QVector<unsigned int> dimSizes);
	virtual ~Field(void);

	/**	��������� ��� */
	void DoMove(const Move&) throw(OutOfFieldException);
	/**	�������� ��������� �������� ������ ����.  */
	Move GetMove(Move::CoordType x, ...) const;
	/**	�������� ��������� �������� ������ ����.  */
	Move GetMove(const Move::MoveCoords&) const;
	/**	����������� ������� ���� */
	QString PrintDimensions() const;

protected:
	typedef unsigned long ShiftType;

	/**	�������� ����� ��������� ���� */
	unsigned int getDimsCount() const;
	/**	�������� ������ ��������� ��������� (������� � ����).
	  *	@returns 0, ���� � ���� ��� ��������� � ����� ������� */
	DimSizeType getDimSize(unsigned int dimm) const;
	/**	�������� �������� ��� m_field */
	ShiftType getMoveShift(const Move& move) const;
	/**	�������� �������� ��� m_field */
	ShiftType getMoveShift(const Move::MoveCoords& coords) const;
	/**	���������, �������� �� ��� � ���� */
	void checkMoveInBounds(const Move&) const throw(OutOfFieldException);
	/**	�������� ����������� ���������� �������� ��� ����� ���� */
	ShiftType getMaxShift() const;

private:
	QVector<Move> m_moves;				///< ������� �����
	QMap<ShiftType, Move*> m_field;		///< ���� - ���-������� ���� �����. ���� - ���������� �������� ����� y*width+x ��� ���������� ����
	FieldDims m_dimSizes;				///< ������� ��������� ����
};

}//namespace AutoBot

#endif // Field_h__
