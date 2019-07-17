# Mutexes

Mutexes required for multithreaded systems. 

In MakiseGUI mutexes are optional.

# Setting

To enable mutexes in Makise GUI you need to define `#define MAKISE_MUTEX 1` in your `makise_config.h`. After that you need to include required headers and define type of your mutex `#define MAKISE_MUTEX_t xSemaphoreHandle` - this is example for FreeRTOS. Also you need to implement for your OS that functions:

```
//create mutex object
uint8_t m_mutex_create (MAKISE_MUTEX_t *sobj);

//delete mutex
uint8_t m_mutex_delete (MAKISE_MUTEX_t *sobj);

//Request Grant to Access some object
uint8_t m_mutex_request_grant (MAKISE_MUTEX_t *sobj);

//Release Grant to Access the Volume
uint8_t m_mutex_release_grant (MAKISE_MUTEX_t *sobj);
```

You can see examples of implementation of that functions for different OS in the examples/mutex.c

# INFO

Mutexes are used in all MakiseGUI functions where it is required. Mutexes is in structures: MHost, MContainer, MInputHostData and in the MElemement structure there are 2 mutexes(`mutex_cont` is for container related things and `mutex` is for `data` and everything else).

When you are using mutexes then you need control elements by functions or request and release mutexes by yourself in your code. To request grant you need to call `MAKISE_MUTEX_REQUEST(&el->mutex);` and to release `MAKISE_MUTEX_RELEASE(&el->mutex);`.

In element's functions: element->draw, element->update, element->predraw, element->input, element->focus mutex access already granted - you don't need to call request it again.
