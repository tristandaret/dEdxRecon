#include <iostream>
#include <iomanip>

#include "Reconstruction/Monitoring.h"

int main(int argc, char *argv[])
{
  std::cout << std::setiosflags(std::ios::fixed);
  Reconstruction::Monitoring();
}