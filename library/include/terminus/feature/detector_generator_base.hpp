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
 * @file    detector_Generator_Base.hpp
 * @author  Marvin Smith
 * @date    8/27/2023
*/
#pragma once

// C++ Libraries
#include <memory>

// Terminus Feature Libraries
#include "detector_base.hpp"

// Terminus Libraries
#include <terminus/error.hpp>

namespace tmns::feature {

class detector_Generator_Base
{
    public:

        typedef std::shared_ptr<detector_Generator_Base> ptr_t;

        virtual Result<detector_Base::ptr_t> generate( detector_Config_Base::ptr_t config ) = 0;

}; // End of detector_Generator_Base class

} // End of tmns::feature namespace