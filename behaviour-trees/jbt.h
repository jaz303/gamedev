/*
 * jbt - Jason's Behaviour Tree Library
 *
 * (c) 2015 Jason Frame
 * Released under the MIT License
 */

#include <stdlib.h>

#ifndef JBT_DEBUG
#define JBT_DEBUG 0
#endif

#ifndef JBT_CONTEXT_TYPE
#define JBT_CONTEXT_TYPE void*
#endif

#ifndef JBT_ENTITY_TYPE
#define JBT_ENTITY_TYPE void*
#endif

#ifndef JBT_CONTEXT_TYPE
#define JBT_CONTEXT_TYPE void*
#endif

enum jbt_status_t {
    JBT_SUCCESS,
    JBT_FAILURE,
    JBT_RUNNING
};

struct jbt_brain_t {
    
    // this is the current state of the root node; if this is not JBT_RUNNING
    // it means that the brain has finished.
    jbt_status_t state;

    // brain storage space
    char *stack;

    // number of bytes available for brain storage
    size_t stack_size;

    // current stack base pointer; points to jbt_state_t of active frame
    char *base_ptr;

    // the entity this brain controls
    JBT_ENTITY_TYPE entity;

    // the global context for this brain (i.e. world, map, game state)
    JBT_CONTEXT_TYPE context;

};

typedef void (jbt_node_init_f)(jbt_brain_t *brain, JBT_CONTEXT_TYPE ctx, void *node);
typedef jbt_status_t (jbt_node_tick_f)(jbt_brain_t *brain, void *entity, void *state);

struct jbt_node_descriptor_t {
    size_t              stack_size;
    jbt_node_init_f     *init;
    jbt_node_tick_f     *tick;
#ifdef JBT_DEBUG
    const char          *name;
#endif
};

struct jbt_state_t {
    jbt_node_descriptor_t *node;
    jbt_state_t *parent;
    
    // jbt_
};

void jbt_init_brain(jbt_brain_t *brain, void *stack, size_t stack_size) {
    brain->stack = (char*)stack;
    brain->stack_size = stack_size;
    brain->base_ptr = brain->stack;
}

void jbt_tick_brain(jbt_brain_t *brain) {

    return;
    
    if (brain->state != JBT_RUNNING) {
        return;
    }
    
    jbt_state_t *node_state = (jbt_state_t*)brain->base_ptr;
    
    
    jbt_status_t result = node_state->node->tick(
        brain,
        brain->entity,
        node_state
    );

    if (result != JBT_RUNNING) {
        // TODO(jwf): pop this one off the stack
    }

}

#define JBT_BEGIN_LEAF(name)

#define JBT_END_LEAF()

#define JBT_STATE()
#define JBT_INIT()
#define JBT_TICK()

#define JBT_LEAF(name, entity_type) \
    jbt_status_t name(jbt_brain_t *brain, entity_type entity, jbt_state_t *state)

#define JBT_STATEFUL_LEAF(name, entity_type, state_type) \
    jbt_status_t name(jbt_brain_t *brain, entity_type entity, state_type state)