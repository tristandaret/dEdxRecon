#/***************************************************************************
 * File: Analysis.cxx
 * Project: dEdxRecon
 *
 * Brief: Small executable entrypoint used to run analysis utilities.
 *        This file defines main() which configures i/o formatting and
 *        invokes the top-level monitoring routine from the Reconstruction
 *        module.
 *
 * Contents: main(int argc, char *argv[])
 *
 * Notes: This is a lightweight runner; most functionality is in
 *        Reconstruction::Monitoring().
 ***************************************************************************/

#include <iomanip>
#include <iostream>

#include "Monitoring.h"

int main(int argc, char *argv[]) {
  std::cout << std::setiosflags(std::ios::fixed);
  Reconstruction::Monitoring();
}