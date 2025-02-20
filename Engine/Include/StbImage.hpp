#pragma once

#ifndef STB_IMAGE_HPP
#define STB_IMAGE_HPP

#pragma warning(disable : 4996)

#include "stb_image.h"
#include "stb_image_write.h"

// Oh, dear stb_image...
#define SAFE_STBI_FAILURE_REASON() (stbi_failure_reason() ? stbi_failure_reason() : "")

#endif
