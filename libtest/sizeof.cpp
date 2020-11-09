#include <xbase/xbase.h>

int main()
{
    std::cout << "char       = " << sizeof(char)     << "\n";
    std::cout << "short      = " << sizeof(short)    << "\n";
    std::cout << "int        = " << sizeof(int)      << "\n";
    std::cout << "long       = " << sizeof(long)     << "\n";
    std::cout << "xbShort    = " << sizeof(xbShort)  << "\n";
    std::cout << "xbUShort   = " << sizeof(xbUShort) << "\n";
    std::cout << "xbLong     = " << sizeof(xbLong)      << "\n";
    std::cout << "xbULong    = " << sizeof(xbULong)     << "\n";
    std::cout << "xbDouble   = " << sizeof(xbDouble)    << "\n";
    std::cout << "xbSchemaRec= " << sizeof(xbSchemaRec) << "\n";    

    xbXBase x;
    std::cout << "EndiandType= "<< char(x.GetEndianType()) << "\n";
}
