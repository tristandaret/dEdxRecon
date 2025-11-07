# ModelSignal — geometry and electronics models

This directory contains models used to simulate and interpret pad
readout signals. These models capture two important domains:

- Readout geometry (pad layout, sizes and indexing) that maps physical
  positions to pad indices and coordinates.
- Electronics response (pulse shapes, ETF helpers) used to simulate
  the detector and to build template responses used in fits.

Key files

- `Model_ReadOutGeometry.h` — describes pad dimensions, indices and
  helpers to query pad centre and edges. Useful for mapping measured
  charges back to physical positions.
- `Model_Electronics` and `Model_ChargeI/0D/1D` (in this folder) —
  represent electronics impulse responses and charge-distribution
  models used during fitting and simulation.

Why this matters

Accurate models let reconstruction algorithms interpret raw
measurements in physical units and allow comparisons with simulated
signals. This directory highlights the modelling and calibration side
of detector reconstruction.
