# (Deprecated) Shox96 - Guaranteed Compression for Short Strings

** This library has been deprecated **. Use [Unishox](https://github.com/siara-cc/Unishox) instead.  It has same features and more.

This is a C library for compressing short strings.  It was developed to individually compress and decompress small strings. In general compression utilities such as `zip`, `gzip` do not compress short strings well and often expand them. They also use lots of memory which makes them unusable in constrained environments like Arduino.

# How it works

Shox96 is an hybrid encoder (entropy and dictionary).  It works by assigning fixed prefix-free codes for each letter in the above Character Set (entropy coding).  It also encodes repeating letter sets separately (dictionary coding).  More information is available in [this article](Shox96_Article_0_2_0.pdf?raw=true).

# Compiling

To compile, just use `make` or use gcc as follows:

```sh
gcc -o shox96 shox96_0_2.c
```

# API

```C
int shox96_0_2_compress(const char *in, int len, char *out, struct lnk_lst *prev_lines);
int shox96_0_2_decompress(const char *in, int len, char *out, struct lnk_lst *prev_lines);
```

The `lnk_list` is used only when a bunch of strings are compressed for use with Arduino Flash Memory.  Just pass `NULL` if you have only one String to compress or decompress.

# Usage

To see Shox96 in action, simply try to compress a string:

```
./shox96 "Hello World"
```

To compress and decompress a file, use:

```
./shox96 -c <input_file> <compressed_file>
./shox96 -d <compressed_file> <decompressed_file>
```

Shox96 does not give good ratios compressing files more than 512 bytes.  It can also not be used for compressing binary files.  It is only suitable for compressing Short Strings and small text files in constrained environments such as Arduino and other microcontroller based systems.  It can also be used to store compressed short strings in a database. See below for real-life projects that use Shox96.

# Character Set

As of now Shox96 works only on `A to Z`, `a to z`, `0 to 9`, special characters, in the range ASCII 32 to 126, Tab (ASCII 9), CR (ASCII 13) and LF (ASCII 10).

# Projects that use Shox96

- [Shox96 Compression Library for Arduino Progmem](https://github.com/siara-cc/Shox96_Arduino_Progmem_lib)
- [Shox96 Compression Library for Arduino](https://github.com/siara-cc/Shox96_Arduino_lib)
- [Sqlite3 User Defined Function as loadable extension](https://github.com/siara-cc/Shox96_Sqlite_UDF)
- [Sqlite3 Library for ESP32](https://github.com/siara-cc/esp32_arduino_sqlite3_lib)
- [Sqlite3 Library for ESP8266](https://github.com/siara-cc/esp_arduino_sqlite3_lib)
- [Sqlite3 Library for ESP-IDF](https://github.com/siara-cc/esp32-idf-sqlite3)
 
# Issues

In case of any issues, please email the Author (Arundale Ramanathan) at arun@siara.cc or create GitHub issue.
