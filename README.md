# Memegen

## What does it do?

Commandline utility to add 'top' and/or 'bottom' text to an image

## Why?

Anyone who doesn't want to be forced to use online meme making tools but also doesn't want to have to fire up GIMP or some other image processor. This creates a nice middle ground.

## Install

Make sure libpng is installed on your system and then simply compile.

There is an install target in the makefile for copying to /usr/bin/ so the script is available globally.

To install locally just copy it to somewhere that your user has in the $PATH

## Usage

```
memegen <-c text color> <-i inputfile> <-o outputfile> <-t top text> <-b bottom text>
```

## How does it work

Uses libpng to load in the PNG file either from standard in or a file if specified and then uses fancy *coding* and *algorithms* to blit 8x8 text of a specific colour on to the image buffer with some scaling up if wanted.

The updated buffer is then written to either standard out or a file if specified
