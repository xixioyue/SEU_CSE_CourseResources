// #include<iostream>
// int main()
// {
//     int n=0;
//     char c[101]={'a'};
//     std::cin>>n;
//     for(int i=0;i<n;i++)
//     {   
//         int count=0;
//         std::cin>>c;
//         for(int j=0;c[j]!='\0';j++)
//         {
//             if(c[j]>='0'&&c[j]<='9')
//                 count++;
//         }
//         std::cout<<count<<'\n';
//     }
//     return 0;
// }

#include <iostream>
#include <string>

int main() {
    int n;
    std::cin >> n;
    std::cin.ignore();

    for (int i = 0; i < n; i++) {
        std::string line;
        std::getline(std::cin, line);
        
        int count = 0;
        for (char ch : line) {
            if (ch >= '0' && ch <= '9') {
                count++;
            }
        }
        std::cout << count << '\n';
    }
    return 0;
}