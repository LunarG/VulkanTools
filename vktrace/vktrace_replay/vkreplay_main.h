/**************************************************************************
 *
 * Copyright 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Jon Ashburn <jon@lunarg.com>
 * Author: Tony Barbour <tony@lunarg.com>
 **************************************************************************/
#ifndef VKREPLAY__MAIN_H
#define VKREPLAY__MAIN_H

typedef struct vkreplayer_settings
{
    char* pTraceFilePath;
    unsigned int numLoops;
    int loopStartFrame;
    int loopEndFrame;
    bool skipGetQueryPoolResults;
    const char* screenshotList;
    const char* verbosity;
} vkreplayer_settings;

#endif // VKREPLAY__MAIN_H
