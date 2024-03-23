<a name="readme-top"></a>

# Secure Password Manager

## Description
This project is a password manager application developed in C++ as part of the coursework for KH5062CEM Programming and Algorithms. It is designed to securely manage and store your passwords.


<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#installation">Installation</a></li>
    <ul>
        <li><a href="#dependencies">Dependencies</a></li>
        <li><a href="#building">Building</a></li>
      </ul>
    <li>
      <a href="#usage">Usage</a>
    </li>
    <li>
      <a href="#usage">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
        <li><a href="#file-structure">File Structure</a></li>
        <li><a href="#features">Features</a></li>
      </ul>
    </li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Installation

### Dependencies
Before installing the Secure Password Manager, you must install Crypto++ library on your Linux system.

1. Update your packaged list:
   ```bash
   sudo apt-get update
   ```
2. Install the Crypto++ library:
   ```bash
   sudo apt-get install libcrypto++-dev libcrypto++-doc libcrypto++-utils
   ```

### Building 

Alternatively, a compiled binary is found in the `bin` directory. This binary was compiled and tested on Ubuntu 20.04.

1. Clone the repository
   ```bash
   git clone https://github.com/IryVk/Secure-Password-Manager.git
   ```
2. Navigate to project directory
   ```bash
   cd Secure-Password-Manager
   ```
3. Build the project
   ```bash
   make
   ```
4. Build the unit tests
   ```bash
   cd tests && make && cd ..
   ```
5. Run the compiled binaries.
   ```bash
   # the main program
   ./password_manager
   # unittests
   tests/run_tests
   ```
<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Usage

DEMO VIDEO HERE

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## About The Project

### Built With

+ <img src="https://img.shields.io/badge/-C++-blue?logo=cplusplus">

### File Structure

```bash
Secure-Password-Manager/  # root
├── assets/  # user files are stored here
├── bin/  # compiled binaries
├── include/  # header files
├── lib/  # compiled static libraries
├── obj/  # compiled object files
├── src/  # source cpp files
├── test/  # unittests
│   └── Makefile  # make file for unit tests
├── .gitignore
├── LICENSE
├── Makefile  # make file for program
└── README.md
```

### Features

+ Handles passwords for multiple users
+ Encrypts and authenticates password files using AES in GCM mode
+ Generates secure random passwords and saves them
+ Add, Update or Delete passwords
+ Fast password lookup using hashtable and linked list (O(1) lookup)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Contact

Arwa Essam Abdelaziz

aa2101585@tkh.edu.eg - arwa.abdelaziz.03@gmail.com

<p align="right">(<a href="#readme-top">back to top</a>)</p>
