#include "state_machine.h"
#include "event_queue.h"
#include "app_events.h"
#include "state_handlers.h"

static app_state_t current_state = STATE_IDLE;
static app_event_t new_event;

void state_machine_init(void)
{
    current_state = STATE_IDLE;
}

app_state_t state_machine_get_state(void)
{
    return current_state;
}

void state_machine_handle_event(app_ctx_t *ctx)
{
    current_state = state_machine_get_state();

    if (!event_queue_is_empty()) 
    {
        if(event_queue_pop(new_event))
        {
            switch(current_state)
            {
                case STATE_IDLE:
                    handle_idle(ctx, new_event);
                    break;
            
                case STATE_RECEIVING_STARTED:
                    handle_receiving(ctx, new_event);
                    break;
        
                case STATE_RECONSTRUCTING:
                    handle_reconstructing(ctx, new_event);
                    break;  
        
                case STATE_PROCESSING:
                    handle_processing(ctx, new_event);
                    break;
        
                case STATE_DECONSTRUCTING:
                    handle_deconstructing(ctx, new_event);
                    break;
        
                case STATE_RESPONDING_STARTED:
                    handle_responding(ctx, new_event);
                    break;
        
                case STATE_ERROR:
                    handle_error(ctx, new_event);
                    break;
        
                default:
                    
                    break;   
            }   
        }
    }
}