#/***************************************************************************
 * File: GiveMe_Uploader.h
 * Project: dEdxRecon
 *
 * Brief: Factory helper that constructs a concrete Uploader instance based
 *        on an integer selector. Creates necessary model objects and
 *        returns a configured Uploader pointer.
 *
 * Contents: GiveMe_Uploader factory declaration.
 *
 * Notes: Caller takes ownership of the returned Uploader pointer; the
 *        created model objects currently leak for lifetime reasons.
 ***************************************************************************/

#include "Misc.h"
#include "Uploader.h"

Uploader *GiveMe_Uploader(const int &intUploader,
                          const std::string &SampleFile);
