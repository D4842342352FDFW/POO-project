#include "../include/CompressedFile.h"
#include <iostream>
#include <queue>
#include <bitset>
#include <cstring>

//crearea tabelui encodarilor
void CompressedFile::buildCodeTable(const std::shared_ptr<Node>& node, const std::string& prefix)
{
    if(!node)
    {
        return;
    }
    if(node->isLeaf())
    {
        codeTable[node->ch] = prefix;
        return;
    }
    buildCodeTable(node->left, prefix + "0");
    buildCodeTable(node->right, prefix + "1");
}

//functie de creare arbor huffman
void CompressedFile::buildHuffmanTree()
{
    //comparator frecvente pentru sortarea in priority queue  
    //=> min heap
    struct Compare{
        bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const{
            return a->freq > b->freq;
        }
    };

    //stocarea frecventei caracterelor
    std::unordered_map<unsigned char, size_t> freq;
    for(size_t i = 0; i < fileSize; ++i)
    {
        unsigned char c = data[i];
        freq[c]++;
    }

    //definire priority queue
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, Compare>pq;

    //adaugare noduri in arbore
    for(auto& pair: freq)
    {
        auto node = std::make_shared<Node>();
        node->ch = pair.first;
        node->freq = pair.second;
        pq.push(node);
    }

    //la arbore gol dam return direct
    if(pq.empty())
    {
        root = nullptr;
        return;
    }

    //la arbore cu un singur nod dummy pentru a nu strica logica
    if(pq.size() == 1)
    {
        auto dummy = std::make_shared<Node>();
        dummy->ch = 0;
        dummy->freq = 0;
        pq.push(dummy);
    }

    //constructie arbore 
    while(pq.size() > 1)
    {   
        auto left = pq.top();
        pq.pop();
        auto right = pq.top();
        pq.pop();
        auto parent = std::make_shared<Node>();
        parent->ch = 0;
        parent->freq = left->freq + right->freq;
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }

    root = pq.top();
    codeTable.clear();
    buildCodeTable(root, "");
}

//transformare continut in sir de biti
std::string CompressedFile::encodeToBits() const{
    std::string bitStream;
    for(size_t i = 0; i < fileSize; ++i)
    {
        unsigned char c = data[i];
        bitStream += codeTable.at(c);
    }
    return bitStream;
}


//decodarea sirului de biti
std::string CompressedFile::decodeFromBits(const std::vector<unsigned char>& compData) const{
    if(!root)
        return "";

    std::string bits;
    for(auto b : compData)
    {
        std::bitset<8> bits8(b);
        for(int i = 7; i >= 0; --i)
        {
            bits.push_back(bits8[i] ? '1' : '0');
        }
    }

    std::string result;
    auto node = root;
    size_t charactersRecovered = 0;

    //refacerea trasului in arbore
    for(char bit : bits)
    {
        if(bit == '0')
            node = node->left;
        else
            node = node->right;
        
        if(!node)
            break;
        
        if(node->isLeaf())
        {
            result.push_back(char(node->ch));
            node = root;
            charactersRecovered++;
            if(charactersRecovered == originalSize)
                return result; 
        }
    }
    return result;
}

//constructor
CompressedFile::CompressedFile(const std::string& name, const std::string& content)
:File(name, content)
{
    compressed = false;
    compress();
}

CompressedFile::CompressedFile(const std::string& name, const std::string& content, bool shouldCompress)
:File(name, content)
{
    compressed = false;
    if(shouldCompress)
    {
        compress();
    }
}

//functia de compresare
void CompressedFile::compress()
{
    if(compressed || fileSize == 0)
        return;

    originalSize = fileSize;
    buildHuffmanTree();
    std::string bitStream = encodeToBits();

    std::vector<unsigned char> encoded;
    unsigned char current = 0;
    int bitCount = 0;

    for(char c : bitStream)
    {
        current = current << 1;
        if(c == '1') 
            current = current | 1;

        bitCount++;

        if(bitCount == 8)
        {
            encoded.push_back(current);
            current = 0;
            bitCount = 0;
        }
    }

    if(bitCount > 0)
    {
        current = current << (8 - bitCount);
        encoded.push_back(current);
    }
    
    fileSize = encoded.size();
    data = new char[fileSize + 1];
    for(size_t i = 0; i < fileSize; ++i)
    {
        data[i] = encoded[i];
    }
    data[fileSize] = '\0';
    compressedData = std::move(encoded);
    compressed = true;
}

//functia de decompresare
void CompressedFile::decompress()
{
    if(!compressed)
        return;
    
    std::string decoded = decodeFromBits(compressedData);

    fileSize = decoded.size();
    data = new char[fileSize + 1];
    for(size_t i = 0; i < fileSize; ++i)
    {
        data[i] = decoded[i];
    }
    data[fileSize] = '\0';

    compressed = false;
}

//afisare date fisier compresat
void CompressedFile::display(int depth) const{
    for(int i = 0; i < depth; ++i)
        std::cout << " ";
    std::string time = ctime(&timestamp);
    time.pop_back();
    
    std::cout<< time << " -> Compressed File " << name << " has " << fileSize << " bytes\n";
}

//getter de dimensiune 
size_t CompressedFile::getSize() const{
    return compressed ? compressedData.size() : fileSize;
}

//afisare continut
std::string CompressedFile::readContent() const{
    return File::readContent();
}

//afisare continut decompresat
std::string CompressedFile ::readDecompressedContent(){
    if(compressed == false)
        return File::readContent();
    if(fileSize == 0)
        return "";
    this->decompress();
    std::string content = File::readContent();
    this->compress();
    return content;
}
