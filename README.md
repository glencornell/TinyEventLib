# Tiny Event Library

This is a rewrite of the event processing loop in Qt Core using C++20.  It's not a clone of Qt and was not intended to
be one.  Rather, I wrote it as a way to educate myself on modern C++ and explore how the Qt framework could work if it
were implemented in C++20.

Don't use this. Use Qt or CopperSpice instead. It's not thread safe and is missing a ton of features. Having said that,
I would like to port this to Arduino as a fun exercise.

# TODO:
- [ ] Implement QSignal using function_ref & add suppoprt for capturing class instance and method pointers.
