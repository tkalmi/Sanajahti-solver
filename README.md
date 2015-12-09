# Sanajahti Solver

This repository contains three subdirectories:

* plan/ for the plan 
* doc/  for the final documentation
* src/  for all the source code and external libraries.

Our program should work as following:

**“Give word list and matrix as arguments to your program and it prints all the words found in
the matrix. The words are printed from longest to shortest and in alphabetical order”**

To be able to compile the program you need the additional libraries installed.

On **Debian/Ubuntu**

```
    apt-get install libleptonica-dev libtesseract-dev tesseract-ocr-fra 
```

## Features to be implemented (possibly)

- Program could try to remove words that were not found in Sanajahti's own dictionary. E.g. use OCR after game to determine the words that Sanajahti approves.
	For the time being program wastes time inputing words that Sanajahti does not accept.
- Program could work around ads independently and start a new game when possible. _Setup and forget -style_
- getopt_long arguments, e.g. --help


## OCR (Optical Character Recognition) support

OCR for automatic matrix recognition, you need to have leptonica and tesseract libraries.
You can use tesseract with any (Sanajahti) image fetched from phone via ```-o image.png``` flag. Notice that only PNG is supported.

## For developers

If you intend to develop this solver program further, using Gtest files included in src/test is recommended.
You also need to install libgtest
```
    apt-get install libgtest-dev
```
