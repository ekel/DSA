/*
 * list.hpp
 *
 *  Created on: 2015年11月20日
 *      Author: ikelhe84(at)gmail.com
 *      参考自deng@tsinghua.edu.cn
 */

#ifndef LIST_HPP_
#define LIST_HPP_

typedef int Rank; //秩
#define ListNodePosi(T) ListNode<T>* //列表节点位置


//列表节点模板类（以双向链表形式实现）
template <typename T>
struct ListNode
{
    // 成员
    T data;                 // 数值
    ListNodePosi(T) pred;   // 前驱
    // 后继
    ListNodePosi(T) succ;
    ListNode() {}
    ListNode (T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
          : data(e), pred(p), succ(s)
    {

    } //默认构造器
    // 操作接口
    ListNodePosi(T) insertAsPred(T const& e); //紧靠当前节点之前插入新节点
    ListNodePosi(T) insertAsSucc(T const& e); //紧随当前节点之后插入新节点
};

// 将e紧靠当前节点之前插入于当前节点所属列表（设有哨兵头节点header）
template <typename T>
ListNodePosi(T) ListNode<T>::insertAsPred(T const& e)
{
    ListNodePosi(T) x = new ListNode(e, pred, this);
    pred->succ = x;
    pred = x;

    return x;
}

// 将e紧随当前节点之后插入于当前节点所属列表（设有哨兵尾节点trailer）
template <typename T>
ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e)
{
    ListNodePosi(T) x = new ListNode(e, this, succ); //创建新节点
    succ->pred = x;
    succ = x; //设置逆向链接
    return x; //返回新节点的位置
}


//列表模板类
template <typename T>
class List
{
public:
    // 构造函数
    List() { init(); }
    List(List<T> const &L); //整体复制列表L
    List(List<T> const &L, Rank r, int n);  //复制列表L中自第r项起的n项
    List(ListNodePosi(T) p, int n);         //复制列表中自位置p起的n项
    // 析构函数
    ~List();    // 释放（包含头、尾哨兵在内的）所有节点
    // 只读访问接口
    Rank size() const { return _size; }
    bool empty() const { return _size <= 0; }
    T& operator[] (Rank r) const;   //重载，支持循秩访问（效率低）
    ListNodePosi(T) first() const { return header->succ; }      //首节点位置
    ListNodePosi(T) last() const { return trailer->pred; }      //末节点位置
    bool valid(ListNodePosi(T) p)   //判断位置p是否对外合法
    { return p && (trailer != p) && (header != p); }    //将头、尾节点等同于NULL
    int disordered() const; //判断列表是否已排序
    ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const; //无序区间查找
    ListNodePosi(T) find(T const& e) const //无序列表查找
    { return find(e, _size, trailer); }

    ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const; //有序区间查找
    ListNodePosi(T) search (T const& e) const //有序列表查找
    { return search(e, _size, trailer); }


    // 可写访问接口
    ListNodePosi(T) insertAsFirst(T const& e);  //将e当作首节点插入
    ListNodePosi(T) insertAsLast (T const& e);  //将e当作末节点插入
    ListNodePosi(T) insertA(ListNodePosi(T) p, T const& e); //将e当作p的后继插入
    ListNodePosi(T) insertB(ListNodePosi(T) p, T const& e); //将e当作p的前驱插入
    T remove(ListNodePosi(T) p); //删除合法位置p处的节点,返回被删除节点

private:
    void init(); //列表创建时的初始化
    void copyNodes (ListNodePosi(T), int); //复制列表中自位置p起的n项
    int clear();

private:
    //规模、头哨兵、尾哨兵
    int _size;
    ListNodePosi(T) header;
    ListNodePosi(T) trailer;
};

//列表初始化，在创建列表对象时统一调用
template <typename T>
void List<T>::init()
{
    header = new ListNode<T>;   //创建头哨兵节点
    trailer = new ListNode<T>;  //创建尾哨兵节点
    header->succ = trailer;
    header->pred = NULL;
    trailer->pred = header;
    trailer->succ = NULL;
    _size = 0;  //记录规模
}

//整体复制列表L
template <typename T>
List<T>::List(List<T> const &L)
{
    copyNodes(L.first(), L._size);
}

//复制L中自第r项起的n项（assert: r+n <= L._size）
template <typename T>
List<T>::List(List<T> const &L, Rank r, int n)
{
    copyNodes(L[r], n);
}

//列表内部方法：复制列表中自位置p起的n项
template <typename T>
void List<T>::copyNodes (ListNodePosi(T) p, int n)
{
    //p合法，且至少有n-1个真后继节点
    init(); //创建头、尾哨兵节点并做初始化
    while (n--)
    {
        insertAsLast(p->data);
        p = p->succ;    // 将起自p的n项依次作为末节点插入
    }
}

//重载下标操作符，以通过秩直接访问列表节点（虽方便，效率低，需慎用）
template <typename T>
T& List<T>::operator[](Rank r) const
{
    ListNodePosi(T) p = first();    //从首节点出发
    while (0 < r--)
    {
        p = p->succ;//顺数第r个节点即是
    }
    return p->data;//目标节点，返回其中所存元素
}

//在无序列表内节点p（可能是trailer）的n个（真）前驱中，找到等于e的最后者
template <typename T>
ListNodePosi(T) List<T>::find(T const& e, int n, ListNodePosi(T) p) const
{
    //逐个比对，直至命中或范围越界
    while (n-- > 0)
    {
        if (e == (p = p->pred)->data)
            return p;
    }
    return NULL; //p越出左边界意味着区间内不含e，查找失败
}

//在有序列表内节点p（可能是trailer）的n个（真）前驱中，找到不大于e的最后者
template <typename T>
ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const
{
    //对于p的最近的n个前驱，从右向左逐个比较
    while (n-- >= 0)
    {
        //直至命中、数值越界或范围越界
        if (((p = p->pred)->data) <= e)
            break;
    }
    return p;
}

//e当作首节点插入
template <typename T>
ListNodePosi(T) List<T>::insertAsFirst(T const& e)
{
    _size++;
    return header->insertAsSucc(e);
}

//e当作末节点插入
template <typename T>
ListNodePosi(T) List<T>::insertAsLast(T const& e)
{
    _size++;
    return trailer->insertAsPred(e);
}

//e当作p的后继插入（After）
template <typename T>
ListNodePosi(T) List<T>::insertA(ListNodePosi(T) p, T const& e)
{
    _size++;
    return p->insertAsSucc(e);
}

//e当作p的前驱插入（Before）
template <typename T>
ListNodePosi(T) List<T>::insertB (ListNodePosi(T) p, T const& e)
{
    _size++;
    return p->insertAsPred(e);
}

//删除合法节点p，返回其数值
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

//清空列表
template <typename T>
int List<T>::clear()
{
   int oldSize = _size;
   while ( 0 < _size )
       remove ( header->succ ); //反复删除首节点，直至列表变空
   return oldSize;
}

//列表析构器
//清空列表，释放头、尾哨兵节点
template <typename T>
List<T>::~List()
{
    clear();
    delete header;
    delete trailer;
}
#endif /* LIST_HPP_ */
