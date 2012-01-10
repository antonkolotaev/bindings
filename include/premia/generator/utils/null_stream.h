#ifndef _premia_generator_utils_null_stream_h_included_
#define _premia_generator_utils_null_stream_h_included_

#include <iostream>

namespace premia {
namespace pygen  {

    struct null_stream : std::basic_ostream<char>
    {
        struct null_buf : std::basic_streambuf<char>
        {
            unsigned xsputn(const char *, unsigned c){
                return c;
            } 
        } buf;
        
        null_stream()
        {
            rdbuf(&buf);
        }
    };    
    
}}

#endif
