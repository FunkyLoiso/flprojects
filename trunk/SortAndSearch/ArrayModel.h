#pragma once
#include <QAbstractListModel>

/**	Модель для отображения вектора int в списке */
class IntArrayModel : public QAbstractListModel
{
public:
	IntArrayModel(QVector<int> &arr) : m_arr(arr), m_curRowCount(0)
	{
		UpdateAll();
	};

	int rowCount(const QModelIndex &parent /* = QModelIndex */) const {return m_arr.size();};
	QVariant data(const QModelIndex &index, int role /* = Qt::DisplayRole */) const
	{
		if(role == Qt::DisplayRole || role == Qt::EditRole) return m_arr[index.row()];
		else if(role == Qt::ToolTipRole) return QString("Индекс элемента: %1").arg(index.row());
		else return QVariant();
	};
	bool setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */)
	{
		if(role == Qt::EditRole)
		{
			m_arr[index.row()] = value.toInt();
			emit dataChanged(index, index);
		}
		return true;
	};
	Qt::ItemFlags flags(const QModelIndex &index) const
	{
		return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
	};

	/**	Обновить все строчки */
	void UpdateAll()
	{
		UpdateRowCount();
		emit dataChanged(index(0), index(m_curRowCount));//обновим всё
	};

	/**	Обновить диапазон */
	void UpdateRange(int first, int last)
	{
		UpdateRowCount();
		emit dataChanged(this->index(first), this->index(last < m_curRowCount ? last : m_curRowCount));
	}

	/**	Обновить только добавленные/удалённые строчки */
	void UpdateRowCount()
	{
		int newRowCount = m_arr.size();
		if(newRowCount > m_curRowCount)
		{
			beginInsertRows(QModelIndex(), m_curRowCount, newRowCount-1);
			endInsertRows();
		}
		else if(newRowCount < m_curRowCount)
		{
			beginRemoveRows(QModelIndex(), newRowCount, m_curRowCount-1);
			endRemoveRows();
		}
		m_curRowCount = newRowCount;
	}

private:
	QVector<int> &m_arr;

	int m_curRowCount;	///< текущее число строк в модели (синхронизуется с размером массива при обновлении)

};
