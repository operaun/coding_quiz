Coding Quiz
=================
This repository includes several programs for solving Quiz  
Currently, below two programs included
+ Vote program
+ BracketCalculation program

Program enviornment
-----------------
+ Ubuntu 16.04 (It might works well in most unix systems, and Windows too)
+ C++11
+ CMake & Make

Way to install program 
-----------------
<pre><code>cmake . && make </code></pre>

Way to unit testing (or debugging)
-----------------
Unit-test files are located in each program folder  
Include 'ENABLE_FILE_INPUT_UNITTEST' definition in CMakeList  
(Option already exist, just uncomment it)  
Automatic checking between input/ouput unittest files are not supported yet.

<pre><code>./vote_program ./Vote_testcase/1.input.txt </code></pre>

For debugging, there exist simple option in CMakeList.
