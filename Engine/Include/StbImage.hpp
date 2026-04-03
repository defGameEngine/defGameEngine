/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#pragma once

#ifndef STB_IMAGE_HPP
#define STB_IMAGE_HPP

#pragma warning(disable : 4996)

#include "stb_image.h"
#include "stb_image_write.h"

// If there is no failure reason then stbi_failure_reason() == nullptr,
// in order to fix that we will use (const char*)""
#define SAFE_STBI_FAILURE_REASON (stbi_failure_reason() ? stbi_failure_reason() : "")

#endif
