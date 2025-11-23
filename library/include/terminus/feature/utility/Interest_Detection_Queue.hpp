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
 * @file    Interest_Detection_Queue.hpp
 * @author  Marvin Smith
 * @date    8/28/2023
*/
#pragma once

/// Feature Detector
#include "../Detector_Base.hpp"
#include "Interest_Point_Detection_Task.hpp"

namespace tmns::feature::utility {

/**
 * Specialized thread pool for interest point detection. 
 * 
 * Breaks the image into tiles, allowing for more control over memory 
 * usage as well as the potential for multi-threading.
 */
template <typename ImageT>
class Interest_Detection_Queue : public core::work::Thread_Pool
{
    public:

        typedef Interest_Point_Detection_Task<ImageT> task_type;

        /**
         * Constructor
         * 
         * @param image
         * @param detector
        */
        Interest_Detection_Queue( const image::Image_Base<ImageT>&       image,
                                  Detector_Base::ptr_t                   detector,
                                  core::work::Work_Queue_Ordered::ptr_t  write_queue,
                                  Interest_Point_List&                   ip_list,
                                  const math::Size2i&                    tile_size,
                                  int                                    desired_num_ip = 0 )
             : m_image( image.impl() ),
               m_detector( detector ),
               m_write_queue( std::move( write_queue ) ),
               m_ip_list( ip_list ),
               m_tile_size( tile_size ),
               m_desired_num_ip( desired_num_ip )
        {
            m_bboxes = m_image.full_bbox().subdivide( tile_size, true );
            this->notify();
        }

        /**
         * Get the number of bounding boxes to process
         */
        size_t size() const
        { 
            return m_bboxes.size();
        }

        /**
         * Get the next task to process
        */
        virtual core::work::Task::ptr_t get_next_task()
        {
            core::conc::Mutex::Lock lock(m_mutex);
            if( m_index == m_bboxes.size() )
            {
                return core::work::Task::ptr_t();
            }

            m_index++;
                
            // The default value means let the detector pick the IP count.
            int num_ip = 0; 

            if( m_desired_num_ip > 0 )
            {
                // Determine the desired number of IP for this tile based on its size
                //  relative to a full sized tile.
                const int MIN_NUM_IP = 1;
                double expected_area = m_tile_size.width() * m_tile_size.height();
                double area          = m_bboxes[m_index-1].area();
                double fraction      = area / expected_area;
                num_ip        = ceil(fraction * static_cast<double>(m_desired_num_ip));
                if (num_ip < MIN_NUM_IP)
                {
                    num_ip = MIN_NUM_IP;
                }
                if( num_ip > m_desired_num_ip )
                {
                    num_ip = m_desired_num_ip;
                }
            }

            return std::make_shared<task_type>( m_image,
                                                m_detector,
                                                m_bboxes[m_index-1],
                                                num_ip,
                                                m_index-1,
                                                m_bboxes.size(),
                                                m_ip_list,
                                                (*m_write_queue) );
        }

    private:

        /// @brief Baseline image to detect points for
        ImageT  m_image;

        /// @brief Feature-Detector
        Detector_Base::ptr_t m_detector;

        /// @brief Feature Point Write Queue
        core::work::Work_Queue_Ordered::ptr_t  m_write_queue;
    
        /// @brief Interest Point List
        Interest_Point_List& m_ip_list;
    
        /// @brief Bounding Boxes 
        std::vector<math::Rect2i> m_bboxes;

        /// @brief Tile size
        math::Size2i m_tile_size;
    
        /// @brief Desired number of interest points
        int m_desired_num_ip;

        /// @brief Internal mutex lock
        core::conc::Mutex m_mutex;
    
        /// Tile Index
        size_t m_index { 0 };

}; // End of Interest_Detection_Queue class

} // End of tmns::feature::utility namespace