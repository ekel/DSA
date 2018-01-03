/*
 * vector.h
 *
 *  Created on: 2015年11月17日
 *      Author: Zhiteng.He
 *      参考自清华Junhui DENG老师
 */

#ifndef VECTOR_HPP_
#define VECTOR_HPP_

typedef int Rank; //秩
#define DEFAULT_CAPACITY  3 //默认的初始容量（实际应用中可设置为更大）

//向量模板类
template <typename T>
class Vector
{
public:
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) //容量为c、规模为s、所有元素初始为v
    {
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v);
    }

    //数组整体复制
    Vector(T const *A, Rank n)
    {
        copyFrom(A, 0, n);
    }

    // 区间
    Vector(T const *A, int lo, int hi)
    {
        copyFrom(A, lo, hi);
    }

    //向量整体复制
    Vector(Vector<T> const &V)
    {
        copyFrom(V._elem, 0, V._size);
    }
    // 区间
    Vector(Vector<T> const &V, Rank lo, Rank hi)
    {
        copyFrom(V._elem, lo, hi);
    }

    ~Vector()
    {
        delete [] _elem;
    }

    // Interface
    Rank size() const
    {
        return _size;
    }
    int capacity() const { return _capacity; }
    bool empty()
    {
        return !_size;
    }

    T &operator[] (Rank r) const; //重载下标操作符，可以类似于数组形式引用各元素
    Vector<T> & operator= (Vector<T> const&); //重载赋值操作符，以便直接克隆向量
    Rank insert(Rank r, T const& e);        //插入元素
    Rank insert(T const& e)     //默认作为末元素插入
    {
        insert(_size, e);
    }
    Rank push_back(T const& e)
    {
        insert(_size, e);
    }

    T remove(Rank r);
    int remove(Rank lo, Rank hi);   //删除秩在区间[lo, hi)之内的元素
    void sort(Rank lo, Rank hi);
    void sort() { sort( 0, _size); } //整体排序
private:
    //规模、容量、数据区
    Rank _size;
    int _capacity;
    T *_elem;

    void copyFrom(T const *A, Rank lo, Rank hi);   //复制数组区间A[lo, hi)
    void expand();  //空间不足时扩容

    void shrink();  //装填因子过小时压缩
    void quickSort(Rank lo, Rank hi); //快速排序算法

    Rank partition ( Rank lo, Rank hi ); //轴点构造算法
};

//以数组区间A[lo, hi)为蓝本复制向量
template <typename T>
void Vector<T>::copyFrom(T const *A, Rank lo, Rank hi)
{
    _elem = new T[_capacity = 2 * (hi - lo)];
    _size = 0;
    while (lo < hi)
        _elem[_size++] == A[lo++];
}

//向量空间不足时扩容
template <typename T>
void Vector<T>::expand()
{
    if (_size < _capacity)
        return;

    if (_capacity < DEFAULT_CAPACITY)
        _capacity = DEFAULT_CAPACITY; //不低于最小容量

    T *_oldElem = _elem;
    _elem = new T[_capacity <<= 1];
    for (int i = 0; i < _size; i++)
        _elem[i] = _oldElem[i];

    delete [] _oldElem;
}

template <typename T>
T& Vector<T>::operator[] (Rank r) const
{
    return _elem[r];
}

template <typename T>
Vector<T>& Vector<T>::operator =(Vector<T> const& V)
{
    if (_elem)
        delete [] _elem;
    copyFrom(V._elem, 0, V._size);

    return *this;
}

template <typename T>
Rank Vector<T>::insert(Rank r, T const& e)
{
    expand(); //若有必要，扩容
    //自后向前，后继元素顺次后移一个单元
    for (int i = _size; i > r; i--)
        _elem[i] = _elem[i-1];
    _elem[r] = e;
    _size++;
    return r;
}

template <typename T>
T Vector<T>::remove(Rank r)
{
    T e = _elem[r];
    remove ( r, r + 1 ); //调用区间删除算法，等效于对区间[r, r + 1)的删除
    return e; //返回被删除元素
}

template <typename T>
int Vector<T>::remove(Rank lo, Rank hi)
{
    if (lo == hi) return 0; //出于效率考虑，单独处理退化情况，比如remove(0, 0)
    while (hi < _size)
        _elem[lo++] = _elem[hi++];

    _size = lo;
    //shrink(); //若有必要，则缩容
    return (hi - lo); //返回被删除元素的数目
}

//向量区间[lo, hi)排序
template <typename T>
void Vector<T>::sort(Rank lo, Rank hi)
{
    //随机选取排序算法。可根据具体问题的特点灵活选取或扩充
    quickSort (lo, hi);
}

template <typename T>
void Vector<T>::quickSort(Rank lo, Rank hi)
{
    if (hi - lo < 2) return;
    Rank mi = partition (lo, hi - 1); //在[lo, hi - 1]内构造轴点
    quickSort(lo, mi); //对前缀递归排序
    quickSort(mi + 1, hi); //对后缀递归排序
}

//轴点构造算法：通过调整元素位置构造区间[lo, hi]的轴点，并返回其秩
// 把中间数值放到中间位置
template <typename T>
Rank Vector<T>::partition(Rank lo, Rank hi)
{
    swap(_elem[lo], _elem[lo + rand() % (hi - lo + 1)]); //任选一个元素与首元素交换
    T pivot = _elem[lo]; //以首元素为候选轴点——经以上交换，等效于随机选取
    while (lo < hi)
    {
        // 从右往左找到 把比最左边lo元素值小的赋值给lo
        // 然后lo+1
        while (lo < hi)
        {
            if (pivot < _elem[hi])
                hi--;
            else
            {
                _elem[lo++] = _elem[hi];
                break;
            }
        }

        while (lo < hi)
        {
            if (_elem[lo] < pivot) //在小于pivot的前提下
            {
                lo++; //向右拓展左端子向量
            }
            else
            {
                _elem[hi--] = _elem[lo];
            }
        }
    }
    _elem[lo] = pivot; //将备份的轴点记录置于前、后子向量之间
    return lo;
}

#endif /* VECTOR_H_ */
