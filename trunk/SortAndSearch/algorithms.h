#ifndef algorithms_h
#define algorithms_h

qulonglong comps, moves;

void shiftDown(QVector<int> &numbers, int root, int bottom)
{
	int done, maxChild, temp;

	done = 0;
	while ((root*2 <= bottom) && (!done))
	{
		comps+=4;
		if (root*2 == bottom)
			maxChild = root * 2;
		else if (numbers[root * 2] > numbers[root * 2 + 1])
			maxChild = root * 2;
		else
			maxChild = root * 2 + 1;
		
		++moves;
		if (numbers[root] < numbers[maxChild])
		{
			temp = numbers[root];
			numbers[root] = numbers[maxChild];
			numbers[maxChild] = temp;
			root = maxChild;
			moves += 4;
		}
		else
			done = 1;
	}
}

/**	ѕирамидальна€ сортировка */
void heapSort(QVector<int> &numbers, int array_size)
{
	int i, temp;

	for (i = (array_size / 2); i >= 0; i--)
		shiftDown(numbers, i, array_size - 1);

	for (i = array_size-1; i >= 1; i--)
	{
		temp = numbers[0];
		numbers[0] = numbers[i];
		numbers[i] = temp;
		shiftDown(numbers, 0, i-1);

		moves += 3;
	}
}

/**	»нтерпол€ционный поиск */
int interpolationSearch (QVector<int> &list, int size, int key)
{
	++comps;
	if(size < 1) return -1;
	int low = 0, high = size - 1;
	moves += 2;

	while ( list[high] >= key && key > list[low] )
	{
		comps += 4;
		moves += 5;
		double low_diff = (double)key - list[low];
		double range_diff = (double)list[high] - list[low];
		double count_diff = (double)high - low;
		int range = (int)( low_diff / range_diff * count_diff + low );
		if ( key > list[range] )
			low = range + 1;
		else if ( key < list[range] )
			high = range - 1;
		else
			low = range;
	}
	++comps;
	if ( key == list[low] )
	{
		return low;
	}

	return -1;
}

#endif algorithms_h
