# ModelEvent — event and data containers

This directory defines the in-memory data structures used to represent
an acquisition `Event` and its contents: `Module`, `Pad`, `Cluster` and
`Track`. These classes hold the measured data and pointers to model
objects (geometry, electronics, charge) used during simulation and
reconstruction.

Highlights

- `Event` aggregates modules and tracks for a single acquisition and
  provides helper methods to access or validate module data.
- The models referenced by Events (geometry, electronics, charge)
  enable simulation and alignment between measured data and expected
  detector behaviour.

Typical uses in the project

- Events are created or read from acquisition files, then passed to
  Fitters and Reconstruction code to extract physics parameters.
- They are the central data exchange format in the repository.
