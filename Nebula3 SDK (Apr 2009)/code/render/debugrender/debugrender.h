#pragma once
#ifndef DEBUG_DEBUGRENDER_H
#define DEBUG_DEBUGRENDER_H
//------------------------------------------------------------------------------
/**
    Macros for debug rendering.

    (C) 2008 Radon Labs GmbH
*/

// debug text render macro
#include "debugrender/debugrenderprotocol.h"
#include "graphics/graphicsinterface.h"
#include "debugrender/debugtextrenderer.h"

#define _debug_text(txt, pos, color) { Debug::DebugTextRenderer::Instance()->DrawText(txt, color, pos); }
#define _debug_text3D(txt, pos, color) { Debug::DebugTextRenderer::Instance()->DrawText3D(txt, color, pos); }
#endif
