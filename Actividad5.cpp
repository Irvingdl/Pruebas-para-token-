#include <algorithm>
#include <cctype>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

// Función para convertir una palabra a mayúsculas
std::string toUpper(const std::string &word) {
    std::string upperWord;
    for (char c : word) {
        upperWord += std::toupper(c);
    }
    return upperWord;
}

// Función para verificar si un string es un número
bool isNumber(const std::string &str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

// Función para contar palabras en un archivo y guardarlas en un mapa
void countWords(const std::string &directoryPath, std::unordered_map<std::string, int> &wordFreq) {
    for (const auto &entry : fs::directory_iterator(directoryPath)) {
        if (entry.path().extension() == ".txt" && entry.path().stem().string().find("_words") != std::string::npos) {
            std::ifstream file(entry.path());
            if (!file) {
                std::cerr << "Error opening file: " << entry.path() << std::endl;
                continue;
            }
            std::string word;
            while (file >> word) {
                if (std::isalpha(word[0])) {
                    std::string upperWord = toUpper(word);
                    wordFreq[upperWord]++;
                }
            }
        }
    }
}

// Función para medir tiempos
template <typename Func>
double measureTime(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

// Función para escribir los resultados en un archivo
void writeResults(const std::unordered_map<std::string, int> &wordFreq, const std::string &outputPath) {
    std::vector<std::pair<std::string, int>> sortedWords(wordFreq.begin(), wordFreq.end());

    // Ordenar alfabéticamente
    std::sort(sortedWords.begin(), sortedWords.end(), [](const auto &a, const auto &b) {
        return a.first < b.first;
    });

    std::ofstream outFile(outputPath);
    for (const auto &entry : sortedWords) {
        outFile << entry.first << ": " << entry.second << std::endl;
    }
}

// Función para escribir resultados ordenados por frecuencia
void writeResultsByFrequency(const std::unordered_map<std::string, int> &wordFreq, const std::string &outputPath) {
    std::vector<std::pair<std::string, int>> sortedWords(wordFreq.begin(), wordFreq.end());

    // Ordenar por frecuencia
    std::sort(sortedWords.begin(), sortedWords.end(), [](const auto &a, const auto &b) {
        return a.second > b.second;
    });

    std::ofstream outFile(outputPath);
    for (const auto &entry : sortedWords) {
        outFile << entry.first << ": " << entry.second << std::endl;
    }
}

int main() {
    std::unordered_map<std::string, int> wordFreq;
    std::string directoryPath = "./";
    
    // Archivo de log
    std::ofstream logFile("a5_matricula.txt");

    // Medir tiempo para consolidar palabras
    double timeToConsolidate = measureTime([&]() {
        countWords(directoryPath, wordFreq);
    });

    logFile << "Tiempo para consolidar palabras: " << timeToConsolidate << " segundos." << std::endl;

    // Medir tiempo para ordenar alfabéticamente y escribir resultados
    double timeToWriteAlpha = measureTime([&]() {
        writeResults(wordFreq, "sorted_words.txt");
    });
    logFile << "Tiempo para escribir archivo ordenado alfabéticamente: " << timeToWriteAlpha << " segundos." << std::endl;

    // Medir tiempo para ordenar por frecuencia y escribir resultados
    double timeToWriteByFrequency = measureTime([&]() {
        writeResultsByFrequency(wordFreq, "sorted_words_by_frequency.txt");
    });
    logFile << "Tiempo para escribir archivo ordenado por frecuencia: " << timeToWriteByFrequency << " segundos." << std::endl;

    double totalTime = timeToConsolidate + timeToWriteAlpha + timeToWriteByFrequency;
    logFile << "Tiempo total de ejecución: " << totalTime << " segundos." << std::endl;

    std::cout << "All tasks completed successfully." << std::endl;

    return 0;
}
