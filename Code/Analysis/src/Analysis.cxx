#include <iostream>
#include <iomanip>

#include "PID/Monitoring.h"

int main(int argc, char *argv[])
{
  std::cout << std::setiosflags(std::ios::fixed);
  PID::Monitoring();

}
