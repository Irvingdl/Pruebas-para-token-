struct PostingEntry {
    std::string filename;
    int frequency;
};

std::unordered_map<std::string, std::vector<PostingEntry>> posting_list;

// Agrega las entradas de prueba al archivo de posting
posting_list["Perro"].push_back({"108.html", 2});
posting_list["Perro"].push_back({"005.html", 2});
posting_list["Perro"].push_back({"444.html", 4});
posting_list["Perro"].push_back({"321.html", 8});

// Y así sucesivamente para los otros tokens
