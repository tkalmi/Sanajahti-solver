# Sanajahti Solver

This repository contains three subdirectories:

* plan/ for the plan 
* doc/  for the final documentation
* src/  for all the source code and external libraries.

Our program should work as following:

**“Give word list and matrix as arguments to your program and it prints all the words found in
the matrix. The words are printed from longest to shortest and in alphabetical order”**

## Features to be implemented
- Support for OCR (Optical Character Recognition)
- Support for Android (fetch the matrix automatically)
- Support for solving the words without the use of GUI.

## OCR (Optical Character Recognition) support
To use OCR for automatic matrix fetch, you need to have leptonica and tesseract libraries installed.
**Debian/Ubuntu**
```
    apt-get install libleptonica-dev libtesseract-dev tesseract-ocr-fra
```
