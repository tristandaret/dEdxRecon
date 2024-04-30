#include "PID/Tools.h"

/* Equivalent of numpy linspace (npoints uniformly spaced between start and end) */
std::vector<double> linspace(double start, double end, int numPoints) {
	std::vector<double> result(numPoints);
	double step = (end - start) / (numPoints - 1);
  
	for (int i = 0; i < numPoints; ++i) {
		result[i] = start + i * step;
  	}
  return result;
}