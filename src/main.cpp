#include <iostream>
#include <string>

int main() {
    std::string_view test = "1 =1";
    std::cout << test << '\n' << test.size() << '\n';
}
