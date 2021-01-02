#include <vector>
#include <fstream>

class Index                         //Index class for indexes in the word board
{
    public:
        int row, col, dir;
        Index();
        Index(int, int, int);
        void dir_update();
};

class DictRange                     //Dictionary range class for minimum and maximum dictionary range
{
    public:
        int min, max;
        DictRange();
        DictRange(int, int);
};

class PrefixMatcher                 //Prefix Matcher class for loading american-english file from /usr/share/dict and matching prefixes
{
    private:
        std::fstream dict_ptr;
        int bin_search(int, int, int, char*, int, DictRange*);
    public:
        std::vector<std::string> dict_words;
        int init();
        DictRange* match(DictRange*, int, char*);
        PrefixMatcher();
        ~PrefixMatcher();
};