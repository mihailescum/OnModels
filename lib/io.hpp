#ifndef ONMODELS_IO_H
#define ONMODELS_IO_H

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <filesystem>

namespace onmodels
{
    template <class T>
    std::string table2str(const std::vector<std::vector<T>> &v)
    {
        std::stringstream ss;
        for (auto row : v)
        {
            copy(row.begin(), row.end(), std::ostream_iterator<T>(ss, " "));
            ss << std::endl;
        }
        return ss.str();
    }

    bool write_txt(const std::string content, const std::string directory, const std::string filename)
    {
        if (!std::filesystem::is_directory(directory))
        {
            if (!std::filesystem::create_directory(directory))
                return false;
        }

        std::ofstream output_file(directory + "/" + filename + ".txt");
        output_file << content;
        output_file.close();

        return true;
    }

    std::string prettyPrintCrosssection(const std::vector<double> &crosssection, unsigned L)
    {
        std::stringstream ss;
        for (unsigned i = 0; i < L; i++)
        {
            for (unsigned j = 0; j < L; j++)
            {
                ss << std::fixed << std::setprecision(2) << crosssection[i * L + j] << " ";
            }
            ss << std::endl;
        }
        return ss.str();
    }
}
#endif