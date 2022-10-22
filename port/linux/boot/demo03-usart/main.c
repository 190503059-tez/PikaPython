/* this demo shows the usage of string arg in method */

#include "BaseObj.h"
#include <stdio.h>

void sendMethod(PikaObj *self, Args *args)
{
    char *data = args_getStr(args, "data");
    /* send to com1 */
    printf("[com1]: %s\r\n", data);
}

PikaObj *New_USART(Args *args)
{
    /*  Derive from the tiny object class.
        Tiny object can not import sub object.
        Tiny object is the smallest object. */
    PikaObj *self = New_TinyObj(args);

    /* bind the method */
    class_defineMethod(self, "send(data:str)", sendMethod);

    /* return the object */
    return self;
}

PikaObj *New_MYROOT(Args *args)
{
    /*  Derive from the base object class .
        BaseObj is the smallest object that can
        import sub object.      */
    PikaObj *self = New_BaseObj(args);

    /* new led object bellow root object */
    obj_newObj(self, "usart", "USART", New_USART);

    /* return the object */
    return self;
}

int32_t main()
{
    /* new root object */
    PikaObj *root = newRootObj("root", New_MYROOT);
    /* user input buff */
    char inputBuff[256] = {0};
    /* run the script with check*/
    obj_run(root, "res = usart.send('hello world')");

    printf("memory used max = %0.2f kB\r\n", pikaMemMax() / 1024.0);
    printf("memory used now = %0.2f kB\r\n", pikaMemNow() / 1024.0);
    while (1)
    {
        /* get user input */
        fgets(inputBuff, sizeof(inputBuff), stdin);

        /* run PikaScript and get res */
        PikaObj *globals = obj_run(root, inputBuff);

        /* get system output of PikaScript*/
        char *sysOut = args_getSysOut(globals->list);;

        if (NULL != sysOut)
        {
            /* print32_t out the system output */
            printf("%s\r\n", sysOut);
        }

        /* deinit the res */
        // obj_deinit(globals);
    }
}
