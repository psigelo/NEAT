// This file is part of the REMOTE API
// 
// Copyright 2006-2014 Dr. Marc Andreas Freese. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// The REMOTE API is licensed under the terms of GNU GPL:
// 
// -------------------------------------------------------------------
// The REMOTE API is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// The REMOTE API is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with the REMOTE API.  If not, see <http://www.gnu.org/licenses/>.
// -------------------------------------------------------------------
//
// This file was automatically created for V-REP release V3.1.0 on January 20th 2014

#ifndef _EXTAPICUSTOM__
#define _EXTAPICUSTOM__

#include "extApiPlatform.h"

#ifdef _WIN32
	#define EXTAPI_DLLEXPORT extern __declspec(dllexport)
#endif
#if defined (__linux) || defined (__APPLE__)
	#define EXTAPI_DLLEXPORT extern
#endif

/* Your custom remote API functions. Following are examples: */
EXTAPI_DLLEXPORT simxInt simxCustomGetObjectCount(simxInt clientID,simxInt* objectCount,simxInt operationMode); /* related to command ID: simx_customcmd_get_object_count */
EXTAPI_DLLEXPORT simxInt simxCustomGetObjectType(simxInt clientID,simxInt objectHandle,simxInt* objectType,simxInt operationMode); /* related to command ID: simx_customcmd_get_object_type */
EXTAPI_DLLEXPORT simxInt simxCustomSetObjectName(simxInt clientID,simxInt objectHandle,const simxChar* objectName,simxInt operationMode); /* related to command ID: simx_customcmd_set_object_name */
EXTAPI_DLLEXPORT simxInt simxCustomGetUIButtonLabel(simxInt clientID,simxInt uiHandle,simxInt buttonID,simxChar** label,simxInt operationMode); /* related to command ID: simx_customcmd_get_ui_button_label */
EXTAPI_DLLEXPORT simxInt simxCustomGetScriptHandle(simxInt clientID,const simxChar* objectName,simxInt* scriptHandle,simxInt operationMode); /* related to command ID: simx_customcmd_get_script_handle */

#endif /* _EXTAPICUSTOM__ */		
