# Sanajahti Solver
------------------
This repository contains three subdirectories:

* plan/ for the plan 
* doc/  for the final documentation
* src/  for all the source code and external libraries.

## What is it?
--------------
This is a word solver for Sanajahti-game. It has very fast algorithm for solving the words from pre-defined word dictionary and it can use Android Debug Bridge and OCR for automatic recognizition, solving and inputting the solved words.

Our program should work as following:

**“Give word list and matrix as arguments to your program and it prints all the words found in
the matrix. The words are printed from longest to shortest and in alphabetical order”**

## How to build it?
-------------------
To be able to compile the program you need the additional libraries installed.

On **Debian/Ubuntu** -- other distros, see links at the end of README

```
apt-get install libleptonica-dev libtesseract-dev tesseract-ocr-fra
```

For the time being, only Tesseract v3.02 and Leptonica v1.70 are supported.


## How do you run it?
---------------------
To run the program with GUI (Graphical User Interface), just run the program from shell

```
./ssolver
```

You can also use it without GUI, using command flags and arguments. For example, solve Sanajahti game with OCR and input them to Android using

```
./ssolver -a -o 
```

More help on using it from shell, run

```
./solver -h
```

### OCR (Optical Character Recognition) and Android support
-----------------------------------------------------------
OCR for automatic matrix recognition, you need to have leptonica and tesseract libraries.
You can use tesseract with any (Sanajahti) image fetched from phone via ```-o image.png``` flag. Notice that only PNG is supported. To use automatic screenshot fetching, you need to have ADB installed and smartphone drivers working.

```
apt-get install android-tools-adb
```

Android input works with sendevent command from adb shell. Event number changes with different smartphone models and it is coded to be 3 as default. You can change it with ```-e``` flag. 

To find out **your Android's** event number, use command "adb shell getevent -l" from linux shell and touch your touchscreen, it should output multiple lines starting with the device path. E.g. "/dev/input/event3".

Currently only tested with Samsung S3 and Nexus 5. Here's an video showing the inputing on Nexus 5: https://www.youtube.com/watch?v=i1URS1P_ujY

### For developers
------------------
If you intend to develop this solver program further, using Gtest files included in src/test is recommended.
You also need to install libgtest

#### Features to be implemented (possibly)
-----------------------------------------
- Program could try to remove words that were not found in Sanajahti's own dictionary.
        For the time being program wastes time inputing words that Sanajahti's dictionary doesn't have.
- Program could work around ads independently and start a new game when possibl$
- getopt_long arguments, e.g. `--help`
- Print words to stdout same time as it inputs to Android. Would create cool atmosphere.



```
apt-get install libgtest-dev
```

#### Additional links
---------------------
You can find instructions for building tesseract and leptonica from the source if there is no compiled version on you.

Tesseract's GitHub https://github.com/tesseract-ocr/tesseracthttps://github.com/tesseract-ocr/tesseract

Leptonica's homepage http://www.leptonica.com/
