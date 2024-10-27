#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <set>
#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>


namespace fs = std::filesystem;
// irving code
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

// Función para verificar si un string contiene caracteres especiales
bool isSpecialCharacter(const std::string &str) {
  return !std::isalnum(str[0]);
}

// Consolidar y ordenar palabras
void consolidateWords() {
  std::vector<std::string> words;
  std::string directoryPath = "./";

  std::cout << "Consolidating words from text files..." << std::endl;

  for (const auto &entry : fs::directory_iterator(directoryPath)) {
    if (entry.path().extension() == ".txt" &&
        entry.path().stem().string().find("_words") != std::string::npos) {
      std::ifstream file(entry.path());
      if (!file) {
        std::cerr << "Error opening file: " << entry.path() << std::endl;
        continue;
      }
      std::string word;
      while (file >> word) {
        if (std::isalpha(word[0])) {
          words.push_back(toUpper(word));
        }
      }
    }
  }

  std::sort(words.begin(), words.end());

  std::ofstream outFile("sorted_words.txt");
  for (const auto &word : words) {
    outFile << word << std::endl;
  }

  std::cout << "Words consolidated and sorted successfully." << std::endl;
}

// Consolidar y ordenar números con validación
void consolidateNumbers() {
  std::vector<int> numbers;
  std::string directoryPath = "./";

  std::cout << "Consolidating numbers from text files..." << std::endl;

  for (const auto &entry : fs::directory_iterator(directoryPath)) {
    if (entry.path().extension() == ".txt" &&
        entry.path().stem().string().find("_words") != std::string::npos) {
      std::ifstream file(entry.path());
      if (!file) {
        std::cerr << "Error opening file: " << entry.path() << std::endl;
        continue;
      }
      std::string token;
      while (file >> token) {
        if (isNumber(token)) {
          try {
            int number = std::stoi(token);
            numbers.push_back(number); // Si el valor es correcto, agregarlo
          } catch (const std::invalid_argument &e) {
            std::cerr << "Invalid number format in file: " << token
                      << std::endl;
          } catch (const std::out_of_range &e) {
            std::cerr << "Number out of range in file: " << token << std::endl;
          }
        }
      }
    }
  }

  std::sort(numbers.begin(), numbers.end());

  std::ofstream outFile("sorted_numbers.txt");
  for (const auto &number : numbers) {
    outFile << number << std::endl;
  }

  std::cout << "Numbers consolidated and sorted successfully." << std::endl;
}

// Consolidar y ordenar caracteres especiales
void consolidateSpecialCharacters() {
  std::vector<std::string> specialChars;
  std::string directoryPath = "./";

  std::cout << "Consolidating special characters from text files..."
            << std::endl;

  for (const auto &entry : fs::directory_iterator(directoryPath)) {
    if (entry.path().extension() == ".txt" &&
        entry.path().stem().string().find("_words") != std::string::npos) {
      std::ifstream file(entry.path());
      if (!file) {
        std::cerr << "Error opening file: " << entry.path() << std::endl;
        continue;
      }
      std::string token;
      while (file >> token) {
        if (isSpecialCharacter(token)) {
          specialChars.push_back(token);
        }
      }
    }
  }

  std::sort(specialChars.begin(), specialChars.end());

  std::ofstream outFile("sorted_special_characters.txt");
  for (const auto &specialChar : specialChars) {
    outFile << specialChar << std::endl;
  }

  std::cout << "Special characters consolidated and sorted successfully."
            << std::endl;
}

// Letter counter

void findRepeatedLetters(const std::string& word) {
    std::unordered_map<char, int> letterCount;
    std::vector<char> repeatedLetters;

    for (char c : word) {
        if (isalpha(c)) { 
            letterCount[c]++;
        }
    }
    for (const auto& pair : letterCount) {
        if (pair.second > 1) {
            repeatedLetters.push_back(pair.first);
        }
    }
    if (!repeatedLetters.empty()) {
            std::cout << "Letras repetidas en '" << word << "': ";
            for (char c : repeatedLetters) {
                std::cout << c << " ";
            }
            std::cout << std::endl;
        } else {
            std::cout << "No hay letras repetidas en '" << word << "'." << std::endl;
        }
    }

    void processFile(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;

        if (!file.is_open()) {
            std::cerr << "No se pudo abrir el archivo: " << filename << std::endl;
            return;
        }
        while (std::getline(file, line)) {
                std::istringstream stream(line);
                std::string word;

                while (stream >> word) {
                    findRepeatedLetters(word);
                }
            }

            file.close();
        }




int main() {
  std::cout << "Processing text files..." << std::endl;

  consolidateWords();
  consolidateNumbers();
  consolidateSpecialCharacters();

    processFile("archivos_words.txt");
    processFile("clases_words.txt");


  std::cout << "All tasks completed successfully." << std::endl;

  return 0;
}
