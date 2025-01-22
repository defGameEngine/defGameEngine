#pragma warning(disable : 4996)

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

// Oh, dear stb_image...
#define SAFE_STBI_FAILURE_REASON() (stbi_failure_reason() ? stbi_failure_reason() : "")
