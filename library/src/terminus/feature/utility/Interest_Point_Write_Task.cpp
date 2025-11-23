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
 * @file    Interest_Point_Write_Task.cpp
 * @author  Marvin Smith
 * @date    8/31/2023
*/
#include "Interest_Point_Write_Task.hpp"

namespace tmns::feature::utility {

/************************************/
/*          Constructor             */
/************************************/
Interest_Point_Write_Task::Interest_Point_Write_Task( Interest_Point_List  local_points,
                                                      Interest_Point_List& global_points )
    : m_points(local_points),
      m_global_points(global_points)
{
}

/********************************/
/*      Function Operator       */
/********************************/
void Interest_Point_Write_Task::operator()()
{
    m_global_points.insert( m_global_points.end(),
                            m_points.begin(),
                            m_points.end() );
}

} // End of tmns::feature::utility namespace