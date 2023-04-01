#include <iostream>
#include <proisam.hpp>

int main()
{
    std::cout << "pisamreader" << std::endl;

    try {
        auto genfile = proisam::ProisamFile::open("C:\\development\\experiment\\genfile.pro", 0);
    }
    catch (const proisam::ProisamException& e) {
        std::cout << e.what() << std::endl;
    }
    
    std::cout << "pisamreader end" << std::endl;
    return 0;
}