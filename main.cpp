#include <iostream>

using namespace std;
#include "list.h"

int main()
{
    cout << "Hello World!" << endl;
    List<int> a;
    List<int> b;
    a = b;
    a = std::move(b);
    return 0;
}
