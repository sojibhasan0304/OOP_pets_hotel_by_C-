# OOP Pets Hotel (C++)

A **console-based Pet Hotel Management System** built in **C++** following **Object-Oriented Programming (OOP)** principles.
The project is structured with separate **`include/`** (headers) and **`src/`** (implementations) folders and uses **CMake** for building.

## Project Overview

This application simulates basic operations of a pet hotel—managing **pets**, **customers**, and daily care-related tasks.
It demonstrates core OOP concepts such as **encapsulation**, **inheritance**, **polymorphism**, and **composition** through separate classes for different entities.

## Key OOP Concepts Demonstrated

* **Encapsulation:** Data and behaviors are organized into classes (e.g., Pet, Customer, Employee).
* **Inheritance & Polymorphism:** Specialized pet types (e.g., `Dog`, `Cat`) extend common pet behavior.
* **Modular Design:** Headers in `include/` and implementations in `src/`.
* **Reusability:** Utility/helper classes (e.g., console/date utilities) for cleaner code.

## Features

* **Pet Management**

  * Add and manage pet information (e.g., name, type, details)
  * Separate classes for different pet types like **Dog** and **Cat**
* **Customer Management**

  * Store customer details and associate pets with owners
* **Care Scheduling**

  * Manage basic daily care tasks/schedules (e.g., routines, schedules)
* **Employee/Staff Structure**

  * Includes employee-related structure/classes (if used in the program flow)
* **Console-Based Interaction**

  * Program runs through terminal/command line with menu-style operations

## Project Structure

```
OOP_pets_hotel_by_C-/
├─ include/               # Header files (.hpp)
│  ├─ Cat.hpp
│  ├─ Dog.hpp
│  ├─ Customer.hpp
│  ├─ Employee.hpp
│  ├─ CareSchedule.hpp
│  ├─ ConsoleUtil.hpp
│  └─ DateUtil.hpp
├─ src/                   # Source files (.cpp)
├─ main.cpp               # Entry point
└─ CMakeLists.txt         # CMake build configuration
```

## Requirements

* A C++ compiler that supports **C++17** 

  * GCC / MinGW / Clang / MSVC
* **CMake** 

## Build and Run (CMake)

From the project root folder:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```
## Author

* **sojibhasan0304**
