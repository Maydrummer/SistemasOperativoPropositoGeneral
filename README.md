# SistemasOperativoPropositoGeneral

## Clase 3

### Signals (Senales)

Los procesos corren de manera aislada entre si, para intercambiar informacion entre si, se utilizan las signals.

Las signals son un mecanismo simple que nos permite transmitir una senal que es un numero.

Los procesos pueden estar programados para hacer lo que se requiera al recibir estos numeros.

Cuando se recibe un signal, se interrumple el codigo y se ejecutara el Handler del signal, analogo al concepto de interrupciones.

Las senales que se definen en el estandar POSIX se muestran a continuacion: 

![ Signals ](./figures/signals.png)

Las primeras 32 senales tienen su respectivo nombre y no son de proposito general. No todos los handlers de las senales se pueden reescribir su comportamiento. Por ejemplo, cuando un proceso recibe la senal 9, el proceso se va a terminar, no es posible modificar el handler para que tenga otro comportamiento. 

En el caso de la senal 15, que tambien termina un proceso, si es posible reescribir su handler para que el proceso haga algo antes de que termine.

### Signals RT

Hay otros tipos de senales que se las llaman REAL TIME, todas son de proposito general, a partir de la 34 en adelante. No tienen un nombre en particular.

Estas signals se encolan y se entregan en el orden que llegaron.


Las primeras 32 senales no se encolan, ejemplo:
- Si se recibe la signal 12, se interrumpe el programa, se ejecuta el respectivo handler, y si vuelve a llegar otra signal 12, esta signal va a quedar pendiente, y cuando se termine la funcion del handler, se vovera a ingresar nuevamente a la funcion del handler. Pero si llega 20 veces la signal 12, cuando salga del handler, no se va a llamar 20 veces el handler. Es como si la cola solo tiene espacio para un numero.

### Envio de signal a un proceso

En la terminal se utiliza el siguiente comando, donde
- sn: Es el numero del signal
- pid: Es el ID del proceso al que se le envia la signal

```
kill -sn pid
```