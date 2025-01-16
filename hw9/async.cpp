#include "async.h"
#include "ContextManager.hpp"

namespace hw9 {

static ContextManager context_manager{2};

ContextID connect(size_t batch_capacity) {
    return context_manager.create_new_context(batch_capacity);
}

void receive(ContextID ctx_id, const std::string& command) {
    context_manager.handle_command(ctx_id, command);
}

void disconnect(ContextID ctx_id) {
    context_manager.remove_context(ctx_id);
}

} // namespace hw9
