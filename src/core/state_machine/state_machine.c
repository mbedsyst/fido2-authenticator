#include "state_machine.h"
#include "event_queue.h"
#include "app_events.h"
#include "state_handlers.h"

static app_state_t current_state = STATE_IDLE;

void state_machine_init(void)
{
    current_state = STATE_IDLE;
}

app_state_t state_machine_get_state(void)
{
    return current_state;
}

static void transition_to(app_state_t new_state)
{
    current_state = new_state;
}

void state_machine_handle_event(app_event_t event)
{
    switch(current_state)
    {
        case STATE_IDLE:
            handle_idle(event);
            break;
    
        case STATE_RECEIVING:

            break;   
        case STATE_RECONSTRUCTING:

            break;   
        case STATE_PROCESSING:

            break;   
        case STATE_DECONSTRUCTING:

            break;   
        case STATE_RESPONDING:

            break;           
        case STATE_ERROR:

            break;        
        default:
            
            break;   
    }
}