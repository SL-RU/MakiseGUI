//Examples of mutex related functions you need to implement for your system.

#if MAKISE_MUTEX
uint8_t m_mutex_create (MAKISE_MUTEX_t *sobj)
{
    /* CMSIS-RTOS */
    //*sobj = osMutexCreate(Mutex + vol);
    //return (int)(*sobj != NULL);

    /* uC/OS-II */
    //OS_ERR err;
    //*sobj = OSMutexCreate(0, &err);
    //return (int)(err == OS_NO_ERR);
    
    /* FreeRTOS */
    *sobj = xSemaphoreCreateMutex();
    xSemaphoreGive(*sobj);
    return (int)(*sobj != NULL);
}
//delete mutex
uint8_t m_mutex_delete (MAKISE_MUTEX_t *sobj)
{
    /* Win32 */
    //return (int)CloseHandle(*sobj);

    /* CMSIS-RTOS */
    //return (int)(osMutexDelete(*sobj) == osOK);
    
    /* Freertos */
    vSemaphoreDelete(*sobj);
    return 1;
}
//Request Grant to Access some object
uint8_t m_mutex_request_grant (MAKISE_MUTEX_t *sobj)
{
    /* Win32 */
    //return (int)(WaitForSingleObject(*sobj, MAKISE_MUTEX_TIMEOUT) == WAIT_OBJECT_0);

    /* FreeRTOS */
    return (int)(xSemaphoreTake(*sobj, MAKISE_MUTEX_TIMEOUT) == pdTRUE);
}
//Release Grant to Access some object
uint8_t m_mutex_release_grant (MAKISE_MUTEX_t *sobj)
{
    /* Win32 */
    //ReleaseMutex(*sobj);

    /* FreeRTOS */
    xSemaphoreGive(*sobj);
    return 1;
}
#endif

