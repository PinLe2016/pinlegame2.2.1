//
//  config.h
//  PinLe
//
//  Created by Kennedy on 15/6/23.
//
//

#ifndef PinLe_BroadcastData_config_h
#define PinLe_BroadcastData_config_h



//==================================[module define]============================
#define M_BROADCAST                 "module_broadcastdata"
#define M_GET_BROADCAST             (IBroadcastData*)IDataManager::getInstance()->getData(M_BROADCAST)



//==================================[message define]============================
#define MES_BROADCAST_GETLIST                   "MES_BROADCAST_GETLIST"
#define MES_BROADCAST_POPITEM                   "MES_BROADCAST_POPITEM"
#define MES_BROADCAST_DISPLAYITEM               "MES_BROADCAST_DISPLAYITEM"
#define MES_BROADCAST_UPDATELIST                "MES_BROADCAST_UPDATELIST"

//=============================================================================
#define broadcast_file_key                      "broadcast_file_key"
#define broadcast_file_info                     "info"







#endif
