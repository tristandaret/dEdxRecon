#include <iostream>
#include <iomanip>

#include "Monitoring.h"

int main(int argc, char *argv[])
{
   std::cout << std::setiosflags(std::ios::fixed);
   Reconstruction::Monitoring();
}