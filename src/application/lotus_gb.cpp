#include <iostream>
#include "IGB.hpp"
#include "gbFactory.hpp"
using namespace std;

int main(int argc, char *argv[]) {
    LOTUSGB::IGB *pGB = getGB_Obj();

    delete pGB;
    return 0;
}