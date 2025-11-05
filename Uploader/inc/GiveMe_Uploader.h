#/**
 * @file GiveMe_Uploader.h
 * @brief Factory that constructs a concrete Uploader instance.
 *
 * Helper that returns a configured Uploader pointer based on an integer
 * selector. The caller receives ownership of the returned pointer.
 */
#include "Misc.h"
#include "Uploader.h"

Uploader *GiveMe_Uploader(const int &intUploader,
                          const std::string &SampleFile);
