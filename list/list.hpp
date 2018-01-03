/*
 * list.hpp
 *
 *  Created on: 2015��11��20��
 *      Author: ikelhe84(at)gmail.com
 *      �ο���deng@tsinghua.edu.cn
 */

#ifndef LIST_HPP_
#define LIST_HPP_

typedef int Rank; //��
#define ListNodePosi(T) ListNode<T>* //�б�ڵ�λ��


//�б�ڵ�ģ���ࣨ��˫��������ʽʵ�֣�
template <typename T>
struct ListNode
{
    // ��Ա
    T data;                 // ��ֵ
    ListNodePosi(T) pred;   // ǰ��
    // ���
    ListNodePosi(T) succ;
    ListNode() {}
    ListNode (T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
          : data(e), pred(p), succ(s)
    {

    } //Ĭ�Ϲ�����
    // �����ӿ�
    ListNodePosi(T) insertAsPred(T const& e); //������ǰ�ڵ�֮ǰ�����½ڵ�
    ListNodePosi(T) insertAsSucc(T const& e); //���浱ǰ�ڵ�֮������½ڵ�
};

// ��e������ǰ�ڵ�֮ǰ�����ڵ�ǰ�ڵ������б������ڱ�ͷ�ڵ�header��
template <typename T>
ListNodePosi(T) ListNode<T>::insertAsPred(T const& e)
{
    ListNodePosi(T) x = new ListNode(e, pred, this);
    pred->succ = x;
    pred = x;

    return x;
}

// ��e���浱ǰ�ڵ�֮������ڵ�ǰ�ڵ������б������ڱ�β�ڵ�trailer��
template <typename T>
ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e)
{
    ListNodePosi(T) x = new ListNode(e, this, succ); //�����½ڵ�
    succ->pred = x;
    succ = x; //������������
    return x; //�����½ڵ��λ��
}


//�б�ģ����
template <typename T>
class List
{
public:
    // ���캯��
    List() { init(); }
    List(List<T> const &L); //���帴���б�L
    List(List<T> const &L, Rank r, int n);  //�����б�L���Ե�r�����n��
    List(ListNodePosi(T) p, int n);         //�����б�����λ��p���n��
    // ��������
    ~List();    // �ͷţ�����ͷ��β�ڱ����ڵģ����нڵ�
    // ֻ�����ʽӿ�
    Rank size() const { return _size; }
    bool empty() const { return _size <= 0; }
    T& operator[] (Rank r) const;   //���أ�֧��ѭ�ȷ��ʣ�Ч�ʵͣ�
    ListNodePosi(T) first() const { return header->succ; }      //�׽ڵ�λ��
    ListNodePosi(T) last() const { return trailer->pred; }      //ĩ�ڵ�λ��
    bool valid(ListNodePosi(T) p)   //�ж�λ��p�Ƿ����Ϸ�
    { return p && (trailer != p) && (header != p); }    //��ͷ��β�ڵ��ͬ��NULL
    int disordered() const; //�ж��б��Ƿ�������
    ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const; //�����������
    ListNodePosi(T) find(T const& e) const //�����б����
    { return find(e, _size, trailer); }

    ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const; //�����������
    ListNodePosi(T) search (T const& e) const //�����б����
    { return search(e, _size, trailer); }


    // ��д���ʽӿ�
    ListNodePosi(T) insertAsFirst(T const& e);  //��e�����׽ڵ����
    ListNodePosi(T) insertAsLast (T const& e);  //��e����ĩ�ڵ����
    ListNodePosi(T) insertA(ListNodePosi(T) p, T const& e); //��e����p�ĺ�̲���
    ListNodePosi(T) insertB(ListNodePosi(T) p, T const& e); //��e����p��ǰ������
    T remove(ListNodePosi(T) p); //ɾ���Ϸ�λ��p���Ľڵ�,���ر�ɾ���ڵ�

private:
    void init(); //�б���ʱ�ĳ�ʼ��
    void copyNodes (ListNodePosi(T), int); //�����б�����λ��p���n��
    int clear();

private:
    //��ģ��ͷ�ڱ���β�ڱ�
    int _size;
    ListNodePosi(T) header;
    ListNodePosi(T) trailer;
};

//�б��ʼ�����ڴ����б����ʱͳһ����
template <typename T>
void List<T>::init()
{
    header = new ListNode<T>;   //����ͷ�ڱ��ڵ�
    trailer = new ListNode<T>;  //����β�ڱ��ڵ�
    header->succ = trailer;
    header->pred = NULL;
    trailer->pred = header;
    trailer->succ = NULL;
    _size = 0;  //��¼��ģ
}

//���帴���б�L
template <typename T>
List<T>::List(List<T> const &L)
{
    copyNodes(L.first(), L._size);
}

//����L���Ե�r�����n�assert: r+n <= L._size��
template <typename T>
List<T>::List(List<T> const &L, Rank r, int n)
{
    copyNodes(L[r], n);
}

//�б��ڲ������������б�����λ��p���n��
template <typename T>
void List<T>::copyNodes (ListNodePosi(T) p, int n)
{
    //p�Ϸ�����������n-1�����̽ڵ�
    init(); //����ͷ��β�ڱ��ڵ㲢����ʼ��
    while (n--)
    {
        insertAsLast(p->data);
        p = p->succ;    // ������p��n��������Ϊĩ�ڵ����
    }
}

//�����±����������ͨ����ֱ�ӷ����б�ڵ㣨�䷽�㣬Ч�ʵͣ������ã�
template <typename T>
T& List<T>::operator[](Rank r) const
{
    ListNodePosi(T) p = first();    //���׽ڵ����
    while (0 < r--)
    {
        p = p->succ;//˳����r���ڵ㼴��
    }
    return p->data;//Ŀ��ڵ㣬������������Ԫ��
}

//�������б��ڽڵ�p��������trailer����n�����棩ǰ���У��ҵ�����e�������
template <typename T>
ListNodePosi(T) List<T>::find(T const& e, int n, ListNodePosi(T) p) const
{
    //����ȶԣ�ֱ�����л�ΧԽ��
    while (n-- > 0)
    {
        if (e == (p = p->pred)->data)
            return p;
    }
    return NULL; //pԽ����߽���ζ�������ڲ���e������ʧ��
}

//�������б��ڽڵ�p��������trailer����n�����棩ǰ���У��ҵ�������e�������
template <typename T>
ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const
{
    //����p�������n��ǰ����������������Ƚ�
    while (n-- >= 0)
    {
        //ֱ�����С���ֵԽ���ΧԽ��
        if (((p = p->pred)->data) <= e)
            break;
    }
    return p;
}

//e�����׽ڵ����
template <typename T>
ListNodePosi(T) List<T>::insertAsFirst(T const& e)
{
    _size++;
    return header->insertAsSucc(e);
}

//e����ĩ�ڵ����
template <typename T>
ListNodePosi(T) List<T>::insertAsLast(T const& e)
{
    _size++;
    return trailer->insertAsPred(e);
}

//e����p�ĺ�̲��루After��
template <typename T>
ListNodePosi(T) List<T>::insertA(ListNodePosi(T) p, T const& e)
{
    _size++;
    return p->insertAsSucc(e);
}

//e����p��ǰ�����루Before��
template <typename T>
ListNodePosi(T) List<T>::insertB (ListNodePosi(T) p, T const& e)
{
    _size++;
    return p->insertAsPred(e);
}

//ɾ���Ϸ��ڵ�p����������ֵ
template <typename T>
T List<T>::remove(ListNodePosi(T) p)
{
    T e = p->data;
    p->pred->succ = p->succ;
    p->succ->pred = p->pred;
    delete p;
    _size--;
    return e;
}

//����б�
template <typename T>
int List<T>::clear()
{
   int oldSize = _size;
   while ( 0 < _size )
       remove ( header->succ ); //����ɾ���׽ڵ㣬ֱ���б���
   return oldSize;
}

//�б�������
//����б��ͷ�ͷ��β�ڱ��ڵ�
template <typename T>
List<T>::~List()
{
    clear();
    delete header;
    delete trailer;
}
#endif /* LIST_HPP_ */
