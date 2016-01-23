#include <iostream>
#include "Integer.hpp"
using namespace std;

int main()
{
    Integer A("123"), B("234");
    A*=B;
    
   cout <<"tak:"<<A.toString();
   return 0;
}

