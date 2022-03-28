#pragma once
#include <vector>
#include <algorithm>
#include "Node.h"
class Heap
{
public:
	std::vector<Node*> items;
	int currentItemCount;

	int count;

	Heap(int maxSize) {
		items.resize(maxSize);
	}

	bool contains(Node item) {
		if (items[item.HeapIndex]->position.x == item.position.x &&
			items[item.HeapIndex]->position.y == item.position.y) {
			return true;
		}
		else {
			return false;
		}
	}

	void add(Node* item) {
		item->HeapIndex = currentItemCount;
		items[currentItemCount] = item;
		sortUp(item);
		currentItemCount++;
	}

	Node* removeFirst() {
		Node* firstItem = items[0];
		currentItemCount--;
		items[0] = items[currentItemCount];
		items[0]->HeapIndex = 0;
		sortDown(items[0]);
		return firstItem;
	}

	void updateItem(Node* item) {
		sortUp(item);
	}

	void sortDown(Node* item) {
		while (true)
		{
			int childIndexLeft = item->HeapIndex * 2 + 1;
			int childIndexRight = item->HeapIndex * 2 + 2;
			int swapIndex = 0;

			if (childIndexLeft < currentItemCount)
			{
				swapIndex = childIndexLeft;

				if (childIndexRight < currentItemCount)
				{
					if (items[childIndexLeft]->compare(*items[childIndexRight]) < 0)
					{
						swapIndex = childIndexRight;
					}
				}

				if (item->compare(*items[swapIndex]) < 0)
				{
					swap(item, items[swapIndex]);
				}
				else
				{
					return;
				}

			}
			else
			{
				return;
			}

		}
	}

	void sortUp(Node* item) {
		int parentIndex = (item->HeapIndex - 1) / 2;
		int itemIndex;
		while (true)
		{
			Node* parentItem = items[parentIndex];
			if (item->compare(*parentItem) > 0)
			{

				swap(item, parentItem);
			}
			else
			{
				break;
			}

			parentIndex = (item->HeapIndex - 1) / 2;
		}
	}

	void swap(Node* itemA, Node* itemB) {

		items[itemA->HeapIndex] = itemB;
		items[itemB->HeapIndex] = itemA;
		int itemAIndex = itemA->HeapIndex;
		itemA->HeapIndex = itemB->HeapIndex;
		itemB->HeapIndex = itemAIndex;
	}

};


