#ifndef Field_h__
#define Field_h__

#include <QVector>
#include <QMap>

#include "Move.h"

namespace AutoBot
{
class OutOfFieldException;

/**	Положение на доске */
class Field
{
public:
	typedef unsigned int DimSizeType;
	typedef QVector<DimSizeType> FieldDims;

	Field(QVector<unsigned int> dimSizes);
	virtual ~Field(void);

	/**	Совершить ход */
	void DoMove(const Move&) throw(OutOfFieldException);
	/**	Получить состояние заданной клетки поля.  */
	Move GetMove(Move::CoordType x, ...) const;
	/**	Получить состояние заданной клетки поля.  */
	Move GetMove(const Move::MoveCoords&) const;
	/**	Распкчатать размеры поля */
	QString PrintDimensions() const;

protected:
	typedef unsigned long ShiftType;

	/**	Получить число измерений поля */
	unsigned int getDimsCount() const;
	/**	Получить размер заданного измерения (начиная с ноля).
	  *	@returns 0, если у поля нет измерения с таким номером */
	DimSizeType getDimSize(unsigned int dimm) const;
	/**	Получить смещение для m_field */
	ShiftType getMoveShift(const Move& move) const;
	/**	Получить смещение для m_field */
	ShiftType getMoveShift(const Move::MoveCoords& coords) const;
	/**	Проверить, совершён ли ход в поле */
	void checkMoveInBounds(const Move&) const throw(OutOfFieldException);
	/**	Получить максимально допустимое смещение для этого поля */
	ShiftType getMaxShift() const;

private:
	QVector<Move> m_moves;				///< история ходов
	QMap<ShiftType, Move*> m_field;		///< Поле - хеш-таблица всех ходов. Ключ - абсолютное смещение вроде y*width+x для двумерного поля
	FieldDims m_dimSizes;				///< Размеры измерений поля
};

}//namespace AutoBot

#endif // Field_h__
