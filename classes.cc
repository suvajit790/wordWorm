#include <iostream>
#include <fstream>
#include <string>
#include "classes.hh"

Index::Index()
{
    row = col = dir = 0;
}

Index::Index(int i, int j, int d)
{
    row = i;
    col = j;
    dir = d;
}

void Index::dir_update()
{
    dir++;
}

DictRange::DictRange()
{
    min = 0;
    max = 0;
}

DictRange::DictRange(int min_range, int max_range)
{
    min = min_range;
    max = max_range;
}

PrefixMatcher::PrefixMatcher()
{
}

int PrefixMatcher::init()
{
    dict_ptr.open("/usr/share/dict/american-english", std::ios::in);

    if (!dict_ptr.is_open())
    {
        std::cout << "can't find word dictionary in your computer" << std::endl;
        return 1;
    }

    std::string temp;
    while (std::getline(dict_ptr, temp))
    {
        std::string t_str = temp;
        int accept = 1;
        for (int i = 0; i < t_str.size(); i++)
        {
            if (t_str.at(i) > 'z' || t_str.at(i) < 'a')
                accept = 0;
        }

        if (accept)
            dict_words.push_back(t_str);
    }

    return 0;
}

int PrefixMatcher::bin_search(int min, int max, int index, char *letter, int dir, DictRange *new_range)
{
    if (max > min)
    {
        int mid = min + (max - min) / 2;

        if (dict_words[mid].size() <= index)
            return 1;

        if (dict_words[mid].at(index) == *letter)
        {
            if (dir == 0)
            {
                int val1 = bin_search(min, mid - 1, index, letter, -1, new_range);
                int val2 = bin_search(mid + 1, max, index, letter, 1, new_range);
                if (val1 == 0 && val2 == 0)
                    return 0;
                else if (val1 == 1 && val2 == 1)
                {
                    new_range->min = mid;
                    new_range->max = mid;
                    return 0;
                }

                if (val1 == 1)
                {
                    new_range->min = mid;
                    return 0;
                }

                if (val2 == 1)
                {
                    new_range->max = mid;
                    return 0;
                }
            }
            else if (dir == 1)
            {
                if (bin_search(mid + 1, max, index, letter, 1, new_range))
                {
                    new_range->max = mid;
                    return 0;
                }
                else
                    return 0;
            }
            else
            {
                if (bin_search(min, mid - 1, index, letter, -1, new_range))
                {
                    new_range->min = mid;
                    return 0;
                }
                else
                    return 0;
            }
        }

        if (dict_words[mid].at(index) > *letter)
            return bin_search(min, mid - 1, index, letter, dir, new_range);

        if (dict_words[mid].at(index) < *letter)
            return bin_search(mid + 1, max, index, letter, dir, new_range);
    }
    else if (min == max)
    {
        if (dict_words[min].size() <= index)
            return 1;

        if (dict_words[min].at(index) == *letter)
        {
            if (dir == 1)
                new_range->max = max;
            else if (dir == -1)
                new_range->min = min;

            return 0;
        }

        return 1;
    }

    return 1;
}

DictRange *PrefixMatcher::match(DictRange *range, int index, char *letter)
{
    DictRange *new_range = new DictRange();

    if (!bin_search(range->min, range->max, index, letter, 0, new_range))
    {
        return new_range;
    }

    return NULL;
}

PrefixMatcher::~PrefixMatcher()
{
    dict_ptr.close();
}