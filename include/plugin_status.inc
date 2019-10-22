#if defined _plugin_status_include
	#endinput
#endif

#define _plugin_status_included

enum PluginState
{
    PL_STATE_NOT_EXIST = 0,
    PL_STATE_RUNNING,
    PL_STATE_DEBUG,
    PL_STATE_PAUSED,
    PL_STATE_STOPPED,
    PL_STATE_ERROR,
    PL_STATE_BAD_LOAD
}

stock bool:is_plugin_running(const szPluginName[], const bool:bUseFilename = false)
{
    new PluginState:iPlState = get_plugin_state(szPluginName, bUseFilename)
    return bool:(iPlState == PL_STATE_RUNNING || iPlState == PL_STATE_DEBUG)
}

stock PluginState:get_plugin_state(const szPluginName[], const bool:bUseFilename = false)
{
    new szStatus[2], iPluginID

    if((iPluginID = is_plugin_loaded(szPluginName, bUseFilename)) == INVALID_PLUGIN_ID) {
        return PL_STATE_NOT_EXIST
    }

    if(get_plugin(iPluginID, .status = szStatus, .len5 = charsmax(szStatus)) != INVALID_PLUGIN_ID)
    {
        switch(szStatus[0])
        {
            case 'r': return PL_STATE_RUNNING     	// "running"
            case 'd': return PL_STATE_DEBUG        	// "debug"
            case 'p': return PL_STATE_PAUSED    	  // "paused"
            case 's': return PL_STATE_STOPPED    	  // "stopped"
            case 'e': return PL_STATE_ERROR        	// "error"
            case 'b': return PL_STATE_BAD_LOAD    	// "bad load"
        }
    }
    
    return PL_STATE_NOT_EXIST
}
