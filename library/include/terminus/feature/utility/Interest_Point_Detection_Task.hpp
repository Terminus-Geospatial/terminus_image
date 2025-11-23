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
 * @file    Interest_Point_Detection_Task.hpp
 * @author  Marvin Smith
 * @date    8/28/2023
*/
#pragma once

// Terminus Feature Libraries
#include "Interest_Point_Write_Task.hpp"

// Terminus Libraries
#include <terminus/image/operations/crop_image.hpp>

namespace tmns::feature::utility {

/**
 * IP task wrapper for use with the Interest_Detection_Queue
 * thread pool class.
 *  - After IPs are found, they are passed to an Interest_Point_Write_Task object.
 */
template <typename ImageT>
class Interest_Point_Detection_Task : public core::work::Task,
                                      private boost::noncopyable 
{
    public:

        /**
         * Constructor
         */
        Interest_Point_Detection_Task( const image::Image_Base<ImageT>&  image,
                                       Detector_Base::ptr_t              detector,
                                       const math::Rect2i&               bbox, 
                                       int                               desired_num_ip,
                                       int                               id,
                                       int                               max_id,
                                       Interest_Point_List&              global_list,
                                       core::work::Work_Queue_Ordered&   write_queue )
            : m_image( image.impl() ),
              m_detector( detector ),
              m_bbox( bbox ),
              m_desired_num_ip( desired_num_ip ),
              m_id( id ),
              m_max_id( max_id ),
              m_global_points( global_list ),
              m_write_queue(write_queue)
        {
        }

        ~Interest_Point_Detection_Task() override = default;

        Interest_Point_List interest_point_list()
        {
            return m_global_points;
        }

        /**
         * Function Operator
        */
        void operator()() override
        {
            {
                std::stringstream sout;
                sout << "Locating interest points in block " << m_id + 1 << "/" 
                     << m_max_id << "   [ " << m_bbox.to_string() << " ] with " << m_desired_num_ip 
                     << " ip.";
                tmns::log::debug( sout.str() );
            }

            // Use the m_detector object to find a set of image points in the cropped section of the image.
            auto detection_results = m_detector->operator()( image::crop_image( m_image.impl(),
                                                                                m_bbox ),
                                                              true,
                                                              m_desired_num_ip );

            auto& new_ip_list = detection_results.value();

            for( auto pt = new_ip_list.begin(); 
                 pt != new_ip_list.end(); 
                 ++pt )
            {
                pt->pixel_loc()  += m_bbox.min();
                pt->raster_loc() += m_bbox.min();
            }

            // Append these interest points to the master list
            // owned by the detect_interest_points() function.
            auto write_task = std::make_shared<Interest_Point_Write_Task>( new_ip_list, m_global_points );
            m_write_queue.add_task( write_task, m_id );

            {
                std::stringstream sout;
                sout << "Finished block " << m_id + 1 << "/" << m_max_id;
                tmns::log::debug( sout.str() );
            }
        }

    private:

        /// @brief  Source Image
        ImageT m_image;
    
        /// @brief Feature Detector Instance
        Detector_Base::ptr_t m_detector;
    
        /// @brief Region of image to render
        math::Rect2i  m_bbox;
    
        /// @brief Desired number of interest points
        int m_desired_num_ip; 

        /// Task ID
        int m_id;

        /// Max Task ID
        int m_max_id;
        
        /// @brief  List of global points
        Interest_Point_List& m_global_points;

        /// @brief  Work Queue for Writing
        core::work::Work_Queue_Ordered& m_write_queue;
    
};


} // End of tmns::feature::utility namespace