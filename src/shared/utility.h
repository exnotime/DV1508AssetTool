#include <string>
static std::string GetDir ( std::string filename )
{
    bool found = false;
    for ( int i = static_cast<int> ( filename.size() ); i > 0; i-- )
    {
        if ( filename.c_str() [i] == '/' )
        {
            found = true;
        }
        if ( !found )
        {
            filename.erase ( i );
        }
    }
    return filename;
}