__fpp/throwless__ is a small library with the implementation of modern solutions in order to encounter fewer exceptions in the cpp code.  
- Tested by [gtest](https://github.com/google/googletest)
- Implementation in the Rust / Haskell style (syntactically and semantically)
- Compliance with the functional style 

The library is small, it requires improvements and improvements, as well as more test coverage. There are still parts of the code marked with a TODO comment, so if you want to add to this code, feel free to send a pull request.

In development:
- functions for convenient handler call chains (parse, match, etc)
- adding asynchronous and multithreading support
- stricter functional style (adding categories through concepts, analogous to traits)