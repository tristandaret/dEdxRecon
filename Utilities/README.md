# Utilities — signal processing and helper functions

Small utility functions used across the project for signal processing,
math helpers and plotting conveniences.

Examples

- `SignalTools.h` / `SignalTools.cxx` — contains helpers to build an
  electronics transfer function (ETF) and other pulse-shape utilities
  used by simulation and template fitting.
- `ParabolaFunction*`, `GaussFunction` — small math helpers used in
  fitting and diagnostics.

Why this matters

Utilities centralize small but important pieces of code so algorithms
in higher-level modules remain focused on their domain logic.
