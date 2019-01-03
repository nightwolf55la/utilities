#include <iostream>
#include <vector>

#include <util/generator.h>

int main() {
    std::cout << "Beginning examples: util::gen" << std::endl << std::endl;

    {
        std::cout << "enumerate" << std::endl;
        std::vector<int> vec{1,2,3};

        std::cout << "Should print (0,1)(1,2)(2,3):" << std::endl;
        for(auto&& [index, value] : util::gen::enumerate{vec}) {
            std::cout << "(" << index << "," << value << ")";
        }
        std::cout << std::endl;

        std::cout << "Should print (10,1)(11,2)(12,3):" << std::endl;
        for(auto&& [index, value] : util::gen::enumerate{vec, 10}) {
            std::cout << "(" << index << "," << value << ")";
        }
        std::cout << std::endl;

        auto get_vec = [](){
            return std::vector<int>{1,2,3};
        };
        std::cout << "Should print (0,1)(1,2)(2,3):" << std::endl;
        for(auto&& [index, value] : util::gen::enumerate{get_vec()}) {
            std::cout << "(" << index << "," << value << ")";
        }
        std::cout << std::endl;
    }
    return 0;
}