#ifndef MQUEUE
#define MQUEUE

template <class T>
struct QNode {
    T data;
    struct QNode<T>* next;
};

template <class T>
class mQueue {
   private:
    struct QNode<T>* front;  // 队头
    struct QNode<T>* rear;   // 队尾
    // 队头是一个空结点
    // 队尾指向最后一个元素结点

    int size;  // 队长

   public:
    mQueue();
    mQueue(const mQueue<T>& Q);
    ~mQueue();
    void enQueue(T& x);  // 入队
    T deQueue();         // 出队
    T getFront();        // 获取队头元素
    bool isEmpty();      // 判空
    int length();        // 返回长度
};

template <class T>
mQueue<T>::mQueue() {
    front = new QNode<T>();
    rear = front;
    size = 0;
    front->next = nullptr;
}

template <class T>
mQueue<T>::mQueue(const mQueue<T>& Q) {
    // 相当于链表复制了
    front = new QNode<T>();
    rear = front;
    front->next = nullptr;
    for (struct QNode<T>* p = Q.front->next; p; p = p->next) {
        struct QNode<T>* q = new QNode<T>();
        q->data = p->data;
        q->next = nullptr;
        rear->next = q;
        rear = q;
    }
    size = Q.size;
}

template <class T>
mQueue<T>::~mQueue() {
    while (front) {
        // rear 用于暂时存储队头的下一个结点
        rear = front->next;
        delete front;
        front = rear;
    }
}

template <class T>
void mQueue<T>::enQueue(T& x) {
    struct QNode<T>* p = new QNode<T>();
    p->data = x;
    p->next = nullptr;
    rear->next = p;
    rear = p;
    size++;
}

template <class T>
T mQueue<T>::deQueue() {  // 删除队头元素
    struct QNode<T>* p = front->next->next;
    T ret = front->next->data;
    delete front->next;
    front->next = p;
    size--;
    if (!size)
        rear = front;
    return ret;
}

template <class T>
T mQueue<T>::getFront() {  // 获得队头元素
    return front->next->data;
}

template <class T>
bool mQueue<T>::isEmpty() {
    return size == 0 ? true : false;
}

template <class T>
int mQueue<T>::length() {
    return size;
}

#endif