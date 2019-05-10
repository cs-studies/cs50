# Questions

## What's `stdint.h`?

This is a C standard library header file, which is included in order to get access to specified width integer data types: exact-width, minimum-width, fastest minimum-width, greatest-width, and capable of holding object pointers. The basic role of these data types is to guarantee a portability of a program between systems.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

These data types reserve the same amount of bites in memory as the data types used by Microsoft implementation of BMP file format. The latter consists of known size structures, that's why fixed size data types are in use.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

`BYTE` - 1 byte (8 bit), `DWORD` - 4 bytes (32 bit), `LONG` - 4 bytes (32 bit), `WORD` - 2 bytes (16 bit)

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

`BM` in ASCII, `0x42 0x4D` in hexadecimal

## What's the difference between `bfSize` and `biSize`?

`bfSize` is a part of `BITMAPFILEHEADER`. It contains a full size of a `BMP` file (including headers) in bytes. `biSize` is a part of `BITMAPINFOHEADER`. It contains a size of `BITMAPINFOHEADER` only, in bytes.

## What does it mean if `biHeight` is negative?

It means that bitmap is stored starting from top to bottom. In other words, an image's top part corresponds to a bitmap's top part.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

`biBitCount`

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

It might happen if `fopen` couldn't open (or create) a file. For example, the wrong file name was provided and it couldn't be found.

## Why is the third argument to `fread` always `1` in our code?

Because we read one `BMP` structure element each time. The size of an element is passed as the second argument to `fread`. The third argument defines how many of such elements should be read.

## What value does line 65 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

3 bytes of padding. Because `3 px of bi.biWidth` + `3 bytes per pixel` + `3 bytes of padding` = `12 bytes of memory`. Resulting size 12 matches a requirement of the BMP format for a 'scanline' size to be a multiple of 4.

## What does `fseek` do?

It sets the file position marker of a given stream (e.g. open file).

## What is `SEEK_CUR`?

`SEEK_CUR` is a symbolic constant, accepted, for example, as the third parameter by `fseek` function. It specifies that offset will be calculated relative to the *current file position*.

## Whodunit?

The irony...
'T was Zamyla
whodunit!
