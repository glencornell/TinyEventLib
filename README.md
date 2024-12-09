# Tiny Event Library

This is a rewrite of the event processing loop in Qt Core.  I wrote it
as a way to educate myself on modern C++.

# TODO:
- [ ] Refactor to run in memory-constrained environments (i.e. Arduino) [staticFunctional](https://github.com/luni64/staticFunctional) is a good example
- [ ] Add signal handlers
- [ ] Consider refactoring implementation of QTimer with timerfd
