#include"libww/ww_company.hpp"




namespace ww{




void
company::
drive(gbstd::task_control&  ctrl, const company*  c) noexcept
{
}


void
company::
step_animation(gbstd::task_control&  ctrl, company*  c) noexcept
{
    if(c->m_render_counter)
    {
      c->m_render_counter = 0;

      c->m_blink_context.step();
    }


    if( c->m_status.test(flags::motion_valid ) &&
       !c->m_status.test(flags::motion_paused))
    {
        if(c->m_current_event.is_stop())
        {
            if(ctrl.get_time() < c->m_event_next_time)
            {
              return;
            }
        }

      else
        if(c->m_current_event.is_move())
        {
          auto&  mov = c->m_current_event.get_data().m_move;

            while(c->m_event_counter)
            {
                if(ctrl.get_time() < c->m_event_next_time)
                {
                  return;
                }


              c->m_event_next_time += mov.get_delay();

              --c->m_event_counter;

              c->m_current_pos.x += mov.get_x_increment();
              c->m_current_pos.y += mov.get_y_increment();
            }
        }


        if(c->m_event_index < c->m_event_queue.size())
        {
          auto&  evt = c->m_event_queue[c->m_event_index++];

            if(evt.is_stop())
            {
              c->m_current_event = evt;

              c->m_event_next_time = ctrl.get_time()+evt.get_data().m_stop.get_delay();
            }

          else
            if(evt.is_move())
            {
              c->m_current_event = evt;

              auto&  mov = evt.get_data().m_move;

              c->m_event_next_time = ctrl.get_time()+mov.get_delay();
              c->m_event_counter   =                 mov.get_count();
            }

          else
            if(evt.is_set_blink())
            {
              c->get_blink_context().enable();
            }

          else
            if(evt.is_unset_blink())
            {
              c->get_blink_context().disable();
            }

          else
            if(evt.is_set_white())
            {
              c->set_white_flag();
            }

          else
            if(evt.is_unset_white())
            {
              c->unset_white_flag();
            }
        }

      else
        {
          c->clear_event_queue();
        }
    }
}


}




