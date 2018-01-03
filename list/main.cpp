/*
 * main.cpp
 *
 *  Created on: 2015Äê11ÔÂ17ÈÕ
 *      Author: Zhiteng.He
 */


#include<iostream>
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string>
using namespace std;

#include "list.hpp"


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
    List<int> La;

	ListNodePosi(int) p = La.insertAsLast(4);
	//La.insertAsFirst(5);
	for (int i = 1; i < iMax; i++)
	{
		p = rand() % 2 ?
          La.insertB ( p, rand() % ( int ) iMax * 2 ) :
          La.insertA ( p, rand() % ( int ) iMax * 2 );
	}
    cout<<"---------------sort---------"<<endl;
}
