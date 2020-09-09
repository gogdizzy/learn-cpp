//
// Created by 关鑫 on 2020/7/30.
//

#include <iostream>
#include <string>
#include <string_view>
int main()
{
    std::wstring_view wcstr_v = L"xyzzy";

    char array[3] = {'B', 'a', 'r'};
    std::string_view array_v(array, std::size(array));

    std::string cppstr = "Foo";
    std::string_view cppstr_v(cppstr);

    std::cout << cppstr_v << '\n'
              << array_v << '\n'
              << wcstr_v.size() << '\n';

    return 0;
}
