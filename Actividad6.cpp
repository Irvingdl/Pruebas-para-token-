#include <algorithm>
#include <cctype>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
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

// Función para contar palabras y archivos donde aparecen
void countWordsInFiles(const std::string &directoryPath, std::unordered_map<std::string, std::pair<int, int>> &wordData) {
    for (const auto &entry : fs::directory_iterator(directoryPath)) {
        if (entry.path().extension() == ".txt" && entry.path().stem().string().find("_words") != std::string::npos) {
            std::ifstream file(entry.path());
            if (!file) {
                std::cerr << "Error opening file: " << entry.path() << std::endl;
                continue;
            }

            std::set<std::string> uniqueWordsInFile;
            std::string word;

            while (file >> word) {
                if (std::isalpha(word[0])) {
                    std::string upperWord = toUpper(word);
                    wordData[upperWord].first++; // Contar la frecuencia total
                    uniqueWordsInFile.insert(upperWord); // Guardar el token en este archivo
                }
            }

            // Incrementar el conteo de archivos para cada palabra única en este archivo
            for (const auto &uniqueWord : uniqueWordsInFile) {
                wordData[uniqueWord].second++;
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

// Función para escribir el diccionario
void writeDictionary(const std::unordered_map<std::string, std::pair<int, int>> &wordData, const std::string &outputPath) {
    std::ofstream outFile(outputPath);

    // Escribir el encabezado
    outFile << "Token|Repeticiones|# de archivos\n";

    for (const auto &entry : wordData) {
        outFile << entry.first << "|" << entry.second.first << "|" << entry.second.second << "\n";
    }
}

int main() {
    std::unordered_map<std::string, std::pair<int, int>> wordData; // <palabra, <frecuencia, # archivos>>
    std::string directoryPath = "./";

    // Archivo de log
    std::ofstream logFile("a6_matricula.txt");

    // Medir tiempo para procesar archivos y contar palabras
    double timeToProcess = measureTime([&]() {
        countWordsInFiles(directoryPath, wordData);
    });

    logFile << "Tiempo para procesar archivos y contar palabras: " << timeToProcess << " segundos." << std::endl;

    // Medir tiempo para escribir el diccionario
    double timeToWrite = measureTime([&]() {
        writeDictionary(wordData, "dictionary.txt");
    });

    logFile << "Tiempo para escribir el diccionario: " << timeToWrite << " segundos." << std::endl;

    double totalTime = timeToProcess + timeToWrite;
    logFile << "Tiempo total de ejecución: " << totalTime << " segundos." << std::endl;

    std::cout << "All tasks completed successfully." << std::endl;

    return 0;
}
