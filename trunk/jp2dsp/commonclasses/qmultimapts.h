#ifndef QMULTIMAPTS_H
#define QMULTIMAPTS_H

#include <QMultiMap>
#include <QMutex>
#include <QPair>

#include "../commonclasses/common.h"
#include "../commonclasses/frame.h"

template <class keyT, class valT> class QMultiMapTS
{
public:
	void clear()
	{
		mutex.lock();
		map.clear();
		mutex.unlock();
	}
	unsigned int count()
	{
		mutex.lock();
		unsigned int ret =  map.count();
		mutex.unlock();
		return ret;
	}
	void insertMulti(const keyT &key, const valT &value)
	{
		printf("\ninsert: before mutex");
		mutex.lock();
		printf("\ninsert: in mutex");
		QMultiMap<int, DecodedBuff>::iterator i = map.insertMulti(key, value);
		printf("\ninsert: inserted");
		mutex.unlock();
	}
	QPair<keyT, valT> first()
	{
		mutex.lock();
		QPair<keyT, valT> ret = QPair<keyT, valT>(map.begin().key(), map.begin().value());
		mutex.unlock();
		return ret;
	}
	int remove(keyT key)
	{
		mutex.lock();
		int ret = map.remove(key);
		mutex.unlock();
		return ret;
	}
	QList<valT> values(keyT key)
	{
		mutex.lock();
		QList<valT> ret = map.values(key);
		mutex.unlock();
		return ret;
	}
	void freeContent()
	{
		mutex.lock();
		DecodedBuff tmpBuff;
		foreach(tmpBuff, map)
		{
			if(tmpBuff.stream == SoundDataID) free(tmpBuff.frame);
			else delete (Frame *)tmpBuff.frame;
		}
		mutex.unlock();
	}

private:
	QMultiMap<keyT, valT> map;
	QMutex mutex;
};

#endif /*QMULTIMAPTS_H*/
