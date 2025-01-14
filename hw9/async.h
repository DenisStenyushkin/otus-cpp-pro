#pragma once

#include <string>

namespace hw9 {

using ContextID = size_t; // unsigned long;

/// @brief Connects a commands source to processing context
/// @param batch_capacity Size of a commands batch
/// @return ID of the processing context
ContextID connect(size_t batch_capacity);

/// @brief Passes a new command to the processing context
/// @param ctx_id The context's ID
/// @param command Command to pass
void receive(ContextID ctx_id, const std::string&& command);

/// @brief Disconnects from the processing context
/// @param ctx_id The context's ID
void disconnect(ContextID ctx_id);

} // namespace hw9
