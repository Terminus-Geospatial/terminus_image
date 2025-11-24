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
 * @file    interest_point_Write_Task.hpp
 * @author  Marvin Smith
 * @date    8/31/2023
 */
#pragma once

// Terminus Feature Libraries
#include "../interest_point.hpp"

// Terminus Libraries
#include <terminus/core/work/Task.hpp>

// Boost Libraries
#include <boost/noncopyable.hpp>

namespace tmns::feature::utility {

/**
 * This task class is used to insure that interest points are written to their list
 * in a repeatable order that is not effected by the order in which the detection 
 * threads start and finish.
 */
class interest_point_Write_Task : public  core::work::Task,
                                  private boost::noncopyable
{
    public:
    
        /**
         * Constructor
        */
        interest_point_Write_Task( interest_point_List  local_points,
                                   interest_point_List& global_points );

        virtual ~interest_point_Write_Task() = default;

        virtual void operator()();

    private:

        interest_point_List  m_points;

        interest_point_List& m_global_points;
}; // End of interest_point_Write_Task class

} // End of tmns::feature::utility namespace