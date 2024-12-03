# HideAway++

**HideAway++** is a unique and smart C++ application designed to be a encrypting and decrypting steganography tool with integraded password cipher scrambles for encryption security. HideAway ++ takes advantage of powerful libraries like ImGui for the user interface, SndFile for Audio file encoding, and libpng for Image Encoding, giving users both a seamless and reliable application for hiding their data.

## Table of Contents

- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Building the Project](#building-the-project)
- [Running the Application](#running-the-application)
- [Usage](#usage)
- [License](#license)

## Features

- **PNG/WAV Encoding and Decodingg:** Hide your data in both audio and image file, whichever you fancy most.
- **Password Protection:** Protect your hidden data with keyed in passwords required for decrypting.
- **Vigenere Cipher Scrambler:** Your encoded passwords are even more secure through the use of an alphabetical cipher
- **Multiple Encoding Types Capabilities:** Choose to encode and decode direct messages, text file, or even automatically executed bash scripts

## Prerequisites

Before setting up **QRGuardPro**, ensure you have the following installed on your system:

- **C++ Compiler:** Compatible with C++17 (e.g., AppleClang on macOS, GCC on Linux, MSVC on Windows)
- **CMake:** Version 3.14 or higher
- **Git:** For cloning the repository
- **Homebrew (macOS Only):** Package manager for installing dependencies
- **ImGui:** For the user interface
- **Libpng:** For image processing
- **Libsndfile:** For audi processing


## Installation

## 1. Activate conda environment

```bash
conda env create -f environment.yml

conda activate steganography_envi  
```

### 2. Create a Build Directory
```bash
mkdir build
cd build
```

### 3. Configure the Project with CMake
```bash
cmake ..
```

CMake will detect all dependencies and configure the build accordingly.


### 4. Build the Project
```bash
make
```

This will compile the source code and generate the executable.



## Running the Application

After a successful build, you can run the application as follows:

  ```bash
  ./project/my_executable
  ```

## Usage

### 1. Encode Image/Audio File
- Proved directory path to image or audio file you wish to encode
- Choose whether you are encoding a direct message, text file, or bash script.
- Direct message: write in the text you wish to encode
- Text file: provide the directory path to the text file you wish to encode
- Bash script: provide the directory path to the bash script file you wish to encode
- Input the 8 character password to be linked to the encoding, and then scrambled by the cipher used

### 2. Decode Image/Audio File
- Proved directory path to image or audio file you wish to decode
- Choose whether you are decoding a direct message, text file, or bash script.
- Input the 8 character password linked to the encoding to prove user has access
- Failed Password = Hidden data is permannetly deleted and lost forever


## License

Copyright <2024> 

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

