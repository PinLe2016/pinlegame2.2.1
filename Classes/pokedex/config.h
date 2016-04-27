//
//  config.h
//  PinLe
//
//  Created by Kennedy on 15/7/1.
//
//

#ifndef PinLe_PokedexData_config_h
#define PinLe_PokedexData_config_h

//FVNUH5LNE3

//==================================[module define]============================
#define M_POKEDEX                   "module_pokedexdata"
#define M_GET_POKEDEX               (IPokedexData*)IDataManager::getInstance()->getData(M_POKEDEX)



//==================================[message define]============================

#define MES_POKEDEX_GOTOPOKEDEX     "MES_POKEDEX_GOTOPOKEDEX"

//=============================================================================
#define     POKEDEX_MAX                 100                     //图鉴存储量
#define     pokedex_file_info           "info"
#define     pokedex_file_key            "pokedex_file_key"




#endif
