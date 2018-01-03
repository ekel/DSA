/*
 * main.cpp
 *
 *  Created on: 2015年11月17日
 *      Author: Zhiteng.He
 */


#include<iostream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string>
using namespace std;

#include "Vector.hpp"


int main(int argc, char* argv[])
{
    if (2 > argc)
    {
        printf( "Usage: %s <size of test>\a\a\n", argv[0]);
        return 1;
    }
    srand (time(NULL));

    int iMax = atoi(argv[1]);
    int j = 0;
    Vector<int> iVec;

    for (j = 0; j < iMax; j++)
    {
        iVec.insert(rand() % (j+1), rand() % (iMax * 3));
    }

    j = 0;
    for (; j < iVec.size(); j++)
    {
        cout<<"index: "<<j<<"\t value:"<<iVec[j]<<endl;
    }
    iVec.sort();
    cout<<"---------------sort---------"<<endl;
    j = 0;
    for (; j < iVec.size(); j++)
    {
        cout<<"index: "<<j<<"\t value:"<<iVec[j]<<endl;
    }

    iVec.remove(0);
    j = 0;
    for (; j < iVec.size(); j++)
    {
        cout<<"index: "<<j<<"\t value:"<<iVec[j]<<endl;
    }
    cout<<"capacity: "<<iVec.capacity()<<"\t size:"<<iVec.size()<<endl;
    // 以下代码会报错
    // 目前只能给基本类型使用 不支持复杂数据结构如string
    // using string
    Vector<string> strVec;
    strVec.push_back("a");
    strVec.push_back("abc");
    strVec.push_back("dd");
    strVec.push_back("ab");
    j = 0;
    for (; j < strVec.size(); j++)
    {
        cout<<"index: "<<j<<"\t value:"<<strVec[j]<<endl;
    }
}
