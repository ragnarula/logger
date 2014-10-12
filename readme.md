# Header only Logging Library

This is a header only lightweight logging library implemented using code from http://www.drdobbs.com/cpp/a-lightweight-logger-for-c/240147505 with some modifications.

## Features
+ Easy to use Macros
+ Easy to enable/disable with definitions
+ Include log file policy to write log to text file
+ Interface class for custom log file policies
+ Multiple log file policy support

## Instructions
Please see the included main.cpp for examples of the following information.

### Macros
+ ````LOG_INIT```` this macro initializes the Logger object
+ ````LOG_INST```` provides a symbol to the same Logger object (as long as you're in the same scope)
+ ````LOG````, ````LOG_ERR````, ````LOG_WARN```` expands to a method call which will print the parametes in the parentesis that follow the macro e.g.
	````LOG("log something...");````
+ ````LOGGING_LEVEL_1````, ````LOGGING_LEVEL_2````, ````LOGGING_LEVEL_3```` These define the level or detail that is logged. They are:
	1. Only ````LOG_WARN```` messages are expanded
	2. ````LOG_WARN```` and ````LOG_ERR```` are expanded
	3. ````LOG_WARN````, ````LOG_ERROR````, and ````LOG```` are expanded

Just to say again, original credit for the code goes to Filip Janiszewski's post on drdobbs.com
