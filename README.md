# dEdxRecon — Detector dE/dx Reconstruction Portfolio

This repository is a portfolio of reconstruction, simulation and fitting
tools developed during my PhD thesis while working on gaseous detector readout and
charge-collection studies. It is intended as a technical showcase — not a packaged product or
production software distribution. The code is research-era C++ built with
CMake and documented with Doxygen; compiled binaries and run scripts are
not provided here as part of a released artifact.

## Executive summary

- Purpose: explore and implement algorithms for reconstructing charge
  deposits on pad-based readout electronics, fit cluster and track
  parameters, and evaluate different electronics and geometry models.
- Audience: this is a technical portfolio for hiring managers,
  product leads and collaborators interested in signal-processing,
  reconstruction algorithms and system-level design tradeoffs.
- Not intended to be run as a product: the repository contains
  experimental code, analysis entries and internal tooling used as
  research artifacts.

## Highlights

- Modular architecture separated into Models (geometry & electronics),
  Events/Modules/Tracks, Fitter implementations and high-level
  reconstruction orchestration.
- Uses ROOT for numerical minimisation and histogramming (fitting
  interfaces wrap ROOT's TVirtualFitter/TMinuit).
- Doxygen-generated API docs included under `docs/doxygen/html` to
  browse class and function-level documentation (useful for non-code
  readers).

## High-level architecture (plain language)

- Apps: small analysis drivers and top-level entry points that invoke
  reconstruction or monitoring flows.
- ModelSignal: models describing electronics response and charge
  distributions used for simulation and fitting.
- ModelEvent: data containers (Event, Module, Cluster, Track) that
  represent a single acquisition/measurement and its derived objects.
- Fitters: classes implementing cluster and track fits (wrapping
  ROOT minimisers) and utilities to run fits across samples and events.
- Reconstruction: orchestration code that ties models, fitters and
  events together into a processing pipeline.
- Uploader: helper code used to interface with experimental acquisition
  and test frameworks (mock and production uploaders).
- Utilities: small signal-processing helpers, maths and plotting helpers.

## What this shows about my skills

- Systems thinking: the repo shows separation between models, data
  containers and algorithms (fitters and orchestration).
- Applied numerical methods: practical use of minimisation, chi²
  evaluation and fit-rescue strategies for robust parameter estimation.
- Signal processing experience: electronics response modelling and
  pulse-shaping utilities.
- Documentation mindset: Doxygen comments and generated HTML docs are
  included for inspection by reviewers.

## How to inspect (non-technical)

1. Open `docs/doxygen/html/index.html` in a browser to read the
   generated API documentation.
2. Browse the subdirectory READMEs for plain-language descriptions of
   each module and example entry points.

## Technical notes (for engineers)

- Language: modern C++ (project uses ROOT types heavily).
- Build system: CMake is present at the repository root.
- Tests: this is a research snapshot; unit tests and CI are not
  included. Code is intended to be read and evaluated rather than
  executed as a packaged product.

## Contact / provenance

If you would like a walkthrough, links to publications/presentations,
or to see runnable examples, I can prepare a short demo and curated
build instructions on request.

---
This repository is maintained as a personal technical portfolio and not
as a product distribution. Any intent to reuse code for production
should be followed by code hardening, dependency pinning and a proper
release process.
