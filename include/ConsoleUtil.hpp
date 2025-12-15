#pragma once
#include <string>
#include <iostream>
#include <limits>

inline int readIntInRange(const std::string& prompt, int lo, int hi) {
    int v;
    for (;;) {
        std::cout << prompt;
        if (std::cin >> v && v >= lo && v <= hi) return v;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please enter a number between " << lo << " and " << hi << ".\n";
    }
}

inline bool readYesNo(const std::string& prompt) {
    for (;;) {
        std::cout << prompt << " (y/n): ";
        std::string s;
        if (!(std::cin >> s)) continue;
        if (s == "y" || s == "Y") return true;
        if (s == "n" || s == "N") return false;
        std::cout << "Please type y or n.\n";
    }
}

inline std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin >> std::ws, s);
    return s;
}
