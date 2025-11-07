# Reconstruction — orchestration and analysis pipelines

This directory ties together events, models and fitters into higher
level reconstruction and analysis tasks. It contains the control and
monitoring entry points that would typically be invoked by analysis
scripts or experiment-run helpers.

Key responsibilities

- Orchestrate the full processing flow: load event data, apply
  model parameters, run cluster and track fits, and collect outputs.
- Provide monitoring and logging helpers for performance and fit
  quality assessment.

Important files

- `Control.h` / `Control.cxx` — a high-level orchestration function
  that sets up a run and invokes reconstruction steps. It accepts
  arguments such as output directory, data selection and uploader
  interface to connect to acquisition infrastructure.
- `Displays.*`, `Monitoring.*` — visualization and monitoring
  utilities for human-in-the-loop assessment.

Notes for non-technical reviewers

Reconstruction is where the models and algorithms are combined to
produce physics-ready outputs. This directory demonstrates a focus on
maintainable orchestration and observability of algorithmic behaviour.
