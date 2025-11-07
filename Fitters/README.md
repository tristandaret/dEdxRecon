# Fitters — cluster and track parameter estimation

This directory contains classes and helpers that implement numerical
fitting for clusters and tracks. Fitting code wraps ROOT's minimisation
routines (TVirtualFitter/TMinuit) and contains strategies to recover
from failed minimiser calls.

Key points for a non-specialist reader:

- Goal: estimate physical parameters (position, charge distribution,
  track slope) from measured pad signals using chi² minimisation.
- Approach: fit parametric models to data measured on pads; use
  numerical minimisers with rescue strategies when fits fail.

Important files / classes

- `ClusterFitter.h` / `ClusterFitter.cxx` — horizontal and diagonal
  cluster fitters. These classes run fits on per-cluster pad charge
  distributions and compute chi² values for parameter estimates.
- `TrackFitter.h` / `TrackFitter.cxx` — track-level fitter which
  assembles cluster information and fits a track model to extract
  slope/position parameters.

Why this matters

Robust parameter estimation is at the core of turning raw sensor
readouts into physics observables. The Fitters directory showcases
practical experience with numerical stability, fit-rescue techniques
and building reusable wrappers around third-party minimisers.
