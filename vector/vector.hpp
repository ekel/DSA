/*
 * vector.h
 *
 *  Created on: 2015��11��17��
 *      Author: Zhiteng.He
 *      �ο����廪Junhui DENG��ʦ
 */

#ifndef VECTOR_HPP_
#define VECTOR_HPP_

typedef int Rank; //��
#define DEFAULT_CAPACITY  3 //Ĭ�ϵĳ�ʼ������ʵ��Ӧ���п�����Ϊ����

//����ģ����
template <typename T>
class Vector
{
public:
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) //����Ϊc����ģΪs������Ԫ�س�ʼΪv
    {
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v);
    }

    //�������帴��
    Vector(T const *A, Rank n)
    {
        copyFrom(A, 0, n);
    }

    // ����
    Vector(T const *A, int lo, int hi)
    {
        copyFrom(A, lo, hi);
    }

    //�������帴��
    Vector(Vector<T> const &V)
    {
        copyFrom(V._elem, 0, V._size);
    }
    // ����
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

    T &operator[] (Rank r) const; //�����±������������������������ʽ���ø�Ԫ��
    Vector<T> & operator= (Vector<T> const&); //���ظ�ֵ���������Ա�ֱ�ӿ�¡����
    Rank insert(Rank r, T const& e);        //����Ԫ��
    Rank insert(T const& e)     //Ĭ����ΪĩԪ�ز���
    {
        insert(_size, e);
    }
    Rank push_back(T const& e)
    {
        insert(_size, e);
    }

    T remove(Rank r);
    int remove(Rank lo, Rank hi);   //ɾ����������[lo, hi)֮�ڵ�Ԫ��
    void sort(Rank lo, Rank hi);
    void sort() { sort( 0, _size); } //��������
private:
    //��ģ��������������
    Rank _size;
    int _capacity;
    T *_elem;

    void copyFrom(T const *A, Rank lo, Rank hi);   //������������A[lo, hi)
    void expand();  //�ռ䲻��ʱ����

    void shrink();  //װ�����ӹ�Сʱѹ��
    void quickSort(Rank lo, Rank hi); //���������㷨

    Rank partition ( Rank lo, Rank hi ); //��㹹���㷨
};

//����������A[lo, hi)Ϊ������������
template <typename T>
void Vector<T>::copyFrom(T const *A, Rank lo, Rank hi)
{
    _elem = new T[_capacity = 2 * (hi - lo)];
    _size = 0;
    while (lo < hi)
        _elem[_size++] == A[lo++];
}

//�����ռ䲻��ʱ����
template <typename T>
void Vector<T>::expand()
{
    if (_size < _capacity)
        return;

    if (_capacity < DEFAULT_CAPACITY)
        _capacity = DEFAULT_CAPACITY; //��������С����

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
    expand(); //���б�Ҫ������
    //�Ժ���ǰ�����Ԫ��˳�κ���һ����Ԫ
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
    remove ( r, r + 1 ); //��������ɾ���㷨����Ч�ڶ�����[r, r + 1)��ɾ��
    return e; //���ر�ɾ��Ԫ��
}

template <typename T>
int Vector<T>::remove(Rank lo, Rank hi)
{
    if (lo == hi) return 0; //����Ч�ʿ��ǣ����������˻����������remove(0, 0)
    while (hi < _size)
        _elem[lo++] = _elem[hi++];

    _size = lo;
    //shrink(); //���б�Ҫ��������
    return (hi - lo); //���ر�ɾ��Ԫ�ص���Ŀ
}

//��������[lo, hi)����
template <typename T>
void Vector<T>::sort(Rank lo, Rank hi)
{
    //���ѡȡ�����㷨���ɸ��ݾ���������ص����ѡȡ������
    quickSort (lo, hi);
}

template <typename T>
void Vector<T>::quickSort(Rank lo, Rank hi)
{
    if (hi - lo < 2) return;
    Rank mi = partition (lo, hi - 1); //��[lo, hi - 1]�ڹ������
    quickSort(lo, mi); //��ǰ׺�ݹ�����
    quickSort(mi + 1, hi); //�Ժ�׺�ݹ�����
}

//��㹹���㷨��ͨ������Ԫ��λ�ù�������[lo, hi]����㣬����������
// ���м���ֵ�ŵ��м�λ��
template <typename T>
Rank Vector<T>::partition(Rank lo, Rank hi)
{
    swap(_elem[lo], _elem[lo + rand() % (hi - lo + 1)]); //��ѡһ��Ԫ������Ԫ�ؽ���
    T pivot = _elem[lo]; //����Ԫ��Ϊ��ѡ��㡪�������Ͻ�������Ч�����ѡȡ
    while (lo < hi)
    {
        // ���������ҵ� �ѱ������loԪ��ֵС�ĸ�ֵ��lo
        // Ȼ��lo+1
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
            if (_elem[lo] < pivot) //��С��pivot��ǰ����
            {
                lo++; //������չ���������
            }
            else
            {
                _elem[hi--] = _elem[lo];
            }
        }
    }
    _elem[lo] = pivot; //�����ݵ�����¼����ǰ����������֮��
    return lo;
}

#endif /* VECTOR_H_ */
