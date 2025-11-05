/***************************************************************************
 * File: DrawOut.cxx
 * Project: dEdxRecon
 *
 * Brief: Small executable that invokes the Reconstruction::DrawOuts
 *        functionality. Acts as a top-level runner for drawing/visual
 *        output generation.
 *
 * Contents: main(int argc, char *argv[])
 *
 * Notes: Drawing code and configuration live in Reconstruction::DrawOuts().
 ***************************************************************************/

#include "DrawOuts.h"

int main(int argc, char *argv[]) { Reconstruction::DrawOuts(); }