// Array Queue

#include <iostream>
#include <string>
#include <cassert>

using namespace std;
const int MAX_QUEUE = 50;

#ifndef Queue_h
#define Queue_h

template<typename T>
class ArrayQueue
{
private:
    T frames[MAX_QUEUE];        // Array of queue frames
    int front;                 // Index to front of queue
    int back;                  // Index to back of queue
    int count;                 // Number of frames currently in the queue
    
public:
    ArrayQueue();
    int size() { return count; }
    bool isEmpty() const;
    int enqueue(const T& newEntry);
    T dequeue();
    T* at(int key);
    T peekFront() const throw(std::logic_error);
    T peekBack() const throw(std::logic_error);
    void PrintQueue();
};

#endif /* Queue_h */

template<typename T>
ArrayQueue<T>::ArrayQueue() : front(0), back(MAX_QUEUE - 1), count(0)
{
} // end default constructor

template<typename T>
bool ArrayQueue<T>::isEmpty() const
{
    return count == 0;
} // end isEmpty

template<typename T>
int ArrayQueue<T>::enqueue(const T& newEntry)
{
    if (count < MAX_QUEUE)
    {
        // Queue has room for another item
        back = (back + 1) % MAX_QUEUE;
        frames[back] = newEntry;
        count++;
    } // end if
    return back;
} // end enqueue

template<typename T>
T ArrayQueue<T>::dequeue()
{
    T results;
    if (!isEmpty())
    {
        results = frames[front];
        front = (front + 1) % MAX_QUEUE;
        count--;
    } // end if
    return results;
} // end dequeue

template<typename T>
T* ArrayQueue<T>::at(int key)
{
    if(!isEmpty())
    {
        // Sequential Seach can be improved with Bianry Search
        for(int i = 0; i < MAX_QUEUE; ++i)
            if(i == key) return &frames[i];
    } 
    return NULL;
} // end at

template<typename T>
T ArrayQueue<T>::peekFront() const throw(std::logic_error)
{
    // Enforce precondition
    if (isEmpty()) throw std::logic_error("peekFront() called with empty queue");
    else return frames[front];
}
template<typename T>
T ArrayQueue<T>::peekBack() const throw(std::logic_error)
{
    // Enforce precondition
    if (isEmpty()) throw std::logic_error("peekBack() called with empty queue");
    else return frames[back];
}
template<typename T>
void ArrayQueue<T>::PrintQueue()
{
    for(int i = front; i < front+count; ++i)
    {
        frames[i].Print();
    }
}
#if 0
// This is a demo of how it is used. Change zero to 1 for a standalone file
int main()
{
    ArrayQueue<int> q;
    int choice, item;
    bool done = false;
    while (!done)
    {
        cout << "1.Insert Element into the Queue" << endl;
        cout << "2.Delete Element from the Queue" << endl;
        cout << "3.Size of the Queue" << endl;
        cout << "4.Front Element of the Queue" << endl;
        cout << "5.Last Element of the Queue" << endl;
        cout << "6.Exit" << endl;
        cout << "Enter your Choice: ";
        cin >> choice;
        switch(choice)
        {
            case 1:
                cout << "Enter value to be inserted: ";
                cin >> item;
                q.enqueue(item);
                break;
            case 2:
                item = q.peekFront();
                q.dequeue();
                cout << "Element " << item << " Deleted" << endl;
                break;
            case 3:
                cout << "Size of the Queue: ";
                cout << q.size() << endl;
                break;
            case 4:
                cout<<"Front Element of the Queue: ";
                cout<<q.peekFront()<<endl;
                break;
            case 5:
                cout<<"Back Element of the Queue: ";
                cout<<q.peekBack()<<endl;
                break;
            default:
                done = true;
        }
    }
    return 0;
}
#endif