#ifndef FileStringH
#define FileStringH

#include <string>
#include <fstream>
#include <iterator>

class FileString : public std::string
{
    void reserve(std::ifstream &inputFileStream)
    {
        inputFileStream.seekg(0, std::ios::end);
        std::string::reserve(size_t(inputFileStream.tellg()) + 1);
        inputFileStream.seekg(0, std::ios::beg);
    }

    void assign(std::ifstream &inputFileStream)
    {
        std::string::assign((std::istreambuf_iterator<char>(inputFileStream)),
            std::istreambuf_iterator<char>());
    }


    public:

        bool load(const char *fileName)
        {
            std::ifstream inputFileStream(fileName);
            if (!inputFileStream.is_open())
                return false;

            reserve(inputFileStream);
            assign(inputFileStream);
            return true;
        }
};

#endif
