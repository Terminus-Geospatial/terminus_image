/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2025 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    GDAL_Codes.cpp
 * @author  Marvin Smith
 * @date    7/16/2023
*/
#include "GDAL_Codes.hpp"

// GDAL Libraries
#include <gdal.h>

// C++ Libraries
#include <iostream>
#include <sstream>

namespace tmns::image::io::gdal {

/********************************************/
/*      Print to log-friendly string        */
/********************************************/
std::string ToLogString( std::vector<int> codes )
{
    std::stringstream sout;
    sout << "(";
    size_t counter = 0;
    for( const auto& code : codes )
    {
        if( counter++ > 0 )
        {
            sout << ", ";
        }
        sout << GDALGetColorInterpretationName( (GDALColorInterp)code );
    }
    sout << " )";
    return sout.str();
}

} // End of tmns::image::io::gdal namespace