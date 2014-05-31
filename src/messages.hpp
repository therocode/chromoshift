#pragma once
#include <fea/messaging.hpp>
#include "direction.hpp"

FEA_DECLARE_MESSAGE(QuitMessage);
FEA_DECLARE_MESSAGE(MoveMessage, Direction);
