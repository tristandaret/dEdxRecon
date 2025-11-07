# Uploader — acquisition and mock interfaces

This directory contains code used to interface with data acquisition
systems and testing frameworks. Implementations include mock uploaders
and a prototype uploader for specific hardware setups.

Purpose

- Provide an adapter layer between raw acquisition outputs and the
  repository's `Event` containers.
- Include mock implementations for offline testing and integration
  with external systems.

Key files

- `GiveMe_Uploader.*` — convenience utilities for test data injection.
- `Uploader_*.cxx` — various uploader implementations (mock/prototype).

This area demonstrates integration experience with experiment
acquisition chains and test harnesses.
