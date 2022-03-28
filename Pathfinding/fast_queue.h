#pragma once
using namespace std;
#include <algorithm>

template<typename T2>

class container_class {
public:
    int priority;
    int queue_index;
    T2 element;
};

template<class T>

class fast_queue {
protected:
    int size;
public:
    container_class<T>* dat;
    int num;
    void allocate(int n) {
        size = n;
        dat = new container_class<T>[n];
    }

    void reset() {
        num = 0;
        for (int i = 0; i < size; i++) {
            dat[i] = 0;
        }
    }

    ~fast_queue() {
        delete[] dat;
    }

    void add(T item, int priority) {
        container_class<T> element;
        num++;

        element.element = item;
        element.priority = priority;
        element.queue_index = num;

        dat[num] = element;

        sort_up(element);

    }


    T& operator[](int index) {
        return dat[index];
    }

    bool contains_(container_class<T> item) {
        return dat[item.queue_index] == item;
    }

    bool contains(T item) {
        
        for (int i = 0; i < num; i++) {
            if (dat[i].element == item) {
                return true;
            }
        }
        return false;
    }

    T peek() {
        return dat[1].element;
    }

    T top() {
        T result = dat[1].element;
        //If the node is already the last node, we can remove it immediately
        if (num == 1)
        {
            num = 0;
            return result;
        }

        //Swap the node with the last node
        container_class<T> formerLastNode = dat[num];
        dat[1] = formerLastNode;
        formerLastNode.queue_index = 1;
        num--;

        //Now bubble formerLastNode (which is no longer the last node) down
        sort_down(formerLastNode);
        return result;
    }

private:
    void sort_up(container_class<T> element) {

        int parent;
        if (element.queue_index > 1)
        {
            parent = element.queue_index >> 1;
            container_class<T> parent_element = dat[parent];
            if (parent_element.priority < element.priority)
                return;

            //element has lower priority value, so move parent down the heap to make room
            dat[element.queue_index] = parent_element;
            parent_element.queue_index = element.queue_index;

            element.queue_index = parent;
        }
        else
        {
            return;
        }
        while (parent > 1)
        {
            parent >>= 1;
            container_class<T> parent_element = dat[parent];
            if (parent_element.priority < element.priority)
                break;

            //element has lower priority value, so move parent down the heap to make room
            dat[element.queue_index] = parent_element;
            parent_element.queue_index = element.queue_index;

            element.queue_index = parent;
        }
        dat[element.queue_index] = element;
    }

    inline bool HasHigherPriority(container_class<T> higher, container_class<T> lower)
    {
        return (higher.priority < lower.priority);
    }

    void sort_down(container_class<T> element) {
        //aka Heapify-down
        int queue_index = element.queue_index;
        int left_index = 2 * queue_index;

        // If leaf element, we're done
        if (left_index > num)
        {
            return;
        }

        // Check if the left-child is higher-priority than the current element
        int right_index = left_index + 1;
        container_class<T> child_left = dat[left_index];
        if (HasHigherPriority(child_left, element))
        {
            // Check if there is a right child. If not, swap and finish.
            if (right_index > num)
            {
                element.queue_index = left_index;
                child_left.queue_index = queue_index;
                dat[queue_index] = child_left;
                dat[left_index] = element;
                return;
            }
            // Check if the left-child is higher-priority than the right-child
            container_class<T> childRight = dat[right_index];
            if (HasHigherPriority(child_left, childRight))
            {
                // left is highest, move it up and continue
                child_left.queue_index = queue_index;
                dat[queue_index] = child_left;
                queue_index = left_index;
            }
            else
            {
                // right is even higher, move it up and continue
                childRight.queue_index = queue_index;
                dat[queue_index] = childRight;
                queue_index = right_index;
            }
        }
        // Not swapping with left-child, does right-child exist?
        else if (right_index > num)
        {
            return;
        }
        else
        {
            // Check if the right-child is higher-priority than the current element
            container_class<T> childRight = dat[right_index];
            if (HasHigherPriority(childRight, element))
            {
                childRight.queue_index = queue_index;
                dat[queue_index] = childRight;
                queue_index = right_index;
            }
            // Neither child is higher-priority than current, so finish and stop.
            else
            {
                return;
            }
        }

        while (true)
        {
            left_index = 2 * queue_index;

            // If leaf element, we're done
            if (left_index > num)
            {
                element.queue_index = queue_index;
                dat[queue_index] = element;
                break;
            }

            // Check if the left-child is higher-priority than the current element
            right_index = left_index + 1;
            child_left = dat[left_index];
            if (HasHigherPriority(child_left, element))
            {
                // Check if there is a right child. If not, swap and finish.
                if (right_index > num)
                {
                    element.queue_index = left_index;
                    child_left.queue_index = queue_index;
                    dat[queue_index] = child_left;
                    dat[left_index] = element;
                    break;
                }
                // Check if the left-child is higher-priority than the right-child
                container_class<T> child_right = dat[right_index];
                if (HasHigherPriority(child_left, child_right))
                {
                    // left is highest, move it up and continue
                    child_left.queue_index = queue_index;
                    dat[queue_index] = child_left;
                    queue_index = left_index;
                }
                else
                {
                    // right is even higher, move it up and continue
                    child_right.queue_index = queue_index;
                    dat[queue_index] = child_right;
                    queue_index = right_index;
                }
            }
            // Not swapping with left-child, does right-child exist?
            else if (right_index > num)
            {
                element.queue_index = queue_index;
                dat[queue_index] = element;
                break;
            }
            else
            {
                // Check if the right-child is higher-priority than the current element
                container_class<T> childRight = dat[right_index];
                if (HasHigherPriority(childRight, element))
                {
                    childRight.queue_index = queue_index;
                    dat[queue_index] = childRight;
                    queue_index = right_index;
                }
                // Neither child is higher-priority than current, so finish and stop.
                else
                {
                    element.queue_index = queue_index;
                    dat[queue_index] = element;
                    break;
                }
            }
        }
    }
};