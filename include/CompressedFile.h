#include "File.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

class CompressedFile : virtual public File{
    
    //structura de nod in arbore huffman
    struct Node{
        unsigned char ch;
        size_t freq;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        bool isLeaf() const { return !left && !right; }
    };

    //arbore huffman 
    std::shared_ptr<Node> root;
    
    //code table ul 
    std::unordered_map<unsigned char, std::string> codeTable;

    //textul compresat
    std::vector<unsigned char> compressedData;

    bool compressed = false;
    size_t originalSize = 0;

    //crearea arborelui Huffman
    void buildHuffmanTree();

    //crearea tabelulu caracter - encodare
    void buildCodeTable(const std::shared_ptr<Node>& node, const std::string& prefix);

    //conversia de text in sirul de biti prescurtat
    std::string encodeToBits() const;
    std::string decodeFromBits(const std::vector<unsigned char>& compData) const;

    public:

    //constructor
    CompressedFile(const std::string& name, const std::string& content);

    //compresare + decompresarea
    void compress();
    void decompress();

    //functia de afisare date
    void display(int depth) const override;

    //getter de dimensiune
    size_t getSize() const override;

    //functiile de afisare continut compresat / decompresat
    std::string readContent() const override;
    std::string readDecompressedContent();
};