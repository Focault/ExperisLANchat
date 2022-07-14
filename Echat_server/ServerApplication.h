#ifndef __SERVERAPPLICATION_H__
#define __SERVERAPPLICATION_H__

typedef enum ServerAppResult
{
    SERVER_APP_SUCCESS,
    SERVER_APP_UNINITIALIZED,
    SERVER_APP_ALLOCATION_FAIL
} ServerAppResult;

typedef struct ServerApp ServerApp;

/**
 * @brief Initialize Server App
 * 
 * @return ServerApp* (or Null if server initialization or allocation failed)
 */
ServerApp* InitializeServerApp();

/**
 * @brief Run the Server App
 * 
 * @param _serverApp - pointer to Server App struct
 * @return ServerAppResult 
 * @retval SERVER_APP_SUCCESS - Got a signal to exit server
 * @retval SERVER_APP_UNINITIALIZED - pointer to Server App struct uninitialized
 * @retval SERVER_APP_ALLOCATION_FAIL - Server's buffer allocation failed
 */
ServerAppResult RunServerApp(ServerApp *_serverApp);

/**
 * @brief Destroy the Server app
 * 
 * @param _serverApp - pointer to pointer to Server App struct
 */
void DestroyServerApp(ServerApp **_serverApp);

#endif /* __SERVERAPPLICATION_H__ */