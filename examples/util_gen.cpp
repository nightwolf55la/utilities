#include <iostream>
#include <vector>
#include <list>

#include <util/generator.h>

int main() {
    std::cout << "Beginning examples: util::gen" << std::endl << std::endl;

    ////////////////////////////////////////////////////////////////////////////////
    // range examples
    ////////////////////////////////////////////////////////////////////////////////
    {
        std::cout << "range" << std::endl;

        // Just using an end index
        std::cout << "Should print (0)(1)(2)(3)(4)" << std::endl << "             ";
        for(int64_t value : util::gen::range(5)) {
            std::cout << "(" << value << ")";
        }
        std::cout << std::endl;

        // Using a beginning and end index
        std::cout << "Should print (3)(4)(5)(6)" << std::endl << "             ";
        for(int64_t value : util::gen::range(3,7)) {
            std::cout << "(" << value << ")";
        }
        std::cout << std::endl;

        // Using a beginning and end index with a non default step
        std::cout << "Should print (2)(5)(8)" << std::endl << "             ";
        for(int64_t value : util::gen::range(2,11,3)) {
            std::cout << "(" << value << ")";
        }
        std::cout << std::endl;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // enumerate examples
    ////////////////////////////////////////////////////////////////////////////////
    std::cout << std::endl;
    {
        std::cout << "enumerate" << std::endl;
        std::vector<int> vec{1,2,3};

        // Enumerate an lvalue reference starting with 0
        std::cout << "Should print (0,1)(1,2)(2,3)" << std::endl << "             ";
        for(auto&& [index, value] : util::gen::enumerate{vec}) {
            std::cout << "(" << index << "," << value << ")";
        }
        std::cout << std::endl;

        // Enumerate a const lvalue reference starting with 0
        std::vector<int> const_vec{1,2,3};
        std::cout << "Should print (0,1)(1,2)(2,3)" << std::endl << "             ";
        for(auto&& [index, value] : util::gen::enumerate{const_vec}) {
            std::cout << "(" << index << "," << value << ")";
            // value = 0; // This would not compile
        }
        std::cout << std::endl;

        // Enumerate an lvalue reference starting with 10
        std::cout << "Should print (10,1)(11,2)(12,3)" << std::endl << "             ";
        for(auto&& [index, value] : util::gen::enumerate{vec, 10}) {
            std::cout << "(" << index << "," << value << ")";
        }
        std::cout << std::endl;

        // Enumerate an rvalue reference - this rvalue's lifetime is extended (no copies or moves performed)
        auto get_vec = [](){
            return std::vector<int>{1,2,3};
        };
        std::cout << "Should print (0,1)(1,2)(2,3)" << std::endl << "             ";
        for(auto&& [index, value] : util::gen::enumerate{get_vec()}) {
            std::cout << "(" << index << "," << value << ")";
        }
        std::cout << std::endl;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // zip examples
    ////////////////////////////////////////////////////////////////////////////////
    std::cout << std::endl;
    {
        std::cout << "zip" << std::endl;
        std::vector<int> vec1{1,2,3};
        std::vector<int> vec2{4,5,6};
        char arr[] = {'a', 'b', 'c'};
        const std::list<double> list1{1.2, 3.4, 5.6};

        // Zip on just one iterable
        std::cout << "Should print (1)(2)(3)" << std::endl << "             ";
        for(auto&& [vec1_val] : util::gen::zip{vec1}) {
            std::cout << "(" << vec1_val << ")";
        }
        std::cout << std::endl;

        // Zip on two iterables
        std::cout << "Should print (1,4)(2,5)(3,6)" << std::endl << "             ";
        for(auto&& [vec1_val, vec2_val] : util::gen::zip{vec1, vec2}) {
            std::cout << "(" << vec1_val << "," << vec2_val << ")";
        }
        std::cout << std::endl;

        // Zip on three iterables
        std::cout << "Should print (1,4,a)(2,5,b)(3,6,c)" << std::endl << "             ";
        for(auto&& [vec1_val, vec2_val, arr_val] : util::gen::zip{vec1, vec2, arr}) {
            std::cout << "(" << vec1_val << "," << vec2_val << "," << arr_val << ")";
        }
        std::cout << std::endl;

        // Zip on three iterables with a const
        std::cout << "Should print (1,4,1.2)(2,5,2.3)(3,6,4.5)" << std::endl << "             ";
        for(auto&& [vec1_val, vec2_val, list1_val] : util::gen::zip{vec1, vec2, list1}) {
            std::cout << "(" << vec1_val << "," << vec2_val << "," << list1_val << ")";
            // list1_val = 0; // This would not compile
        }
        std::cout << std::endl;

        // Zip on three iterables with one being an rvalue - Rvalue's lifetime is extended (no copies or moves performed)
        auto get_vec = [](){
            return std::vector<int>{7,8,9};
        };
        std::cout << "Should print (1,7,a)(2,8,b)(3,9,c)" << std::endl << "             ";
        for(auto&& [vec1_val, vec2_val, arr_val] : util::gen::zip{vec1, get_vec(), arr}) {
            std::cout << "(" << vec1_val << "," << vec2_val << "," << arr_val << ")";
        }
        std::cout << std::endl;

        // Zip on three iterables with one being shorter (length is limited to shortest iterable)
        auto get_short_vec = [](){
            return std::vector<int>{7,8};
        };
        std::cout << "Should print (1,7,a)(2,8,b)" << std::endl << "             ";
        for(auto&& [vec1_val, vec2_val, arr_val] : util::gen::zip{vec1, get_short_vec(), arr}) {
            std::cout << "(" << vec1_val << "," << vec2_val << "," << arr_val << ")";
        }
        std::cout << std::endl;
    }

    return 0;
}